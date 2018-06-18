#ifndef interceptor_trampoline_cclass(h
#define interceptor_trampoline_cclass(h

#include "core.h"
#include "hookzz.h"
#include "interceptor.h"

#define interceptor_trampoline_cclass(member) cclass(interceptor_trampoline, member)

void interceptor_trampoline_cclass(prepare)(hook_entry_t *entry);

void interceptor_trampoline_cclass(free)(hook_entry_t *entry);

void interceptor_trampoline_cclass(build_all)(hook_entry_t *entry);

ARCH_API void interceptor_trampoline_cclass(active)(hook_entry_t *entry);

ARCH_API void interceptor_trampoline_cclass(build_for_enter)(hook_entry_t *entry);

ARCH_API void interceptor_trampoline_cclass(build_for_enter_transfer)(struct _InterceptorBackend *self,
                                                                      hook_entry_t *entry);

ARCH_API void interceptor_trampoline_cclass(build_for_invoke)(hook_entry_t *entry);

ARCH_API void interceptor_trampoline_cclass(build_for_leave)(hook_entry_t *entry);

ARCH_API void interceptor_trampoline_cclass(build_for_dynamic_binary_instrumentation)(struct _InterceptorBackend *self,
                                                                                      hook_entry_t *entry);

#endif