#pragma once

#include "inttypes.hpp"

namespace th095
{

enum PhotoEnemyMovementMode
{
    PHOTO_ENEMY_MOVEMENT_VELOCITY = 0,
    PHOTO_ENEMY_MOVEMENT_POLAR = 1,
    PHOTO_ENEMY_MOVEMENT_INTERPOLATED = 2,
    PHOTO_ENEMY_MOVEMENT_ORBIT = 3,
};

enum PhotoEnemyMovementEasing
{
    PHOTO_ENEMY_EASING_LINEAR = 0,
    PHOTO_ENEMY_EASING_IN_QUADRATIC = 1,
    PHOTO_ENEMY_EASING_IN_CUBIC = 2,
    PHOTO_ENEMY_EASING_IN_QUARTIC = 3,
    PHOTO_ENEMY_EASING_OUT_QUADRATIC = 4,
    PHOTO_ENEMY_EASING_OUT_CUBIC = 5,
    PHOTO_ENEMY_EASING_OUT_QUARTIC = 6,
};

enum PhotoEnemyMovementControlMask
{
    PHOTO_ENEMY_MOVEMENT_MODE_SHIFT = 10,
    PHOTO_ENEMY_MOVEMENT_EASING_SHIFT = 12,
    PHOTO_ENEMY_MIRROR_MOVEMENT_X_SHIFT = 16,
    PHOTO_ENEMY_MOVEMENT_MODE_MASK = 0x00000c00,
};

// Shared vocabulary for the compact enemy control word at +0x2BF4.  This is
// a four-byte value type, not a second projection of the enemy allocation.
struct PhotoEnemyControlBits
{
    u32 active : 1;
    u32 photoTarget : 1;
    u32 collidable : 1;
    u32 unknown003 : 1;
    u32 hiddenFromDrawGroups : 1;
    u32 unknown005 : 3;
    u32 lifecycleState : 2;
    u32 movementMode : 2;
    u32 movementEasing : 3;
    u32 deferShotInstruction : 1;
    u32 mirrorMovementX : 1;
    u32 clampToMovementBounds : 1;
    u32 unknown018 : 4;
    u32 hasEnteredPlayfield : 1;
    u32 unknown023 : 1;
    u32 suppressEclCallStack : 1;
    u32 unknown025 : 1;
    u32 skipOffscreenCheck : 1;
    u32 unknown027 : 4;
    u32 alternateAnmBank : 1;
};

typedef char PhotoEnemyControlBitsSizeIs4[
    (sizeof(PhotoEnemyControlBits) == sizeof(u32)) ? 1 : -1];

} // namespace th095
