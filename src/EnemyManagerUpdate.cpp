#include "AnmManager.hpp"
#include "AnmVmId.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoEnemyEclManager.hpp"
#include "PhotoGamePlayer.hpp"
#include "PhotoEnemy.hpp"
#include "PhotoEnemyManager.hpp"
#include "PhotoBulletManager.hpp"
#include "SceneData.hpp"
#include <stdlib.h>
#include <string.h>

namespace th095
{

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

struct PhotoEnemyView;
struct PhotoEnemyManagerView;

EnemyEclContext::EnemyEclContext()
{
}

i32 PhotoEnemyEclManagerView::Load(char *path)
{
    i32 index;

    this->eclFile = reinterpret_cast<PhotoEnemyEclFileView *>(
        FileSystem::OpenFile(path, NULL, FALSE));
    if (this->eclFile == NULL)
    {
        g_GameErrorContext.Log(
            "\x93\x47\x83\x66\x81\x5b\x83\x5e\x82\xcc"
            "\x93\xc7\x82\xdd\x8d\x9e\x82\xdd\x82\xc9"
            "\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5"
            "\x82\xbd\x81\x41\x83\x66\x81\x5b\x83\x5e"
            "\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xe9"
            "\x82\xa9\x8e\xb8\x82\xed\x82\xea\x82\xc4"
            "\x82\xa2\x82\xdc\x82\xb7\r\n");
        return ZUN_ERROR;
    }

    if (this->eclFile->version != 0x800)
    {
        g_GameErrorContext.Log(
            "\x93\x47\x83\x66\x81\x5b\x83\x5e\x82\xcc"
            "\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93"
            "\x82\xaa\x88\xe1\x82\xa2\x82\xdc\x82\xb7"
            "\r\n");
        return ZUN_ERROR;
    }

    for (index = 0; index < this->eclFile->timelineCount; ++index)
    {
        this->eclFile->timelineOffsets[index] +=
            reinterpret_cast<u32>(this->eclFile);
    }

    this->subroutineTable = this->eclFile->subroutineOffsets;
    for (index = 0; index < this->eclFile->subroutineCount; ++index)
    {
        this->subroutineTable[index] +=
            reinterpret_cast<u32>(this->eclFile);
    }
    return ZUN_SUCCESS;
}

i32 PhotoEnemyEclManagerView::InitializeContext(
    EnemyEclContext *context, i16 subroutineId)
{
    if (subroutineId < 0)
    {
        return ZUN_SUCCESS;
    }

    context->currentInstr = reinterpret_cast<EclRawInstruction *>(
        this->subroutineTable[subroutineId]);
    context->time = 0;
    context->secondaryTime = 0;
    context->subId = subroutineId;
    return ZUN_SUCCESS;
}

#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
inline PhotoEnemyTimelineView::PhotoEnemyTimelineView()
{
    // VC7.1 retains one 0x28 allocation phase for the generated
    // sixteen-timeline construction loop in PhotoEnemyManagerView.
    // 0x24 and 0x2C controls move the same deep compiler-home family
    // one dword shallow/deep, so keep the phase on this real constructor
    // frontend rather than reserving storage in the manager body.
    u8 compilerStorage[0x28];
}
#endif

struct PhotoEnemyTimelineInstruction
{
    i32 time;
    i16 opcode;
    u8 size;
    u8 unknown07;
};

struct PhotoEnemyTimelineSpawnArgs
{
    i32 subroutineId;
    f32 x;
    f32 y;
    i32 life;
    i32 itemDrop;
    i32 score;
};

struct PhotoEnemyTimelineRandomRangeArgs
{
    i32 subroutineId;
    f32 minimumX;
    f32 maximumX;
    f32 y;
    i32 life;
    i32 itemDrop;
    i32 score;
};

struct PhotoEnemyTimelineRandomWidthArgs
{
    i32 subroutineId;
    f32 y;
    i32 life;
    i32 itemDrop;
    i32 score;
};

struct PhotoEnemyTimelineExtendedSpawnArgs
{
    i32 subroutineId;
    f32 x;
    f32 y;
    i32 life;
    i32 timelineParam0;
    i32 timelineParam1;
    i32 score;
};

PhotoEnemyManagerView *g_PhotoEnemyManager; // Retail 0x004BDDC0
extern f32 g_AnmGameSpeed;

#ifdef TH095_MATCH_EXACT
extern f32 g_GameSpeed;
#define TH095_PHOTO_ENEMY_GAME_SPEED g_GameSpeed
#else
#define TH095_PHOTO_ENEMY_GAME_SPEED g_AnmGameSpeed
#endif

PhotoEnemyView::PhotoEnemyView()
{
    // The target's VC7.1 constructor frame retains two unconsumed local slots.
    i32 unconsumedConstructorLocals[2];
}

#if !defined(TH095_MATCH_EXACT) && !defined(DIFFBUILD)
PhotoEnemyTimelineView::PhotoEnemyTimelineView()
{
}
#endif

#pragma var_order(i, enemy, this)
PhotoEnemyManagerView::PhotoEnemyManagerView()
{
    i32 i;
    PhotoEnemyView *enemy;

    utils::DebugPrint("initialize EnemyCtrlInf\n");
    memset(this, 0, sizeof(*this));
    g_PhotoEnemyManager = this;

    for (i = 0; (u32)i < 4; ++i)
    {
        this->timelineEventSlots[i] = -1;
    }

    enemy = &this->spawnTemplate;
    memset(enemy, 0, sizeof(*enemy));
    for (i = 0; i < 96; ++i)
    {
        enemy->trailSamples[i].position.x = -999.0f;
    }

    enemy->flags1 |= 0x00000001;
    enemy->eclTimer = 0;
    enemy->flags1 &= ~0x00400000;
    enemy->collisionSize = Float3(24.0f, 24.0f, 24.0f);
    *reinterpret_cast<Float3 *>(&enemy->velocity) =
        Float3(0.0f, 0.0f, 0.0f);
    enemy->angularVelocity = 0.0f;
    enemy->movementAngle = 0.0f;
    enemy->acceleration = 0.0f;
    enemy->speed = 0.0f;
    enemy->flags1 &= ~0x00000c00;
    enemy->flags1 &= ~0x00008000;
    enemy->flags1 &= ~0x00010000;
    enemy->flags1 &= ~0x00000002;
    enemy->activeEclCallStackDepth = 0;
    enemy->life = 1;
    enemy->score = 100;
    enemy->shootIntervalFrames = 0;
    enemy->shootIntervalTimer = 0;
    enemy->shootOffset = Float3(0.0f, 0.0f, 0.0f);
    enemy->anmScripts.moveLeft = -1;
    enemy->anmScripts.moveRight = -1;
    enemy->anmScripts.idleInitial = -1;
    enemy->flags1 &= ~0x00000004;
    enemy->flags1 |= 0x00000008;
    enemy->flags1 &= ~0x00000010;
    enemy->flags1 |= 0x00000040;
    enemy->flags1 &= ~0x00000080;
    enemy->flags1 |= 0x04000000;
    enemy->flags1 &= ~0x001c0000;
    enemy->photoCaptureEclSubroutineId = -1;
    enemy->flags1 &= ~PHOTO_ENEMY_CLAMP_TO_MOVEMENT_BOUNDS_MASK;
    enemy->pendingEclSubroutineIndex = -1;
    for (i = 0; i < PHOTO_ENEMY_SCHEDULE_SLOT_COUNT; ++i)
    {
        enemy->eclSchedule.photoIndices[i] = PHOTO_ENEMY_SCHEDULE_INACTIVE;
    }
    enemy->pendingCallbackFrame = -1;
    reinterpret_cast<u8 *>(enemy)[0x2be6] = 0;
    enemy->flags1 &= ~0x00800000;
    enemy->bulletSpawnDescriptor.spawnSound = 7;
    enemy->bulletSpawnDescriptor.transformSound = 24;
    enemy->minimumPlayerDistanceSquared = 0.0f;
    enemy->stateTimer = 0;
}

i32 PhotoEnemyManagerView::LoadResources()
{
    this->enemyAnm =
        g_AnmManager->PreloadAnm(8, g_SelectedScene->enemyAnmPath);
    if (this->enemyAnm == NULL)
    {
        g_GameErrorContext.Log(
            "\x93\x47\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc"
            "\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4"
            "\x82\xa2\x82\xdc\x82\xb7\r\n");
        return ZUN_ERROR;
    }

    this->eclManager = new PhotoEnemyEclManagerView;
    if (this->eclManager->Load(
            g_SelectedScene->enemyEclPath) != ZUN_SUCCESS)
    {
        g_GameErrorContext.Log(
            "\x93\x47\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc"
            "\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4"
            "\x82\xa2\x82\xdc\x82\xb7\r\n");
        return ZUN_ERROR;
    }
    return ZUN_SUCCESS;
}

#if defined(TH095_MATCH_EXACT)
#define FreePhotoEnemyChildEclBlock FreePhotoEnemyEclArgument
#endif
static __forceinline void FreePhotoEnemyChildEclBlock(
    PhotoEnemyView *enemy, i32 argumentIndex)
{
    void *argument = enemy->childEclBlocks[argumentIndex];
    free(argument);
}

PhotoEnemyManagerView::~PhotoEnemyManagerView()
{
    utils::DebugPrint("shutdown EnemyCtrlInf\n");
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);

