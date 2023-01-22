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
    
    void *GetNativePointer(VAddr vaddr)
    {
        ASSERT(vaddr.ToReg() < 0xC0000000);
        auto page = memory_.GetPage(vaddr);
        ASSERT(page != nullptr);
        return page->GetData() + vaddr.offs;
    }
    Page *GetPage(VAddr vaddr)
    {
        ASSERT(vaddr.ToReg() < 0xC0000000);
        auto p_tlb = TryTLB(vaddr);
        if (p_tlb == nullptr) {
            auto p = memory_.GetPage(vaddr);
            tlb_[vaddr.GenVPN()] = p;
            return p;
        }
        return p_tlb;
    }

    Page *TryTLB(VAddr vaddr)
    {
        const auto &res = tlb_.find(vaddr.GenVPN());
        if (res != tlb_.end()) {
            return res->second;
        }
        return nullptr;
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

    std::unordered_map<uint32_t, Page *> tlb_;
};

} // namespace sim
