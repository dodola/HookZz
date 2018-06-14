//
// Created by jmpews on 2018/6/14.
//

#ifndef HOOKZZ_CLOSUREBRIDGE_H
#define HOOKZZ_CLOSUREBRIDGE_H

#include <stdint.h>
#include <vector>
#include <libssh2_publickey.h>

#define PRIAVE

#include "hookzz.h"

#include "CommonClass/DesignPattern/Singleton.h"


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

class ClosureBridge {
public:
  std::vector<ClosureBridgeInfo *> bridge_infos;
  std::vector<ClosureBridgeTrampolineTable *> trampoline_tables;
public:
  ClosureBridge(void);
  ClosureBridgeInfo *allocateClosureBridge(void *user_data, void *user_code);
  ClosureBridgeTrampolineTable *allocateClosureBridgeTrampolineTable();
};

typedef struct _DynamicClosureBridgeInfo{
  void *trampolineTo PRIAVE;

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

class DynamicClosureBridge : public Singleton {
public:
  std::vector<DynamicClosureBridgeInfo *> bridge_infos;
  std::vector<DynamicClosureBridgeTrampolineTable *> trampoline_tables;
public:
  DynamicClosureBridge(void);

  ClosureBridgeInfo *allocateDynamicClosurceBridge(void *user_data, void *user_code);
  DynamicClosureBridgeTrampolineTable *addDynamicClosurceBridgeTrampolineTable();
};

typedef void (*USER_CODE_CALL)(RegState *rs, ClosureBridgeInfo *cbi);
typedef void (*DYNAMIC_USER_CODE_CALL)(RegState *rs, DynamicClosureBridgeInfo *cbi);

void closure_bridge_trampoline_template();

void closure_bridge_template();

void dynamic_closure_bridge_template();

void dynamic_closure_trampoline_table_page();


#endif //HOOKZZ_CLOSUREBRIDGE_H
