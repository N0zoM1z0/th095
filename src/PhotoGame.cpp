#include "ControllerInputSlot.hpp"
#include "PhotoGamePlayer.hpp"
#include "Global.hpp"
#include "PhotoStage.hpp"
#include "PhotoBulletManager.hpp"
#include "AnmVmId.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoGameTask.hpp"
#include "PhotoEffectRuntime.hpp"
#include "PhotoEnemyManager.hpp"
#include "SoundPlayer.hpp"

namespace th095
{

struct PhotoAnmVmIdValue
{
    i32 value;

    PhotoAnmVmIdValue(i32 value)
    {
        this->value = value;
    }
};

static __forceinline i32 PhotoGameFocusVmIsZero(const PhotoAnmVmId *vm)
{
    return vm->value == PhotoAnmVmIdValue(0).value;
}

static __forceinline void PhotoGameClearFocusVm(PhotoAnmVmId *vm)
{
    PhotoAnmVmId clearedVm;
    clearedVm = 0;
    *vm = clearedVm;
}

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

static inline u16 PhotoGameInputMask(u16 input, u16 mask)
{
    return input & mask;
}

PhotoCameraState::PhotoCameraState()
{
}

static __forceinline void PhotoCameraInitializeViewfinderPhase(AnmVm *vm)
{
    // Exact PhotoItem/Bullet InitializeVm frontends independently prove this
    // 0x2C VC7.1 allocation phase.  A single live VM pointer is essential:
    // extra explicit parameters add a non-target dword to each inline phase.
    u8 compilerStorage[0x2c];
    g_PhotoStageState->anm->InitializeVm(vm, 0x24);
}

void PhotoCameraState::Initialize()
{
    memset(this, 0, sizeof(*this));
    this->vmIds[0] = TH095_PHOTO_ANM_CREATE_VM(
        g_PhotoStageState->anm, 0x16, 0);
    this->vmIds[1] = TH095_PHOTO_ANM_CREATE_VM(
        g_PhotoStageState->anm, 0x17, 0);
    this->viewfinderPosition = g_RuntimePlayerOwner->playerPosition;
    this->viewfinderPosition.x = 0.0f;
    this->viewfinderPosition.y = 400.0f;
    this->charge = 0.5f;
    this->photoLimit = 0;
    this->chargeUiState = PHOTO_CAMERA_CHARGE_UI_INITIAL;
    this->captureRequested = 1;
    this->trackingRadius = 56.0f;

    PhotoCameraInitializeViewfinderPhase(&this->viewfinderVms[0]);
    PhotoCameraInitializeViewfinderPhase(&this->viewfinderVms[1]);
    PhotoCameraInitializeViewfinderPhase(&this->viewfinderVms[2]);
    PhotoCameraInitializeViewfinderPhase(&this->viewfinderVms[3]);
    g_PhotoStageState->anm->SetSprite(&this->viewfinderVms[0], 0x10);
    g_PhotoStageState->anm->SetSprite(&this->viewfinderVms[1], 0xf);
    g_PhotoStageState->anm->SetSprite(&this->viewfinderVms[2], 0xf);
    g_PhotoStageState->anm->SetSprite(&this->viewfinderVms[3], 0x19);

    this->viewfinderVms[0].position = Float3(-36.0f, -12.0f, 0.0f);
    this->viewfinderVms[1].position = Float3(-27.0f, -12.0f, 0.0f);
    this->viewfinderVms[2].position = Float3(-18.0f, -12.0f, 0.0f);
    this->viewfinderVms[3].position = Float3(-9.0f, -12.0f, 0.0f);
    this->trackingAngle = -1.5707964f;
}

static __forceinline void PhotoGameConstructorBodyPhase(
    PhotoGameUpdateView *view)
{
    u8 compilerStorage[0x0c];
    utils::DebugPrint("initialize PlayerInf\n");
    memset(view, 0, sizeof(*view));
    g_RuntimePlayerOwner = view;
}

PhotoGameUpdateView::PhotoGameUpdateView()
{
    PhotoGameConstructorBodyPhase(this);
}

PhotoGameUpdateView::~PhotoGameUpdateView()
{
    utils::DebugPrint("shutdown PlayerInf\n");
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawPlayerChain);
    g_Chain.Cut(this->drawCameraChain);
    g_RuntimePlayerOwner = NULL;
    g_AnmManager->MarkVmsForDeletion(
        reinterpret_cast<AnmLoaded *>(this->effectAnm));
    if (this->movementConfig != NULL)
    {
        PhotoPlayerMovementConfigView *movementConfig = this->movementConfig;
        free(movementConfig);
    }
}

