
#include "simulator/elf_loader/elf_loader.h"
#include "common/macro.h"

#include <cstdlib>
#include <cassert>

namespace sim {

ElfLoader::ElfLoader(const std::string& elfFileName) {
    file_name_ = elfFileName;
    int ret = LoadFromFile();
    if (ret != 0) {
        throw std::runtime_error("Failed to construct elf loader");
    }
}

int ElfLoader::LoadFromFile() {
    if (!elf_file_.load(file_name_)) {
        std::cerr << "ERROR: could not load file " << file_name_ << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "INFO: Successfully loaded " << file_name_ << std::endl;
    return 0;
}

VAddr ElfLoader::GetMainEntryPoint()
{
    const ELFIO::section *symt_sect = nullptr;
    for (const auto& section: elf_file_.sections) {
        if (section->get_type() == ELFIO::SHT_SYMTAB) {
            symt_sect = section.get();
            break;
        }
    }
    ELFIO::const_symbol_section_accessor ssa(elf_file_, symt_sect);
    
    ELFIO::Elf64_Addr va_addr;
    ELFIO::Elf_Xword size;
    unsigned char bind;
    unsigned char type;
    ELFIO::Elf_Half section_index;
    unsigned char other;
    bool res = ssa.get_symbol("main", va_addr, size, bind, type, section_index, other);
    ASSERT(res);

    return va_addr;
}

} // namespace sim
