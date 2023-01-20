#pragma once

#include <cstdint>
#include <list>
using PhysAddr = uint32_t;
using mem_t = uint8_t;

namespace sim {
    // 2^6 = 64
    constexpr unsigned SIZE_DATA_BLOCK = 8;
    constexpr unsigned NUM_CACHE_LINE_L1 = 2;
    constexpr unsigned NUM_CACHE_LINE_L2 = 4;

    struct CacheLine {
        uint8_t flags;
        PhysAddr address;
        mem_t data[SIZE_DATA_BLOCK];
    };

    class Cache {
    public:
        Cache();
        ~Cache();
        void dump();

        PhysAddr getAlignedAddressCacheLine(PhysAddr address) {
            return address & ~(SIZE_DATA_BLOCK - 1);
        }

        mem_t* findCacheLineInCache(PhysAddr address);
        mem_t* fillCacheFromMemory(PhysAddr aligned_address, char* pointer_phys_addr);

        const CacheLine* getCacheLine(uint8_t cache_level, uint32_t cache_line);
    private:
        CacheLine* L1_cache_;
        CacheLine* L2_cache_;
        std::list<uint32_t> L1_queue_;
        std::list<uint32_t> L2_queue_;

        uint32_t findFreeCacheL1();
        uint32_t findFreeCacheL2();
    };
}
