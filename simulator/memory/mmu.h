#pragma once

#include "memory.h"
#include "primitives.h"
#include "simulator/elf_loader/elf_loader.h"
#include "common/macro.h"


namespace sim {

class MMU {
public:
    MMU(const std::string& elfFileName);
    
    Page *AllocPage(VAddr vaddr)
    {
        return memory_.AllocPage(vaddr);
    }
    Page *GetOrAllocPage(VAddr vaddr)
    {
        return memory_.GetOrAllocPage(vaddr);
    }
    Page *GetPage(VAddr vaddr)
    {
        return memory_.GetPage(vaddr);
    }
    VAddr GetEntryPoint()
    {
        return entry_;
    }

    //PhysAddr generatePhysAddr(VAddr vaddr);
    //uint64_t getPPN(uint64_t VPN);
private:
    void LoadElfData(const ElfLoader &loader);
private:
    Memory memory_;
    VAddr entry_;
};

} // namespace sim
