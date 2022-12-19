
#include <elfio/elfio.hpp>

#include "cpu.h"
#include "simulator.h"

namespace sim {
    void Simulator::runSimulation() {
        cpu_.runPipeline();
    };

    int Simulator::loadELF(std::string& elfFileName) {
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
        size_t size = 0;

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
                memory_.setEntry(size + entryOffset);
            }

            if (segment_type == ELFIO::PT_LOAD) {
                auto segmentData = segment->get_data();
                auto segmentDataLen = segment->get_memory_size();
                std::cout << "Segment data length: " << segmentDataLen << std::endl;
                    std::cout << "INFO: loading segment " << counter << " into memory..." << std::endl;
                if (size >= static_cast<uint64_t>(DRAM_SIZE)) {
                    std::cerr << "ERROR: ELF file segments are to large. Maximum memory limit: " << DRAM_SIZE << std::endl;
                }
                memory_.upload(size, segmentData, static_cast<size_t>(segmentDataLen));
                size += static_cast<size_t>(segmentDataLen);
            }

            counter++;
        }

        memory_.setSize(size);

        std::cout << "Entrypoint address: " << std::hex << memory_.getEntry() << std::endl;

        return 0;
    }

} // namespace sim
