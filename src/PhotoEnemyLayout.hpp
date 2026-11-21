#pragma once

#include "AnmVertex.hpp"
#include "PhotoEnemyAnmVmIdStorage.hpp"
#include "EclInstruction.hpp"
#include "EnemyAnmScripts.hpp"
#include "EnemyEclContext.hpp"
#include "EnemyMovementBounds.hpp"
#include "PhotoBulletSpawnDescriptor.hpp"
#include "PhotoEnemyControl.hpp"
#include "ZunTimer.hpp"
#include "inttypes.hpp"

#include <stddef.h>

namespace th095
{

enum
{
    PHOTO_ENEMY_SCHEDULE_SLOT_COUNT = 10,
    PHOTO_ENEMY_SCHEDULE_INACTIVE = -1,
};

// Opcode 115 writes the full word. Dispatch consumes its low signed half.
union PhotoEnemyScheduledCall
{
    i32 rawValue;
    struct
    {
        i16 subroutineId;
        i16 unknown02;
    };
};

struct PhotoEnemyEclSchedule
{
    i32 photoIndices[PHOTO_ENEMY_SCHEDULE_SLOT_COUNT];
    PhotoEnemyScheduledCall calls[PHOTO_ENEMY_SCHEDULE_SLOT_COUNT];
};

enum PhotoEnemyTrailFlag
{
    PHOTO_ENEMY_TRAIL_INITIALIZE_STRIP = 1 << 3,
};

struct PhotoEnemyTrailConfig
{
    u8 flags;
    u8 unknown01;
    i16 length;
    i16 unknown04;
    i16 sampleStride;
};

struct PhotoEnemyTrailSampleView
{
    Float3 position;
    Float3 velocity;
    f32 angle;