    PhotoEnemyView *enemy = &this->enemyPool[0];
    for (i32 enemyIndex = 0; enemyIndex < 128; ++enemyIndex, ++enemy)
    {
        for (i32 argumentIndex = 0; argumentIndex < 16; ++argumentIndex)
        {
            if (enemy->childEclBlocks[argumentIndex] != NULL)
            {
                FreePhotoEnemyChildEclBlock(enemy, argumentIndex);
            }
        }
    }

    if (this->eclManager != NULL)
    {
        delete this->eclManager;
        this->eclManager = NULL;
    }

    if (g_Supervisor.flags.resultRestartActive != 0)
    {
        g_AnmManager->MarkVmsForDeletion(this->enemyAnm);
    }
    else
    {
        g_AnmManager->ReleaseAnm(8);
    }

    g_PhotoEnemyManager = NULL;
}

void PhotoEnemyManagerView::Destroy()
{
    PhotoEnemyManagerView *enemyManager = this;
    if (enemyManager != NULL)
    {
        delete enemyManager;
        enemyManager = NULL;
    }
}

// FUNCTION: TH095 0x004163F0.
// TH08 supplies the ancestral raw-argument source shape.  The target-local
// 20-byte special-spawn record and GetRandomF32InRange call are compiler-
// observed: together they restore the original VC7.1 local chronology.
void PhotoEnemyTimelineView::Run()
{
    u32 mirrorMovementX = 0;

    while (static_cast<PhotoEnemyTimelineInstruction *>(this->instruction)
            ->time >= 0)
    {
        if (this->timer ==
            static_cast<PhotoEnemyTimelineInstruction *>(this->instruction)
                ->time)
        {
            mirrorMovementX = 0;
            switch (static_cast<PhotoEnemyTimelineInstruction *>(
                        this->instruction)->opcode)
            {
            case 1:
                mirrorMovementX = 1;
            case 0:
            {
                i32 *args = reinterpret_cast<i32 *>(
                    reinterpret_cast<u8 *>(this->instruction) + 8);
                Float3 position;
                position.x = *reinterpret_cast<f32 *>(&args[1]);
                position.y = *reinterpret_cast<f32 *>(&args[2]);
                position.z = 0.0f;
                g_PhotoEnemyManager->Spawn(
                    args[0], &position, args[3], args[4], args[5],
                    mirrorMovementX);
                break;
            }

            case 15:
            {
                i32 *args = reinterpret_cast<i32 *>(
                    reinterpret_cast<u8 *>(this->instruction) + 8);
                Float3 position;
                position.x = *reinterpret_cast<f32 *>(&args[1]);
                position.y = *reinterpret_cast<f32 *>(&args[2]);
                position.z = 0.0f;
                g_PhotoEnemyManager->Spawn(
                    args[0], &position, args[3], args[4], args[5],
                    mirrorMovementX);
                break;
            }

            case 12:
                mirrorMovementX = 1;
            case 11:
            {
                // One fully-live record preserves the target's contiguous
                // position/spawned/args homes without padding.
                struct SpecialSpawnLocals
                {
                    Float3 position;
                    PhotoEnemyView *enemy;
                    i32 *args;
                } locals;
                locals.args = reinterpret_cast<i32 *>(
                    reinterpret_cast<u8 *>(this->instruction) + 8);
                locals.position.x = *reinterpret_cast<f32 *>(&locals.args[1]);
                locals.position.y = *reinterpret_cast<f32 *>(&locals.args[2]);
                locals.position.z = 0.0f;
                locals.enemy = g_PhotoEnemyManager->Spawn(
                    locals.args[0], &locals.position, locals.args[3], -1,
                    locals.args[6], mirrorMovementX);
                locals.enemy->timelineParam0 = locals.args[4];
                locals.enemy->timelineParam1 = locals.args[5];
                break;
            }

            case 4:
                mirrorMovementX = 1;
            case 2:
            {
                i32 *args = reinterpret_cast<i32 *>(
                    reinterpret_cast<u8 *>(this->instruction) + 8);
                Float3 position;
                position.x = g_Rng.GetRandomF32InRange(
                        *reinterpret_cast<f32 *>(&args[2]) -
                        *reinterpret_cast<f32 *>(&args[1])) +
                    *reinterpret_cast<f32 *>(&args[1]);
                position.y = *reinterpret_cast<f32 *>(&args[3]);
                position.z = 0.0f;
                g_PhotoEnemyManager->Spawn(
                    args[0], &position, args[4], args[5], args[6],
                    mirrorMovementX);
                break;
            }

            case 5:
                mirrorMovementX = 1;
            case 3:
            {
                i32 *args = reinterpret_cast<i32 *>(
                    reinterpret_cast<u8 *>(this->instruction) + 8);
                Float3 position;
                position.x = g_Rng.GetRandomF32() * 384.0f;
                position.y = *reinterpret_cast<f32 *>(&args[1]);
                position.z = 0.0f;
                g_PhotoEnemyManager->Spawn(
                    args[0], &position, args[2], args[3], args[4],
                    mirrorMovementX);
                break;
            }

            case 8:
                g_PhotoEnemyManager->timelineEnemySlots[
                    reinterpret_cast<i32 *>(
                        reinterpret_cast<u8 *>(this->instruction) + 8)[0]]
                    ->pendingEclSubroutineIndex = static_cast<i16>(
                        reinterpret_cast<i32 *>(
                            reinterpret_cast<u8 *>(this->instruction) + 8)[1]);
                break;

            case 10:
                if (g_PhotoEnemyManager->timelineEnemySlots[
                        reinterpret_cast<i32 *>(
                            reinterpret_cast<u8 *>(this->instruction) + 8)[0]] !=
                        NULL &&
                    g_PhotoEnemyManager->timelineEnemySlots[
                        reinterpret_cast<i32 *>(
                            reinterpret_cast<u8 *>(this->instruction) + 8)[0]]
                            ->control.active != 0)
                {
                    this->timer.Decrement(1);
                    goto finish;
                }
                break;
            }
        }
        else if (this->timer <
            static_cast<PhotoEnemyTimelineInstruction *>(this->instruction)
                ->time)
        {
            break;
        }

        this->instruction = reinterpret_cast<u8 *>(this->instruction) +
            static_cast<PhotoEnemyTimelineInstruction *>(this->instruction)
                ->size;
    }

finish:
    this->timer.Tick();
}

