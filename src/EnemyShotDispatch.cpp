#include "AnmManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoBulletManagerLayout.hpp"
#include "PhotoEnemyLayout.hpp"
#include "PhotoGamePlayer.hpp"
#include "EclInstruction.hpp"
#include "ecl/EclOperands.hpp"

namespace th095
{
namespace EclRunHigh
{
void __fastcall DispatchShotInstruction(PhotoEnemyView *enemy, EclShotInstruction *instruction)
{
    PhotoBulletSpawnDescriptor *descriptor;
    EclShotOperands *args;
    i32 packed;

    args = &instruction->operands;
    descriptor = &enemy->bulletSpawnDescriptor;

    // Preserve the TH08-ancestral nested early-return source shape.
    if (enemy->minimumPlayerDistanceSquared > 0.0f)
    {
        if (((enemy->worldPosition.x - g_RuntimePlayerOwner->playerPosition.x) *
                 (enemy->worldPosition.x - g_RuntimePlayerOwner->playerPosition.x) +
             (enemy->worldPosition.y - g_RuntimePlayerOwner->playerPosition.y) *
                 (enemy->worldPosition.y - g_RuntimePlayerOwner->playerPosition.y)) <
            enemy->minimumPlayerDistanceSquared)
            return;
    }

    reinterpret_cast<Float3 &>(descriptor->position) =
        enemy->worldPosition + enemy->shootOffset;

    packed = args->bulletType;
    descriptor->bulletType = (instruction->header.operandFlags & 1)
        ? EclOperands::ResolveInt(enemy, packed)
        : packed;
    descriptor->aimMode = instruction->header.opcode - 0x56;
    descriptor->count1 = (instruction->header.operandFlags & 4)
        ? EclOperands::ResolveInt(enemy, args->count1)
        : args->count1;
    descriptor->count2 = (instruction->header.operandFlags & 8)
        ? EclOperands::ResolveInt(enemy, args->count2)
        : args->count2;
    descriptor->angle = (instruction->header.operandFlags & 0x40)
        ? enemy->ResolveFloat(args->angle)
        : args->angle;
    descriptor->speed1 = (instruction->header.operandFlags & 0x10)
        ? enemy->ResolveFloat(args->speed1)
        : args->speed1;
    descriptor->angleStep = (instruction->header.operandFlags & 0x80)
        ? enemy->ResolveFloat(args->angleStep)
        : args->angleStep;
    descriptor->speed2 = (instruction->header.operandFlags & 0x20)
        ? enemy->ResolveFloat(args->speed2)
        : args->speed2;
    descriptor->unknown1FA = 0;
    descriptor->transformFlags = args->transformFlags;
    packed = args->color;
    descriptor->color = (instruction->header.operandFlags & 2)
        ? EclOperands::ResolveInt(enemy, packed)
        : packed;
    g_PhotoBulletManager->SpawnBulletPattern(descriptor);
}
}
}
