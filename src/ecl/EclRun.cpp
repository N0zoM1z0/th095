#include "EclManager.hpp"
#include "EclOperands.hpp"
#include "Gui.hpp"
#include "BulletManager.hpp"
#include "Background.hpp"
#include "GameManager.hpp"
#include "ItemManager.hpp"
#include "Player.hpp"
#include "Spellcard.hpp"

#include <string.h>
#include <stdlib.h>
#include <math.h>

#define TH08_ECL_RUN_DECLARATIONS_ONLY
#include "EclRunLow.inl"
#include "EclRunHigh.inl"
#undef TH08_ECL_RUN_DECLARATIONS_ONLY

#include "../GameplayGlobals.hpp"
#ifndef DIFFBUILD
#include "../PhotoEffectRuntime.hpp"
#include "../PhotoPlayerRuntime.hpp"
#endif

#ifndef DIFFBUILD
#define g_Th095Player \
    TH095_RUNTIME_GLOBAL_PTR(Player, ::th095::g_RuntimePlayerOwner)
#define g_Th095PhotoCamera \
    TH095_RUNTIME_GLOBAL_PTR(PhotoCamera, ::th095::g_RuntimePlayerOwner)
#define g_Th095GameManager \
    TH095_RUNTIME_GLOBAL_PTR(u8, ::th095::g_RuntimeGlobalStateOwner)
#endif

#ifdef DIFFBUILD
#define TH095_ECL_PLAYER_ANGLE(point) g_Th095Player->AngleToPoint(point)
#define TH095_ECL_PHOTO_ANGLE(point) g_Th095PhotoCamera->GetAngle(point)
#else
#define TH095_ECL_PLAYER_ANGLE(point) \
    TH095_RUNTIME_GLOBAL_PTR(::th095::PhotoPlayerRuntimeView, ::th095::g_RuntimePlayerOwner) \
        ->AngleFromPoint(point)
#define TH095_ECL_PHOTO_ANGLE(point) \
    TH095_RUNTIME_GLOBAL_PTR(::th095::PhotoPlayerRuntimeView, ::th095::g_RuntimePlayerOwner) \
        ->AngleFromPoint(point)
#endif

#ifdef DIFFBUILD
#define TH095_ECL_RUNTIME EclRunHigh::g_Th095Runtime
#define TH095_ECL_ENEMY_SPAWN(subroutineId, position, life, itemDrop, score, contextValues) \
    reinterpret_cast<EclRunHigh::Th095RuntimeManager *>(TH095_ECL_RUNTIME)->SpawnEnemy( \
        (subroutineId), (position), (life), (itemDrop), (score), (contextValues))
#define TH095_ECL_ENEMY_RESET() \
    reinterpret_cast<EclRunHigh::Th095RuntimeManager *>(TH095_ECL_RUNTIME)->ResetEnemies()
#else
#define TH095_ECL_RUNTIME \
    TH095_RUNTIME_GLOBAL_PTR(u8, ::th095::g_RuntimeEnemyManagerOwner)
#define TH095_ECL_ENEMY_SPAWN(subroutineId, position, life, itemDrop, score, contextValues) \
    ::th095::Th095EclSpawnEnemy( \
        (subroutineId), (position), (life), (itemDrop), (score), (contextValues))
#define TH095_ECL_ENEMY_RESET() ::th095::Th095EclResetEnemies()
#endif

#ifdef DIFFBUILD
#define TH095_ECL_STAGE_STATE EclRunHigh::g_Th095StageState
#else
#define TH095_ECL_STAGE_STATE \
    TH095_RUNTIME_GLOBAL_PTR(u8, ::th095::g_RuntimeStageStateOwner)
#endif

