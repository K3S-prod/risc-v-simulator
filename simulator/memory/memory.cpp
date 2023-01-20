#include "memory.h"

#include <array>
#include <cstddef>
#include <string>
#include <iostream>

namespace sim {

Memory::Memory() : memory_(DRAM_SIZE / PAGE_SIZE)
{
}

/*
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
*/
} // namespace sim
