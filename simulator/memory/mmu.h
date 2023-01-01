#pragma once

#include <unordered_map>


namespace sim {

// 4096 = 2^12
constexpr unsigned PAGE_SIZE = 4096;

// Pontentially addressable only last 22 bits
using PhysAddr = uint32_t;
using VirtAddr = uint64_t;

/* MMU specs:
    m = 2^22 - number of addresses in physical address space
    n = 2^64 - number of addressed in virtual address space
    p = 2^12 - page size
*/

#define VPO_MASK 0xFFF
#define VPN_MASK 0xFFFFFFFFFFFFF000

class MMU {
std::unordered_map<bool, uint64_t> pageTable;
public:
    PhysAddr generatePhysAddr(VirtAddr virtAddr);
    uint64_t getPPN(uint64_t VPN);
};

} // namespace sim
