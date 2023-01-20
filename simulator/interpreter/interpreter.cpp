#include "interpreter.h"
#include "simulator/decoder/decoder.h"

#include <cstdint>
#include <array>

namespace sim {

#define R() decoder.r
#define I() decoder.i
#define S() decoder.s
#define U() decoder.u
#define B() decoder.b
#define U() decoder.u
#define J() decoder.j
#define A() decoder.a
    
#define DISPATCH() \
{                                                                                       \
    VAddr va = GetPc();                                                                 \
    auto *page = mmu_->GetPage(va);                                                      \
    const auto *native_ptr = static_cast<const uint8_t *>(page->GetNativePtr(va.offs));  \
    size_t dispatch_idx = static_cast<size_t>(decoder.Decode(native_ptr));  \
    goto *DISPATCH_TABLE[dispatch_idx];                                 \
}

int Interpreter::Invoke()
{
    Decoder decoder;

#include <simulator/interpreter/generated/dispatch_table.inl>

    // Begin execution:
    DISPATCH();

    U_LUI: {
        LOG_DEBUG(INTERPRETER, U().Dump("LUI"));
        AdvancePc(sizeof(Decoder::U));
        DISPATCH();
    }
    
    U_AUIPC: {
        LOG_DEBUG(INTERPRETER, U().Dump("AUIPC"));
        AdvancePc(sizeof(Decoder::U));
        DISPATCH();
    }
    
    J_JAL: {
        LOG_DEBUG(INTERPRETER, J().Dump("JAL"));
        AdvancePc(sizeof(Decoder::J));
        DISPATCH();
    }
    
    I_JALR: {
        LOG_DEBUG(INTERPRETER, I().Dump("JALR"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    B_BEQ: {
        LOG_DEBUG(INTERPRETER, B().Dump("BEQ"));
        AdvancePc(sizeof(Decoder::B));
        DISPATCH();
    }
    
    B_BNE: {
        LOG_DEBUG(INTERPRETER, B().Dump("BNE"));
        AdvancePc(sizeof(Decoder::B));
        DISPATCH();
    }
    
    B_BLT: {
        LOG_DEBUG(INTERPRETER, B().Dump("BLT"));
        AdvancePc(sizeof(Decoder::B));
        DISPATCH();
    }
    
    B_BGE: {
        LOG_DEBUG(INTERPRETER, B().Dump("BGE"));
        AdvancePc(sizeof(Decoder::B));
        DISPATCH();
    }
    
    B_BLTU: {
        LOG_DEBUG(INTERPRETER, B().Dump("BLTU"));
        AdvancePc(sizeof(Decoder::B));
        DISPATCH();
    }
    
    B_BGEU: {
        LOG_DEBUG(INTERPRETER, B().Dump("BGEU"));
        AdvancePc(sizeof(Decoder::B));
        DISPATCH();
    }
    
    I_LB: {
        LOG_DEBUG(INTERPRETER, I().Dump("LB"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_LH: {
        LOG_DEBUG(INTERPRETER, I().Dump("LH"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_LW: {
        LOG_DEBUG(INTERPRETER, I().Dump("LW"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_LBU: {
        LOG_DEBUG(INTERPRETER, I().Dump("LBU"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_LHU: {
        LOG_DEBUG(INTERPRETER, I().Dump("LHU"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    S_SB: {
        LOG_DEBUG(INTERPRETER, S().Dump("SB"));
        AdvancePc(sizeof(Decoder::S));
        DISPATCH();
    }
    
    S_SH: {
        LOG_DEBUG(INTERPRETER, S().Dump("SH"));
        AdvancePc(sizeof(Decoder::S));
        DISPATCH();
    }
    
    S_SW: {
        LOG_DEBUG(INTERPRETER, S().Dump("SW"));
        AdvancePc(sizeof(Decoder::S));
        DISPATCH();
    }
    
    I_ADDI: {
        LOG_DEBUG(INTERPRETER, I().Dump("ADDI"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_SLTI: {
        LOG_DEBUG(INTERPRETER, I().Dump("SLTI"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_SLTIU: {
        LOG_DEBUG(INTERPRETER, I().Dump("SLTIU"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_XORI: {
        LOG_DEBUG(INTERPRETER, I().Dump("XORI"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_ORI: {
        LOG_DEBUG(INTERPRETER, I().Dump("ORI"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_ANDI: {
        LOG_DEBUG(INTERPRETER, I().Dump("ANDI"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    R_SLLI: {
        LOG_DEBUG(INTERPRETER, R().Dump("SLLI"));
        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SRLI: {
        LOG_DEBUG(INTERPRETER, R().Dump("SRLI"));
        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SRAI: {
        LOG_DEBUG(INTERPRETER, R().Dump("SRAI"));
        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_ADD: {
        LOG_DEBUG(INTERPRETER, R().Dump("ADD"));
        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SUB: {
        LOG_DEBUG(INTERPRETER, R().Dump("SUB"));
        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SLL: {
        LOG_DEBUG(INTERPRETER, R().Dump("SLL"));
        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SLT: {
        LOG_DEBUG(INTERPRETER, R().Dump("SLT"));
        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SLTU: {
        LOG_DEBUG(INTERPRETER, R().Dump("SLTU"));
        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_XOR: {
        LOG_DEBUG(INTERPRETER, R().Dump("XOR"));
        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SRL: {
        LOG_DEBUG(INTERPRETER, R().Dump("SRL"));
        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SRA: {
        LOG_DEBUG(INTERPRETER, R().Dump("SRA"));
        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_OR: {
        LOG_DEBUG(INTERPRETER, R().Dump("OR"));
        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_AND: {
        LOG_DEBUG(INTERPRETER, R().Dump("AND"));
        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    I_FENCE: {
        LOG_DEBUG(INTERPRETER, I().Dump("FENCE"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_FENCE_I: {
        LOG_DEBUG(INTERPRETER, I().Dump("FENCE_I"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_ECALL: {
        LOG_DEBUG(INTERPRETER, I().Dump("ECALL"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_EBREAK: {
        LOG_DEBUG(INTERPRETER, I().Dump("EBREAK"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_CSRRW: {
        LOG_DEBUG(INTERPRETER, I().Dump("CSRRW"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_CSRRS: {
        LOG_DEBUG(INTERPRETER, I().Dump("CSRRS"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_CSRRC: {
        LOG_DEBUG(INTERPRETER, I().Dump("CSRRC"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_CSRRWI: {
        LOG_DEBUG(INTERPRETER, I().Dump("CSRRWI"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_CSRRSI: {
        LOG_DEBUG(INTERPRETER, I().Dump("CSRRSI"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_CSRRCI: {
        LOG_DEBUG(INTERPRETER, I().Dump("CSRRCI"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }

    A_LR_W: {
        LOG_DEBUG(INTERPRETER, A().Dump("LR_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_SC_W: {
        LOG_DEBUG(INTERPRETER, A().Dump("SC_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOSWAP_W: {
        LOG_DEBUG(INTERPRETER, A().Dump("AMOSWAP_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOADD_W: {
        LOG_DEBUG(INTERPRETER, A().Dump("AMOADD_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOXOR_W: {
        LOG_DEBUG(INTERPRETER, A().Dump("AMOXOR_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOAND_W: {
        LOG_DEBUG(INTERPRETER, A().Dump("AMOAND_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOOR_W: {
        LOG_DEBUG(INTERPRETER, A().Dump("AMOOR_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOMIN_W: {
        LOG_DEBUG(INTERPRETER, A().Dump("AMOMIN_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOMAX_W: {
        LOG_DEBUG(INTERPRETER, A().Dump("AMOMAX_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOMINU_W: {
        LOG_DEBUG(INTERPRETER, A().Dump("AMOMINU_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOMAXU_W: {
        LOG_DEBUG(INTERPRETER, A().Dump("AMOMAXU_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    UNREACHABLE();
}

} // namespace sim