    PhotoEnemyTrailSampleView()
    {
    }
};

// Canonical normal-source owner of the compact TH095 enemy element.  The
// manager contains one template followed by 128 inline elements at stride
// 0x4CC0.  Fields without independent producer/consumer evidence remain
// explicitly unknown.
struct PhotoEnemyView
{
    PhotoEnemyView *nextInDrawGroup;       // +0x0000
    u8 unknown0004[4];
    AnmVm vm;                              // +0x0008
    PhotoEnemyAnmVmIdStorage anmHandles[2]; // +0x02d4
    EnemyEclContext mainEclContext; // +0x02dc
    EnemyEclContext eclCallStack[16]; // +0x050c
    EnemyEclContext *activeEclContext; // +0x280c
    EnemyEclContext *activeEclCallStack; // +0x2810
    i32 eclIntVariables[8];                // +0x2814
    f32 eclFloatVariables[8];              // +0x2834
    i16 mainEclCallStackDepth;             // +0x2854
    i16 activeEclCallStackDepth;           // +0x2856
    u8 unknown2858[2];
    i16 photoCaptureEclSubroutineId;       // +0x285a
    i16 eclSubroutineIds[32];              // +0x285c
    i16 pendingEclSubroutineIndex;         // +0x289c
    u8 unknown289e[2];
    Float3 position;                       // +0x28a0
    Float3 positionOffset;                 // +0x28ac
    Float3 velocity;                       // +0x28b8
    Float3 previousPosition;               // +0x28c4
    Float3 positionDelta;                  // +0x28d0
    Float3 collisionSize;                  // +0x28dc
    Float3 secondaryHitboxDimensions;      // +0x28e8
    Float3 worldPosition;                  // +0x28f4
    f32 movementAngle;                     // +0x2900
    f32 angularVelocity;                   // +0x2904
    f32 orbitAngle;                        // +0x2908
    f32 orbitAngularVelocity;              // +0x290c
    PhotoEnemyView *parentEnemy;           // +0x2910
    f32 speed;                             // +0x2914
    f32 acceleration;                      // +0x2918
    f32 orbitRadius;                       // +0x291c
    f32 radialVelocity;                    // +0x2920
    Float3 shootOffset;                    // +0x2924
    Float3 movementInterpolationDelta;     // +0x2930
    Float3 movementInterpolationOrigin;    // +0x293c
    ZunTimer movementTimer;                // +0x2948
    i32 movementDuration;                  // +0x2954
    i32 life;                              // +0x2958
    i32 maximumLife;                       // +0x295c
    i32 phaseStartingLife;                 // +0x2960
    i32 score;                             // +0x2964
    i32 enemyIndex;                        // +0x2968
    ZunTimer eclTimer;                     // +0x296c
    ZunTimer stateTimer;                   // +0x2978
    u8 unknown2984[4];
    u32 displayColor;                      // +0x2988
    PhotoBulletSpawnDescriptor bulletSpawnDescriptor; // +0x298c
    EclShotInstruction pendingShotInstruction;       // +0x2b9c
    i32 shootIntervalFrames;               // +0x2bc8
    ZunTimer shootIntervalTimer;           // +0x2bcc
    i32 itemDropType;                      // +0x2bd8
    i32 timelineParam0;                    // +0x2bdc
    i32 timelineParam1;                    // +0x2be0
    u8 unknown2be4;
    u8 photoTargetSlot;                    // +0x2be5
    u8 unknown2be6[2];
    ZunTimer auxiliaryTimer;               // +0x2be8
    union
    {
        u32 flags1;                        // +0x2bf4
        PhotoEnemyControlBits control;
    };
    union
    {
        u32 flags2;                        // +0x2bf8
        PhotoEnemySecondaryControlBits secondaryControl;
    };
    ZunTimer photoMarkerPulseTimer;        // +0x2bfc
    u8 unknown2c08[2];
    u8 anmDirection;                       // +0x2c0a
    u8 drawGroup;                          // +0x2c0b
    u8 unknown2c0c[2];
    EnemyAnmScripts anmScripts;            // +0x2c0e
    u8 unknown2c1a[2];
    PhotoEnemyAnmVmIdStorage photoPulseVmId; // +0x2c1c
    PhotoEnemyAnmVmIdStorage photoMarkerVmId; // +0x2c20
    ZunTimer photoPulseTimer;              // +0x2c24
    ZunTimer photoPulseDurationTimer;      // +0x2c30
    EnemyMovementBounds movementBounds;    // +0x2c3c
    f32 minimumPlayerDistanceSquared;      // +0x2c4c
    i32 unknown2c50;                       // +0x2c50
    PhotoEnemyEclSchedule eclSchedule;     // +0x2c54
    i32 pendingCallbackFrame;              // +0x2ca4
    i32 unknown2ca8;                       // +0x2ca8
    EnemyChildEclBlock *childEclBlocks[16]; // +0x2cac
    PhotoEnemyTrailSampleView trailSamples[96]; // +0x2cec
    AnmVertex trailVertices[194]; // +0x376c
    PhotoEnemyTrailConfig trailConfig;     // +0x4ca4
    ZunTimer timer4cac;                     // +0x4cac
    u8 unknown4cb8[4];
    PhotoEnemyAnmVmIdStorage attachedVmId; // +0x4cbc

    i32 HasActivePhotoPulse() const
    {
        return this->photoPulseTimer.current > 0;
    }

