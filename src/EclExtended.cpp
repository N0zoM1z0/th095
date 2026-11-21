#include "PhotoGameTask.hpp"
#include "ZunBool.hpp"
#include "Background.hpp"
#include "PhotoEnemyLayout.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoGamePlayer.hpp"
#include "PhotoEnemyControl.hpp"
#include "PhotoRotatingLaser.hpp"
#include "PhotoEffectRuntime.hpp"
#include "PhotoEnemyManagerLayout.hpp"
#include "PhotoBulletManagerLayout.hpp"
#ifndef DIFFBUILD
#include "ScreenEffect.hpp"
#endif
#include "Rng.hpp"
#include "SoundPlayer.hpp"
#include <string.h>

namespace th095
{
extern f32 g_AnmGameSpeed;
#ifndef DIFFBUILD
i32 __fastcall GetPhotoBulletScriptBase(i32 bulletType);
Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);
extern AnmManager *g_AnmManager;
static __forceinline AnmManager *EclExtendedCanonicalAnmManager()
{
    return g_AnmManager;
}
#endif

namespace EclExtended
{
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
#include "ecl/EclExtendedAnmEmission.inl"
#else
// Production uses the canonical runtime types.  The legacy names remain only
// as local token aliases so the shared callback bodies do not acquire a second
// handle or loaded-ANM representation.
typedef AnmVmId ExtendedVmHandle;
typedef AnmLoaded ExtendedAnmSpawner;
#endif

#ifdef DIFFBUILD
#include "ecl/EclExtendedMathEmission.inl"
#define TH095_EXTENDED_FROM_ANGLE(vector, angle, magnitude) \
    vector.FromAngleMagnitude(angle, magnitude)
#else
typedef Float3 ExtendedVector;
#define TH095_EXTENDED_FROM_ANGLE(vector, angle, magnitude) \
    reinterpret_cast<Float3 *>(&(vector))->FromAngleMagnitude( \
        (angle), (magnitude))
#endif

#ifdef DIFFBUILD
#define TH095_EXT_ANM_INITIALIZE(spawner, vm, script) \
    (spawner)->InitializeVm((vm), (script))
#define TH095_EXT_ANM_EXECUTE(vm) AnmManagerLookupView::ExecuteScript(vm)
#else
#define TH095_EXT_ANM_INITIALIZE(spawner, vm, script) \
    (spawner)->InitializeVm((vm), (script))
#define TH095_EXT_ANM_EXECUTE(vm) ::th095::AnmManager::ExecuteScript(vm)
#endif

#ifdef DIFFBUILD
#define TH095_EXT_ANM_GET_VM(handle) g_AnmManager->GetVm(handle)
#define TH095_EXT_CREATE_VM_WORLD(spawner, script, position)     (spawner)->CreateVmAtWorld((script), (position))
#define TH095_EXT_CREATE_VM_WORLD_INTO(spawner, output, script, position)     (spawner)->CreateVmAtWorldInto((output), (script), (position))
#define TH095_EXT_HANDLE_GET_VM(handle) (handle).GetVm()
#define TH095_EXT_HANDLE_SET_SPRITE(handle, sprite) (handle).SetSprite(sprite)
#else
static __forceinline AnmVmId ExtendedCanonicalAnmId(i32 value)
{
    AnmVmId id;
    id.value = value;
    return id;
}
#define TH095_EXT_ANM_GET_VM(handle)     ::th095::EclExtendedCanonicalAnmManager()->GetVm(         ExtendedCanonicalAnmId(handle))
#define TH095_EXT_CREATE_VM_WORLD(spawner, script, position)     (spawner)->CreateVmAtWorld((script), (position))
#define TH095_EXT_CREATE_VM_WORLD_INTO(spawner, output, script, position)     (*(output) = (spawner)->CreateVmAtWorld((script), (position)))
#define TH095_EXT_HANDLE_GET_VM(handle) (handle).GetVm()
#define TH095_EXT_HANDLE_SET_SPRITE(handle, sprite) (handle).SetSprite(sprite)
#endif

#ifdef DIFFBUILD
#define TH095_EXT_PHOTO_TO_SCREEN(output, position) \
    PhotoToScreen((output), (position))
#else
#define TH095_EXT_PHOTO_TO_SCREEN(output, position) \
    ::th095::PhotoToScreen((output), (position))
#endif

#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
#include "ecl/EclExtendedBulletEmission.inl"
#else
typedef ::th095::PhotoBulletView ExtendedBulletView;
#endif

#ifdef DIFFBUILD
extern AnmManagerLookupView *g_AnmManager;
#endif
#ifdef DIFFBUILD
i32 __fastcall GetPhotoBulletScriptBase(i32 bulletType);
#define TH095_EXTENDED_SCRIPT_BASE GetPhotoBulletScriptBase
#else
#define TH095_EXTENDED_SCRIPT_BASE ::th095::GetPhotoBulletScriptBase
#endif

// The legacy ECL ANM declaration needs the existing method receiver.
#define TH095_EXT_BULLET_ANM \
    reinterpret_cast<ExtendedAnmSpawner *>(g_PhotoBulletManager->bulletAnm)

#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
__forceinline void ExtendedBulletView::ReinitializeDirect()
{
    // Extended entries 2/3 repeat this target 0x2C InitializeVm phase.
    u8 compilerStorage[0x2c];
    TH095_EXT_ANM_INITIALIZE(
        TH095_EXT_BULLET_ANM, &this->vm,
        TH095_EXTENDED_SCRIPT_BASE(this->bulletType) + this->color);
}

__forceinline void ExtendedBulletView::ReinitializeShifted()
{
    // Entry 2 uses the same phase but selects the shifted script bank.
    u8 compilerStorage[0x2c];
    TH095_EXT_ANM_INITIALIZE(
        TH095_EXT_BULLET_ANM, &this->vm,
        TH095_EXTENDED_SCRIPT_BASE(this->bulletType) + 0x10 + this->color);
}
#define TH095_EXT_REINITIALIZE_DIRECT(bullet) (bullet)->ReinitializeDirect()
#define TH095_EXT_REINITIALIZE_SHIFTED(bullet) (bullet)->ReinitializeShifted()
#else
static __forceinline void ReinitializeExtendedBulletDirect(
    ExtendedBulletView *bullet)
{
    // The normal callback operates on the canonical BulletInf element.
    u8 compilerStorage[0x2c];
    TH095_EXT_ANM_INITIALIZE(
        TH095_EXT_BULLET_ANM, &bullet->vm,
        TH095_EXTENDED_SCRIPT_BASE(bullet->bulletType) + bullet->color);
}

static __forceinline void ReinitializeExtendedBulletShifted(
    ExtendedBulletView *bullet)
{
    u8 compilerStorage[0x2c];
    TH095_EXT_ANM_INITIALIZE(
        TH095_EXT_BULLET_ANM, &bullet->vm,
        TH095_EXTENDED_SCRIPT_BASE(bullet->bulletType) + 0x10 + bullet->color);
}
#define TH095_EXT_REINITIALIZE_DIRECT(bullet) \
    ReinitializeExtendedBulletDirect(bullet)
#define TH095_EXT_REINITIALIZE_SHIFTED(bullet) \
    ReinitializeExtendedBulletShifted(bullet)
#endif

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

#define TH095_ECL_EXT_RNG ::th095::g_Rng
#ifdef TH095_MATCH_EXACT
extern f32 g_AnmGameSpeed;
extern u32 g_PhotoScreenFadeColor;
#define TH095_ECL_EXT_GAME_SPEED g_AnmGameSpeed
#define TH095_ECL_EXT_FADE_COLOR TH095_BACKBUFFER_CLEAR_COLOR
#else
#define TH095_ECL_EXT_GAME_SPEED ::th095::g_AnmGameSpeed
#define TH095_ECL_EXT_FADE_COLOR TH095_BACKBUFFER_CLEAR_COLOR
#endif
#define TH095_ECL_EXT_SOUND_PLAYER ::th095::g_SoundPlayer

#ifdef DIFFBUILD
Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);
i32 __fastcall DispatchExtendedValue(
    i32 mode, i32 value0, i32 value1, i32 value2, i32 value3, i32 type);
