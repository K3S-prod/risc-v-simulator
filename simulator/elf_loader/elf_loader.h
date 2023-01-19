#pragma once

#include "third-party/ELFIO/elfio/elfio.hpp"

#include <cstdint>
#include <string>

namespace sim {

class ElfLoader {
    std::string fileName_;
    ELFIO::elfio elfFile_;
    char* data_ = NULL;
    unsigned dataSize_ = 0;

    int loadFromFile(std::string& elfFileName);
public:
    ElfLoader(std::string& elfFileName);
    size_t getEntryPoint();
    size_t recalculateEntryPoint(size_t entryPoint);
    unsigned calcEntrySegNum(size_t entryPoint);
    size_t calcEntrySegOffset(size_t entryPoint, unsigned segNum);
    void loadData();
    char* getData() {
        return data_;
    }

    unsigned getDataSize() {
        return dataSize_;
    }

    size_t getRecalcEntry() {
        size_t entry = getEntryPoint();
        return recalculateEntryPoint(entry);
    }
    
    ~ElfLoader() {
        free(data_);
    }
};

} // namespace sim
