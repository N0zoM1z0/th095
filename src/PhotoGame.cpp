#include "PhotoCamera.hpp"
#include "AnmVmId.hpp"

namespace th095
{

void Rotate(Float3 *outVector, Float3 *point, f32 angle);

struct PhotoAnmVmIdValue
{
    i32 value;

    PhotoAnmVmIdValue(i32 value)
    {
        this->value = value;
    }
};

__forceinline i32 PhotoAnmVmId::operator==(PhotoAnmVmIdValue other) const
{
    return this->value == other.value;
}

struct PhotoResetTargetView
{
    void ResetForPhotoTransition();
};

struct PhotoGameGlobalStateView
{
    u8 unknown000[0xfc];
    union
    {
        u32 flags;
        struct
        {
            u32 blocksPlayerUpdate0 : 1;
            u32 blocksPlayerUpdate1 : 1;
            u32 blocksPlayerDraw : 1;
            u32 unknown003 : 7;
            u32 photoCaptureInputMode : 1;
            u32 unknown011 : 21;
        };
    };
};

extern PhotoResetTargetView *g_PhotoRuntimeResetTarget;
extern PhotoResetTargetView *g_PhotoBulletResetTarget;
extern PhotoResetTargetView *g_PhotoStageResetTarget;
extern PhotoGameGlobalStateView *g_PhotoGameGlobalState;
extern u16 g_PhotoInput;

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

struct PhotoGameStageStateView
{
    u8 unknown00000[0x2571c];
    PhotoAnmLoadedView *anm;
};

extern PhotoGameStageStateView *g_PhotoStageStateForPlayer;

struct PhotoGameAnmSpawnerView
{
    AnmVmId CreateVm(i32 scriptIndex, i32 renderMode);
};

struct PhotoGameSoundPlayerView
{
    void PlaySoundByIdx(i32 idx, i32 pan);
};

static inline PhotoGameSoundPlayerView *PhotoGameSoundPlayer()
{
    return reinterpret_cast<PhotoGameSoundPlayerView *>(&g_SoundPlayer);
}

struct PhotoPlayerMovementConfigView
{
    u16 unknown000;
    u16 shotPowerLevelCount;
    f32 hurtboxSize;
    f32 grazeSize;
    f32 unknown00c;
    f32 photoTargetSize;
    f32 normalAxisSpeed;
    f32 focusedAxisSpeed;
    f32 normalDiagonalSpeed;
    f32 focusedDiagonalSpeed;
    struct ShotPowerLevel
    {
        void *descriptors;
        i32 unknown004;
    } shotPowerLevels[1];
};

struct PhotoPlayerShotDescriptorView
{
    i16 fireInterval;
    u8 unknown002[0x38 - 2];
};

typedef char PhotoPlayerShotDescriptorSizeIs38[
    (sizeof(PhotoPlayerShotDescriptorView) == 0x38) ? 1 : -1];

typedef char PhotoPlayerNormalAxisSpeedAt14[
    (offsetof(PhotoPlayerMovementConfigView, normalAxisSpeed) == 0x14) ? 1 : -1];
typedef char PhotoPlayerFocusedDiagonalSpeedAt20[
    (offsetof(PhotoPlayerMovementConfigView, focusedDiagonalSpeed) == 0x20) ? 1 : -1];

enum PhotoPlayerMovementDirection
{
    PHOTO_PLAYER_DIRECTION_NONE = 0,
    PHOTO_PLAYER_DIRECTION_UP = 1,
    PHOTO_PLAYER_DIRECTION_DOWN = 2,
    PHOTO_PLAYER_DIRECTION_LEFT = 3,
    PHOTO_PLAYER_DIRECTION_RIGHT = 4,
    PHOTO_PLAYER_DIRECTION_UP_LEFT = 5,
    PHOTO_PLAYER_DIRECTION_UP_RIGHT = 6,
    PHOTO_PLAYER_DIRECTION_DOWN_LEFT = 7,
    PHOTO_PLAYER_DIRECTION_DOWN_RIGHT = 8,
};

static inline u16 PhotoGameInputMask(u16 input, u16 mask)
{
    return input & mask;
}

struct PhotoPlayerFrameStateView
{
    ZunTimer timer;
    u8 unknown00c[0x34 - 0x0c];
};

typedef char PhotoPlayerFrameStateSizeIs34[
    (sizeof(PhotoPlayerFrameStateView) == 0x34) ? 1 : -1];

struct PhotoGameUpdateView
{
    i32 mode;
    PhotoAnmLoadedView *effectAnm;
    AnmVm effectVm;
    i32 movementState;
    i32 cameraTrackingMode;
    Float3 previousPosition;
    Float3 positionHistory[16];
    Float3 hurtboxBoundsMin;
    Float3 hurtboxBoundsMax;
    Float3 grazeBoundsMin;
    Float3 grazeBoundsMax;
    Float3 hurtboxHalfSize;
    Float3 grazeHalfSize;
    Float3 photoTargetHalfSize;
    Float3 velocity;
    f32 currentHorizontalSpeed;
    f32 currentVerticalSpeed;
    PhotoPlayerMovementConfigView *movementConfig;
    ZunTimer stateTimer;
    ZunTimer completionTimer;
    PhotoPlayerFrameStateView frameStates[128];
    PhotoAnmVmId focusVm;
    Float3 playerPosition;
    PhotoCameraState camera;
    f32 cameraUpdateScale;
    ChainElem *calcChain;
    ChainElem *drawPlayerChain;
    ChainElem *drawCameraChain;
    Float3 photoTargetBoundsMin;
    Float3 photoTargetBoundsMax;

