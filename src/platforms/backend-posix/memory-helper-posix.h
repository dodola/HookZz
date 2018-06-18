#ifndef platforms_backend_darwin_memory_helper_linux_h
#define platforms_backend_darwin_memory_helper_linux_h

#define posix_memory_helper_cclass(member) cclass(posix_memory_helper, member)

void posix_memory_helper_cclass(patch_code)(void *dest, void *src, int count);

#endif
