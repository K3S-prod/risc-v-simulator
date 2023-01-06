
#include <iostream>
#include "mmu.h"

namespace sim {

uint64_t MMU::getPPN(uint64_t VPN) {
    auto hit = pageTable.find(VPN);
    // Not a page fault. Page fault occurs only if PhysAddr computed does not 
    if (hit == pageTable.end()) {
        return 0;
    }
    return 0;
}

PhysAddr MMU::generatePhysAddr(VirtAddr virtAddr) {
    uint64_t VPO = virtAddr & VPO_MASK;
    uint64_t VPN = virtAddr & VPN_MASK;

    std::cout << "INFO: VPO = " << std::hex << VPO << std::endl;
    std::cout << "INFO: VPN = " << std::hex << VPN << std::endl;
    uint64_t PPO = VPO;
    // WARNING: No PPN from page table is generated. For now we just cut off higher part
    //          of the address in order to map virtual memory to our physical memory in Memory.cpp
    // uint64_t PPN = getPPN(VPN);
    PhysAddr PPN = 0x0;
    return DATA_OFFSET + static_cast<PhysAddr>(PPN | PPO);
}

} // namespace sim
