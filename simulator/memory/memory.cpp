

#include <array>
#include <cstddef>
#include <string>
#include <iostream>
#include <elfio/elfio.hpp>

#include "memory.h"
#include"elf_loader.h"

namespace sim {

int Memory::setFromElfLoader(ElfLoader& loader) {
        // memcpy(memory_, )
    return EXIT_SUCCESS;
}

// void Memory::dump() {
//     std::cout << "Memory offset: " << std::dec << size_ << std::endl;
//     std::cout << "Dumping memory from address 0x0..." << std::endl;

//     std::ofstream dumpFile("memory_dump.sim");
//     if (dumpFile.is_open()) {
//         dumpFile.write(addrSpace_, size_);
//         dumpFile.close();
//     }
//     std::cout << "Done." << std::endl;
// }

// void Memory::loadMemory(size_t offset, const char* data, size_t len) {
//     memcpy(addrSpace_ + offset, data, len);
// }

} // namespace sim
