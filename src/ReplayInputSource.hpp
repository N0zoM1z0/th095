#pragma once

#include "inttypes.hpp"
#include <stddef.h>

namespace th095
{

// Target-backed view over the replay/controller input state at 0x004BE218.
// ReplayManager consumes +0x00/+0x06, while Update owns the history block at
// +0x2C..+0x58.
struct ReplayInputSource
{
    u16 currentInput;          // +0x00
    u16 unknown002;
    u16 repeatOutput;          // +0x04
    u16 pressedInput;          // +0x06
    u8 unknown008[0x24];
    u16 historyCurrent;        // +0x2C
    u16 historyPrevious;       // +0x2E
    u16 historyRepeat;         // +0x30
    u16 historyPressed;        // +0x32
    u16 historyReleased;       // +0x34
    u16 unknown036;
    u16 heldFrames[16];        // +0x38

    void Update();
};

typedef char ReplayInputCurrentAt00[(offsetof(ReplayInputSource, currentInput) == 0x00) ? 1 : -1];
typedef char ReplayInputPressedAt06[(offsetof(ReplayInputSource, pressedInput) == 0x06) ? 1 : -1];
typedef char ReplayInputHistoryAt2C[(offsetof(ReplayInputSource, historyCurrent) == 0x2c) ? 1 : -1];
typedef char ReplayInputHeldAt38[(offsetof(ReplayInputSource, heldFrames) == 0x38) ? 1 : -1];

extern ReplayInputSource g_ReplayInputSource;

} // namespace th095