#ifdef DIFFBUILD
#define TH095_ECL_PHOTO_MODE EclRunHigh::g_Th095PhotoMode
#define TH095_ECL_PHOTO_MODE_BEGIN() TH095_ECL_PHOTO_MODE->Begin()
#define TH095_ECL_PHOTO_MODE_END() TH095_ECL_PHOTO_MODE->End()
#define TH095_ECL_SESSION_REPLACE(session) (session)->ReplaceActive()
#define TH095_ECL_SESSION_FINISH(session) (session)->Finish()
#define TH095_ECL_SESSION_CREATE(descriptor) (descriptor)->Create()
#else
#define TH095_ECL_PHOTO_MODE \
    (reinterpret_cast<EclRunHigh::PhotoModeController *>(::th095::g_Background))
#define TH095_ECL_PHOTO_MODE_BEGIN() ::th095::g_Background->StartSpellBackground()
#define TH095_ECL_PHOTO_MODE_END() ::th095::g_Background->StopSpellBackground()
#define TH095_ECL_SESSION_REPLACE(session) \
    reinterpret_cast<::th095::PhotoCardInfoView *>(session)->Destroy()
#define TH095_ECL_SESSION_FINISH(session) \
    reinterpret_cast<::th095::PhotoCardInfoView *>(session)->Show()
#define TH095_ECL_SESSION_CREATE(descriptor) \
    reinterpret_cast<EclRunHigh::PhotoSession *>( \
        ::th095::PhotoCardInfoView::Create(reinterpret_cast<char *>(descriptor)))
#endif

#ifdef DIFFBUILD
#define TH095_ECL_BULLET_MANAGER EclRunHigh::g_Th095BulletManager
#define TH095_ECL_BULLET_SPAWN(descriptor) \
    TH095_ECL_BULLET_MANAGER->SpawnEnemyPattern(descriptor)
#else
#define TH095_ECL_BULLET_MANAGER \
    TH095_RUNTIME_GLOBAL_PTR(EclRunHigh::Th095BulletManager, ::th095::g_RuntimeBulletManagerOwner)
#define TH095_ECL_BULLET_SPAWN(descriptor) \
    reinterpret_cast<::th095::PhotoBulletManagerView *>(TH095_ECL_BULLET_MANAGER) \
        ->SpawnBulletPattern( \
            reinterpret_cast<::th095::PhotoBulletSpawnDescriptor *>(descriptor))
#endif

#ifdef DIFFBUILD
#define TH095_ECL_ANM_MANAGER EclRunHigh::g_Th095AnmManager
#else
#define TH095_ECL_ANM_MANAGER \
    (reinterpret_cast<EclRunHigh::AnmManagerLookup *>(::th095::g_AnmManager))
#endif

#ifdef DIFFBUILD
#define TH095_ECL_ANM_GET_VM(id) TH095_ECL_ANM_MANAGER->FindVm(id)
#define TH095_ECL_ANM_MARK_DELETE(id) TH095_ECL_ANM_MANAGER->RemoveVm(id)
#define TH095_ECL_ANM_SPAWN_WORLD(spawner, script, position) \
    (spawner)->Spawn((script), (position))
#else
#define TH095_ECL_ANM_GET_VM(id) \
    ::th095::g_AnmManager->GetVm(::th095::Th095EclAnmId(id))
#define TH095_ECL_ANM_MARK_DELETE(id) \
    ::th095::g_AnmManager->MarkVmForDeletion(::th095::Th095EclAnmId(id))
#define TH095_ECL_ANM_SPAWN_WORLD(spawner, script, position) \
    ::th095::Th095EclSpawnWorld((spawner), (script), (position))
#endif

#ifdef DIFFBUILD
#define TH095_ECL_RESOLVE_FLOAT(enemy, operand) \
    reinterpret_cast<EclRunHigh::EnemyFloatOperandView *>(enemy)->ResolveFloat(operand)
#define TH095_ECL_CLAMP_POSITION(enemy) (enemy)->ClampPosition()
#define TH095_ECL_CONFIGURE_PHOTO_ANM(vm, work, value) \
    TH095_ECL_ANM_MANAGER->ConfigureEnemyPhotoAnm((vm), (work), (value))
