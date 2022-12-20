

#include <array>
#include <cstddef>
#include <string>
#include <iostream>
#include <elfio/elfio.hpp>
#include <elf.h>

#include "sim_memory.h"

namespace sim {

    void Memory::dump() {
        std::cout << "Memory offset: " << std::dec << size_ << std::endl;
        std::cout << "Dumping memory from address 0x0..." << std::endl;

        std::ofstream dumpFile("memory_dump.sim");
        if (dumpFile.is_open()) {
            dumpFile.write(addrSpace_, size_);
            dumpFile.close();
        }
        std::cout << "Done." << std::endl;
    }

    void Memory::upload(size_t offset, const char* data, size_t len) {
        memcpy(addrSpace_ + offset, data, len);
    }

    unsigned Memory::getAccessRegion(size_t offset) {
        unsigned regionID = 0;
        for (const auto& regionBorders: memoryRegions) {
            auto&[left, right] = regionBorders.second;
            regionID = regionBorders.first;
            if (offset >= left && offset <= right) {
                return regionID;
            }
        }
        std::cerr << "ERROR: memory region with offset" << offset << "not found" << std::endl;
        throw std::runtime_error("Memory region initialization failure");
    }

    void Memory::accessMemory(size_t offset, size_t nbytes, enum accessModes accessMode) {
        auto regionID = getAccessRegion(offset);
        auto segmentPermissions = regionPermissions.at(regionID);
        auto nextRegionID = getAccessRegion(offset + nbytes);
        if (regionID != nextRegionID) {
            throw std::runtime_error("ERROR: memory access range overflows segment");
        }
        if (offset + nbytes > DRAM_SIZE) {
            throw std::runtime_error("ERROR: invalid memory access range");

        }
        if (accessMode == accessModes::READ) {
            if (segmentPermissions != PF_R) {
                throw std::runtime_error("ERROR: accessing memory: permission denied");
            }
        } else if (accessMode == accessModes::WRITE) {
            if (segmentPermissions != PF_W) {
                throw std::runtime_error("ERROR: accessing memory: permission denied");
            }
        }
    }

} // namespace sim
