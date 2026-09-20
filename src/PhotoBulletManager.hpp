#pragma once

#include "AnmManager.hpp"
#include "PhotoBulletRuntime.hpp"
#include "PhotoBulletSpawnDescriptor.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>
#include <string.h>

namespace th095
{

class ChainElem;


struct PhotoBulletExState
{
    ZunTimer timer;
    union
    {
        f32 float0;
        f32 accelerationMagnitude;
        f32 speedDelta;
        f32 directionChangeSpeed;
        f32 bounceSpeed;
    };
    union
    {
        f32 float1;
        f32 accelerationAngle;
        f32 angleDelta;
        f32 directionChangeAngle;
    };
    PhotoBulletVector vector;
    union
    {
        i32 int0;
        i32 durationFrames;
        i32 directionChangeIntervalFrames;
        i32 bouncesCompleted;
    };
    union
    {
        i32 int1;
        i32 directionChangeRepeatCount;
        i32 bounceLimit;
    };
    union
    {
        i32 int2;
        i32 directionChangesCompleted;
    };
};

typedef char PhotoBulletExStateSizeIs2C[
    (sizeof(PhotoBulletExState) == 0x2c) ? 1 : -1];

struct PhotoBulletView
{
    union
    {
        u32 flags;                     // +0x000
        struct
        {
            u32 unknownFlag0 : 1;
            u32 collidable : 1;
            u32 unknownFlags2 : 2;
            u32 captureDisabled : 1;
            u32 unknownFlags5 : 27;
        };
    };
    AnmVm vm;                          // +0x004
    PhotoBulletVector position;        // +0x2d0
    PhotoBulletVector velocity;        // +0x2dc
    PhotoBulletVector acceleration;    // +0x2e8
    f32 speed;                         // +0x2f4
    u32 unknown2f8[2];
    f32 angle;                         // +0x300
    u32 unknown304[2];
    PhotoBulletVector collisionSize;   // +0x30c
    ZunTimer stateTimer;               // +0x318
    ZunTimer activeTimer;              // +0x324
    i32 ownerTag;                      // +0x330
    u8 unknown334[0x344 - 0x334];
    i32 offscreenCullDelayFrames;      // +0x344
    u32 activeTransformFlags;          // +0x348
    u32 transformFlags;                // +0x34c
    i16 unknown350;
    PhotoBulletState state;            // +0x352
    u16 offscreenFrames;
    u16 unknown356;
    PhotoBulletView *nextInDrawBucket; // +0x358
    union
    {
        i32 zoneTransitionCooldownFrames;
        PhotoBulletView *nextCaptured; // +0x35c
    };
    i32 field360;
    i32 transformSound;                // +0x364
    i32 transformIndex;                // +0x368
    i32 drawBucketIndex;               // +0x36c
    PhotoBulletTransformRecord transforms[18]; // +0x370
    PhotoBulletExState exStates[7];    // +0x520
    i8 collisionDisabled;              // +0x654
    u8 unknown655;
    i16 bulletType;                    // +0x656
    i16 color;                         // +0x658
    u8 trailingAlignment65A[2];

    PhotoBulletView();
    ~PhotoBulletView();
    void Deactivate();
    void AdvanceTransformProgram();
    i32 BeginDespawn();
    void UpdateDeceleration();
    void UpdateVectorAcceleration();
    void UpdatePolarAcceleration();
    void UpdateRelativeDirectionChange();
    void UpdateAbsoluteDirectionChange();
    void UpdateAimedDirectionChange();
    void UpdateBoundaryBounce();
    void UpdateHorizontalWrap();
    void UpdateVerticalWrap();
};

typedef char PhotoBulletSizeIs65C[
    (sizeof(PhotoBulletView) == 0x65c) ? 1 : -1];
typedef char PhotoBulletPositionAt2D0[
    (offsetof(PhotoBulletView, position) == 0x2d0) ? 1 : -1];
typedef char PhotoBulletCollisionAt30C[
    (offsetof(PhotoBulletView, collisionSize) == 0x30c) ? 1 : -1];
typedef char PhotoBulletStateAt352[
    (offsetof(PhotoBulletView, state) == 0x352) ? 1 : -1];
typedef char PhotoBulletTransformsAt370[
    (offsetof(PhotoBulletView, transforms) == 0x370) ? 1 : -1];
typedef char PhotoBulletExStatesAt520[
    (offsetof(PhotoBulletView, exStates) == 0x520) ? 1 : -1];

struct PhotoBulletManagerView
{
    PhotoBulletView *bulletCursor;       // +0x000000
    PhotoBulletView *drawBucketHeads[6]; // +0x000004
    PhotoBulletView *drawBucketTails[6]; // +0x00001c
    PhotoBulletVector capturePosition;   // +0x000034
    PhotoBulletVector captureSize;       // +0x000040
    PhotoBulletView bullets[0x641];      // +0x00004c
    ChainElem *calcChain;                // +0x27c5a8
    ChainElem *drawChain;                // +0x27c5ac
    AnmLoaded *bulletAnm;                // +0x27c5b0
    i32 activeBulletCount;               // +0x27c5b4

    PhotoBulletManagerView();
    ~PhotoBulletManagerView();
    i32 Initialize();
    static PhotoBulletManagerView *__fastcall Create();
    void Destroy();
    i32 Update();
    i32 Draw();
    i32 DrawBucket(i32 bucketIndex);
    static i32 __fastcall OnUpdate(PhotoBulletManagerView *bulletManager);
    static i32 __fastcall OnDraw(PhotoBulletManagerView *bulletManager);
    i32 SpawnSingleBullet(PhotoBulletSpawnDescriptor *descriptor,
                          i32 index1, i32 index2, f32 angleToPlayer);
    i32 SpawnBulletPattern(PhotoBulletSpawnDescriptor *descriptor);
    PhotoBulletView *CapturePhotoTargets(
        PhotoBulletVector *position, PhotoBulletVector *size);
    i32 ClearCapturedBullets();
    void DespawnAllBullets();
    i32 CountNearbyTargets(PhotoBulletVector *position, f32 radius);
};

typedef char PhotoBulletManagerBulletsAt4C[
    (offsetof(PhotoBulletManagerView, bullets) == 0x4c) ? 1 : -1];
typedef char PhotoBulletManagerSizeIs27C5B8[
    (sizeof(PhotoBulletManagerView) == 0x27c5b8) ? 1 : -1];
typedef char PhotoBulletManagerChainsAt27C5A8[
    (offsetof(PhotoBulletManagerView, calcChain) == 0x27c5a8 &&
     offsetof(PhotoBulletManagerView, drawChain) == 0x27c5ac) ? 1 : -1];
typedef char PhotoBulletManagerAnmAt27C5B0[
    (offsetof(PhotoBulletManagerView, bulletAnm) == 0x27c5b0) ? 1 : -1];
typedef char PhotoBulletManagerCountAt27C5B4[
    (offsetof(PhotoBulletManagerView, activeBulletCount) == 0x27c5b4) ? 1 : -1];

} // namespace th095
