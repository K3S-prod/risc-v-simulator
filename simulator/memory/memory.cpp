

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

void Memory::dump() {
    std::cout << "Memory size: " << std::dec << size_ << std::endl;
    std::cout << "Dumping memory from address 0x0..." << std::endl;

    std::ofstream dumpFile("memory_dump.sim");
    if (dumpFile.is_open()) {
        dumpFile.write(memory_, size_);
        dumpFile.close();
    }
    std::cout << "Done." << std::endl;
}

} // namespace sim
