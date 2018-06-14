//
// Created by z on 2018/6/14.
//

#include "Interceptor.h"


Interceptor *Interceptor::GETInstance() {
    if(interceptor == NULL) {
        interceptor = new Interceptor();
        interceptor->mm = MemoryManager::GetInstance();
    }
    return interceptor;
}

HookEntry* Interceptor::findHookEntry(void *target_address) {
    for(auto entry : hook_entries) {
        if(entry->target_address == target_address) {
            return entry;
        }
    }
}


void Interceptor::addHookEntry(HookEntry *hook_entry) {
    hook_entries.push_back(hook_entry);
}

