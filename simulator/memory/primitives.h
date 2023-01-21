#pragma once

#include "common/macro.h"

namespace sim {

// 4096 = 2^12
constexpr unsigned PAGE_SIZE = 4096;

// 1048576 = 2^20 = 2^(32 - 12) - number of VPNs
constexpr unsigned VPNS_NUM = 1048576;

// Pontentially addressable only last 22 bits
struct PAddr {
    PAddr (uint32_t addr)
    {
        std::memcpy(this, &addr, sizeof(uint32_t));
    }
    // TODO: Change ppn_1 size to 12.
    uint32_t 
        offs : 12,
        ppn_0 : 10,
        ppn_1 : 10;
};
struct VAddr {
    VAddr()
    {
        uint32_t addr = 0;
        std::memcpy(this, &addr, sizeof(uint32_t));
    }
    VAddr(int32_t addr)
    {
        std::memcpy(this, &addr, sizeof(uint32_t));
    }
    VAddr(uint32_t addr)
    {
        std::memcpy(this, &addr, sizeof(uint32_t));
    }
    VAddr(uint64_t addr)
    {
        std::memcpy(this, &addr, sizeof(uint32_t));
    }

    uint32_t ToReg() const
    {
        auto reg = *reinterpret_cast<const uint32_t *>(this);
#ifndef NDEBUG
        uint32_t check_reg = 0;
        check_reg |= vpn_1;
        check_reg <<= 10U;
        check_reg |= vpn_0;
        check_reg <<= 12U;
        check_reg |= offs;
        ASSERT(check_reg == reg);
#endif  // NDEBUG
        return reg;
    }
    uint32_t 
        offs : 12,
        vpn_0 : 10,
        vpn_1 : 10;
};

/* MMU specs:
    m = 2^22 - number of addresses in physical address space
    n = 2^32 - number of addressed in virtual address space
    p = 2^12 - page size
*/

class Page {
public:
    Page()
    {
        data_ = calloc(1U, PAGE_SIZE);
        ASSERT(data_ != nullptr);
    }

    ~Page()
    {
        free(data_);
    }

    void *GetData()
    {
        return data_;
    }

    void Write(uint16_t offs, const void *data, uint16_t size)
    {
        ASSERT(offs < PAGE_SIZE);
        ASSERT(offs + size <= PAGE_SIZE);
        std::memcpy((data_ + offs), data, size);
    }

    void *GetNativePtr(uint16_t offs)
    {
        ASSERT(offs < PAGE_SIZE);
        return data_ + offs;
    }

private:
    void *data_ {nullptr};
};

#define VPN_OFFSET 12
#define VPO_MASK 0xFFF
#define VPN_MASK 0xFFFFFFFFFFFFF000

struct PTE {
public:
    void InitializeRootPTEIfNecessary(uint16_t ppn_1, uint16_t ppn_0)
    {
        if ((ppn1_ != 0) || (ppn0_ != 0)) {
            return;
        }
        // Change this to 12 bits:
        constexpr uint16_t PPN_1_INV_MASK = 0b1111110000000000;
        ASSERT((ppn_1 & PPN_1_INV_MASK) == 0);
        constexpr uint16_t PPN_0_INV_MASK = 0b1111110000000000;
        ASSERT((ppn_0 & PPN_0_INV_MASK) == 0);
        ASSERT(ppn1_  == 0);
        ASSERT(ppn0_  == 0);
        ppn1_ = ppn_1;
        ppn0_ = ppn_0;
        v_ = 1;
        // Pointer to next PT flags combination:
        x_ = 0;
        w_ = 0;
        r_ = 0;
    }

    void Initialize(uint16_t ppn_1, uint16_t ppn_0)
    {
        // Change this to 12 bits:
        constexpr uint16_t PPN_1_INV_MASK = 0b1111110000000000;
        ASSERT((ppn_1 & PPN_1_INV_MASK) == 0);
        constexpr uint16_t PPN_0_INV_MASK = 0b1111110000000000;
        ASSERT((ppn_0 & PPN_0_INV_MASK) == 0);
        ASSERT(ppn1_  == 0);
        ASSERT(ppn0_  == 0);
        ppn1_ = ppn_1;
        ppn0_ = ppn_0;
        v_ = 1;
        // Pointer to next PT flags combination:
        x_ = 1;
        w_ = 1;
        r_ = 1;
    }
public:
    uint32_t
        ppn1_ : 12,
        ppn0_ : 10,
        rsw_ : 2,
        d_ : 1,
        a_ : 1,
        g_ : 1,
        u_ : 1,
        x_ : 1,
        w_ : 1,
        r_ : 1,
        v_ : 1;
};

static_assert(sizeof(PTE) == 4U);
static_assert(sizeof(PTE) * (1U << 10U) == PAGE_SIZE);

} // namespace sim
