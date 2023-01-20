#pragma once

#include "simulator/memory/memory.h"
#include "simulator/interpreter/interpreter.h"
#include "simulator/memory/mmu.h"
#include "simulator/memory/cache.h"

#include <string>

namespace sim {

class Simulator {

public:
    Simulator(std::string& elfFileName) : loader_(elfFileName), memory_(loader_) {}
    void runSimulation()
    {
        //const auto *code = static_cast<const uint8_t *>(static_cast<const void *>(memory_.getRawMemory()));
        //interp_.Invoke(code, memory_.getEntry());
    }

    void dumpCache()
    {
        cache_.dump();
    }

    Cache* getCache()
    {
        return &cache_;
    }

    mem_t* loadData(PhysAddr address);

private:
    ElfLoader loader_;
    Memory memory_;
    Interpreter interp_;
    MMU mmu_;
    Cache cache_;
};

} // namespace sim
