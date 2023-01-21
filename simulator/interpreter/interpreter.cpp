#include "interpreter.h"
#include "simulator/decoder/decoder.h"
#include "simulator/simulator.h"

#include <cstdint>
#include <array>
#include <iomanip>

namespace sim {

#define R() decoder.r
#define I() decoder.i
#define S() decoder.s
#define U() decoder.u
#define B() decoder.b
#define U() decoder.u
#define J() decoder.j
#define A() decoder.a
    

#define LOAD_FROM_MEMORY(addr) \
{                                                                                       \
    VAddr va = addr;                                                                    \
    auto *page = mmu_->GetPage(va);                                                     \
    loaded = *static_cast<const uint32_t *>(page->GetNativePtr(va.offs));               \
}

#define STORE_TO_MEMORY(addr, value) \
{                                                                                       \
    VAddr va = addr;                                                                    \
    auto *page = mmu_->GetPage(va);                                                     \
    auto native_addr = static_cast<uint32_t *>(page->GetNativePtr(va.offs));            \
    *native_addr = value;                                                               \
}

#define SIGNED(value) static_cast<int32_t>(value)
#define UNSIGNED(value) static_cast<uint32_t>(value)
#define GETPC() GetPc()
#define SETPC(value) SetPc(value)

#define GET_J_IMM(imm) \
{                                                                       \
    for (int i = 20; i < 32; ++i) {                                     \
        imm |= J().imm_20 << i;                                         \
    }                                                                   \
    imm |= J().imm_19_12 << 12 | J().imm_11 << 11 | J().imm_10_1 << 1;  \
}

#define GET_B_IMM(imm) \
{                                                                       \
    for (int i = 12; i < 32; ++i) {                                     \
        imm |= B().imm_12 << i;                                         \
    }                                                                   \
    imm |= B().imm_4_1 << 1 | B().imm_10_5 << 5 | B().imm_11 << 11;     \
}

#define GET_I_IMM(imm) \
{                                                                       \
    imm = I().imm_11_0;                                                 \
    uint32_t sign_bit = imm >> 11;                                      \
    for (int i = 12; i < 32; ++i) {                                     \
        imm |= sign_bit << i;                                           \
    }                                                                   \
}

#define GET_S_IMM(imm) \
{                                                                       \
    imm = S().imm_4_0 | S().imm_11_5 << 5;                              \
    uint32_t sign_bit = imm >> 11;                                      \
    for (int i = 12; i < 32; ++i) {                                     \
        imm |= sign_bit << i;                                           \
    }                                                                   \
}

#define DECL_DISPATCH_IDX() \
    VAddr va = GetPc();                                                                 \
    auto *page = mmu_->GetPage(va);                                                     \
    const auto *native_ptr = static_cast<const uint8_t *>(page->GetNativePtr(va.offs)); \
    size_t dispatch_idx = static_cast<size_t>(decoder.Decode(native_ptr));

Interpreter::Interpreter(Simulator *runtime, MMU *mmu) : mmu_(mmu), runtime_(runtime)
{
    // Declare No-breaking invoke type:
    #define DISPATCH() \
    {                                        \
        DECL_DISPATCH_IDX();                 \
        goto *DISPATCH_TABLE[dispatch_idx];  \
    }

    no_breaking_dispatch_ = 
    #include "interpreter_handlers.inl"
    #undef DISPATCH

    // Declare invoke type with breakpoints supported:
    #define DISPATCH() \
    {                                               \
        if (runtime_->IsBreakpoint(GetPc()) &&      \
            (num_exected_insts_ != 0)) {            \
            /* Pass control to the runtime: */      \
            return;                                 \
        }                                           \
        if (num_insts_to_exec_ == 0) {              \
            /* Pass control to the runtime: */      \
            return;                                 \
        }                                           \
        if (num_insts_to_exec_ != ((size_t) - 1)) { \
            num_insts_to_exec_--;                   \
        }                                           \
        num_exected_insts_++;                       \
        DECL_DISPATCH_IDX();                        \
        goto *DISPATCH_TABLE[dispatch_idx];         \
    }
    breakpointed_dispatch_ = 
    #include "interpreter_handlers.inl"
    #undef DISPATCH
}

#undef DISPATCH
#undef DISPATCH_IDX

} // namespace sim
