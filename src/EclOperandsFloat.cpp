#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#ifndef DIFFBUILD
#include "PhotoPlayerRuntime.hpp"
#endif

namespace th095
{

struct EclSharedFloatOperandView
{
    u8 unknown000[0x168];
    i32 intVariables[4];
    f32 floatVariables[4];
};

struct EclFloatOperandRuntimeView
{
    u8 unknown000000[0x4df4];
    EclSharedFloatOperandView *sharedOperands;
    u8 unknown004df8[0x26ae00 - 0x4df8];
    Enemy *bosses[8];
};

struct EclFloatPhotoCounterView
{
    i32 value;

    operator i32() const
    {
        return this->value;
    }
};

struct EclFloatOperandCameraView
{
    u8 unknown000[0xba8];
    EclFloatPhotoCounterView photoIndex;
    EclFloatPhotoCounterView photosTaken;
};

struct EclFloatOperandPlayerView
{
    u8 unknown0000[0x1e30];
    Float3 position;
    EclFloatOperandCameraView camera;

    f32 AngleFromPoint(Float3 *point);
};

extern EclFloatOperandRuntimeView *g_EclFloatOperandRuntime;
extern EclFloatOperandPlayerView *g_EclFloatOperandPlayer;

#ifndef DIFFBUILD
#define g_EclFloatOperandRuntime \
    TH095_RUNTIME_GLOBAL_PTR(EclFloatOperandRuntimeView, g_RuntimeEnemyManagerOwner)
#define g_EclFloatOperandPlayer \
    TH095_RUNTIME_GLOBAL_PTR(EclFloatOperandPlayerView, g_RuntimePlayerOwner)
#endif

#ifdef DIFFBUILD
#define TH095_ECL_FLOAT_PLAYER_ANGLE(point) g_EclFloatOperandPlayer->AngleFromPoint(point)
#else
#define TH095_ECL_FLOAT_PLAYER_ANGLE(point) \
    TH095_RUNTIME_GLOBAL_PTR(PhotoPlayerRuntimeView, g_RuntimePlayerOwner)->AngleFromPoint(point)
#endif

#ifdef DIFFBUILD
#define TH095_ECL_FLOAT_PLAYER_POSITION (g_EclFloatOperandPlayer->position)
#else
#define TH095_ECL_FLOAT_PLAYER_POSITION \
    (TH095_RUNTIME_GLOBAL_PTR(PhotoPlayerRuntimeView, g_RuntimePlayerOwner)->playerPosition)
#endif

#ifdef DIFFBUILD
#define TH095_ECL_FLOAT_PHOTO_INDEX (g_EclFloatOperandPlayer->camera.photoIndex)
#define TH095_ECL_FLOAT_PHOTOS_TAKEN (g_EclFloatOperandPlayer->camera.photosTaken)
#else
#define TH095_ECL_FLOAT_PHOTO_INDEX \
    (TH095_RUNTIME_GLOBAL_PTR(PhotoPlayerRuntimeView, g_RuntimePlayerOwner)->camera.photoIndex)
#define TH095_ECL_FLOAT_PHOTOS_TAKEN \
    (TH095_RUNTIME_GLOBAL_PTR(PhotoPlayerRuntimeView, g_RuntimePlayerOwner)->camera.photosTaken)
#endif

#define ENEMY_I32(owner, offset) \
    (*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(owner) + (offset)))
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_ENEMY_LIFE(owner) ENEMY_I32((owner), 0x2958)
#else
struct EclFloatOperandEnemyLifeView
{
    u8 unknown0000[0x2958];
    i32 life;
};
typedef char EclFloatOperandEnemyLifeAt2958[
    (offsetof(EclFloatOperandEnemyLifeView, life) == 0x2958) ? 1 : -1];
#define TH095_ECL_ENEMY_LIFE(owner) \
    (reinterpret_cast<EclFloatOperandEnemyLifeView *>(owner)->life)
#endif
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_TIMER_CURRENT(owner) ENEMY_I32((owner), 0x2974)
#else
struct EclFloatOperandTimerView
{
    i32 previous;
    f32 subFrame;
    i32 current;
};
typedef char EclFloatOperandTimerViewSizeC[
    (sizeof(EclFloatOperandTimerView) == 0x0c) ? 1 : -1];
struct EclFloatOperandEnemyTimerView
{
    u8 unknown0000[0x296c];
    EclFloatOperandTimerView eclTimer;
};
typedef char EclFloatOperandEnemyTimerAt296C[
    (offsetof(EclFloatOperandEnemyTimerView, eclTimer) == 0x296c) ? 1 : -1];
#define TH095_ECL_TIMER_CURRENT(owner) \
    (reinterpret_cast<EclFloatOperandEnemyTimerView *>(owner)->eclTimer.current)
#endif
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_ENEMY_SCORE(owner) ENEMY_I32((owner), 0x2964)
#else
struct EclFloatOperandEnemyScoreView
{
    u8 unknown0000[0x2964];
    i32 score;
};
typedef char EclFloatOperandEnemyScoreAt2964[
    (offsetof(EclFloatOperandEnemyScoreView, score) == 0x2964) ? 1 : -1];
