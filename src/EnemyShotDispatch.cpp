#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoBulletManagerLayout.hpp"
#include "PhotoEnemyEclAccess.hpp"
#include "PhotoPlayerRuntime.hpp"
#include "ecl/EclManager.hpp"
#include "ecl/EclOperands.hpp"

namespace th095
{
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

static __forceinline PhotoBulletSpawnDescriptor *TargetShotDescriptor(Enemy *enemy)
{
    return TH095_ECL_BULLET_DESCRIPTOR(enemy, PhotoBulletSpawnDescriptor);
}
static __forceinline f32 &TargetMinimumDistanceSquared(Enemy *enemy)
{
    return TH095_ECL_MINIMUM_PLAYER_DISTANCE_SQUARED(enemy);
}
static __forceinline const Float3 &TargetPlayerPosition()
{
    return reinterpret_cast<const PhotoPlayerRuntimeView *>(g_RuntimePlayerOwner)
        ->playerPosition;
}

void __fastcall DispatchShotInstruction(Enemy *enemy, EclRawInstruction *instruction)
{
    PhotoBulletSpawnDescriptor *descriptor;
    ShotArgs *args;
    i32 packed;

    args = reinterpret_cast<ShotArgs *>(instruction->operands);
    descriptor = TargetShotDescriptor(enemy);

    // Preserve the TH08-ancestral nested early-return source shape.
    if (TargetMinimumDistanceSquared(enemy) > 0.0f)
    {
        if (((enemy->worldPosition.x - TargetPlayerPosition().x) *
                 (enemy->worldPosition.x - TargetPlayerPosition().x) +
             (enemy->worldPosition.y - TargetPlayerPosition().y) *
                 (enemy->worldPosition.y - TargetPlayerPosition().y)) <
            TargetMinimumDistanceSquared(enemy))
            return;
    }

    reinterpret_cast<Float3 &>(descriptor->position) =
        enemy->worldPosition + enemy->shootOffset;

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
    descriptor->unknown1FA = 0;
    descriptor->transformFlags = args->transformFlags;
    packed = args->color;
    descriptor->color = (instruction->operandFlags & 2)
        ? EclOperands::ResolveInt(enemy, packed)
        : packed;
    g_PhotoBulletManager->SpawnBulletPattern(descriptor);
}
}
}