PhotoEnemyView *PhotoEnemyManagerView::Spawn(
    i32 subroutineId,
    const Float3 *position,
    i32 life,
    i32 itemDrop,
    i32 score,
    u32 mirrorMovementX)
{
    struct EnemySpawnCopy
    {
        u32 words[sizeof(PhotoEnemyView) / sizeof(u32)];
    };
    i32 enemyIndex;
    PhotoEnemyView *enemy;

    enemy = &this->enemyPool[0];
    for (enemyIndex = 0; enemyIndex < 128; ++enemyIndex, ++enemy)
    {
        if (enemy->control.active != 0)
        {
            continue;
        }

        *reinterpret_cast<EnemySpawnCopy *>(enemy) =
            *reinterpret_cast<const EnemySpawnCopy *>(this);
        enemy->enemyIndex = enemyIndex;
        enemy->control.mirrorMovementX = mirrorMovementX;
        if (life >= 0)
        {
            enemy->life = life;
        }
        *reinterpret_cast<Float3 *>(&enemy->position) = *position;
        this->eclManager->InitializeContext(
            &enemy->mainEclContext,
            static_cast<i16>(subroutineId));
        if (this->eclManager->RunEcl(enemy) ==
            TH095_LEGACY_ZUN_ERROR)
        {
            enemy->Deactivate();
            enemyIndex = 128;
        }
        else
        {
            enemy->displayColor = enemy->vm.color1.color;
            enemy->itemDropType = static_cast<i8>(itemDrop);
            if (score >= 0)
            {
                enemy->score = score;
            }
            enemy->maximumLife = enemy->life;
            enemy->phaseStartingLife = enemy->maximumLife;
        }
        break;
    }
    return enemy;
}