#endif

// ECL extended callback table entry 0 @ 0x00413380.
void __fastcall SpawnDeathPhotoVms(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    TH095_EXT_CREATE_VM_WORLD(TH095_EXT_BULLET_ANM, 0x123, &enemy->position);
    for (i32 i = 0; i < 32; ++i)
        TH095_EXT_CREATE_VM_WORLD(TH095_EXT_BULLET_ANM, 0x122, &enemy->position);
    TH095_ECL_EXT_SOUND_PLAYER.PlaySoundByIdx((SoundIdx)0x12, 0);
    TH095_ECL_EXT_GAME_SPEED = 0.25f;
}

// ECL extended callback table entry 6 @ 0x00413AA0.
void __fastcall UpdatePlayerProximityAndMarker(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    struct ProximityLocals
    {
        Float3 *playerPosition;
        Float3 *enemyPosition;
        AnmVm *vm;
        f32 distanceSquared;
    } locals;

    locals.enemyPosition = &enemy->position;
    locals.playerPosition =
        &g_RuntimePlayerOwner->playerPosition;
    locals.distanceSquared =
        (locals.playerPosition->y - locals.enemyPosition->y) *
            (locals.playerPosition->y - locals.enemyPosition->y) +
        (locals.playerPosition->x - locals.enemyPosition->x) *
            (locals.playerPosition->x - locals.enemyPosition->x);
    if (locals.distanceSquared < 1024.0f)
        g_RuntimePlayerOwner->movementScale = 0.25f;
    else if (locals.distanceSquared < 4096.0f)
        g_RuntimePlayerOwner->movementScale =
            (locals.distanceSquared - 1024.0f) / 3072.0f * 0.75f + 0.25f;

    locals.vm = TH095_EXT_ANM_GET_VM(
        enemy->anmHandles[0].value);
    if (locals.vm != NULL)
        TH095_EXT_PHOTO_TO_SCREEN(&locals.vm->positionOffset, &enemy->position);
}

