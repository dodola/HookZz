#ifndef invocation_h
#define invocation_h

#include "hookzz.h"
#include "interceptor.h"

ARCH_API void *get_ret_addr_PTR(reg_state_t *rs);

ARCH_API void *get_next_hop_addr_PTR(reg_state_t *rs);

void interceptor_routing_begin(reg_state_t *rs, hook_entry_t *entry, void *nextHop, void *retAddr);

void interceptor_routing_begin_bridge_handler(reg_state_t *rs, hook_entry_t *entry, void *nextHop, void *retAddr);

void interceptor_routing_end(reg_state_t *rs, hook_entry_t *entry, void *nextHop);

void interceptor_routing_dynamic_binary_instrumentation(reg_state_t *rs, hook_entry_t *entry, void *nextHop);

void interceptor_routing_begin_bridge_handler(reg_state_t *rs, ClosureBridgeData *cbd);

void interceptor_routing_end_bridge_handler(reg_state_t *rs, ClosureBridgeData *cbd);

void interceptor_routing_dynamic_binary_instrumentation_bridge_handler(reg_state_t *rs, ClosureBridgeData *cbd);

void interceptor_routing_common_bridge_handler(reg_state_t *rs, ClosureBridgeData *cbd);

void dynamic_context_begin_invocation_bridge_handler(reg_state_t *rs, DynamicClosureTrampoline *cbd);

void dynamic_context_end_invocation_bridge_handler(reg_state_t *rs, DynamicClosureTrampoline *cbd);

void dynamic_common_bridge_handler(reg_state_t *rs, DynamicClosureTrampoline *cbd);
#endif