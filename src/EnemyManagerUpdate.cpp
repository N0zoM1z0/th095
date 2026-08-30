#include "AnmManager.hpp"
#include "AnmVmId.hpp"

#include <stdlib.h>
#include <string.h>

namespace th095
{

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

struct PhotoEnemyView;
struct PhotoEnemyManagerView;

struct PhotoEnemyEclFileView
{
    u8 unknown000[6];
    i16 timelineCount;
    void *timelines[1];
};

struct PhotoEnemyEclManagerView
{
    PhotoEnemyEclFileView *eclFile;

    void InitializeContext(void *context, i16 subroutineId);
    i32 RunEcl(PhotoEnemyView *enemy);
};

struct PhotoEnemyTimelineView
{
    ZunTimer timer;
    void *instruction;

    void Run();
};

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

typedef char PhotoEnemyTimelineSizeIs10[
    (sizeof(PhotoEnemyTimelineView) == 0x10) ? 1 : -1];

struct PhotoEnemyAnmSpawnerView
{
    AnmVmId CreateVm(i32 scriptIndex, Float3 *position);
};

struct PhotoEnemyBulletManagerView
{
    u8 unknown000000[0x27c5b0];
    PhotoEnemyAnmSpawnerView *anmSpawner;
};

struct PhotoEnemyPlayerView
{
    i32 CheckBulletCollision(Float3 *position, Float3 *size);
};

struct PhotoEnemyGameView
{
    u8 unknown0000[0x29e4];
    i32 frameCounter;
};

struct PhotoEnemyScheduledCall
{
    i16 subroutineId;
    i16 unknown02;
};

typedef char PhotoEnemyScheduledCallSizeIs4[
    (sizeof(PhotoEnemyScheduledCall) == 4) ? 1 : -1];

extern PhotoEnemyBulletManagerView *g_PhotoEnemyBulletManager;
extern PhotoEnemyManagerView *g_PhotoEnemyManager;
extern PhotoEnemyPlayerView *g_PhotoEnemyPlayer;
extern PhotoEnemyGameView *g_PhotoEnemyGame;
extern f32 g_PhotoEnemyEffectInterpolation;
extern f32 g_GameSpeed;

struct PhotoEnemyView
{
    PhotoEnemyView *nextInDrawGroup;       // +0x0000
    u8 unknown0004[4];
    AnmVm vm;                              // +0x0008
    u8 unknown02d4[0x2898 - 0x02d4];
    i16 mainEclSubroutineId;                // +0x2898
    i16 photoTargetEclSubroutineId;         // +0x289a
    i16 timelineValue;                     // +0x289c
    u8 unknown289e[2];
    D3DXVECTOR3 worldPosition;             // +0x28a0
    u8 unknown28ac[0x28b8 - 0x28ac];
    D3DXVECTOR3 velocity;                  // +0x28b8
    D3DXVECTOR3 previousPosition;          // +0x28c4
    D3DXVECTOR3 positionDelta;             // +0x28d0
    Float3 collisionSize;                  // +0x28dc
    u8 unknown28e8[0x28f4 - 0x28e8];
    Float3 photoMarkerPosition;            // +0x28f4
    u8 unknown2900[0x2958 - 0x2900];
    i32 life;                              // +0x2958
    i32 maximumLife;                       // +0x295c
    i32 phaseStartingLife;                 // +0x2960
    i32 score;                             // +0x2964
    i32 enemyIndex;                        // +0x2968
    ZunTimer eclTimer;                     // +0x296c
    ZunTimer stateTimer;                   // +0x2978
    u8 unknown2984[4];
    u32 displayColor;                      // +0x2988
    u8 unknown298c[0x2bd8 - 0x298c];
    i32 itemDropType;                      // +0x2bd8
    i32 timelineParam0;                    // +0x2bdc
    i32 timelineParam1;                    // +0x2be0
    u8 unknown2be4[0x2bf4 - 0x2be4];
    union
    {
        u32 flags1;                        // +0x2bf4
        struct
        {
            u32 active : 1;
            u32 photoTarget : 1;
            u32 collidable : 1;
            u32 unknownFlags003 : 1;
            u32 hiddenFromDrawGroups : 1;
            u32 unknownFlags005 : 3;
            u32 lifecycleState : 2;
            u32 unknownFlags010 : 6;
            u32 mirrorXVelocity : 1;
            u32 clampToMovementBounds : 1;
            u32 unknownFlags018 : 4;
            u32 hasEnteredPlayfield : 1;
            u32 unknownFlags023 : 3;
            u32 skipOffscreenCheck : 1;
            u32 unknownFlags027 : 5;
        };
    };
    union
    {
        u32 flags2;                        // +0x2bf8
        struct
        {
            u32 unknownFlags2_000 : 6;
            u32 showPhotoMarker : 1;
            u32 freezeAttachedVm : 1;
            u32 unknownFlags2_008 : 24;
        };
    };
    ZunTimer photoMarkerPulseTimer;         // +0x2bfc
    u8 unknown2c08[3];
    u8 drawGroup;                          // +0x2c0b
    u8 unknown2c0c[0x2c1c - 0x2c0c];
    AnmVmId photoPulseVmId;                // +0x2c1c
    AnmVmId photoMarkerVmId;               // +0x2c20
    ZunTimer photoMarkerTimer;              // +0x2c24
    u8 unknown2c30[4];
    f32 photoMarkerDuration;                // +0x2c34
    u8 unknown2c38[4];
    Float2 movementBoundsMin;               // +0x2c3c
    Float2 movementBoundsMax;               // +0x2c44
    u8 unknown2c4c[8];
    i32 scheduledCallFrames[10];            // +0x2c54
    PhotoEnemyScheduledCall scheduledCalls[10]; // +0x2c7c
    u8 unknown2ca4[8];
    void *allocatedEclArgs[16];              // +0x2cac
    u8 unknown2cec[0x4cbc - 0x2cec];
    AnmVmId attachedVmId;                  // +0x4cbc

