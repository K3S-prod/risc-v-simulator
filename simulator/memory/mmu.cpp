
#include <iostream>
#include "mmu.h"

namespace sim {

MMU::MMU(const std::string& elfFileName)
{
    ElfLoader loader(elfFileName);

    LoadElfData(loader);
    entry_ = loader.GetMainEntryPoint();
}

void MMU::LoadElfData(const ElfLoader &loader)
{
    const auto &elf_file = loader.GetElfFile();

    size_t size = 0;
    for (const auto& segment: elf_file.segments) {
        if (segment->get_type() == ELFIO::PT_LOAD) {
            auto seg_addr = segment->get_virtual_address();
            auto seg_size = segment->get_memory_size();
            auto seg_data = segment->get_data();
            VAddr va(seg_addr);
            LOG_DEBUG(MMU, "loading segment at address " << seg_addr << ", size: " << seg_size << std::endl);
            auto page = GetOrAllocPage(va);
            uint32_t remainder = 0;
            if ((va.offs + seg_size) > PAGE_SIZE) {
                remainder = (va.offs + seg_size) - PAGE_SIZE;
            }
            auto real_write_size = seg_size - remainder;
            page->Write(va.offs, seg_data, real_write_size);
            if (remainder != 0) {
                auto seg_addr_2 = seg_addr + real_write_size;
                auto seg_data_2 = seg_data + real_write_size;
                VAddr va_2(seg_addr_2);
                auto page_2 = GetOrAllocPage(va_2);
                ASSERT(remainder < PAGE_SIZE);
                ASSERT(va_2.offs == 0);
                page->Write(va_2.offs, seg_data_2, remainder);
            }
            size += static_cast<size_t>(seg_size);
        }
    }
}

// uint64_t MMU::getFirstLeastUsed() {
//     uint64_t maxAccessCount = 10000;
//     uint64_t VPN = 0x0;
//     for (auto& page: pageTable_) {
//         PTE pte = page.second;
//         if (pte.getAccessCount() < maxAccessCount) {
//             maxAccessCount = pte.getAccessCount();
//             VPN = page.first;
//         }
//     }
//     return VPN;
// }

// void MMU::handlePageFault(uint64_t VPN) {
//     // Replace entry with 'false' valid field to entry with 'true' valid field.
//     // Find recently used page and evict it from memory
//     auto leastUsedVPN = getFirstLeastUsed();
//     pageTable_.erase(leastUsedVPN);
//     pageTable_.emplace(leastUsedVPN);
// }
/*
uint64_t MMU::getPPN(uint64_t VPN) {
    auto hit = pageTable_.find(VPN);

    if (hit == pageTable_.end()) {
        std::cerr << "ERROR: No VPN " << VPN << "found in page table" << std::endl;
        return 0;
    }

    auto PTE = hit->second;
    if (!PTE.isValid()) {
        std::cout << "VPN " << VPN << " marked as invalid" << std::endl;
        PTE.makeValid();
        PTE.setPPN(freePPN);
        std::cout << "VPN " << VPN << " validated." << " PPN set: " << freePPN << std::endl;
        freePPN++;
    }

    std::cout << "INFO: VPN " << VPN << " found with PPN " << PTE.getPPN() << std::endl;
    return PTE.getPPN();  
}

PhysAddr MMU::generatePhysAddr(VirtAddr virtAddr) {
    uint64_t VPO = virtAddr & VPO_MASK;
    uint64_t VPN = (virtAddr & VPN_MASK) >> VPN_OFFSET;

    std::cout << "INFO: VPO = " << std::hex << VPO << std::endl;
    std::cout << "INFO: VPN = " << std::hex << VPN << std::endl;
    uint64_t PPO = VPO;
    PhysAddr PPN = getPPN(VPN);
    return static_cast<PhysAddr>((  PPN << VPN_OFFSET) | PPO);
}
*/
} // namespace sim
