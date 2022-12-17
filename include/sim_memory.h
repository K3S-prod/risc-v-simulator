#pragma once

#include <array>
constexpr unsigned DRAM_SIZE = 4096 * 1024;

namespace sim {

class Memory {
    char* addrSpace;
    uint64_t offset = 0x0;
public:
    Memory() {
        addrSpace = (char*) calloc(DRAM_SIZE, sizeof(char));
    }

    void fetchInstruction(uint64_t addr);
    uint64_t load(uint64_t addr);
    void  store(uint64_t  addr);
    int loadELF(std::string& elfFileName);
    void dump();

    ~Memory() {
        free(addrSpace);
    }
};

} // namespace sim