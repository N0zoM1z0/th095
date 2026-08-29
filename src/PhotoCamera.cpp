#include "PhotoCamera.hpp"

namespace th095
{

struct PhotoEnemyView
{
    u8 unknown0000[0x28a0];
    Float3 position;
};

struct PhotoRuntimeView
{
    u8 unknown000000[0x26ae00];
    PhotoEnemyView *enemies[8];
};

struct PhotoGlobalStateView
{
    u8 unknown000[0xfc];
    u32 flags;
};

struct PhotoStageStateView
{
    u8 unknown00000[0x2571c];
    PhotoAnmLoadedView *anm;
    u32 flags;
};

struct PhotoBulletManagerView
{
    u8 unknown0000[0x1760];
    ZunColor photoColor;

    void BeginPhotoCapture(const Float3 *position, const Float3 *size);
};

struct PhotoAnmManagerView
{
    AnmVm *FindVm(i32 id);
    void SetVmInterrupt(i32 id, i16 interrupt);
    void RemoveVm(i32 id);
    void SetVmPosition(i32 id, const Float3 *position);
};

struct PhotoSoundPlayerView
{
    void PlaySoundByIdx(i32 idx, i32 pan);
    void PlaySoundPositionedByIdx(i32 idx, f32 pan);
    void StopSoundByIdx(i32 idx);
};

static inline PhotoAnmManagerView *PhotoAnmManager()
{
    return reinterpret_cast<PhotoAnmManagerView *>(g_AnmManager);
}

static inline PhotoSoundPlayerView *PhotoSoundPlayer()
{
    return reinterpret_cast<PhotoSoundPlayerView *>(&g_SoundPlayer);
}

extern PhotoGameStateView *g_PhotoGame;
extern PhotoRuntimeView *g_PhotoRuntime;
extern PhotoGlobalStateView *g_PhotoGlobalState;
extern PhotoStageStateView *g_PhotoStageState;
extern PhotoBulletManagerView *g_PhotoBulletManager;
extern u16 g_PhotoInput;
extern u16 g_PhotoInputPressed;

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

enum PhotoCameraFlags
{
    PHOTO_FLAG_ALTERNATE_CAPTURE = 1 << 0,
    PHOTO_FLAG_LOCK_TO_PLAYER = 1 << 1,
    PHOTO_FLAG_TARGET_FRAME_ACTIVE = 1 << 2,
    PHOTO_FLAG_CHARGE_UI_MASK = 3 << 3,
    PHOTO_FLAG_TARGET_SOUND_PLAYED = 1 << 6,
};

static inline bool PhotoSoundsEnabled()
{
    return ((g_PhotoGlobalState->flags >> 9) & 1) == 0;
}

static inline void SetPhotoVmColor(AnmVm *vm, u8 red, u8 green, u8 blue)
{
    vm->color1.a = 0xff;
    vm->color1.r = red;
    vm->color1.g = green;
    vm->color1.b = blue;
}

static inline i32 PreservePhotoId(i32 value)
{
    return *reinterpret_cast<volatile i32 *>(&value);
}

f32 PhotoGameStateView::AngleToPoint(const Float3 *point)
{
    f32 deltaX = point->x - this->playerPosition.x;
    f32 deltaY = point->y - this->playerPosition.y;

    if (deltaY == 0.0f && deltaX == 0.0f)
    {
        return 1.5707964f;
    }
    return atan2f(deltaY, deltaX);
}

void PhotoCameraState::BeginCapture()
{
    if (this->mode != PHOTO_CAMERA_TRACKING)
    {
        return;
    }
    this->mode = PHOTO_CAMERA_CHARGING;
    this->modeTimer = 0;
    this->flags &= ~PHOTO_FLAG_TARGET_SOUND_PLAYED;
    this->vmIds[2] = g_PhotoStageState->anm->CreateVm(0x18, 0);
    this->vmIds[3] = g_PhotoStageState->anm->CreateVm(0x19, 0);
    this->vmIds[4] = g_PhotoStageState->anm->CreateVm(0x1a, 0);
    this->vmIds[5] = g_PhotoStageState->anm->CreateVm(0x1b, 0);
    this->vmIds[6] = g_PhotoStageState->anm->CreateVm(0x1c, 0);
    if (this->vmIds[9] != 0)
    {
        PhotoAnmManager()->RemoveVm(this->vmIds[9].value);
        this->vmIds[9].value = PreservePhotoId(0);
    }
    if (this->vmIds[10] != 0)
    {
        PhotoAnmManager()->RemoveVm(this->vmIds[10].value);
        this->vmIds[10].value = PreservePhotoId(0);
    }
    if (((g_PhotoGlobalState->flags >> 9) & 1) == 0)
    {
        PhotoSoundPlayer()->PlaySoundByIdx(0x2c, 0);
    }
}

f32 __fastcall PhotoDistance2D(const Float3 *left, const Float3 *right)
{
    return sqrtf(
        (left->x - right->x) * (left->x - right->x) +
        (left->y - right->y) * (left->y - right->y));
}

void __fastcall UpdatePhotoCamera(PhotoCameraState *camera)
{
    PhotoEnemyView *boss = g_PhotoRuntime->enemies[0];
    Float3 screenPosition;
    Float3 effectPosition;
    AnmVm *frameVm;
    i32 chargeDisplay;
    i32 angleSector;
    f32 targetAngle;

    switch (camera->mode)
    {
    case PHOTO_CAMERA_TRACKING:
        if ((camera->flags & PHOTO_FLAG_LOCK_TO_PLAYER) == 0)
        {
            if (boss == NULL)
            {
                camera->cameraOffset = g_PhotoGame->playerPosition;
                camera->cameraOffset.y -= 64.0f;
            }
            else
            {
                if (g_PhotoGame->cameraTrackingMode == 2)
                {
                    camera->trackingRadius = 56.0f;
                }
                else if (g_PhotoGame->cameraTrackingMode == 1)
                {
                    f32 playerDistance = PhotoDistance2D(
                        &g_PhotoGame->playerPosition, &camera->viewfinderPosition);
                    f32 bossDistance = PhotoDistance2D(
                        &boss->position, &g_PhotoGame->playerPosition);
                    if (playerDistance < 56.0f)
                    {
                        camera->trackingRadius = 56.0f;
                    }
                    else if (playerDistance < bossDistance)
                    {
                        camera->trackingRadius = playerDistance + 2.0f;
                    }
                    else if (bossDistance < playerDistance)
                    {
                        camera->trackingRadius = playerDistance - 2.0f;
                    }
                    if (camera->trackingRadius >= 88.0f)
                    {
                        camera->trackingRadius = 88.0f;
                    }
                }
                else if (camera->trackingRadius >= 48.0f)
                {
                    camera->trackingRadius -= 1.0f;
                }
                else if (camera->trackingRadius >= 44.0f)
                {
                    camera->trackingRadius += 1.0f;
                }

                if (g_PhotoGame->cameraTrackingMode == 0)
                {
                    Float3 playerDelta;
                    playerDelta.x = g_PhotoGame->playerPosition.x - camera->previousTrackingOrigin.x;
                    playerDelta.y = g_PhotoGame->playerPosition.y - camera->previousTrackingOrigin.y;
                    playerDelta.z = g_PhotoGame->playerPosition.z - camera->previousTrackingOrigin.z;
                    if (playerDelta.x * playerDelta.x + playerDelta.y * playerDelta.y >= 0.1f)
                    {
                        targetAngle = atan2f(playerDelta.y, playerDelta.x);
                    }
                    else
                    {
                        targetAngle = g_PhotoGame->AngleToPoint(&boss->position);
                    }
                    targetAngle = AddNormalizeAngle(targetAngle - camera->trackingAngle, 0.0f);
                    camera->trackingAngle += targetAngle * 0.04f;
                    camera->cameraOffset.FromAngleMagnitude(
                        camera->trackingAngle, camera->trackingRadius);
                }
                else
                {
                    camera->cameraOffset.x = boss->position.x - g_PhotoGame->playerPosition.x;
                    camera->cameraOffset.y = boss->position.y - g_PhotoGame->playerPosition.y;
                    camera->cameraOffset.z = boss->position.z - g_PhotoGame->playerPosition.z;
                    D3DXVec3Normalize(
                        reinterpret_cast<D3DXVECTOR3 *>(&camera->cameraOffset),
                        reinterpret_cast<D3DXVECTOR3 *>(&camera->cameraOffset));
                    camera->cameraOffset.x *= camera->trackingRadius;
                    camera->cameraOffset.y *= camera->trackingRadius;
                    camera->cameraOffset.z *= camera->trackingRadius;
                }
                camera->cameraOffset += g_PhotoGame->playerPosition;
                camera->previousTrackingOrigin = g_PhotoGame->playerPosition;
            }

            if (g_PhotoGame->cameraTrackingMode == 0)
            {
                Float3 delta;
                delta.x = camera->cameraOffset.x - camera->viewfinderPosition.x;
                delta.y = camera->cameraOffset.y - camera->viewfinderPosition.y;
                delta.z = camera->cameraOffset.z - camera->viewfinderPosition.z;
                camera->viewfinderPosition.x += delta.x * 0.4f;
                camera->viewfinderPosition.y += delta.y * 0.4f;
                camera->viewfinderPosition.z += delta.z * 0.4f;
            }
            else
            {
                Float3 delta;
                delta.x = camera->cameraOffset.x - camera->viewfinderPosition.x;
                delta.y = camera->cameraOffset.y - camera->viewfinderPosition.y;
                delta.z = camera->cameraOffset.z - camera->viewfinderPosition.z;
                camera->viewfinderPosition.x += delta.x * 0.4f;
                camera->viewfinderPosition.y += delta.y * 0.4f;
                camera->viewfinderPosition.z += delta.z * 0.4f;
                camera->trackingAngle = atan2f(
                    camera->viewfinderPosition.y - g_PhotoGame->playerPosition.y,
                    camera->viewfinderPosition.x - g_PhotoGame->playerPosition.x);
            }
        }
        else
        {
            camera->cameraOffset = g_PhotoGame->playerPosition;
            Float3 delta;
            delta.x = camera->cameraOffset.x - camera->viewfinderPosition.x;
            delta.y = camera->cameraOffset.y - camera->viewfinderPosition.y;
            delta.z = camera->cameraOffset.z - camera->viewfinderPosition.z;
            camera->viewfinderPosition.x += delta.x * 0.4f;
            camera->viewfinderPosition.y += delta.y * 0.4f;
            camera->viewfinderPosition.z += delta.z * 0.4f;
        }

        if (camera->viewfinderPosition.x <= -176.0f)
            camera->viewfinderPosition.x = -176.0f;
        else if (camera->viewfinderPosition.x > 176.0f)
            camera->viewfinderPosition.x = 176.0f;
        if (camera->viewfinderPosition.y <= 16.0f)
            camera->viewfinderPosition.y = 16.0f;
        else if (camera->viewfinderPosition.y > 432.0f)
            camera->viewfinderPosition.y = 432.0f;
        goto updateCharge;

    case PHOTO_CAMERA_CHARGING:
        camera->UpdateViewfinder();
        if ((g_PhotoInput & 2) == 0)
        {
            camera->TakePhoto();
            break;
        }
        if (camera->modeTimer.current >= 4)
        {
            camera->charge -= 1.0f / 42.0f;
        }
        if (camera->charge <= 0.0f)
        {
            camera->CancelCapture();
            break;
        }
        if (!PhotoSoundsEnabled())
        {
            PhotoSoundPlayer()->StopSoundByIdx(0x2c);
        }
        goto cameraActive;

    case PHOTO_CAMERA_CAPTURED:
        if (camera->modeTimer.current != camera->modeTimer.previous &&
            camera->modeTimer.current == 20)
        {
            if (g_PhotoGame->movementState >= 0 && g_PhotoGame->movementState <= 2)
            {
                g_PhotoGame->effectAnm->InitializeVm(&g_PhotoGame->effectVm, 0);
            }
            if ((camera->flags & PHOTO_FLAG_ALTERNATE_CAPTURE) != 0)
            {
                if (PhotoSoundsEnabled())
                {
                    PhotoSoundPlayer()->PlaySoundPositionedByIdx(
                        0x21, camera->viewfinderPosition.x);
                }
                PhotoToScreen(&effectPosition, &g_PhotoGame->playerPosition);
                PhotoAnmVmId effect = g_PhotoStageState->anm->CreateVm(0x21, 0);
                PhotoAnmManager()->SetVmPosition(effect.value, &effectPosition);
            }
            else
            {
                if (PhotoSoundsEnabled())
                {
                    PhotoSoundPlayer()->PlaySoundPositionedByIdx(
                        0x25, camera->viewfinderPosition.x);
                }
                PhotoToScreen(&effectPosition, &g_PhotoGame->playerPosition);
                PhotoAnmVmId effect = g_PhotoStageState->anm->CreateVm(0x22, 0);
                PhotoAnmManager()->SetVmPosition(effect.value, &effectPosition);
            }
        }
        if (camera->modeTimer.current >= 60)
        {
            camera->mode = PHOTO_CAMERA_RECOVERING;
            camera->modeTimer.Initialize();
            camera->charge = 0.0f;
        }
        break;

    case PHOTO_CAMERA_RECOVERING:
        if (camera->modeTimer.current >= 20)
        {
            camera->mode = PHOTO_CAMERA_TRACKING;
        }
        goto updateCharge;

    case PHOTO_CAMERA_DISABLED:
        camera->charge = 0.0f;
        if (camera->vmIds[0].value != 0)
        {
            PhotoAnmManager()->SetVmInterrupt(camera->vmIds[0].value, 1);
            PhotoAnmManager()->SetVmInterrupt(camera->vmIds[1].value, 1);
            camera->vmIds[0].value = 0;
            camera->vmIds[1].value = 0;
        }
        break;
    }
    goto finish;

updateCharge:
    camera->UpdateCharge();
    if (camera->auxiliaryTimer.current >= 60)
    {
        if (camera->charge < 1.0f)
        {
            if ((camera->flags & PHOTO_FLAG_CHARGE_UI_MASK) != 0)
            {
                if (camera->vmIds[9].value != 0)
                {
                    PhotoAnmManager()->RemoveVm(camera->vmIds[9].value);
                    camera->vmIds[9].value = 0;
                }
                if (camera->vmIds[10].value == 0)
                {
                    camera->vmIds[10] = g_PhotoStageState->anm->CreateVm(0x20, 0);
                }
                PhotoAnmManager()->SetVmInterrupt(camera->vmIds[0].value, 3);
                PhotoAnmManager()->SetVmInterrupt(camera->vmIds[1].value, 3);
                camera->flags &= ~PHOTO_FLAG_CHARGE_UI_MASK;
                for (i32 i = 0; i < 4; ++i)
                {
                    camera->viewfinderVms[i].pendingInterrupt = 3;
                }
                frameVm = camera->vmIds[0].GetVm();
                SetPhotoVmColor(frameVm, 0xff, 0xff, 0xff);
            }
        }
        else
        {
            if ((camera->flags & PHOTO_FLAG_CHARGE_UI_MASK) != (1 << 3))
            {
                if (PhotoSoundsEnabled())
                {
                    PhotoSoundPlayer()->PlaySoundByIdx(0x2b, 0);
                }
                if (camera->vmIds[10].value != 0)
                {
                    PhotoAnmManager()->RemoveVm(camera->vmIds[10].value);
                    camera->vmIds[10].value = 0;
                }
                if (camera->vmIds[9].value == 0)
                {
                    camera->vmIds[9] = g_PhotoStageState->anm->CreateVm(0x1f, 0);
                }
                camera->vmIds[0].SetInterrupt(2);
                camera->vmIds[1].SetInterrupt(2);
                camera->flags = (camera->flags & ~PHOTO_FLAG_CHARGE_UI_MASK) | (1 << 3);
                for (i32 i = 0; i < 4; ++i)
                {
                    camera->viewfinderVms[i].pendingInterrupt = 2;
                }
            }
            camera->viewfinderSize.x = 256.0f;
            camera->viewfinderSize.y = 192.0f;
            camera->viewfinderSize.z = 0.0f;
            frameVm = camera->vmIds[0].GetVm();
            if (camera->CountPhotoTargets(NULL, NULL) == 0)
                SetPhotoVmColor(frameVm, 0xff, 0xff, 0xff);
            else
                SetPhotoVmColor(frameVm, 0x20, 0x20, 0xff);
        }
    }

    if ((g_PhotoInputPressed & 2) != 0)
        camera->captureRequested = 1;
    else if ((g_PhotoInput & 2) == 0)
        camera->captureRequested = 0;
    else if ((g_PhotoInput & 3) == 3)
        camera->captureRequested = 1;

    if ((camera->flags & PHOTO_FLAG_LOCK_TO_PLAYER) == 0 &&
        camera->charge >= 1.0f &&
        (g_PhotoInput & 2) != 0 &&
        (g_PhotoInput & 1) == 0 &&
        camera->captureRequested != 0 &&
        (camera->focusHeldFrames >= 5 || (g_PhotoInputPressed & 2) != 0))
    {
        camera->BeginCapture();
        camera->flags |= PHOTO_FLAG_TARGET_FRAME_ACTIVE;

cameraActive:
        targetAngle = g_PhotoGame->AngleToPoint(&camera->viewfinderPosition) + 0.3926991f;
        if (targetAngle < 0.0f)
            targetAngle += 6.2831855f;
        angleSector = (i32)(targetAngle / 0.7853982f);
        g_PhotoGame->effectVm.anmFile =
            reinterpret_cast<AnmLoaded *>(g_PhotoGame->effectAnm);
        g_PhotoGame->effectVm.scriptIndex = 5;
        g_PhotoGame->effectAnm->SetAndExecuteScript(
            &g_PhotoGame->effectVm, g_PhotoGame->effectAnm->scripts[5]);
        g_PhotoGame->effectAnm->SetSprite(&g_PhotoGame->effectVm, angleSector + 0x18);

        if (camera->CountPhotoTargets(NULL, NULL) == 0)
        {
            camera->flags &= ~PHOTO_FLAG_TARGET_SOUND_PLAYED;
            if ((camera->flags & PHOTO_FLAG_TARGET_FRAME_ACTIVE) != 0)
            {
                frameVm = camera->vmIds[0].GetVm();
                SetPhotoVmColor(frameVm, 0xff, 0xff, 0xff);
                for (i32 i = 2; i <= 6; ++i)
                    PhotoAnmManager()->SetVmInterrupt(camera->vmIds[i].value, 3);
                camera->flags &= ~PHOTO_FLAG_TARGET_FRAME_ACTIVE;
            }
        }
        else
        {
            if ((camera->flags & PHOTO_FLAG_TARGET_SOUND_PLAYED) == 0)
            {
                if (PhotoSoundsEnabled())
                    PhotoSoundPlayer()->PlaySoundByIdx(0x2e, 0);
                camera->flags |= PHOTO_FLAG_TARGET_SOUND_PLAYED;
            }
            if ((camera->flags & PHOTO_FLAG_TARGET_FRAME_ACTIVE) == 0)
            {
                frameVm = camera->vmIds[0].GetVm();
                SetPhotoVmColor(frameVm, 0x20, 0x20, 0xff);
                for (i32 i = 2; i <= 6; ++i)
                    PhotoAnmManager()->SetVmInterrupt(camera->vmIds[i].value, 2);
                camera->flags |= PHOTO_FLAG_TARGET_FRAME_ACTIVE;
            }
        }

        g_PhotoBulletManager->BeginPhotoCapture(
            &camera->viewfinderPosition, &camera->viewfinderSize);
        if (camera->charge >= 0.35f)
        {
            g_AnmGameSpeed = 0.25f;
            g_PhotoBulletManager->photoColor.color = 0x60404040;
        }
        else
        {
            f32 slowRate = (0.35f - camera->charge) / 0.35f;
            g_AnmGameSpeed = slowRate * 0.75f + 0.25f;
            g_PhotoBulletManager->photoColor.a = (u8)(slowRate * 32.0f) + 0x60;
            g_PhotoBulletManager->photoColor.r = (u8)(slowRate * 64.0f) + 0x40;
            g_PhotoBulletManager->photoColor.g = (u8)(slowRate * 64.0f) + 0x40;
            g_PhotoBulletManager->photoColor.b = (u8)(slowRate * 64.0f) + 0x40;
        }
    }

finish:
    PhotoToScreen(&screenPosition, &camera->viewfinderPosition);
    PhotoAnmManager()->SetVmPosition(camera->vmIds[0].value, &screenPosition);
    PhotoAnmManager()->SetVmPosition(camera->vmIds[1].value, &screenPosition);
    PhotoAnmManager()->SetVmPosition(camera->vmIds[9].value, &screenPosition);
    PhotoAnmManager()->SetVmPosition(camera->vmIds[10].value, &screenPosition);
    if (((g_PhotoStageState->flags >> 2) & 1) != 0)
    {
        PhotoAnmManager()->SetVmInterrupt(camera->vmIds[0].value, 5);
        PhotoAnmManager()->SetVmInterrupt(camera->vmIds[1].value, 5);
        PhotoAnmManager()->SetVmInterrupt(camera->vmIds[9].value, 5);
        PhotoAnmManager()->SetVmInterrupt(camera->vmIds[10].value, 5);
    }
    for (i32 i = 0; i < 4; ++i)
    {
        camera->viewfinderVms[i].positionOffset = screenPosition;
    }

    chargeDisplay = (i32)(camera->charge * 100.0f);
    if (chargeDisplay / 100 == 0)
    {
        camera->viewfinderVms[0].flagsWord &= ~2U;
    }
    else
    {
        g_PhotoStageState->anm->SetSprite(
            &camera->viewfinderVms[0], chargeDisplay / 100 + 0xf);
        camera->viewfinderVms[0].flagsWord |= 2;
    }
    if (chargeDisplay / 10 == 0)
    {
        camera->viewfinderVms[1].flagsWord &= ~2U;
    }
    else
    {
        g_PhotoStageState->anm->SetSprite(
            &camera->viewfinderVms[1], chargeDisplay / 10 % 10 + 0xf);
        camera->viewfinderVms[1].flagsWord |= 2;
    }
    g_PhotoStageState->anm->SetSprite(
        &camera->viewfinderVms[2], chargeDisplay % 10 + 0xf);
    camera->viewfinderVms[2].flagsWord |= 2;
    camera->viewfinderVms[3].flagsWord |= 2;

    AnmManager::ExecuteScript(&camera->viewfinderVms[0]);
    AnmManager::ExecuteScript(&camera->viewfinderVms[1]);
    AnmManager::ExecuteScript(&camera->viewfinderVms[2]);
    AnmManager::ExecuteScript(&camera->viewfinderVms[3]);
    camera->modeTimer.Tick();
    camera->auxiliaryTimer.Tick();
}

} // namespace th095
