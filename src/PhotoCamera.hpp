#ifndef TH095_PHOTO_CAMERA_HPP
#define TH095_PHOTO_CAMERA_HPP

#include "AnmManager.hpp"

namespace th095
{

struct PhotoCapturedBulletView;

enum PhotoCameraMode
{
    PHOTO_CAMERA_TRACKING = 0,
    PHOTO_CAMERA_CHARGING = 1,
    PHOTO_CAMERA_CAPTURED = 2,
    PHOTO_CAMERA_RECOVERING = 3,
    PHOTO_CAMERA_DISABLED = 4,
};

struct PhotoAnmVmId
{
    i32 value;

    operator i32() const
    {
        return this->value;
    }

    void operator=(i32 value)
    {
        this->value = value;
    }

    AnmVm *GetVm();
    void SetInterrupt(i32 interrupt);
};

typedef char PhotoAnmVmIdSizeIs4[(sizeof(PhotoAnmVmId) == 4) ? 1 : -1];

struct PhotoAnmLoadedView
{
    i32 anmIdx;
    void *rawData;
    i32 totalEntries;
    AnmLoadedSprite *sprites;
    AnmRawInstr **scripts;
    void *textures;
    i32 numberEntriesToBeLoaded;

    ZunResult SetSprite(AnmVm *vm, i32 spriteIdx);
    void SetAndExecuteScript(AnmVm *vm, AnmRawInstr *beginningOfScript);
    void InitializeVm(AnmVm *vm, i32 scriptIndex);
    PhotoAnmVmId CreateVm(i32 scriptIndex, i32 renderMode);
};

typedef char PhotoAnmLoadedViewSizeIs1C[
    (sizeof(PhotoAnmLoadedView) == 0x1c) ? 1 : -1];

struct PhotoCameraState
{
    i32 mode;                         // +0x000
    Float3 cameraOffset;              // +0x004
    PhotoAnmVmId vmIds[11];           // +0x010
    AnmVm viewfinderVms[4];           // +0x03c
    f32 trackingRadius;               // +0xb6c
    f32 trackingAngle;                // +0xb70
    Float3 previousTrackingOrigin;    // +0xb74
    f32 charge;                       // +0xb80
    ZunTimer modeTimer;               // +0xb84
    ZunTimer chargeTimer;             // +0xb90
    ZunTimer auxiliaryTimer;          // +0xb9c
    i32 photoIndex;                   // +0xba8
    i32 photosTaken;                  // +0xbac
    i32 photoLimit;                   // +0xbb0
    u32 flags;                        // +0xbb4
    i32 unknownbb8;                   // +0xbb8
    i32 focusHeldFrames;              // +0xbbc
    i32 captureRequested;             // +0xbc0
    Float3 viewfinderPosition;        // +0xbc4
    Float3 viewfinderSize;            // +0xbd0

    void BeginCapture();
    void UpdateViewfinder();
    u32 TakePhoto();
    i32 CalculatePhotoScore(PhotoCapturedBulletView *bulletTargets,
                            i32 *scoreData, i32 runtimeTargets,
                            i32 stageTargets);
    void CancelCapture();
    i32 CountPhotoTargets(f32 *score, f32 *rate);
    void UpdateCharge();
    void Draw();
};

typedef char PhotoCameraVmsAt03C[
    (offsetof(PhotoCameraState, viewfinderVms) == 0x03c) ? 1 : -1];
typedef char PhotoCameraChargeAtB80[
    (offsetof(PhotoCameraState, charge) == 0xb80) ? 1 : -1];
typedef char PhotoCameraFlagsAtBB4[
    (offsetof(PhotoCameraState, flags) == 0xbb4) ? 1 : -1];
typedef char PhotoCameraPositionAtBC4[
    (offsetof(PhotoCameraState, viewfinderPosition) == 0xbc4) ? 1 : -1];
typedef char PhotoCameraStateSizeIsBDC[
    (sizeof(PhotoCameraState) == 0xbdc) ? 1 : -1];

struct PhotoGameStateView
{
    i32 mode;                          // +0x0000
    PhotoAnmLoadedView *effectAnm;     // +0x0004
    AnmVm effectVm;                    // +0x0008
    i32 movementState;                 // +0x02d4
    i32 cameraTrackingMode;            // +0x02d8
    u8 unknown02dc[0x0420 - 0x02dc];
    ZunTimer completionTimer;           // +0x0420
    u8 unknown042c[0x1e30 - 0x042c];
    Float3 playerPosition;             // +0x1e30
    PhotoCameraState camera;            // +0x1e3c

    f32 AngleToPoint(const Float3 *point);
};

typedef char PhotoGamePlayerPositionAt1E30[
    (offsetof(PhotoGameStateView, playerPosition) == 0x1e30) ? 1 : -1];
typedef char PhotoGameCompletionTimerAt420[
    (offsetof(PhotoGameStateView, completionTimer) == 0x0420) ? 1 : -1];
typedef char PhotoGameCameraAt1E3C[
    (offsetof(PhotoGameStateView, camera) == 0x1e3c) ? 1 : -1];

f32 __fastcall PhotoDistance2D(const Float3 *left, const Float3 *right);
void __fastcall UpdatePhotoCamera(PhotoCameraState *camera);

} // namespace th095

#endif
