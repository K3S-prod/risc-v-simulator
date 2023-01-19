#ifndef SIMULATOR_DECODER_DECODER_H
#define SIMULATOR_DECODER_DECODER_H

#include "common/macro.h"

#include <cstdint>
#include <cstddef>

namespace sim {

struct Decoder {
public:
    static constexpr uint16_t OPC_MASK = 0b000000001111111;
    static constexpr uint16_t FUNCT_3_MASK = 0b0111000000000000;
    static constexpr uint8_t FUNCT_3_SHIFT = 12;
    static constexpr uint32_t FUNCT_7_MASK = 0b11111110000000000000000000000000;
    static constexpr uint8_t FUNCT_7_SHIFT = 25;
    static constexpr uint32_t FUNCT_5_MASK = 0b11111000000000000000000000000000;
    static constexpr uint8_t FUNCT_5_SHIFT = 27;
    static constexpr uint32_t IMM_11_0_MASK = 0b11111111111100000000000000000000;
    static constexpr uint8_t IMM_11_0_SHIFT = 20;

#include <simulator/decoder/generated/dispatch_labels_enum.inl>
    
public:
    uint16_t PeekOpcode(const uint8_t *insts_buf)
    {
        const auto * buf_16 = static_cast<const uint16_t *>(static_cast<const void *>(insts_buf));
        return *buf_16 & OPC_MASK;
    }
    uint16_t PeekFunct_3(const uint8_t *insts_buf)
    {
        const auto * buf_16 = static_cast<const uint16_t *>(static_cast<const void *>(insts_buf));
        return (*buf_16 & FUNCT_3_MASK) >> FUNCT_3_SHIFT;
    }
    uint16_t PeekFunct_7(const uint8_t *insts_buf)
    {
        const auto * buf_32 = static_cast<const uint32_t *>(static_cast<const void *>(insts_buf));
        return (*buf_32 & FUNCT_7_MASK) >> FUNCT_7_SHIFT;
    }
    uint16_t PeekFunct_5(const uint8_t *insts_buf)
    {
        const auto * buf_32 = static_cast<const uint32_t *>(static_cast<const void *>(insts_buf));
        return (*buf_32 & FUNCT_5_MASK) >> FUNCT_5_SHIFT;
    }
    uint16_t PeekImm_11_0(const uint8_t *insts_buf)
    {
        const auto * buf_32 = static_cast<const uint32_t *>(static_cast<const void *>(insts_buf));
        return (*buf_32 & IMM_11_0_MASK) >> IMM_11_0_SHIFT;
    }
#include <simulator/decoder/generated/decoder.inl>

public:
#include <simulator/decoder/generated/signatures_definition.inl>
};

}  // namespace sim::interpreter

#endif  // SIMULATOR_DECODER_DECODER_H
