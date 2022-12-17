#pragma once

#include <array>
constexpr unsigned DRAM_LEN = 4096;

namespace sim {

class Memory {
    std::array<std::byte, DRAM_LEN> addrSpace;
public:
    Memory() = default;
    void fetchInstruction(uint64_t addr);
    uint64_t load(uint64_t addr);
    void  store(uint64_t  addr);
    int loadELF(std::string& elfFileName);
};

} // namespace sim
