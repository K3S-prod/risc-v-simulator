#pragma once

#include "simulator/elf_loader/elf_loader.h"
#include "common/macro.h"

#include <array>
#include <vector>
#include <unordered_map>

namespace sim {

// 4096 * 1024 = 4194304 = 2^22
constexpr size_t PP_NUM = 1024;
constexpr size_t DRAM_SIZE = PAGE_SIZE * PP_NUM;

class Memory {
public:
    Memory();
    void dump();
    char* fetchInstruction(VAddr virtAddr);
    void store(uint64_t value, VAddr virtAddr);
    uint64_t load(VAddr virtAddr);
    
    uint32_t GenPPN(uint16_t ppn_1, uint16_t ppn_0)
    {
        // Change this to 12 bits:
        constexpr uint16_t PPN_1_INV_MASK = 0b1111110000000000;
        ASSERT((ppn_1 & PPN_1_INV_MASK) == 0);
        constexpr uint16_t PPN_0_INV_MASK = 0b1111110000000000;
        ASSERT((ppn_0 & PPN_0_INV_MASK) == 0);

        uint32_t ppn = ppn_1;
        ppn <<= 10U;
        ppn |= ppn_0;
        return ppn;
    }

    Page *GetOrAllocPage(VAddr vaddr)
    {
        auto *p = GetPage(vaddr);
        if (p != nullptr) {
            return p;
        }
        return AllocPage(vaddr);
    }

    Page *AllocPage(VAddr vaddr)
    {
        PTE *pt = static_cast<PTE *>(root_pt_.GetData());
        ASSERT(free_pp_idx_ < PP_NUM);
        ASSERT(memory_.size() == PP_NUM);
        auto &root_pte = pt[vaddr.vpn_1];
        root_pte.InitializeRootPTEIfNecessary(free_pp_idx_, vaddr.vpn_0);
        ASSERT(root_pte.v_);

        auto l2_pt_ppn = GenPPN(root_pte.ppn1_, root_pte.ppn0_);
        PTE *l2_pt = static_cast<PTE *>(GetOrAllocL2PT(l2_pt_ppn)->GetData());

        auto new_page_ppn = GenPPN(free_pp_idx_, 0);
        ASSERT(ppn_to_page_idx_map_.find(new_page_ppn) == ppn_to_page_idx_map_.end());
        l2_pt[vaddr.vpn_0].Initialize(free_pp_idx_, 0);
        ppn_to_page_idx_map_[new_page_ppn] = free_pp_idx_;

        LOG_DEBUG(MMU, "Alloc PP");
        LOG_DEBUG(MMU, "[PPN = " << new_page_ppn  << ", va = " << std::hex << vaddr.ToReg() << std::dec << "]");
        free_pp_idx_++;
        return &memory_[free_pp_idx_ - 1];
    }

    Page *GetPage(VAddr vaddr)
    {
        PTE *root_pt = static_cast<PTE *>(root_pt_.GetData());
        ASSERT(vaddr.vpn_1 < PP_NUM);
        auto root_pte = root_pt[vaddr.vpn_1];
        if (root_pte.v_) {
            auto l2_pt_ppn = GenPPN(root_pte.ppn1_, root_pte.ppn0_);
            PTE *l2_pt = static_cast<PTE *>(GetL2PT(l2_pt_ppn)->GetData());
            PTE l2_pte = l2_pt[vaddr.vpn_0];
            auto ppn = GenPPN(l2_pte.ppn1_, l2_pte.ppn0_);
            auto *p = GetPageFromPPN(ppn);
            if (p != nullptr) {
                LOG_DEBUG(MMU, "Found PP [PPN = " << ppn << ", va=" << std::hex << vaddr.ToReg() << std::dec << "]");
            }
            return p;
        }
        return nullptr;
    }

    Page *GetL2PT(uint32_t ppn)
    {
        if (ppn_to_page_idx_map_.find(ppn) != ppn_to_page_idx_map_.end()) {
            return &memory_[ppn_to_page_idx_map_[ppn]];
        }
        return nullptr;
    }

    Page *GetOrAllocL2PT(uint32_t ppn)
    {
        auto *p = GetL2PT(ppn);
        if (p != nullptr) {
            return p;
        }
        ppn_to_page_idx_map_[ppn] = free_pp_idx_;
        free_pp_idx_++;
        return &memory_[free_pp_idx_ - 1];
    }

    Page *GetPageFromPPN(uint32_t ppn)
    {
        if (ppn_to_page_idx_map_.find(ppn) != ppn_to_page_idx_map_.end()) {
            return &memory_[ppn_to_page_idx_map_[ppn]];
        }
        return nullptr;
    }

    Page *GetRootPT()
    {
        return &root_pt_;
    }

private:
    std::vector<Page> memory_;
    Page root_pt_;
    std::unordered_map<uint32_t, size_t> ppn_to_page_idx_map_;
    size_t free_pp_idx_ = 0;
};

} // namespace sim
