#ifndef SIMULATOR_INTERPRETER_INTERPRETER
#define SIMULATOR_INTERPRETER_INTERPRETER

#include <cstdint>
#include <cstddef>
#include <array>
#include "simulator/memory/mmu.h"

namespace sim {

class Interpreter {
public:
    using reg_t = uint32_t;

    struct State {
        std::array<reg_t, 31> x_regs {};
        reg_t pc;
    };

public:
    Interpreter(MMU *mmu) : mmu_(mmu) {}

    reg_t GetReg(uint8_t id)
    {
        if (id == 0) {
            return reg_t(0);
        }
        return state_.x_regs[id - 1U];
    }
    void SetReg(uint8_t id, reg_t v)
    {
        if (id != 0) {
            state_.x_regs[id - 1U] = v;
        }
    }
    reg_t GetPc()
    {
        return state_.pc;
    }
    void SetPc(reg_t v)
    {
        state_.pc = v;
    }
    void AdvancePc(reg_t v)
    {
        state_.pc += v;
    }

    int Invoke();

private:
    State state_;
    MMU *mmu_;
};


} // namespace sim

#endif  // SIMULATOR_INTERPRETER_INTERPRETER
