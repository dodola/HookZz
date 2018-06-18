#include "memory_manager.h"
#include "std_kit/std_kit.h"
#include "std_kit/std_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MemoryManager *g_mm = NULL;

MemoryManager *memory_manger_class(shared_instance)() {
    if (g_mm == NULL) {
        g_mm = safe_malloc();
        CHECK(g_mm != NULL);
    }
    return g_mm;
}

void *memory_manger_class(allocate_code_slice)(MemoryManager *self, int size) {
    CodeSlice *cs       = NULL;
    list_iterator_t *it = list_iterator_new(self->free_memory_blocks, LIST_HEAD);
    for (int i = 0; i < self->free_memory_blocks->len; i++) {
        FreeMemoryBlock *fmb = (FreeMemoryBlock *)list_at(free_memory_blocks, i);
        if ((fmb->total_size - fmb->used_size) > size) {
            cs       = SAFE_MALLOC_TYPE(CodeSlice);
            cs->data = (void *)(fmb->address + fmb->used_size);
            cs->size = size;

            fmb->used_size += size;
            return cs;
        }
    }
}

inline void *search_dummy_code_cave(void *search_start, void *search_end, int size) {
    assert(search_start);
    assert(search_start < search_end);

    zz_addr_t cur_addr          = search_start;
    unsigned char dummy_0[1024] = {0};

    while (cur_addr < search_end) {
        if (!memcpy((void *)cur_addr, dummy_0, size)) {
            return (void *)cur_addr;
        }

        cur_addr += size;
    }
    return NULL;
}

CodeCave *memory_manger_class(search_code_cave)(void *address, int range, int size) {
    CodeCave *cc = NULL;
    zz_addr_t limit_start, limit_end;
    zz_addr_t search_start, search_end;

    limit_start = (zz_addr_t)address - range;
    limit_start = (zz_addr_t)address + range - size;
    for (auto mb : process_memory_layout) {
        search_start = mb->address > limit_start ? mb->address : limit_start;
        search_end   = (mb->address + mb->size) < limit_end ? (mb->address + mb->size) : limit_end;
        void *p      = search_dummy_code_cave(search_start, search_end, size);
        if (p) {
            cc          = SAFE_MALLOC_TYPE(CodeCave);
            cc->size    = size;
            cc->address = (zz_addr_t)p;
            return cc;
        }
    }
    return NULL;
}