static __forceinline void PhotoGameSetStateTimerPhase(ZunTimer *timer)
{
    u8 compilerStorage[0x2c];
    *timer = -1;
}

i32 PhotoGameUpdateView::Initialize()
{
    this->effectAnm = reinterpret_cast<PhotoAnmLoadedView *>(
        g_AnmManager->PreloadAnm(7, "player.anm"));
    if (this->effectAnm == NULL)
    {
        g_GameErrorContext.Log(
            "\x8e\xa9\x8b\x40\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9"
            "\x82\xf1\x81\x42\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7"
            "\r\n");
        return ZUN_ERROR;
    }

    if (this->LoadSht("player.sht") != ZUN_SUCCESS)
    {
        g_GameErrorContext.Log(
            "\x8e\xa9\x8b\x40\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9"
            "\x82\xf1\x81\x42\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7"
            "\r\n");
        return ZUN_ERROR;
    }

    this->camera.Initialize();
    this->effectAnm->InitializeVm(&this->effectVm, 0);
    this->playerPosition.x = 0.0f;
    this->playerPosition.y = 400.0f;
    PhotoGameSetStateTimerPhase(&this->stateTimer);
    this->hurtboxHalfSize.x =
        this->hurtboxHalfSize.y = this->movementConfig->hurtboxSize / 2.0f;
    this->hurtboxHalfSize.z = 5.0f;
    this->grazeHalfSize.x =
        this->grazeHalfSize.y = this->movementConfig->grazeSize / 2.0f;
    this->grazeHalfSize.z = 5.0f;
    this->photoTargetHalfSize.x =
        this->photoTargetHalfSize.y =
            this->movementConfig->photoTargetSize / 2.0f;
    this->photoTargetHalfSize.z = 5.0f;
    return ZUN_SUCCESS;
}

i32 LoadPhotoPlayerAnm()
{
    if (g_AnmManager->PreloadAnm(7, "player.anm") == NULL)
    {
        g_GameErrorContext.Log(
            "\x8e\xa9\x8b\x40\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9"
            "\x82\xf1\x81\x42\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7"
            "\r\n");
        return ZUN_ERROR;
    }
    return ZUN_SUCCESS;
}

i32 ReleasePhotoPlayerAnm()
{
    g_AnmManager->ReleaseAnm(7);
    return ZUN_SUCCESS;
}

PhotoGameUpdateView *PhotoGameUpdateView::Create()
{
    struct
    {
        PhotoGameUpdateView *player;
        ChainElem *elem;
    } locals;

#define player locals.player
#define elem locals.elem

    player = new PhotoGameUpdateView();
    if (player->Initialize() != ZUN_SUCCESS)
    {
        goto failure;
    }

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoGameUpdateView::OnUpdate));
    elem->arg = player;
    g_Chain.AddToCalcChain(elem, 0xb);
    player->calcChain = elem;

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoGameUpdateView::OnDrawPlayer));
    elem->arg = player;
    g_Chain.AddToDrawChain(elem, 0xb);
    player->drawPlayerChain = elem;

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoGameUpdateView::OnDrawCamera));
    elem->arg = player;
    g_Chain.AddToDrawChain(elem, 0x12);
    player->drawCameraChain = elem;
    return player;

