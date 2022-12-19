#pragma once

#include <array>
constexpr unsigned DRAM_SIZE = 4096 * 1024;

namespace sim {

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

    void fetchInstruction(uint64_t addr);
    uint64_t load(uint64_t addr);
    void  store(uint64_t  addr);
    int loadELF(const std::string& elfFileName);
    void dump();

    ~Memory() {
        free(addrSpace_);
    }
private:
    char* addrSpace_;
    size_t size_ = 0x0;
    size_t entry_ = 0x0;
};

} // namespace sim
