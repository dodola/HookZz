//
// Created by jmpews on 2018/6/15.
//

#ifndef HOOKZZ_INTERCEPTORBACKEND_H
#define HOOKZZ_INTERCEPTORBACKEND_H

#include "Interceptor.h"

class InterceptorBackend {
  public:
    virtual void PrepareTrampoline(HookEntry *entry){};

    virtual void BuildForEnterTransferTrampoline(HookEntry *entry){};

    virtual void BuildForEnterTrampoline(HookEntry *entry){};

    virtual void BuildForDynamicBinaryInstrumentationTrampoline(HookEntry *entry){};

    virtual void BuildForLeaveTrampoline(HookEntry *entry){};

    virtual void BuildForInvokeTrampoline(HookEntry *entry){};

    virtual void ActiveTrampoline(HookEntry *entry){};

    void BuildAllTrampoline(HookEntry *entry);
};
#endif //HOOKZZ_INTERCEPTORBACKEND_H
