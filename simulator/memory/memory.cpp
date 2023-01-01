

#include <array>
#include <cstddef>
#include <string>
#include <iostream>
#include <elfio/elfio.hpp>

#include "memory.h"
#include "elf_loader.h"

namespace sim {

Memory::Memory(ElfLoader& loader) {
    memory_ = (char*) calloc(DRAM_SIZE, sizeof(char));
    auto data = loader.getData();
    memcpy(memory_, data, loader.getDataSize());
    entry_ = loader.getRecalcEntry();
    size_ = loader.getDataSize();
}

char* Memory::fetchInstruction(VirtAddr virtAddr) {
    throw std::runtime_error("ERROR: MMU not implemented");
}

void Memory::store(uint64_t value, VirtAddr virtAddr) {
    PhysAddr physAddr = mmu_.generatePhysAddr(virtAddr);
}

uint64_t Memory::load(VirtAddr virtAddr) {
    throw std::runtime_error("ERROR: MMU not implemented");
}

void Memory::dump() {
    std::cout << "INFO: Memory size: " << std::dec << size_ << std::endl;
    std::cout << "INFO: Dumping memory from address 0x0..." << std::endl;

    std::ofstream dumpFile("memory_dump.sim");
    if (dumpFile.is_open()) {
        dumpFile.write(memory_, size_);
        dumpFile.close();
    }
    std::cout << "INFO: Done." << std::endl;
}

} // namespace sim