PhotoEnemyView *PhotoEnemyManagerView::SpawnWithContext(
    i32 subroutineId,
    const Float3 *position,
    i32 life,
    i32 itemDrop,
    i32 score,
    const EclScriptState *scriptState)
{
    struct EnemySpawnCopy
    {
        u32 words[sizeof(PhotoEnemyView) / sizeof(u32)];
    };
    i32 enemyIndex;
    PhotoEnemyView *enemy;

    enemy = &this->enemyPool[0];
    for (enemyIndex = 0; enemyIndex < 128; ++enemyIndex, ++enemy)
    {
        if (enemy->control.active != 0)
        {
            continue;
        }

        *reinterpret_cast<EnemySpawnCopy *>(enemy) =
            *reinterpret_cast<const EnemySpawnCopy *>(this);
        enemy->enemyIndex = enemyIndex;
        if (life >= 0)
        {
            enemy->life = life;
        }
        *reinterpret_cast<Float3 *>(&enemy->position) = *position;
        this->eclManager->InitializeContext(
            &enemy->mainEclContext,
            static_cast<i16>(subroutineId));
        enemy->mainEclContext.scriptState = *scriptState;
        if (this->eclManager->RunEcl(enemy) ==
            TH095_LEGACY_ZUN_ERROR)
        {
            enemy->Deactivate();
            enemyIndex = 128;
        }
        else
        {
            enemy->displayColor = enemy->vm.color1.color;
            enemy->itemDropType = static_cast<i8>(itemDrop);
            if (score >= 0)
            {
                enemy->score = score;
            }
            enemy->maximumLife = enemy->life;
            enemy->phaseStartingLife = enemy->maximumLife;
        }
        break;
    }
    return enemy;
}