// ECL extended callback table entry 9 @ 0x00413DA0.
void __fastcall DispatchContextValues(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
#ifdef DIFFBUILD
    DispatchExtendedValue(
        7,
        enemy->activeEclContext->scriptState.intVariables[0],
        enemy->activeEclContext->scriptState.intVariables[1],
        enemy->activeEclContext->scriptState.intVariables[2],
        enemy->activeEclContext->scriptState.intVariables[3],
        29);
#else
    ScreenEffect::RegisterChain(
        SCREEN_EFFECT_SHAKE_ENVELOPE,
        enemy->activeEclContext->scriptState.intVariables[0],
        enemy->activeEclContext->scriptState.intVariables[1],
        enemy->activeEclContext->scriptState.intVariables[2],
        enemy->activeEclContext->scriptState.intVariables[3],
        29);
#endif
}

// ECL extended callback table entry 11 @ 0x00413F90.
void __fastcall PublishGameSpeed(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    TH095_ECL_EXT_GAME_SPEED = enemy->activeEclContext->scriptState.floatVariables[7];
}

// ECL extended callback table entry 12 @ 0x00413FC0.
void __fastcall SetBackgroundVmsState2(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    AnmVm *secondVm;
    AnmVm *firstVm;
    firstVm = TH095_EXT_ANM_GET_VM(
        g_Background->spellBackgroundVmIds[0].value);
    firstVm->pendingInterrupt = 2;
    secondVm = TH095_EXT_ANM_GET_VM(
        g_Background->spellBackgroundVmIds[1].value);
    secondVm->pendingInterrupt = 2;
}

// ECL extended callback table entry 13 @ 0x00414020.
void __fastcall SetBackgroundVmsState3(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    AnmVm *secondVm;
    AnmVm *firstVm;
    firstVm = TH095_EXT_ANM_GET_VM(
        g_Background->spellBackgroundVmIds[0].value);
    firstVm->pendingInterrupt = 3;
    secondVm = TH095_EXT_ANM_GET_VM(
        g_Background->spellBackgroundVmIds[1].value);
    secondVm->pendingInterrupt = 3;
    TH095_ECL_EXT_GAME_SPEED = 1.0f;
}

// ECL extended callback table entry 15 @ 0x00414230.
void __fastcall SetPhotoFlag200(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    g_RuntimeGlobalStateOwner->photoSoundSuppressed = 1;
}

// ECL extended callback table entry 16 @ 0x00414260.
void __fastcall ClearPhotoFlag200(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    g_RuntimeGlobalStateOwner->photoSoundSuppressed = 0;
}

