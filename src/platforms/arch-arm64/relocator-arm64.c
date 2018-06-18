#include "relocator-arm64.h"
#include "ARM64AssemblyCore.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

ARM64Relocator *arm64_assembly_relocator_cclass(new)(ARM64Relocator *self, ARM64AssemblyReader *input,
                                                     ARM64AssemblyWriter *output) {
    self->input  = input;
    self->output = output;
}

void arm64_assembly_relocator_cclass(reset)(ARM64Relocator *self);

void arm64_assembly_relocator_cclass(try_relocate)(ARM64Relocator *self, void *address, int bytes_min, int *bytes_max);

void arm64_assembly_relocator_cclass(relocate_to)(ARM64Relocator *self, void *target_address);

void arm64_assembly_relocator_cclass(double_write)(ARM64Relocator *self, void *target_address);

void arm64_assembly_relocator_cclass(register_literal_instCTX)(ARM64Relocator *self, ARM64InstructionCTX *instCTX);

void arm64_assembly_relocator_cclass(relocate_write)(ARM64Relocator *self);

void arm64_assembly_relocator_cclass(relocate_write_all)(ARM64Relocator *self);

void arm64_assembly_relocator_cclass(rewrite_LoadLiteral)(ARM64Relocator *self, ARM64InstructionCTX *instCTX);
void arm64_assembly_relocator_cclass(rewrite_BaseCmpBranch)(ARM64Relocator *self, ARM64InstructionCTX *instCTX);
void arm64_assembly_relocator_cclass(rewrite_BranchCond)(ARM64Relocator *self, ARM64InstructionCTX *instCTX);
void arm64_assembly_relocator_cclass(rewrite_B)(ARM64Relocator *self, ARM64InstructionCTX *instCTX);
void arm64_assembly_relocator_cclass(rewrite_BL)(ARM64Relocator *self, ARM64InstructionCTX *instCTX);