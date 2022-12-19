#pragma once

#include <cstdint>
#include <array>

namespace sim {

constexpr int NUM_REGS = 32;

class Cpu {
public:
    Cpu() = default;
    void runPipeline();
    void RunInterpreter();

private:
    uint64_t pc_;

    // Simulator work with RISC-V which have 64-bit registers
    using Register = uint64_t;
    // RISC-V has 32 integer register
    std::array<Register, NUM_REGS> regs_;
};

} // namespace sim
