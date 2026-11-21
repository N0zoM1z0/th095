#include "EnemyManager.hpp"
#include "ecl/EclManager.hpp"
#include "ecl/EclOperands.hpp"
#include "PhotoEnemyLayout.hpp"
#include "ZunMath.hpp"
#include <math.h>

namespace th095
{
namespace EclHelpers
{
#define ReadInt(enemy, instruction, index) \
    ((instruction)->operandFlags & (1 << (index)) \
         ? EclOperands::ResolveInt(enemy, (instruction)->operands[index].asInt) \
         : (instruction)->operands[index].asInt)
#define ReadFloat(enemy, instruction, index) \
    ((instruction)->operandFlags & (1 << (index)) \
         ? enemy->ResolveFloat((instruction)->operands[index].asFloat) \
         : (instruction)->operands[index].asFloat)

// FUNCTION: TH095 0x00411150; TH08 0x00422020 is the source-shape oracle.
void __fastcall ConfigurePolarMotion(PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    f32 angle = AddNormalizeAngle(ReadFloat(enemy, instruction, 2), 0.0f);
    enemy->movementInterpolationDelta.x = cosf(angle) * ReadFloat(enemy, instruction, 3) * ReadInt(enemy, instruction, 0);
    enemy->movementInterpolationDelta.y = sinf(angle) * ReadFloat(enemy, instruction, 3) * ReadInt(enemy, instruction, 0);
    enemy->movementInterpolationDelta.z = 0.0f;
    enemy->movementInterpolationOrigin = enemy->worldPosition;
    enemy->movementTimer = (enemy->movementDuration = ReadInt(enemy, instruction, 0));
    enemy->control.movementEasing = ReadInt(enemy, instruction, 1);
    enemy->control.movementMode = PHOTO_ENEMY_MOVEMENT_INTERPOLATED;
    if (enemy->control.mirrorMovementX)
        enemy->movementInterpolationDelta.x = -enemy->movementInterpolationDelta.x;
}

// FUNCTION: TH095 0x00411390; TH08 0x00422260 is the source-shape oracle.
void __fastcall ConfigureRelativeMotion(PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    Float3 target;
    target.x = ReadFloat(enemy, instruction, 2);
    target.y = ReadFloat(enemy, instruction, 3);
    target.z = 0.0f;
    enemy->movementInterpolationDelta = target - enemy->worldPosition;
    enemy->movementInterpolationOrigin = enemy->position;
    enemy->movementTimer = (enemy->movementDuration = ReadInt(enemy, instruction, 0));
    enemy->control.movementEasing = ReadInt(enemy, instruction, 1);
    enemy->control.movementMode = PHOTO_ENEMY_MOVEMENT_INTERPOLATED;
    enemy->velocity = Float3(0.0f, 0.0f, 0.0f);
    if (enemy->control.mirrorMovementX)
        enemy->movementInterpolationDelta.x = -enemy->movementInterpolationDelta.x;
}
#undef ReadFloat
#undef ReadInt
}
}