static __forceinline i32 IsPhotoEnemyOutsidePlayfield(
    Float3 *position, f32 spriteWidth, f32 spriteHeight)
{
    return spriteWidth + position->x <= -192.0f ||
        position->x - spriteWidth >= 192.0f ||
        spriteHeight + position->y <= 0.0f ||
        position->y - spriteHeight >= 448.0f;
}

static __forceinline i32 GetPhotoEnemyAnmVmIdValue(AnmVmId *id)
{
    return id->value;
}

static __forceinline i32 IsPhotoEnemyAnmVmIdNull(AnmVmId *id)
{
    AnmVmId nullId;
    return *id == nullId;
}

static __forceinline void ResetPhotoEnemyAnmVmId(AnmVmId *id)
{
    AnmVmId nullId;
    *id = nullId;
}

// TH08 ancestry keeps movement as one contiguous Clamp/Integrate/Clamp block.
// TH095 has two movement operations, but the target allocation boundary cannot
// be attributed more narrowly: placing the same target-attested 0x20 phase on
// Integrate, Clamp, immediately before Clamp, or the pair is byte-identical.
// The size is strict: 0x1C/0x24 controls miss 60 comparable bytes.
static __forceinline void PhotoEnemyMovementPhase(PhotoEnemyView *enemy)
{
    u8 compilerStorage[0x20];
    enemy->IntegrateMovement();
    enemy->ClampPosition();
}

