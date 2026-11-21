#pragma once

#include "inttypes.hpp"

#include <stddef.h>

namespace th095
{

namespace EclRunLow
{
enum EclComparisonOpcode
{
    ECL_COMPARE_INT_EQUAL = 40,
    ECL_COMPARE_FLOAT_EQUAL = 41,
    ECL_COMPARE_INT_NOT_EQUAL = 42,
    ECL_COMPARE_FLOAT_NOT_EQUAL = 43,
    ECL_COMPARE_INT_LESS = 44,
    ECL_COMPARE_FLOAT_LESS = 45,
    ECL_COMPARE_INT_LESS_EQUAL = 46,
    ECL_COMPARE_FLOAT_LESS_EQUAL = 47,
    ECL_COMPARE_INT_GREATER = 48,
    ECL_COMPARE_FLOAT_GREATER = 49,
    ECL_COMPARE_INT_GREATER_EQUAL = 50,
    ECL_COMPARE_FLOAT_GREATER_EQUAL = 51,
};
} // namespace EclRunLow

union EclRawOperand
{
    i32 asInt;
    f32 asFloat;
    i16 lowWord;
    u8 lowByte;
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
typedef char EclRawOperandSizeIs4[(sizeof(EclRawOperand) == 4) ? 1 : -1];
typedef char EclRawInstructionSizeIs10[
    (sizeof(EclRawInstruction) == 0x10) ? 1 : -1];
typedef char EclShotOperandsOffsetIsC[
    (offsetof(EclShotInstruction, operands) == 0xc) ? 1 : -1];
typedef char EclShotInstructionSizeIs2C[
    (sizeof(EclShotInstruction) == 0x2c) ? 1 : -1];

} // namespace th095