failure:
    if (player != NULL)
    {
        delete player;
        player = NULL;
    }
#undef elem
#undef player
    return NULL;
}

void PhotoGameUpdateView::Destroy()
{
    PhotoGameUpdateView *player = this;
    if (player != NULL)
    {
        delete player;
        player = NULL;
    }
}

i32 PhotoGameUpdateView::LoadSht(char *path)
{
    struct
    {
        f32 focusedDiagonalFactor;
        f32 normalDiagonalFactor;
        PhotoPlayerShotDescriptorView *descriptor;
        i32 powerLevelIndex;
    } locals;

#define focusedDiagonalFactor locals.focusedDiagonalFactor
#define normalDiagonalFactor locals.normalDiagonalFactor
#define descriptor locals.descriptor
#define powerLevelIndex locals.powerLevelIndex

    this->movementConfig = reinterpret_cast<PhotoPlayerMovementConfigView *>(
        FileSystem::OpenFile(path, NULL, 0));
    if (this->movementConfig == NULL)
    {
        return ZUN_ERROR;
    }

    normalDiagonalFactor = static_cast<f32>(cos(0.7853982f));
    this->movementConfig->normalDiagonalSpeed = normalDiagonalFactor *
                                                this->movementConfig->normalAxisSpeed;
    focusedDiagonalFactor = static_cast<f32>(cos(0.7853982f));
    this->movementConfig->focusedDiagonalSpeed = focusedDiagonalFactor *
                                                 this->movementConfig->focusedAxisSpeed;

    for (powerLevelIndex = 0;
         powerLevelIndex < this->movementConfig->shotPowerLevelCount;
         ++powerLevelIndex)
    {
        reinterpret_cast<u32 &>(
            this->movementConfig->shotPowerLevels[powerLevelIndex].descriptors) +=
            reinterpret_cast<u32>(this->movementConfig);
        descriptor = reinterpret_cast<PhotoPlayerShotDescriptorView *>(
            this->movementConfig->shotPowerLevels[powerLevelIndex].descriptors);
        while (descriptor->fireInterval >= 0)
        {
            ++descriptor;
        }
    }
#undef powerLevelIndex
#undef descriptor
#undef normalDiagonalFactor
#undef focusedDiagonalFactor
    return ZUN_SUCCESS;
}

#include "PhotoGamePlayerCollision.inl"

void PhotoGameUpdateView::Die()
{
    Float3 screenPosition;

    this->mode = PHOTO_PLAYER_MODE_DEATH_TRANSITION;
    PhotoToScreen(&screenPosition, &this->playerPosition);
    g_AnmManager->SetPosition(
        g_PhotoBulletManager->bulletAnm->CreateVm(0x121, 0),
        &screenPosition);
    for (i32 i = 0; i < 32; ++i)
    {
        g_AnmManager->SetPosition(
            g_PhotoBulletManager->bulletAnm->CreateVm(0x122, 0),
            &screenPosition);
    }
    this->completionTimer = 0;
    if (g_RuntimeGlobalStateOwner->photoSoundSuppressed == 0)
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_PICHUN, 0);
    }
    g_AnmGameSpeed = 0.5f;
}

i32 PhotoGameUpdateView::DrawPlayer()
{
    if (this->mode == PHOTO_PLAYER_MODE_DEATH_TRANSITION)
    {
        return 1;
    }

    PhotoToScreen(&this->effectVm.positionOffset, &this->playerPosition);
    g_AnmManager->Draw(&this->effectVm);
    return 1;
}

i32 PhotoGameUpdateView::DrawCamera()
{
    this->camera.Draw();
    return 1;
}

static __forceinline i32 PhotoGameEitherFlag(i32 first, i32 second)
{
    return first | second;
}

