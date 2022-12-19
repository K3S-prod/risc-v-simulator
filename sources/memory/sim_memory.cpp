

#include <array>
#include <cstddef>
#include <string>
#include <iostream>
#include <elfio/elfio.hpp>

#include "sim_memory.h"

namespace sim {

    void Memory::dump() {
        std::cout << "Memory offset: " << std::dec << size << std::endl;
        std::cout << "Dumping memory from address 0x0..." << std::endl;

        std::ofstream dumpFile("memory_dump.sim");
        if (dumpFile.is_open()) {
            dumpFile.write(addrSpace, size);
            dumpFile.close();
        }
        std::cout << "Done." << std::endl;
    }

    void Memory::upload(size_t offset, const char* data, size_t len) {
        memcpy(addrSpace + offset, data, len);
    }

} // namespace sim
