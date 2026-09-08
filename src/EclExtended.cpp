#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "Rng.hpp"
#include "SoundPlayer.hpp"
#include <string.h>

namespace th095
{
extern f32 g_AnmGameSpeed;
#ifndef DIFFBUILD
extern AnmManager *g_AnmManager;
#endif
#ifndef TH095_MATCH_EXACT
struct Background;
extern Background *g_Background;
#endif

namespace EclExtended
{
struct AnmManagerLookupView
{
    AnmVm *GetVm(i32 handle);
    static i32 __fastcall ExecuteScript(AnmVm *vm);
};

struct ExtendedPhotoEnemyView;
struct ExtendedPhotoEnemyManagerView
{
    ExtendedPhotoEnemyView *Spawn(
        i32 subroutineId, const Float3 *position, i32 life,
        i32 itemDrop, i32 score, u32 mirrorMovementX);
};

struct PhotoGlobalStateView
{
    u8 unknown000[0xfc];
    u32 flags;
};
typedef char PhotoGlobalFlagsAtFC[(offsetof(PhotoGlobalStateView, flags) == 0xfc) ? 1 : -1];

struct ExtendedVector
{
    f32 x;
    f32 y;
    f32 z;
    void FromAngleMagnitude(f32 angle, f32 magnitude);
};
typedef char ExtendedVectorSizeC[(sizeof(ExtendedVector) == 0x0c) ? 1 : -1];

struct ExtendedVmHandle
{
    i32 value;
    AnmVm *GetVm();
    void SetSprite(i32 spriteIndex);
};

struct ExtendedAnmSpawner
{
    ExtendedVmHandle CreateVmAtWorld(i32 scriptIndex, Float3 *position);
    void CreateVmAtWorldInto(
        ExtendedVmHandle *output, i32 scriptIndex, Float3 *position);
    void InitializeVm(AnmVm *vm, i32 scriptIndex);
};

struct ExtendedPhotoEffectArgs
{
    Float3 position;
    f32 field0C;
    f32 field10;
    f32 field14;
    f32 angle;
    f32 angle2;
    f32 speed;
    f32 field24;
    f32 field28;
    f32 mode;
    i32 field30;
    i32 field34;
    i32 field38;
    i32 field3C;
    i16 type;
    i16 color;
    union
    {
        u32 flags;
        struct { u32 flag0 : 1; u32 flags01_31 : 31; };
    };
};
typedef char ExtendedPhotoEffectArgsSize48[(sizeof(ExtendedPhotoEffectArgs) == 0x48) ? 1 : -1];

struct ExtendedPhotoEffectNode
{
    u8 unknown000[8];
    ExtendedPhotoEffectNode *next;
    u8 unknown00c[0x40];
    i32 id;
    ExtendedPhotoEffectArgs spawn;
    AnmVm vm;
    u8 unknown364[0x228];
    u32 flags;
};
typedef char ExtendedPhotoEffectNodeIdAt4C[(offsetof(ExtendedPhotoEffectNode, id) == 0x4c) ? 1 : -1];
typedef char ExtendedPhotoEffectNodeVmAt98[(offsetof(ExtendedPhotoEffectNode, vm) == 0x98) ? 1 : -1];
typedef char ExtendedPhotoEffectNodeFlagsAt58C[(offsetof(ExtendedPhotoEffectNode, flags) == 0x58c) ? 1 : -1];

struct ExtendedPhotoEffectManager
{
    u8 unknown000[8];
    ExtendedPhotoEffectNode *first;
    u8 unknown00c[0x4c];
    i32 spawnedId;
    i32 Spawn(i32 type, void *args);
};
typedef char ExtendedPhotoEffectManagerSpawnedIdAt58[(offsetof(ExtendedPhotoEffectManager, spawnedId) == 0x58) ? 1 : -1];

struct ExtendedBulletManager
{
    u8 unknown000000[0x27c5b0];
    ExtendedAnmSpawner *anmSpawner;
};
typedef char ExtendedBulletManagerAnmAt27C5B0[
    (offsetof(ExtendedBulletManager, anmSpawner) == 0x27c5b0) ? 1 : -1];

struct ExtendedPhotoCameraView
{
    i32 mode;
    u8 unknown004[0xbc0];
    Float3 viewfinderPosition;
    Float3 viewfinderSize;

