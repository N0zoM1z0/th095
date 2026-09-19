#pragma once

#include "ZunMath.hpp"
#include "inttypes.hpp"

#include <stddef.h>

namespace th095
{

// Spawn packet for PhotoEffectManagerView::Spawn @ 0x0041DBD0 kind 1.  The
// manager forwards all 0x48 bytes to PhotoRotatingLaserView; Initialize @
// 0x0041F380 and Update @ 0x0041F550 independently establish the fields.
// Keep this owner constructor-free because all ECL producers explicitly zero
// their stack packet before publishing it.
struct PhotoRotatingLaserSpawnArgs
{
    Float3 position;              // +0x00
    Float3 velocity;              // +0x0c
    f32 angle;                    // +0x18
    f32 angularVelocity;          // +0x1c
    f32 maximumLength;            // +0x20
    f32 initialLength;            // +0x24
    f32 maximumWidth;             // +0x28
    f32 speed;                    // +0x2c
    i32 startupDuration;          // +0x30
    i32 growthDuration;           // +0x34
    i32 sustainDuration;          // +0x38
    i32 fadeDuration;             // +0x3c
    i16 type;                     // +0x40
    i16 color;                    // +0x42
    union
    {
        u32 flags;                // +0x44
        struct
        {
            u32 followPhotoTarget : 1;
            u32 unknownFlags001_031 : 31;
        };
    };
};

C_ASSERT(sizeof(PhotoRotatingLaserSpawnArgs) == 0x48);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, velocity) == 0x0c);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, angle) == 0x18);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, angularVelocity) == 0x1c);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, maximumLength) == 0x20);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, initialLength) == 0x24);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, maximumWidth) == 0x28);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, speed) == 0x2c);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, startupDuration) == 0x30);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, growthDuration) == 0x34);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, sustainDuration) == 0x38);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, fadeDuration) == 0x3c);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, type) == 0x40);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, color) == 0x42);
C_ASSERT(offsetof(PhotoRotatingLaserSpawnArgs, flags) == 0x44);

} // namespace th095