i32 __fastcall PhotoGameUpdateView::OnUpdate(PhotoGameUpdateView *player)
{
    if (PhotoGameEitherFlag(g_RuntimeGlobalStateOwner->captureActive,
                            g_RuntimeGlobalStateOwner->gameplayLoadActive) != 0 ||
        g_RuntimeGlobalStateOwner->capturedPhotoActive != 0)
    {
        return 1;
    }

    if (g_RuntimeGlobalStateOwner->photoTransitionActive != 0)
    {
        if (PhotoGameInputMask(g_ControllerInputSlots[0].replayHistory.pressed, 2) != 0)
        {
            player->camera.captureRequested = 1;
        }
        return 1;
    }
    return player->Update();
}

i32 __fastcall PhotoGameUpdateView::OnDrawPlayer(PhotoGameUpdateView *player)
{
    if (g_RuntimeGlobalStateOwner->gameplayLoadActive != 0)
    {
        return 1;
    }
    return player->DrawPlayer();
}

i32 __fastcall PhotoGameUpdateView::OnDrawCamera(PhotoGameUpdateView *player)
{
    return player->DrawCamera();
}

i32 PhotoGameUpdateView::UpdateMainState()
{
    struct
    {
        i32 historyIndex;
        Float3 screenPosition;
        f32 horizontalSpeed;
        f32 verticalSpeed;
        i32 oldDirection;
    } locals;

#define historyIndex locals.historyIndex
#define screenPosition locals.screenPosition
#define horizontalSpeed locals.horizontalSpeed
#define verticalSpeed locals.verticalSpeed
#define oldDirection locals.oldDirection

    horizontalSpeed = 0.0f;
    verticalSpeed = 0.0f;
    oldDirection = this->movementState;

    if (this->camera.mode != PHOTO_CAMERA_CHARGING)
    {
        if (PhotoGameInputMask(g_ControllerInputSlots[0].replayHistory.current, TH_BUTTON_UP_LEFT) == TH_BUTTON_UP_LEFT)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_UP_LEFT;
        }
        else if (PhotoGameInputMask(g_ControllerInputSlots[0].replayHistory.current, TH_BUTTON_DOWN_LEFT) == TH_BUTTON_DOWN_LEFT)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_DOWN_LEFT;
        }
        else if (PhotoGameInputMask(g_ControllerInputSlots[0].replayHistory.current, TH_BUTTON_UP_RIGHT) == TH_BUTTON_UP_RIGHT)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_UP_RIGHT;
        }
        else if (PhotoGameInputMask(g_ControllerInputSlots[0].replayHistory.current, TH_BUTTON_DOWN_RIGHT) == TH_BUTTON_DOWN_RIGHT)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_DOWN_RIGHT;
        }
        else if (PhotoGameInputMask(g_ControllerInputSlots[0].replayHistory.current, TH_BUTTON_DOWN) != 0)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_DOWN;
        }
        else if (PhotoGameInputMask(g_ControllerInputSlots[0].replayHistory.current, TH_BUTTON_UP) != 0)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_UP;
        }
        else if (PhotoGameInputMask(g_ControllerInputSlots[0].replayHistory.current, TH_BUTTON_LEFT) != 0)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_LEFT;
        }
        else if (PhotoGameInputMask(g_ControllerInputSlots[0].replayHistory.current, TH_BUTTON_RIGHT) != 0)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_RIGHT;
        }
        else
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_NONE;
        }
    }
    else
    {
        this->movementState = PHOTO_PLAYER_DIRECTION_NONE;
    }

    this->cameraTrackingMode = static_cast<PhotoPlayerCameraTrackingMode>(
        PhotoGameInputMask(g_ControllerInputSlots[0].replayHistory.current, TH_BUTTON_SHOOT) != 0);

    if (this->cameraTrackingMode != PHOTO_PLAYER_CAMERA_TRACKING_FREE)
    {
        if (PhotoGameFocusVmIsZero(&this->focusVm))
        {
            this->focusVm = TH095_PHOTO_ANM_CREATE_VM(
                g_PhotoBulletManager->bulletAnm,
                0x11f, 6);
        }

        switch (this->movementState)
        {
        case PHOTO_PLAYER_DIRECTION_RIGHT:
            horizontalSpeed = this->movementConfig->focusedAxisSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_LEFT:
            horizontalSpeed = -this->movementConfig->focusedAxisSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_UP:
            verticalSpeed = -this->movementConfig->focusedAxisSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_DOWN:
            verticalSpeed = this->movementConfig->focusedAxisSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_UP_LEFT:
            horizontalSpeed = -this->movementConfig->focusedDiagonalSpeed;
            verticalSpeed = horizontalSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_DOWN_LEFT:
            verticalSpeed = this->movementConfig->focusedDiagonalSpeed;
            horizontalSpeed = -verticalSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_UP_RIGHT:
            horizontalSpeed = this->movementConfig->focusedDiagonalSpeed;
            verticalSpeed = -horizontalSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_DOWN_RIGHT:
            horizontalSpeed = this->movementConfig->focusedDiagonalSpeed;
            verticalSpeed = horizontalSpeed;
            break;
        default:
            break;
        }

        if (PhotoGameInputMask(g_ControllerInputSlots[0].replayHistory.current, TH_BUTTON_BOMB) != 0)
        {
            verticalSpeed *= 0.22f;
            horizontalSpeed *= 0.22f;
            this->cameraTrackingMode = PHOTO_PLAYER_CAMERA_TRACKING_TARGET_SLOW;
        }
    }
    else
    {
        if (this->focusVm != 0)
        {
            g_AnmManager->SetInterrupt(
                *reinterpret_cast<AnmVmId *>(&this->focusVm), 1);
            PhotoGameClearFocusVm(&this->focusVm);
        }

        switch (this->movementState)
        {
        case PHOTO_PLAYER_DIRECTION_RIGHT:
            horizontalSpeed = this->movementConfig->normalAxisSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_LEFT:
            horizontalSpeed = -this->movementConfig->normalAxisSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_UP:
            verticalSpeed = -this->movementConfig->normalAxisSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_DOWN:
            verticalSpeed = this->movementConfig->normalAxisSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_UP_LEFT:
            horizontalSpeed = -this->movementConfig->normalDiagonalSpeed;
            verticalSpeed = horizontalSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_DOWN_LEFT:
            verticalSpeed = this->movementConfig->normalDiagonalSpeed;
            horizontalSpeed = -verticalSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_UP_RIGHT:
            horizontalSpeed = this->movementConfig->normalDiagonalSpeed;
            verticalSpeed = -horizontalSpeed;
            break;
        case PHOTO_PLAYER_DIRECTION_DOWN_RIGHT:
            horizontalSpeed = this->movementConfig->normalDiagonalSpeed;
            verticalSpeed = horizontalSpeed;
            break;
        default:
            break;
        }
    }

    horizontalSpeed *= this->movementScale;
    verticalSpeed *= this->movementScale;