    i32 CountPhotoTargets(f32 *closestDistance, f32 *bossRate);
};
typedef char ExtendedCameraPositionAtBC4[
    (offsetof(ExtendedPhotoCameraView, viewfinderPosition) == 0xbc4) ? 1 : -1];
typedef char ExtendedCameraSizeBDC[
    (sizeof(ExtendedPhotoCameraView) == 0xbdc) ? 1 : -1];

struct ExtendedPlayerView
{
    u8 unknown0000[0x1e30];
    Float3 position;
    ExtendedPhotoCameraView camera;
    f32 proximityScale;
};
typedef char ExtendedPlayerPositionAt1E30[
    (offsetof(ExtendedPlayerView, position) == 0x1e30) ? 1 : -1];
typedef char ExtendedPlayerScaleAt2A18[
    (offsetof(ExtendedPlayerView, proximityScale) == 0x2a18) ? 1 : -1];

struct ExtendedBulletView
{
    u32 flags;
    AnmVm vm;
    ExtendedVector position;
    ExtendedVector velocity;
    ExtendedVector acceleration;
    f32 speed;
    u32 unknown2f8[2];
    f32 angle;
    u32 unknown304[2];
    ExtendedVector collisionSize;
    ZunTimer stateTimer;
    ZunTimer activeTimer;
    i32 ownerTag;
    u8 unknown334[0x14];
    i32 field348;
    i32 field34c;
    i16 unknown350;
    u16 state;
    u16 offscreenFrames;
    u16 unknown356;
    ExtendedBulletView *nextInDrawBucket;
    i32 field35c;
    i32 field360;
    i32 transformSound;
    i32 transformIndex;
    i32 drawBucketIndex;
    u8 unknown370[0x2e4];
    i8 collisionDisabled;
    u8 unknown655;
    i16 bulletType;
    i16 color;
    u8 trailingAlignment65A[2];
    void ReinitializeDirect();
    void ReinitializeShifted();
};
typedef char ExtendedBulletSize65C[
    (sizeof(ExtendedBulletView) == 0x65c) ? 1 : -1];
typedef char ExtendedBulletPositionAt2D0[
    (offsetof(ExtendedBulletView, position) == 0x2d0) ? 1 : -1];
typedef char ExtendedBulletVelocityAt2DC[
    (offsetof(ExtendedBulletView, velocity) == 0x2dc) ? 1 : -1];
typedef char ExtendedBulletSpeedAt2F4[
    (offsetof(ExtendedBulletView, speed) == 0x2f4) ? 1 : -1];
typedef char ExtendedBulletAngleAt300[
    (offsetof(ExtendedBulletView, angle) == 0x300) ? 1 : -1];
typedef char ExtendedBulletOwnerAt330[
    (offsetof(ExtendedBulletView, ownerTag) == 0x330) ? 1 : -1];
typedef char ExtendedBulletStateAt352[
    (offsetof(ExtendedBulletView, state) == 0x352) ? 1 : -1];

struct ExtendedRuntimeView
{
    u8 unknown0000[0x4df8];
    ExtendedAnmSpawner *markerAnm;

};
typedef char ExtendedRuntimeMarkerAt4DF8[
    (offsetof(ExtendedRuntimeView, markerAnm) == 0x4df8) ? 1 : -1];

extern AnmManagerLookupView *g_AnmManager;
#ifndef DIFFBUILD
#define g_AnmManager \
    (reinterpret_cast<AnmManagerLookupView *>(::th095::g_AnmManager))
#endif
extern PhotoGlobalStateView *g_PhotoGlobalState;
extern u8 *g_Background;
extern ExtendedBulletManager *g_PhotoBulletManager;
extern ExtendedPhotoEffectManager *g_PhotoEffectManager;
extern ExtendedPhotoEnemyManagerView *g_ExtendedPhotoEnemyManager;

#ifndef DIFFBUILD
static __forceinline u8 *ExtendedBackgroundOwner()
{
    return reinterpret_cast<u8 *>(::th095::g_Background);
}
#define g_Background ExtendedBackgroundOwner()
#define g_PhotoBulletManager \
    TH095_RUNTIME_GLOBAL_PTR(ExtendedBulletManager, ::th095::g_RuntimeBulletManagerOwner)
#define g_PhotoEffectManager \
    TH095_RUNTIME_GLOBAL_PTR(ExtendedPhotoEffectManager, ::th095::g_RuntimeEffectManagerOwner)
#define g_PhotoGlobalState \
    TH095_RUNTIME_GLOBAL_PTR(PhotoGlobalStateView, ::th095::g_RuntimeGameTaskOwner)
#endif

i32 __fastcall GetPhotoBulletScriptBase(i32 bulletType);

__forceinline void ExtendedBulletView::ReinitializeDirect()
{
    // Extended entries 2/3 repeat this target 0x2C InitializeVm phase.
    u8 compilerStorage[0x2c];
    g_PhotoBulletManager->anmSpawner->InitializeVm(
        &this->vm, GetPhotoBulletScriptBase(this->bulletType) + this->color);
}

__forceinline void ExtendedBulletView::ReinitializeShifted()
{
    // Entry 2 uses the same phase but selects the shifted script bank.
    u8 compilerStorage[0x2c];
    g_PhotoBulletManager->anmSpawner->InitializeVm(
        &this->vm, GetPhotoBulletScriptBase(this->bulletType) + 0x10 + this->color);
}

#ifdef TH095_MATCH_EXACT
static __forceinline void InitializeExtendedTimerExact(ZunTimer *timer)
{
    timer->current = 0;
    timer->subFrame = 0.0f;
    timer->previous = -999999;
}
#endif

static __forceinline void FinalizeExtendedBulletAfterExecute(
    AnmVm *vm, i32 interpolationMode)
{
    // Target-strict parameter order: the two inline value homes follow the
    // two timer-generated compiler temporaries. Reversing the parameters
    // changes the 145-instruction body.
#ifdef TH095_MATCH_EXACT
    InitializeExtendedTimerExact(&vm->interpCurrentTimers[2]);
#else
    vm->interpCurrentTimers[2].Initialize();
#endif
    vm->interpEndTimers[2] = 1;
    vm->interpModes[2] = (u8)interpolationMode;
    vm->color1Initial.a = 0xff;
    vm->color1Final.a = 0x40;
}

extern ExtendedPlayerView *g_Player;
extern ExtendedRuntimeView *g_ExtendedRuntime;
#ifndef DIFFBUILD
#define g_ExtendedPhotoEnemyManager \
    TH095_RUNTIME_GLOBAL_PTR(ExtendedPhotoEnemyManagerView, ::th095::g_RuntimeEnemyManagerOwner)
#define g_ExtendedRuntime \
    TH095_RUNTIME_GLOBAL_PTR(ExtendedRuntimeView, ::th095::g_RuntimeEnemyManagerOwner)
#endif

#ifdef TH095_MATCH_EXACT
struct SoundPlayerView
{
    void PlaySoundByIdx(i32 soundIndex, i32 pan);
};
struct ExtendedRng
{
    f32 GetRandomF32();
};
extern SoundPlayerView g_SoundPlayer;
extern ExtendedRng g_Rng;
extern f32 g_AnmGameSpeed;
extern u32 g_PhotoScreenFadeColor;
#define TH095_ECL_EXT_SOUND_PLAYER g_SoundPlayer
#define TH095_ECL_EXT_RNG g_Rng
#define TH095_ECL_EXT_GAME_SPEED g_AnmGameSpeed
#define TH095_ECL_EXT_FADE_COLOR g_PhotoScreenFadeColor
#else
#define TH095_ECL_EXT_SOUND_PLAYER ::th095::g_SoundPlayer
#define TH095_ECL_EXT_RNG ::th095::g_Rng
#define TH095_ECL_EXT_GAME_SPEED ::th095::g_AnmGameSpeed
#define TH095_ECL_EXT_FADE_COLOR ::th095::g_PhotoScreenFadeColor
#endif

#ifndef DIFFBUILD
#define g_Player \
    TH095_RUNTIME_GLOBAL_PTR(ExtendedPlayerView, ::th095::g_RuntimePlayerOwner)
#endif
Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);
i32 __fastcall DispatchExtendedValue(
    i32 mode, i32 value0, i32 value1, i32 value2, i32 value3, i32 type);

// ECL extended callback table entry 0 @ 0x00413380.
void __fastcall SpawnDeathPhotoVms(
    Enemy *enemy, EclRawInstruction *instruction)
{
    g_PhotoBulletManager->anmSpawner->CreateVmAtWorld(0x123, &enemy->position);
    for (i32 i = 0; i < 32; ++i)
        g_PhotoBulletManager->anmSpawner->CreateVmAtWorld(0x122, &enemy->position);
    TH095_ECL_EXT_SOUND_PLAYER.PlaySoundByIdx((SoundIdx)0x12, 0);
    TH095_ECL_EXT_GAME_SPEED = 0.25f;
}

// ECL extended callback table entry 6 @ 0x00413AA0.
void __fastcall UpdatePlayerProximityAndMarker(
    Enemy *enemy, EclRawInstruction *instruction)
{
    struct ProximityLocals
    {
        Float3 *playerPosition;
        Float3 *enemyPosition;
        AnmVm *vm;
        f32 distanceSquared;
    } locals;

    locals.enemyPosition = &enemy->position;
    locals.playerPosition = &g_Player->position;
    locals.distanceSquared =
        (locals.playerPosition->y - locals.enemyPosition->y) *
            (locals.playerPosition->y - locals.enemyPosition->y) +
        (locals.playerPosition->x - locals.enemyPosition->x) *
            (locals.playerPosition->x - locals.enemyPosition->x);
    if (locals.distanceSquared < 1024.0f)
        g_Player->proximityScale = 0.25f;
    else if (locals.distanceSquared < 4096.0f)
        g_Player->proximityScale =
            (locals.distanceSquared - 1024.0f) / 3072.0f * 0.75f + 0.25f;

    locals.vm = g_AnmManager->GetVm(
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2d4));
    if (locals.vm != NULL)
        PhotoToScreen(&locals.vm->positionOffset, &enemy->position);
}