#define TH095_ECL_ENEMY_SCORE(owner) \
    (reinterpret_cast<EclFloatOperandEnemyScoreView *>(owner)->score)
#endif
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_ITEM_DROP_TYPE(owner) ENEMY_I32((owner), 0x2bd8)
#else
struct EclFloatOperandItemDropTypeView
{
    u8 unknown0000[0x2bd8];
    i32 itemDropType;
};
typedef char EclFloatOperandItemDropTypeAt2BD8[
    (offsetof(EclFloatOperandItemDropTypeView, itemDropType) == 0x2bd8) ? 1 : -1];
#define TH095_ECL_ITEM_DROP_TYPE(owner) \
    (reinterpret_cast<EclFloatOperandItemDropTypeView *>(owner)->itemDropType)
#endif
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_SCHEDULED_FRAME0(owner) ENEMY_I32(owner, 0x2c54)
#define TH095_ECL_SCHEDULED_FRAME1(owner) ENEMY_I32(owner, 0x2c58)
#define TH095_ECL_SCHEDULED_FRAME2(owner) ENEMY_I32(owner, 0x2c5c)
#define TH095_ECL_SCHEDULED_FRAME3(owner) ENEMY_I32(owner, 0x2c60)
#else
struct EclFloatOperandScheduledCallFrameView
{
    u8 unknown0000[0x2c54];
    i32 scheduledCallFrames[10];
};
typedef char EclFloatOperandScheduledCallFramesAt2C54[
    (offsetof(EclFloatOperandScheduledCallFrameView, scheduledCallFrames) == 0x2c54)
        ? 1
        : -1];
#define TH095_ECL_SCHEDULED_FRAME0(owner) \
    (reinterpret_cast<EclFloatOperandScheduledCallFrameView *>(owner)->scheduledCallFrames[0])
#define TH095_ECL_SCHEDULED_FRAME1(owner) \
    (reinterpret_cast<EclFloatOperandScheduledCallFrameView *>(owner)->scheduledCallFrames[1])
#define TH095_ECL_SCHEDULED_FRAME2(owner) \
    (reinterpret_cast<EclFloatOperandScheduledCallFrameView *>(owner)->scheduledCallFrames[2])
#define TH095_ECL_SCHEDULED_FRAME3(owner) \
    (reinterpret_cast<EclFloatOperandScheduledCallFrameView *>(owner)->scheduledCallFrames[3])
#endif
#define ENEMY_U8(owner, offset) \
    (*reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(owner) + (offset)))