    void IntegrateMovement();
    void ClampPosition();
    void RestartEcl();
    void UpdatePhotoMarkerPulse();
    i32 UpdateScheduledEclCalls();
    void Deactivate();
};

typedef char PhotoEnemySizeIs4CC0[
    (sizeof(PhotoEnemyView) == 0x4cc0) ? 1 : -1];
typedef char PhotoEnemyVmAt8[
    (offsetof(PhotoEnemyView, vm) == 0x08) ? 1 : -1];
typedef char PhotoEnemyPositionAt28A0[
    (offsetof(PhotoEnemyView, worldPosition) == 0x28a0) ? 1 : -1];
typedef char PhotoEnemyFlagsAt2BF4[
    (offsetof(PhotoEnemyView, flags1) == 0x2bf4) ? 1 : -1];
typedef char PhotoEnemyAttachedVmAt4CBC[
    (offsetof(PhotoEnemyView, attachedVmId) == 0x4cbc) ? 1 : -1];

struct PhotoEnemyManagerView
{
    u8 unknown000000[0x4cc0];
    PhotoEnemyTimelineView timelines[16];  // +0x4cc0
    PhotoEnemyView *drawGroupHeads[4];     // +0x4dc0
    u8 unknown4dd0[0x14];
    PhotoEnemyView *timelineEnemySlots[4];  // +0x4de4
    PhotoEnemyEclManagerView *eclManager;  // +0x4df4
    u8 unknown4df8[8];
    PhotoEnemyView enemies[128];           // +0x4e00
    PhotoEnemyView *photoTargets[8];        // +0x26ae00
    u8 unknown26ae20[0x0c];
    i32 activeEnemyCount;                  // +0x26ae2c

