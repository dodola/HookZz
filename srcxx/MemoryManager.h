//
// Created by z on 2018/6/14.
//

#ifndef HOOKZZ_MEMORYMANAGER_H
#define HOOKZZ_MEMORYMANAGER_H

#include <stdint.h>

#include <iostream>
#include <vector>

#include "CommonClass/DesignPattern/Singleton.h"

#include "hookzz.h"

typedef enum _MemoryAttribute { MEM_RX } MemoryAttribute;

typedef struct _CodeSlice {
    void *data;
    int size;
} CodeSlice;

typedef struct _CodeCave {
    int size;
    void *backup;
    zz_addr_t address;
} CodeCave;

typedef struct _MemoryBlock {
    int prot; // memory permission
    int size;
    zz_addr_t address;
} MemoryBlock;

typedef struct _FreeMemoryBlock {
    MemoryAttribute prot; // memory permission
    int total_size;
    int used_size;
    zz_addr_t address;
} FreeMemoryBlock;

class MemoryManager : public Singleton<MemoryManager> {
  public:
    bool is_support_rx_memory;
    std::vector<CodeCave *> code_caves;
    std::vector<MemoryBlock *> process_memory_layout;
    std::vector<FreeMemoryBlock *> free_memory_blocks;

  public:
    static bool isSupportRXMemory();

    static int PageSize();

    static void *allocateMemoryPage(MemoryAttribute prot, int n);

    static void CodePatch(void *dest, void *src, int count);

    CodeSlice *allocateCodeSlice(int need_size);

    void getProcessMemoryLayout();

    CodeCave *searchNearCodeCave(void *address, int range, int need_size);
};

#endif //HOOKZZ_MEMORYMANAGER_H
