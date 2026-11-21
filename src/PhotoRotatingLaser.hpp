#pragma once

#include "AnmManager.hpp"
#include "PhotoEffectRuntime.hpp"
#include "PhotoRotatingLaserArgs.hpp"

#include <string.h>

namespace th095
{

// Effect construction sets a default before Initialize copies the packet.
struct PhotoRotatingLaserParameters : PhotoRotatingLaserSpawnArgs
{
    PhotoRotatingLaserParameters()
    {
        memset(this, 0, sizeof(*this));
        this->speed = 8.0f;
    }
};

struct PhotoRotatingLaserView : PhotoEffectBaseView
{
    PhotoRotatingLaserParameters spawn;     // +0x050
    AnmVm bodyVm;                            // +0x098
    AnmVm tailVm;                            // +0x364

    PhotoRotatingLaserView();
    i32 Initialize(void *args);
    i32 Update();
    i32 Draw();
    i32 DrawSecondary();
    i32 CountPhotoTargets(
        Float3 *position, Float3 *size, i32 capture);
    i32 CheckCollision(Float3 *position, Float3 *size, i32 capture);
    i32 CountNearbyTargets(Float3 *position, f32 radius);
};

typedef char PhotoRotatingLaserSizeIs630[
    (sizeof(PhotoRotatingLaserView) == 0x630) ? 1 : -1];
typedef char PhotoRotatingLaserSpawnAt50[
    (offsetof(PhotoRotatingLaserView, spawn) == 0x50) ? 1 : -1];
typedef char PhotoRotatingLaserBodyVmAt98[
    (offsetof(PhotoRotatingLaserView, bodyVm) == 0x98) ? 1 : -1];
typedef char PhotoRotatingLaserTailVmAt364[
    (offsetof(PhotoRotatingLaserView, tailVm) == 0x364) ? 1 : -1];

} // namespace th095
