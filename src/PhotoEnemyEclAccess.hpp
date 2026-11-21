#pragma once

#include "PhotoEnemyControl.hpp"
#include "inttypes.hpp"

namespace th095
{

enum
{
    PHOTO_ENEMY_SCHEDULE_SLOT_COUNT = 10,
    PHOTO_ENEMY_SCHEDULE_INACTIVE = -1,
};

// Opcode 115 writes the full word. Dispatch consumes its low signed half.
union PhotoEnemyScheduledCall
{
    i32 rawValue;
    struct
    {
        i16 subroutineId;
        i16 unknown02;
    };
};

struct PhotoEnemyEclSchedule
{
    i32 photoIndices[PHOTO_ENEMY_SCHEDULE_SLOT_COUNT];
    PhotoEnemyScheduledCall calls[PHOTO_ENEMY_SCHEDULE_SLOT_COUNT];
};

// Dependency-light bridge for ECL symbols that still receive the legacy
// Enemy* ABI spelling.  This is not a second object layout: every offset is
// pinned back to the canonical PhotoEnemyView by assertions in PhotoEnemy.hpp,
// and every build profile expands the same expression.
enum PhotoEnemyEclOperandOffset
{
    PHOTO_ENEMY_ECL_VM_ROTATION_Z_OFFSET = 0x28,
    PHOTO_ENEMY_ECL_ANM_HANDLES_OFFSET = 0x2d4,
    PHOTO_ENEMY_ECL_PHOTO_CAPTURE_SUBROUTINE_OFFSET = 0x285a,
    PHOTO_ENEMY_ECL_LIFE_OFFSET = 0x2958,
    PHOTO_ENEMY_ECL_MAXIMUM_LIFE_OFFSET = 0x295c,
    PHOTO_ENEMY_ECL_PHASE_STARTING_LIFE_OFFSET = 0x2960,
    PHOTO_ENEMY_ECL_SCORE_OFFSET = 0x2964,
    PHOTO_ENEMY_ECL_TIMER_OFFSET = 0x296c,
    PHOTO_ENEMY_ECL_TIMER_CURRENT_OFFSET = 0x2974,
    PHOTO_ENEMY_ECL_BULLET_DESCRIPTOR_OFFSET = 0x298c,
    PHOTO_ENEMY_ECL_BULLET_TRANSFORMS_OFFSET = 0x29ac,
    PHOTO_ENEMY_ECL_BULLET_TRANSFORM_FLAGS_OFFSET = 0x2b88,
    PHOTO_ENEMY_ECL_BULLET_SPAWN_SOUND_OFFSET = 0x2b8c,
    PHOTO_ENEMY_ECL_BULLET_TRANSFORM_SOUND_OFFSET = 0x2b90,
    PHOTO_ENEMY_ECL_ITEM_DROP_TYPE_OFFSET = 0x2bd8,
    PHOTO_ENEMY_ECL_PHOTO_TARGET_SLOT_OFFSET = 0x2be5,
    PHOTO_ENEMY_ECL_CONTROL_OFFSET = 0x2bf4,
    PHOTO_ENEMY_ECL_SECONDARY_CONTROL_OFFSET = 0x2bf8,
    PHOTO_ENEMY_ECL_PHOTO_MARKER_TIMER_OFFSET = 0x2bfc,
    PHOTO_ENEMY_ECL_DRAW_GROUP_OFFSET = 0x2c0b,
    PHOTO_ENEMY_ECL_SPECIAL_ANM_SCRIPT_OFFSET = 0x2c18,
    PHOTO_ENEMY_ECL_PHOTO_PULSE_VM_OFFSET = 0x2c1c,
    PHOTO_ENEMY_ECL_PHOTO_MARKER_VM_OFFSET = 0x2c20,
    PHOTO_ENEMY_ECL_PHOTO_PULSE_TIMER_OFFSET = 0x2c24,
    PHOTO_ENEMY_ECL_PHOTO_PULSE_DURATION_TIMER_OFFSET = 0x2c30,
    PHOTO_ENEMY_ECL_MOVEMENT_BOUNDS_OFFSET = 0x2c3c,
    PHOTO_ENEMY_ECL_MINIMUM_PLAYER_DISTANCE_SQUARED_OFFSET = 0x2c4c,
    PHOTO_ENEMY_ECL_UNKNOWN_2C50_OFFSET = 0x2c50,
    PHOTO_ENEMY_ECL_SCHEDULE_OFFSET = 0x2c54,
    PHOTO_ENEMY_ECL_PENDING_CALLBACK_FRAME_OFFSET = 0x2ca4,
    PHOTO_ENEMY_ECL_PENDING_CALLBACK_SUBROUTINE_OFFSET = 0x2ca8,
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

// Dependency-light access to the two manager fields consumed by the operand
// resolver family.  PhotoEnemyManagerView remains the only object-layout
// owner; its offsetof assertions pin these constants back to that declaration.
// This opaque spelling keeps the legacy Enemy ABI header out of the canonical
// manager header while giving exact and normal builds the same source path.

enum PhotoEnemyEclOperandRuntimeOffset
{
    PHOTO_ENEMY_ECL_MANAGER_OFFSET = 0x4df4,
    PHOTO_ENEMY_ECL_PHOTO_TARGETS_OFFSET = 0x26ae00,
};

} // namespace th095

#define TH095_PHOTO_ENEMY_I32(owner, offset) \
    (*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(owner) + (offset)))