    static i32 __fastcall OnUpdate(PhotoEnemyManagerView *enemyManager);
    PhotoEnemyView *Spawn(
        i32 subroutineId,
        const Float3 *position,
        i32 life,
        i32 itemDrop,
        i32 score,
        u32 mirrorMovementX);
    PhotoEnemyView *SpawnWithContext(
        i32 subroutineId,
        const Float3 *position,
        i32 life,
        i32 itemDrop,
        i32 score,
        const i32 *contextValues);
    static void __fastcall ResetNonPhotoTargets(
        PhotoEnemyManagerView *enemyManager);
    static void __fastcall RestartPhotoTargetEcls(
        PhotoEnemyManagerView *enemyManager);
    static void __fastcall ResetNonPhotoTargetsAndPhotoTargetEcls(
        PhotoEnemyManagerView *enemyManager);
};

typedef char PhotoEnemyManagerTimelinesAt4CC0[
    (offsetof(PhotoEnemyManagerView, timelines) == 0x4cc0) ? 1 : -1];
typedef char PhotoEnemyManagerEnemiesAt4E00[
    (offsetof(PhotoEnemyManagerView, enemies) == 0x4e00) ? 1 : -1];
typedef char PhotoEnemyManagerCountAt26AE2C[
    (offsetof(PhotoEnemyManagerView, activeEnemyCount) == 0x26ae2c) ? 1 : -1];

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
                PhotoEnemyTimelineSpawnArgs *args =
                    reinterpret_cast<PhotoEnemyTimelineSpawnArgs *>(
                        reinterpret_cast<u8 *>(this->instruction) + 8);
                Float3 position;
                position.x = args->x;
                position.y = args->y;
                position.z = 0.0f;
                g_PhotoEnemyManager->Spawn(
                    args->subroutineId,
                    &position,
                    args->life,
                    args->itemDrop,
                    args->score,
                    mirrorMovementX);
                break;
            }

            case 15:
            {
                PhotoEnemyTimelineSpawnArgs *args =
                    reinterpret_cast<PhotoEnemyTimelineSpawnArgs *>(
                        reinterpret_cast<u8 *>(this->instruction) + 8);
                Float3 position;
                position.x = args->x;
                position.y = args->y;
                position.z = 0.0f;
                g_PhotoEnemyManager->Spawn(
                    args->subroutineId,
                    &position,
                    args->life,
                    args->itemDrop,
                    args->score,
                    mirrorMovementX);
                break;
            }

            case 12:
                mirrorMovementX = 1;
            case 11:
            {
                PhotoEnemyTimelineExtendedSpawnArgs *args =
                    reinterpret_cast<PhotoEnemyTimelineExtendedSpawnArgs *>(
                        reinterpret_cast<u8 *>(this->instruction) + 8);
                Float3 position;
                position.x = args->x;
                position.y = args->y;
                position.z = 0.0f;
                PhotoEnemyView *enemy = g_PhotoEnemyManager->Spawn(
                    args->subroutineId,
                    &position,
                    args->life,
                    -1,
                    args->score,
                    mirrorMovementX);
                enemy->timelineParam0 = args->timelineParam0;
                enemy->timelineParam1 = args->timelineParam1;
                break;
            }

            case 4:
                mirrorMovementX = 1;
            case 2:
            {
                PhotoEnemyTimelineRandomRangeArgs *args =
                    reinterpret_cast<PhotoEnemyTimelineRandomRangeArgs *>(
                        reinterpret_cast<u8 *>(this->instruction) + 8);
                Float3 position;
                position.x = (args->maximumX - args->minimumX) *
                        g_Rng.GetRandomF32() +
                    args->minimumX;
                position.y = args->y;
                position.z = 0.0f;
                g_PhotoEnemyManager->Spawn(
                    args->subroutineId,
                    &position,
                    args->life,
                    args->itemDrop,
                    args->score,
                    mirrorMovementX);
                break;
            }

            case 5:
                mirrorMovementX = 1;
            case 3:
            {
                PhotoEnemyTimelineRandomWidthArgs *args =
                    reinterpret_cast<PhotoEnemyTimelineRandomWidthArgs *>(
                        reinterpret_cast<u8 *>(this->instruction) + 8);
                Float3 position;
                position.x = g_Rng.GetRandomF32() * 384.0f;
                position.y = args->y;
                position.z = 0.0f;
                g_PhotoEnemyManager->Spawn(
                    args->subroutineId,
                    &position,
                    args->life,
                    args->itemDrop,
                    args->score,
                    mirrorMovementX);
                break;
            }

            case 8:
            {
                g_PhotoEnemyManager->timelineEnemySlots[
                    reinterpret_cast<i32 *>(
                        reinterpret_cast<u8 *>(this->instruction) + 8)[0]]
                    ->timelineValue = static_cast<i16>(
                        reinterpret_cast<i32 *>(
                            reinterpret_cast<u8 *>(this->instruction) + 8)[1]);
                break;
            }

            case 10:
            {
                if (g_PhotoEnemyManager->timelineEnemySlots[
                        reinterpret_cast<i32 *>(
                            reinterpret_cast<u8 *>(this->instruction) + 8)[0]] !=
                        NULL &&
                    g_PhotoEnemyManager->timelineEnemySlots[
                        reinterpret_cast<i32 *>(
                            reinterpret_cast<u8 *>(this->instruction) + 8)[0]]
                            ->active != 0)
                {
                    this->timer.Decrement(1);
                    goto finish;
                }
                break;
            }
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

    enemy = &this->enemies[0];
    for (enemyIndex = 0; enemyIndex < 128; ++enemyIndex, ++enemy)
    {
        if (enemy->active != 0)
        {
            continue;
        }

        *reinterpret_cast<EnemySpawnCopy *>(enemy) =
            *reinterpret_cast<const EnemySpawnCopy *>(this);
        enemy->enemyIndex = enemyIndex;
        enemy->mirrorXVelocity = mirrorMovementX;
        if (life >= 0)
        {
            enemy->life = life;
        }
        *reinterpret_cast<Float3 *>(&enemy->worldPosition) = *position;
        this->eclManager->InitializeContext(
            reinterpret_cast<u8 *>(enemy) + 0x2dc,
            static_cast<i16>(subroutineId));
        if (this->eclManager->RunEcl(enemy) == ZUN_ERROR)
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
    const i32 *contextValues)
{
    struct EnemySpawnCopy
    {
        u32 words[sizeof(PhotoEnemyView) / sizeof(u32)];
    };
    struct EnemyContextCopy
    {
        u32 words[0x80 / sizeof(u32)];
    };
    i32 enemyIndex;
    PhotoEnemyView *enemy;

    enemy = &this->enemies[0];
    for (enemyIndex = 0; enemyIndex < 128; ++enemyIndex, ++enemy)
    {
        if (enemy->active != 0)
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
        *reinterpret_cast<Float3 *>(&enemy->worldPosition) = *position;
        this->eclManager->InitializeContext(
            reinterpret_cast<u8 *>(enemy) + 0x2dc,
            static_cast<i16>(subroutineId));
        *reinterpret_cast<EnemyContextCopy *>(
            reinterpret_cast<u8 *>(enemy) + 0x2f4) =
            *reinterpret_cast<const EnemyContextCopy *>(contextValues);
        if (this->eclManager->RunEcl(enemy) == ZUN_ERROR)
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

i32 __fastcall PhotoEnemyManagerView::OnUpdate(
    PhotoEnemyManagerView *enemyManager)
{
    PhotoEnemyView *enemy = &enemyManager->enemies[0];

    for (i32 timelineIndex = 0;
         timelineIndex < enemyManager->eclManager->eclFile->timelineCount;
         ++timelineIndex)
    {
        if (enemyManager->timelines[timelineIndex].instruction == NULL)
        {
            void *instruction;
            instruction =
                enemyManager->eclManager->eclFile->timelines[timelineIndex];
            enemyManager->timelines[timelineIndex].instruction =
                instruction;
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
        if (enemy->active == 0)
        {
            continue;
        }

        if (enemy->lifecycleState != 0)
        {
            if (enemy->lifecycleState >= 2)
            {
                enemy->Deactivate();
                continue;
            }
            enemy->lifecycleState++;
            goto enqueueEnemy;
        }

        enemy->UpdatePhotoMarkerPulse();
        enemy->UpdateScheduledEclCalls();
        if (enemyManager->eclManager->RunEcl(enemy) == -1)
        {
            enemy->Deactivate();
            continue;
        }

        enemy->IntegrateMovement();
        enemy->ClampPosition();

        if (enemy->attachedVmId.value != 0 &&
            enemy->freezeAttachedVm == 0)
        {
            D3DXVECTOR3 attachedPosition =
                *reinterpret_cast<D3DXVECTOR3 *>(
                    g_AnmManager->GetPosition(enemy->attachedVmId));
            D3DXVECTOR3 screenPosition;
            PhotoToScreen(
                reinterpret_cast<Float3 *>(&screenPosition),
                reinterpret_cast<Float3 *>(&enemy->worldPosition));
            attachedPosition +=
                (screenPosition - attachedPosition) *
                g_PhotoEnemyEffectInterpolation;
            g_AnmManager->SetPosition(
                enemy->attachedVmId,
                reinterpret_cast<Float3 *>(&attachedPosition));
        }

        if (enemy->photoMarkerTimer.current > 0)
        {
            enemy->photoMarkerTimer.Add(-1.0f);
        }

        if (enemy->photoPulseVmId.value != 0)
        {
            AnmVm *photoPulseVm =
                g_AnmManager->GetVm(enemy->photoPulseVmId);
            if (enemy->photoMarkerTimer.current <= 0)
            {
                g_AnmManager->MarkVmForDeletion(enemy->photoPulseVmId);
                enemy->photoPulseVmId.value = 0;
            }
            else
            {
                PhotoToScreen(
                    &photoPulseVm->positionOffset,
                    reinterpret_cast<Float3 *>(&enemy->worldPosition));
                photoPulseVm->scale.y =
                    enemy->photoMarkerTimer.subFrame /
                    enemy->photoMarkerDuration * 2.0f;
                photoPulseVm->scale.x = photoPulseVm->scale.y;
            }
        }

        if (enemy->showPhotoMarker == 0 || enemy->photoTarget == 0)
        {
            g_AnmManager->MarkVmForDeletion(enemy->photoMarkerVmId);
            enemy->photoMarkerVmId.value = 0;
        }
        else if (enemy->photoMarkerVmId.value == 0)
        {
            enemy->photoMarkerVmId =
                g_PhotoEnemyBulletManager->anmSpawner->CreateVm(
                    0x127, &enemy->photoMarkerPosition);
        }
        else
        {
            AnmVm *photoMarkerVm =
                g_AnmManager->GetVm(enemy->photoMarkerVmId);
            PhotoToScreen(
                &photoMarkerVm->positionOffset,
                reinterpret_cast<Float3 *>(&enemy->worldPosition));
        }

        if (enemy->skipOffscreenCheck == 0)
        {
            f32 spriteWidth;
            f32 spriteHeight;
            if (enemy->vm.loadedSprite == NULL)
            {
                spriteWidth = 0.0f;
                spriteHeight = 0.0f;
            }
            else
            {
                spriteWidth =
                    enemy->vm.loadedSprite->widthPx * enemy->vm.scale.x;
                spriteHeight =
                    enemy->vm.loadedSprite->heightPx * enemy->vm.scale.y;
            }

            bool outsidePlayfield =
                spriteWidth + enemy->worldPosition.x <= -192.0f ||
                enemy->worldPosition.x - spriteWidth >= 192.0f ||
                spriteHeight + enemy->worldPosition.y <= 0.0f ||
                enemy->worldPosition.y - spriteHeight >= 448.0f;
            if (outsidePlayfield)
            {
                if (enemy->hasEnteredPlayfield != 0)
                {
                    enemy->Deactivate();
                    continue;
                }
            }
            else
            {
                enemy->hasEnteredPlayfield = 1;
            }
        }

        if (enemy->collidable != 0)
        {
            g_PhotoEnemyPlayer->CheckBulletCollision(
                reinterpret_cast<Float3 *>(&enemy->worldPosition),
                &enemy->collisionSize);
        }
        AnmManager::ExecuteScript(&enemy->vm);

    enqueueEnemy:
        if (enemy->hiddenFromDrawGroups == 0)
        {
            if (enemyManager->drawGroupHeads[enemy->drawGroup] == NULL)
            {
                enemyManager->drawGroupHeads[enemy->drawGroup] = enemy;
            }
            else
            {
                drawGroupTails[enemy->drawGroup]->nextInDrawGroup = enemy;
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
    this->positionDelta = this->worldPosition - this->previousPosition;
    this->previousPosition = this->worldPosition;

    if (this->mirrorXVelocity == 0)
    {
        this->worldPosition.x += g_GameSpeed * this->velocity.x;
    }
    else
    {
        this->worldPosition.x -= g_GameSpeed * this->velocity.x;
    }
    this->worldPosition.y += g_GameSpeed * this->velocity.y;
    this->worldPosition.z += g_GameSpeed * this->velocity.z;
}

void PhotoEnemyView::ClampPosition()
{
    if (this->clampToMovementBounds != 0)
    {
        if (this->worldPosition.x < this->movementBoundsMin.x)
        {
            this->worldPosition.x = this->movementBoundsMin.x;
        }
        else if (this->worldPosition.x > this->movementBoundsMax.x)
        {
            this->worldPosition.x = this->movementBoundsMax.x;
        }

        if (this->worldPosition.y < this->movementBoundsMin.y)
        {
            this->worldPosition.y = this->movementBoundsMin.y;
        }
        else if (this->worldPosition.y > this->movementBoundsMax.y)
        {
            this->worldPosition.y = this->movementBoundsMax.y;
        }
    }
}

void PhotoEnemyView::UpdatePhotoMarkerPulse()
{
    if (this->showPhotoMarker != 0)
    {
        this->photoMarkerPulseTimer.Decrement(1);
        if (this->photoMarkerPulseTimer <= 0)
        {
            this->showPhotoMarker = 0;
        }
    }
}

void PhotoEnemyView::RestartEcl()
{
    g_PhotoEnemyManager->eclManager->InitializeContext(
        reinterpret_cast<u8 *>(this) + 0x2dc,
        this->mainEclSubroutineId);
}

void __fastcall PhotoEnemyManagerView::ResetNonPhotoTargets(
    PhotoEnemyManagerView *enemyManager)
{
    for (i32 enemyIndex = 0; enemyIndex < 128; ++enemyIndex)
    {
        if (enemyManager->enemies[enemyIndex].photoTarget == 0)
        {
            enemyManager->enemies[enemyIndex].Deactivate();
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
        if (enemyManager->enemies[enemyIndex].photoTarget == 0)
        {
            enemyManager->enemies[enemyIndex].Deactivate();
        }
    }

    for (i32 targetIndex = 0; targetIndex < 8; ++targetIndex)
    {
        if (enemyManager->photoTargets[targetIndex] != NULL)
        {
            enemyManager->eclManager->InitializeContext(
                reinterpret_cast<u8 *>(enemyManager->photoTargets[targetIndex]) +
                    0x2dc,
                enemyManager->photoTargets[targetIndex]
                    ->photoTargetEclSubroutineId);
        }
    }
}

void PhotoEnemyView::Deactivate()
{
    i32 argumentIndex;
    i32 pulseVmId = this->photoPulseVmId.value;
    if (pulseVmId != 0)
    {
        g_AnmManager->MarkVmForDeletion(this->photoPulseVmId);
    }

    i32 attachedVmId = this->attachedVmId.value;
    if (attachedVmId != 0)
    {
        g_AnmManager->MarkVmForDeletion(this->attachedVmId);
    }

    for (argumentIndex = 0; argumentIndex < 16; ++argumentIndex)
    {
        if (this->allocatedEclArgs[argumentIndex] != NULL)
        {
            void *argument = this->allocatedEclArgs[argumentIndex];
            free(argument);
        }
    }

    memset(this, 0, sizeof(*this));
}

i32 PhotoEnemyView::UpdateScheduledEclCalls()
{
    i32 activeScheduleCount = 0;
    for (i32 scheduleIndex = 0; scheduleIndex < 10; ++scheduleIndex)
    {
        if (this->scheduledCallFrames[scheduleIndex] < 0)
        {
            continue;
        }

        activeScheduleCount++;
        i32 argumentIndex;
        i32 currentFrame = g_PhotoEnemyGame->frameCounter;
        if (currentFrame >= this->scheduledCallFrames[scheduleIndex])
        {
            g_PhotoEnemyManager->eclManager->InitializeContext(
                reinterpret_cast<u8 *>(this) + 0x2dc,
                this->scheduledCalls[scheduleIndex].subroutineId);
            this->scheduledCallFrames[scheduleIndex] = -1;

            for (argumentIndex = 0; argumentIndex < 16; ++argumentIndex)
            {
                if (this->allocatedEclArgs[argumentIndex] != NULL)
                {
                    void *argument = this->allocatedEclArgs[argumentIndex];
                    free(argument);
                    this->allocatedEclArgs[argumentIndex] = NULL;
                }
            }

            memcpy(
                reinterpret_cast<u8 *>(this) + 0x298c,
                reinterpret_cast<u8 *>(g_PhotoEnemyManager) + 0x298c,
                0x210);
            *reinterpret_cast<i32 *>(
                reinterpret_cast<u8 *>(this) + 0x2bc8) = 0;
        }
    }

    return 0;
}

} // namespace th095