// ECL extended callback table entry 9 @ 0x00413DA0.
void __fastcall DispatchContextValues(
    Enemy *enemy, EclRawInstruction *instruction)
{
    DispatchExtendedValue(
        7,
        enemy->activeEclContext->intVariables[0],
        enemy->activeEclContext->intVariables[1],
        enemy->activeEclContext->intVariables[2],
        enemy->activeEclContext->intVariables[3],
        29);
}

// ECL extended callback table entry 11 @ 0x00413F90.
void __fastcall PublishGameSpeed(
    Enemy *enemy, EclRawInstruction *instruction)
{
    TH095_ECL_EXT_GAME_SPEED = enemy->activeEclContext->floatVariables[7];
}

// ECL extended callback table entry 12 @ 0x00413FC0.
void __fastcall SetBackgroundVmsState2(
    Enemy *enemy, EclRawInstruction *instruction)
{
    AnmVm *secondVm;
    AnmVm *firstVm;
    firstVm = g_AnmManager->GetVm(
        *reinterpret_cast<i32 *>(g_Background + 0x1fe4));
    firstVm->pendingInterrupt = 2;
    secondVm = g_AnmManager->GetVm(
        *reinterpret_cast<i32 *>(g_Background + 0x1fe8));
    secondVm->pendingInterrupt = 2;
}

