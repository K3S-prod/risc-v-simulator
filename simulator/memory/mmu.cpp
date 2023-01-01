
#include "mmu.h"

namespace sim {

uint64_t MMU::getPPN(uint64_t VPN) {
    return 0;
}

PhysAddr MMU::generatePhysAddr(VirtAddr virtAddr) {
    uint64_t VPO = virtAddr & VPO_MASK;
    uint64_t VPN = virtAddr & VPN_MASK;
    uint64_t PPO = VPO;
    uint64_t PPN = getPPN(VPN);
    return static_cast<PhysAddr>(PPN | PPO);
}

} // namespace sim
