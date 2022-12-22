
#include <gtest/gtest.h>

#include "elf_loader.h"
#include "memory.h"
#include "simulator.h"

TEST(Elf_loader, Elf_loader_basic_construct) {
    std::string fileName = "fibbonacci";
    sim::ElfLoader elf_loader(fileName);
}

TEST(Elf_loader, Elf_loader_entry) {
    std::string fileName = "fibbonacci";
    sim::ElfLoader elf_loader(fileName);
    auto entryPoint = elf_loader.getEntryPoint();

    ASSERT_EQ(entryPoint, 0x1040);
}

TEST(Elf_loader, Elf_loader_recalc_entry) {
    std::string fileName = "fibbonacci";
    sim::ElfLoader elf_loader(fileName);
    auto entryPoint = elf_loader.getEntryPoint();
    auto recalc = elf_loader.recalculateEntryPoint(entryPoint);

    ASSERT_EQ(recalc, 0x608);
}

TEST(Memory, memory_init) {
    std::string elfFileName = "fibbonacci";
    sim::ElfLoader loader(elfFileName);
    loader.loadData();

    sim::Memory memory(loader);
    ASSERT_EQ(memory.getEntry(), 0x608);
    ASSERT_EQ(memory.getSize(), 2933);
}

TEST(Sim, integration) {
    std::string elfFileName = "fibbonacci";
    sim::Simulator state(elfFileName);
    state.runSimulation();
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
