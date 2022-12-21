#pragma once

#include <array>
#include "elf_loader.h"
constexpr unsigned DRAM_SIZE = 4096 * 1024;

namespace sim {

class Memory {
public:
    Memory() {
        memory_ = (char*) calloc(DRAM_SIZE, sizeof(char));
    }
    
    char* getRawMemory() {
        return memory_;
    }

    size_t getEntry() {
        return entry_;
    }

    void setEntry(size_t entry) {
        entry_ = entry;
    }

    void setSize(size_t size) {
        size_ = size;
    }

    int setFromElfLoader(ElfLoader& loader);

    ~Memory() {
        free(memory_);
    }

private:
    char* memory_;
    size_t size_ = 0x0;
    size_t entry_ = 0x0;
};

} // namespace sim