// FUNCTION: TH095 0x004105A0.  This is the float half of TH095's operand
// resolver pair, including photography counters and the active boss slots.
f32 Enemy::ResolveFloat(f32 operand)
{
    switch ((i32)operand)
    {
    case 0x2710: return (f32)this->activeEclContext->intVariables[0];
    case 0x2711: return (f32)this->activeEclContext->intVariables[1];
    case 0x2712: return (f32)this->activeEclContext->intVariables[2];
    case 0x2713: return (f32)this->activeEclContext->intVariables[3];
    case 0x2714: return (f32)this->activeEclContext->intVariables[4];
    case 0x2715: return (f32)this->activeEclContext->intVariables[5];
    case 0x2716: return (f32)this->activeEclContext->intVariables[6];
    case 0x2717: return (f32)this->activeEclContext->intVariables[7];

    case 0x2734: return (f32)this->activeEclContext->callParameterInts[0];
    case 0x2735: return (f32)this->activeEclContext->callParameterInts[1];
    case 0x2736: return (f32)this->activeEclContext->callParameterInts[2];
    case 0x2737: return (f32)this->activeEclContext->callParameterInts[3];
    case 0x2724: return (f32)this->activeEclContext->extraIntVariables[0];
    case 0x2725: return (f32)this->activeEclContext->extraIntVariables[1];
    case 0x2726: return (f32)this->activeEclContext->extraIntVariables[2];
    case 0x2727: return (f32)this->activeEclContext->extraIntVariables[3];

    case 0x2720: return (f32)(g_Rng.GetRandomU32() & 0x7fffffff);
    case 0x2721: return g_Rng.GetRandomF32();
    case 0x2722: return (f32)(i32)g_Rng.GetRandomU32();
    case 0x2723: return g_Rng.GetRandomF32Signed();
    case 0x2751: return g_Rng.GetRandomF32() * 6.2831855f - 3.1415927f;

    case 0x2731: return (f32)TH095_ECL_TIMER_CURRENT(this);
    case 0x2733: return (f32)TH095_ECL_ENEMY_LIFE(this);
    case 0x275b: return (f32)TH095_ECL_ITEM_DROP_TYPE(this);
    case 0x275c: return (f32)TH095_ECL_ENEMY_SCORE(this);

    case 0x273c: return (f32)g_EclFloatOperandRuntime->sharedOperands->intVariables[0];
    case 0x273d: return (f32)g_EclFloatOperandRuntime->sharedOperands->intVariables[1];
    case 0x273e: return (f32)g_EclFloatOperandRuntime->sharedOperands->intVariables[2];
    case 0x273f: return (f32)g_EclFloatOperandRuntime->sharedOperands->intVariables[3];
    case 0x2740: return g_EclFloatOperandRuntime->sharedOperands->floatVariables[0];
    case 0x2741: return g_EclFloatOperandRuntime->sharedOperands->floatVariables[1];
    case 0x2742: return g_EclFloatOperandRuntime->sharedOperands->floatVariables[2];
    case 0x2743: return g_EclFloatOperandRuntime->sharedOperands->floatVariables[3];

    case 0x2718: return this->activeEclContext->floatVariables[0];
    case 0x2719: return this->activeEclContext->floatVariables[1];
    case 0x271a: return this->activeEclContext->floatVariables[2];
    case 0x271b: return this->activeEclContext->floatVariables[3];
    case 0x271c: return this->activeEclContext->floatVariables[4];
    case 0x271d: return this->activeEclContext->floatVariables[5];
    case 0x271e: return this->activeEclContext->floatVariables[6];
    case 0x271f: return this->activeEclContext->floatVariables[7];
    case 0x2738: return this->activeEclContext->callParameterFloats[0];
    case 0x2739: return this->activeEclContext->callParameterFloats[1];
    case 0x273a: return this->activeEclContext->callParameterFloats[2];
    case 0x273b: return this->activeEclContext->callParameterFloats[3];

    case 0x272a: return this->worldPosition.x;
    case 0x272b: return this->worldPosition.y;
    case 0x272c: return this->worldPosition.z;
    case 0x272d: return TH095_ECL_FLOAT_PLAYER_POSITION.x;
    case 0x272e: return TH095_ECL_FLOAT_PLAYER_POSITION.y;
    case 0x272f: return TH095_ECL_FLOAT_PLAYER_POSITION.z;
    case 0x275d: return this->activeEclContext->extraFloatVariables[0];
    case 0x275e: return this->activeEclContext->extraFloatVariables[1];
    case 0x275f: return this->activeEclContext->extraFloatVariables[2];
    case 0x2760: return this->activeEclContext->extraFloatVariables[3];
    case 0x2749: return this->movementInterpolationOrigin.x;
    case 0x274a: return this->movementInterpolationOrigin.y;
    case 0x274b: return this->movementInterpolationOrigin.z;
    case 0x274e: return this->movementInterpolationDelta.x;
    case 0x274f: return this->movementInterpolationDelta.y;
    case 0x2750: return this->movementInterpolationDelta.z;
    case 0x2754: return this->lastFrameDisplacement.x;
    case 0x2755: return this->lastFrameDisplacement.y;
    case 0x2756: return this->lastFrameDisplacement.z;

    case 0x2757: return (f32)TH095_ECL_SCHEDULED_FRAME0(this);
    case 0x2758: return (f32)TH095_ECL_SCHEDULED_FRAME1(this);
    case 0x2759: return (f32)TH095_ECL_SCHEDULED_FRAME2(this);
    case 0x275a: return (f32)TH095_ECL_SCHEDULED_FRAME3(this);
    case 0x2730: return TH095_ECL_FLOAT_PLAYER_ANGLE(&this->worldPosition);
    case 0x2765:
    {
        Float3 position = this->worldPosition + this->shootOffset;
        return TH095_ECL_FLOAT_PLAYER_ANGLE(&position);
    }

    case 0x2744: return this->movementAngle;
    case 0x2745: return this->angularVelocity;
    case 0x2746: return this->speed;
    case 0x2747: return this->acceleration;
    case 0x2748: return this->orbitRadius;
    case 0x274c: return this->orbitAngle;
    case 0x274d: return this->orbitAngularVelocity;
    case 0x2753: return (f32)ENEMY_U8(this, 0x2be5);
    case 0x2752: return (f32)ENEMY_I32(this, 0x2c50);
    case 0x2732:
    {
        Float3 delta = TH095_ECL_FLOAT_PLAYER_POSITION - this->worldPosition;
        return D3DXVec3Length(reinterpret_cast<D3DXVECTOR3 *>(&delta));
    }
    case 0x2761:
        return (f32)(i32)TH095_ECL_FLOAT_PHOTO_INDEX;
    case 0x2764:
        return (f32)(i32)TH095_ECL_FLOAT_PHOTOS_TAKEN;
    case 0x2762: return g_EclFloatOperandRuntime->bosses[0]->worldPosition.x;
    case 0x2763: return g_EclFloatOperandRuntime->bosses[0]->worldPosition.y;
    default: return operand;
    }
}

#undef TH095_ECL_FLOAT_PHOTOS_TAKEN
#undef TH095_ECL_FLOAT_PHOTO_INDEX
#undef TH095_ECL_FLOAT_PLAYER_POSITION
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
