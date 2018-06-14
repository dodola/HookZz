//
// Created by jmpews on 2018/6/14.
//

#include "InterceptorARM64.h"
#include <string.h>

#define ARM64_TINY_REDIRECT_SIZE 4
#define ARM64_FULL_REDIRECT_SIZE 16

void Interceptor::initializeBackend(MemoryManager *mm) {
    if(!mm->is_support_rx_memory) {
        // LOG-NEED
    }

    InterceptorBackend *backend = new(InterceptorBackend);
    backend->readerARM64 = new ARM64AssemblyReader(0, 0);
    backend->writerARM64 = new ARM64AssemblerWriter();
    backend->relocatorARM64 = new ARM64Relocator(backend->readerARM64, backend->writerARM64);
    backend->mm = mm;

    this->backend = static_cast<InterceptorBackend *>(this);
}

void InterceptorBackend::PrepareTrampoline(HookEntry *entry) {
    zz_addr_t target_address    = (zz_addr_t)entry->target_address;
    int redirect_limit = 0;
    ARM64HookEntryBackend *entry_backend = new(ARM64HookEntryBackend);

    entry->backend = (struct HookEntryBackend *)entry_backend;

    if (entry->isTryNearJump) {
        entry_backend->limit_relocate_inst_size = ARM64_TINY_REDIRECT_SIZE;
    } else {
        // check the first few instructions, preparatory work of instruction-fix
        relocatorARM64->tryRelocate(entry->target_address, ARM64_FULL_REDIRECT_SIZE, &redirect_limit);
        if (redirect_limit != 0 && redirect_limit > ARM64_TINY_REDIRECT_SIZE &&
            redirect_limit < ARM64_FULL_REDIRECT_SIZE) {
            entry->isNearJump              = true;
            entry_backend->limit_relocate_inst_size = ARM64_TINY_REDIRECT_SIZE;
        } else if (redirect_limit != 0 && redirect_limit < ARM64_TINY_REDIRECT_SIZE) {
            return;
        } else {
            entry_backend->limit_relocate_inst_size = ARM64_FULL_REDIRECT_SIZE;
        }
    }

    relocatorARM64->limitRelocateInstSize = entry_backend->limit_relocate_inst_size;


    // save original prologue
    memcpy(entry->origin_prologue.data, entry->target_address, entry_backend->limit_relocate_inst_size);
    entry->origin_prologue.size    = entry_backend->limit_relocate_inst_size;
    entry->origin_prologue.address = entry->target_address;

    // arm64_relocator initialize
    relocatorARM64->input->reset(entry->target_address, entry->target_address);
}