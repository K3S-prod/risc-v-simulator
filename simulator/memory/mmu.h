#pragma once

#include <unordered_map>


namespace sim {

// 4096 = 2^12
constexpr unsigned PAGE_SIZE = 4096;

// 1048576 = 2^20 = 2^(32 - 12) - number of VPNs
constexpr unsigned VPNS_NUM = 1048576;

// Pontentially addressable only last 22 bits
using PhysAddr = uint32_t;
using VirtAddr = uint32_t;

/* MMU specs:
    m = 2^22 - number of addresses in physical address space
    n = 2^32 - number of addressed in virtual address space
    p = 2^12 - page size
*/

#define VPN_OFFSET 12
#define VPO_MASK 0xFFF
#define VPN_MASK 0xFFFFFFFFFFFFF000

class PTE {
    bool valid_;
    uint64_t PPN_;
public:
    PTE(uint64_t accessCount, bool valid, uint64_t PPN): valid_(valid),
                                                         PPN_(PPN) {};
    bool isValid() {
        return valid_;
    }

    uint64_t getPPN() {
        return PPN_;
    }

    void setPPN(uint64_t PPN) {
        PPN_ = PPN;
    }

    void makeValid() {
        valid_ = true;
    }

};

class MMU {
std::unordered_map<uint64_t, PTE> pageTable_;
uint64_t freePPN = 0;

public:
    MMU();
    PhysAddr generatePhysAddr(VirtAddr virtAddr);
    uint64_t getPPN(uint64_t VPN);
};

} // namespace sim
