#pragma once

#include "inttypes.hpp"

namespace th095
{

// Flags for the canonical TH095 0x7BC Supervisor owner at +0x444. The
// TH08-shaped 0x364 compatibility owner in Supervisor.hpp has a distinct,
// intentionally incomplete declaration.
struct SupervisorFlags
{
    u32 usingHardwareTL : 1;
    u32 lockableBackbuffer : 1;
    u32 using32BitGraphics : 1;
    u32 speedhackDetected : 1;
    u32 d3dDeviceNeedsReset : 1;
    u32 forceExtraTimerStep : 1;
    u32 dummyMidiTimerEnabled : 1;
    u32 receivedCloseMsg : 1;
    // Set at constructor 0x00426428; cleared at startup 0x0042445A.
    // No independent consumer establishes the old score-backup interpretation.
    u32 unknownBit8 : 1;
    u32 resultRestartActive : 1;
    u32 keyboardAvailable : 1;
    u32 controllerAvailable : 1;
    u32 restartPhotoGame : 1;
    u32 unknown13 : 19;
};

typedef char SupervisorFlagsSizeIs4[
    (sizeof(SupervisorFlags) == 4) ? 1 : -1];

} // namespace th095
