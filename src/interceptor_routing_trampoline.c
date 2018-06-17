#include <stdlib.h>

#include "trampoline.h"

void trampoline_build_all(struct _InterceptorBackend *self, hook_entry_t *entry) {
    if (entry->type == HOOK_TYPE_FUNCTION_via_PRE_POST) {
        trampoline_prepare(self, entry);
        trampoline_build_for_enter(self, entry);
        trampoline_build_for_invoke(self, entry);
        trampoline_build_for_leave(self, entry);
    } else if (entry->type == HOOK_TYPE_FUNCTION_via_REPLACE) {
        trampoline_prepare(self, entry);
        trampoline_build_for_enter_transfer(self, entry);
        trampoline_build_for_invoke(self, entry);
    } else if (entry->type == HOOK_TYPE_FUNCTION_via_GOT) {
        // trampoline_prepare(self, entry);
        trampoline_build_for_enter(self, entry);
        trampoline_build_for_leave(self, entry);
    } else if (entry->type == HOOK_TYPE_DBI) {
        trampoline_prepare(self, entry);
        trampoline_build_for_dynamic_binary_instrumentation(self, entry);
        trampoline_build_for_invoke(self, entry);
    }
    return;
}
