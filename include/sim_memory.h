#pragma once

#include <array>
constexpr unsigned DRAM_SIZE = 4096 * 1024;

namespace sim {

class Memory {
    char* addrSpace;
    size_t size = 0x0;
    size_t entry = 0x0;
public:
    Memory() {
        addrSpace = (char*) calloc(DRAM_SIZE, sizeof(char));
    }
    
    char* getRawMemory() {
        return addrSpace;
    }

    size_t getEntry() {
        return entry;
    }

    void setEntry(size_t e_entry) {
        entry = e_entry;
    }

    void upload(size_t offset, const char* data, size_t len);

    void setSize(size_t e_size) {
        size = e_size;
    }

    void fetchInstruction(uint64_t addr);
    uint64_t load(uint64_t addr);
    void  store(uint64_t  addr);
    int loadELF(const std::string& elfFileName);
    void dump();

    ~Memory() {
        free(addrSpace);
    }
};

} // namespace sim