    PhotoEnemyView();
    ~PhotoEnemyView()
    {
    }
    f32 ResolveFloat(f32 operand);
    void UpdateShotAndAnm();
    void IntegrateMovement();
    void ClampPosition();
    void RestartEcl();
    void UpdatePhotoMarkerPulse();
    i32 UpdateScheduledEclCalls();
    void Deactivate();
};

typedef char PhotoEnemyTrailSampleSizeIs1C[
    (sizeof(PhotoEnemyTrailSampleView) == 0x1c) ? 1 : -1];
typedef char PhotoEnemyScheduledCallSizeIs4[
    (sizeof(PhotoEnemyScheduledCall) == 4) ? 1 : -1];
typedef char PhotoEnemySizeIs4CC0[
    (sizeof(PhotoEnemyView) == 0x4cc0) ? 1 : -1];
typedef char PhotoEnemyVmAt8[
    (offsetof(PhotoEnemyView, vm) == 0x08) ? 1 : -1];
typedef char PhotoEnemyVmRotationZAt28[
    (offsetof(PhotoEnemyView, vm) + offsetof(AnmVm, rotation) +
         offsetof(Float3, z) == 0x28)
        ? 1 : -1];
typedef char PhotoEnemyAnmHandlesAt2D4[
    (offsetof(PhotoEnemyView, anmHandles) == 0x2d4) ? 1 : -1];
typedef char PhotoEnemyMainEclContextAt2DC[
    (offsetof(PhotoEnemyView, mainEclContext) == 0x2dc) ? 1 : -1];
typedef char PhotoEnemyMainEclScriptStateAt2F4[
    (offsetof(PhotoEnemyView, mainEclContext) +
         offsetof(EnemyEclContext, scriptState) == 0x2f4) ? 1 : -1];
typedef char PhotoEnemyActiveEclContextAt280C[
    (offsetof(PhotoEnemyView, activeEclContext) == 0x280c) ? 1 : -1];
typedef char PhotoEnemyPhotoCaptureSubroutineAt285A[
    (offsetof(PhotoEnemyView, photoCaptureEclSubroutineId) == 0x285a) ? 1 : -1];
typedef char PhotoEnemyPositionAt28A0[
    (offsetof(PhotoEnemyView, position) == 0x28a0)
        ? 1 : -1];
typedef char PhotoEnemyWorldPositionAt28F4[
    (offsetof(PhotoEnemyView, worldPosition) == 0x28f4) ? 1 : -1];
typedef char PhotoEnemyMovementAt2900[
    (offsetof(PhotoEnemyView, movementAngle) == 0x2900 &&
     offsetof(PhotoEnemyView, angularVelocity) == 0x2904 &&
     offsetof(PhotoEnemyView, orbitAngle) == 0x2908 &&
     offsetof(PhotoEnemyView, orbitAngularVelocity) == 0x290c &&
     offsetof(PhotoEnemyView, speed) == 0x2914 &&
     offsetof(PhotoEnemyView, acceleration) == 0x2918 &&
     offsetof(PhotoEnemyView, orbitRadius) == 0x291c) ? 1 : -1];
typedef char PhotoEnemyMovementInterpolationAt2930[
    (offsetof(PhotoEnemyView, movementInterpolationDelta) == 0x2930 &&
     offsetof(PhotoEnemyView, movementInterpolationOrigin) == 0x293c) ? 1 : -1];
typedef char PhotoEnemyLifeAt2958[
    (offsetof(PhotoEnemyView, life) == 0x2958) ? 1 : -1];
typedef char PhotoEnemyLifeSequenceAt2958[
    (offsetof(PhotoEnemyView, maximumLife) == 0x295c &&
     offsetof(PhotoEnemyView, phaseStartingLife) == 0x2960) ? 1 : -1];
typedef char PhotoEnemyScoreAt2964[
    (offsetof(PhotoEnemyView, score) == 0x2964) ? 1 : -1];
typedef char PhotoEnemyEclTimerAt296C[
    (offsetof(PhotoEnemyView, eclTimer) == 0x296c)
        ? 1 : -1];
typedef char PhotoEnemyEclTimerCurrentAt2974[
    (offsetof(PhotoEnemyView, eclTimer) + offsetof(ZunTimer, current) == 0x2974) ? 1 : -1];
typedef char PhotoEnemyShotCadenceAt2B9C[
    (offsetof(PhotoEnemyView, pendingShotInstruction) == 0x2b9c &&
     offsetof(PhotoEnemyView, shootIntervalFrames) == 0x2bc8 &&
     offsetof(PhotoEnemyView, shootIntervalTimer) == 0x2bcc) ? 1 : -1];
typedef char PhotoEnemyBulletDescriptorAt298C[
    (offsetof(PhotoEnemyView, bulletSpawnDescriptor) == 0x298c &&
     offsetof(PhotoEnemyView, bulletSpawnDescriptor) +
         offsetof(PhotoBulletSpawnDescriptor, transforms) == 0x29ac &&
     offsetof(PhotoEnemyView, bulletSpawnDescriptor) +
         offsetof(PhotoBulletSpawnDescriptor, transformFlags) == 0x2b88 &&
     offsetof(PhotoEnemyView, bulletSpawnDescriptor) +
         offsetof(PhotoBulletSpawnDescriptor, spawnSound) == 0x2b8c &&
     offsetof(PhotoEnemyView, bulletSpawnDescriptor) +
         offsetof(PhotoBulletSpawnDescriptor, transformSound) == 0x2b90) ? 1 : -1];
typedef char PhotoEnemyPhotoTargetSlotAt2BE5[
    (offsetof(PhotoEnemyView, photoTargetSlot) == 0x2be5) ? 1 : -1];
typedef char PhotoEnemyItemDropTypeAt2BD8[
    (offsetof(PhotoEnemyView, itemDropType) == 0x2bd8) ? 1 : -1];
typedef char PhotoEnemyEclScheduleAt2C54[
    (offsetof(PhotoEnemyView, eclSchedule) == 0x2c54 &&
     offsetof(PhotoEnemyEclSchedule, calls) == 0x28 &&
     sizeof(PhotoEnemyEclSchedule) == 0x50) ? 1 : -1];
typedef char PhotoEnemyUnknownAt2C50[
    (offsetof(PhotoEnemyView, unknown2c50) == 0x2c50) ? 1 : -1];
typedef char PhotoEnemyFlagsAt2BF4[
    (offsetof(PhotoEnemyView, flags1) == 0x2bf4 &&
     offsetof(PhotoEnemyView, flags2) == 0x2bf8) ? 1 : -1];
typedef char PhotoEnemyPhotoMarkerAt2BFC[
    (offsetof(PhotoEnemyView, photoMarkerPulseTimer) == 0x2bfc) ? 1 : -1];
typedef char PhotoEnemyAnmDirectionAt2C0A[
    (offsetof(PhotoEnemyView, anmDirection) == 0x2c0a) ? 1 : -1];
typedef char PhotoEnemyDrawGroupAt2C0B[
    (offsetof(PhotoEnemyView, drawGroup) == 0x2c0b)
        ? 1 : -1];
typedef char PhotoEnemyAnmScriptsAt2C0E[
    (offsetof(PhotoEnemyView, anmScripts) == 0x2c0e) ? 1 : -1];
typedef char PhotoEnemyChildEclBlocksAt2CAC[
    (offsetof(PhotoEnemyView, childEclBlocks) == 0x2cac) ? 1 : -1];
typedef char PhotoEnemyPhotoPulseAt2C1C[
    (offsetof(PhotoEnemyView, photoPulseVmId) == 0x2c1c &&
     offsetof(PhotoEnemyView, photoMarkerVmId) == 0x2c20 &&
     offsetof(PhotoEnemyView, photoPulseTimer) == 0x2c24 &&
     offsetof(PhotoEnemyView, photoPulseDurationTimer) == 0x2c30) ? 1 : -1];
typedef char PhotoEnemyMovementBoundsAt2C3C[
    (offsetof(PhotoEnemyView, movementBounds) == 0x2c3c &&
     offsetof(PhotoEnemyView, minimumPlayerDistanceSquared) == 0x2c4c) ? 1 : -1];
typedef char PhotoEnemyPendingCallbackFrameAt2CA4[
    (offsetof(PhotoEnemyView, pendingCallbackFrame) == 0x2ca4 &&
     offsetof(PhotoEnemyView, unknown2ca8) == 0x2ca8) ? 1 : -1];
typedef char PhotoEnemyTrailSamplesAt2CEC[
    (offsetof(PhotoEnemyView, trailSamples) == 0x2cec) ? 1 : -1];
typedef char PhotoEnemyTrailVerticesAt376C[
    (offsetof(PhotoEnemyView, trailVertices) == 0x376c) ? 1 : -1];
typedef char PhotoEnemyTrailConfigLayout[
    (sizeof(PhotoEnemyTrailConfig) == 8 &&
     offsetof(PhotoEnemyTrailConfig, flags) == 0 &&
     offsetof(PhotoEnemyTrailConfig, length) == 2 &&
     offsetof(PhotoEnemyTrailConfig, unknown04) == 4 &&
     offsetof(PhotoEnemyTrailConfig, sampleStride) == 6 &&
     offsetof(PhotoEnemyView, trailConfig) == 0x4ca4)
        ? 1 : -1];
typedef char PhotoEnemyTimerAt4CAC[
    (offsetof(PhotoEnemyView, timer4cac) == 0x4cac)
        ? 1 : -1];
typedef char PhotoEnemyAttachedVmAt4CBC[
    (offsetof(PhotoEnemyView, attachedVmId) == 0x4cbc) ? 1 : -1];

} // namespace th095
