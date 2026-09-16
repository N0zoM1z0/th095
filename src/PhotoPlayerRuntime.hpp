#ifndef TH095_PHOTO_PLAYER_RUNTIME_HPP
#define TH095_PHOTO_PLAYER_RUNTIME_HPP

#include "ZunMath.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

namespace th095
{

#if !defined(TH095_MATCH_EXACT) && !defined(DIFFBUILD)
enum PhotoPlayerMode
{
    PHOTO_PLAYER_MODE_ENTERING = 0,
    PHOTO_PLAYER_MODE_ACTIVE = 1,
    PHOTO_PLAYER_MODE_DEATH_TRANSITION = 2,
    PHOTO_PLAYER_MODE_PHOTO_LIMIT_TRANSITION = 3,
};

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

typedef char PhotoPlayerMovementDirectionSizeIs4[
    (sizeof(PhotoPlayerMovementDirection) == 4) ? 1 : -1];

enum PhotoPlayerCameraTrackingMode
{
    PHOTO_PLAYER_CAMERA_TRACKING_FREE = 0,
    PHOTO_PLAYER_CAMERA_TRACKING_TARGET = 1,
    PHOTO_PLAYER_CAMERA_TRACKING_TARGET_SLOW = 2,
};

typedef char PhotoPlayerCameraTrackingModeSizeIs4[
    (sizeof(PhotoPlayerCameraTrackingMode) == 4) ? 1 : -1];
#endif

#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
#define TH095_PHOTO_PLAYER_CAMERA_TRACKING_FREE 0
#define TH095_PHOTO_PLAYER_CAMERA_TRACKING_TARGET 1
#define TH095_PHOTO_PLAYER_CAMERA_TRACKING_TARGET_SLOW 2
#else
#define TH095_PHOTO_PLAYER_CAMERA_TRACKING_FREE PHOTO_PLAYER_CAMERA_TRACKING_FREE
#define TH095_PHOTO_PLAYER_CAMERA_TRACKING_TARGET PHOTO_PLAYER_CAMERA_TRACKING_TARGET
#define TH095_PHOTO_PLAYER_CAMERA_TRACKING_TARGET_SLOW PHOTO_PLAYER_CAMERA_TRACKING_TARGET_SLOW
#endif

// Shared production view of the target PlayerInf object published through
// g_RuntimePlayerOwner.  Only offsets used by independently target-proven
// production consumers are named here.
struct PhotoPlayerCameraRuntimeView
{
    u8 unknown000[0x0b80];
    f32 charge;                                      // +0x0b80
    u8 unknownb84[0x0ba8 - 0x0b84];
    i32 photoIndex;                                  // +0x0ba8
    i32 photosTaken;                                 // +0x0bac
    i32 photoLimit;                                  // +0x0bb0
    u32 flags;                                       // +0x0bb4
    u8 unknownbb8[0x0bdc - 0x0bb8];
};

typedef char PhotoPlayerCameraChargeAt0B80[
    (offsetof(PhotoPlayerCameraRuntimeView, charge) == 0x0b80) ? 1 : -1];
typedef char PhotoPlayerCameraPhotoIndexAt0BA8[
    (offsetof(PhotoPlayerCameraRuntimeView, photoIndex) == 0x0ba8) ? 1 : -1];
typedef char PhotoPlayerCameraPhotosTakenAt0BAC[
    (offsetof(PhotoPlayerCameraRuntimeView, photosTaken) == 0x0bac) ? 1 : -1];
typedef char PhotoPlayerCameraPhotoLimitAt0BB0[
    (offsetof(PhotoPlayerCameraRuntimeView, photoLimit) == 0x0bb0) ? 1 : -1];
typedef char PhotoPlayerCameraFlagsAt0BB4[
    (offsetof(PhotoPlayerCameraRuntimeView, flags) == 0x0bb4) ? 1 : -1];
typedef char PhotoPlayerCameraRuntimeSizeIs0BDC[
    (sizeof(PhotoPlayerCameraRuntimeView) == 0x0bdc) ? 1 : -1];

struct PhotoPlayerRuntimeView
{
#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
    i32 mode;                                      // +0x0000
    u8 unknown0004[0x03a8 - 0x0004];
#else
    PhotoPlayerMode mode;                          // +0x0000
    u8 unknown0004[0x02d4 - 0x0004];
    PhotoPlayerMovementDirection movementState;    // +0x02d4
    PhotoPlayerCameraTrackingMode cameraTrackingMode; // +0x02d8
    u8 unknown02dc[0x03a8 - 0x02dc];
#endif
    Float3 hurtboxBoundsMin;                       // +0x03a8
    Float3 hurtboxBoundsMax;                       // +0x03b4
    u8 unknown03c0[0x0420 - 0x03c0];
    ZunTimer completionTimer;                      // +0x0420
    u8 unknown042c[0x1e30 - 0x042c];
    Float3 playerPosition;                         // +0x1e30
    PhotoPlayerCameraRuntimeView camera;           // +0x1e3c
#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
    u8 unknown2a18[0x2a28 - 0x2a18];
#else
    f32 movementScale;                             // +0x2a18
    u8 unknown2a1c[0x2a28 - 0x2a1c];
#endif
    Float3 photoTargetBoundsMin;                   // +0x2a28
    Float3 photoTargetBoundsMax;                   // +0x2a34

