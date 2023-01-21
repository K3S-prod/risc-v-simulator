[this]() {
    Decoder decoder;

#include <simulator/interpreter/generated/dispatch_table.inl>

    // Begin execution:
    DISPATCH();

    U_LUI: {
        LOG_DEBUG(INTERPRETER, U().Dump(GetPc().DumpHex() + " LUI"));

        auto rd = U().rd;
        auto imm = U().imm_31_12;
        imm <<= 12;

        SetReg(rd, imm);

        AdvancePc(sizeof(Decoder::U));
        DISPATCH();
    }
    
    U_AUIPC: {
        LOG_DEBUG(INTERPRETER, U().Dump(GetPc().DumpHex() + " AUIPC"));

        auto rd = U().rd;
        auto imm = U().imm_31_12;
        auto pc = GETPC();
        
        imm <<= 12;
        SetReg(rd, imm + pc);

        AdvancePc(sizeof(Decoder::U));
        DISPATCH();
    }
    
    J_JAL: {
        LOG_DEBUG(INTERPRETER, J().Dump(GetPc().DumpHex() + " JAL"));

        auto rd = J().rd;
        uint32_t imm = 0;
        GET_J_IMM(imm);
        auto pc = GETPC();

        SetReg(rd, pc + 4U);
        SETPC(pc + SIGNED(imm));

        DISPATCH();
    }
    
    I_JALR: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " JALR"));

        auto imm = 0;
        GET_I_IMM(imm);
        auto pc = GETPC();

        SetReg(I().rd, pc + 4U);

        SETPC(SIGNED(GetReg(I().rs1)) + SIGNED(imm));

        /* TODO: check if we need this
        auto new_pc = GETPC();
        SETPC(new_pc & 0xFFFFFFFE);
        */

        DISPATCH();
    }
    
    B_BEQ: {
        LOG_DEBUG(INTERPRETER, B().Dump(GetPc().DumpHex() + " BEQ"));

        int32_t rs1 = GetReg(B().rs1);
        int32_t rs2 = GetReg(B().rs2);
        auto pc = GETPC();
        uint32_t imm = 0;
        GET_B_IMM(imm);

        if (rs1 == rs2) {
            SetPc(pc + SIGNED(imm));
        } else {
            SetPc(pc + 4);
        }
        
        DISPATCH();
    }
    
    B_BNE: {
        LOG_DEBUG(INTERPRETER, B().Dump(GetPc().DumpHex() + " BNE"));

        int32_t rs1 = GetReg(B().rs1);
        int32_t rs2 = GetReg(B().rs2);
        auto pc = GETPC();
        uint32_t imm = 0;
        GET_B_IMM(imm);

        if (rs1 != rs2) {
            SetPc(pc + SIGNED(imm));
        } else {
            SetPc(pc + 4);
        }

        DISPATCH();
    }
    
    B_BLT: {
        LOG_DEBUG(INTERPRETER, B().Dump(GetPc().DumpHex() + " BLT"));

        int32_t rs1 = GetReg(B().rs1);
        int32_t rs2 = GetReg(B().rs2);
        auto pc = GETPC();
        uint32_t imm = 0;
        GET_B_IMM(imm);

        if (rs1 < rs2) {
            SetPc(pc + SIGNED(imm));
        } else {
            SetPc(pc + 4);
        }

        DISPATCH();
    }
    
    B_BGE: {
        LOG_DEBUG(INTERPRETER, B().Dump(GetPc().DumpHex() + " BGE"));

        int32_t rs1 = GetReg(B().rs1);
        int32_t rs2 = GetReg(B().rs2);
        auto pc = GETPC();
        uint32_t imm = 0;
        GET_B_IMM(imm);

        if (rs1 >= rs2) {
            SetPc(pc + SIGNED(imm));
        } else {
            SetPc(pc + 4);
        }

        DISPATCH();
    }
    
    B_BLTU: {
        LOG_DEBUG(INTERPRETER, B().Dump(GetPc().DumpHex() + " BLTU"));

        uint32_t rs1 = GetReg(B().rs1);
        uint32_t rs2 = GetReg(B().rs2);
        auto pc = GETPC();
        uint32_t imm = 0;
        GET_B_IMM(imm);

        if (rs1 < rs2) {
            SetPc(pc + SIGNED(imm));
        } else {
            SetPc(pc + 4);
        }

        DISPATCH();
    }
    
    B_BGEU: {
        LOG_DEBUG(INTERPRETER, B().Dump(GetPc().DumpHex() + " BGEU"));

        uint32_t rs1 = GetReg(B().rs1);
        uint32_t rs2 = GetReg(B().rs2);
        auto pc = GETPC();
        uint32_t imm = 0;
        GET_B_IMM(imm);
        
        if (rs1 >= rs2) {
            SetPc(pc + SIGNED(imm));
        } else {
            SetPc(pc + 4);
        }

        DISPATCH();
    }
    
    I_LB: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " LB"));

        auto imm = 0;
        GET_I_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_LH: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " LH"));

        auto imm = 0;
        GET_I_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_LW: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " LW"));

        auto rs1 = I().rs1;
        auto rd = I().rd;
        auto imm = 0;
        GET_I_IMM(imm);

        int loaded = 0;
        LOAD_FROM_MEMORY(GetReg(rs1) + imm);
        SetReg(rd, loaded);

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_LBU: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " LBU"));

        auto imm = 0;
        GET_I_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_LHU: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " LHU"));

        auto imm = 0;
        GET_I_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    S_SB: {
        LOG_DEBUG(INTERPRETER, S().Dump(GetPc().DumpHex() + " SB"));

        uint32_t imm = 0;
        GET_S_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::S));
        DISPATCH();
    }
    
    S_SH: {
        LOG_DEBUG(INTERPRETER, S().Dump(GetPc().DumpHex() + " SH"));

        uint32_t imm = 0;
        GET_S_IMM(imm);
        throw std::runtime_error("Not implemented");

        AdvancePc(sizeof(Decoder::S));
        DISPATCH();
    }
    
    S_SW: {
        LOG_DEBUG(INTERPRETER, S().Dump(GetPc().DumpHex() + " SW"));

        auto base = GetReg(S().rs1);
        auto src = GetReg(S().rs2);
        uint32_t imm = 0;
        GET_S_IMM(imm);

        auto addr = SIGNED(base) + imm;
        STORE_TO_MEMORY(addr, src);

        AdvancePc(sizeof(Decoder::S));
        DISPATCH();
    }
    
    I_ADDI: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " ADDI"));

        auto rs1 = GetReg(I().rs1);
        auto imm = 0;
        GET_I_IMM(imm);
        
        SetReg(I().rd, rs1 + SIGNED(imm));

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_SLTI: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " SLTI"));

        auto rs1 = GetReg(I().rs1);
        auto imm = 0;
        GET_I_IMM(imm);

        if (SIGNED(rs1) < SIGNED(imm)) {
            SetReg(I().rd, 1U);
        } else {
            SetReg(I().rd, 0U);
        }

        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_SLTIU: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " SLTIU"));

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
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " XORI"));

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
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " ORI"));

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
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " ANDI"));

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
        LOG_DEBUG(INTERPRETER, R().Dump(GetPc().DumpHex() + " SLLI"));

        auto rs1 = R().rs1;
        auto rd = R().rd;
        auto shamt = R().rs2;

        SetReg(rd, GetReg(rs1) << shamt);

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SRLI: {
        LOG_DEBUG(INTERPRETER, R().Dump(GetPc().DumpHex() + " SRLI"));

        auto rs1 = R().rs1;
        auto rd = R().rd;
        auto shamt = R().rs2;

        SetReg(rd, GetReg(rs1) >> shamt);

        AdvancePc(sizeof(Decoder::R));
        DISPATCH();
    }
    
    R_SRAI: {
        LOG_DEBUG(INTERPRETER, R().Dump(GetPc().DumpHex() + " SRAI"));

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
        LOG_DEBUG(INTERPRETER, R().Dump(GetPc().DumpHex() + " ADD"));

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
        LOG_DEBUG(INTERPRETER, R().Dump(GetPc().DumpHex() + " SUB"));

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
        LOG_DEBUG(INTERPRETER, R().Dump(GetPc().DumpHex() + " SLL"));
        
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
        LOG_DEBUG(INTERPRETER, R().Dump(GetPc().DumpHex() + " SLT"));

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
        LOG_DEBUG(INTERPRETER, R().Dump(GetPc().DumpHex() + " SLTU"));

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
        LOG_DEBUG(INTERPRETER, R().Dump(GetPc().DumpHex() + " XOR"));

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
        LOG_DEBUG(INTERPRETER, R().Dump(GetPc().DumpHex() + " SRL"));

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
        LOG_DEBUG(INTERPRETER, R().Dump(GetPc().DumpHex() + " SRA"));

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
        LOG_DEBUG(INTERPRETER, R().Dump(GetPc().DumpHex() + " OR"));

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
        LOG_DEBUG(INTERPRETER, R().Dump(GetPc().DumpHex() + " AND"));

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
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " FENCE"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_FENCE_I: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " FENCE_I"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_ECALL: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " ECALL"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_EBREAK: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " EBREAK"));
        return;
    }
    
    I_CSRRW: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " CSRRW"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_CSRRS: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " CSRRS"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_CSRRC: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " CSRRC"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_CSRRWI: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " CSRRWI"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_CSRRSI: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " CSRRSI"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }
    
    I_CSRRCI: {
        LOG_DEBUG(INTERPRETER, I().Dump(GetPc().DumpHex() + " CSRRCI"));
        AdvancePc(sizeof(Decoder::I));
        DISPATCH();
    }

    A_LR_W: {
        LOG_DEBUG(INTERPRETER, A().Dump(GetPc().DumpHex() + " LR_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_SC_W: {
        LOG_DEBUG(INTERPRETER, A().Dump(GetPc().DumpHex() + " SC_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOSWAP_W: {
        LOG_DEBUG(INTERPRETER, A().Dump(GetPc().DumpHex() + " AMOSWAP_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOADD_W: {
        LOG_DEBUG(INTERPRETER, A().Dump(GetPc().DumpHex() + " AMOADD_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOXOR_W: {
        LOG_DEBUG(INTERPRETER, A().Dump(GetPc().DumpHex() + " AMOXOR_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOAND_W: {
        LOG_DEBUG(INTERPRETER, A().Dump(GetPc().DumpHex() + " AMOAND_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOOR_W: {
        LOG_DEBUG(INTERPRETER, A().Dump(GetPc().DumpHex() + " AMOOR_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOMIN_W: {
        LOG_DEBUG(INTERPRETER, A().Dump(GetPc().DumpHex() + " AMOMIN_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOMAX_W: {
        LOG_DEBUG(INTERPRETER, A().Dump(GetPc().DumpHex() + " AMOMAX_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOMINU_W: {
        LOG_DEBUG(INTERPRETER, A().Dump(GetPc().DumpHex() + " AMOMINU_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    A_AMOMAXU_W: {
        LOG_DEBUG(INTERPRETER, A().Dump(GetPc().DumpHex() + " AMOMAXU_W"));
        AdvancePc(sizeof(Decoder::A));
        DISPATCH();
    }
    UNREACHABLE();
};
