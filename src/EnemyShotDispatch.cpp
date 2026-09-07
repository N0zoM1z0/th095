#include "EnemyManager.hpp"
#include "ecl/EclManager.hpp"
#include "ecl/EclOperands.hpp"

namespace th095
{
// Private target-local ABI view for this ECL lane.
struct PhotoBulletTransformRecordView
{
    u8 bytes[0x18];
};
struct EnemyShotDescriptorView
{
    i16 bulletType;
    i16 color;
    Float3 position;
    f32 angle;
    f32 angleStep;
    f32 speed1;
    f32 speed2;
    PhotoBulletTransformRecordView transforms[18];
    u8 laserFields[0x24];
    i16 count1;
    i16 count2;
    u16 aimMode;
    u16 unconsumedWord1FA;
    u32 transformFlags;
    i32 spawnSound;
    i32 transformSound;
    i32 transformStartIndex;
    void *templateSprites;
};
typedef char ShotDescriptorSizeIs210[(sizeof(EnemyShotDescriptorView) == 0x210) ? 1 : -1];

struct Player;

struct EnemyShotBulletManagerView
{
    i32 SpawnBulletPattern(EnemyShotDescriptorView *descriptor);
};
extern EnemyShotBulletManagerView *g_EnemyShotBulletManager;
extern u8 *g_EnemyShotPlayer;

namespace EclRunHigh
{
struct ShotArgs
{
    i16 bulletType;
    i16 color;
    i32 count1;
    i32 count2;
    f32 speed1;
    f32 speed2;
    f32 angle;
    f32 angleStep;
    u32 transformFlags;
};
typedef char ShotArgsSizeIs20[(sizeof(ShotArgs) == 0x20) ? 1 : -1];

static __forceinline EnemyShotDescriptorView *TargetShotDescriptor(Enemy *enemy)
{
    return reinterpret_cast<EnemyShotDescriptorView *>(reinterpret_cast<u8 *>(enemy) + 0x298c);
}
static __forceinline Float3 &TargetWorldPosition(Enemy *enemy)
{
    return *reinterpret_cast<Float3 *>(reinterpret_cast<u8 *>(enemy) + 0x28f4);
}
static __forceinline Float3 &TargetShootOffset(Enemy *enemy)
{
    return *reinterpret_cast<Float3 *>(reinterpret_cast<u8 *>(enemy) + 0x2924);
}
static __forceinline f32 &TargetMinimumDistanceSquared(Enemy *enemy)
{
    return *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(enemy) + 0x2c4c);
}
static __forceinline Float3 &TargetPlayerPosition()
{
    return *reinterpret_cast<Float3 *>(g_EnemyShotPlayer + 0x1e30);
}

void __fastcall DispatchShotInstruction(Enemy *enemy, EclRawInstruction *instruction)
{
    EnemyShotDescriptorView *descriptor;
    ShotArgs *args;
    i32 packed;

    args = reinterpret_cast<ShotArgs *>(instruction->operands);
    descriptor = TargetShotDescriptor(enemy);

    // Preserve the TH08-ancestral nested early-return source shape.
    if (TargetMinimumDistanceSquared(enemy) > 0.0f)
    {
        if (((TargetWorldPosition(enemy).x - TargetPlayerPosition().x) *
         (TargetWorldPosition(enemy).x - TargetPlayerPosition().x) +
     (TargetWorldPosition(enemy).y - TargetPlayerPosition().y) *
         (TargetWorldPosition(enemy).y - TargetPlayerPosition().y)) < TargetMinimumDistanceSquared(enemy))
            return;
    }

        descriptor->position = TargetWorldPosition(enemy) + TargetShootOffset(enemy);

        packed = args->bulletType;
        descriptor->bulletType = (instruction->operandFlags & 1)
            ? EclOperands::ResolveInt(enemy, packed)
            : packed;
        descriptor->aimMode = instruction->opcode - 0x56;
        descriptor->count1 = (instruction->operandFlags & 4)
            ? EclOperands::ResolveInt(enemy, args->count1)
            : args->count1;
        descriptor->count2 = (instruction->operandFlags & 8)
            ? EclOperands::ResolveInt(enemy, args->count2)
            : args->count2;
        descriptor->angle = (instruction->operandFlags & 0x40)
            ? enemy->ResolveFloat(args->angle)
            : args->angle;
        descriptor->speed1 = (instruction->operandFlags & 0x10)
            ? enemy->ResolveFloat(args->speed1)
            : args->speed1;
        descriptor->angleStep = (instruction->operandFlags & 0x80)
            ? enemy->ResolveFloat(args->angleStep)
            : args->angleStep;
        descriptor->speed2 = (instruction->operandFlags & 0x20)
            ? enemy->ResolveFloat(args->speed2)
            : args->speed2;
        descriptor->unconsumedWord1FA = 0;
        descriptor->transformFlags = args->transformFlags;
        packed = args->color;
        descriptor->color = (instruction->operandFlags & 2)
            ? EclOperands::ResolveInt(enemy, packed)
            : packed;
        g_EnemyShotBulletManager->SpawnBulletPattern(descriptor);
}
}
}