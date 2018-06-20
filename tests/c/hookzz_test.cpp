#define CATCH_CONFIG_MAIN
#include "catch.hpp"

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

#include "memory_manager.h"
TEST_CASE(">>> memory_manager_t", "[memory]") {
    memory_manager_t *memory_manager = memory_manager_cclass(shared_instance)();
    REQUIRE(memory_manager != NULL);

    int page_size = memory_manager_cclass(get_page_size)();
    REQUIRE(page_size > 0);

    void *page_ptr = memory_manager_cclass(allocate_page)(memory_manager, 1 | 2, 1);
    REQUIRE(page_ptr != NULL);

    memory_manager_cclass(get_process_memory_layout)(memory_manager);
    REQUIRE(memory_manager->process_memory_layout->len > 0);

    CodeSlice *cs = memory_manager_cclass(allocate_code_slice)(memory_manager, 0x16);
    REQUIRE(cs != NULL);
    REQUIRE(cs->size == 0x16);
}

#include "core.h"
#include "interceptor.h"
TEST_CASE(">>> interceptor_t", "[interceptor]") {
    interceptor_t *interceptor = interceptor_cclass(shared_instance)();
    REQUIRE(interceptor != NULL);
    hook_entry_t *entry   = SAFE_MALLOC_TYPE(hook_entry_t);
    entry->target_address = (void *)0x1234;
    interceptor_cclass(add_hook_entry)(interceptor, entry);
    REQUIRE(interceptor->hook_entries->len == 1);
    hook_entry_t *find_entry = interceptor_cclass(find_hook_entry)(interceptor, (void *)0x1234);
    REQUIRE(find_entry != NULL);
}

#include "core.h"
#include "platforms/arch-arm64/relocator-arm64.h"
// mov x0, x0
// ldr x0, #0x10
// b #0x20
// cbz x0, #0x20
__attribute__((aligned(4))) uint32_t test_func[4]       = {0xE00300AA, 0x80000058, 0x08000014, 0x000100B4};
__attribute__((aligned(4))) uint32_t relocated_func[32] = {0};

#define ARM64_FULL_REDIRECT_SIZE 16

TEST_CASE(">>> ARM64Relocator", "[relocator]") {

    ARM64AssemblyReader *reader_arm64;
    ARM64AssemblyWriter *writer_arm64;
    ARM64Relocator *relocator_arm64;
    reader_arm64    = arm64_assembly_reader_cclass(new)(test_func, test_func + 4);
    writer_arm64    = arm64_assembly_writer_cclass(new)(0);
    relocator_arm64 = arm64_assembly_relocator_cclass(new)(reader_arm64, writer_arm64);

    int limit_relocate_inst_size = 0;
    arm64_assembly_relocator_cclass(try_relocate)(test_func, ARM64_FULL_REDIRECT_SIZE, &limit_relocate_inst_size);
    printf(">>> limit_relocate_inst_size: %d", limit_relocate_inst_size);

    // relocate `mov x0, x0`
    arm64_assembly_reader_cclass(read_inst)(reader_arm64);
    arm64_assembly_relocator_cclass(relocate_write)(relocator_arm64);
    return;
}