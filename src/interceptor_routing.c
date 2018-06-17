#include "interceptor_routing.h"
#include "hookzz.h"


void interceptor_routing_begin(RegState *rs, HookEntry *entry, void *nextHop, void *retAddr) {
    // DEBUG_LOG("target %p call begin-invocation", entry->target_ptr);

    // For iOS Easy Debug Breakpoint
    // if (!strcmp((char *)(rs->general.regs.x1), "_beginBackgroundTaskWithName:expirationHandler:")) {
    // }

    ThreadStackManager *threadStack = ThreadStackManager::initializeFromThreadLocalKey(entry->thread_local_key);
    if (!threadStack) {
        threadStack = new ThreadStackManager(entry->thread_local_key);
    }
    CallStackManager *callStack = new CallStackManager();
    threadStack->pushCallStack(callStack);

    // call pre_call
    if (entry->pre_call) {
        PRECALL pre_call;
        HookEntryInfo entryInfo;
        entryInfo.hook_id      = entry->id;
        entryInfo.hook_address = entry->target_address;
        pre_call               = static_cast<PRECALL>(entry->pre_call);
        (*pre_call)(rs, (ThreadStackPublic *)threadStack, (CallStackPublic *)callStack, &entryInfo);
    }

    // set next hop
    if (entry->replace_call) {
        *(zz_ptr_t *)nextHop = entry->replace_call;
    } else {
        *(zz_ptr_t *)nextHop = entry->on_invoke_trampoline;
    }

    if (entry->hook_type == HOOK_TYPE_FUNCTION_via_PRE_POST || entry->hook_type == HOOK_TYPE_FUNCTION_via_GOT) {
        callStack->retAddr   = *(zz_ptr_t *)retAddr;
        *(zz_ptr_t *)retAddr = entry->on_leave_trampoline;
    }
}

void interceptor_routing_end(RegState *rs, HookEntry *entry, void *nextHop) {
    // DEBUG_LOG("%p call end-invocation", entry->target_ptr);

    ThreadStackManager *threadStack = ThreadStackManager::initializeFromThreadLocalKey(entry->thread_local_key);
    if (!threadStack) {
        // LOG-NEED
        exit(-1);
    }
    CallStackManager *callStack = threadStack->popCallStack();

    // call post_call
    if (entry->post_call) {
        POSTCALL post_call;
        HookEntryInfo entryInfo;
        entryInfo.hook_id      = entry->id;
        entryInfo.hook_address = entry->target_address;
        post_call              = static_cast<POSTCALL>(entry->post_call);
        (*post_call)(rs, (ThreadStackPublic *)threadStack, (CallStackPublic *)callStack,
                     (const HookEntryInfo *)&entryInfo);
    }

    // set next hop
    *(zz_ptr_t *)nextHop = callStack->retAddr;
}

void interceptor_routing_dynamic_binary_instrumentation(RegState *rs, HookEntry *entry, void *nextHop) {
    // DEBUG_LOG("target %p call dynamic-binary-instrumentation-invocation", entry->target_ptr);

    if (entry->stub_call) {
        STUBCALL stub_call;
        HookEntryInfo entryInfo;
        entryInfo.hook_id      = entry->id;
        entryInfo.hook_address = entry->target_address;
        stub_call              = static_cast<STUBCALL>(entry->stub_call);
        (*stub_call)(rs, (const HookEntryInfo *)&entryInfo);
    }

    *(zz_ptr_t *)nextHop = entry->on_invoke_trampoline;
}

void interceptor_routing_begin_bridge_handler(reg_state_t *rs, ClosureBridgeData *cbd) {
    hook_entry_t *entry = cbd->user_data;
    void *next_hop_PTR  = get_next_hop_addr_PTR(rs);
    void *ret_PTR       = get_ret_addr_PTR(rs);
    interceptor_routing_begin(rs, entry, next_hop_PTR, ret_PTR);
    return;
}

void interceptor_routing_end_bridge_handler(reg_state_t *rs, ClosureBridgeData *cbd) {
    hook_entry_t *entry = cbd->user_data;
    void *next_hop_PTR  = get_next_hop_addr_PTR(rs);
    interceptor_routing_end(rs, entry, next_hop_PTR);
    return;
}

void interceptor_routing_dynamic_binary_instrumentation_bridge_handler(reg_state_t *rs, ClosureBridgeData *cbd) {
    hook_entry_t *entry = cbd->user_data;
    void *next_hop_PTR  = get_next_hop_addr_PTR(rs);
    interceptor_routing_dynamic_binary_instrumentation(rs, entry, next_hop_PTR);
    return;
}

void interceptor_routing_common_bridge_handler(reg_state_t *rs, ClosureBridgeData *cbd) {
    USER_CODE_CALL userCodeCall = cbd->user_code;
    userCodeCall(rs, cbd);
    return;
}

void dynamic_context_begin_invocation_bridge_handler(reg_state_t *rs, DynamicClosureTrampoline *cbd) {
    hook_entry_t *entry = cbd->user_data;
    void *next_hop_PTR  = get_next_hop_addr_PTR(rs);
    void *ret_PTR       = get_ret_addr_PTR(rs);
    context_begin_invocation(rs, entry, next_hop_PTR, ret_PTR);
    return;
}

void dynamic_context_end_invocation_bridge_handler(reg_state_t *rs, DynamicClosureTrampoline *cbd) {
    hook_entry_t *entry = cbd->user_data;
    void *next_hop_PTR  = get_next_hop_addr_PTR(rs);
    context_end_invocation(rs, entry, next_hop_PTR);
    return;
}

void dynamic_common_bridge_handler(reg_state_t *rs, DynamicClosureTrampoline *cbd) {

    DYNAMIC_USER_CODE_CALL userCodeCall = cbd->user_code;
    userCodeCall(rs, cbd);
    return;
}