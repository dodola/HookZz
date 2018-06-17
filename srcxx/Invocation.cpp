//
// Created by jmpews on 2018/6/14.
//

#include "Invocation.h"

#include "Interceptor.h"
#include "StackManager.h"
#include "hookzz.h"

void context_begin_invocation(RegState *rs, HookEntry *entry, void *nextHop, void *retAddr) {
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
        pre_call               = (PRECALL)(entry->pre_call);
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

void context_end_invocation(RegState *rs, HookEntry *entry, void *nextHop) {
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
        post_call              = (POSTCALL)(entry->post_call);
        (*post_call)(rs, (ThreadStackPublic *)threadStack, (CallStackPublic *)callStack,
                     (const HookEntryInfo *)&entryInfo);
    }

    // set next hop
    *(zz_ptr_t *)nextHop = callStack->retAddr;
}

void dynamic_binary_instrumentation_invocation(RegState *rs, HookEntry *entry, void *nextHop) {
    // DEBUG_LOG("target %p call dynamic-binary-instrumentation-invocation", entry->target_ptr);

    if (entry->stub_call) {
        STUBCALL stub_call;
        HookEntryInfo entryInfo;
        entryInfo.hook_id      = entry->id;
        entryInfo.hook_address = entry->target_address;
        stub_call              = (STUBCALL)(entry->stub_call);
        (*stub_call)(rs, (const HookEntryInfo *)&entryInfo);
    }

    *(zz_ptr_t *)nextHop = entry->on_invoke_trampoline;
}
