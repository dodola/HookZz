//
// Created by z on 2018/6/14.
//

#ifndef HOOKZZ_MEMORYMANAGER_H
#define HOOKZZ_MEMORYMANAGER_H

#include <stdint.h>

#include <iostream>
#include <vector>

#include "hookzz.h"

typedef struct _CodeSlice {
    bool isUsed;
    bool isCodeCave;
    void data;
    int size;
} CodeSlice;

typedef struct _MemoryBlock {
    bool isCodeCave;
    zz_addr_t start_address;
    zz_addr_t current_address;
    int total_size;
    int used_size;
} MemoryBlock;

class MemoryManager {
public:
    std::vector<MemoryBlock *> memory_blocks;
private:
    static MemoryManager

public:
    static Interceptor* sharedInstance();
    HookEntry *findHookEntry(void *target_address);
    void addHookEntry(HookEntry *hook_entry);

private:
    Interceptor(){}
    ~Interceptor(){}
};

#endif //HOOKZZ_MEMORYMANAGER_H