#define SET_PHOTO_PLAYER_SCRIPT(idx)                                            \
    this->effectAnm->SetAndExecuteScriptIdx(&this->effectVm, (idx))

    if (horizontalSpeed < 0.0f && this->currentHorizontalSpeed >= 0.0f)
        SET_PHOTO_PLAYER_SCRIPT(1);
    else if (horizontalSpeed == 0.0f && this->currentHorizontalSpeed < 0.0f)
        SET_PHOTO_PLAYER_SCRIPT(2);
    if (horizontalSpeed > 0.0f && this->currentHorizontalSpeed <= 0.0f)
        SET_PHOTO_PLAYER_SCRIPT(3);
    else if (horizontalSpeed == 0.0f && this->currentHorizontalSpeed > 0.0f)
        SET_PHOTO_PLAYER_SCRIPT(4);

#undef SET_PHOTO_PLAYER_SCRIPT

    this->currentHorizontalSpeed = horizontalSpeed;
    this->currentVerticalSpeed = verticalSpeed;
    this->velocity.x = horizontalSpeed * g_AnmGameSpeed;
    this->velocity.y = verticalSpeed * g_AnmGameSpeed;
    this->playerPosition.operator f32 *()[0] += this->velocity.x;
    this->playerPosition.operator f32 *()[1] += this->velocity.y;

    this->playerPosition.x =
        static_cast<f32>(floor(this->playerPosition.x * 100.0f) / 100.0f);
    this->playerPosition.y =
        static_cast<f32>(floor(this->playerPosition.y * 100.0f) / 100.0f);

    if (this->playerPosition.x < -184.0)
        this->playerPosition.x = -184.0f;
    else if (this->playerPosition.x > 184.0)
        this->playerPosition.x = 184.0f;
    if (this->playerPosition.y < 32.0f)
        this->playerPosition.y = 32.0f;
    else if (this->playerPosition.y > 436.0f)
        this->playerPosition.y = 436.0f;

    this->hurtboxBoundsMin = this->playerPosition - this->hurtboxHalfSize;
    this->hurtboxBoundsMax = this->playerPosition + this->hurtboxHalfSize;
    this->grazeBoundsMin = this->playerPosition - this->grazeHalfSize;
    this->grazeBoundsMax = this->playerPosition + this->grazeHalfSize;
    this->photoTargetBoundsMin =
        this->playerPosition - this->photoTargetHalfSize;
    this->photoTargetBoundsMax =
        this->playerPosition + this->photoTargetHalfSize;

    if (this->focusVm != 0)
    {
        PhotoToScreen(&screenPosition, &this->playerPosition);
        g_AnmManager->SetPosition(
            *reinterpret_cast<AnmVmId *>(&this->focusVm), &screenPosition);
    }

    if (verticalSpeed != 0.0f || horizontalSpeed != 0.0f)
    {
        for (historyIndex = 15; historyIndex > 0; --historyIndex)
            this->positionHistory[historyIndex] =
                this->positionHistory[historyIndex - 1];
        this->positionHistory[0] = this->playerPosition;
    }

