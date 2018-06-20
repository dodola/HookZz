#include <stdio.h>

// fake the not found function.
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
void closure_bridge_template();
void closure_bridge_trampoline_template();
#ifdef __cplusplus
}
#endif //__cplusplus
void closure_bridge_template() {}
void closure_bridge_trampoline_template() {}

#include "core.h"
#include "platforms/arch-arm64/relocator-arm64.h"
// mov x0, x0
// ldr x0, #0x10
// b #0x20
// cbz x0, #0x20
__attribute__((aligned(4))) uint32_t test_func[4]       = {0xAA0003E0, 0x58000080, 0x14000008, 0xB4000100};
__attribute__((aligned(4))) uint32_t relocated_func[32] = {0};

int get_input_relocate_ouput_count(ARM64Relocator *relocator, int i) {
    io_index_t *io_index = (io_index_t *)(list_at(relocator->io_indexs, i)->val);
    if (i == relocator->io_indexs->len - 1) {
        return relocator->output->instCTXs->len - io_index->output_index;
    } else {
        io_index_t *io_index_next = (io_index_t *)(list_at(relocator->io_indexs, i + 1)->val);
        return io_index_next->output_index - io_index->output_index;
    }
}

#define ARM64_FULL_REDIRECT_SIZE 16

int main(int argc, char *argv[]) {
    ARM64AssemblyReader *reader_arm64;
    ARM64AssemblyWriter *writer_arm64;
    ARM64Relocator *relocator_arm64;
    reader_arm64    = arm64_assembly_reader_cclass(new)(test_func, test_func + 4);
    writer_arm64    = arm64_assembly_writer_cclass(new)(0);
    relocator_arm64 = arm64_assembly_relocator_cclass(new)(reader_arm64, writer_arm64);

    int limit_relocate_inst_size = 0;
    arm64_assembly_relocator_cclass(try_relocate)(test_func, ARM64_FULL_REDIRECT_SIZE, &limit_relocate_inst_size);
    printf(">>> limit_relocate_inst_size: %d\n", limit_relocate_inst_size);

    // relocate `mov x0, x0`
    arm64_assembly_reader_cclass(read_inst)(reader_arm64);
    arm64_assembly_relocator_cclass(relocate_write)(relocator_arm64);
    printf("Relocate INFO:\n");
    int count            = get_input_relocate_ouput_count(relocator_arm64, 0);
    io_index_t *io_index = (io_index_t *)(list_at(relocator_arm64->io_indexs, 0)->val);
    for (int i = io_index->output_index; i < count + io_index->output_index; i++) {
        ARM64InstructionCTX *instCTX = (ARM64InstructionCTX *)(list_at(relocator_arm64->output->instCTXs, i)->val);
        printf("0x%x\n", instCTX->bytes);
    }
    return 0;
}