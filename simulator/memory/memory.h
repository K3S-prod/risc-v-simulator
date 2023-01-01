#pragma once

#include <array>
#include "elf_loader.h"

namespace sim {

// 4096 * 1024 = 4194304 = 2^22
constexpr unsigned DRAM_SIZE = 4096 * 1024;
// 4096 = 2^12
constexpr unsigned PAGE_SIZE = 4096;

/* MMU specs:
    m = 2^22 - number of addresses in physical address space
    n = 2^64 - number of addressed in virtual address space
    p = 2^12 - page size
*/

using VirtAddr = uint64_t;

class Memory {
    char* memory_;
    size_t size_ = 0x0;
    size_t entry_ = 0x0;
public:
    Memory(ElfLoader& loader);
    Memory() = default;
    void dump();
    char* fetchInstruction(uint64_t addr);
    void store(uint64_t value, VirtAddr addr);
    uint64_t load(VirtAddr addr);

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
