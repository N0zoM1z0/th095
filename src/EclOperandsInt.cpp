#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoEnemyEclAccess.hpp"
#ifndef DIFFBUILD
#include "PhotoPlayerRuntime.hpp"
#endif
#include "ecl/EclOperands.hpp"

#include <d3dx8.h>

namespace th095
{

// TH095's runtime-owned ECL parameter block is distinct from an enemy's
// current context.  The target accesses this pointer through the gameplay
// runtime at +0x4DF4.
struct EclSharedOperandView
{
    u8 unknown000[0x168];
    i32 intVariables[4];
    f32 floatVariables[4];
};

struct EclPhotoCounterView
{
    i32 value;

    operator i32() const
    {
        return this->value;
    }
};

struct EclOperandCameraView
{
    u8 unknown000[0xba8];
    EclPhotoCounterView photoIndex;
    EclPhotoCounterView photosTaken;
};

struct EclOperandPlayerView
{
    u8 unknown0000[0x1e30];
    Float3 position;
    EclOperandCameraView camera;

    f32 AngleFromPoint(Float3 *point);
};

extern PhotoEnemyEclOperandRuntimeOwner *g_EclOperandRuntime;
extern EclOperandPlayerView *g_EclOperandPlayer;

#ifndef DIFFBUILD
#define g_EclOperandRuntime \
    TH095_RUNTIME_GLOBAL_PTR( \
        PhotoEnemyEclOperandRuntimeOwner, g_RuntimeEnemyManagerOwner)
#define g_EclOperandPlayer \
    TH095_RUNTIME_GLOBAL_PTR(EclOperandPlayerView, g_RuntimePlayerOwner)
#endif

#ifdef DIFFBUILD
#define TH095_ECL_INT_PLAYER_ANGLE(point) g_EclOperandPlayer->AngleFromPoint(point)
#else
#define TH095_ECL_INT_PLAYER_ANGLE(point) \
    TH095_RUNTIME_GLOBAL_PTR(PhotoPlayerRuntimeView, g_RuntimePlayerOwner)->AngleFromPoint(point)
#endif

#ifdef DIFFBUILD
#define TH095_ECL_INT_PLAYER_POSITION (g_EclOperandPlayer->position)
#else
#define TH095_ECL_INT_PLAYER_POSITION \
    (TH095_RUNTIME_GLOBAL_PTR(PhotoPlayerRuntimeView, g_RuntimePlayerOwner)->playerPosition)
#endif

#ifdef DIFFBUILD
#define TH095_ECL_INT_PHOTO_INDEX (g_EclOperandPlayer->camera.photoIndex)
#define TH095_ECL_INT_PHOTOS_TAKEN (g_EclOperandPlayer->camera.photosTaken)
#else
#define TH095_ECL_INT_PHOTO_INDEX \
    (TH095_RUNTIME_GLOBAL_PTR(PhotoPlayerRuntimeView, g_RuntimePlayerOwner)->camera.photoIndex)
#define TH095_ECL_INT_PHOTOS_TAKEN \
    (TH095_RUNTIME_GLOBAL_PTR(PhotoPlayerRuntimeView, g_RuntimePlayerOwner)->camera.photosTaken)
#endif

namespace EclOperands
{

// FUNCTION: TH095 0x0040FAE0.  TH08's resolver supplies the source-shape
// oracle, while every selector and field below is independently observed in
// the TH095 target.  Source order follows the target's emitted case bodies.
i32 __fastcall ResolveInt(Enemy *enemy, i32 operand)
{
    switch (operand)
    {
    case 0x2710: return enemy->activeEclContext->intVariables[0];
    case 0x2711: return enemy->activeEclContext->intVariables[1];
    case 0x2712: return enemy->activeEclContext->intVariables[2];
    case 0x2713: return enemy->activeEclContext->intVariables[3];
    case 0x2714: return enemy->activeEclContext->intVariables[4];
    case 0x2715: return enemy->activeEclContext->intVariables[5];
    case 0x2716: return enemy->activeEclContext->intVariables[6];
    case 0x2717: return enemy->activeEclContext->intVariables[7];

    case TH095_ECL_CALL_PARAMETER_INT0: return enemy->activeEclContext->callParameterInts[0];
    case TH095_ECL_CALL_PARAMETER_INT1: return enemy->activeEclContext->callParameterInts[1];
    case TH095_ECL_CALL_PARAMETER_INT2: return enemy->activeEclContext->callParameterInts[2];
    case TH095_ECL_CALL_PARAMETER_INT3: return enemy->activeEclContext->callParameterInts[3];
    case 0x2724: return enemy->activeEclContext->extraIntVariables[0];
    case 0x2725: return enemy->activeEclContext->extraIntVariables[1];
    case 0x2726: return enemy->activeEclContext->extraIntVariables[2];
    case 0x2727: return enemy->activeEclContext->extraIntVariables[3];

    case TH095_ECL_RANDOM_SELECTOR(0x2720, ECL_RANDOM_U31): return (i32)(g_Rng.GetRandomU32() & 0x7fffffff);
    case TH095_ECL_RANDOM_SELECTOR(0x2721, ECL_RANDOM_F32): return (i32)g_Rng.GetRandomF32();
    case TH095_ECL_RANDOM_SELECTOR(0x2722, ECL_RANDOM_I32): return (i32)g_Rng.GetRandomU32();
    case TH095_ECL_RANDOM_SELECTOR(0x2723, ECL_RANDOM_F32_SIGNED): return (i32)g_Rng.GetRandomF32Signed();

    case 0x2731: return TH095_ECL_TIMER_CURRENT(enemy);
    case 0x2733: return TH095_ECL_ENEMY_LIFE(enemy);
    case 0x275d: return (i32)enemy->activeEclContext->extraFloatVariables[0];
    case 0x275e: return (i32)enemy->activeEclContext->extraFloatVariables[1];
    case 0x275f: return (i32)enemy->activeEclContext->extraFloatVariables[2];
    case 0x2760: return (i32)enemy->activeEclContext->extraFloatVariables[3];

    case 0x2718: return (i32)enemy->activeEclContext->floatVariables[0];
    case 0x2719: return (i32)enemy->activeEclContext->floatVariables[1];
    case 0x271a: return (i32)enemy->activeEclContext->floatVariables[2];
    case 0x271b: return (i32)enemy->activeEclContext->floatVariables[3];
    case 0x271c: return (i32)enemy->activeEclContext->floatVariables[4];
    case 0x271d: return (i32)enemy->activeEclContext->floatVariables[5];
    case 0x271e: return (i32)enemy->activeEclContext->floatVariables[6];
    case 0x271f: return (i32)enemy->activeEclContext->floatVariables[7];
    case TH095_ECL_CALL_PARAMETER_FLOAT0: return (i32)enemy->activeEclContext->callParameterFloats[0];
    case TH095_ECL_CALL_PARAMETER_FLOAT1: return (i32)enemy->activeEclContext->callParameterFloats[1];
    case TH095_ECL_CALL_PARAMETER_FLOAT2: return (i32)enemy->activeEclContext->callParameterFloats[2];
    case TH095_ECL_CALL_PARAMETER_FLOAT3: return (i32)enemy->activeEclContext->callParameterFloats[3];

    case 0x273c: return TH095_ECL_RUNTIME_SHARED_OPERANDS(g_EclOperandRuntime, EclSharedOperandView)->intVariables[0];
    case 0x273d: return TH095_ECL_RUNTIME_SHARED_OPERANDS(g_EclOperandRuntime, EclSharedOperandView)->intVariables[1];
    case 0x273e: return TH095_ECL_RUNTIME_SHARED_OPERANDS(g_EclOperandRuntime, EclSharedOperandView)->intVariables[2];
    case 0x273f: return TH095_ECL_RUNTIME_SHARED_OPERANDS(g_EclOperandRuntime, EclSharedOperandView)->intVariables[3];
    case 0x2740: return (i32)TH095_ECL_RUNTIME_SHARED_OPERANDS(g_EclOperandRuntime, EclSharedOperandView)->floatVariables[0];
    case 0x2741: return (i32)TH095_ECL_RUNTIME_SHARED_OPERANDS(g_EclOperandRuntime, EclSharedOperandView)->floatVariables[1];
    case 0x2742: return (i32)TH095_ECL_RUNTIME_SHARED_OPERANDS(g_EclOperandRuntime, EclSharedOperandView)->floatVariables[2];
    case 0x2743: return (i32)TH095_ECL_RUNTIME_SHARED_OPERANDS(g_EclOperandRuntime, EclSharedOperandView)->floatVariables[3];

    case 0x272a: return (i32)enemy->worldPosition.x;
    case 0x272b: return (i32)enemy->worldPosition.y;
    case 0x272c: return (i32)enemy->worldPosition.z;
    case 0x272d: return (i32)TH095_ECL_INT_PLAYER_POSITION.x;
    case 0x272e: return (i32)TH095_ECL_INT_PLAYER_POSITION.y;
    case 0x272f: return (i32)TH095_ECL_INT_PLAYER_POSITION.z;
    case 0x2749: return (i32)enemy->movementInterpolationOrigin.x;
    case 0x274a: return (i32)enemy->movementInterpolationOrigin.y;
    case 0x274b: return (i32)enemy->movementInterpolationOrigin.z;
    case 0x2754: return (i32)enemy->lastFrameDisplacement.x;
    case 0x2755: return (i32)enemy->lastFrameDisplacement.y;
    case 0x2756: return (i32)enemy->lastFrameDisplacement.z;

    case 0x2757: return TH095_ECL_SCHEDULED_FRAME0(enemy);
    case 0x2758: return TH095_ECL_SCHEDULED_FRAME1(enemy);
    case 0x2759: return TH095_ECL_SCHEDULED_FRAME2(enemy);
    case 0x275a: return TH095_ECL_SCHEDULED_FRAME3(enemy);
    case 0x2744: return (i32)enemy->movementAngle;
    case 0x2745: return (i32)enemy->angularVelocity;
    case 0x2746: return (i32)enemy->speed;
    case 0x2747: return (i32)enemy->acceleration;
    case 0x2748: return (i32)enemy->orbitRadius;
    case 0x274c: return (i32)enemy->orbitAngle;
    case 0x274d: return (i32)enemy->orbitAngularVelocity;
    case 0x2752: return TH095_PHOTO_ENEMY_I32(
        enemy, PHOTO_ENEMY_ECL_UNKNOWN_2C50_OFFSET);
    case 0x2753: return TH095_ECL_PHOTO_TARGET_SLOT(enemy);
    case 0x275b: return TH095_ECL_ITEM_DROP_TYPE(enemy);
    case 0x275c: return TH095_ECL_ENEMY_SCORE(enemy);

    case 0x2730:
        return (i32)TH095_ECL_INT_PLAYER_ANGLE(&enemy->worldPosition);
    case 0x2732:
    {
        Float3 delta = TH095_ECL_INT_PLAYER_POSITION - enemy->worldPosition;
        return (i32)D3DXVec3Length(reinterpret_cast<D3DXVECTOR3 *>(&delta));
    }
    case 0x2761:
        return TH095_ECL_INT_PHOTO_INDEX;
    case 0x2764:
        return TH095_ECL_INT_PHOTOS_TAKEN;
    case 0x2762: return (i32)TH095_ECL_RUNTIME_PHOTO_TARGET(g_EclOperandRuntime, 0, Enemy)->worldPosition.x;
    case 0x2763: return (i32)TH095_ECL_RUNTIME_PHOTO_TARGET(g_EclOperandRuntime, 0, Enemy)->worldPosition.y;
    default: return operand;
    }
}

} // namespace EclOperands

#undef TH095_ECL_INT_PHOTOS_TAKEN
#undef TH095_ECL_INT_PHOTO_INDEX
#undef TH095_ECL_INT_PLAYER_POSITION
#undef TH095_ECL_PHOTO_TARGET_SLOT
#undef TH095_PHOTO_ENEMY_U8
#undef TH095_ECL_SCHEDULED_FRAME3
#undef TH095_ECL_SCHEDULED_FRAME2
#undef TH095_ECL_SCHEDULED_FRAME1
#undef TH095_ECL_SCHEDULED_FRAME0
#undef TH095_ECL_SCHEDULED_FRAME
#undef TH095_ECL_ITEM_DROP_TYPE
#undef TH095_ECL_ENEMY_SCORE
#undef TH095_ECL_TIMER_CURRENT
#undef TH095_ECL_ENEMY_LIFE
#undef TH095_PHOTO_ENEMY_I32

} // namespace th095
