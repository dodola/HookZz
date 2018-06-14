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

typedef struct _FunctionBackup {
    void  *address;
    int size;
    char data[32];
} FunctionBackup;

class Interceptor;

struct HookEntryBackend;

typedef struct _HookEntry {
    zz_ptr_t target_address;

    HookType hook_type;

    unsigned long id;

    bool isTryNearJump;

    bool isNearJump;

    bool isEnabled;

    ThreadLocalKey *thread_local_key;

    void * pre_call;
    void * post_call;
    void * stub_call;
    void * replace_call;

    void * on_enter_transfer_trampoline;
    void * on_enter_trampoline;
    void * on_invoke_trampoline;
    void * on_leave_trampoline;
    void * on_dynamic_binary_instrumentation_trampoline;

    FunctionBackup origin_prologue;

    struct HookEntryBackend *backend;

    class Interceptor *interceptor;
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

    void initializeBackend(MemoryManager *mm);


  private:
    Interceptor() {}
    ~Interceptor() {}
};

#endif //HOOKZZ_INTERCEPTOR_H
