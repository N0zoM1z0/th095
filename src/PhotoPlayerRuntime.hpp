#ifndef TH095_PHOTO_PLAYER_RUNTIME_HPP
#define TH095_PHOTO_PLAYER_RUNTIME_HPP

#include "ZunMath.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

namespace th095
{

// Shared production view of the target PlayerInf object published through
// g_RuntimePlayerOwner.  Only offsets used by the canonical angle/collision/
// death cluster are named here; every named field is independently fixed by
// exact code at 0x004303E0, 0x00430450, and 0x004306D0.
struct PhotoPlayerRuntimeView
{
    i32 mode;                                      // +0x0000
    u8 unknown0004[0x03a8 - 0x0004];
    Float3 hurtboxBoundsMin;                       // +0x03a8
    Float3 hurtboxBoundsMax;                       // +0x03b4
    u8 unknown03c0[0x0420 - 0x03c0];
    ZunTimer completionTimer;                      // +0x0420
    u8 unknown042c[0x1e30 - 0x042c];
    Float3 playerPosition;                         // +0x1e30

    f32 AngleFromPoint(Float3 *position);
    i32 CheckBulletCollision(Float3 *position, Float3 *size);
    void Die();
};

typedef char PhotoPlayerRuntimeModeAt0000[
    (offsetof(PhotoPlayerRuntimeView, mode) == 0x0000) ? 1 : -1];
typedef char PhotoPlayerRuntimeHurtboxMinAt03A8[
    (offsetof(PhotoPlayerRuntimeView, hurtboxBoundsMin) == 0x03a8) ? 1 : -1];
typedef char PhotoPlayerRuntimeHurtboxMaxAt03B4[
    (offsetof(PhotoPlayerRuntimeView, hurtboxBoundsMax) == 0x03b4) ? 1 : -1];
typedef char PhotoPlayerRuntimeCompletionTimerAt0420[
    (offsetof(PhotoPlayerRuntimeView, completionTimer) == 0x0420) ? 1 : -1];
typedef char PhotoPlayerRuntimePositionAt1E30[
    (offsetof(PhotoPlayerRuntimeView, playerPosition) == 0x1e30) ? 1 : -1];

} // namespace th095

#endif