    f32 AngleFromPoint(Float3 *position);
    i32 CheckBulletCollision(Float3 *position, Float3 *size);
    void Die();
};

typedef char PhotoPlayerRuntimeModeAt0000[
    (offsetof(PhotoPlayerRuntimeView, mode) == 0x0000) ? 1 : -1];
#if !defined(TH095_MATCH_EXACT) && !defined(DIFFBUILD)
typedef char PhotoPlayerRuntimeMovementStateAt02D4[
    (offsetof(PhotoPlayerRuntimeView, movementState) == 0x02d4) ? 1 : -1];
typedef char PhotoPlayerRuntimeCameraTrackingModeAt02D8[
    (offsetof(PhotoPlayerRuntimeView, cameraTrackingMode) == 0x02d8) ? 1 : -1];
#endif
typedef char PhotoPlayerRuntimeHurtboxMinAt03A8[
    (offsetof(PhotoPlayerRuntimeView, hurtboxBoundsMin) == 0x03a8) ? 1 : -1];
typedef char PhotoPlayerRuntimeHurtboxMaxAt03B4[
    (offsetof(PhotoPlayerRuntimeView, hurtboxBoundsMax) == 0x03b4) ? 1 : -1];
typedef char PhotoPlayerRuntimeCompletionTimerAt0420[
    (offsetof(PhotoPlayerRuntimeView, completionTimer) == 0x0420) ? 1 : -1];
typedef char PhotoPlayerRuntimePositionAt1E30[
    (offsetof(PhotoPlayerRuntimeView, playerPosition) == 0x1e30) ? 1 : -1];
typedef char PhotoPlayerRuntimeCameraAt1E3C[
    (offsetof(PhotoPlayerRuntimeView, camera) == 0x1e3c) ? 1 : -1];
typedef char PhotoPlayerRuntimeCameraChargeAt29BC[
    (offsetof(PhotoPlayerRuntimeView, camera.charge) == 0x29bc) ? 1 : -1];
typedef char PhotoPlayerRuntimePhotoIndexAt29E4[
    (offsetof(PhotoPlayerRuntimeView, camera.photoIndex) == 0x29e4) ? 1 : -1];
typedef char PhotoPlayerRuntimePhotosTakenAt29E8[
    (offsetof(PhotoPlayerRuntimeView, camera.photosTaken) == 0x29e8) ? 1 : -1];
typedef char PhotoPlayerRuntimePhotoLimitAt29EC[
    (offsetof(PhotoPlayerRuntimeView, camera.photoLimit) == 0x29ec) ? 1 : -1];
typedef char PhotoPlayerRuntimeCameraFlagsAt29F0[
    (offsetof(PhotoPlayerRuntimeView, camera.flags) == 0x29f0) ? 1 : -1];
#if !defined(TH095_MATCH_EXACT) && !defined(DIFFBUILD)
typedef char PhotoPlayerRuntimeMovementScaleAt2A18[
    (offsetof(PhotoPlayerRuntimeView, movementScale) == 0x2a18) ? 1 : -1];
#endif
typedef char PhotoPlayerRuntimePhotoTargetBoundsMinAt2A28[
    (offsetof(PhotoPlayerRuntimeView, photoTargetBoundsMin) == 0x2a28) ? 1 : -1];
typedef char PhotoPlayerRuntimePhotoTargetBoundsMaxAt2A34[
    (offsetof(PhotoPlayerRuntimeView, photoTargetBoundsMax) == 0x2a34) ? 1 : -1];
typedef char PhotoPlayerRuntimeSizeIs2A40[
    (sizeof(PhotoPlayerRuntimeView) == 0x2a40) ? 1 : -1];

} // namespace th095

#endif