    PhotoGameUpdateView();
    ~PhotoGameUpdateView();
    i32 Initialize();
    i32 LoadSht(char *path);
    f32 AngleFromPoint(Float3 *position);
    i32 CheckBulletCollision(Float3 *position, Float3 *size);
    u32 CalcLaserHitbox(Float3 *origin, f32 angle, f32 width, f32 length);
    void Die();
    i32 UpdateMainState();
    i32 Update();
    i32 DrawPlayer();
    i32 DrawCamera();

    static PhotoGameUpdateView *Create();
    void Destroy();
    static i32 __fastcall OnUpdate(PhotoGameUpdateView *player);
    static i32 __fastcall OnDrawPlayer(PhotoGameUpdateView *player);
    static i32 __fastcall OnDrawCamera(PhotoGameUpdateView *player);
};

extern PhotoGameUpdateView *g_PhotoGame;

typedef char PhotoGameUpdateHistoryAt2E8[
    (offsetof(PhotoGameUpdateView, positionHistory) == 0x02e8) ? 1 : -1];
typedef char PhotoGameUpdateBoundsAt3A8[
    (offsetof(PhotoGameUpdateView, hurtboxBoundsMin) == 0x03a8) ? 1 : -1];
typedef char PhotoGameUpdateVelocityAt3FC[
    (offsetof(PhotoGameUpdateView, velocity) == 0x03fc) ? 1 : -1];
typedef char PhotoGameUpdateMovementConfigAt410[
    (offsetof(PhotoGameUpdateView, movementConfig) == 0x0410) ? 1 : -1];
typedef char PhotoGameUpdateStateTimerAt414[
    (offsetof(PhotoGameUpdateView, stateTimer) == 0x0414) ? 1 : -1];
typedef char PhotoGameUpdateFrameStatesAt42C[
    (offsetof(PhotoGameUpdateView, frameStates) == 0x042c) ? 1 : -1];
typedef char PhotoGameUpdateFocusVmAt1E2C[
    (offsetof(PhotoGameUpdateView, focusVm) == 0x1e2c) ? 1 : -1];
typedef char PhotoGameUpdateCameraAt1E3C[
    (offsetof(PhotoGameUpdateView, camera) == 0x1e3c) ? 1 : -1];
typedef char PhotoGameUpdateScaleAt2A18[
    (offsetof(PhotoGameUpdateView, cameraUpdateScale) == 0x2a18) ? 1 : -1];
typedef char PhotoGameUpdatePhotoBoundsAt2A28[
    (offsetof(PhotoGameUpdateView, photoTargetBoundsMin) == 0x2a28) ? 1 : -1];
typedef char PhotoGameUpdateSizeIs2A40[
    (sizeof(PhotoGameUpdateView) == 0x2a40) ? 1 : -1];

PhotoCameraState::PhotoCameraState()
{
}

PhotoCameraState::~PhotoCameraState()
{
}

void PhotoCameraState::Initialize()
{
    memset(this, 0, sizeof(*this));
    this->vmIds[0] = g_PhotoStageStateForPlayer->anm->CreateVm(0x16, 0);
    this->vmIds[1] = g_PhotoStageStateForPlayer->anm->CreateVm(0x17, 0);
    this->viewfinderPosition = g_PhotoGame->playerPosition;
    this->viewfinderPosition.x = 0.0f;
    this->viewfinderPosition.y = 400.0f;
    this->charge = 0.5f;
    this->photoLimit = 0;
    this->flags = (this->flags & ~0x18) | 0x10;
    this->captureRequested = 1;
    this->trackingRadius = 56.0f;

    g_PhotoStageStateForPlayer->anm->InitializeVm(&this->viewfinderVms[0], 0x24);
    g_PhotoStageStateForPlayer->anm->InitializeVm(&this->viewfinderVms[1], 0x24);
    g_PhotoStageStateForPlayer->anm->InitializeVm(&this->viewfinderVms[2], 0x24);
    g_PhotoStageStateForPlayer->anm->InitializeVm(&this->viewfinderVms[3], 0x24);
    g_PhotoStageStateForPlayer->anm->SetSprite(&this->viewfinderVms[0], 0x10);
    g_PhotoStageStateForPlayer->anm->SetSprite(&this->viewfinderVms[1], 0xf);
    g_PhotoStageStateForPlayer->anm->SetSprite(&this->viewfinderVms[2], 0xf);
    g_PhotoStageStateForPlayer->anm->SetSprite(&this->viewfinderVms[3], 0x19);

    this->viewfinderVms[0].position = Float3(-36.0f, -12.0f, 0.0f);
    this->viewfinderVms[1].position = Float3(-27.0f, -12.0f, 0.0f);
    this->viewfinderVms[2].position = Float3(-18.0f, -12.0f, 0.0f);
    this->viewfinderVms[3].position = Float3(-9.0f, -12.0f, 0.0f);
    this->trackingAngle = -1.5707964f;
}

PhotoGameUpdateView::PhotoGameUpdateView()
{
    utils::DebugPrint("initialize PlayerInf\n");
    memset(this, 0, sizeof(*this));
    g_PhotoGame = this;
}

PhotoGameUpdateView::~PhotoGameUpdateView()
{
    utils::DebugPrint("shutdown PlayerInf\n");
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawPlayerChain);
    g_Chain.Cut(this->drawCameraChain);
    g_PhotoGame = NULL;
    g_AnmManager->MarkVmsForDeletion(
        reinterpret_cast<AnmLoaded *>(this->effectAnm));
    if (this->movementConfig != NULL)
    {
        free(this->movementConfig);
    }
}

