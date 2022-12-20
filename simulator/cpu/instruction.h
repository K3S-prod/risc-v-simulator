#pragma once

#include <iostream>
#include <cstdint>

enum class Opcode {
    ADD_SUB = 0,
    SLL = 1,
	SLT = 2,
	SLTU = 3,
	XOR = 4,
	SRL_SRA = 5,
	OR = 6,
	AND = 7
};

namespace sim {

Opcode GetOpcode(uint32_t inst) {
    const int OPCODE_SHIFT = 0;
    const int OPCODE_MASK = 0x7;

    Opcode opcode {(inst >> OPCODE_SHIFT) & OPCODE_MASK};

    switch (opcode) {
        case Opcode::ADD_SUB: {
            std::cout << "found ADD_SUB" << std::endl;
        }
    }

    return opcode;
}

class Instruction {
public:
    Instruction(uint32_t inst);
    virtual ~Instruction();

    int GetOpcode() { return opcode_; }
    virtual void Apply() = 0;

private:
    uint8_t opcode_;
};

class RegisterInstruction : public Instruction {
public:
    RegisterInstruction(uint32_t inst) : Instruction(inst) {
        reg_destination_ = (inst >> DESTINATION_REGISTER_SHIFT) & DESTINATION_REGISTER_MASK;
        funct3_ = (inst >> FUNCT3_SHIFT) & FUNCT3_MASK;
        reg_source_first_ = (inst >> SOURCE_REGISTER_FIRST_SHIFT) & SOURCE_REGISTER_FIRST_MASK;
        reg_source_second_ = (inst >> SOURCE_REGISTER_SECOND_SHIFT) & SOURCE_REGISTER_SECOND_MASK;
        funct7_ = (inst >> FUNCT7_SHIFT) & FUNCT7_MASK;
    }
    virtual ~RegisterInstruction();

    virtual void Apply() override {
        std::cout << "Apply register instruction" << std::endl;
    }

    static const int DESTINATION_REGISTER_SHIFT = 7;
    static const int DESTINATION_REGISTER_MASK = 0x1F;

    static const int FUNCT3_SHIFT = 12;
    static const int FUNCT3_MASK = 0x07;

    static const int SOURCE_REGISTER_FIRST_SHIFT = 15;
    static const int SOURCE_REGISTER_FIRST_MASK = 0x1F;

    static const int SOURCE_REGISTER_SECOND_SHIFT = 20;
    static const int SOURCE_REGISTER_SECOND_MASK = 0x1F;

    static const int FUNCT7_SHIFT = 25;
    static const int FUNCT7_MASK = 0x07;

private:
    uint8_t reg_destination_;
    uint8_t funct3_;
    uint8_t reg_source_first_;
    uint8_t reg_source_second_;
    uint8_t funct7_;
};

class ImmediateInstruction : public Instruction {
public:
    ImmediateInstruction(uint32_t inst) : Instruction(inst) {
        reg_destination_ = (inst >> DESTINATION_REGISTER_SHIFT) & DESTINATION_REGISTER_MASK;
        funct3_ = (inst >> FUNCT3_SHIFT) & FUNCT3_MASK;
        reg_source_first_ = (inst >> SOURCE_REGISTER_FIRST_SHIFT) & SOURCE_REGISTER_FIRST_MASK;
        immediate_ = (inst >> IMMEDIATE_SHIFT) & IMMEDIATE_MASK;
    }
    virtual ~ImmediateInstruction();

    virtual void Apply() override {
        std::cout << "Apply immediate instruction" << std::endl;
    }
    static const int DESTINATION_REGISTER_SHIFT = 7;
    static const int DESTINATION_REGISTER_MASK = 0x1F;

    static const int FUNCT3_SHIFT = 12;
    static const int FUNCT3_MASK = 0x07;

    static const int SOURCE_REGISTER_FIRST_SHIFT = 15;
    static const int SOURCE_REGISTER_FIRST_MASK = 0x1F;

    static const int IMMEDIATE_SHIFT = 20;
    static const int IMMEDIATE_MASK = 0xFFF;

private:
    uint8_t reg_destination_;
    uint8_t funct3_;
    uint8_t reg_source_first_;
    uint16_t immediate_;
};

} // namespace sim
