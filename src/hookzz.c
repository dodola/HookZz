#include "hookzz.h"
#include "core.h"
#include "interceptor.h"
#include "interceptor_routing_trampoline.h"
#include "std_kit/std_kit.h"

ret_status_t ZzHook(void *target_address, void *replace_call, void **origin_call, PRECALL pre_call,
                    POSTCALL post_call) {
    HookType type;
    if (pre_call || post_call) {
        type = HOOK_TYPE_FUNCTION_via_PRE_POST;
    } else {
        type = HOOK_TYPE_FUNCTION_via_REPLACE;
    }

    hook_entry_t *entry   = SAFE_MALLOC_TYPE(hook_entry_t);
    entry->target_address = target_address;
    entry->replace_call   = replace_call;
    entry->pre_call       = pre_call;
    entry->post_call      = post_call;

    // interceptor_t *interceptor = interceptor_t::GETInstance();
    // interceptor->addHookEntry(entry);
    // interceptor->backend->BuildAllTrampoline(entry);
    // interceptor->backend->ActiveTrampoline(entry);
    return RS_SUCCESS;
}