i32 __fastcall PhotoEnemyManagerView::OnUpdate(
    PhotoEnemyManagerView *enemyManager)
{
    PhotoEnemyView *enemy = &enemyManager->enemyPool[0];

    for (i32 timelineIndex = 0;
         timelineIndex < enemyManager->eclManager->eclFile->timelineCount;
         ++timelineIndex)
    {
        if (enemyManager->timelines[timelineIndex].instruction == NULL)
        {
            enemyManager->timelines[timelineIndex].instruction =
                enemyManager->eclManager->eclFile->GetTimeline(timelineIndex);
        }
        enemyManager->timelines[timelineIndex].Run();
    }

    enemyManager->activeEnemyCount = 0;
    enemyManager->drawGroupHeads[3] = NULL;
    enemyManager->drawGroupHeads[2] = NULL;
    enemyManager->drawGroupHeads[1] = NULL;
    enemyManager->drawGroupHeads[0] = NULL;

    PhotoEnemyView *drawGroupTails[4];
    drawGroupTails[3] = NULL;
    drawGroupTails[2] = drawGroupTails[3];
    drawGroupTails[1] = drawGroupTails[2];
    drawGroupTails[0] = drawGroupTails[1];

    for (i32 enemyIndex = 0;
         enemyIndex < 128;
         ++enemyIndex, ++enemy)
    {
        if (enemy->control.active == 0)
        {
            continue;
        }

        if (enemy->control.lifecycleState != 0)
        {
            if (enemy->control.lifecycleState >= 2)
            {
                enemy->Deactivate();
                continue;
            }
            enemy->control.lifecycleState++;
            goto enqueueEnemy;
        }

        enemy->UpdatePhotoMarkerPulse();
        enemy->UpdateScheduledEclCalls();
        if (enemyManager->eclManager->RunEcl(enemy) ==
            TH095_LEGACY_ZUN_ERROR)
        {
            enemy->Deactivate();
            continue;
        }

        PhotoEnemyMovementPhase(enemy);

        if (GetPhotoEnemyAnmVmIdValue(
                reinterpret_cast<AnmVmId *>(&enemy->attachedVmId)) != 0 &&
            enemy->secondaryControl.freezeAttachedVm == 0)
        {
            struct AttachedPositions
            {
                Float3 attached;
                Float3 screen;
            } positions;
            positions.attached =
                *g_AnmManager->GetPosition(
                    *reinterpret_cast<AnmVmId *>(
                        &enemy->attachedVmId));
            PhotoToScreen(
                &positions.screen,
                reinterpret_cast<Float3 *>(&enemy->position));
            positions.attached =
                (positions.screen - positions.attached) * 0.07f +
                positions.attached;
            g_AnmManager->SetPosition(
                *reinterpret_cast<AnmVmId *>(&enemy->attachedVmId),
                &positions.attached);
        }

        if (enemy->photoPulseTimer > 0)
        {
            enemy->photoPulseTimer.Decrement(1);
        }

        if (GetPhotoEnemyAnmVmIdValue(
                reinterpret_cast<AnmVmId *>(&enemy->photoPulseVmId)) != 0)
        {
            AnmVm *photoPulseVm =
                g_AnmManager->GetVm(
                    *reinterpret_cast<AnmVmId *>(
                        &enemy->photoPulseVmId));
            if (enemy->photoPulseTimer > 0)
            {
                PhotoToScreen(
                    &photoPulseVm->positionOffset,
                    reinterpret_cast<Float3 *>(&enemy->position));
                photoPulseVm->scale.y =
                    static_cast<f32>(enemy->photoPulseTimer) /
                    static_cast<f32>(enemy->photoPulseDurationTimer) * 2.0f;
                photoPulseVm->scale.x = photoPulseVm->scale.y;
            }
            else
            {
                g_AnmManager->MarkVmForDeletion(
                    *reinterpret_cast<AnmVmId *>(
                        &enemy->photoPulseVmId));
                ResetPhotoEnemyAnmVmId(
                    reinterpret_cast<AnmVmId *>(
                        &enemy->photoPulseVmId));
            }
        }

        if (enemy->secondaryControl.showPhotoMarker != 0 && enemy->control.photoTarget != 0)
        {
            if (IsPhotoEnemyAnmVmIdNull(
                    reinterpret_cast<AnmVmId *>(
                        &enemy->photoMarkerVmId)))
            {
                *reinterpret_cast<AnmVmId *>(
                    &enemy->photoMarkerVmId) =
                    g_PhotoBulletManager->bulletAnm
                        ->CreateVmAtWorld(0x127, &enemy->worldPosition);
            }
            else
            {
                AnmVm *photoMarkerVm =
                    g_AnmManager->GetVm(
                        *reinterpret_cast<AnmVmId *>(
                            &enemy->photoMarkerVmId));
                PhotoToScreen(
                    &photoMarkerVm->positionOffset,
                    reinterpret_cast<Float3 *>(&enemy->position));
            }
        }
        else
        {
            g_AnmManager->MarkVmForDeletion(
                *reinterpret_cast<AnmVmId *>(
                    &enemy->photoMarkerVmId));
            ResetPhotoEnemyAnmVmId(
                reinterpret_cast<AnmVmId *>(
                    &enemy->photoMarkerVmId));
        }

        if (enemy->control.skipOffscreenCheck == 0)
        {
            f32 spriteWidth;
            f32 spriteHeight;
            if (enemy->vm.loadedSprite != NULL)
            {
                spriteWidth =
                    enemy->vm.loadedSprite->widthPx * enemy->vm.scale.x;
                spriteHeight =
                    enemy->vm.loadedSprite->heightPx * enemy->vm.scale.y;
            }
            else
            {
                spriteHeight = 0.0f;
                spriteWidth = spriteHeight;
            }

            if (IsPhotoEnemyOutsidePlayfield(
                    &enemy->position, spriteWidth, spriteHeight))
            {
                if (enemy->control.hasEnteredPlayfield != 0)
                {
                    enemy->Deactivate();
                    continue;
                }
            }
            else
            {
                enemy->control.hasEnteredPlayfield = 1;
            }
        }

        if (enemy->control.collidable != 0)
        {
            g_RuntimePlayerOwner->CheckBulletCollision(
                reinterpret_cast<Float3 *>(&enemy->position),
                &enemy->collisionSize);
        }
        AnmManager::ExecuteScript(&enemy->vm);

    enqueueEnemy:
        if (enemy->control.hiddenFromDrawGroups == 0)
        {
            if (enemyManager->drawGroupHeads[enemy->drawGroup] != NULL)
            {
                drawGroupTails[enemy->drawGroup]->nextInDrawGroup = enemy;
            }
            else
            {
                enemyManager->drawGroupHeads[enemy->drawGroup] = enemy;
            }
            enemy->nextInDrawGroup = NULL;
            drawGroupTails[enemy->drawGroup] = enemy;
        }
        enemyManager->activeEnemyCount++;
        enemy->stateTimer.Tick();
        enemy->eclTimer.Tick();
    }

    return 1;
}

