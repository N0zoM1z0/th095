#ifndef TH095_PHOTO_CAMERA_HPP
#define TH095_PHOTO_CAMERA_HPP

#include "AnmManager.hpp"
#include "PhotoAnmCreateVmEmission.hpp"
#include "PhotoAnmVmId.hpp"
#include "PhotoCameraMode.hpp"
#include "PhotoPlayerRuntime.hpp"
#include "PhotoScoreBreakdown.hpp"

namespace th095
{

struct PhotoBulletView;
// Compiler-only four-byte equality temporary; this is not handle storage.
struct PhotoAnmVmIdValue;

typedef AnmLoaded PhotoAnmLoadedView;

typedef char PhotoAnmLoadedViewSizeIs1C[
    (sizeof(PhotoAnmLoadedView) == 0x1c) ? 1 : -1];

enum PhotoCameraChargeUiState
{
    PHOTO_CAMERA_CHARGE_UI_BELOW_FULL = 0,
    PHOTO_CAMERA_CHARGE_UI_FULL = 1,
    PHOTO_CAMERA_CHARGE_UI_INITIAL = 2
};

struct PhotoCameraState
{
    PhotoCameraMode mode;             // +0x000
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
    union
    {
        u32 flags;                    // +0xbb4
        struct
        {
            u32 alternateCapture : 1;
            u32 focused : 1;
            u32 targetFrameActive : 1;
            u32 chargeUiState : 2;
            u32 chargeEffectActive : 1;
            u32 targetSoundPlayed : 1;
            u32 unknownFlags7_31 : 25;
        };
    };
    i32 focusChargeFrames;            // +0xbb8
    i32 focusHeldFrames;              // +0xbbc
    i32 captureRequested;             // +0xbc0
    Float3 viewfinderPosition;        // +0xbc4
    Float3 viewfinderSize;            // +0xbd0

    PhotoCameraState();
    ~PhotoCameraState() {}
    void Initialize();
    void BeginCapture();
    void UpdateViewfinder();
    u32 TakePhoto();
    i32 CalculatePhotoScore(PhotoBulletView *bulletTargets,
                            PhotoScoreBreakdownView *scoreData, i32 runtimeTargets,
                            i32 stageTargets);
    void CancelCapture();
    i32 CountPhotoTargets(f32 *score, f32 *rate);
    void UpdateCharge();
    void Draw();

    // VC7.1 materializes these return values in the ECL operand readers.
    i32 GetPhotoIndex() const { return this->photoIndex; }
    i32 GetPhotosTaken() const { return this->photosTaken; }
};

typedef char PhotoCameraVmsAt03C[
    (offsetof(PhotoCameraState, viewfinderVms) == 0x03c) ? 1 : -1];
typedef char PhotoCameraModeAt000[
    (offsetof(PhotoCameraState, mode) == 0x000) ? 1 : -1];
typedef char PhotoCameraChargeAtB80[
    (offsetof(PhotoCameraState, charge) == 0xb80) ? 1 : -1];
typedef char PhotoCameraFlagsAtBB4[
    (offsetof(PhotoCameraState, flags) == 0xbb4) ? 1 : -1];
typedef char PhotoCameraFocusChargeFramesAtBB8[
    (offsetof(PhotoCameraState, focusChargeFrames) == 0xbb8) ? 1 : -1];
typedef char PhotoCameraPositionAtBC4[
    (offsetof(PhotoCameraState, viewfinderPosition) == 0xbc4) ? 1 : -1];
typedef char PhotoCameraStateSizeIsBDC[
    (sizeof(PhotoCameraState) == 0xbdc) ? 1 : -1];

f32 __fastcall PhotoDistance2D(const Float3 *left, const Float3 *right);
void __fastcall UpdatePhotoCamera(PhotoCameraState *camera);

} // namespace th095

#endif