// ECL extended callback table entry 13 @ 0x00414020.
void __fastcall SetBackgroundVmsState3(
    Enemy *enemy, EclRawInstruction *instruction)
{
    AnmVm *secondVm;
    AnmVm *firstVm;
    firstVm = g_AnmManager->GetVm(
        *reinterpret_cast<i32 *>(g_Background + 0x1fe4));
    firstVm->pendingInterrupt = 3;
    secondVm = g_AnmManager->GetVm(
        *reinterpret_cast<i32 *>(g_Background + 0x1fe8));
    secondVm->pendingInterrupt = 3;
    TH095_ECL_EXT_GAME_SPEED = 1.0f;
}

// ECL extended callback table entry 15 @ 0x00414230.
void __fastcall SetPhotoFlag200(
    Enemy *enemy, EclRawInstruction *instruction)
{
    g_PhotoGlobalState->flags |= 0x200;
}

// ECL extended callback table entry 16 @ 0x00414260.
void __fastcall ClearPhotoFlag200(
    Enemy *enemy, EclRawInstruction *instruction)
{
    g_PhotoGlobalState->flags &= ~0x200U;
}

// ECL extended callback table entry 18 @ 0x00414430.
void __fastcall EnablePhotoTransition(
    Enemy *enemy, EclRawInstruction *instruction)
{
    AnmVm *secondVm;
    AnmVm *firstVm;
    g_PhotoGlobalState->flags |= 0x400;
    firstVm = g_AnmManager->GetVm(
        *reinterpret_cast<i32 *>(g_Background + 0x1fe4));
    firstVm->pendingInterrupt = 2;
    AnmManagerLookupView::ExecuteScript(firstVm);
    secondVm = g_AnmManager->GetVm(
        *reinterpret_cast<i32 *>(g_Background + 0x1fe8));
    secondVm->pendingInterrupt = 2;
    AnmManagerLookupView::ExecuteScript(secondVm);
    TH095_ECL_EXT_SOUND_PLAYER.PlaySoundByIdx((SoundIdx)0x26, 0);
    TH095_ECL_EXT_GAME_SPEED = 1.0f;
}

// ECL extended callback table entry 19 @ 0x004144E0.
void __fastcall DisablePhotoTransition(
    Enemy *enemy, EclRawInstruction *instruction)
{
    AnmVm *secondVm;
    AnmVm *firstVm;
    g_PhotoGlobalState->flags &= ~0x400U;
    firstVm = g_AnmManager->GetVm(
        *reinterpret_cast<i32 *>(g_Background + 0x1fe4));
    firstVm->pendingInterrupt = 3;
    AnmManagerLookupView::ExecuteScript(firstVm);
    secondVm = g_AnmManager->GetVm(
        *reinterpret_cast<i32 *>(g_Background + 0x1fe8));
    secondVm->pendingInterrupt = 3;
    AnmManagerLookupView::ExecuteScript(secondVm);
    TH095_ECL_EXT_SOUND_PLAYER.PlaySoundByIdx((SoundIdx)0x0f, 0);
}

// ECL extended callback table entry 21 @ 0x00414930.
void __fastcall ResetOwnedBulletMotion(
    Enemy *enemy, EclRawInstruction *instruction)
{
    ExtendedBulletView *bullet =
        reinterpret_cast<ExtendedBulletView *>(
            reinterpret_cast<u8 *>(g_PhotoBulletManager) + 0x4c);
    for (i32 i = 0; i < 0x640; ++i, ++bullet)
    {
        if (bullet->state == 0)
            continue;
        if (bullet->ownerTag ==
            enemy->activeEclContext->extraIntVariables[2])
        {
            bullet->field34c = 0;
            bullet->field348 = 0;
            bullet->speed = 4.5f;
            bullet->velocity.FromAngleMagnitude(
                bullet->angle, bullet->speed);
        }
    }
}



