#pragma once

#include "ZunMath.hpp"
#include "inttypes.hpp"

#include <stddef.h>
#include <string.h>

namespace th095
{

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
typedef char PhotoBulletTransformRecordSizeIs18[
    (sizeof(PhotoBulletTransformRecord) == 0x18) ? 1 : -1];
typedef char PhotoBulletSpawnDescriptorSizeIs210[
    (sizeof(PhotoBulletSpawnDescriptor) == 0x210) ? 1 : -1];

} // namespace th095
