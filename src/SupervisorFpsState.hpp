#ifndef TH095_SUPERVISOR_FPS_STATE_HPP
#define TH095_SUPERVISOR_FPS_STATE_HPP

#include "inttypes.hpp"

namespace th095
{
// Main-owned process globals at target 0x004C4E2C / 0x004C4E30. They happen
// to follow the 0x7BC Supervisor but are outside its constructor/owner extent.
// CalculateFps alone updates them; neither is a pointer or serialized offset.
extern i32 g_FpsClockAnomalyCount;
extern f64 g_LastFpsTimestamp;

typedef char SupervisorFpsAnomalyCountIsFourBytes[
    (sizeof(g_FpsClockAnomalyCount) == 4) ? 1 : -1];
typedef char SupervisorFpsTimestampIsEightBytes[
    (sizeof(g_LastFpsTimestamp) == 8) ? 1 : -1];
}

#endif
