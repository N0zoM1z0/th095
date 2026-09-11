#pragma once

#include "inttypes.hpp"
#include <stddef.h>

namespace th095
{

// TH095 RunEcl and its sibling movement/shot helpers use a compact enemy ABI
// whose control word is at +0x2BF4. It is not EnemyManager.hpp::Enemy::flags1:
// that later/larger shared view places its flags at a different offset and
// uses different movement-bit positions. Name only bits with TH095-local
// producer/consumer evidence; unresolved ECL-only spellings remain local to
// their exact-facing translation unit rather than being promoted here.
struct EnemyEclControlBits
{
    u32 active : 1;
    u32 photoTargetRegistered : 1;
    u32 collidable : 1;
    u32 unknown3 : 1;
    u32 hiddenFromDrawGroups : 1;
    u32 unknown5 : 1;
    u32 unknown6 : 1;
    u32 unknown7 : 1;
    u32 lifecycleState : 2;
    u32 movementMode : 2;
    u32 movementEasing : 3;
    u32 deferShotInstruction : 1;
    u32 mirrorMovementX : 1;
    u32 clampToMovementBounds : 1;
    u32 unknown18_21 : 4;
    u32 hasEnteredPlayfield : 1;
    u32 unknown23 : 1;
    u32 suppressEclCallStack : 1;
    u32 unknown25 : 1;
    u32 skipOffscreenCheck : 1;
    u32 unknown27_30 : 4;
    u32 alternateAnmBank : 1;
};
typedef char EnemyEclControlBitsSizeIs4[
    (sizeof(EnemyEclControlBits) == 4) ? 1 : -1];

struct EnemyEclRuntimeView
{
    u8 unknown0000[0x2bf4];
    union
    {
        u32 controlWord;
        EnemyEclControlBits control;
    };
};
typedef char EnemyEclRuntimeControlAt2BF4[
    (offsetof(EnemyEclRuntimeView, controlWord) == 0x2bf4) ? 1 : -1];

#define TH095_ENEMY_ECL_CONTROL_WORD(enemy) \
    (reinterpret_cast<EnemyEclRuntimeView *>(enemy)->controlWord)
#define TH095_ENEMY_ECL_CONTROL_BITS(enemy) \
    (reinterpret_cast<EnemyEclRuntimeView *>(enemy)->control)

} // namespace th095
