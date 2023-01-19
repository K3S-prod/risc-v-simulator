#pragma once

#include "simulator/elf_loader/elf_loader.h"
#include <array>
constexpr unsigned DRAM_SIZE = 4096 * 1024;

namespace sim {

class Memory {
public:
    Memory(ElfLoader& loader);
    Memory() = default;
    void dump();
    
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

    size_t getSize() {
        return size_;
    }

    ~Memory() {
        free(memory_);
    }

private:
    char* memory_;
    size_t size_ = 0x0;
    size_t entry_ = 0x0;
};

} // namespace sim
