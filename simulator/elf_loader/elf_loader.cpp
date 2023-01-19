#include "elf_loader.h"
#include "simulator/memory/memory.h"
#include "common/macro.h"

#include <cstdlib>
#include <cassert>

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

size_t ElfLoader::getEntryPoint()
{
    const ELFIO::section *symt_sect = nullptr;
    for (const auto& section: elfFile_.sections) {
        if (section->get_type() == ELFIO::SHT_SYMTAB) {
            symt_sect = section.get();
            break;
        }
    }
    ELFIO::const_symbol_section_accessor ssa(elfFile_, symt_sect);
    
    ELFIO::Elf64_Addr va_addr;
    ELFIO::Elf_Xword size;
    unsigned char bind;
    unsigned char type;
    ELFIO::Elf_Half section_index;
    unsigned char other;
    bool res = ssa.get_symbol("main", va_addr, size, bind, type, section_index, other);
    ASSERT(res);

    return static_cast<size_t>(va_addr);
}

unsigned ElfLoader::calcEntrySegNum(size_t entryPoint) {
    ELFIO::Elf_Half segNum = 0;
    for (const auto& segment: elfFile_.segments) {
        auto segVAddr = segment->get_virtual_address();
        auto segNum = segment->get_index();
        if (entryPoint > segVAddr && entryPoint < segVAddr + segment->get_memory_size()) {
            return segNum;
        }
    }
    return segNum;
}

size_t ElfLoader::calcEntrySegOffset(size_t entryPoint, unsigned segNum) {
    auto segment = elfFile_.segments[segNum];
    return entryPoint - segment->get_offset();
}

// TODO(dkofanov): Remove this when virtual memory will be implemented: 
size_t ElfLoader::recalculateEntryPoint(size_t entryPoint) {
    auto entrySegNum = calcEntrySegNum(entryPoint);
    std::cout << "INFO: entry point in segment number: " << entrySegNum << std::endl;
    
    auto entrySegOffset = calcEntrySegOffset(entryPoint, entrySegNum);
    std::cout << "INFO: entry offset: " << std::hex << entrySegOffset << std::endl;

    for (const auto& segment: elfFile_.segments) {
        if (segment->get_index() == entrySegNum) {
            size_t recalculatedEntryPoint = entryPoint - segment->get_virtual_address();
            std::cout << "INFO: recalculated entrypoint: " << std::hex << recalculatedEntryPoint << std::endl;
            return recalculatedEntryPoint;
        }
    }
    UNREACHABLE();
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
