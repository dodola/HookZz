//
// Created by jmpews on 2018/6/14.
//

#ifndef HOOKZZ_INVOCATION_H
#define HOOKZZ_INVOCATION_H

#include "Interceptor.h"
#include "hookzz.h"

void context_begin_invocation(RegState *rs, HookEntry *entry, void *nextHop, void *retAddr);

void context_end_invocation(RegState *rs, HookEntry *entry, void *nextHop);

void dynamic_binary_instrumentation_invocation(RegState *rs, HookEntry *entry, void *nextHop);

#endif //HOOKZZ_INVOCATION_H
