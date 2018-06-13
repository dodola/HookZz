//
// Created by z on 2018/6/14.
//

#include "MemoryManager.h"


Interceptor* Interceptor::sharedInstance() {
    if(!interceptor)
        interceptor = new Interceptor();
    return interceptor;
}