#ifndef TH095_PHOTO_GAME_PLAYER_HPP
#define TH095_PHOTO_GAME_PLAYER_HPP

#include "PhotoCamera.hpp"

namespace th095
{

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

struct PhotoPlayerFrameStateView
{
    ZunTimer timer;
    u8 unknown00c[0x34 - 0x0c];
};

typedef char PhotoPlayerFrameStateSizeIs34[
    (sizeof(PhotoPlayerFrameStateView) == 0x34) ? 1 : -1];

// Allocation at the PlayerInf lifecycle boundary.
struct PhotoGameUpdateView
{
    PhotoPlayerMode mode;
    PhotoAnmLoadedView *effectAnm;
    AnmVm effectVm;
    PhotoPlayerMovementDirection movementState;
    PhotoPlayerCameraTrackingMode cameraTrackingMode;
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
    f32 movementScale;
    ChainElem *calcChain;
    ChainElem *drawPlayerChain;
    ChainElem *drawCameraChain;
    Float3 photoTargetBoundsMin;
    Float3 photoTargetBoundsMax;

    PhotoGameUpdateView();
    ~PhotoGameUpdateView();
    i32 Initialize();
    i32 LoadSht(char *path);
    f32 AngleToPoint(const Float3 *point);
    f32 AngleFromPoint(Float3 *position);
    i32 CheckBulletCollision(Float3 *position, Float3 *size);
    u32 CalcLaserHitbox(Float3 *origin, f32 angle, f32 width, f32 length);
    void Die();
    i32 UpdateMainState();
    i32 Update();
    i32 DrawPlayer();
    i32 DrawCamera();

    static PhotoGameUpdateView *__fastcall Create();
    void Destroy();
    static i32 __fastcall OnUpdate(PhotoGameUpdateView *player);
    static i32 __fastcall OnDrawPlayer(PhotoGameUpdateView *player);
    static i32 __fastcall OnDrawCamera(PhotoGameUpdateView *player);
};

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
typedef char PhotoGameUpdatePositionAt1E30[
    (offsetof(PhotoGameUpdateView, playerPosition) == 0x1e30) ? 1 : -1];
typedef char PhotoGameUpdateCameraAt1E3C[
    (offsetof(PhotoGameUpdateView, camera) == 0x1e3c) ? 1 : -1];
typedef char PhotoGameUpdateScaleAt2A18[
    (offsetof(PhotoGameUpdateView, movementScale) == 0x2a18) ? 1 : -1];
typedef char PhotoGameUpdatePhotoBoundsAt2A28[
    (offsetof(PhotoGameUpdateView, photoTargetBoundsMin) == 0x2a28) ? 1 : -1];
typedef char PhotoGameUpdateSizeIs2A40[
    (sizeof(PhotoGameUpdateView) == 0x2a40) ? 1 : -1];

} // namespace th095

#endif
