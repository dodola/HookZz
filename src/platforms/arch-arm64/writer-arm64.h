/**
 *    Copyright 2017 jmpews
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef platforms_arch_arm64_writer_arm64_h
#define platforms_arch_arm64_writer_arm64_h

#include "instruction.h"
#include "register-arm64.h"
#include "writer-arm64.h"

#include "std_kit/std_buffer_array.h"
#include "std_kit/std_list.h"

typedef struct _ARM64AssemblyWriter {
    void *start_pc;
    void *start_address;

    list_t *instCTXs;
    buffer_array_t *inst_bytes;
} ARM64AssemblyWriter;

#define arm64_assembly_writer_cclass(member) cclass(arm64_assembly_writer, member)

ARM64AssemblyWriter *arm64_assembly_writer_cclass(new)(void *pc);
void arm64_assembly_writer_cclass(reset)(ARM64AssemblyWriter *self, void *pc);
void arm64_assembly_writer_cclass(patch_to)(ARM64AssemblyWriter *self, void *target_address);
void arm64_assembly_writer_cclass(near_patch_to)(ARM64AssemblyWriter *self, void *target_address, int range);
void arm64_assembly_writer_cclass(relocate_patch_to)(ARM64AssemblyWriter *self, void *target_address,
                                                     ARM64Relocator *relocator_arm64);

inline void arm64_assembly_writer_cclass(put_bytes)(ARM64AssemblyWriter *self, void *data, int length);

inline void arm64_assembly_writer_cclass(put_ldr_reg_imm)(ARM64AssemblyWriter *self, ARM64Reg reg, uint32_t offset) {
    ARM64RegInfo ri;
    DescribeARM64Reigster(reg, &ri);

    uint32_t imm19, Rt;
    imm19         = offset >> 2;
    Rt            = ri.index;
    uint32_t inst = 0x58000000 | imm19 << 5 | Rt;

    arm64_assembly_writer_cclass(put_bytes)(self, (void *)&inst, 4);
}
inline void arm64_assembly_writer_cclass(put_str_reg_reg_offset)(ARM64AssemblyWriter *self, ARM64Reg src_reg,
                                                                 ARM64Reg dest_reg, uint64_t offset) {
    ARM64RegInfo rs, rd;
    DescribeARM64Reigster(src_reg, &rs);
    DescribeARM64Reigster(dest_reg, &rd);

    uint32_t size, v = 0, opc = 0, Rn_ndx, Rt_ndx;
    Rn_ndx = rd.index;
    Rt_ndx = rs.index;

    if (rs.isInteger) {
        size = (rs.width == 64) ? 0b11 : 0b10;
    }

    uint32_t imm12 = offset >> size;
    uint32_t inst  = 0x39000000 | size << 30 | opc << 22 | imm12 << 10 | Rn_ndx << 5 | Rt_ndx;
    arm64_assembly_writer_cclass(put_bytes)(self, (void *)&inst, 4);
}
inline void arm64_assembly_writer_cclass(put_ldr_reg_reg_offset)(ARM64AssemblyWriter *self, RM64Reg dest_reg,
                                                                 ARM64Reg src_reg, uint64_t offset) {
    ARM64RegInfo rs, rd;
    DescribeARM64Reigster(src_reg, &rs);
    DescribeARM64Reigster(dest_reg, &rd);

    uint32_t size, v = 0, opc = 0b01, Rn_ndx, Rt_ndx;
    Rn_ndx = rs.index;
    Rt_ndx = rd.index;

    if (rs.isInteger) {
        size = (rs.width == 64) ? 0b11 : 0b10;
    }

    uint32_t imm12 = offset >> size;
    uint32_t inst  = 0x39000000 | size << 30 | opc << 22 | imm12 << 10 | Rn_ndx << 5 | Rt_ndx;
    arm64_assembly_writer_cclass(put_bytes)(self, (void *)&inst, 4);
}
inline void arm64_assembly_writer_cclass(put_br_reg)(ARM64AssemblyWriter *self, ARM64Reg reg) {
    ARM64RegInfo ri;
    DescribeARM64Reigster(reg, &ri);

    uint32_t op   = 0, Rn_ndx;
    Rn_ndx        = ri.index;
    uint32_t inst = 0xd61f0000 | op << 21 | Rn_ndx << 5;
    arm64_assembly_writer_cclass(put_bytes)(self, (void *)&inst, 4);
}
inline void arm64_assembly_writer_cclass(put_blr_reg)(ARM64AssemblyWriter *self, ARM64Reg reg) {
    ARM64RegInfo ri;
    DescribeARM64Reigster(reg, &ri);

    uint32_t op = 0b01, Rn_ndx;

    Rn_ndx        = ri.index;
    uint32_t inst = 0xd63f0000 | op << 21 | Rn_ndx << 5;
    arm64_assembly_writer_cclass(put_bytes)(self, (void *)&inst, 4);
}
inline void arm64_assembly_writer_cclass(put_b_imm)(ARM64AssemblyWriter *self, uint64_t offset) {
    uint32_t op   = 0b0, imm26;
    imm26         = (offset >> 2) & 0x03ffffff;
    uint32_t inst = 0x14000000 | op << 31 | imm26;
    arm64_assembly_writer_cclass(put_bytes)(self, (void *)&inst, 4);
}

#endif