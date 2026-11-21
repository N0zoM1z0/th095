#include "AnmManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoEnemyEclManager.hpp"
#include "PhotoEnemyManagerLayout.hpp"
#include "ecl/EclOperands.hpp"
#include "PhotoGamePlayer.hpp"

namespace th095
{

// FUNCTION: TH095 0x004105A0.  This is the float half of TH095's operand
// resolver pair, including photography counters and the active boss slots.
f32 PhotoEnemyView::ResolveFloat(f32 operand)
{
    switch ((i32)operand)
    {
    case 0x2710: return (f32)this->activeEclContext->scriptState.intVariables[0];
    case 0x2711: return (f32)this->activeEclContext->scriptState.intVariables[1];
    case 0x2712: return (f32)this->activeEclContext->scriptState.intVariables[2];
    case 0x2713: return (f32)this->activeEclContext->scriptState.intVariables[3];
    case 0x2714: return (f32)this->activeEclContext->scriptState.intVariables[4];
    case 0x2715: return (f32)this->activeEclContext->scriptState.intVariables[5];
    case 0x2716: return (f32)this->activeEclContext->scriptState.intVariables[6];
    case 0x2717: return (f32)this->activeEclContext->scriptState.intVariables[7];

    case EclOperands::ECL_CALL_PARAMETER_INT0: return (f32)this->activeEclContext->scriptState.callParameters.ints[0];
    case EclOperands::ECL_CALL_PARAMETER_INT1: return (f32)this->activeEclContext->scriptState.callParameters.ints[1];
    case EclOperands::ECL_CALL_PARAMETER_INT2: return (f32)this->activeEclContext->scriptState.callParameters.ints[2];
    case EclOperands::ECL_CALL_PARAMETER_INT3: return (f32)this->activeEclContext->scriptState.callParameters.ints[3];
    case 0x2724: return (f32)this->activeEclContext->scriptState.extraIntVariables[0];
    case 0x2725: return (f32)this->activeEclContext->scriptState.extraIntVariables[1];
    case 0x2726: return (f32)this->activeEclContext->scriptState.extraIntVariables[2];
    case 0x2727: return (f32)this->activeEclContext->scriptState.extraIntVariables[3];

    case EclOperands::ECL_RANDOM_U31: return (f32)(g_Rng.GetRandomU32() & 0x7fffffff);
    case EclOperands::ECL_RANDOM_F32: return g_Rng.GetRandomF32();
    case EclOperands::ECL_RANDOM_I32: return (f32)(i32)g_Rng.GetRandomU32();
    case EclOperands::ECL_RANDOM_F32_SIGNED: return g_Rng.GetRandomF32Signed();
    case EclOperands::ECL_RANDOM_ANGLE: return g_Rng.GetRandomF32() * ZUN_2PI - ZUN_PI;

    case EclOperands::ECL_ENEMY_TIMER: return (f32)this->eclTimer.current;
    case EclOperands::ECL_ENEMY_LIFE: return (f32)this->life;
    case EclOperands::ECL_ENEMY_ITEM_DROP_TYPE: return (f32)this->itemDropType;
    case EclOperands::ECL_ENEMY_SCORE: return (f32)this->score;

    case EclOperands::ECL_MANAGER_PARAMETER_INT0: return (f32)g_PhotoEnemyManager->eclManager->callParameters.ints[0];
    case EclOperands::ECL_MANAGER_PARAMETER_INT1: return (f32)g_PhotoEnemyManager->eclManager->callParameters.ints[1];
    case EclOperands::ECL_MANAGER_PARAMETER_INT2: return (f32)g_PhotoEnemyManager->eclManager->callParameters.ints[2];
    case EclOperands::ECL_MANAGER_PARAMETER_INT3: return (f32)g_PhotoEnemyManager->eclManager->callParameters.ints[3];
    case EclOperands::ECL_MANAGER_PARAMETER_FLOAT0: return g_PhotoEnemyManager->eclManager->callParameters.floats[0];
    case EclOperands::ECL_MANAGER_PARAMETER_FLOAT1: return g_PhotoEnemyManager->eclManager->callParameters.floats[1];
    case EclOperands::ECL_MANAGER_PARAMETER_FLOAT2: return g_PhotoEnemyManager->eclManager->callParameters.floats[2];
    case EclOperands::ECL_MANAGER_PARAMETER_FLOAT3: return g_PhotoEnemyManager->eclManager->callParameters.floats[3];

    case 0x2718: return this->activeEclContext->scriptState.floatVariables[0];
    case 0x2719: return this->activeEclContext->scriptState.floatVariables[1];
    case 0x271a: return this->activeEclContext->scriptState.floatVariables[2];
    case 0x271b: return this->activeEclContext->scriptState.floatVariables[3];
    case 0x271c: return this->activeEclContext->scriptState.floatVariables[4];
    case 0x271d: return this->activeEclContext->scriptState.floatVariables[5];
    case 0x271e: return this->activeEclContext->scriptState.floatVariables[6];
    case 0x271f: return this->activeEclContext->scriptState.floatVariables[7];
    case EclOperands::ECL_CALL_PARAMETER_FLOAT0: return this->activeEclContext->scriptState.callParameters.floats[0];
    case EclOperands::ECL_CALL_PARAMETER_FLOAT1: return this->activeEclContext->scriptState.callParameters.floats[1];
    case EclOperands::ECL_CALL_PARAMETER_FLOAT2: return this->activeEclContext->scriptState.callParameters.floats[2];
    case EclOperands::ECL_CALL_PARAMETER_FLOAT3: return this->activeEclContext->scriptState.callParameters.floats[3];

    case EclOperands::ECL_ENEMY_POSITION_X: return this->worldPosition.x;
    case EclOperands::ECL_ENEMY_POSITION_Y: return this->worldPosition.y;
    case EclOperands::ECL_ENEMY_POSITION_Z: return this->worldPosition.z;
    case EclOperands::ECL_PLAYER_POSITION_X: return g_RuntimePlayerOwner->playerPosition.x;
    case EclOperands::ECL_PLAYER_POSITION_Y: return g_RuntimePlayerOwner->playerPosition.y;
    case EclOperands::ECL_PLAYER_POSITION_Z: return g_RuntimePlayerOwner->playerPosition.z;
    case 0x275d: return this->activeEclContext->scriptState.extraFloatVariables[0];
    case 0x275e: return this->activeEclContext->scriptState.extraFloatVariables[1];
    case 0x275f: return this->activeEclContext->scriptState.extraFloatVariables[2];
    case 0x2760: return this->activeEclContext->scriptState.extraFloatVariables[3];
    case EclOperands::ECL_ENEMY_INTERPOLATION_ORIGIN_X: return this->movementInterpolationOrigin.x;
    case EclOperands::ECL_ENEMY_INTERPOLATION_ORIGIN_Y: return this->movementInterpolationOrigin.y;
    case EclOperands::ECL_ENEMY_INTERPOLATION_ORIGIN_Z: return this->movementInterpolationOrigin.z;
    case EclOperands::ECL_ENEMY_INTERPOLATION_DELTA_X: return this->movementInterpolationDelta.x;
    case EclOperands::ECL_ENEMY_INTERPOLATION_DELTA_Y: return this->movementInterpolationDelta.y;
    case EclOperands::ECL_ENEMY_INTERPOLATION_DELTA_Z: return this->movementInterpolationDelta.z;
    case 0x2754: return this->positionDelta.x;
    case 0x2755: return this->positionDelta.y;
    case 0x2756: return this->positionDelta.z;

    case EclOperands::ECL_SCHEDULED_PHOTO_INDEX0: return (f32)this->eclSchedule.photoIndices[0];
    case EclOperands::ECL_SCHEDULED_PHOTO_INDEX1: return (f32)this->eclSchedule.photoIndices[1];
    case EclOperands::ECL_SCHEDULED_PHOTO_INDEX2: return (f32)this->eclSchedule.photoIndices[2];
    case EclOperands::ECL_SCHEDULED_PHOTO_INDEX3: return (f32)this->eclSchedule.photoIndices[3];
    case EclOperands::ECL_ANGLE_TO_PLAYER: return g_RuntimePlayerOwner->AngleFromPoint(&this->worldPosition);
    case EclOperands::ECL_SHOT_ANGLE_TO_PLAYER:
    {
        Float3 position = this->worldPosition + this->shootOffset;
        return g_RuntimePlayerOwner->AngleFromPoint(&position);
    }

    case EclOperands::ECL_ENEMY_MOVEMENT_ANGLE: return this->movementAngle;
    case EclOperands::ECL_ENEMY_ANGULAR_VELOCITY: return this->angularVelocity;
    case EclOperands::ECL_ENEMY_SPEED: return this->speed;
    case EclOperands::ECL_ENEMY_ACCELERATION: return this->acceleration;
    case EclOperands::ECL_ENEMY_ORBIT_RADIUS: return this->orbitRadius;
    case EclOperands::ECL_ENEMY_ORBIT_ANGLE: return this->orbitAngle;
    case EclOperands::ECL_ENEMY_ORBIT_ANGULAR_VELOCITY: return this->orbitAngularVelocity;
    case EclOperands::ECL_PHOTO_TARGET_SLOT: return (f32)this->photoTargetSlot;
    case 0x2752: return (f32)this->unknown2c50;
    case EclOperands::ECL_DISTANCE_TO_PLAYER:
    {
        Float3 delta = g_RuntimePlayerOwner->playerPosition - this->worldPosition;
        return D3DXVec3Length(reinterpret_cast<D3DXVECTOR3 *>(&delta));
    }
    case EclOperands::ECL_PLAYER_PHOTO_INDEX:
        return (f32)g_RuntimePlayerOwner->camera.GetPhotoIndex();
    case EclOperands::ECL_PLAYER_PHOTOS_TAKEN:
        return (f32)g_RuntimePlayerOwner->camera.GetPhotosTaken();
    case EclOperands::ECL_FIRST_PHOTO_TARGET_WORLD_X: return g_PhotoEnemyManager->photoTargets[0]->worldPosition.x;
    case EclOperands::ECL_FIRST_PHOTO_TARGET_WORLD_Y: return g_PhotoEnemyManager->photoTargets[0]->worldPosition.y;
    default: return operand;
    }
}

} // namespace th095
