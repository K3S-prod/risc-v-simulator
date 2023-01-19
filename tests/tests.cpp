
#include <gtest/gtest.h>

#include "simulator/elf_loader/elf_loader.h"
#include "simulator/memory/memory.h"
#include "simulator/simulator.h"

TEST(Elf_loader, Elf_loader_basic_construct) {
    std::string fileName = "bin/elfs/fibbonacci_riscv";
    sim::ElfLoader elf_loader(fileName);
}

TEST(Elf_loader, Elf_loader_entry) {
    std::string fileName = "bin/elfs/fibbonacci_riscv";
    sim::ElfLoader elf_loader(fileName);
    auto entryPoint = elf_loader.getEntryPoint();

    ASSERT_EQ(entryPoint, 0x103e0);
}

TEST(Elf_loader, Elf_loader_recalc_entry) {
    std::string fileName = "bin/elfs/fibbonacci_riscv";
    sim::ElfLoader elf_loader(fileName);
    auto entryPoint = elf_loader.getEntryPoint();
    auto recalc = elf_loader.recalculateEntryPoint(entryPoint);

    ASSERT_EQ(recalc, 0x3e0);
}

TEST(Memory, memory_init) {
    std::string elfFileName = "bin/elfs/fibbonacci_riscv";
    sim::ElfLoader loader(elfFileName);
    sim::Memory memory(loader);

    ASSERT_EQ(memory.getEntry(), 0x3e0);
    ASSERT_EQ(memory.getSize(), 1484);
}

TEST(Sim, integration) {
    GTEST_SKIP();
    std::string elfFileName = "bin/elfs/fibbonacci_riscv";
    sim::Simulator state(elfFileName);
    state.runSimulation();
}

TEST(MMU, get_physical_address) {
    sim::VirtAddr virtAddr = 0x3fc0;
    sim::MMU mmu;
    auto addr = mmu.generatePhysAddr(virtAddr);
    std::cout << "INFO: MMU generated address: " << std::dec << addr << "(" <<
                                             std::hex << addr << ")" << std::endl;
    std::cout << "INFO: Memory limit: " << std::dec << sim::DRAM_SIZE << std::endl;
    ASSERT_EQ(addr, 0xfc0);
}

TEST(MMU, get_physical_addresses_multiple) {
    sim::VirtAddr virtAddr = 0xDEADBEEF;
    sim::MMU mmu;
    auto addr = mmu.generatePhysAddr(virtAddr);
    std::cout << "INFO: MMU generated address: " << std::dec << addr << "(" <<
                                             std::hex << addr << ")" << std::endl;
    ASSERT_EQ(addr,0xEEF);

    virtAddr = 0xEDA65;
    addr = mmu.generatePhysAddr(virtAddr);
    std::cout << "INFO: MMU generated address: " << std::dec << addr << "(" <<
                                             std::hex << addr << ")" << std::endl;
    ASSERT_EQ(addr, 0x1a65);
                                             
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
