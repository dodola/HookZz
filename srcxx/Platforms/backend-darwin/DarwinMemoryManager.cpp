//
// Created by jmpews on 2018/6/14.
//

#include "DarwinMemoryManager.h"

#include <errno.h>
#include <mach-o/dyld.h>
#include <mach/mach.h>
#include <sys/mman.h>
#include <mach/vm_map.h>

#include "mach_vm.h"

inline void get_memory_info(void *address, vm_prot_t *prot, vm_inherit_t *inherit) {
    vm_address_t region  = (vm_address_t)address;
    vm_size_t region_size = 0;
    struct vm_region_submap_short_info_64 info;
    mach_msg_type_number_t info_count = VM_REGION_SUBMAP_SHORT_INFO_COUNT_64;
    natural_t max_depth               = 99999;
    kern_return_t kr;
    kr = vm_region_recurse_64(mach_task_self(), &region, &region_size, &max_depth, (vm_region_recurse_info_t)&info, &info_count);
    if (kr != KERN_SUCCESS) {
        return;
    }
    *prot    = info.protection;
    *inherit = info.inheritance;
}

inline void set_page_memory_permission(void *address, int prot) {
    kern_return_t kr;

    int pageSize = MemoryManager::PageSize();

    kr = mach_vm_protect(mach_task_self(), (vm_address_t)address, pageSize, FALSE, prot);
    if (kr != KERN_SUCCESS) {
        // LOG-NEED
        return FALSE;
    }
}

void MemoryManager::getProcessMemoryLayout() {

    mach_msg_type_number_t count;
    struct vm_region_submap_info_64 info;
    vm_size_t nesting_depth;

    kern_return_t kr         = KERN_SUCCESS;
    vm_address_t tmpAddr = 0;
    vm_size_t tmpSize       = 0;

    MemoryBlock *mb = new(MemoryBlock);
    process_memory_layout.push_back(mb);

    while (1) {
        count = VM_REGION_SUBMAP_INFO_COUNT_64;
        kr    = vm_region_recurse_64(mach_task_self(), &tmpAddr, &tmpSize, (natural_t *)&nesting_depth,
                                     (vm_region_info_64_t)&info, &count);
        if (kr == KERN_INVALID_ADDRESS) {
            break;
        } else if (kr) {
            mach_error("vm_region:", kr);
            break; /* last region done */
        }

        if (info.is_submap) {
            nesting_depth++;
        } else {
            tmpAddr += tmpSize;
            mb->address =tmpAddr - tmpSize;
            mb->size = tmpSize;
            mb->prot = info.protection;
        }
    }
}

/*
  REF:
  substitute/lib/darwin/execmem.c:execmem_foreign_write_with_pc_patch
  frida-gum-master/gum/gummemory.c:gum_memory_patch_code
  frida-gum-master/gum/backend-darwin/gummemory-darwin.c:gum_alloc_n_pages

  mach mmap use __vm_allocate and __vm_map
  https://github.com/bminor/glibc/blob/master/sysdeps/mach/hurd/mmap.c
  https://github.com/bminor/glibc/blob/master/sysdeps/mach/munmap.c

  http://shakthimaan.com/downloads/hurd/A.Programmers.Guide.to.the.Mach.System.Calls.pdf
*/

void MemoryManager::CodePatch(void *dest, void *src, int count) {

    vm_address_t destPage;
    vm_size_t offset;

    int pageSize = this->PageSize();

    // https://www.gnu.org/software/hurd/gnumach-doc/Memory-Attributes.html
    destPage = (zz_addr_t) dest & ~(pageSize - 1);
    offset = (zz_addr_t) dest - destPage;

    vm_prot_t prot;
    vm_inherit_t inherit;
    kern_return_t kr;
    mach_port_t task_self = mach_task_self();

    get_memory_info((void *) destPage, &prot, &inherit);

    // For another method, pelease read `REF`;
    // zz_ptr_t code_mmap = mmap(NULL, range_size, PROT_READ | PROT_WRITE,
    //                           MAP_ANON | MAP_SHARED, -1, 0);
    // if (code_mmap == MAP_FAILED) {
    //   return;
    // }

    void *copyPage = allocateMemoryPage(MEM_RX, 1);

    kr = vm_copy(task_self, destPage, pageSize, (vm_address_t) copyPage);
    if (kr != KERN_SUCCESS) {
        // LOG-NEED
        return;
    }
    memcpy(copyPage + offset, src, count);

    // SAME: mprotect(code_mmap, range_size, prot);
    set_page_memory_permission(copyPage, PROT_EXEC | PROT_READ);

    // TODO: need check `memory region` again.
    /*
        // if only this, `memory region` is `r-x`
        vm_protect((vm_map_t)mach_task_self(), 0x00000001816b2030, 16, FALSE, 0x13);
        // and with this, `memory region` is `rwx`
        *(char *)0x00000001816b01a8 = 'a';
    */

    mach_vm_address_t target_address = destPage;
    vm_prot_t cur_protectionc, max_protection;
    kr = mach_vm_remap(task_self, &target_address, pageSize, 0, VM_FLAGS_OVERWRITE, task_self, (mach_vm_address_t) copyPage,
        /*copy*/ TRUE, &cur_protectionc, &max_protection, inherit);

    if (kr != KERN_SUCCESS) {
        // LOG-NEED
        return FALSE;
    }
    // read `REF`
    // munmap(code_mmap, range_size);
    kr = mach_vm_deallocate(task_self, (mach_vm_address_t) copyPage, pageSize);
    if (kr != KERN_SUCCESS) {
        // LOG-NEED
        return FALSE;
    }
    return TRUE;

}