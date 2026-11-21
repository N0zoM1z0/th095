#pragma once

#include "AnmManager.hpp"
#include "PhotoEffectRuntime.hpp"
#include "PhotoStraightLaserArgs.hpp"

#include <string.h>

namespace th095
{

struct PhotoStraightLaserParameters : PhotoStraightLaserSpawnArgs
{
    PhotoStraightLaserParameters()
    {
        memset(this, 0, sizeof(*this));
    }
};

struct PhotoStraightLaserView : PhotoEffectBaseView
{
    PhotoStraightLaserParameters spawn;       // +0x050
    AnmVm bodyVm;                            // +0x078
    AnmVm tailVm;                            // +0x344

    PhotoStraightLaserView();
    i32 Initialize(void *args);
    i32 Update();
    i32 Draw();
    i32 DrawSecondary();
    i32 CountPhotoTargets(
        Float3 *position, Float3 *size, i32 capture);
    i32 CheckCollision(Float3 *position, Float3 *size, i32 capture);
    i32 CountNearbyTargets(Float3 *position, f32 radius);
};

typedef char PhotoStraightLaserSizeIs610[
    (sizeof(PhotoStraightLaserView) == 0x610) ? 1 : -1];
typedef char PhotoStraightLaserSpawnAt50[
    (offsetof(PhotoStraightLaserView, spawn) == 0x50) ? 1 : -1];
typedef char PhotoStraightLaserBodyVmAt78[
    (offsetof(PhotoStraightLaserView, bodyVm) == 0x78) ? 1 : -1];
typedef char PhotoStraightLaserTailVmAt344[
    (offsetof(PhotoStraightLaserView, tailVm) == 0x344) ? 1 : -1];

} // namespace th095
