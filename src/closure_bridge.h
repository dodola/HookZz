#ifndef closure_bridge_h
#define closure_bridge_h

#include "core.h"
#include "std_kit/std_list.h"
#include <stdint.h>

#define PRIVATE

// closure bridge
typedef struct _ClosureBridgeInfo {
    void *user_code;
    void *user_data;
    void *redirect_trampoline;
} ClosureBridgeInfo;

typedef struct _ClosureBridgeTrampolineTable {
    void *entry;
    void *trampoline_page;
    uint16_t used_count;
    uint16_t free_count;
} ClosureBridgeTrampolineTable;

typedef struct _ClosureBridge {
    list_t *bridge_infos;
    list_t *trampoline_tables;
} ClosureBridge;

#define ClosureBridgeCClass(member) cclass(ClosureBridge, member)

ClosureBridge *ClosureBridgeCClass(SharedInstance)();
ClosureBridgeInfo *ClosureBridgeCClass(AllocateClosureBridge)(ClosureBridge *self, void *user_data, void *user_code);
ClosureBridgeTrampolineTable *ClosureBridgeCClass(AllocateClosureBridgeTrampolineTable)(ClosureBridge *self);

// dynamic closure bridge
typedef struct _DynamicClosureBridgeInfo {
    void *trampolineTo PRIVATE;

    void *user_code;
    void *user_data;
    void *redirect_trampoline;
} DynamicClosureBridgeInfo;

typedef struct _DynamicClosureTrampolineTable {
    void *entry;
    void *trampoline_page;
    void *data_page;
    uint16_t used_count;
    uint16_t free_count;
} DynamicClosureBridgeTrampolineTable;

typedef struct _DynamicClosureBridge {
    list_t *dynamic_bridge_infos;
    list_t *dynamic_trampoline_tables;
} DynamicClosureBridge;

#define DynamicClosureBridgeCClass(member) cclass(DynamicClosureBridge, member)

DynamicClosureBridge *DynamicClosureBridgeCClass(SharedInstance)();
DynamicClosureBridgeInfo *DynamicClosureBridgeCClass(AllocateDynamicClosureBridge)(DynamicClosureBridge *self,
                                                                                   void *user_data, void *user_code);
DynamicClosureBridgeTrampolineTable *
    DynamicClosureBridgeCClass(AllocateDynamicClosureBridgeTrampolineTable)(DynamicClosureBridge *self);

typedef void (*USER_CODE_CALL)(RegState *rs, ClosureBridgeInfo *cb_info);
typedef void (*DYNAMIC_USER_CODE_CALL)(RegState *rs, DynamicClosureBridgeInfo *dcb_info);

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void closure_bridge_trampoline_template();

void closure_bridge_template();

void dynamic_closure_bridge_template();

void dynamic_closure_trampoline_table_page();

#ifdef __cplusplus
}
#endif //__cplusplus

#endif