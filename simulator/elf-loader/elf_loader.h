#pragma once

#include <string>
#include <elfio/elfio.hpp>

namespace sim {

class ElfLoader {
    std::string fileName_;
    ELFIO::elfio elfFile_;
    int loadFromFile(std::string& elfFileName);
public:
    ElfLoader(std::string& elfFileName);
    size_t getEntryPoint();
    size_t recalculateEntryPoint(size_t entryPoint);
    unsigned calcEntrySegNum(size_t entryPoint);
    size_t calcEntrySegOffset(size_t entryPoint, unsigned segNum);
    ~ElfLoader() = default;
};

} // namespace sim