#define TH095_PHOTO_ENEMY_U32(owner, offset) \
    (*reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(owner) + (offset)))
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
#define TH095_ECL_PHOTO_CAPTURE_SUBROUTINE(owner) \
    TH095_PHOTO_ENEMY_I16( \
        (owner), th095::PHOTO_ENEMY_ECL_PHOTO_CAPTURE_SUBROUTINE_OFFSET)
#define TH095_ECL_ENEMY_LIFE(owner) \
    TH095_PHOTO_ENEMY_I32((owner), th095::PHOTO_ENEMY_ECL_LIFE_OFFSET)
#define TH095_ECL_ENEMY_MAXIMUM_LIFE(owner) \
    TH095_PHOTO_ENEMY_I32( \
        (owner), th095::PHOTO_ENEMY_ECL_MAXIMUM_LIFE_OFFSET)
#define TH095_ECL_ENEMY_PHASE_STARTING_LIFE(owner) \
    TH095_PHOTO_ENEMY_I32( \
        (owner), th095::PHOTO_ENEMY_ECL_PHASE_STARTING_LIFE_OFFSET)
#define TH095_ECL_TIMER(owner) \
    TH095_PHOTO_ENEMY_OBJECT( \
        (owner), th095::PHOTO_ENEMY_ECL_TIMER_OFFSET, ZunTimer)
#define TH095_ECL_TIMER_CURRENT(owner) \
    TH095_PHOTO_ENEMY_I32((owner), th095::PHOTO_ENEMY_ECL_TIMER_CURRENT_OFFSET)
#define TH095_ECL_ENEMY_SCORE(owner) \
    TH095_PHOTO_ENEMY_I32((owner), th095::PHOTO_ENEMY_ECL_SCORE_OFFSET)
#define TH095_ECL_ITEM_DROP_TYPE(owner) \
    TH095_PHOTO_ENEMY_I32((owner), th095::PHOTO_ENEMY_ECL_ITEM_DROP_TYPE_OFFSET)
#define TH095_ECL_BULLET_DESCRIPTOR(owner, type) \
    TH095_PHOTO_ENEMY_OBJECT_PTR( \
        (owner), th095::PHOTO_ENEMY_ECL_BULLET_DESCRIPTOR_OFFSET, type)
#define TH095_ECL_BULLET_TRANSFORM(owner, index, type) \
    TH095_PHOTO_ENEMY_OBJECT_PTR( \
        (owner), \
        th095::PHOTO_ENEMY_ECL_BULLET_TRANSFORMS_OFFSET + \
            sizeof(type) * (index), \
        type)