// ECL extended callback table entry 5 @ 0x00413990.
void __fastcall FadeOwnedCapturedBullets(
    Enemy *enemy, EclRawInstruction *instruction)
{
    struct FadeLocals
    {
        AnmVm *vm;
        i32 interpolationMode;
        ZunTimer *endTimer;
        ZunTimer *currentTimer;
        i32 i;
        ExtendedBulletView *bullet;
    } locals;

    locals.bullet = reinterpret_cast<ExtendedBulletView *>(
        reinterpret_cast<u8 *>(g_PhotoBulletManager) + 0x4c);
    for (locals.i = 0; locals.i < 0x640; ++locals.i, ++locals.bullet)
    {
        if (locals.bullet->state == 0)
            continue;
        if (locals.bullet->ownerTag ==
            enemy->activeEclContext->extraIntVariables[2])
        {
            if (((locals.bullet->flags >> 4) & 1U) != 0)
            {
                locals.interpolationMode = 0;
                locals.vm = &locals.bullet->vm;
                locals.currentTimer = &locals.vm->interpCurrentTimers[2];
                locals.currentTimer->current = 0;
                locals.currentTimer->subFrame = 0.0f;
                locals.currentTimer->previous = -999999;
                locals.endTimer = &locals.vm->interpEndTimers[2];
                locals.endTimer->current = 30;
                locals.endTimer->subFrame = 30.0f;
                locals.endTimer->previous = -999999;
                locals.vm->interpModes[2] =
                    static_cast<u8>(locals.interpolationMode);
                locals.vm->color1Initial.a = 0x40;
                locals.vm->color1Final.a = 0xff;
            }
        }
    }
}


// ECL extended callback table entry 7 @ 0x00413B90.
// One real 12-byte vector local is reused: first as the 24px direction
// offset, then as the projected first-marker position.
void __fastcall UpdateEnemyMarkerVms(Enemy *enemy, EclRawInstruction *instruction)
{
    ExtendedVector position;
    AnmVm *firstVm;
    firstVm = g_AnmManager->GetVm(*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2d4));
    if (firstVm != NULL)
    {
        position.FromAngleMagnitude(enemy->movementAngle, 24.0f);
        firstVm->positionOffset = enemy->position + *reinterpret_cast<Float3 *>(&position);
        PhotoToScreen(&firstVm->positionOffset, &firstVm->positionOffset);
        firstVm->positionOffset.x -= 128.0f;
        firstVm->positionOffset.y -= 16.0f;
        firstVm->rotation.z = enemy->movementAngle;
        position = *reinterpret_cast<ExtendedVector *>(&firstVm->positionOffset);
        firstVm = g_AnmManager->GetVm(*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2d8));
        firstVm->positionOffset = *reinterpret_cast<Float3 *>(&position);
        firstVm->rotation.z = enemy->movementAngle;
    }
}

// ECL extended callback table entry 8 @ 0x00413CF0.
void __fastcall SpawnEnemyMarkerVm(
    Enemy *enemy, EclRawInstruction *instruction)
{
    struct MarkerLocals
    {
        AnmVm *vm;
        ExtendedVmHandle handle;
    } locals;

    g_ExtendedRuntime->markerAnm->CreateVmAtWorldInto(
        &locals.handle,
        enemy->activeEclContext->extraIntVariables[2],
        &enemy->worldPosition);
    locals.vm = locals.handle.GetVm();
    locals.handle.SetSprite(enemy->vm.activeSpriteIndex);
    locals.vm->flip = enemy->vm.flip;
    locals.vm->rotation = enemy->vm.rotation;
}

static __forceinline i32 ExtendedCameraIsCharging(
    ExtendedPhotoCameraView *camera)
{
    return camera->mode == 1;
}

#define EXT_MOVEMENT_FLAGS(enemy) \
    (*reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4))