#else
#define TH095_ECL_RESOLVE_FLOAT(enemy, operand) \
    (enemy)->ResolveFloat((operand).asFloat)
#define TH095_ECL_CLAMP_POSITION(enemy) \
    reinterpret_cast<::th095::PhotoEnemyView *>(enemy)->ClampPosition()
#define TH095_ECL_CONFIGURE_PHOTO_ANM(vm, work, value) \
    ::th095::g_AnmManager->InitializeHorizontalTextureStrip( \
        (vm), reinterpret_cast<::th095::AnmVertex *>(work), (value))
#endif

#ifdef DIFFBUILD
#define TH095_ECL_EFFECT_MANAGER EclRunHigh::g_Th095PhotoEffectManager
#define TH095_ECL_STAGE_CONTROLLER EclRunHigh::g_Th095StageController
#define TH095_ECL_BULLET_RESET() TH095_ECL_BULLET_MANAGER->ResetEnemyPatterns()
#define TH095_ECL_STAGE_RESET() TH095_ECL_STAGE_CONTROLLER->ResetEnemyState()
#else
#define TH095_ECL_EFFECT_MANAGER \
    TH095_RUNTIME_GLOBAL_PTR(::th095::PhotoEffectManagerView, ::th095::g_RuntimeEffectManagerOwner)
#define TH095_ECL_STAGE_CONTROLLER \
    TH095_RUNTIME_GLOBAL_PTR(EclRunHigh::Th095StageController, ::th095::g_RuntimeEffectManagerOwner)
#define TH095_ECL_BULLET_RESET() \
    reinterpret_cast<::th095::PhotoBulletManagerView *>(TH095_ECL_BULLET_MANAGER) \
        ->DespawnAllBullets()
#define TH095_ECL_STAGE_RESET() \
    ::th095::PhotoEffectManagerView::DrawSecondary(TH095_ECL_EFFECT_MANAGER)
#endif

// RunEcl and CallEclSub return the target's global ::ZunResult enum in both
// production and exact builds. The numeric values match th095::ZunResult, but
// the enum identity is part of the VC7.1 decorated symbol.
#define ZUN_SUCCESS TH095_LEGACY_ZUN_SUCCESS
#define ZUN_ERROR TH095_LEGACY_ZUN_ERROR

