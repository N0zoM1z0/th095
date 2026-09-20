#ifndef TH095_PHOTO_CAMERA_HPP
#define TH095_PHOTO_CAMERA_HPP

#include "AnmManager.hpp"
#include "PhotoCameraMode.hpp"
#if !defined(TH095_MATCH_EXACT) && !defined(DIFFBUILD)
#include "PhotoBulletManager.hpp"
#endif
#include "PhotoPlayerRuntime.hpp"

namespace th095
{

struct PhotoCapturedBulletView;
// Compiler-only four-byte equality temporary; this is not handle storage.
struct PhotoAnmVmIdValue;

// Photo camera/stage storage is a trivial four-byte handle: the target
// PhotoCameraState constructor initializes only its AnmVm array and does not
// run AnmVmId default constructors for these slots.  Operations cross this
// storage boundary through the canonical AnmVmId ABI.
struct PhotoAnmVmId
{
    i32 value;

    operator i32() const
    {
        return this->value;
    }

    operator AnmVmId() const
    {
        AnmVmId id;
        id.value = this->value;
        return id;
    }

    void operator=(i32 value)
    {
        this->value = value;
    }

    PhotoAnmVmId &operator=(AnmVmId id)
    {
        this->value = id.value;
        return *this;
    }

    AnmVm *GetVm()
    {
        return reinterpret_cast<AnmVmId *>(this)->GetVm();
    }

    void SetInterrupt(i32 interrupt)
    {
        reinterpret_cast<AnmVmId *>(this)->SetInterrupt(interrupt);
    }
};

typedef char PhotoAnmVmIdSizeIs4[(sizeof(PhotoAnmVmId) == 4) ? 1 : -1];
typedef char PhotoAnmVmIdValueAt0[
    (offsetof(PhotoAnmVmId, value) == 0x0) ? 1 : -1];

#ifdef TH095_MATCH_EXACT
struct PhotoAnmLoadedView
{
    i32 anmIdx;
    void *rawData;
    i32 totalEntries;
    AnmLoadedSprite *sprites;
    AnmRawInstr **scripts;
    void *textures;
    i32 postloadEntryNumber;

    ZunResult SetSprite(AnmVm *vm, i32 spriteIdx);
    void SetAndExecuteScript(AnmVm *vm, AnmRawInstr *beginningOfScript);

    void SetAndExecuteScriptIdx(AnmVm *vm, i32 scriptIndex)
    {
        vm->anmFile = reinterpret_cast<AnmLoaded *>(this);
        vm->scriptIndex = scriptIndex;
        this->SetAndExecuteScript(vm, this->scripts[scriptIndex]);
    }

    void InitializeVm(AnmVm *vm, i32 scriptIndex);
    PhotoAnmVmId CreateVm(i32 scriptIndex, i32 renderMode);
};
#else
typedef AnmLoaded PhotoAnmLoadedView;
#endif

typedef char PhotoAnmLoadedViewSizeIs1C[
    (sizeof(PhotoAnmLoadedView) == 0x1c) ? 1 : -1];

#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
#include "PhotoCameraBulletEmission.inl"
#endif

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
