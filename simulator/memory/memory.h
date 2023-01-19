#pragma once

#include "simulator/elf_loader/elf_loader.h"
#include "simulator/memory/mmu.h"
#include <array>

namespace sim {

// 4096 * 1024 = 4194304 = 2^22
constexpr unsigned DRAM_SIZE = 4096 * 1024;

class Memory {
    char* memory_;
    size_t size_ = 0x0;
    size_t entry_ = 0x0;

    MMU mmu_;
public:
    Memory(ElfLoader& loader);
    Memory() = default;
    void dump();
    char* fetchInstruction(VirtAddr virtAddr);
    void store(uint64_t value, VirtAddr virtAddr);
    uint64_t load(VirtAddr virtAddr);

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
};

} // namespace sim
