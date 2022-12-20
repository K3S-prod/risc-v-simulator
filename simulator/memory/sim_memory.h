#pragma once

#include <array>
#include <vector>
#include <elfio/elfio.hpp>
constexpr unsigned DRAM_SIZE = 4096 * 1024;

namespace sim {

enum class accessModes {
    READ,
    WRITE
};

class Memory {
public:
    Memory() {
        addrSpace_ = (char*) calloc(DRAM_SIZE, sizeof(char));
    }
    
    char* getRawMemory() {
        return addrSpace_;
    }

    size_t getEntry() {
        return entry_;
    }

    void setEntry(size_t e_entry) {
        entry_ = e_entry;
    }

    void upload(size_t offset, const char* data, size_t len);

    void setSize(size_t e_size) {
        size_ = e_size;
    }

    void setMemoryRegion(unsigned regionID, size_t left, size_t right, ELFIO::Elf_Word permission) {
        std::pair<size_t, size_t> borders = std::make_pair(left, right);
        memoryRegions.emplace(regionID, borders);
        regionPermissions.emplace(regionID, permission);
    }

    unsigned getAccessRegion(size_t offset);
    void accessMemory(size_t offset, size_t nelem, enum accessModes);
    void fetchInstruction(uint64_t addr);
    uint64_t load(uint64_t addr);
    void  store(uint64_t  addr);
    void dump();

    ~Memory() {
        free(addrSpace_);
    }
private:
    char* addrSpace_;
    size_t size_ = 0x0;
    size_t entry_ = 0x0;
    std::unordered_map<unsigned, std::pair<size_t, size_t>> memoryRegions;
    std::unordered_map<unsigned, ELFIO::Elf_Word> regionPermissions;
};

} // namespace sim
