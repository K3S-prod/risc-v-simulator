#pragma once

#include "simulator/memory/mmu.h"
#include "simulator/interpreter/interpreter.h"

#include <string>

namespace sim {

class Simulator {

public:
    Simulator(const std::string& elfFileName) : mmu_(elfFileName), interp_(&mmu_) {}
    void runSimulation()
    {
        interp_.SetPc(mmu_.GetEntryPoint().ToReg());
        interp_.Invoke();
    }

private:
    MMU mmu_;
    Interpreter interp_;
};

} // namespace sim