// ECL extended callback table entry 20 @ 0x00414580.
void __fastcall RunPhotoTransition(
    Enemy *enemy, EclRawInstruction *instruction)
{
    struct TransitionLocals
    {
        ZunTimer *movementTimer;
        Float3 *worldPosition;
        f32 deltaX;
        f32 deltaY;
        f32 deltaZ;
        AnmVm *secondStartVm;
        AnmVm *firstStartVm;
        AnmVm *secondEndVm;
        AnmVm *firstEndVm;
        Float3 zeroVelocity;
        Float3 movementDelta;
        f32 targetX;
        f32 targetY;
        f32 zeroZ;
    } locals;

    if (enemy->activeEclContext->extraIntVariables[2] != 0)
    {
        --enemy->activeEclContext->extraIntVariables[2];
        if (enemy->activeEclContext->extraIntVariables[2] == 60)
        {
            g_PhotoGlobalState->flags &= ~0x400U;
            locals.firstEndVm = g_AnmManager->GetVm(
                *reinterpret_cast<i32 *>(g_Background + 0x1fe4));
            locals.firstEndVm->pendingInterrupt = 3;
            AnmManagerLookupView::ExecuteScript(locals.firstEndVm);
            locals.secondEndVm = g_AnmManager->GetVm(
                *reinterpret_cast<i32 *>(g_Background + 0x1fe8));
            locals.secondEndVm->pendingInterrupt = 3;
            AnmManagerLookupView::ExecuteScript(locals.secondEndVm);
            TH095_ECL_EXT_SOUND_PLAYER.PlaySoundByIdx((SoundIdx)0x0f, 0);
        }
    }

    if (((g_PhotoGlobalState->flags >> 10) & 1U) == 0 &&
        enemy->activeEclContext->extraIntVariables[2] == 0 &&
        ExtendedCameraIsCharging(&g_Player->camera) &&
        g_Player->camera.CountPhotoTargets(NULL, NULL) != 0)
    {
        g_PhotoGlobalState->flags |= 0x400U;
        locals.firstStartVm = g_AnmManager->GetVm(
            *reinterpret_cast<i32 *>(g_Background + 0x1fe4));
        locals.firstStartVm->pendingInterrupt = 2;
        AnmManagerLookupView::ExecuteScript(locals.firstStartVm);
        locals.secondStartVm = g_AnmManager->GetVm(
            *reinterpret_cast<i32 *>(g_Background + 0x1fe8));
        locals.secondStartVm->pendingInterrupt = 2;
        AnmManagerLookupView::ExecuteScript(locals.secondStartVm);
        TH095_ECL_EXT_SOUND_PLAYER.PlaySoundByIdx((SoundIdx)0x26, 0);
        TH095_ECL_EXT_GAME_SPEED = 1.0f;
        enemy->activeEclContext->extraIntVariables[2] = 120;

        if (g_Player->camera.viewfinderPosition.x < 0.0f)
            locals.targetX =
                g_Player->camera.viewfinderSize.x * 0.60000002f +
                g_Player->camera.viewfinderPosition.x;
        else
            locals.targetX =
                g_Player->camera.viewfinderPosition.x -
                g_Player->camera.viewfinderSize.x * 0.60000002f;

        if (g_Player->position.y < enemy->position.y)
            locals.targetY =
                TH095_ECL_EXT_RNG.GetRandomF32() * 64.0f + enemy->position.y;
        else
            locals.targetY =
                enemy->position.y - TH095_ECL_EXT_RNG.GetRandomF32() * 64.0f;

        locals.zeroZ = 0.0f;
        locals.worldPosition = &enemy->worldPosition;
        locals.deltaZ = locals.zeroZ - locals.worldPosition->z;
        locals.deltaY = locals.targetY - locals.worldPosition->y;
        locals.deltaX = locals.targetX - locals.worldPosition->x;
        locals.movementDelta.x = locals.deltaX;
        locals.movementDelta.y = locals.deltaY;
        locals.movementDelta.z = locals.deltaZ;
        enemy->movementInterpolationDelta = locals.movementDelta;
        enemy->movementInterpolationOrigin = enemy->position;

        enemy->movementDuration = 60;
        locals.movementTimer = &enemy->movementTimer;
        locals.movementTimer->current = 60;
        locals.movementTimer->subFrame = 60.0f;
        locals.movementTimer->previous = -999999;

        EXT_MOVEMENT_FLAGS(enemy) =
            (EXT_MOVEMENT_FLAGS(enemy) & 0xffff8fffU) | 0x4000U;
        EXT_MOVEMENT_FLAGS(enemy) =
            (EXT_MOVEMENT_FLAGS(enemy) & 0xfffff3ffU) | 0x0800U;

        locals.zeroVelocity.x = 0.0f;
        locals.zeroVelocity.y = 0.0f;
        locals.zeroVelocity.z = 0.0f;
        enemy->velocity = locals.zeroVelocity;
    }
}

struct ExtendedEffectCallbackLocals
{
    ExtendedPhotoEffectArgs args;
    ExtendedPhotoEffectNode *effect;
    i32 spawnId;
    __forceinline void PublishFlags()
    {
        // Entries 10/14/17 independently repeat this target tail class.
        // VC7.1 places the callback fastcall homes after this inline phase;
        // 0x28/0x30 controls leave them one dword shallow/deep.
        u8 compilerStorage[0x2c];
        this->effect->flags &= ~2U;
    }
};
typedef char ExtendedEffectCallbackLocalsSize50[(sizeof(ExtendedEffectCallbackLocals) == 0x50) ? 1 : -1];

static __forceinline void FindSpawnedExtendedEffect(ExtendedEffectCallbackLocals *locals)
{
    ExtendedPhotoEffectNode *cursor;
    cursor = g_PhotoEffectManager->first;
    while (cursor != NULL)
    {
        if (cursor->id == g_PhotoEffectManager->spawnedId)
        {
            locals->effect = cursor;
            return;
        }
        cursor = cursor->next;
    }
    locals->effect = NULL;
}

