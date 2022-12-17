

#include <array>
#include <cstddef>
#include <string>
#include <iostream>
#include <elfio/elfio.hpp>

#include "sim_memory.h"

namespace sim {
    
    int Memory::loadELF(std::string& elfFileName) {
        ELFIO::elfio reader;
        if (!reader.load(elfFileName)) {
            std::cerr << "ERROR: could not load file " << elfFileName << std::endl;
            return EXIT_FAILURE;
        }

        std::cout << "Reading segments from" << elfFileName << "..." << std::endl;
        ELFIO::Elf_Half segNum = reader.segments.size();
        std::cout << "Number of segments: " << segNum << std::endl;

        std::cout << "\t\tflags\tvaddr\tfsize\tmemory_size"<< std::endl;
        unsigned counter = 0;
        for (const auto& segment: reader.segments) {
            std::cout << "Segment [" << counter++ << "]:" << "\t";
            std::cout << std::hex << "0x" << segment->get_flags() << "\t";
            std::cout << std::hex << "0x" << segment->get_virtual_address() << "\t";
            std::cout << std::hex << "0x" << segment->get_file_size() << "\t";
            std::cout << std::hex << "0x" << segment->get_memory_size() << std::endl;
        }

        return 0;
    }

} // namespace sim