#undef oldDirection
#undef verticalSpeed
#undef horizontalSpeed
#undef screenPosition
#undef historyIndex

    return 0;
}

i32 PhotoGameUpdateView::Update()
{
    switch (this->mode)
    {
    case PHOTO_PLAYER_MODE_ENTERING:
        this->playerPosition.y =
            -80.0f * static_cast<f32>(this->completionTimer) / 60.0f +
            480.0f;
        if (this->completionTimer >= 60)
        {
            this->mode = PHOTO_PLAYER_MODE_ACTIVE;
            this->completionTimer = 0;
        }
        else
        {
            break;
        }

    case PHOTO_PLAYER_MODE_ACTIVE:
        this->UpdateMainState();
        UpdatePhotoCamera(&this->camera);
        break;

    case PHOTO_PLAYER_MODE_DEATH_TRANSITION:
        if (this->completionTimer >= 30)
        {
            g_RuntimeGlobalStateOwner->playerDeathTransitionComplete = 1;
            g_AnmGameSpeed = 1.0f;
        }
        break;

    case PHOTO_PLAYER_MODE_PHOTO_LIMIT_TRANSITION:
        if (this->completionTimer == 4)
        {
            PhotoEnemyManagerView::ResetNonPhotoTargetsAndPhotoTargetEcls(
                g_PhotoEnemyManager);
        }
        else if (this->completionTimer == 15)
        {
            g_PhotoBulletManager->DespawnAllBullets();
            PhotoEffectManagerView::DrawSecondary(g_PhotoEffectManager);
        }
        else if (this->completionTimer == 30)
        {
            g_RuntimeGlobalStateOwner->photoLimitTransitionComplete = 1;
        }
        break;
    }

    this->movementScale = 1.0f;
    AnmManager::ExecuteScript(&this->effectVm);
    this->completionTimer.Tick();
    return 1;
}

} // namespace th095
