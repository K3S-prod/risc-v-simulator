
#include <elfio/elfio.hpp>
#include <cstdlib>
#include <cassert>
#include "elf_loader.h"
#include "memory.h"

namespace sim {

ElfLoader::ElfLoader(std::string& elfFileName) {
    data_ = (char*) calloc(DRAM_SIZE, sizeof(char));
    assert(data_ != NULL);
    fileName_ = elfFileName;
    int ret = loadFromFile(elfFileName);
    if (ret != 0) {
        throw std::runtime_error("Failed to construct elf loader");
    }
}

int ElfLoader::loadFromFile(std::string& elfFileName) {
    if (!elfFile_.load(elfFileName)) {
        std::cerr << "ERROR: could not load file " << elfFileName << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "INFO: Successfully loaded " << elfFileName << std::endl;
    return 0;
}

size_t ElfLoader::getEntryPoint() {
    return static_cast<size_t>(elfFile_.get_entry());
}

unsigned ElfLoader::calcEntrySegNum(size_t entryPoint) {
    ELFIO::Elf_Half segNum = 0;
    for (const auto& segment: elfFile_.segments) {
        auto segOffset = segment->get_offset();
        auto segNum = segment->get_index();
        if (entryPoint > segOffset && entryPoint < segOffset + segment->get_memory_size()) {
            return segNum;
        }
    }
    return segNum;
}

size_t ElfLoader::calcEntrySegOffset(size_t entryPoint, unsigned segNum) {
    auto segment = elfFile_.segments[segNum];
    return entryPoint - segment->get_offset();
}

size_t ElfLoader::recalculateEntryPoint(size_t entryPoint) {
    auto entrySegNum = calcEntrySegNum(entryPoint);
    std::cout << "INFO: entry point in segment number: " << entrySegNum << std::endl;
    
    auto entrySegOffset = calcEntrySegOffset(entryPoint, entrySegNum);
    std::cout << "INFO: entry offset: " << std::hex << entrySegOffset << std::endl;

    size_t recalculatedEntryPoint = 0;
    size_t segmentsSize = 0;
    for (const auto& segment: elfFile_.segments) {
        if (segment->get_type() == ELFIO::PT_LOAD) {
            auto segSize = segment->get_memory_size();
            if (segment->get_index() == entrySegNum) {
                recalculatedEntryPoint = segmentsSize + entrySegOffset;
            }
            segmentsSize += static_cast<size_t>(segSize);
        }
    }

    std::cout << "INFO: recalculated entrypoint: " << std::hex << recalculatedEntryPoint << std::endl;
    return recalculatedEntryPoint;
}

void ElfLoader::loadData() {
    size_t size = 0;
    for (const auto& segment: elfFile_.segments) {
        if (segment->get_type() == ELFIO::PT_LOAD) {
            auto segAddr = segment->get_virtual_address();
            auto segSize = segment->get_memory_size();
            auto segData = segment->get_data();
            std::cout << "INFO: loading segment at address " << segAddr << ", size: " << segSize << std::endl;
            memcpy(data_ + size, segData, segSize);
            size += static_cast<size_t>(segSize);
        }
    }
    dataSize_ = size;
}

} // namespace sim
