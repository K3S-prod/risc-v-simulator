#pragma once

#include <string>

#include "cpu.h"
#include "sim_memory.h"
#include "instruction.h"

namespace sim {

class Simulator {
public:
    Simulator() = default;


    Memory& getMemory() {
        return memory_;
    }

    int loadELF(std::string& elfFileName);
    void runSimulation();

private:
    void FetchAndExecude();
    std::string elfFileName_;
    sim::Cpu cpu_;
    sim::Memory memory_;
};

} // namespace sim
