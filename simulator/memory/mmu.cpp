
#include <iostream>
#include "mmu.h"

namespace sim {

MMU::MMU() {
    for (uint64_t vpnNum = 0; vpnNum < VPNS_NUM; ++vpnNum) {
        pageTable_.emplace(vpnNum, PTE(0, 0, vpnNum));
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

} // namespace sim
