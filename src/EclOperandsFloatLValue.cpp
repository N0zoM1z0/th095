#include "AnmManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoEnemyEclManager.hpp"
#include "PhotoEnemyManagerLayout.hpp"
#include "PhotoGamePlayer.hpp"
#include "ecl/EclOperands.hpp"

namespace th095
{

namespace EclOperands
{

// FUNCTION: TH095 0x00410DB0.  This is the writable subset of the float
// resolver.  The source order follows the target's sparse-switch handlers.
f32 *__fastcall ResolveFloatLValue(
    PhotoEnemyView *enemy, f32 *operand, u16 flags, i32 flagIndex)
{
    if (flagIndex >= 0 && !(flags & (1 << flagIndex)))
        return operand;

    switch ((i32)*operand)
    {
    case 0x2718: return &enemy->activeEclContext->scriptState.floatVariables[0];
    case 0x2719: return &enemy->activeEclContext->scriptState.floatVariables[1];
    case 0x271a: return &enemy->activeEclContext->scriptState.floatVariables[2];
    case 0x271b: return &enemy->activeEclContext->scriptState.floatVariables[3];
    case 0x271c: return &enemy->activeEclContext->scriptState.floatVariables[4];
    case 0x271d: return &enemy->activeEclContext->scriptState.floatVariables[5];
    case 0x271e: return &enemy->activeEclContext->scriptState.floatVariables[6];
    case 0x271f: return &enemy->activeEclContext->scriptState.floatVariables[7];

    case ECL_CALL_PARAMETER_FLOAT0: return &enemy->activeEclContext->scriptState.callParameters.floats[0];
    case ECL_CALL_PARAMETER_FLOAT1: return &enemy->activeEclContext->scriptState.callParameters.floats[1];
    case ECL_CALL_PARAMETER_FLOAT2: return &enemy->activeEclContext->scriptState.callParameters.floats[2];
    case ECL_CALL_PARAMETER_FLOAT3: return &enemy->activeEclContext->scriptState.callParameters.floats[3];

    case ECL_ENEMY_POSITION_X: return &enemy->position.x;
    case ECL_ENEMY_POSITION_Y: return &enemy->position.y;
    case ECL_ENEMY_POSITION_Z: return &enemy->position.z;
    case ECL_PLAYER_POSITION_X: return &g_RuntimePlayerOwner->playerPosition.x;
    case ECL_PLAYER_POSITION_Y: return &g_RuntimePlayerOwner->playerPosition.y;
    case ECL_PLAYER_POSITION_Z: return &g_RuntimePlayerOwner->playerPosition.z;

    case 0x275d: return &enemy->activeEclContext->scriptState.extraFloatVariables[0];
    case 0x275e: return &enemy->activeEclContext->scriptState.extraFloatVariables[1];
    case 0x275f: return &enemy->activeEclContext->scriptState.extraFloatVariables[2];
    case 0x2760: return &enemy->activeEclContext->scriptState.extraFloatVariables[3];

    case ECL_MANAGER_PARAMETER_FLOAT0: return &g_PhotoEnemyManager->eclManager->callParameters.floats[0];
    case ECL_MANAGER_PARAMETER_FLOAT1: return &g_PhotoEnemyManager->eclManager->callParameters.floats[1];
    case ECL_MANAGER_PARAMETER_FLOAT2: return &g_PhotoEnemyManager->eclManager->callParameters.floats[2];
    case ECL_MANAGER_PARAMETER_FLOAT3: return &g_PhotoEnemyManager->eclManager->callParameters.floats[3];

    case ECL_ENEMY_INTERPOLATION_ORIGIN_X: return &enemy->movementInterpolationOrigin.x;
    case ECL_ENEMY_INTERPOLATION_ORIGIN_Y: return &enemy->movementInterpolationOrigin.y;
    case ECL_ENEMY_INTERPOLATION_ORIGIN_Z: return &enemy->movementInterpolationOrigin.z;
    case ECL_ENEMY_INTERPOLATION_DELTA_X: return &enemy->movementInterpolationDelta.x;
    case ECL_ENEMY_INTERPOLATION_DELTA_Y: return &enemy->movementInterpolationDelta.y;
    case ECL_ENEMY_INTERPOLATION_DELTA_Z: return &enemy->movementInterpolationDelta.z;

    case ECL_ENEMY_MOVEMENT_ANGLE: return &enemy->movementAngle;
    case ECL_ENEMY_ANGULAR_VELOCITY: return &enemy->angularVelocity;
    case ECL_ENEMY_SPEED: return &enemy->speed;
    case ECL_ENEMY_ACCELERATION: return &enemy->acceleration;
    case ECL_ENEMY_ORBIT_RADIUS: return &enemy->orbitRadius;
    case ECL_ENEMY_ORBIT_ANGLE: return &enemy->orbitAngle;
    case ECL_ENEMY_ORBIT_ANGULAR_VELOCITY: return &enemy->orbitAngularVelocity;
    default: return operand;
    }
}

} // namespace EclOperands
} // namespace th095
