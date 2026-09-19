#pragma once

#include "ZunMath.hpp"
#include "inttypes.hpp"

#include <stddef.h>

namespace th095
{

// Spawn packet for PhotoEffectManagerView::Spawn @ 0x0041DBD0 kind 0.  The
// manager forwards the complete 0x28-byte value to PhotoStraightLaserView;
// Initialize @ 0x0041E0C0 and Update @ 0x0041E2C0 independently establish
// every float slot.
// Keep this owner constructor-free because ECL initializes the packet with an
// explicit memset and PhotoEffect has one copy-initialized fragment path.
struct PhotoStraightLaserSpawnArgs
{
    Float3 position;          // +0x00
    f32 angle;                // +0x0c
    f32 maximumLength;        // +0x10
    f32 initialLength;        // +0x14
    f32 terminalDistance;     // +0x18
    f32 width;                // +0x1c
    f32 speed;                // +0x20
    i16 type;                 // +0x24
    i16 color;                // +0x26
};

C_ASSERT(sizeof(PhotoStraightLaserSpawnArgs) == 0x28);
C_ASSERT(offsetof(PhotoStraightLaserSpawnArgs, angle) == 0x0c);
C_ASSERT(offsetof(PhotoStraightLaserSpawnArgs, maximumLength) == 0x10);
C_ASSERT(offsetof(PhotoStraightLaserSpawnArgs, initialLength) == 0x14);
C_ASSERT(offsetof(PhotoStraightLaserSpawnArgs, terminalDistance) == 0x18);
C_ASSERT(offsetof(PhotoStraightLaserSpawnArgs, width) == 0x1c);
C_ASSERT(offsetof(PhotoStraightLaserSpawnArgs, speed) == 0x20);
C_ASSERT(offsetof(PhotoStraightLaserSpawnArgs, type) == 0x24);
C_ASSERT(offsetof(PhotoStraightLaserSpawnArgs, color) == 0x26);

} // namespace th095
