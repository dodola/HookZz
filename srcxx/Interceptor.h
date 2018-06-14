//
// Created by z on 2018/6/14.
//

#ifndef HOOKZZ_INTERCEPTOR_H
#define HOOKZZ_INTERCEPTOR_H

#include "MemoryManager.h"
#include "ThreadManager.h"
#include "hookzz.h"
#include <iostream>
#include <vector>

class Interceptor;
class HookEntryBackend;

typedef struct _HookEntry {
    zz_ptr_t target_address;

    HookType hook_type;

    unsigned long id;

    bool isEnabled;

    bool isNearJump;

    ThreadLocalKey *thread_local_key;

    zz_ptr_t pre_call;
    zz_ptr_t post_call;
    zz_ptr_t stub_call;
    zz_ptr_t replace_call;

    zz_ptr_t on_enter_transfer_trampoline;
    zz_ptr_t on_enter_trampoline;
    zz_ptr_t on_invoke_trampoline;
    zz_ptr_t on_leave_trampoline;
    zz_ptr_t on_dynamic_binary_instrumentation_trampoline;

    struct _HookEntryBackend *backend;
    // struct _Interceptor *interceptor;
} HookEntry;

class _InterceptorBackend;

class Interceptor {
  public:
    bool isSupportRXMemory;
    std::vector<HookEntry *> hook_entries;

  private:
    struct _InterceptorBackend *backend;
    MemoryManager *mm;
    static Interceptor *interceptor;

  public:
    static Interceptor *GETInstance();
    HookEntry *findHookEntry(void *target_address);
    void addHookEntry(HookEntry *hook_entry);

  private:
    Interceptor() {}
    ~Interceptor() {}
};

#endif //HOOKZZ_INTERCEPTOR_H
