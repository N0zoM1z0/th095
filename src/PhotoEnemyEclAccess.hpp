#pragma once

#include "PhotoEnemyControl.hpp"
#include "inttypes.hpp"

namespace th095
{

// Dependency-light bridge for ECL symbols that still receive the legacy
// Enemy* ABI spelling.  This is not a second object layout: every offset is
// pinned back to the canonical PhotoEnemyView by assertions in PhotoEnemy.hpp,
// and every build profile expands the same expression.
enum PhotoEnemyEclOperandOffset
{
    PHOTO_ENEMY_ECL_VM_ROTATION_Z_OFFSET = 0x28,
    PHOTO_ENEMY_ECL_ANM_HANDLES_OFFSET = 0x2d4,
    PHOTO_ENEMY_ECL_PHOTO_MARKER_TIMER_OFFSET = 0x2bfc,
    PHOTO_ENEMY_ECL_DRAW_GROUP_OFFSET = 0x2c0b,
    PHOTO_ENEMY_ECL_SPECIAL_ANM_SCRIPT_OFFSET = 0x2c18,
    PHOTO_ENEMY_ECL_PHOTO_PULSE_VM_OFFSET = 0x2c1c,
    PHOTO_ENEMY_ECL_PHOTO_MARKER_VM_OFFSET = 0x2c20,
    PHOTO_ENEMY_ECL_PHOTO_PULSE_TIMER_OFFSET = 0x2c24,
    PHOTO_ENEMY_ECL_PHOTO_PULSE_DURATION_TIMER_OFFSET = 0x2c30,
    PHOTO_ENEMY_ECL_TRAIL_VERTICES_OFFSET = 0x376c,
    PHOTO_ENEMY_ECL_PHOTO_ANM_CONFIG_OFFSET = 0x4ca4,
    PHOTO_ENEMY_ECL_TIMER_4CAC_OFFSET = 0x4cac,
    PHOTO_ENEMY_ECL_ATTACHED_VM_OFFSET = 0x4cbc,
};

enum PhotoEnemyTrailFlag
{
    PHOTO_ENEMY_TRAIL_INITIALIZE_STRIP = 1 << 3,
};

struct PhotoEnemyTrailConfig
{
    u8 flags;
    u8 unknown01;
    i16 length;
    i16 unknown04;
    i16 sampleStride;
};

} // namespace th095

#define TH095_PHOTO_ENEMY_I32(owner, offset) \
    (*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(owner) + (offset)))
#define TH095_PHOTO_ENEMY_I16(owner, offset) \
    (*reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(owner) + (offset)))
#define TH095_PHOTO_ENEMY_I8(owner, offset) \
    (*reinterpret_cast<i8 *>(reinterpret_cast<u8 *>(owner) + (offset)))
#define TH095_PHOTO_ENEMY_U8(owner, offset) \
    (*reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(owner) + (offset)))
#define TH095_PHOTO_ENEMY_F32(owner, offset) \
    (*reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(owner) + (offset)))
#define TH095_PHOTO_ENEMY_POINTER(owner, offset, type) \
    (*reinterpret_cast<type **>( \
        reinterpret_cast<u8 *>(owner) + (offset)))
#define TH095_PHOTO_ENEMY_OBJECT(owner, offset, type) \
    (*reinterpret_cast<type *>(reinterpret_cast<u8 *>(owner) + (offset)))
#define TH095_PHOTO_ENEMY_OBJECT_PTR(owner, offset, type) \
    (reinterpret_cast<type *>(reinterpret_cast<u8 *>(owner) + (offset)))
#define TH095_ECL_ENEMY_VM_ROTATION_Z(owner) \
    TH095_PHOTO_ENEMY_F32( \
        (owner), th095::PHOTO_ENEMY_ECL_VM_ROTATION_Z_OFFSET)
#define TH095_ECL_ENEMY_ANM_HANDLE(owner, index, type) \
    TH095_PHOTO_ENEMY_OBJECT( \
        (owner), \
        th095::PHOTO_ENEMY_ECL_ANM_HANDLES_OFFSET + sizeof(i32) * (index), \
        type)
#define TH095_ECL_PHOTO_MARKER_TIMER(owner) \
    TH095_PHOTO_ENEMY_OBJECT( \
        (owner), th095::PHOTO_ENEMY_ECL_PHOTO_MARKER_TIMER_OFFSET, ZunTimer)
#define TH095_ECL_DRAW_GROUP(owner) \
    TH095_PHOTO_ENEMY_U8((owner), th095::PHOTO_ENEMY_ECL_DRAW_GROUP_OFFSET)
#define TH095_ECL_SPECIAL_ANM_SCRIPT(owner) \
    TH095_PHOTO_ENEMY_I16((owner), th095::PHOTO_ENEMY_ECL_SPECIAL_ANM_SCRIPT_OFFSET)
#define TH095_ECL_TRAIL_CONFIG(owner) \
    TH095_PHOTO_ENEMY_OBJECT((owner), th095::PHOTO_ENEMY_ECL_PHOTO_ANM_CONFIG_OFFSET, \
                             th095::PhotoEnemyTrailConfig)
#define TH095_ECL_PHOTO_PULSE_VM(owner, type) \
    TH095_PHOTO_ENEMY_OBJECT( \
        (owner), th095::PHOTO_ENEMY_ECL_PHOTO_PULSE_VM_OFFSET, type)
#define TH095_ECL_PHOTO_PULSE_TIMER(owner) \
    TH095_PHOTO_ENEMY_OBJECT( \
        (owner), th095::PHOTO_ENEMY_ECL_PHOTO_PULSE_TIMER_OFFSET, ZunTimer)
#define TH095_ECL_PHOTO_PULSE_DURATION_TIMER(owner) \
    TH095_PHOTO_ENEMY_OBJECT( \
        (owner), th095::PHOTO_ENEMY_ECL_PHOTO_PULSE_DURATION_TIMER_OFFSET, \
        ZunTimer)
#define TH095_ECL_ATTACHED_VM(owner, type) \
    TH095_PHOTO_ENEMY_OBJECT( \
        (owner), th095::PHOTO_ENEMY_ECL_ATTACHED_VM_OFFSET, type)
