#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
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

struct EclOperandRuntimeView
{
    u8 unknown000000[0x4df4];
    EclSharedOperandView *sharedOperands;
    u8 unknown004df8[0x26ae00 - 0x4df8];
    Enemy *bosses[8];
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

extern EclOperandRuntimeView *g_EclOperandRuntime;
extern EclOperandPlayerView *g_EclOperandPlayer;

#ifndef DIFFBUILD
#define g_EclOperandRuntime \
    TH095_RUNTIME_GLOBAL_PTR(EclOperandRuntimeView, g_RuntimeEnemyManagerOwner)
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

#define ENEMY_I32(owner, offset) \
    (*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(owner) + (offset)))
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_ENEMY_LIFE(owner) ENEMY_I32((owner), 0x2958)
#else
struct EclIntOperandEnemyLifeView
{
    u8 unknown0000[0x2958];
    i32 life;
};
typedef char EclIntOperandEnemyLifeAt2958[
    (offsetof(EclIntOperandEnemyLifeView, life) == 0x2958) ? 1 : -1];
#define TH095_ECL_ENEMY_LIFE(owner) \
    (reinterpret_cast<EclIntOperandEnemyLifeView *>(owner)->life)
#endif
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_TIMER_CURRENT(owner) ENEMY_I32((owner), 0x2974)
#else
struct EclIntOperandTimerView
{
    i32 previous;
    f32 subFrame;
    i32 current;
};
typedef char EclIntOperandTimerViewSizeC[
    (sizeof(EclIntOperandTimerView) == 0x0c) ? 1 : -1];
struct EclIntOperandEnemyTimerView
{
    u8 unknown0000[0x296c];
    EclIntOperandTimerView eclTimer;
};
typedef char EclIntOperandEnemyTimerAt296C[
    (offsetof(EclIntOperandEnemyTimerView, eclTimer) == 0x296c) ? 1 : -1];
#define TH095_ECL_TIMER_CURRENT(owner) \
    (reinterpret_cast<EclIntOperandEnemyTimerView *>(owner)->eclTimer.current)
#endif
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_ENEMY_SCORE(owner) ENEMY_I32((owner), 0x2964)
#else
struct EclIntOperandEnemyScoreView
{
    u8 unknown0000[0x2964];
    i32 score;
};
typedef char EclIntOperandEnemyScoreAt2964[
    (offsetof(EclIntOperandEnemyScoreView, score) == 0x2964) ? 1 : -1];
#define TH095_ECL_ENEMY_SCORE(owner) \
    (reinterpret_cast<EclIntOperandEnemyScoreView *>(owner)->score)
#endif
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_ITEM_DROP_TYPE(owner) ENEMY_I32((owner), 0x2bd8)
#else
struct EclIntOperandItemDropTypeView
{
    u8 unknown0000[0x2bd8];
    i32 itemDropType;
};
typedef char EclIntOperandItemDropTypeAt2BD8[
    (offsetof(EclIntOperandItemDropTypeView, itemDropType) == 0x2bd8) ? 1 : -1];
#define TH095_ECL_ITEM_DROP_TYPE(owner) \
    (reinterpret_cast<EclIntOperandItemDropTypeView *>(owner)->itemDropType)
#endif
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_SCHEDULED_FRAME0(owner) ENEMY_I32(owner, 0x2c54)
#define TH095_ECL_SCHEDULED_FRAME1(owner) ENEMY_I32(owner, 0x2c58)
#define TH095_ECL_SCHEDULED_FRAME2(owner) ENEMY_I32(owner, 0x2c5c)
#define TH095_ECL_SCHEDULED_FRAME3(owner) ENEMY_I32(owner, 0x2c60)
#else
struct EclIntOperandScheduledCallFrameView
{
    u8 unknown0000[0x2c54];
    i32 scheduledCallFrames[10];
};
typedef char EclIntOperandScheduledCallFramesAt2C54[
    (offsetof(EclIntOperandScheduledCallFrameView, scheduledCallFrames) == 0x2c54)
        ? 1
        : -1];
