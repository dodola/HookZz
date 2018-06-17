//
// Created by jmpews on 2018/6/15.
//

#include "InterceptorBackend.h"

void InterceptorBackend::BuildAllTrampoline(HookEntry *entry) {
    if (entry->hook_type == HOOK_TYPE_FUNCTION_via_PRE_POST) {
        PrepareTrampoline(entry);
        BuildForEnterTrampoline(entry);
        BuildForInvokeTrampoline(entry);
        BuildForLeaveTrampoline(entry);
    } else if (entry->hook_type == HOOK_TYPE_FUNCTION_via_REPLACE) {
        PrepareTrampoline(entry);
        BuildForEnterTransferTrampoline(entry);
        BuildForInvokeTrampoline(entry);
    } else if (entry->hook_type == HOOK_TYPE_FUNCTION_via_GOT) {
        BuildForEnterTrampoline(entry);
        BuildForLeaveTrampoline(entry);
    } else if (entry->hook_type == HOOK_TYPE_DBI) {
        PrepareTrampoline(entry);
        BuildForDynamicBinaryInstrumentationTrampoline(entry);
        BuildForInvokeTrampoline(entry);
    }
}