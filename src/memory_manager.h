#ifndef memory_allocator_h
#define memory_allocator_h

#include "core.h"
#include "hookzz.h"
#include "std_kit/std_list.h"

#include <stdint.h>

typedef struct _CodeSlice {
    void *data;
    int size;
} CodeSlice;

typedef struct _CodeCave {
    int size;
    void *backup;
    void *address;
} CodeCave;

typedef struct _MemoryBlock {
    int prot; // memory permission
    int size;
    void *address;
} MemoryBlock;

typedef struct _FreeMemoryBlock {
    int prot; // memory permission
    int total_size;
    int used_size;
    void *address;
} FreeMemoryBlock;

typedef struct _MemoryManager {
    bool is_support_rx_memory;
    list_t code_caves;
    list_t process_memory_layout;
    list_t free_memory_blocks;
} MemoryManager, mm;

#define memory_manger_class(member) cclass(memory_manager, member)

PLATFORM_API static bool memory_manger_class(is_support_allocate_rx_memory)(MemoryManager *self);

PLATFORM_API static int memory_manger_class(get_page_size)(MemoryManager *self);

PLATFORM_API void *memory_manger_class(allocate_page)(MemoryManager *self, int prot, int n);

PLATFORM_API void memory_manger_class(patch_code)(MemoryManager *self, void *dest, void *src, int count);

PLATFORM_API void memory_manger_class(get_process_memory_layout)(MemoryManager *self);

PLATFORM_API CodeCave *memory_manger_class(search_code_cave)(MemoryManager *self, void *address, int range, int size);

MemoryManager *memory_manger_class(shared_instance)();

CodeSlice *memory_manger_class(allocate_code_slice)(MemoryManager *self, int size);

#endif