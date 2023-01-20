#include "cache.h"
#include <assert.h>
#include <cstring>
#include <string>
#include <iostream>

namespace sim {

Cache::Cache():
    L1_cache_(new CacheLine[NUM_CACHE_LINE_L1]),
    L2_cache_(new CacheLine[NUM_CACHE_LINE_L2]) {}

Cache::~Cache() {
    delete [] L1_cache_;
    delete [] L2_cache_;
}

mem_t* Cache::findCacheLineInCache(PhysAddr address) {
    assert(address % SIZE_DATA_BLOCK == 0);

    for (int i = 0; i < NUM_CACHE_LINE_L1; i++) {
        if (L1_cache_[i].address == address)
            return L1_cache_[i].data;
    }

    for (int i = 0; i < NUM_CACHE_LINE_L2; i++) {
        if (L2_cache_[i].address == address) {
            uint32_t free_line = 0;
            if (L1_queue_.size() == NUM_CACHE_LINE_L1) {
                free_line = L1_queue_.back();
                L1_queue_.pop_back();
                L1_queue_.push_front(free_line);
            } else {
                assert(L1_queue_.size() < NUM_CACHE_LINE_L1);

                free_line = L1_queue_.size();
                L1_queue_.push_front(free_line);
            }
            memcpy(&L1_cache_[free_line].data, &L2_cache_[i].data, SIZE_DATA_BLOCK);
            return L1_cache_[free_line].data;
        }
    }
    return nullptr;
}

mem_t* Cache::fillCacheFromMemory(PhysAddr aligned_address, char* pointer_phys_addr) {
    uint32_t index_l2 = findFreeCacheL2();
    memcpy(&L2_cache_[index_l2].data, pointer_phys_addr, SIZE_DATA_BLOCK);
    L2_cache_[index_l2].address = aligned_address;

    uint32_t index_l1 = findFreeCacheL1();
    memcpy(&L1_cache_[index_l1].data, &L2_cache_[index_l2].data, SIZE_DATA_BLOCK);
    L1_cache_[index_l1].address = aligned_address;

    return L1_cache_[index_l1].data;
}

#define DECLARE_FIND_FREE_CACHE(name)                           \
uint32_t Cache::findFreeCache##name() {                         \
    uint32_t free_line = 0;                                     \
    if (name##_queue_.size() == NUM_CACHE_LINE_##name) {        \
        free_line = name##_queue_.back();                       \
        name##_queue_.pop_back();                               \
        name##_queue_.push_front(free_line);                    \
    } else {                                                    \
        assert(name##_queue_.size() < NUM_CACHE_LINE_##name);   \
                                                                \
        free_line = name##_queue_.size();                       \
        name##_queue_.push_front(free_line);                    \
    }                                                           \
    return free_line;                                           \
}

DECLARE_FIND_FREE_CACHE(L1)
DECLARE_FIND_FREE_CACHE(L2)

#undef DECLARE_FIND_FREE_CACHE

void Cache::dump() {
    fprintf(stderr, "-----------------------\nCache L1:\n");
    for (int i = 0; i < NUM_CACHE_LINE_L1; i++) {
        fprintf(stderr, "Line %d |Flags [%d] Address [0x%x] Data:\n[", i, (uint32_t)L1_cache_[i].flags, L1_cache_[i].address);
        uint8_t counter = 0;
        for (int j = 0; j < SIZE_DATA_BLOCK; j++) {
            fprintf(stderr, "%3x", L1_cache_[i].data[j] & 0xff);
            counter++;
        }
        fprintf(stderr, "]\n");
    }

    fprintf(stderr, "\nCache L2:\n");
    for (int i = 0; i < NUM_CACHE_LINE_L2; i++) {
        fprintf(stderr, "Line %d |Flags [%d] Address [0x%x] Data:\n[", i, (uint32_t)L2_cache_[i].flags, L2_cache_[i].address);
        uint8_t counter = 0;
        for (int j = 0; j < SIZE_DATA_BLOCK; j++) {
            fprintf(stderr, "%3x", L2_cache_[i].data[j] & 0xff);
            counter++;
        }
        fprintf(stderr, "]\n");
    }
    fprintf(stderr, "-----------------------\n\n");
}

const CacheLine* Cache::getCacheLine(uint8_t cache_level, uint32_t cache_line){
    if (cache_level == 1) {
        assert(cache_line < NUM_CACHE_LINE_L1);
        return &L1_cache_[cache_line];
    } else if (cache_level == 2) {
        assert(cache_line < NUM_CACHE_LINE_L2);
        return &L2_cache_[cache_line];
    } else {
        assert(cache_level == 1 || cache_level == 2);
        return nullptr;
    }
}

}
