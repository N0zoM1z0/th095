#pragma once

#include "ControllerButtonMapping.hpp"
#include "diffbuild.hpp"

#include <stddef.h>

namespace th095
{

struct ReplayInputHistory
{
    u16 current;
    u16 previous;
    u16 repeat;
    u16 pressed;
    u16 released;
    u16 unknown00a;
    u16 heldFrames[16];
};

struct ControllerInputSlotView
{
    ControllerInputSlotView();
    void UpdateReplayHistory();

    u16 current;
    u16 previous;
    u16 repeat;
    u16 pressed;
    u16 released;
    u16 heldFrames[16];
    u8 unknown02a[2];
    ReplayInputHistory replayHistory;
    ControllerButtonMapping mappings[3];
};

typedef char ReplayInputHistoryLayout[
    (offsetof(ControllerInputSlotView, replayHistory) == 0x2c &&
     offsetof(ReplayInputHistory, current) == 0 &&
     offsetof(ReplayInputHistory, previous) == 2 &&
     offsetof(ReplayInputHistory, repeat) == 4 &&
     offsetof(ReplayInputHistory, pressed) == 6 &&
     offsetof(ReplayInputHistory, released) == 8 &&
     offsetof(ReplayInputHistory, heldFrames) == 0x0c &&
     sizeof(ReplayInputHistory) == 0x2c) ? 1 : -1];

DIFFABLE_EXTERN_ARRAY(ControllerInputSlotView, 3, g_ControllerInputSlots);

} // namespace th095
