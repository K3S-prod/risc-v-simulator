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
{                                                                       \
    size_t dispatch_idx = static_cast<size_t>(decoder.Decode(GetPc())); \
    goto *DISPATCH_TABLE[dispatch_idx];                                 \
}

#define SIGNED(value) static_cast<int32_t>(value)
#define UNSIGNED(value) static_cast<uint32_t>(value)
#define GETPC() reinterpret_cast<uint64_t>(GetPc())
#define SETPC(value) SetPc(reinterpret_cast<uint8_t*>(value))

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

int Interpreter::Invoke(const uint8_t *code, size_t recalc_entryp_offset)
{
    // TODO(dkofanov): This should be changed to `SetPc(entrypoint)` when MMU is implemented:
    SetPc(code + recalc_entryp_offset);
    Decoder decoder;

#include <simulator/interpreter/generated/dispatch_table.inl>

    // Begin execution:
    DISPATCH();

    U_LUI: {
        LOG_DEBUG(INTERPRETER, U().Dump("LUI"));

        auto rd = U().rd;
        auto imm = U().imm_31_12;
        imm <<= 12;

        SetReg(rd, imm);

        AdvancePc(sizeof(Decoder::U));
        DISPATCH();
    }
    
    U_AUIPC: {
        LOG_DEBUG(INTERPRETER, U().Dump("AUIPC"));

        auto rd = U().rd;
        auto imm = U().imm_31_12;
        auto pc = GETPC();
        
        imm <<= 12;
        SetReg(rd, imm + pc);

        AdvancePc(sizeof(Decoder::U));
        DISPATCH();
    }
    
    J_JAL: {
        LOG_DEBUG(INTERPRETER, J().Dump("JAL"));

        auto rd = J().rd;
        uint32_t imm = 0;
        GET_J_IMM(imm);
        auto pc = GETPC();

        if (rd != 0) {
            SetReg(rd, pc + 4U);
        }
        SETPC(pc + SIGNED(imm));

        //AdvancePc(sizeof(Decoder::J));
        DISPATCH();
    }
    
    I_JALR: {
        LOG_DEBUG(INTERPRETER, I().Dump("JALR"));

        auto rd = I().rd;
        auto rs1 = I().rs1;
        auto imm = 0;
        GET_I_IMM(imm);
        auto pc = GETPC();

        if (rd != 0) {
            SetReg(rd, pc + 4U);
        }
        SETPC(SIGNED(GetReg(rs1)) + SIGNED(imm));

        /* TODO: check if we need this
        auto new_pc = GETPC();
        SETPC(new_pc & 0xFFFFFFFE);
        */

        //AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    B_BEQ: {
        LOG_DEBUG(INTERPRETER, B().Dump("BEQ"));

        auto rs1 = B().rs1;
        auto rs2 = B().rs2;
        auto pc = GETPC();
        uint32_t imm = 0;
        GET_B_IMM(imm);

        if (rs1 == rs2) {
            SETPC(pc + SIGNED(imm));
        }
        
        //AdvancePc(sizeof(Decoder::B));
        DISPATCH();
    }
    
    B_BNE: {
        LOG_DEBUG(INTERPRETER, B().Dump("BNE"));

        auto rs1 = B().rs1;
        auto rs2 = B().rs2;
        auto pc = GETPC();
        uint32_t imm = 0;
        GET_B_IMM(imm);

        if (rs1 != rs2) {
            SETPC(pc + SIGNED(imm));
        }

        AdvancePc(sizeof(Decoder::B));
        DISPATCH();
    }
    
    B_BLT: {
        LOG_DEBUG(INTERPRETER, B().Dump("BLT"));

        auto rs1 = B().rs1;
        auto rs2 = B().rs2;
        auto pc = GETPC();
        uint32_t imm = 0;
        GET_B_IMM(imm);

        if (rs1 < rs2) {
            SETPC(pc + SIGNED(imm));
        }

        AdvancePc(sizeof(Decoder::B));
        DISPATCH();
    }
    
    B_BGE: {
        LOG_DEBUG(INTERPRETER, B().Dump("BGE"));

        auto rs1 = B().rs1;
        auto rs2 = B().rs2;
        auto pc = GETPC();
        uint32_t imm = 0;
        GET_B_IMM(imm);

        if (rs1 >= rs2) {
            SETPC(pc + imm);
        }

        AdvancePc(sizeof(Decoder::B));
        DISPATCH();
    }
    
    B_BLTU: {
        // TODO: FIXME this one should operate with unsigned rs1 and rs2 values
        LOG_DEBUG(INTERPRETER, B().Dump("BLTU"));

        auto rs1 = B().rs1;
        auto rs2 = B().rs2;
        auto pc = GETPC();
        uint32_t imm = 0;
        GET_B_IMM(imm);

        if (rs1 < rs2) {
            SETPC(pc + imm);
        }

        AdvancePc(sizeof(Decoder::B));
        DISPATCH();
    }
    
    B_BGEU: {
        // TODO: FIXME this one should operate with unsigned rs1 and rs2 values
        LOG_DEBUG(INTERPRETER, B().Dump("BGEU"));

        auto rs1 = B().rs1;
        auto rs2 = B().rs2;
        auto pc = GETPC();
        uint32_t imm = 0;
        GET_B_IMM(imm);
        
        if (rs1 >= rs2) {
            SETPC(pc + imm);
        }

        AdvancePc(sizeof(Decoder::B));
        DISPATCH();
    }
    
    I_LB: {
        LOG_DEBUG(INTERPRETER, I().Dump("LB"));

        auto imm = 0;
        GET_I_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_LH: {
        LOG_DEBUG(INTERPRETER, I().Dump("LH"));

        auto imm = 0;
        GET_I_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_LW: {
        LOG_DEBUG(INTERPRETER, I().Dump("LW"));

        auto imm = 0;
        GET_I_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_LBU: {
        LOG_DEBUG(INTERPRETER, I().Dump("LBU"));

        auto imm = 0;
        GET_I_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_LHU: {
        LOG_DEBUG(INTERPRETER, I().Dump("LHU"));

        auto imm = 0;
        GET_I_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    S_SB: {
        LOG_DEBUG(INTERPRETER, S().Dump("SB"));

        uint32_t imm = 0;
        GET_S_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::S));
        DISPATCH();
    }
    
    S_SH: {
        LOG_DEBUG(INTERPRETER, S().Dump("SH"));

        uint32_t imm = 0;
        GET_S_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::S));
        DISPATCH();
    }
    
    S_SW: {
        LOG_DEBUG(INTERPRETER, S().Dump("SW"));

        uint32_t imm = 0;
        GET_S_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::S));
        DISPATCH();
    }
    
    I_ADDI: {
        LOG_DEBUG(INTERPRETER, I().Dump("ADDI"));

        auto rs1 = I().rs1;
        auto rd = I().rd;
        auto imm = 0;
        GET_I_IMM(imm);
        
        if (rd != 0) {
            SetReg(rd, GetReg(rs1) + SIGNED(imm));
        }

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_SLTI: {
        LOG_DEBUG(INTERPRETER, I().Dump("SLTI"));

        auto rs1 = I().rs1;
        auto rd = I().rd;
        auto imm = 0;
        GET_I_IMM(imm);

        if (SIGNED(rs1) < SIGNED(imm)) {
            SetReg(rd, 1U);
        } else {
            SetReg(rd, 0U);

        }

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_SLTIU: {
        LOG_DEBUG(INTERPRETER, I().Dump("SLTIU"));

        auto rs1 = I().rs1;
        auto rd = I().rd;
        auto imm = 0;
        GET_I_IMM(imm);

        if (rs1 < imm) {
            SetReg(rd, 1U);
        } else {
            SetReg(rd, 0U);

        }

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_XORI: {
        LOG_DEBUG(INTERPRETER, I().Dump("XORI"));

        auto rs1 = I().rs1;
        auto rd = I().rd;
        auto imm = 0;
        GET_I_IMM(imm);
        
        if (rd != 0) {
            SetReg(rd, SIGNED(GetReg(rs1)) ^ SIGNED(imm));
        }

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_ORI: {
        LOG_DEBUG(INTERPRETER, I().Dump("ORI"));

        auto rs1 = I().rs1;
        auto rd = I().rd;
        auto imm = 0;
        GET_I_IMM(imm);
        
        if (rd != 0) {
            SetReg(rd, SIGNED(GetReg(rs1)) | SIGNED(imm));
        }

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_ANDI: {
        LOG_DEBUG(INTERPRETER, I().Dump("ANDI"));

        auto rs1 = I().rs1;
        auto rd = I().rd;
        auto imm = 0;
        GET_I_IMM(imm);
        
        if (rd != 0) {
            SetReg(rd, SIGNED(GetReg(rs1)) & SIGNED(imm));
        }

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    R_SLLI: {
        LOG_DEBUG(INTERPRETER, R().Dump("SLLI"));

        auto rs1 = R().rs1;
        auto rd = R().rd;
        auto shamt = R().rs2;

        SetReg(rd, GetReg(rs1) << shamt);

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SRLI: {
        LOG_DEBUG(INTERPRETER, R().Dump("SRLI"));

        auto rs1 = R().rs1;
        auto rd = R().rd;
        auto shamt = R().rs2;

        SetReg(rd, GetReg(rs1) >> shamt);

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SRAI: {
        LOG_DEBUG(INTERPRETER, R().Dump("SRAI"));

        auto rs1 = R().rs1;
        auto rd = R().rd;
        auto shamt = R().rs2;

        auto rs1_value = GetReg(rs1);
        auto ans = (rs1_value >> shamt) | (rs1_value & (1 << 31));
        SetReg(rd, ans);

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_ADD: {
        LOG_DEBUG(INTERPRETER, R().Dump("ADD"));

        auto rd = R().rd;
        auto rs1 = R().rs1;
        auto rs2 = R().rs2;

        if (rd != 0) {
            auto value = SIGNED(GetReg(rs1)) + SIGNED(GetReg(rs2));
            SetReg(rd, value);
        }

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SUB: {
        LOG_DEBUG(INTERPRETER, R().Dump("SUB"));

        auto rd = R().rd;
        auto rs1 = R().rs1;
        auto rs2 = R().rs2;

        if (rd != 0) {
            auto value = SIGNED(GetReg(rs1)) - SIGNED(GetReg(rs2));
            SetReg(rd, value);
        }

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SLL: {
        LOG_DEBUG(INTERPRETER, R().Dump("SLL"));
        
        auto rd = R().rd;
        auto rs1 = R().rs1;
        auto rs2 = R().rs2;

        if (rd != 0) {
            auto value = GetReg(rs1) << GetReg(rs2);
            SetReg(rd, value);
        }

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SLT: {
        LOG_DEBUG(INTERPRETER, R().Dump("SLT"));

        auto rd = R().rd;
        auto rs1 = R().rs1;
        auto rs2 = R().rs2;

        if (rd != 0) {
            if (SIGNED(GetReg(rs1)) < SIGNED(GetReg(rs2))) {
                SetReg(rd, 1U);
            } else {
                SetReg(rd, 0U);
            }
        }

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SLTU: {
        LOG_DEBUG(INTERPRETER, R().Dump("SLTU"));

        auto rd = R().rd;
        auto rs1 = R().rs1;
        auto rs2 = R().rs2;

        if (rd != 0) {
            if (GetReg(rs1) < GetReg(rs2)) {
                SetReg(rd, 1U);
            } else {
                SetReg(rd, 0U);
            }
        }

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_XOR: {
        LOG_DEBUG(INTERPRETER, R().Dump("XOR"));

        auto rd = R().rd;
        auto rs1 = R().rs1;
        auto rs2 = R().rs2;

        if (rd != 0) {
            auto value = GetReg(rs1) ^ GetReg(rs2);
            SetReg(rd, value);
        }

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SRL: {
        LOG_DEBUG(INTERPRETER, R().Dump("SRL"));

        auto rd = R().rd;
        auto rs1 = R().rs1;
        auto rs2 = R().rs2;

        if (rd != 0) {
            auto value = GetReg(rs1) >> GetReg(rs2);
            SetReg(rd, value);
        }

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SRA: {
        LOG_DEBUG(INTERPRETER, R().Dump("SRA"));

        auto rd = R().rd;
        auto rs1 = R().rs1;
        auto rs2 = R().rs2;

        if (rd != 0) {
            auto value = SIGNED(GetReg(rs1)) >> SIGNED(GetReg(rs2));
            SetReg(rd, value);
        }

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_OR: {
        LOG_DEBUG(INTERPRETER, R().Dump("OR"));

        auto rd = R().rd;
        auto rs1 = R().rs1;
        auto rs2 = R().rs2;

        if (rd != 0) {
            auto value = GetReg(rs1) | GetReg(rs2);
            SetReg(rd, value);
        }

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_AND: {
        LOG_DEBUG(INTERPRETER, R().Dump("AND"));

        auto rd = R().rd;
        auto rs1 = R().rs1;
        auto rs2 = R().rs2;

        if (rd != 0) {
            auto value = GetReg(rs1) & GetReg(rs2);
            SetReg(rd, value);
        }

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