// ECL extended callback table entry 10 @ 0x00413DF0.
void __fastcall Callback10(Enemy *enemy, EclRawInstruction *instruction)
{
    ExtendedEffectCallbackLocals locals;

    memset(&locals.args, 0, sizeof(locals.args));
    locals.args.mode = 8.0f;
    locals.args.position = enemy->worldPosition + enemy->shootOffset;
    locals.args.type = 0;
    locals.args.color = 0;
    locals.args.angle = *reinterpret_cast<f32 *>(
        reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x70);
    locals.args.speed = *reinterpret_cast<f32 *>(
        reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x74);
    locals.args.field24 = locals.args.speed;
    locals.args.field28 = 16.0f;
    locals.args.field30 = 1;
    locals.args.field34 = 15;
    locals.args.field38 = 40;
    locals.args.field3C = 6;
    locals.args.angle2 = 0.0f;
    locals.args.flag0 = 0;

    locals.spawnId = g_PhotoEffectManager->Spawn(1, &locals.args);
    FindSpawnedExtendedEffect(&locals);

    g_ExtendedRuntime->markerAnm->InitializeVm(
        &locals.effect->vm,
        *reinterpret_cast<i32 *>(
            reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x60));
    locals.PublishFlags();
}


// ECL extended callback table entry 14 @ 0x00414090.
void __fastcall Callback14(Enemy *enemy, EclRawInstruction *instruction)
{
    ExtendedEffectCallbackLocals locals;

    memset(&locals.args, 0, sizeof(locals.args));
    locals.args.mode = 8.0f;
    locals.args.position = enemy->worldPosition + enemy->shootOffset;
    locals.args.type = 0;
    locals.args.color = 0;
    locals.args.angle = *reinterpret_cast<f32 *>(
        reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x70);
    locals.args.speed = *reinterpret_cast<f32 *>(
        reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x74);
    locals.args.field24 = locals.args.speed;
    locals.args.field28 = 16.0f;
    locals.args.field30 = 1;
    locals.args.field34 = 15;
    locals.args.field38 = 300;
    locals.args.field3C = 6;
    locals.args.angle2 = 0.0f;
    locals.args.flag0 = 0;

    locals.spawnId = g_PhotoEffectManager->Spawn(1, &locals.args);
    FindSpawnedExtendedEffect(&locals);

    g_ExtendedRuntime->markerAnm->InitializeVm(
        &locals.effect->vm,
        *reinterpret_cast<i32 *>(
            reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x60));
    locals.PublishFlags();
}


// ECL extended callback table entry 17 @ 0x00414290.
void __fastcall Callback17(Enemy *enemy, EclRawInstruction *instruction)
{
    ExtendedEffectCallbackLocals locals;

    memset(&locals.args, 0, sizeof(locals.args));
    locals.args.mode = 8.0f;
    locals.args.position = enemy->worldPosition + enemy->shootOffset;
    locals.args.type = 0;
    locals.args.color = 0;
    locals.args.angle = *reinterpret_cast<f32 *>(
        reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x70);
    locals.args.speed = *reinterpret_cast<f32 *>(
        reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x74);
    locals.args.field24 = locals.args.speed;
    locals.args.field28 = 16.0f;
    locals.args.field30 = 1;
    locals.args.field34 = 15;
    locals.args.field38 = 120;
    locals.args.field3C = 6;
    locals.args.angle2 = 0.0f;
    locals.args.flag0 = 0;

    locals.spawnId = g_PhotoEffectManager->Spawn(1, &locals.args);
    FindSpawnedExtendedEffect(&locals);

    g_ExtendedRuntime->markerAnm->InitializeVm(
        &locals.effect->vm,
        *reinterpret_cast<i32 *>(
            reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x60));
    locals.PublishFlags();
}


static __forceinline void SetExtendedBackgroundVm0State2()
{
    AnmVm *vm;
    vm = g_AnmManager->GetVm(
        *reinterpret_cast<i32 *>(g_Background + 0x1fe4));
    vm->pendingInterrupt = 2;
}

static __forceinline void SetExtendedBackgroundVm1State2()
{
    AnmVm *vm;
    vm = g_AnmManager->GetVm(
        *reinterpret_cast<i32 *>(g_Background + 0x1fe8));
    vm->pendingInterrupt = 2;
}

static __forceinline void SetExtendedBackgroundVm0State3()
{
    AnmVm *vm;
    vm = g_AnmManager->GetVm(
        *reinterpret_cast<i32 *>(g_Background + 0x1fe4));
    vm->pendingInterrupt = 3;
}

static __forceinline void SetExtendedBackgroundVm1State3()
{
    AnmVm *vm;
    vm = g_AnmManager->GetVm(
        *reinterpret_cast<i32 *>(g_Background + 0x1fe8));
    vm->pendingInterrupt = 3;
}

