#pragma once

#include "inttypes.hpp"

namespace th095
{

// Dependency-light bridge for ECL resolver symbols that still receive the
// legacy Enemy* ABI spelling.  This is not a second object layout: every
// offset is pinned back to the canonical PhotoEnemyView by assertions in
// PhotoEnemy.hpp, and every build profile expands the same expression.
enum PhotoEnemyEclOperandOffset
{
    PHOTO_ENEMY_ECL_LIFE_OFFSET = 0x2958,
    PHOTO_ENEMY_ECL_SCORE_OFFSET = 0x2964,
    PHOTO_ENEMY_ECL_TIMER_CURRENT_OFFSET = 0x2974,
    PHOTO_ENEMY_ECL_ITEM_DROP_TYPE_OFFSET = 0x2bd8,
    PHOTO_ENEMY_ECL_PHOTO_TARGET_SLOT_OFFSET = 0x2be5,
    PHOTO_ENEMY_ECL_UNKNOWN_2C50_OFFSET = 0x2c50,
    PHOTO_ENEMY_ECL_SCHEDULED_FRAMES_OFFSET = 0x2c54,
};

} // namespace th095

#define TH095_PHOTO_ENEMY_I32(owner, offset) \
    (*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(owner) + (offset)))
#define TH095_PHOTO_ENEMY_U8(owner, offset) \
    (*reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(owner) + (offset)))
#define TH095_ECL_ENEMY_LIFE(owner) \
    TH095_PHOTO_ENEMY_I32((owner), th095::PHOTO_ENEMY_ECL_LIFE_OFFSET)
#define TH095_ECL_TIMER_CURRENT(owner) \
    TH095_PHOTO_ENEMY_I32((owner), th095::PHOTO_ENEMY_ECL_TIMER_CURRENT_OFFSET)
#define TH095_ECL_ENEMY_SCORE(owner) \
    TH095_PHOTO_ENEMY_I32((owner), th095::PHOTO_ENEMY_ECL_SCORE_OFFSET)
#define TH095_ECL_ITEM_DROP_TYPE(owner) \
    TH095_PHOTO_ENEMY_I32((owner), th095::PHOTO_ENEMY_ECL_ITEM_DROP_TYPE_OFFSET)
#define TH095_ECL_SCHEDULED_FRAME(owner, index) \
    TH095_PHOTO_ENEMY_I32( \
        (owner), \
        th095::PHOTO_ENEMY_ECL_SCHEDULED_FRAMES_OFFSET + \
            sizeof(i32) * (index))
#define TH095_ECL_SCHEDULED_FRAME0(owner) TH095_ECL_SCHEDULED_FRAME(owner, 0)
#define TH095_ECL_SCHEDULED_FRAME1(owner) TH095_ECL_SCHEDULED_FRAME(owner, 1)
#define TH095_ECL_SCHEDULED_FRAME2(owner) TH095_ECL_SCHEDULED_FRAME(owner, 2)
#define TH095_ECL_SCHEDULED_FRAME3(owner) TH095_ECL_SCHEDULED_FRAME(owner, 3)
#define TH095_ECL_PHOTO_TARGET_SLOT(owner) \
    TH095_PHOTO_ENEMY_U8( \
        (owner), th095::PHOTO_ENEMY_ECL_PHOTO_TARGET_SLOT_OFFSET)
