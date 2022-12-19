

#include <array>
#include <cstddef>
#include <string>
#include <iostream>
#include <elfio/elfio.hpp>

#include "sim_memory.h"

namespace sim {
    
    int Memory::loadELF(const std::string& elfFileName) {
        ELFIO::elfio reader;
        if (!reader.load(elfFileName)) {
            std::cerr << "ERROR: could not load file " << elfFileName << std::endl;
            return EXIT_FAILURE;
        }


        std::cout << "Reading segments from " << elfFileName << "..." << std::endl;
        auto segNum = reader.segments.size();
        std::cout << "Number of segments: " << segNum << std::endl;

        std::cout << "\t\tflags\tvaddr\tfsize\tmemory_size"<< std::endl;

        unsigned counter = 0;

        auto globalEntry = reader.get_entry();
        unsigned entrySegmentNum = 0;
        int entryOffset = 0;

        for (const auto& segment: reader.segments) {
            auto segOffset = segment->get_offset();
            if (globalEntry > segOffset && globalEntry < segOffset + segment->get_memory_size()) {
                entrySegmentNum = counter;
                entryOffset = globalEntry - segOffset;
            }
            ++counter;
        }
        counter = 0;

        for (const auto& segment: reader.segments) {
            std::cout << "Segment [" << counter << "]:" << "\t";
            std::cout << std::hex << "0x" << segment->get_flags() << "\t";
            std::cout << std::hex << "0x" << segment->get_virtual_address() << "\t";
            std::cout << std::hex << "0x" << segment->get_file_size() << "\t";
            std::cout << std::hex << "0x" << segment->get_memory_size() << std::endl;
            
            auto segment_type = segment->get_type();
            std::cout << "Segment type: " << std::hex << "0x" << segment->get_type() << std::endl;
            std::cout << "Segment memory size: " << std::hex << "0x" << segment->get_memory_size() << std::endl;

            if (counter == entrySegmentNum) {
                entry = offset + entryOffset;
            }

            if (segment_type == ELFIO::PT_LOAD) {
                auto segmentData = segment->get_data();
                auto segmentDataLen = segment->get_memory_size();
                std::cout << "Segment data length: " << segmentDataLen << std::endl;
                    std::cout << "INFO: loading segment " << counter << " into memory..." << std::endl;
                if (offset >= static_cast<uint64_t>(DRAM_SIZE)) {
                    std::cerr << "ERROR: ELF file segments are to large. Maximum memory limit: " << DRAM_SIZE << std::endl;
                }
                memcpy(addrSpace + offset, segmentData, static_cast<size_t>(segmentDataLen));
                offset += static_cast<size_t>(segmentDataLen);
            }

            counter++;
        }

        std::cout << "Entrypoint address: " << std::hex << entry << std::endl;

        dump();
        return 0;
    }

    void Memory::dump() {
        std::cout << "Memory offset: " << std::dec << offset << std::endl;
        std::cout << "Dumping memory from address 0x0..." << std::endl;

        std::ofstream dumpFile("memory_dump.sim");
        if (dumpFile.is_open()) {
            dumpFile.write(addrSpace, offset);
            dumpFile.close();
        }
        std::cout << "Done." << std::endl;
    }

} // namespace sim