// ECL extended callback table entry 18 @ 0x00414430.
void __fastcall EnablePhotoTransition(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    AnmVm *secondVm;
    AnmVm *firstVm;
    g_RuntimeGlobalStateOwner->photoTransitionActive = 1;
    firstVm = TH095_EXT_ANM_GET_VM(
        g_Background->spellBackgroundVmIds[0].value);
    firstVm->pendingInterrupt = 2;
    TH095_EXT_ANM_EXECUTE(firstVm);
    secondVm = TH095_EXT_ANM_GET_VM(
        g_Background->spellBackgroundVmIds[1].value);
    secondVm->pendingInterrupt = 2;
    TH095_EXT_ANM_EXECUTE(secondVm);
    TH095_ECL_EXT_SOUND_PLAYER.PlaySoundByIdx((SoundIdx)0x26, 0);
    TH095_ECL_EXT_GAME_SPEED = 1.0f;
}

// ECL extended callback table entry 19 @ 0x004144E0.
void __fastcall DisablePhotoTransition(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    AnmVm *secondVm;
    AnmVm *firstVm;
    g_RuntimeGlobalStateOwner->photoTransitionActive = 0;
    firstVm = TH095_EXT_ANM_GET_VM(
        g_Background->spellBackgroundVmIds[0].value);
    firstVm->pendingInterrupt = 3;
    TH095_EXT_ANM_EXECUTE(firstVm);
    secondVm = TH095_EXT_ANM_GET_VM(
        g_Background->spellBackgroundVmIds[1].value);
    secondVm->pendingInterrupt = 3;
    TH095_EXT_ANM_EXECUTE(secondVm);
    TH095_ECL_EXT_SOUND_PLAYER.PlaySoundByIdx((SoundIdx)0x0f, 0);
}

// ECL extended callback table entry 21 @ 0x00414930.
void __fastcall ResetOwnedBulletMotion(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    ExtendedBulletView *bullet =
        reinterpret_cast<ExtendedBulletView *>(g_PhotoBulletManager->bullets);
    for (i32 i = 0; i < 0x640; ++i, ++bullet)
    {
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
        if (bullet->state == 0)
#else
        if (bullet->state == PHOTO_BULLET_STATE_INACTIVE)
#endif
            continue;
        if (bullet->ownerTag ==
            enemy->activeEclContext->scriptState.extraIntVariables[2])
        {
#if defined(TH095_MATCH_EXACT)
            bullet->field34c = 0;
            bullet->field348 = 0;
#else
            bullet->transformFlags = 0;
            bullet->activeTransformFlags = 0;
#endif
            bullet->speed = 4.5f;
            TH095_EXTENDED_FROM_ANGLE(
                bullet->velocity, bullet->angle, bullet->speed);
        }
    }
}



