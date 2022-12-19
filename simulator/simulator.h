#pragma once

#include <string>

#include "cpu.h"
#include "sim_memory.h"

namespace sim {

class Simulator {
public:
    Simulator() = default;

    Memory& getMemory() {
        return memory_;
    }

    int loadELF(std::string& elfFileName);

private:
    std::string elfFileName_;
    sim::Cpu cpu_;
    sim::Memory memory_;
    void runSimulation();
};

} // namespace sim
