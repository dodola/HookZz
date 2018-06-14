//
// Created by jmpews on 2018/6/14.
//

#include "ARM64Relocator.h"


ARM64Relocator::ARM64Relocator() {

}

void ARM64Relocator::reset() {
    output->reset();
    input->reset();
    literalInstCTXs.clear();
    indexRelocatedInputOutput.clear();
}

void ARM64Relocator::tryRelocate(void *address, int bytes_min, int *bytes_max) {
    int tmpSize = 0;
    bool earlyEnd = false;

    ARM64InstructionCTX *instCTX;

    ARM64AssemblyReader *reader = new ARM64AssemblyReader(address);

    do {
        instCTX = reader->readInstruction();
        switch (getInstType(instCTX->bytes)) {
        case BImm:
            earlyEnd = true;
            break;
        default:;
        }
        tmpSize += instCTX->size;

    }while (tmpSize < bytes_min);

    if(earlyEnd) {
        *bytes_max = bytes_min;
    }
    delete(reader);
}

void ARM64Relocator::relocateTo(void *target_address) {
    for(auto instCTX : literalInstCTXs) {
        zz_addr_t literal_target_address;
        literal_target_address = *(zz_addr_t *)instCTX->address;
        if(literal_target_address > (zz_addr_t)input->start_pc && literal_target_address < ((zz_addr_t)input->start_pc + input->instBytes.size())) {
            for(auto it : indexRelocatedInputOutput) {
                ARM64InstructionCTX *inputInstCTX = input->instCTXs[it.first];
                if(inputInstCTX->address == literal_target_address) {
                    *(zz_addr_t *)instCTX->address = output->instCTXs[it.second]->pc - (zz_addr_t)output->start_pc + (zz_addr_t)target_address;
                    break;
                }
            }
        }
    }
}

void ARM64Relocator::doubleWrite(void *target_address) {
    assert((zz_addr_t)target_address % 4 == 0);

    int originInstByteSize = output->instBytes.size();
    output->reset();

    literalInstCTXs.clear();
    indexRelocatedInputOutput.clear();
    relocateWriteAll();

    int noNeedRelocateInstByteSize = output->instBytes[]


}

void* ARM64Relocator::registerLiteralInstCTX(ARM64InstructionCTX *instCTX) {
    literalInstCTXs.push_back(instCTX);
}