#pragma once

#include "AnmManager.hpp"
#include "PhotoBulletRuntime.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>
#include <string.h>

namespace th095
{

class ChainElem;

struct PhotoBulletVector
{
    f32 x;
    f32 y;
    f32 z;

    PhotoBulletVector()
    {
    }

    PhotoBulletVector(f32 x, f32 y, f32 z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    PhotoBulletVector operator*(f32 scalar) const
    {
        return PhotoBulletVector(
            this->x * scalar, this->y * scalar, this->z * scalar);
    }

    PhotoBulletVector operator+(const PhotoBulletVector &other) const
    {
        return PhotoBulletVector(
            this->x + other.x,
            this->y + other.y,
            this->z + other.z);
    }

    PhotoBulletVector operator-(const PhotoBulletVector &other) const
    {
        return PhotoBulletVector(
            this->x - other.x,
            this->y - other.y,
            this->z - other.z);
    }

    PhotoBulletVector operator/(f32 scalar) const
    {
        f32 reciprocal = 1.0f / scalar;
        return PhotoBulletVector(
            this->x * reciprocal,
            this->y * reciprocal,
            this->z * reciprocal);
    }

    void operator+=(const PhotoBulletVector &other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
    }

    void operator-=(const PhotoBulletVector &other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
    }

    void FromAngleMagnitude(f32 angle, f32 magnitude);
};

typedef char PhotoBulletVectorSizeIsC[
    (sizeof(PhotoBulletVector) == 0x0c) ? 1 : -1];
typedef char PhotoBulletVectorMatchesFloat3[
    (sizeof(PhotoBulletVector) == sizeof(Float3) &&
     offsetof(PhotoBulletVector, x) == offsetof(Float3, x) &&
     offsetof(PhotoBulletVector, y) == offsetof(Float3, y) &&
     offsetof(PhotoBulletVector, z) == offsetof(Float3, z)) ? 1 : -1];

enum PhotoBulletTransformKind
{
    PHOTO_BULLET_TRANSFORM_NONE = 0,
    PHOTO_BULLET_TRANSFORM_DECELERATE = 0x00000001,
    PHOTO_BULLET_TRANSFORM_SPAWN_FAST = 0x00000002,
    PHOTO_BULLET_TRANSFORM_SPAWN_NORMAL = 0x00000004,
    PHOTO_BULLET_TRANSFORM_SPAWN_SLOW = 0x00000008,
    PHOTO_BULLET_TRANSFORM_ACCELERATE_VECTOR = 0x00000010,
    PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR = 0x00000020,
    PHOTO_BULLET_TRANSFORM_CHANGE_DIRECTION_RELATIVE = 0x00000040,
    PHOTO_BULLET_TRANSFORM_CHANGE_DIRECTION_AIMED = 0x00000080,
    PHOTO_BULLET_TRANSFORM_CHANGE_DIRECTION_ABSOLUTE = 0x00000100,
    PHOTO_BULLET_TRANSFORM_PLAY_SPAWN_SOUND = 0x00000200,
    PHOTO_BULLET_TRANSFORM_BOUNCE_ALL_EDGES = 0x00000400,
    PHOTO_BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM = 0x00000800,
    PHOTO_BULLET_TRANSFORM_CANCEL_IMMUNE = 0x00001000,
    PHOTO_BULLET_TRANSFORM_SET_CULL_DELAY = 0x00002000,
    PHOTO_BULLET_TRANSFORM_SET_SPRITE = 0x00004000,
    PHOTO_BULLET_TRANSFORM_WAIT = 0x00008000,
    PHOTO_BULLET_TRANSFORM_DESPAWN = 0x00010000,
    PHOTO_BULLET_TRANSFORM_PLAY_SOUND = 0x00020000,
    PHOTO_BULLET_TRANSFORM_WRAP_X = 0x00100000,
    PHOTO_BULLET_TRANSFORM_WRAP_Y = 0x00200000,
    PHOTO_BULLET_TRANSFORM_SPAWN_CHILD_PATTERN = 0x00400000,
    PHOTO_BULLET_TRANSFORM_SET_OWNER_TAG = 0x01000000,
    PHOTO_BULLET_TRANSFORM_JUMP = 0x02000000,
};

typedef u16 PhotoBulletAimMode;
enum PhotoBulletAimModeValue
{
    PHOTO_BULLET_AIM_FAN_AIMED = 0,
    PHOTO_BULLET_AIM_FAN = 1,
    PHOTO_BULLET_AIM_CIRCLE_AIMED = 2,
    PHOTO_BULLET_AIM_CIRCLE = 3,
    PHOTO_BULLET_AIM_OFFSET_CIRCLE_AIMED = 4,
    PHOTO_BULLET_AIM_OFFSET_CIRCLE = 5,
    PHOTO_BULLET_AIM_RANDOM_ANGLE = 6,
    PHOTO_BULLET_AIM_RANDOM_SPEED = 7,
    PHOTO_BULLET_AIM_RANDOM = 8,
};
typedef char PhotoBulletAimModeSizeIs2[
    (sizeof(PhotoBulletAimMode) == sizeof(u16)) ? 1 : -1];

struct PhotoBulletTransformPayload
{
    union
    {
        f32 float0;
        f32 accelerationMagnitude;
        f32 speedDelta;
        f32 directionChangeAngle;
        f32 bounceSpeed;
        f32 childSpeed1;
    };
    union
    {
        f32 float1;
        f32 accelerationAngle;
        f32 angleDelta;
        f32 directionChangeSpeed;
        f32 childSpeed2;
    };
    union
    {
        i32 int0;
        i32 ownerTag;
        i32 durationFrames;
        i32 directionChangeIntervalFrames;
        i32 bounceLimit;
        i32 soundIndex;
        i32 packedChildPattern;
        i32 childCount2;
    };
    union
    {
        i32 int1;
        i32 directionChangeRepeatCount;
        i32 childCount1;
        i32 childTransformFlags;
    };
};

struct PhotoBulletTransformRecord
{
    PhotoBulletTransformPayload payload;
    u32 kind;
    i32 allowWhileActive;
};

struct PhotoBulletSpawnDescriptor
{
    i16 bulletType;
    i16 color;
    PhotoBulletVector position;
    f32 angle;
    f32 angleStep;
    f32 speed1;
    f32 speed2;
    PhotoBulletTransformRecord transforms[18];
    u8 laserFields[0x24];
    i16 count1;
    i16 count2;
    PhotoBulletAimMode aimMode;
    u16 unknown1FA;
    u32 transformFlags;
    i32 spawnSound;
    i32 transformSound;
    i32 transformStartIndex;
    void *templateSprites;

    PhotoBulletSpawnDescriptor()
    {
        memset(this, 0, sizeof(*this));
        this->transformSound = -1;
    }
};

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

typedef char PhotoBulletTransformRecordSizeIs18[
    (sizeof(PhotoBulletTransformRecord) == 0x18) ? 1 : -1];
typedef char PhotoBulletSpawnDescriptorSizeIs210[
    (sizeof(PhotoBulletSpawnDescriptor) == 0x210) ? 1 : -1];
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
