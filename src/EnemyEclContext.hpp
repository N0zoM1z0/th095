#pragma once

#include "EclScriptState.hpp"
#include "ZunTimer.hpp"
#include "ecl/EclCallback.hpp"

namespace th095
{

struct PhotoEnemyView;
struct EnemyEclInterpolationSlot;
typedef void (__fastcall *EnemyEclInterpolatorCallback)(
    PhotoEnemyView *enemy, EnemyEclInterpolationSlot *slot, f32 progress);

struct EnemyEclInterpolationSlot
{
    EnemyEclInterpolationSlot()
    {
    }

    EnemyEclInterpolatorCallback callback;
    ZunTimer timer;
    i32 duration;
    i32 callbackIndex;
    i32 easing;
    f32 parameters[4];
    f32 affectedVariable;
};
typedef char EnemyEclInterpolationSlotSize[
    (sizeof(EnemyEclInterpolationSlot) == 0x30) ? 1 : -1];
typedef char EnemyEclInterpolationSlotDurationOffset[
    (offsetof(EnemyEclInterpolationSlot, duration) == 0x10) ? 1 : -1];
typedef char EnemyEclInterpolationSlotCallbackIndexOffset[
    (offsetof(EnemyEclInterpolationSlot, callbackIndex) == 0x14) ? 1 : -1];
typedef char EnemyEclInterpolationSlotParametersOffset[
    (offsetof(EnemyEclInterpolationSlot, parameters) == 0x1c) ? 1 : -1];
typedef char EnemyEclInterpolationSlotAffectedVariableOffset[
    (offsetof(EnemyEclInterpolationSlot, affectedVariable) == 0x2c) ? 1 : -1];

// The main context, call stack and allocated child contexts share this layout.
struct EnemyEclContext
{
    EnemyEclContext();

    EclRawInstruction *currentInstr;
    ZunTimer time;
    EclExtendedCallback perFrameCallback;
    EclRawInstruction *perFrameInstruction;
    EclScriptState scriptState;
    ZunTimer secondaryTime;
    EnemyEclInterpolationSlot interpolationSlots[8];
    i32 unknown224;
    i32 childContextSlot;
    i16 subId;
    u8 contextTailAlignment22E[2];
};
typedef char EnemyEclContextCallbackOffsetCheck[offsetof(EnemyEclContext, perFrameCallback) == 0x10 ? 1 : -1];
typedef char EnemyEclContextCallbackInstructionOffsetCheck[offsetof(EnemyEclContext, perFrameInstruction) == 0x14 ? 1 : -1];
typedef char EnemyEclContextScriptStateOffsetCheck[offsetof(EnemyEclContext, scriptState) == 0x18 ? 1 : -1];
typedef char EnemyEclContextSecondaryTimerOffsetCheck[offsetof(EnemyEclContext, secondaryTime) == 0x98 ? 1 : -1];
typedef char EnemyEclContextInterpolationSlotsOffsetCheck[offsetof(EnemyEclContext, interpolationSlots) == 0xa4 ? 1 : -1];
typedef char EnemyEclContextUnconsumedOffsetCheck[offsetof(EnemyEclContext, unknown224) == 0x224 ? 1 : -1];
typedef char EnemyEclContextChildSlotOffsetCheck[offsetof(EnemyEclContext, childContextSlot) == 0x228 ? 1 : -1];
typedef char EnemyEclContextSubIdOffsetCheck[offsetof(EnemyEclContext, subId) == 0x22c ? 1 : -1];
typedef char EnemyEclContextTailAlignmentOffsetCheck[offsetof(EnemyEclContext, contextTailAlignment22E) == 0x22e ? 1 : -1];
typedef char EnemyEclContextSizeCheck[sizeof(EnemyEclContext) == 0x230 ? 1 : -1];

// Allocated by ECL opcode 117. The target clears the complete block,
// installs one child context at +0x8, and uses the 16 following contexts as
// that child's call stack.
struct EnemyChildEclBlock
{
    i32 subId;
    u16 unconsumedWord04;
    i16 callStackDepth;
    EnemyEclContext eclContext;
    EnemyEclContext callStack[16];
};
typedef char EnemyChildEclBlockSize[
    (sizeof(EnemyChildEclBlock) == 0x2538) ? 1 : -1];
typedef char EnemyChildEclBlockSubIdOffset[
    (offsetof(EnemyChildEclBlock, subId) == 0x0) ? 1 : -1];
typedef char EnemyChildEclBlockUnconsumedWord04Offset[
    (offsetof(EnemyChildEclBlock, unconsumedWord04) == 0x4) ? 1 : -1];
typedef char EnemyChildEclBlockCallStackDepthOffset[
    (offsetof(EnemyChildEclBlock, callStackDepth) == 0x6) ? 1 : -1];
typedef char EnemyChildEclBlockEclContextOffset[
    (offsetof(EnemyChildEclBlock, eclContext) == 0x8) ? 1 : -1];
typedef char EnemyChildEclBlockCallStackOffset[
    (offsetof(EnemyChildEclBlock, callStack) == 0x238) ? 1 : -1];

} // namespace th095
