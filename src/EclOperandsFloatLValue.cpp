#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "ecl/EclOperands.hpp"
#if !defined(TH095_MATCH_EXACT) && !defined(DIFFBUILD)
#include "PhotoEnemyManager.hpp"
#endif

namespace th095
{

struct EclSharedFloatLValueView
{
    u8 unknown000[0x178];
    f32 floatVariables[4];
};

#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
struct EclFloatLValueRuntimeView
{
    u8 unknown000000[0x4df4];
    EclSharedFloatLValueView *sharedOperands;
};
#define TH095_ECL_FLOAT_LVALUE_SHARED(runtime) ((runtime)->sharedOperands)
#else
typedef PhotoEnemyManagerView EclFloatLValueRuntimeView;
#define TH095_ECL_FLOAT_LVALUE_SHARED(runtime) \
    reinterpret_cast<EclSharedFloatLValueView *>((runtime)->eclManager)
#endif

struct EclFloatLValuePlayerView
{
    u8 unknown0000[0x1e30];
    Float3 position;
};

extern EclFloatLValueRuntimeView *g_EclFloatLValueRuntime;
extern EclFloatLValuePlayerView *g_EclFloatLValuePlayer;

#ifndef DIFFBUILD
#define g_EclFloatLValueRuntime \
    TH095_RUNTIME_GLOBAL_PTR(EclFloatLValueRuntimeView, g_RuntimeEnemyManagerOwner)
#define g_EclFloatLValuePlayer \
    TH095_RUNTIME_GLOBAL_PTR(EclFloatLValuePlayerView, g_RuntimePlayerOwner)
#endif

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
    case 0x2718: return &enemy->activeEclContext->floatVariables[0];
    case 0x2719: return &enemy->activeEclContext->floatVariables[1];
    case 0x271a: return &enemy->activeEclContext->floatVariables[2];
    case 0x271b: return &enemy->activeEclContext->floatVariables[3];
    case 0x271c: return &enemy->activeEclContext->floatVariables[4];
    case 0x271d: return &enemy->activeEclContext->floatVariables[5];
    case 0x271e: return &enemy->activeEclContext->floatVariables[6];
    case 0x271f: return &enemy->activeEclContext->floatVariables[7];

    case TH095_ECL_CALL_PARAMETER_FLOAT0: return &enemy->activeEclContext->callParameterFloats[0];
    case TH095_ECL_CALL_PARAMETER_FLOAT1: return &enemy->activeEclContext->callParameterFloats[1];
    case TH095_ECL_CALL_PARAMETER_FLOAT2: return &enemy->activeEclContext->callParameterFloats[2];
    case TH095_ECL_CALL_PARAMETER_FLOAT3: return &enemy->activeEclContext->callParameterFloats[3];

    case 0x272a: return &enemy->position.x;
    case 0x272b: return &enemy->position.y;
    case 0x272c: return &enemy->position.z;
    case 0x272d: return &g_EclFloatLValuePlayer->position.x;
    case 0x272e: return &g_EclFloatLValuePlayer->position.y;
    case 0x272f: return &g_EclFloatLValuePlayer->position.z;

    case 0x275d: return &enemy->activeEclContext->extraFloatVariables[0];
    case 0x275e: return &enemy->activeEclContext->extraFloatVariables[1];
    case 0x275f: return &enemy->activeEclContext->extraFloatVariables[2];
    case 0x2760: return &enemy->activeEclContext->extraFloatVariables[3];

    case 0x2740: return &TH095_ECL_FLOAT_LVALUE_SHARED(g_EclFloatLValueRuntime)->floatVariables[0];
    case 0x2741: return &TH095_ECL_FLOAT_LVALUE_SHARED(g_EclFloatLValueRuntime)->floatVariables[1];
    case 0x2742: return &TH095_ECL_FLOAT_LVALUE_SHARED(g_EclFloatLValueRuntime)->floatVariables[2];
    case 0x2743: return &TH095_ECL_FLOAT_LVALUE_SHARED(g_EclFloatLValueRuntime)->floatVariables[3];

    case 0x2749: return &enemy->movementInterpolationOrigin.x;
    case 0x274a: return &enemy->movementInterpolationOrigin.y;
    case 0x274b: return &enemy->movementInterpolationOrigin.z;
    case 0x274e: return &enemy->movementInterpolationDelta.x;
    case 0x274f: return &enemy->movementInterpolationDelta.y;
    case 0x2750: return &enemy->movementInterpolationDelta.z;

    case 0x2744: return &enemy->movementAngle;
    case 0x2745: return &enemy->angularVelocity;
    case 0x2746: return &enemy->speed;
    case 0x2747: return &enemy->acceleration;
    case 0x2748: return &enemy->orbitRadius;
    case 0x274c: return &enemy->orbitAngle;
    case 0x274d: return &enemy->orbitAngularVelocity;
    default: return operand;
    }
}

} // namespace EclOperands
} // namespace th095
