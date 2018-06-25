
#include "thread_stack.h"
#include "core.h"
#include "thread_local_storage.h"

thread_stack_manager_t *thread_stack_cclass(shared_instance)() {
    thread_stack_manager_t *g_thread_stack_manager = (thread_stack_manager_t *)get_thread_variable_value();
    if (g_thread_stack_manager == NULL) {
        g_thread_stack_manager              = SAFE_MALLOC_TYPE(thread_stack_manager_t);
        g_thread_stack_manager->call_stacks = list_new();
        set_thread_variable_value((void *)g_thread_stack_manager);
    }

    return g_thread_stack_manager;
}