#define TH095_ECL_BULLET_TRANSFORM_FLAGS(owner) \
    TH095_PHOTO_ENEMY_U32( \
        (owner), th095::PHOTO_ENEMY_ECL_BULLET_TRANSFORM_FLAGS_OFFSET)
#define TH095_ECL_BULLET_SPAWN_SOUND(owner) \
    TH095_PHOTO_ENEMY_I32( \
        (owner), th095::PHOTO_ENEMY_ECL_BULLET_SPAWN_SOUND_OFFSET)
#define TH095_ECL_BULLET_TRANSFORM_SOUND(owner) \
    TH095_PHOTO_ENEMY_I32( \
        (owner), th095::PHOTO_ENEMY_ECL_BULLET_TRANSFORM_SOUND_OFFSET)
#define TH095_ECL_CONTROL_WORD(owner) \
    TH095_PHOTO_ENEMY_U32((owner), th095::PHOTO_ENEMY_ECL_CONTROL_OFFSET)
#define TH095_ECL_CONTROL_BITS(owner) \
    TH095_PHOTO_ENEMY_OBJECT( \
        (owner), th095::PHOTO_ENEMY_ECL_CONTROL_OFFSET, \
        th095::PhotoEnemyControlBits)
#define TH095_ECL_SECONDARY_CONTROL_WORD(owner) \
    TH095_PHOTO_ENEMY_U32( \
        (owner), th095::PHOTO_ENEMY_ECL_SECONDARY_CONTROL_OFFSET)
#define TH095_ECL_SECONDARY_CONTROL_BITS(owner) \
    TH095_PHOTO_ENEMY_OBJECT( \
        (owner), th095::PHOTO_ENEMY_ECL_SECONDARY_CONTROL_OFFSET, \
        th095::PhotoEnemySecondaryControlBits)
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
#define TH095_ECL_MOVEMENT_BOUNDS(owner, type) \
    TH095_PHOTO_ENEMY_OBJECT( \
        (owner), th095::PHOTO_ENEMY_ECL_MOVEMENT_BOUNDS_OFFSET, type)
// Keep the dereference itself unparenthesized.  In RunEcl opcode 82, VC7.1
// otherwise schedules the second owner load between x87 fld/fmul; this source
// shape reproduces the target's two owner loads before the x87 pair.  The
// canonical PhotoEnemyView assertion independently pins the literal offset.
#define TH095_ECL_MINIMUM_PLAYER_DISTANCE_SQUARED(owner) \
    *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(owner) + \
                             th095::PHOTO_ENEMY_ECL_MINIMUM_PLAYER_DISTANCE_SQUARED_OFFSET)
#define TH095_ECL_SCHEDULE(owner) \
    TH095_PHOTO_ENEMY_OBJECT( \
        (owner), th095::PHOTO_ENEMY_ECL_SCHEDULE_OFFSET, \
        th095::PhotoEnemyEclSchedule)
#define TH095_ECL_PENDING_CALLBACK_FRAME(owner) \
    TH095_PHOTO_ENEMY_I32( \
        (owner), th095::PHOTO_ENEMY_ECL_PENDING_CALLBACK_FRAME_OFFSET)
#define TH095_ECL_PENDING_CALLBACK_SUBROUTINE(owner) \
    TH095_PHOTO_ENEMY_I32( \
        (owner), th095::PHOTO_ENEMY_ECL_PENDING_CALLBACK_SUBROUTINE_OFFSET)
#define TH095_ECL_ATTACHED_VM(owner, type) \
    TH095_PHOTO_ENEMY_OBJECT( \
        (owner), th095::PHOTO_ENEMY_ECL_ATTACHED_VM_OFFSET, type)
#define TH095_ECL_PHOTO_TARGET_SLOT(owner) \
    TH095_PHOTO_ENEMY_U8( \
        (owner), th095::PHOTO_ENEMY_ECL_PHOTO_TARGET_SLOT_OFFSET)
