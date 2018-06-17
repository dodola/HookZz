//
// Created by jmpews on 2018/6/15.
//

#ifndef HOOKZZ_MACROS_H
#define HOOKZZ_MACROS_H

#define ZZ_INT5_MASK 0x0000001f
#define ZZ_INT8_MASK 0x000000ff
#define ZZ_INT10_MASK 0x000003ff
#define ZZ_INT11_MASK 0x000007ff
#define ZZ_INT12_MASK 0x00000fff
#define ZZ_INT14_MASK 0x00003fff
#define ZZ_INT16_MASK 0x0000ffff
#define ZZ_INT18_MASK 0x0003ffff
#define ZZ_INT19_MASK 0x0007ffff
#define ZZ_INT24_MASK 0x00ffffff
#define ZZ_INT26_MASK 0x03ffffff
#define ZZ_INT28_MASK 0x0fffffff

#define THUMB_FUNCTION_ADDRESS(address) ((uintptr_t)address & ~(uintptr_t)1)
#define INSTRUCTION_IS_THUMB(insn_addr) (((uintptr_t)insn_addr & 0x1) == 0x1)
#define ALIGN_FLOOR(address, range) ((uintptr_t)address & ~((uintptr_t)range - 1))
#define ALIGN_CEIL(address, range) (((uintptr_t)address + (uintptr_t)range - 1) & ~((uintptr_t)range - 1))

#endif //HOOKZZ_MACROS_H