#define TH095_ECL_SCHEDULED_FRAME0(owner) \
    (reinterpret_cast<EclIntOperandScheduledCallFrameView *>(owner)->scheduledCallFrames[0])
#define TH095_ECL_SCHEDULED_FRAME1(owner) \
    (reinterpret_cast<EclIntOperandScheduledCallFrameView *>(owner)->scheduledCallFrames[1])
#define TH095_ECL_SCHEDULED_FRAME2(owner) \
    (reinterpret_cast<EclIntOperandScheduledCallFrameView *>(owner)->scheduledCallFrames[2])
#define TH095_ECL_SCHEDULED_FRAME3(owner) \
    (reinterpret_cast<EclIntOperandScheduledCallFrameView *>(owner)->scheduledCallFrames[3])
#endif
#define ENEMY_U8(owner, offset) \
    (*reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(owner) + (offset)))

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

    case 0x2734: return enemy->activeEclContext->callParameterInts[0];
    case 0x2735: return enemy->activeEclContext->callParameterInts[1];
    case 0x2736: return enemy->activeEclContext->callParameterInts[2];
    case 0x2737: return enemy->activeEclContext->callParameterInts[3];
    case 0x2724: return enemy->activeEclContext->extraIntVariables[0];
    case 0x2725: return enemy->activeEclContext->extraIntVariables[1];
    case 0x2726: return enemy->activeEclContext->extraIntVariables[2];
    case 0x2727: return enemy->activeEclContext->extraIntVariables[3];

    case 0x2720: return (i32)(g_Rng.GetRandomU32() & 0x7fffffff);
    case 0x2721: return (i32)g_Rng.GetRandomF32();
    case 0x2722: return (i32)g_Rng.GetRandomU32();
    case 0x2723: return (i32)g_Rng.GetRandomF32Signed();

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
    case 0x2738: return (i32)enemy->activeEclContext->callParameterFloats[0];
    case 0x2739: return (i32)enemy->activeEclContext->callParameterFloats[1];
    case 0x273a: return (i32)enemy->activeEclContext->callParameterFloats[2];
    case 0x273b: return (i32)enemy->activeEclContext->callParameterFloats[3];

    case 0x273c: return g_EclOperandRuntime->sharedOperands->intVariables[0];
    case 0x273d: return g_EclOperandRuntime->sharedOperands->intVariables[1];
    case 0x273e: return g_EclOperandRuntime->sharedOperands->intVariables[2];
    case 0x273f: return g_EclOperandRuntime->sharedOperands->intVariables[3];
    case 0x2740: return (i32)g_EclOperandRuntime->sharedOperands->floatVariables[0];
    case 0x2741: return (i32)g_EclOperandRuntime->sharedOperands->floatVariables[1];
    case 0x2742: return (i32)g_EclOperandRuntime->sharedOperands->floatVariables[2];
    case 0x2743: return (i32)g_EclOperandRuntime->sharedOperands->floatVariables[3];

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
    case 0x2752: return ENEMY_I32(enemy, 0x2c50);
    case 0x2753: return ENEMY_U8(enemy, 0x2be5);
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
    case 0x2762: return (i32)g_EclOperandRuntime->bosses[0]->worldPosition.x;
    case 0x2763: return (i32)g_EclOperandRuntime->bosses[0]->worldPosition.y;
    default: return operand;
    }
}

} // namespace EclOperands

#undef TH095_ECL_INT_PHOTOS_TAKEN
#undef TH095_ECL_INT_PHOTO_INDEX
#undef TH095_ECL_INT_PLAYER_POSITION
#undef ENEMY_U8
#undef TH095_ECL_SCHEDULED_FRAME3
#undef TH095_ECL_SCHEDULED_FRAME2
#undef TH095_ECL_SCHEDULED_FRAME1
#undef TH095_ECL_SCHEDULED_FRAME0
#undef TH095_ECL_ITEM_DROP_TYPE
#undef TH095_ECL_ENEMY_SCORE
#undef TH095_ECL_TIMER_CURRENT
#undef TH095_ECL_ENEMY_LIFE
#undef ENEMY_I32

} // namespace th095