i32 PhotoGameUpdateView::Initialize()
{
    this->effectAnm = reinterpret_cast<PhotoAnmLoadedView *>(
        g_AnmManager->LoadAnm(7, "player.anm"));
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
    this->stateTimer = -1;
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
    if (g_AnmManager->LoadAnm(7, "player.anm") == NULL)
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

f32 PhotoGameUpdateView::AngleFromPoint(Float3 *position)
{
    f32 xDelta = this->playerPosition.x - position->x;
    f32 yDelta = this->playerPosition.y - position->y;

    if (yDelta == 0.0f && xDelta == 0.0f)
    {
        return 1.5707964f;
    }
    return atan2f(yDelta, xDelta);
}

i32 PhotoGameUpdateView::CheckBulletCollision(Float3 *position, Float3 *size)
{
    Float3 boundsMin;
    Float3 boundsMax;

    boundsMin.x = position->x - size->x / 2.0f;
    boundsMin.y = position->y - size->y / 2.0f;
    boundsMax.x = size->x / 2.0f + position->x;
    boundsMax.y = size->y / 2.0f + position->y;

    if (this->hurtboxBoundsMin.x > boundsMax.x ||
        this->hurtboxBoundsMin.y > boundsMax.y ||
        this->hurtboxBoundsMax.x < boundsMin.x ||
        this->hurtboxBoundsMax.y < boundsMin.y)
    {
        return 0;
    }
    if (this->mode == 2)
    {
        return 0;
    }
    if (this->mode == 3)
    {
        return 0;
    }
    this->Die();
    return 1;
}

u32 PhotoGameUpdateView::CalcLaserHitbox(
    Float3 *origin, f32 angle, f32 width, f32 length)
{
    Float3 incomingMin;
    Float3 incomingMax;

    incomingMin = this->playerPosition - *origin;
    Rotate(&incomingMax, &incomingMin, -angle);
    incomingMin = incomingMax - this->hurtboxHalfSize;
    incomingMax = incomingMax + this->hurtboxHalfSize;

    if (incomingMin.x > length ||
        width / 2.0f < incomingMin.y ||
        incomingMax.x < 0.0f ||
        incomingMax.y < -width / 2.0f)
    {
        return 0;
    }
    if (this->mode == 2)
    {
        return 0;
    }
    if (this->mode == 3)
    {
        return 0;
    }
    this->Die();
    return 1;
}

void PhotoGameUpdateView::Die()
{
    Float3 screenPosition;

    this->mode = 2;
    PhotoToScreen(&screenPosition, &this->playerPosition);
    g_AnmManager->SetPosition(
        reinterpret_cast<PhotoGameAnmSpawnerView *>(
            g_PhotoBulletManager->anmSpawner)->CreateVm(0x121, 0),
        &screenPosition);
    for (i32 i = 0; i < 32; ++i)
    {
        g_AnmManager->SetPosition(
            reinterpret_cast<PhotoGameAnmSpawnerView *>(
                g_PhotoBulletManager->anmSpawner)->CreateVm(0x122, 0),
            &screenPosition);
    }
    this->completionTimer = 0;
    if ((g_PhotoGameGlobalState->flags >> 9 & 1) == 0)
    {
        PhotoGameSoundPlayer()->PlaySoundByIdx(4, 0);
    }
    g_AnmGameSpeed = 0.5f;
}

i32 PhotoGameUpdateView::DrawPlayer()
{
    if (this->mode == 2)
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

i32 __fastcall PhotoGameUpdateView::OnUpdate(PhotoGameUpdateView *player)
{
    if ((g_PhotoGameGlobalState->blocksPlayerDraw |
         g_PhotoGameGlobalState->blocksPlayerUpdate0) != 0 ||
        g_PhotoGameGlobalState->blocksPlayerUpdate1 != 0)
    {
        return 1;
    }

    if (g_PhotoGameGlobalState->photoCaptureInputMode != 0)
    {
        if (PhotoGameInputMask(g_PhotoInput, 2) != 0)
        {
            player->camera.captureRequested = 1;
        }
        return 1;
    }
    return player->Update();
}

i32 __fastcall PhotoGameUpdateView::OnDrawPlayer(PhotoGameUpdateView *player)
{
    if (g_PhotoGameGlobalState->blocksPlayerDraw != 0)
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
        if (PhotoGameInputMask(g_PhotoInput, 0x50) == 0x50)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_UP_LEFT;
        }
        else if (PhotoGameInputMask(g_PhotoInput, 0x60) == 0x60)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_DOWN_LEFT;
        }
        else if (PhotoGameInputMask(g_PhotoInput, 0x90) == 0x90)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_UP_RIGHT;
        }
        else if (PhotoGameInputMask(g_PhotoInput, 0xa0) == 0xa0)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_DOWN_RIGHT;
        }
        else if (PhotoGameInputMask(g_PhotoInput, 0x20) != 0)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_DOWN;
        }
        else if (PhotoGameInputMask(g_PhotoInput, 0x10) != 0)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_UP;
        }
        else if (PhotoGameInputMask(g_PhotoInput, 0x40) != 0)
        {
            this->movementState = PHOTO_PLAYER_DIRECTION_LEFT;
        }
        else if (PhotoGameInputMask(g_PhotoInput, 0x80) != 0)
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

    this->cameraTrackingMode = PhotoGameInputMask(g_PhotoInput, 1) != 0;

    if (this->cameraTrackingMode != 0)
    {
        if (this->focusVm == PhotoAnmVmIdValue(0))
        {
            this->focusVm =
                reinterpret_cast<PhotoAnmLoadedView *>(
                    g_PhotoBulletManager->anmSpawner)->CreateVm(0x11f, 6);
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

        if (PhotoGameInputMask(g_PhotoInput, 2) != 0)
        {
            verticalSpeed *= 0.22f;
            horizontalSpeed *= 0.22f;
            this->cameraTrackingMode = 2;
        }
    }
    else
    {
        if (this->focusVm != 0)
        {
            g_AnmManager->SetInterrupt(
                *reinterpret_cast<AnmVmId *>(&this->focusVm), 1);
            PhotoAnmVmId clearedVm;
            clearedVm = 0;
            this->focusVm = clearedVm;
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

    horizontalSpeed *= this->cameraUpdateScale;
    verticalSpeed *= this->cameraUpdateScale;

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
    case 0:
        this->playerPosition.y =
            -80.0f * static_cast<f32>(this->completionTimer) / 60.0f +
            480.0f;
        if (this->completionTimer >= 60)
        {
            this->mode = 1;
            this->completionTimer = 0;
        }
        else
        {
            break;
        }

    case 1:
        this->UpdateMainState();
        UpdatePhotoCamera(&this->camera);
        break;

    case 2:
        if (this->completionTimer >= 30)
        {
            g_PhotoGameGlobalState->flags |= 0x20;
            g_AnmGameSpeed = 1.0f;
        }
        break;

    case 3:
        if (this->completionTimer == 4)
        {
            g_PhotoRuntimeResetTarget->ResetForPhotoTransition();
        }
        else if (this->completionTimer == 15)
        {
            g_PhotoBulletResetTarget->ResetForPhotoTransition();
            g_PhotoStageResetTarget->ResetForPhotoTransition();
        }
        else if (this->completionTimer == 30)
        {
            g_PhotoGameGlobalState->flags |= 0x40;
        }
        break;
    }

    this->cameraUpdateScale = 1.0f;
    AnmManager::ExecuteScript(&this->effectVm);
    this->completionTimer.Tick();
    return 1;
}

} // namespace th095
