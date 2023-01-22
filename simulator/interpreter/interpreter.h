#ifndef SIMULATOR_INTERPRETER_INTERPRETER
#define SIMULATOR_INTERPRETER_INTERPRETER

#include "register.h"
#include "simulator/memory/mmu.h"

#include <cstdint>
#include <cstddef>
#include <array>

namespace sim {
class Simulator;
class Interpreter {
public:
    struct State {
        std::array<Register, 31> x_regs {};
        Register pc;
    };

public:
    Interpreter(Simulator *runtime, MMU *mmu);

    Register GetReg(uint8_t id)
    {
        if (id == 0) {
            return Register(0);
        }
        return state_.x_regs[id - 1U];
    }
    void SetReg(uint8_t id, Register v)
    {
        if (id != 0) {
            state_.x_regs[id - 1U] = v;
        }
    }
    Register GetPc()
    {
        return state_.pc;
    }
    void SetPc(Register v)
    {
        state_.pc = v;
    }
    void AdvancePc(Register v)
    {
        state_.pc += v;
    }

    int Invoke()
    {
        num_executed_insts_ = 0;
        no_breaking_dispatch_();
        return 0;
    }

    void InvokeBreakpointed()
    {
        num_executed_insts_ = 0;
        breakpointed_dispatch_();
    }

    void SetNumInstsToExec(size_t n)
    {
        num_insts_to_exec_ = n;
    }
    size_t GetNumInstsToExec()
    {
        return num_insts_to_exec_;
    }

    void SaveSP()
    {
        saved_sp_ = state_.x_regs[1];
    }
    void ClearSavedSP()
    {
        saved_sp_ = 0;
    }


private:
    State state_{};
    MMU *mmu_{};
    std::function<void()> no_breaking_dispatch_;
    std::function<void()> breakpointed_dispatch_;
    Simulator *runtime_{};

    // Exec-controll members (breakpoints, trackers, etc.):
    size_t num_insts_to_exec_{};
    size_t num_executed_insts_{};
    VAddr saved_sp_{};
};


} // namespace sim

#endif  // SIMULATOR_INTERPRETER_INTERPRETER
