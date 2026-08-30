#include "PhotoCamera.hpp"
#include "AnmVmId.hpp"

namespace th095
{

struct PhotoResetTargetView
{
    void ResetForPhotoTransition();
};

struct PhotoGameGlobalStateView
{
    u8 unknown000[0xfc];
    u32 flags;
};

extern PhotoResetTargetView *g_PhotoRuntimeResetTarget;
extern PhotoResetTargetView *g_PhotoBulletResetTarget;
extern PhotoResetTargetView *g_PhotoStageResetTarget;
extern PhotoGameGlobalStateView *g_PhotoGameGlobalState;
extern u16 g_PhotoInput;

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

struct PhotoPlayerMovementConfigView
{
    u8 unknown000[0x14];
    f32 normalAxisSpeed;
    f32 focusedAxisSpeed;
    f32 normalDiagonalSpeed;
    f32 focusedDiagonalSpeed;
};

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
    u8 unknown0414[0x0420 - 0x0414];
    ZunTimer completionTimer;
    u8 unknown042c[0x1e2c - 0x042c];
    PhotoAnmVmId focusVm;
    Float3 playerPosition;
    PhotoCameraState camera;
    f32 cameraUpdateScale;
    u8 unknown2a1c[0x2a28 - 0x2a1c];
    Float3 photoTargetBoundsMin;
    Float3 photoTargetBoundsMax;

    i32 UpdateMainState();
    i32 Update();
};

typedef char PhotoGameUpdateHistoryAt2E8[
    (offsetof(PhotoGameUpdateView, positionHistory) == 0x02e8) ? 1 : -1];
typedef char PhotoGameUpdateBoundsAt3A8[
    (offsetof(PhotoGameUpdateView, hurtboxBoundsMin) == 0x03a8) ? 1 : -1];
typedef char PhotoGameUpdateVelocityAt3FC[
    (offsetof(PhotoGameUpdateView, velocity) == 0x03fc) ? 1 : -1];
typedef char PhotoGameUpdateMovementConfigAt410[
    (offsetof(PhotoGameUpdateView, movementConfig) == 0x0410) ? 1 : -1];
typedef char PhotoGameUpdateFocusVmAt1E2C[
    (offsetof(PhotoGameUpdateView, focusVm) == 0x1e2c) ? 1 : -1];
typedef char PhotoGameUpdateCameraAt1E3C[
    (offsetof(PhotoGameUpdateView, camera) == 0x1e3c) ? 1 : -1];
typedef char PhotoGameUpdateScaleAt2A18[
    (offsetof(PhotoGameUpdateView, cameraUpdateScale) == 0x2a18) ? 1 : -1];
typedef char PhotoGameUpdatePhotoBoundsAt2A28[
    (offsetof(PhotoGameUpdateView, photoTargetBoundsMin) == 0x2a28) ? 1 : -1];

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
        if (this->focusVm == PhotoAnmVmId(0))
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
