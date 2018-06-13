//
// Created by z on 2018/6/14.
//

#include "Interceptor.h"

Interceptor* Interceptor::sharedInstance() {
    if(!interceptor)
        interceptor = new Interceptor();
    return interceptor;
}

HookEntry* Interceptor::findHookEntry(void *target_address) {

}