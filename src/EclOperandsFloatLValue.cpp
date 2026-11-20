#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoEnemyEclAccess.hpp"
#include "PhotoEnemyManagerLayout.hpp"
#include "PhotoPlayerRuntime.hpp"
#include "ecl/EclOperands.hpp"

namespace th095
{

struct EclSharedFloatLValueView
{
    u8 unknown000[0x178];
    f32 floatVariables[4];
};

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
    case 0x2718: return &TH095_ECL_CONTEXT_FLOAT_VARIABLE(enemy, 0);
    case 0x2719: return &TH095_ECL_CONTEXT_FLOAT_VARIABLE(enemy, 1);
    case 0x271a: return &TH095_ECL_CONTEXT_FLOAT_VARIABLE(enemy, 2);
    case 0x271b: return &TH095_ECL_CONTEXT_FLOAT_VARIABLE(enemy, 3);
    case 0x271c: return &TH095_ECL_CONTEXT_FLOAT_VARIABLE(enemy, 4);
    case 0x271d: return &TH095_ECL_CONTEXT_FLOAT_VARIABLE(enemy, 5);
    case 0x271e: return &TH095_ECL_CONTEXT_FLOAT_VARIABLE(enemy, 6);
    case 0x271f: return &TH095_ECL_CONTEXT_FLOAT_VARIABLE(enemy, 7);

    case TH095_ECL_CALL_PARAMETER_FLOAT0: return &TH095_ECL_CONTEXT_CALL_PARAMETER_FLOAT(enemy, 0);
    case TH095_ECL_CALL_PARAMETER_FLOAT1: return &TH095_ECL_CONTEXT_CALL_PARAMETER_FLOAT(enemy, 1);
    case TH095_ECL_CALL_PARAMETER_FLOAT2: return &TH095_ECL_CONTEXT_CALL_PARAMETER_FLOAT(enemy, 2);
    case TH095_ECL_CALL_PARAMETER_FLOAT3: return &TH095_ECL_CONTEXT_CALL_PARAMETER_FLOAT(enemy, 3);

    case 0x272a: return &TH095_ECL_ENEMY_POSITION(enemy, 0);
    case 0x272b: return &TH095_ECL_ENEMY_POSITION(enemy, 1);
    case 0x272c: return &TH095_ECL_ENEMY_POSITION(enemy, 2);
    case 0x272d: return &reinterpret_cast<PhotoPlayerRuntimeView *>(g_RuntimePlayerOwner)->playerPosition.x;
    case 0x272e: return &reinterpret_cast<PhotoPlayerRuntimeView *>(g_RuntimePlayerOwner)->playerPosition.y;
    case 0x272f: return &reinterpret_cast<PhotoPlayerRuntimeView *>(g_RuntimePlayerOwner)->playerPosition.z;

    case 0x275d: return &TH095_ECL_CONTEXT_EXTRA_FLOAT_VARIABLE(enemy, 0);
    case 0x275e: return &TH095_ECL_CONTEXT_EXTRA_FLOAT_VARIABLE(enemy, 1);
    case 0x275f: return &TH095_ECL_CONTEXT_EXTRA_FLOAT_VARIABLE(enemy, 2);
    case 0x2760: return &TH095_ECL_CONTEXT_EXTRA_FLOAT_VARIABLE(enemy, 3);

    case 0x2740: return &reinterpret_cast<EclSharedFloatLValueView *>(g_PhotoEnemyManager->eclManager)->floatVariables[0];
    case 0x2741: return &reinterpret_cast<EclSharedFloatLValueView *>(g_PhotoEnemyManager->eclManager)->floatVariables[1];
    case 0x2742: return &reinterpret_cast<EclSharedFloatLValueView *>(g_PhotoEnemyManager->eclManager)->floatVariables[2];
    case 0x2743: return &reinterpret_cast<EclSharedFloatLValueView *>(g_PhotoEnemyManager->eclManager)->floatVariables[3];

    case 0x2749: return &TH095_ECL_MOVEMENT_INTERPOLATION_ORIGIN(enemy, 0);
    case 0x274a: return &TH095_ECL_MOVEMENT_INTERPOLATION_ORIGIN(enemy, 1);
    case 0x274b: return &TH095_ECL_MOVEMENT_INTERPOLATION_ORIGIN(enemy, 2);
    case 0x274e: return &TH095_ECL_MOVEMENT_INTERPOLATION_DELTA(enemy, 0);
    case 0x274f: return &TH095_ECL_MOVEMENT_INTERPOLATION_DELTA(enemy, 1);
    case 0x2750: return &TH095_ECL_MOVEMENT_INTERPOLATION_DELTA(enemy, 2);

    case 0x2744: return &TH095_ECL_MOVEMENT_ANGLE(enemy);
    case 0x2745: return &TH095_ECL_ANGULAR_VELOCITY(enemy);
    case 0x2746: return &TH095_ECL_SPEED(enemy);
    case 0x2747: return &TH095_ECL_ACCELERATION(enemy);
    case 0x2748: return &TH095_ECL_ORBIT_RADIUS(enemy);
    case 0x274c: return &TH095_ECL_ORBIT_ANGLE(enemy);
    case 0x274d: return &TH095_ECL_ORBIT_ANGULAR_VELOCITY(enemy);
    default: return operand;
    }
}

} // namespace EclOperands
} // namespace th095
