#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "memory_manager.h"
#include "writer-arm64.h"

inline void ReadBytes(void *data, void *address, int length) { memcpy(data, address, length); }

ARM64AssemblyWriter *arm64_assembly_writer_cclass(new)(void *pc) {
    ARM64AssemblyWriter *writer = SAFE_MALLOC_TYPE(ARM64AssemblyWriter);
    writer->start_pc            = pc;
    writer->instCTXs            = list_new();
    writer->inst_bytes          = buffer_array_create(64);
    return;
}

void arm64_assembly_writer_cclass(reset)(ARM64AssemblyWriter *self, void *pc) {
    self->start_pc = pc;

    list_destroy(self->instCTXs);
    self->instCTXs = list_new();

    buffer_array_clear(self->inst_bytes);
    return;
}

void arm64_assembly_writer_cclass(patch_to)(ARM64AssemblyWriter *self, void *target_address) {
    self->start_address = target_address;
    memory_manager_t *memory_manager;
    memory_manager = memory_manger_class(shared_instance)();
    memory_manger_class(patch_code)(memory_manager, target_address, self->inst_bytes->data, self->inst_bytes->size);
    return;
}

void arm64_assembly_writer_cclass(near_patch_to)(ARM64AssemblyWriter *self, void *target_address, int range) {
    self->start_address = target_address;
    CodeCave *cc;
    memory_manager_t *memory_manager;
    memory_manager = memory_manger_class(shared_instance)();
    cc = memory_manger_class(search_near_code_cave)(memory_manager, target_address, range, self->inst_bytes.size);
    CHECK(cc);
    memory_manger_class(patch_code)(memory_manager, target_address, self->inst_bytes->data, self->inst_bytes->size);
    SAFE_FREE(cc);
    return;
}

void arm64_assembly_writer_cclass(relocate_patch_to)(ARM64AssemblyWriter *self, void *target_address,
                                                     ARM64Relocator *relocator) {
    self->start_address = target_address;
    CodeSlice *cs;
    memory_manager_t *memory_manager;
    memory_manager = memory_manger_class(shared_instance)();
    cs             = memory_manger_class(allocate_code_slice)(memory_manager, self->inst_bytes->size);
    CHECK(cs);
    arm64_assembly_relocator_cclass(double_write)(relocator, cs->data);
    memory_manger_class(patch_code)(memory_manager, cs->data, self->inst_bytes->data, self->inst_bytes->size);
    SAFE_FREE(cc);
    return;
}

void arm64_assembly_writer_cclass(put_bytes)(ARM64AssemblyWriter *self, void *data, int length) {
    ARM64InstructionCTX *instCTX = SAFE_MALLOC_TYPE(ARM64InstructionCTX);

    instCTX->pc      = (zz_addr_t)self->start_pc + self->inst_bytes->size;
    instCTX->address = (zz_addr_t)self->inst_bytes->data + self->inst_bytes->size;
    instCTX->size    = 4;
    ReadBytes(&instCTX->bytes, (void *)instCTX->address, 4);

    buffer_array_put(self->inst_bytes, (void *)instCTX->address, 4);

    list_rpush(self->instCTXs, (list_node_t *)instCTX);
}
