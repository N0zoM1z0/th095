#include "PhotoCamera.hpp"

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

struct PhotoGameUpdateView
{
    i32 mode;
    PhotoAnmLoadedView *effectAnm;
    AnmVm effectVm;
    i32 movementState;
    i32 cameraTrackingMode;
    u8 unknown02dc[0x0420 - 0x02dc];
    ZunTimer completionTimer;
    u8 unknown042c[0x1e30 - 0x042c];
    Float3 playerPosition;
    PhotoCameraState camera;
    f32 cameraUpdateScale;

    void UpdateMainState();
    i32 Update();
};

typedef char PhotoGameUpdateCameraAt1E3C[
    (offsetof(PhotoGameUpdateView, camera) == 0x1e3c) ? 1 : -1];
typedef char PhotoGameUpdateScaleAt2A18[
    (offsetof(PhotoGameUpdateView, cameraUpdateScale) == 0x2a18) ? 1 : -1];

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
