#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoEnemyEclManager.hpp"
#include "PhotoEnemyEclAccess.hpp"
#include "PhotoEnemyManagerLayout.hpp"
#include "PhotoPlayerRuntime.hpp"
#include "ecl/EclOperands.hpp"

namespace th095
{

namespace EclOperands
{

// FUNCTION: TH095 0x00410DB0.  This is the writable subset of the float
// resolver.  The source order follows the target's sparse-switch handlers.
f32 *__fastcall ResolveFloatLValue(
    Enemy *enemy, f32 *operand, u16 flags, i32 flagIndex)
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

    case ECL_ENEMY_POSITION_X: return &reinterpret_cast<PhotoEnemyView *>(enemy)->position.x;
    case ECL_ENEMY_POSITION_Y: return &reinterpret_cast<PhotoEnemyView *>(enemy)->position.y;
    case ECL_ENEMY_POSITION_Z: return &reinterpret_cast<PhotoEnemyView *>(enemy)->position.z;
    case 0x272d: return &reinterpret_cast<PhotoPlayerRuntimeView *>(g_RuntimePlayerOwner)->playerPosition.x;
    case 0x272e: return &reinterpret_cast<PhotoPlayerRuntimeView *>(g_RuntimePlayerOwner)->playerPosition.y;
    case 0x272f: return &reinterpret_cast<PhotoPlayerRuntimeView *>(g_RuntimePlayerOwner)->playerPosition.z;

    case 0x275d: return &enemy->activeEclContext->scriptState.extraFloatVariables[0];
    case 0x275e: return &enemy->activeEclContext->scriptState.extraFloatVariables[1];
    case 0x275f: return &enemy->activeEclContext->scriptState.extraFloatVariables[2];
    case 0x2760: return &enemy->activeEclContext->scriptState.extraFloatVariables[3];

    case ECL_MANAGER_PARAMETER_FLOAT0: return &g_PhotoEnemyManager->eclManager->callParameters.floats[0];
    case ECL_MANAGER_PARAMETER_FLOAT1: return &g_PhotoEnemyManager->eclManager->callParameters.floats[1];
    case ECL_MANAGER_PARAMETER_FLOAT2: return &g_PhotoEnemyManager->eclManager->callParameters.floats[2];
    case ECL_MANAGER_PARAMETER_FLOAT3: return &g_PhotoEnemyManager->eclManager->callParameters.floats[3];

    case ECL_ENEMY_INTERPOLATION_ORIGIN_X: return &reinterpret_cast<PhotoEnemyView *>(enemy)->movementInterpolationOrigin.x;
    case ECL_ENEMY_INTERPOLATION_ORIGIN_Y: return &reinterpret_cast<PhotoEnemyView *>(enemy)->movementInterpolationOrigin.y;
    case ECL_ENEMY_INTERPOLATION_ORIGIN_Z: return &reinterpret_cast<PhotoEnemyView *>(enemy)->movementInterpolationOrigin.z;
    case ECL_ENEMY_INTERPOLATION_DELTA_X: return &reinterpret_cast<PhotoEnemyView *>(enemy)->movementInterpolationDelta.x;
    case ECL_ENEMY_INTERPOLATION_DELTA_Y: return &reinterpret_cast<PhotoEnemyView *>(enemy)->movementInterpolationDelta.y;
    case ECL_ENEMY_INTERPOLATION_DELTA_Z: return &reinterpret_cast<PhotoEnemyView *>(enemy)->movementInterpolationDelta.z;

    case ECL_ENEMY_MOVEMENT_ANGLE: return &reinterpret_cast<PhotoEnemyView *>(enemy)->movementAngle;
    case ECL_ENEMY_ANGULAR_VELOCITY: return &reinterpret_cast<PhotoEnemyView *>(enemy)->angularVelocity;
    case ECL_ENEMY_SPEED: return &reinterpret_cast<PhotoEnemyView *>(enemy)->speed;
    case ECL_ENEMY_ACCELERATION: return &reinterpret_cast<PhotoEnemyView *>(enemy)->acceleration;
    case ECL_ENEMY_ORBIT_RADIUS: return &reinterpret_cast<PhotoEnemyView *>(enemy)->orbitRadius;
    case ECL_ENEMY_ORBIT_ANGLE: return &reinterpret_cast<PhotoEnemyView *>(enemy)->orbitAngle;
    case ECL_ENEMY_ORBIT_ANGULAR_VELOCITY: return &reinterpret_cast<PhotoEnemyView *>(enemy)->orbitAngularVelocity;
    default: return operand;
    }
}

} // namespace EclOperands
} // namespace th095
