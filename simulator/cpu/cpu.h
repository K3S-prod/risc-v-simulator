#pragma once

#include <cstdint>
#include <array>

namespace sim {

constexpr int NUM_REGS = 32;

class Cpu {
public:
    Cpu() = default;
    void runPipeline();

    uint64_t GetPC() {
        return pc_;
    }

    void SetPC(uint64_t pos) {
        pc_ = pos;
    }

    void MovePC(int64_t offset) {
        // TODO add checks
        pc_ += offset;
    }

    // Temporary contains only one instruction
    uint32_t cache_;
private:
    uint64_t pc_;

    // Simulator work with RISC-V which have 64-bit registers
    using Register = uint64_t;
    // RISC-V has 32 integer register
    std::array<Register, NUM_REGS> regs_;
};

} // namespace sim
