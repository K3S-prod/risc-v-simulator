#include "instruction.h"

#include <iostream>

namespace sim {

Opcode GetOpcode(uint32_t inst) {
    const int OPCODE_SHIFT = 0;
    const int OPCODE_MASK = 0x7F;

    Opcode opc {(inst >> OPCODE_SHIFT) & OPCODE_MASK};
    return opc;
}

}