namespace th095
{

#ifndef DIFFBUILD
struct AnmVertex;
struct PhotoBulletSpawnDescriptor;
struct PhotoBulletManagerView
{
    i32 SpawnBulletPattern(PhotoBulletSpawnDescriptor *descriptor);
    void DespawnAllBullets();
};
struct PhotoCardInfoView
{
    static PhotoCardInfoView *__fastcall Create(char *text);
    i32 Show();
    void Destroy();
};
struct PhotoEnemyView
{
    void ClampPosition();
};
struct PhotoEnemyManagerView
{
    PhotoEnemyView *SpawnWithContext(
        i32 subroutineId, const Float3 *position, i32 life,
        i32 itemDrop, i32 score, const i32 *contextValues);
    static void __fastcall ResetNonPhotoTargets(
        PhotoEnemyManagerView *enemyManager);
};
extern AnmManager *g_AnmManager;
static __forceinline Enemy *Th095EclSpawnEnemy(
    i32 subroutineId, Float3 *position, i32 life,
    i32 itemDrop, i32 score, i32 *contextValues)
{
    return reinterpret_cast<Enemy *>(
        reinterpret_cast<PhotoEnemyManagerView *>(TH095_ECL_RUNTIME)
            ->SpawnWithContext(
                subroutineId, position, life, itemDrop, score, contextValues));
}
static __forceinline void Th095EclResetEnemies()
{
    PhotoEnemyManagerView *manager =
        reinterpret_cast<PhotoEnemyManagerView *>(TH095_ECL_RUNTIME);
    PhotoEnemyManagerView::ResetNonPhotoTargets(manager);
}
static __forceinline AnmVmId Th095EclAnmId(i32 value)
{
    AnmVmId id;
    id.value = value;
    return id;
}
static __forceinline EclRunHigh::PhotoAnmHandle Th095EclSpawnWorld(
    EclRunHigh::PhotoAnmSpawner *spawner, i32 script, Float3 *position)
{
    AnmVmId id = reinterpret_cast<AnmLoaded *>(spawner)
        ->CreateVmAtWorld(script, position);
    EclRunHigh::PhotoAnmHandle result;
    result.value = id.value;
    return result;
}
#endif

// The low/high opcode bodies are included lexically below so VC7 can reproduce
// RunEcl's target handler order, shared labels, locals, and stack frame.
#undef TH08_ECL_CONTEXT_ENEMY
#undef TH08_ECL_CONTEXT_INSTRUCTION
#undef TH08_ECL_CONTEXT_CHILD
#define TH08_ECL_CONTEXT_ENEMY(unusedContext) (enemy)
#define TH08_ECL_CONTEXT_INSTRUCTION(unusedContext) (instruction)
#define TH08_ECL_CONTEXT_CHILD(unusedContext) (activeChildContext)

#ifdef TH095_MATCH_EXACT
static __forceinline void InitializeEclTargetTimerExact(ZunTimer *timer)
{
    timer->current = 0;
    timer->subFrame = 0.0f;
    timer->previous = -999999;
}
#endif

// FUNCTION: TH095 0x00408E70; TH08 0x004184B0 is the adjacent source oracle.
EclRunResult EclManager::RunEcl(Enemy *enemy)
{
    using namespace EclRunLow;
    using namespace EclRunHigh;

    EclRawInstruction *instruction;
    i32 activeChildContext = -1;
    i32 lhsInt;

    enemy->activeEclCallStack = enemy->mainEclCallStackStorage;
    enemy->activeEclContext = &enemy->mainEclContextStorage;
    enemy->activeEclCallStackDepth = enemy->mainEclCallStackDepth;

restart_context:
    instruction = enemy->activeEclContext->currentInstr;

    for (;;)
    {
        if (enemy->pendingEclSubroutineIndex >= 0)
            goto enter_subroutine;

low_redispatch_instruction:
        *reinterpret_cast<D3DXVECTOR3 *>(&enemy->worldPosition) =
            *reinterpret_cast<D3DXVECTOR3 *>(&enemy->position) +
            *reinterpret_cast<D3DXVECTOR3 *>(&enemy->positionOffset);

        if ((int)enemy->activeEclContext->secondaryTime > 0)
        {
            DecrementTimer(&enemy->activeEclContext->secondaryTime, 1);
            DecrementTimer(&enemy->activeEclContext->time, 1);
            break;
        }

        if (enemy->activeEclContext->time == instruction->time)
        {
            {
#define context (enemy->activeEclContext)
#define ctx unusedContext
#define TH08_ECL_RUN_LOW_BODY
#define TH08_ECL_RUN_HIGH_BODY
#define TH08_ECL_RUN_SHARED_SWITCH
            switch (instruction->opcode)
            {
                {
#include "EclRunLow.inl"
                }
                {
#include "EclRunTargetHigh.inl"
                }
            default:
                break;
            }
#undef TH08_ECL_RUN_SHARED_SWITCH
#undef TH08_ECL_RUN_HIGH_BODY
#undef TH08_ECL_RUN_LOW_BODY
#undef ctx
#undef context
            }

low_advance_instruction:
            instruction = reinterpret_cast<EclRawInstruction *>(
                reinterpret_cast<u8 *>(instruction) + instruction->nextOffset);
            goto low_redispatch_instruction;

            break;
        }

        break;
    }

    // Target 0x0041E7F8..0x0041ECBD is part of RunEcl itself.  Keep the
    // complete frame tail lexical so VC7 can share RunEcl's locals and emit
    // the observed in-function easing switch and child-context back edge.
    if (TH095_ENEMY_LIFE(enemy) > 0)
    {
        i32 i;
        f32 progress;
        {
        i32 restorePosition = 0;
        EnemyEclInterpolationSlot *entry =
            enemy->activeEclContext->interpolationSlots;
        {
        Float3 savedPosition = enemy->position;

        if (enemy->activeEclContext->perFrameCallback)
            enemy->activeEclContext->perFrameCallback(
                enemy, enemy->activeEclContext->perFrameInstruction);

        for (i = 0; i < 8; ++i, ++entry)
        {
            if (entry->callback)
            {
                entry->timer++;
                if (entry->timer >= entry->duration)
                    SetTimerValue(&entry->timer, entry->duration);

                progress = static_cast<f32>(entry->timer) / entry->duration;
                switch (entry->easing)
                {
                case 1: progress = progress * progress; break;
                case 2: progress = progress * progress * progress; break;
                case 3: progress = progress * progress * progress * progress; break;
                case 4:
                    progress = 1.0f - progress;
                    progress = progress * progress;
                    progress = 1.0f - progress;
                    break;
                case 5:
                    progress = 1.0f - progress;
                    progress = progress * progress * progress;
                    progress = 1.0f - progress;
                    break;
                case 6:
                    progress = 1.0f - progress;
                    progress = progress * progress * progress * progress;
                    progress = 1.0f - progress;
                    break;
                }

                entry->callback(enemy, entry, progress);
                if (entry->timer >= entry->duration)
                    entry->callback = 0;

                if (entry->affectedVariable == 10042.0f ||
                    entry->affectedVariable == 10043.0f ||
                    entry->affectedVariable == 10044.0f)
                    restorePosition = 1;
            }
        }

        if (restorePosition)
        {
            enemy->velocity.x = enemy->position.x - savedPosition.x;
            enemy->velocity.y = enemy->position.y - savedPosition.y;
            enemy->movementAngle = atan2f(enemy->velocity.y, enemy->velocity.x);
            enemy->position = savedPosition;
        }
        }
        }
    }

    if (activeChildContext == -1)
        enemy->mainEclCallStackDepth = enemy->activeEclCallStackDepth;
    else
        TH095_ENEMY_CHILD_BLOCK_VIEW(enemy)->childEclBlocks[activeChildContext]->callStackDepth =
            enemy->activeEclCallStackDepth;

    enemy->activeEclContext->currentInstr = instruction;
    enemy->activeEclContext->time.operator++(0);

low_select_next_context:
    for (i32 next = activeChildContext + 1; next < 16; ++next)
    {
        if (TH095_ENEMY_CHILD_BLOCK_VIEW(enemy)->childEclBlocks[next])
        {
            EnemyChildEclBlock *childContext =
                TH095_ENEMY_CHILD_BLOCK_VIEW(enemy)->childEclBlocks[next];
            enemy->activeEclCallStack = childContext->callStack;
            enemy->activeEclContext = &childContext->eclContext;
            instruction = enemy->activeEclContext->currentInstr;
            enemy->activeEclContext->childContextSlot = next + 1;
            enemy->activeEclCallStackDepth = childContext->callStackDepth;
            activeChildContext = next;
            goto low_redispatch_instruction;
        }
    }

    enemy->activeEclCallStack = enemy->mainEclCallStackStorage;
    enemy->activeEclContext = &enemy->mainEclContextStorage;
    enemy->UpdateMovement();
    enemy->UpdateShotAndAnm();

    return ZUN_SUCCESS;
}

#undef TH08_ECL_CONTEXT_ENEMY
#undef TH08_ECL_CONTEXT_INSTRUCTION
#undef TH08_ECL_CONTEXT_CHILD

} // namespace th095
