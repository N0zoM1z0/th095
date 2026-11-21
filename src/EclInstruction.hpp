#pragma once

#include "inttypes.hpp"

#include <stddef.h>

namespace th095
{

union EclRawOperand
{
    i32 asInt;
    f32 asFloat;
};

struct EclInstructionHeader
{
    i32 time;
    i16 opcode;
    i16 nextOffset;
    u8 serializedReserved08;
    u8 difficultyMask;
    u16 operandFlags;
};

struct EclRawInstruction : EclInstructionHeader
{
    EclRawOperand operands[1];
};

struct EclShotOperands
{
    i16 bulletType;
    i16 color;
    i32 count1;
    i32 count2;
    f32 speed1;
    f32 speed2;
    f32 angle;
    f32 angleStep;
    u32 transformFlags;
};
typedef char EclShotOperandsSizeIs20[(sizeof(EclShotOperands) == 0x20) ? 1 : -1];

// Opcodes 86 through 94 carry these fixed operands after the common header.
struct EclShotInstruction
{
    EclInstructionHeader header;
    EclShotOperands operands;
};

typedef char EclInstructionHeaderSizeIsC[
    (sizeof(EclInstructionHeader) == 0xc) ? 1 : -1];
typedef char EclRawInstructionSizeIs10[
    (sizeof(EclRawInstruction) == 0x10) ? 1 : -1];
typedef char EclShotOperandsOffsetIsC[
    (offsetof(EclShotInstruction, operands) == 0xc) ? 1 : -1];
typedef char EclShotInstructionSizeIs2C[
    (sizeof(EclShotInstruction) == 0x2c) ? 1 : -1];

} // namespace th095
