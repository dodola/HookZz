#include <stdlib.h>
#include <string.h>

#include "interceptor.h"
#include "std_kit/std_kit.h"

interceptor_t g_interceptor = NULL;
interceptor_t *interceptor_cclass(shared_instance)(void) {
    if (g_interceptor == NULL) {
        g_interceptor = SAFE_MALLOC_TYPE(interceptor_t);
    }
    return g_interceptor;
}

hook_entry_t *interceptor_cclass(find_hook_entry)(interceptor_t *self) {
    if (!self)
        self = interceptor_cclass(shared_instance);

    list_iterator_t *it = list_iterator_new(self->hook_entries, LIST_HEAD);
    for (int i = 0; i < self->hook_entries->len; i++) {
        hook_entry_t *entry = (hook_entry_t *)list_at(self->hook_entries, i);
        if (entry->target_address == target_address) {
            return entry;
        }
    }
}

void *interceptor_cclass(add_hook_entry)(hook_entry_t *entry) {
    list_rpush(self->hook_entries, (list_node_t)entry);
    return;
}