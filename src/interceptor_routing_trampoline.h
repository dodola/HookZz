#ifndef trampoline_h
#define trampoline_h

#include "core.h"
#include "hookzz.h"
#include "interceptor.h"

void trampoline_prepare(struct _InterceptorBackend *self, hook_entry_t *entry);

void trampoline_free(hook_entry_t *entry);

void trampoline_build_all(struct _InterceptorBackend *self, hook_entry_t *entry);

ARCH_API void trampoline_active(struct _InterceptorBackend *self, hook_entry_t *entry);

ARCH_API void trampoline_build_for_enter(struct _InterceptorBackend *self, hook_entry_t *entry);

ARCH_API void trampoline_build_for_enter_transfer(struct _InterceptorBackend *self, hook_entry_t *entry);

ARCH_API void trampoline_build_for_invoke(struct _InterceptorBackend *self, hook_entry_t *entry);

ARCH_API void trampoline_build_for_leave(struct _InterceptorBackend *self, hook_entry_t *entry);

ARCH_API void trampoline_build_for_dynamic_binary_instrumentation(struct _InterceptorBackend *self,
                                                                  hook_entry_t *entry);

#endif