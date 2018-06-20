#include "closure-bridge-posix.h"
#include "closure_bridge.h"

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define closure_bridge_trampoline_template_length (7 * 4)

ClosureBridge *gClosureBridge = NULL;
ClosureBridge *ClosureBridgeCClass(SharedInstance)() {
    if (gClosureBridge == NULL) {
        gClosureBridge = SAFE_MALLOC_TYPE(ClosureBridge);
    }
    return gClosureBridge;
}
ClosureBridgeInfo *ClosureBridgeCClass(AllocateClosureBridge)(ClosureBridge *self, void *user_data, void *user_code) {
    ClosureBridgeInfo *cb_info;
    ClosureBridgeTrampolineTable *table;
    long page_size = sysconf(_SC_PAGESIZE);

    list_iterator_t *it = list_iterator_new(self->trampoline_tables, LIST_HEAD);
    for (int i = 0; i < self->trampoline_tables->len; i++) {
        ClosureBridgeTrampolineTable *tmp_table =
            (ClosureBridgeTrampolineTable *)(list_at(self->trampoline_tables, i)->val);
        if (tmp_table->free_count > 0) {
            table = tmp_table;
        }
    }

    if (!table) {
        table = ClosureBridgeCClass(AllocateClosureBridgeTrampolineTable)(self);
    }

    uint16_t trampoline_used_count = table->used_count;
    void *redirect_trampoline =
        (void *)((intptr_t)table->trampoline_page + closure_bridge_trampoline_template_length * trampoline_used_count);

    cb_info                      = SAFE_MALLOC_TYPE(ClosureBridgeInfo);
    cb_info->user_code           = user_code;
    cb_info->user_data           = user_data;
    cb_info->redirect_trampoline = redirect_trampoline;

    // bind data to trampline
    void *tmp = (void *)((intptr_t)cb_info->redirect_trampoline + 4 * 3);
    memcpy(tmp, &cb_info, sizeof(ClosureBridgeInfo *));

    // set trampoline to bridge
    void *tmpX = (void *)closure_bridge_template;
    tmp        = (void *)((intptr_t)cb_info->redirect_trampoline + 4 * 5);
    memcpy(tmp, &tmpX, sizeof(void *));

    if (mprotect(table->trampoline_page, (size_t)page_size, (PROT_READ | PROT_EXEC))) {
        // LOG-NEED
        return NULL;
    }

    table->used_count++;
    table->free_count--;

    list_rpush(self->bridge_infos, list_node_new(cb_info));
    return cb_info;
}
ClosureBridgeTrampolineTable *ClosureBridgeCClass(AllocateClosureBridgeTrampolineTable)(ClosureBridge *self) {
    void *mmap_page;
    long page_size;
    page_size = sysconf(_SC_PAGESIZE);

    mmap_page = mmap(0, 1, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (mmap_page == MAP_FAILED) {
        // LOG-NEED
        return NULL;
    }

    if (mprotect(mmap_page, (size_t)page_size, (PROT_READ | PROT_WRITE))) {
        // LOG-NEED
        return NULL;
    }

    int t              = page_size / closure_bridge_trampoline_template_length;
    void *copy_address = mmap_page;
    for (int i = 0; i < t; ++i) {
        copy_address = (void *)((intptr_t)mmap_page + i * closure_bridge_trampoline_template_length);
        memcpy(copy_address, (void *)closure_bridge_trampoline_template, closure_bridge_trampoline_template_length);
    }

    if (mprotect(mmap_page, (size_t)page_size, (PROT_READ | PROT_EXEC))) {
        // LOG-NEED
        return NULL;
    }

    ClosureBridgeTrampolineTable *table = (ClosureBridgeTrampolineTable *)malloc(sizeof(ClosureBridgeTrampolineTable));
    table->entry                        = mmap_page;
    table->trampoline_page              = mmap_page;
    table->used_count                   = 0;
    table->free_count                   = (uint16_t)t;

    list_rpush(self->trampoline_tables, list_node_new(table));
    return table;
}