#ifndef SIMULATOR_INTERPRETER_INTERPRETER
#define SIMULATOR_INTERPRETER_INTERPRETER

#include <cstdint>
#include <cstddef>
#include <array>

namespace sim {

class Interpreter {
public:

    // TODO(dkofanov): change this to uint32_t when virtual memory will be implemented:
    // Currently, this should be able to store a native pointer (AMD64):
    using reg_t = uint64_t;

    struct State {
        std::array<reg_t, 31> x_regs {};
        reg_t pc;
    };

public:
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
    const uint8_t *GetPc()
    {
        return reinterpret_cast<const uint8_t *>(state_.pc);
    }
    void SetPc(const uint8_t *v)
    {
        state_.pc = reinterpret_cast<reg_t>(v);
    }
    void AdvancePc(reg_t v)
    {
        state_.pc += v;
    }

    int Invoke(const uint8_t *code, size_t entryp);

private:
    State state_;
};


} // namespace sim

#endif  // SIMULATOR_INTERPRETER_INTERPRETER
