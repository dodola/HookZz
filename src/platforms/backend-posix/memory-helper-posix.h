#ifndef platforms_backend_darwin_memory_helper_linux_h
#define platforms_backend_darwin_memory_helper_linux_h

#include "memory-manager-posix.h"
#include "std_kit/std_kit.h"
#include <sys/mman.h>
#include <unistd.h>

#define posix_memory_helper_cclass(member) cclass(posix_memory_helper, member)

int posix_memory_helper_cclass(get_page_size)();

void *posix_memory_helper_cclass(allocate_page)(int prot, int n);

void posix_memory_helper_cclass(patch_code)(void *dest, void *src, int count);

void posix_memory_helper_cclass(set_page_memory_permission)(void *address, int prot);

#endif
