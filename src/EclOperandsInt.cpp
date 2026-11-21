#include "AnmManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoEnemyEclManager.hpp"
#include "PhotoEnemyManagerLayout.hpp"
#include "PhotoGamePlayer.hpp"
#include "ecl/EclOperands.hpp"

#include <d3dx8.h>

namespace th095
{

namespace EclOperands
{

// FUNCTION: TH095 0x0040FAE0.  TH08's resolver supplies the source-shape
// oracle, while every selector and field below is independently observed in
// the TH095 target.  Source order follows the target's emitted case bodies.
i32 __fastcall ResolveInt(PhotoEnemyView *enemy, i32 operand)
{
    switch (operand)
    {
    case 0x2710: return enemy->activeEclContext->scriptState.intVariables[0];
    case 0x2711: return enemy->activeEclContext->scriptState.intVariables[1];
    case 0x2712: return enemy->activeEclContext->scriptState.intVariables[2];
    case 0x2713: return enemy->activeEclContext->scriptState.intVariables[3];
    case 0x2714: return enemy->activeEclContext->scriptState.intVariables[4];
    case 0x2715: return enemy->activeEclContext->scriptState.intVariables[5];
    case 0x2716: return enemy->activeEclContext->scriptState.intVariables[6];
    case 0x2717: return enemy->activeEclContext->scriptState.intVariables[7];

    case ECL_CALL_PARAMETER_INT0: return enemy->activeEclContext->scriptState.callParameters.ints[0];
    case ECL_CALL_PARAMETER_INT1: return enemy->activeEclContext->scriptState.callParameters.ints[1];
    case ECL_CALL_PARAMETER_INT2: return enemy->activeEclContext->scriptState.callParameters.ints[2];
    case ECL_CALL_PARAMETER_INT3: return enemy->activeEclContext->scriptState.callParameters.ints[3];
    case 0x2724: return enemy->activeEclContext->scriptState.extraIntVariables[0];
    case 0x2725: return enemy->activeEclContext->scriptState.extraIntVariables[1];
    case 0x2726: return enemy->activeEclContext->scriptState.extraIntVariables[2];
    case 0x2727: return enemy->activeEclContext->scriptState.extraIntVariables[3];

    case ECL_RANDOM_U31: return (i32)(g_Rng.GetRandomU32() & 0x7fffffff);
    case ECL_RANDOM_F32: return (i32)g_Rng.GetRandomF32();
    case ECL_RANDOM_I32: return (i32)g_Rng.GetRandomU32();
    case ECL_RANDOM_F32_SIGNED: return (i32)g_Rng.GetRandomF32Signed();

    case ECL_ENEMY_TIMER: return enemy->eclTimer.current;
    case ECL_ENEMY_LIFE: return enemy->life;
    case 0x275d: return (i32)enemy->activeEclContext->scriptState.extraFloatVariables[0];
    case 0x275e: return (i32)enemy->activeEclContext->scriptState.extraFloatVariables[1];
    case 0x275f: return (i32)enemy->activeEclContext->scriptState.extraFloatVariables[2];
    case 0x2760: return (i32)enemy->activeEclContext->scriptState.extraFloatVariables[3];

    case 0x2718: return (i32)enemy->activeEclContext->scriptState.floatVariables[0];
    case 0x2719: return (i32)enemy->activeEclContext->scriptState.floatVariables[1];
    case 0x271a: return (i32)enemy->activeEclContext->scriptState.floatVariables[2];
    case 0x271b: return (i32)enemy->activeEclContext->scriptState.floatVariables[3];
    case 0x271c: return (i32)enemy->activeEclContext->scriptState.floatVariables[4];
    case 0x271d: return (i32)enemy->activeEclContext->scriptState.floatVariables[5];
    case 0x271e: return (i32)enemy->activeEclContext->scriptState.floatVariables[6];
    case 0x271f: return (i32)enemy->activeEclContext->scriptState.floatVariables[7];
    case ECL_CALL_PARAMETER_FLOAT0: return (i32)enemy->activeEclContext->scriptState.callParameters.floats[0];
    case ECL_CALL_PARAMETER_FLOAT1: return (i32)enemy->activeEclContext->scriptState.callParameters.floats[1];
    case ECL_CALL_PARAMETER_FLOAT2: return (i32)enemy->activeEclContext->scriptState.callParameters.floats[2];
    case ECL_CALL_PARAMETER_FLOAT3: return (i32)enemy->activeEclContext->scriptState.callParameters.floats[3];

    case ECL_MANAGER_PARAMETER_INT0: return g_PhotoEnemyManager->eclManager->callParameters.ints[0];
    case ECL_MANAGER_PARAMETER_INT1: return g_PhotoEnemyManager->eclManager->callParameters.ints[1];
    case ECL_MANAGER_PARAMETER_INT2: return g_PhotoEnemyManager->eclManager->callParameters.ints[2];
    case ECL_MANAGER_PARAMETER_INT3: return g_PhotoEnemyManager->eclManager->callParameters.ints[3];
    case ECL_MANAGER_PARAMETER_FLOAT0: return (i32)g_PhotoEnemyManager->eclManager->callParameters.floats[0];
    case ECL_MANAGER_PARAMETER_FLOAT1: return (i32)g_PhotoEnemyManager->eclManager->callParameters.floats[1];
    case ECL_MANAGER_PARAMETER_FLOAT2: return (i32)g_PhotoEnemyManager->eclManager->callParameters.floats[2];
    case ECL_MANAGER_PARAMETER_FLOAT3: return (i32)g_PhotoEnemyManager->eclManager->callParameters.floats[3];

    case ECL_ENEMY_POSITION_X: return (i32)enemy->worldPosition.x;
    case ECL_ENEMY_POSITION_Y: return (i32)enemy->worldPosition.y;
    case ECL_ENEMY_POSITION_Z: return (i32)enemy->worldPosition.z;
    case ECL_PLAYER_POSITION_X: return (i32)g_RuntimePlayerOwner->playerPosition.x;
    case ECL_PLAYER_POSITION_Y: return (i32)g_RuntimePlayerOwner->playerPosition.y;
    case ECL_PLAYER_POSITION_Z: return (i32)g_RuntimePlayerOwner->playerPosition.z;
    case ECL_ENEMY_INTERPOLATION_ORIGIN_X: return (i32)enemy->movementInterpolationOrigin.x;
    case ECL_ENEMY_INTERPOLATION_ORIGIN_Y: return (i32)enemy->movementInterpolationOrigin.y;
    case ECL_ENEMY_INTERPOLATION_ORIGIN_Z: return (i32)enemy->movementInterpolationOrigin.z;
    case 0x2754: return (i32)enemy->positionDelta.x;
    case 0x2755: return (i32)enemy->positionDelta.y;
    case 0x2756: return (i32)enemy->positionDelta.z;

    case ECL_SCHEDULED_PHOTO_INDEX0: return enemy->eclSchedule.photoIndices[0];
    case ECL_SCHEDULED_PHOTO_INDEX1: return enemy->eclSchedule.photoIndices[1];
    case ECL_SCHEDULED_PHOTO_INDEX2: return enemy->eclSchedule.photoIndices[2];
    case ECL_SCHEDULED_PHOTO_INDEX3: return enemy->eclSchedule.photoIndices[3];
    case ECL_ENEMY_MOVEMENT_ANGLE: return (i32)enemy->movementAngle;
    case ECL_ENEMY_ANGULAR_VELOCITY: return (i32)enemy->angularVelocity;
    case ECL_ENEMY_SPEED: return (i32)enemy->speed;
    case ECL_ENEMY_ACCELERATION: return (i32)enemy->acceleration;
    case ECL_ENEMY_ORBIT_RADIUS: return (i32)enemy->orbitRadius;
    case ECL_ENEMY_ORBIT_ANGLE: return (i32)enemy->orbitAngle;
    case ECL_ENEMY_ORBIT_ANGULAR_VELOCITY: return (i32)enemy->orbitAngularVelocity;
    case 0x2752: return enemy->unknown2c50;
    case ECL_PHOTO_TARGET_SLOT: return enemy->photoTargetSlot;
    case ECL_ENEMY_ITEM_DROP_TYPE: return enemy->itemDropType;
    case ECL_ENEMY_SCORE: return enemy->score;

    case ECL_ANGLE_TO_PLAYER:
        return (i32)g_RuntimePlayerOwner->AngleFromPoint(&enemy->worldPosition);
    case ECL_DISTANCE_TO_PLAYER:
    {
        Float3 delta = g_RuntimePlayerOwner->playerPosition - enemy->worldPosition;
        return (i32)D3DXVec3Length(reinterpret_cast<D3DXVECTOR3 *>(&delta));
    }
    case ECL_PLAYER_PHOTO_INDEX:
        return g_RuntimePlayerOwner->camera.GetPhotoIndex();
    case ECL_PLAYER_PHOTOS_TAKEN:
        return g_RuntimePlayerOwner->camera.GetPhotosTaken();
    case ECL_FIRST_PHOTO_TARGET_WORLD_X: return (i32)g_PhotoEnemyManager->photoTargets[0]->worldPosition.x;
    case ECL_FIRST_PHOTO_TARGET_WORLD_Y: return (i32)g_PhotoEnemyManager->photoTargets[0]->worldPosition.y;
    default: return operand;
    }
}

} // namespace EclOperands

} // namespace th095
