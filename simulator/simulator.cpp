#include "simulator.h"

namespace sim {

mem_t* Simulator::loadData(PhysAddr address) {
    PhysAddr aligned_address = cache_.getAlignedAddressCacheLine(address);
    mem_t* pointer_cache_line = cache_.findCacheLineInCache(aligned_address);

    if (pointer_cache_line != nullptr) {
        return pointer_cache_line + (address - aligned_address);
    }

    return cache_.fillCacheFromMemory(aligned_address, memory_.getPointerPhysAddr(aligned_address)) +
           (address - aligned_address);
}

}
