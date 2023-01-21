#pragma once

#include "third-party/ELFIO/elfio/elfio.hpp"
#include "simulator/memory/primitives.h"

#include <cstdint>
#include <string>

namespace sim {

class ElfLoader {
public:
    ElfLoader(const std::string& elfFileName);
    int LoadFromFile();
    VAddr GetEntryPoint()
    {
        return elf_file_.get_entry();
    }

    VAddr GetMainEntryPoint();  // deprecated;

    const auto &GetElfFile() const
    {
        return elf_file_;
    }

private:
    std::string file_name_;
    ELFIO::elfio elf_file_;
};

} // namespace sim