void PhotoEnemyView::IntegrateMovement()
{
    this->positionDelta = this->position - this->previousPosition;
    this->previousPosition = this->position;

    if (this->control.mirrorMovementX == 0)
    {
        this->position.x += TH095_PHOTO_ENEMY_GAME_SPEED * this->velocity.x;
    }
    else
    {
        this->position.x -= TH095_PHOTO_ENEMY_GAME_SPEED * this->velocity.x;
    }
    this->position.y += TH095_PHOTO_ENEMY_GAME_SPEED * this->velocity.y;
    this->position.z += TH095_PHOTO_ENEMY_GAME_SPEED * this->velocity.z;
}

void PhotoEnemyView::ClampPosition()
{
    if (this->control.clampToMovementBounds != 0)
    {
        if (this->position.x < this->movementBounds.lower.x)
        {
            this->position.x = this->movementBounds.lower.x;
        }
        else if (this->position.x > this->movementBounds.upper.x)
        {
            this->position.x = this->movementBounds.upper.x;
        }

        if (this->position.y < this->movementBounds.lower.y)
        {
            this->position.y = this->movementBounds.lower.y;
        }
        else if (this->position.y > this->movementBounds.upper.y)
        {
            this->position.y = this->movementBounds.upper.y;
        }
    }
}

void PhotoEnemyView::UpdatePhotoMarkerPulse()
{
    if (this->secondaryControl.showPhotoMarker != 0)
    {
        this->photoMarkerPulseTimer.Decrement(1);
        if (this->photoMarkerPulseTimer <= 0)
        {
            this->secondaryControl.showPhotoMarker = 0;
        }
    }
}

void PhotoEnemyView::RestartEcl()
{
    g_PhotoEnemyManager->eclManager->InitializeContext(
        &this->mainEclContext,
        this->eclSubroutineIds[30]);
}

void __fastcall PhotoEnemyManagerView::ResetNonPhotoTargets(
    PhotoEnemyManagerView *enemyManager)
{
    for (i32 enemyIndex = 0; enemyIndex < 128; ++enemyIndex)
    {
        if (enemyManager->enemyPool[enemyIndex].control.photoTarget == 0)
        {
            enemyManager->enemyPool[enemyIndex].Deactivate();
        }
    }
}

