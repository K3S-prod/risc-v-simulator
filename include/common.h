#pragma once

#include <string>

#include "cpu.h"
#include "sim_memory.h"

namespace sim {

class State {
    std::string elfFileName;
    sim::CPU cpu;
    sim::Memory memory;
    void runSimulation();
public:
    State() = default;
    
    Memory& getMemory() {
        return memory;
    }

    int loadELF(std::string& elfFileName);
};

} // namespace sim