// ECL extended callback table entry 5 @ 0x00413990.
void __fastcall FadeOwnedCapturedBullets(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
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

    locals.bullet = reinterpret_cast<ExtendedBulletView *>(g_PhotoBulletManager->bullets);
    for (locals.i = 0; locals.i < 0x640; ++locals.i, ++locals.bullet)
    {
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
        if (locals.bullet->state == 0)
#else
        if (locals.bullet->state == PHOTO_BULLET_STATE_INACTIVE)
#endif
            continue;
        if (locals.bullet->ownerTag ==
            enemy->activeEclContext->scriptState.extraIntVariables[2])
        {
#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
            if (((locals.bullet->flags >> 4) & 1U) != 0)
#else
            if (locals.bullet->captureDisabled != 0)
#endif
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
void __fastcall UpdateEnemyMarkerVms(PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    ExtendedVector position;
    AnmVm *firstVm;
    firstVm = TH095_EXT_ANM_GET_VM(
        enemy->anmHandles[0].value);
    if (firstVm != NULL)
    {
        TH095_EXTENDED_FROM_ANGLE(position, enemy->movementAngle, 24.0f);
        firstVm->positionOffset = enemy->position + *reinterpret_cast<Float3 *>(&position);
        TH095_EXT_PHOTO_TO_SCREEN(
            &firstVm->positionOffset, &firstVm->positionOffset);
        firstVm->positionOffset.x -= 128.0f;
        firstVm->positionOffset.y -= 16.0f;
        firstVm->rotation.z = enemy->movementAngle;
        position = *reinterpret_cast<ExtendedVector *>(&firstVm->positionOffset);
        firstVm = TH095_EXT_ANM_GET_VM(
            enemy->anmHandles[1].value);
        firstVm->positionOffset = *reinterpret_cast<Float3 *>(&position);
        firstVm->rotation.z = enemy->movementAngle;
    }
}

// ECL extended callback table entry 8 @ 0x00413CF0.
void __fastcall SpawnEnemyMarkerVm(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    struct MarkerLocals
    {
        AnmVm *vm;
        ExtendedVmHandle handle;
    } locals;

    TH095_EXT_CREATE_VM_WORLD_INTO(
        reinterpret_cast<ExtendedAnmSpawner *>(g_PhotoEnemyManager->enemyAnm), &locals.handle,
        enemy->activeEclContext->scriptState.extraIntVariables[2],
        &enemy->worldPosition);
    locals.vm = TH095_EXT_HANDLE_GET_VM(locals.handle);
    TH095_EXT_HANDLE_SET_SPRITE(locals.handle, enemy->vm.activeSpriteIndex);
    locals.vm->flip = enemy->vm.flip;
    locals.vm->rotation = enemy->vm.rotation;
}

static __forceinline i32 ExtendedCameraIsCharging(
    PhotoCameraState *camera)
{
    return camera->mode == PHOTO_CAMERA_CHARGING;
}

// ECL extended callback table entry 20 @ 0x00414580.
void __fastcall RunPhotoTransition(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
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

    if (enemy->activeEclContext->scriptState.extraIntVariables[2] != 0)
    {
        --enemy->activeEclContext->scriptState.extraIntVariables[2];
        if (enemy->activeEclContext->scriptState.extraIntVariables[2] == 60)
        {
            g_RuntimeGlobalStateOwner->photoTransitionActive = 0;
            locals.firstEndVm = TH095_EXT_ANM_GET_VM(
                g_Background->spellBackgroundVmIds[0].value);
            locals.firstEndVm->pendingInterrupt = 3;
            TH095_EXT_ANM_EXECUTE(locals.firstEndVm);
            locals.secondEndVm = TH095_EXT_ANM_GET_VM(
                g_Background->spellBackgroundVmIds[1].value);
            locals.secondEndVm->pendingInterrupt = 3;
            TH095_EXT_ANM_EXECUTE(locals.secondEndVm);
            TH095_ECL_EXT_SOUND_PLAYER.PlaySoundByIdx((SoundIdx)0x0f, 0);
        }
    }

    if (!g_RuntimeGlobalStateOwner->photoTransitionActive &&
        enemy->activeEclContext->scriptState.extraIntVariables[2] == 0 &&
        ExtendedCameraIsCharging(
            &g_RuntimePlayerOwner->camera) &&
        g_RuntimePlayerOwner->camera.CountPhotoTargets(NULL, NULL) != 0)
    {
        g_RuntimeGlobalStateOwner->photoTransitionActive = 1;
        locals.firstStartVm = TH095_EXT_ANM_GET_VM(
            g_Background->spellBackgroundVmIds[0].value);
        locals.firstStartVm->pendingInterrupt = 2;
        TH095_EXT_ANM_EXECUTE(locals.firstStartVm);
        locals.secondStartVm = TH095_EXT_ANM_GET_VM(
            g_Background->spellBackgroundVmIds[1].value);
        locals.secondStartVm->pendingInterrupt = 2;
        TH095_EXT_ANM_EXECUTE(locals.secondStartVm);
        TH095_ECL_EXT_SOUND_PLAYER.PlaySoundByIdx((SoundIdx)0x26, 0);
        TH095_ECL_EXT_GAME_SPEED = 1.0f;
        enemy->activeEclContext->scriptState.extraIntVariables[2] = 120;

        if (g_RuntimePlayerOwner->camera.viewfinderPosition.x < 0.0f)
            locals.targetX =
                g_RuntimePlayerOwner->camera.viewfinderSize.x *
                    0.60000002f +
                g_RuntimePlayerOwner->camera.viewfinderPosition.x;
        else
            locals.targetX =
                g_RuntimePlayerOwner->camera.viewfinderPosition.x -
                g_RuntimePlayerOwner->camera.viewfinderSize.x *
                    0.60000002f;

        if (g_RuntimePlayerOwner->playerPosition.y <
            enemy->position.y)
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

        enemy->control.movementEasing =
            PHOTO_ENEMY_EASING_OUT_QUADRATIC;
        enemy->control.movementMode =
            PHOTO_ENEMY_MOVEMENT_INTERPOLATED;

        locals.zeroVelocity.x = 0.0f;
        locals.zeroVelocity.y = 0.0f;
        locals.zeroVelocity.z = 0.0f;
        enemy->velocity = locals.zeroVelocity;
    }
}

struct ExtendedEffectCallbackLocals
{
    PhotoRotatingLaserSpawnArgs args;
    PhotoRotatingLaserView *effect;
    i32 spawnId;
    __forceinline void DisableTailDraw()
    {
        // Entries 10/14/17 independently repeat this target tail class.
        // VC7.1 places the callback fastcall homes after this inline phase;
        // 0x28/0x30 controls leave them one dword shallow/deep.
        u8 compilerStorage[0x2c];
        this->effect->tailVm.drawEnabled = 0;
    }
};
typedef char ExtendedEffectCallbackLocalsSize50[(sizeof(ExtendedEffectCallbackLocals) == 0x50) ? 1 : -1];

static __forceinline void FindSpawnedExtendedEffect(ExtendedEffectCallbackLocals *locals)
{
    // The original callbacks search the current manager ID even at capacity,
    // where Spawn returns zero and leaves that ID unchanged.
    PhotoEffectBaseView *cursor;
    cursor = g_PhotoEffectManager->listRoot.next;
    while (cursor != NULL)
    {
        if (cursor->id == g_PhotoEffectManager->nextId)
        {
            locals->effect = static_cast<PhotoRotatingLaserView *>(cursor);
            return;
        }
        cursor = cursor->next;
    }
    locals->effect = NULL;
}

// ECL extended callback table entry 10 @ 0x00413DF0.
void __fastcall Callback10(PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    ExtendedEffectCallbackLocals locals;

    memset(&locals.args, 0, sizeof(locals.args));
    locals.args.speed = 8.0f;
    locals.args.position = enemy->worldPosition + enemy->shootOffset;
    locals.args.type = 0;
    locals.args.color = 0;
    locals.args.angle = enemy->activeEclContext->scriptState.extraFloatVariables[2];
    locals.args.maximumLength = enemy->activeEclContext->scriptState.extraFloatVariables[3];
    locals.args.initialLength = locals.args.maximumLength;
    locals.args.maximumWidth = 16.0f;
    locals.args.startupDuration = 1;
    locals.args.growthDuration = 15;
    locals.args.sustainDuration = 40;
    locals.args.fadeDuration = 6;
    locals.args.angularVelocity = 0.0f;
    locals.args.followPhotoTarget = 0;

    locals.spawnId = g_PhotoEffectManager->Spawn(
        PHOTO_EFFECT_SPAWN_ROTATING_LASER, &locals.args);
    FindSpawnedExtendedEffect(&locals);

    TH095_EXT_ANM_INITIALIZE(
        reinterpret_cast<ExtendedAnmSpawner *>(g_PhotoEnemyManager->enemyAnm), &locals.effect->bodyVm,
        enemy->activeEclContext->scriptState.extraIntVariables[2]);
    locals.DisableTailDraw();
}


// ECL extended callback table entry 14 @ 0x00414090.
void __fastcall Callback14(PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    ExtendedEffectCallbackLocals locals;

    memset(&locals.args, 0, sizeof(locals.args));
    locals.args.speed = 8.0f;
    locals.args.position = enemy->worldPosition + enemy->shootOffset;
    locals.args.type = 0;
    locals.args.color = 0;
    locals.args.angle = enemy->activeEclContext->scriptState.extraFloatVariables[2];
    locals.args.maximumLength = enemy->activeEclContext->scriptState.extraFloatVariables[3];
    locals.args.initialLength = locals.args.maximumLength;
    locals.args.maximumWidth = 16.0f;
    locals.args.startupDuration = 1;
    locals.args.growthDuration = 15;
    locals.args.sustainDuration = 300;
    locals.args.fadeDuration = 6;
    locals.args.angularVelocity = 0.0f;
    locals.args.followPhotoTarget = 0;

    locals.spawnId = g_PhotoEffectManager->Spawn(
        PHOTO_EFFECT_SPAWN_ROTATING_LASER, &locals.args);
    FindSpawnedExtendedEffect(&locals);

    TH095_EXT_ANM_INITIALIZE(
        reinterpret_cast<ExtendedAnmSpawner *>(g_PhotoEnemyManager->enemyAnm), &locals.effect->bodyVm,
        enemy->activeEclContext->scriptState.extraIntVariables[2]);
    locals.DisableTailDraw();
}


// ECL extended callback table entry 17 @ 0x00414290.
void __fastcall Callback17(PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    ExtendedEffectCallbackLocals locals;

    memset(&locals.args, 0, sizeof(locals.args));
    locals.args.speed = 8.0f;
    locals.args.position = enemy->worldPosition + enemy->shootOffset;
    locals.args.type = 0;
    locals.args.color = 0;
    locals.args.angle = enemy->activeEclContext->scriptState.extraFloatVariables[2];
    locals.args.maximumLength = enemy->activeEclContext->scriptState.extraFloatVariables[3];
    locals.args.initialLength = locals.args.maximumLength;
    locals.args.maximumWidth = 16.0f;
    locals.args.startupDuration = 1;
    locals.args.growthDuration = 15;
    locals.args.sustainDuration = 120;
    locals.args.fadeDuration = 6;
    locals.args.angularVelocity = 0.0f;
    locals.args.followPhotoTarget = 0;

    locals.spawnId = g_PhotoEffectManager->Spawn(
        PHOTO_EFFECT_SPAWN_ROTATING_LASER, &locals.args);
    FindSpawnedExtendedEffect(&locals);

    TH095_EXT_ANM_INITIALIZE(
        reinterpret_cast<ExtendedAnmSpawner *>(g_PhotoEnemyManager->enemyAnm), &locals.effect->bodyVm,
        enemy->activeEclContext->scriptState.extraIntVariables[2]);
    locals.DisableTailDraw();
}


static __forceinline void SetExtendedBackgroundVm0State2()
{
    AnmVm *vm;
    vm = TH095_EXT_ANM_GET_VM(
        g_Background->spellBackgroundVmIds[0].value);
    vm->pendingInterrupt = 2;
}

static __forceinline void SetExtendedBackgroundVm1State2()
{
    AnmVm *vm;
    vm = TH095_EXT_ANM_GET_VM(
        g_Background->spellBackgroundVmIds[1].value);
    vm->pendingInterrupt = 2;
}

static __forceinline void SetExtendedBackgroundVm0State3()
{
    AnmVm *vm;
    vm = TH095_EXT_ANM_GET_VM(
        g_Background->spellBackgroundVmIds[0].value);
    vm->pendingInterrupt = 3;
}

static __forceinline void SetExtendedBackgroundVm1State3()
{
    AnmVm *vm;
    vm = TH095_EXT_ANM_GET_VM(
        g_Background->spellBackgroundVmIds[1].value);
    vm->pendingInterrupt = 3;
}

// ECL extended callback table entry 1 @ 0x00413410.
void __fastcall Callback01(PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    ExtendedBulletView *index;
    i32 bullet;

    index = reinterpret_cast<ExtendedBulletView *>(g_PhotoBulletManager->bullets);
    for (bullet = 0; bullet < 0x640; bullet++, index++)
    {
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
        if (index->state == 0 || index->vm.loadedSprite->widthPx < 64.0f)
#else
        if (index->state == PHOTO_BULLET_STATE_INACTIVE ||
            index->vm.loadedSprite->widthPx < 64.0f)
#endif
            continue;

        g_PhotoEnemyManager->Spawn(
            0,
            reinterpret_cast<const Float3 *>(&index->position),
            1, 0, 0, 0);
    }
}

// ECL extended callback table entry 2 @ 0x004134A0.
void __fastcall Callback02(PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    // Exact-facing lexical name; this stores the raw bits of vm.rotation.z.
    u32 savedActiveSprite;
    ExtendedBulletView *index;
    i32 bullet;

    index = reinterpret_cast<ExtendedBulletView *>(g_PhotoBulletManager->bullets);
    for (bullet = 0; bullet < 0x640; bullet++, index++)
    {
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
        if (index->state == 0)
#else
        if (index->state == PHOTO_BULLET_STATE_INACTIVE)
#endif
            continue;
        if (index->ownerTag == enemy->activeEclContext->scriptState.extraIntVariables[2])
        {
            savedActiveSprite = *reinterpret_cast<u32 *>(&index->vm.rotation.z);
            TH095_EXT_REINITIALIZE_SHIFTED(index);
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
            index->vm.flagsWord &= 0xf7ffffffU;
#else
            index->vm.rotateWithBulletAngle = 0;
#endif
            index->vm.pendingInterrupt = 2;
            *reinterpret_cast<u32 *>(&index->vm.rotation.z) = savedActiveSprite;
            TH095_EXTENDED_FROM_ANGLE(
                index->velocity,
                enemy->activeEclContext->scriptState.extraFloatVariables[2],
                enemy->activeEclContext->scriptState.extraFloatVariables[3]);
            index->flags &= ~2U;
#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
            index->flags |= 0x10U;
#else
            index->captureDisabled = 1;
#endif
        }
    }

    SetExtendedBackgroundVm0State2();
    SetExtendedBackgroundVm1State2();
    TH095_ECL_EXT_FADE_COLOR = 0;
}

// ECL extended callback table entry 3 @ 0x00413620.
void __fastcall Callback03(PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    ExtendedBulletView *index;
    i32 bullet;

    index = reinterpret_cast<ExtendedBulletView *>(g_PhotoBulletManager->bullets);
    for (bullet = 0; bullet < 0x640; bullet++, index++)
    {
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
        if (index->state == 0)
#else
        if (index->state == PHOTO_BULLET_STATE_INACTIVE)
#endif
            continue;

        TH095_EXT_REINITIALIZE_DIRECT(index);
        index->vm.pendingInterrupt = 2;
        TH095_EXTENDED_FROM_ANGLE(index->velocity, index->angle, index->speed);
        index->flags |= 2U;
#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
        index->flags &= ~0x10U;
#else
        index->captureDisabled = 0;
#endif
    }

    SetExtendedBackgroundVm0State3();
    SetExtendedBackgroundVm1State3();
}

// ECL extended callback table entry 4 @ 0x00413750.
void __fastcall Callback04(PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    // Exact-facing lexical name; this stores the raw bits of vm.rotation.z.
    u32 savedActiveSprite;
    ExtendedBulletView *index;
    i32 bullet;

    index = reinterpret_cast<ExtendedBulletView *>(g_PhotoBulletManager->bullets);
    for (bullet = 0; bullet < 0x640; bullet++, index++)
    {
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
        if (index->state == 0)
#else
        if (index->state == PHOTO_BULLET_STATE_INACTIVE)
#endif
            continue;
        if (index->ownerTag == enemy->activeEclContext->scriptState.extraIntVariables[2])
        {
            savedActiveSprite = *reinterpret_cast<u32 *>(&index->vm.rotation.z);
            TH095_EXT_REINITIALIZE_SHIFTED(index);
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
            index->vm.flagsWord &= 0xf7ffffffU;
#else
            index->vm.rotateWithBulletAngle = 0;
#endif
            index->vm.pendingInterrupt = 2;
            *reinterpret_cast<u32 *>(&index->vm.rotation.z) = savedActiveSprite;
            TH095_EXTENDED_FROM_ANGLE(
                index->velocity,
                enemy->activeEclContext->scriptState.extraFloatVariables[2] +
                    index->angle,
                enemy->activeEclContext->scriptState.extraFloatVariables[3] > -999.0f
                    ? enemy->activeEclContext->scriptState.extraFloatVariables[3]
                    : index->speed);
            index->flags &= ~2U;
#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
            index->flags |= 0x10U;
#else
            index->captureDisabled = 1;
#endif
            TH095_EXT_ANM_EXECUTE(&index->vm);
            FinalizeExtendedBulletAfterExecute(&index->vm, 0);
        }
    }

    SetExtendedBackgroundVm0State2();
    SetExtendedBackgroundVm1State2();
    TH095_ECL_EXT_FADE_COLOR = 0;
}

} // namespace EclExtended

// Target 0x004A4270: opcode-118 extension entries 0..21.  Every pointer names
// its reconstructed exact callback; no preferred-base address is embedded.
EclExtendedCallback g_EclExtendedCallbacks[22] = {
    EclExtended::SpawnDeathPhotoVms,
    EclExtended::Callback01,
    EclExtended::Callback02,
    EclExtended::Callback03,
    EclExtended::Callback04,
    EclExtended::FadeOwnedCapturedBullets,
    EclExtended::UpdatePlayerProximityAndMarker,
    EclExtended::UpdateEnemyMarkerVms,
    EclExtended::SpawnEnemyMarkerVm,
    EclExtended::DispatchContextValues,
    EclExtended::Callback10,
    EclExtended::PublishGameSpeed,
    EclExtended::SetBackgroundVmsState2,
    EclExtended::SetBackgroundVmsState3,
    EclExtended::Callback14,
    EclExtended::SetPhotoFlag200,
    EclExtended::ClearPhotoFlag200,
    EclExtended::Callback17,
    EclExtended::EnablePhotoTransition,
    EclExtended::DisablePhotoTransition,
    EclExtended::RunPhotoTransition,
    EclExtended::ResetOwnedBulletMotion};
} // namespace th095