void __fastcall PhotoEnemyManagerView::RestartPhotoTargetEcls(
    PhotoEnemyManagerView *enemyManager)
{
    for (i32 targetIndex = 0; targetIndex < 8; ++targetIndex)
    {
        if (enemyManager->photoTargets[targetIndex] != NULL)
        {
            enemyManager->photoTargets[targetIndex]->RestartEcl();
        }
    }
}

void __fastcall PhotoEnemyManagerView::ResetNonPhotoTargetsAndPhotoTargetEcls(
    PhotoEnemyManagerView *enemyManager)
{
    for (i32 enemyIndex = 0; enemyIndex < 128; ++enemyIndex)
    {
        if (enemyManager->enemyPool[enemyIndex].control.photoTarget == 0)
        {
            enemyManager->enemyPool[enemyIndex].Deactivate();
        }
    }

    for (i32 targetIndex = 0; targetIndex < 8; ++targetIndex)
    {
        if (enemyManager->photoTargets[targetIndex] != NULL)
        {
            enemyManager->eclManager->InitializeContext(
                &enemyManager->photoTargets[targetIndex]->mainEclContext,
                enemyManager->photoTargets[targetIndex]
                    ->eclSubroutineIds[31]);
        }
    }
}

void PhotoEnemyView::Deactivate()
{
    i32 argumentIndex;
    i32 pulseVmId = this->photoPulseVmId.value;
    if (pulseVmId != 0)
    {
        g_AnmManager->MarkVmForDeletion(
            *reinterpret_cast<AnmVmId *>(&this->photoPulseVmId));
    }

    i32 attachedVmId = this->attachedVmId.value;
    if (attachedVmId != 0)
    {
        g_AnmManager->MarkVmForDeletion(
            *reinterpret_cast<AnmVmId *>(&this->attachedVmId));
    }

    for (argumentIndex = 0; argumentIndex < 16; ++argumentIndex)
    {
        if (this->childEclBlocks[argumentIndex] != NULL)
        {
            void *argument = this->childEclBlocks[argumentIndex];
            free(argument);
        }
    }

    memset(this, 0, sizeof(*this));
}

// Stock VC7.1 identifier buckets reproduce the target's two scheduled-call
// locals without relying on the patched TH08 var_order frontend.
#define scheduledArgumentIndex restartCommandProcessingLocal05
#define scheduledCurrentPhoto averagedPanLocal12

i32 PhotoEnemyView::UpdateScheduledEclCalls()
{
    i32 activeScheduleCount = 0;
    for (i32 scheduleIndex = 0; scheduleIndex < PHOTO_ENEMY_SCHEDULE_SLOT_COUNT; ++scheduleIndex)
    {
        if (this->eclSchedule.photoIndices[scheduleIndex] < 0)
        {
            continue;
        }

        activeScheduleCount++;
        i32 scheduledArgumentIndex;
        i32 scheduledCurrentPhoto = g_RuntimePlayerOwner->camera.photoIndex;
        if (scheduledCurrentPhoto >= this->eclSchedule.photoIndices[scheduleIndex])
        {
            g_PhotoEnemyManager->eclManager->InitializeContext(
                &this->mainEclContext,
                this->eclSchedule.calls[scheduleIndex].subroutineId);
            this->eclSchedule.photoIndices[scheduleIndex] = PHOTO_ENEMY_SCHEDULE_INACTIVE;

            for (scheduledArgumentIndex = 0; scheduledArgumentIndex < 16; ++scheduledArgumentIndex)
            {
                if (this->childEclBlocks[scheduledArgumentIndex] != NULL)
                {
                    void *argument = this->childEclBlocks[scheduledArgumentIndex];
                    free(argument);
                    this->childEclBlocks[scheduledArgumentIndex] = NULL;
                }
            }

            memcpy(
                &this->bulletSpawnDescriptor,
                &g_PhotoEnemyManager->spawnTemplate.bulletSpawnDescriptor,
                sizeof(this->bulletSpawnDescriptor));
            this->shootIntervalFrames = 0;
        }
    }

    return 0;
}
#undef scheduledArgumentIndex
#undef scheduledCurrentPhoto

} // namespace th095

#if defined(TH095_MATCH_EXACT)
#undef FreePhotoEnemyChildEclBlock
#endif
