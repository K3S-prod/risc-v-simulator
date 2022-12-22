#pragma once

#include <string>

#include "cpu.h"
#include "memory.h"

namespace sim {

class Simulator {
    sim::ElfLoader loader_;
    sim::Cpu cpu_;
    sim::Memory memory_;

public:
    Simulator(std::string& elfFileName);
    void runSimulation();
};

} // namespace sim