// ECL extended callback table entry 1 @ 0x00413410.
void __fastcall Callback01(Enemy *enemy, EclRawInstruction *instruction)
{
    ExtendedBulletView *index;
    i32 bullet;

    index = reinterpret_cast<ExtendedBulletView *>(
        reinterpret_cast<u8 *>(g_PhotoBulletManager) + 0x4c);
    for (bullet = 0; bullet < 0x640; bullet++, index++)
    {
        if (index->state == 0 || index->vm.loadedSprite->widthPx < 64.0f)
            continue;

        g_ExtendedPhotoEnemyManager->Spawn(
            0, reinterpret_cast<const Float3 *>(&index->position),
            1, 0, 0, 0);
    }
}

// ECL extended callback table entry 2 @ 0x004134A0.
void __fastcall Callback02(Enemy *enemy, EclRawInstruction *instruction)
{
    u32 savedActiveSprite;
    ExtendedBulletView *index;
    i32 bullet;

    index = reinterpret_cast<ExtendedBulletView *>(
        reinterpret_cast<u8 *>(g_PhotoBulletManager) + 0x4c);
    for (bullet = 0; bullet < 0x640; bullet++, index++)
    {
        if (index->state == 0)
            continue;
        if (index->ownerTag == *reinterpret_cast<i32 *>(
                reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x60))
        {
            savedActiveSprite = *reinterpret_cast<u32 *>(
                reinterpret_cast<u8 *>(index) + 0x24);
            index->ReinitializeShifted();
            *reinterpret_cast<u32 *>(
                reinterpret_cast<u8 *>(&index->vm) + 0x228) &= 0xf7ffffffU;
            index->vm.pendingInterrupt = 2;
            *reinterpret_cast<u32 *>(
                reinterpret_cast<u8 *>(index) + 0x24) = savedActiveSprite;
            index->velocity.FromAngleMagnitude(
                *reinterpret_cast<f32 *>(
                    reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x70),
                *reinterpret_cast<f32 *>(
                    reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x74));
            index->flags &= ~2U;
            index->flags |= 0x10U;
        }
    }

    SetExtendedBackgroundVm0State2();
    SetExtendedBackgroundVm1State2();
    TH095_ECL_EXT_FADE_COLOR = 0;
}

// ECL extended callback table entry 3 @ 0x00413620.
void __fastcall Callback03(Enemy *enemy, EclRawInstruction *instruction)
{
    ExtendedBulletView *index;
    i32 bullet;

    index = reinterpret_cast<ExtendedBulletView *>(
        reinterpret_cast<u8 *>(g_PhotoBulletManager) + 0x4c);
    for (bullet = 0; bullet < 0x640; bullet++, index++)
    {
        if (index->state == 0)
            continue;

        index->ReinitializeDirect();
        index->vm.pendingInterrupt = 2;
        index->velocity.FromAngleMagnitude(index->angle, index->speed);
        index->flags |= 2U;
        index->flags &= ~0x10U;
    }

    SetExtendedBackgroundVm0State3();
    SetExtendedBackgroundVm1State3();
}

// ECL extended callback table entry 4 @ 0x00413750.
void __fastcall Callback04(Enemy *enemy, EclRawInstruction *instruction)
{
    u32 savedActiveSprite;
    ExtendedBulletView *index;
    i32 bullet;

    index = reinterpret_cast<ExtendedBulletView *>(
        reinterpret_cast<u8 *>(g_PhotoBulletManager) + 0x4c);
    for (bullet = 0; bullet < 0x640; bullet++, index++)
    {
        if (index->state == 0)
            continue;
        if (index->ownerTag == *reinterpret_cast<i32 *>(
                reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x60))
        {
            savedActiveSprite = *reinterpret_cast<u32 *>(
                reinterpret_cast<u8 *>(index) + 0x24);
            index->ReinitializeShifted();
            index->vm.flagsWord &= 0xf7ffffffU;
            index->vm.pendingInterrupt = 2;
            *reinterpret_cast<u32 *>(
                reinterpret_cast<u8 *>(index) + 0x24) = savedActiveSprite;
            index->velocity.FromAngleMagnitude(
                *reinterpret_cast<f32 *>(
                    reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x70) +
                    index->angle,
                *reinterpret_cast<f32 *>(
                    reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x74) > -999.0f
                    ? *reinterpret_cast<f32 *>(
                          reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x74)
                    : index->speed);
            index->flags &= ~2U;
            index->flags |= 0x10U;
            AnmManagerLookupView::ExecuteScript(&index->vm);
            FinalizeExtendedBulletAfterExecute(&index->vm, 0);
        }
    }

    SetExtendedBackgroundVm0State2();
    SetExtendedBackgroundVm1State2();
    TH095_ECL_EXT_FADE_COLOR = 0;
}

#undef EXT_MOVEMENT_FLAGS

} // namespace EclExtended
} // namespace th095
