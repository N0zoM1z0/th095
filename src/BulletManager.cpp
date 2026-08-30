#include "AnmManager.hpp"

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
};

typedef char PhotoBulletVectorSizeIsC[
    (sizeof(PhotoBulletVector) == 0x0c) ? 1 : -1];

struct PhotoBulletView
{
    union
    {
        u32 flags;                     // +0x000
        struct
        {
            u32 unknownFlag0 : 1;
            u32 collidable : 1;
            u32 unknownFlags2 : 30;
        };
    };
    AnmVm vm;                          // +0x004
    PhotoBulletVector position;        // +0x2d0
    PhotoBulletVector velocity;        // +0x2dc
    u8 unknown2e8[0x30c - 0x2e8];
    PhotoBulletVector collisionSize;   // +0x30c
    ZunTimer stateTimer;               // +0x318
    u8 unknown324[0x348 - 0x324];
    u32 activeTransformFlags;           // +0x348
    u8 unknown34c[0x352 - 0x34c];
    u16 state;                          // +0x352
    u8 unknown354[0x358 - 0x354];
    PhotoBulletView *nextInDrawBucket; // +0x358
    u8 unknown35c[0x36c - 0x35c];
    i32 drawBucketIndex;                // +0x36c
    u8 unknown370[0x5fc - 0x370];
    ZunTimer waitTimer;                 // +0x5fc
    u8 unknown608[0x65c - 0x608];

    void Deactivate();
    void AdvanceTransformProgram();
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

struct PhotoBulletManagerView
{
    u8 unknown000[0x04];
    PhotoBulletView *drawBucketHeads[6]; // +0x04
    PhotoBulletView *drawBucketTails[6]; // +0x1c
    u8 unknown034[0x4c - 0x34];
    PhotoBulletView bullets[0x641];      // +0x4c
    u8 unknown27c5a8[8];
    void *anmSpawner;                    // +0x27c5b0
    i32 activeBulletCount;               // +0x27c5b4

    static i32 __fastcall OnUpdate(PhotoBulletManagerView *bulletManager);
};

typedef char PhotoBulletManagerBulletsAt4C[
    (offsetof(PhotoBulletManagerView, bullets) == 0x4c) ? 1 : -1];
typedef char PhotoBulletManagerAnmAt27C5B0[
    (offsetof(PhotoBulletManagerView, anmSpawner) == 0x27c5b0) ? 1 : -1];
typedef char PhotoBulletManagerCountAt27C5B4[
    (offsetof(PhotoBulletManagerView, activeBulletCount) == 0x27c5b4) ? 1 : -1];

struct PhotoBulletGlobalStateView
{
    u8 unknown000[0xfc];
    union
    {
        u32 flags;
        struct
        {
            u32 unknownFlag0 : 1;
            u32 blocksBulletUpdate : 1;
            u32 unknownFlags2 : 8;
            u32 photoCaptureInputMode : 1;
            u32 unknownFlags11 : 21;
        };
    };
};

struct PhotoBulletPlayerView
{
    i32 CheckBulletCollision(
        PhotoBulletVector *position, PhotoBulletVector *size);
};

extern PhotoBulletGlobalStateView *g_PhotoBulletGlobalState;
extern PhotoBulletPlayerView *g_PhotoBulletPlayer;

static inline i32 PhotoBulletIsOutsidePlayfield(
    PhotoBulletVector *position, f32 width, f32 height)
{
    return width + position->x <= -192.0f ||
           position->x - width >= 192.0f ||
           height + position->y <= 0.0f ||
           position->y - height >= 448.0f;
}

i32 __fastcall PhotoBulletManagerView::OnUpdate(
    PhotoBulletManagerView *bulletManager)
{
    PhotoBulletView *bullet = &bulletManager->bullets[0];

    bulletManager->drawBucketHeads[5] = NULL;
    bulletManager->drawBucketHeads[4] = NULL;
    bulletManager->drawBucketHeads[3] = NULL;
    bulletManager->drawBucketHeads[2] = NULL;
    bulletManager->drawBucketHeads[1] = NULL;
    bulletManager->drawBucketHeads[0] = NULL;
    bulletManager->drawBucketTails[5] = NULL;
    bulletManager->drawBucketTails[4] = NULL;
    bulletManager->drawBucketTails[3] = NULL;
    bulletManager->drawBucketTails[2] = NULL;
    bulletManager->drawBucketTails[1] = NULL;
    bulletManager->drawBucketTails[0] = NULL;
    bulletManager->activeBulletCount = 0;

    for (i32 bulletIndex = 0;
         bulletIndex < 0x640;
         ++bulletIndex, ++bullet)
    {
        if (bullet->state == 0)
        {
            continue;
        }

        if (g_PhotoBulletGlobalState->blocksBulletUpdate != 0)
        {
            goto enqueueBullet;
        }
        if (g_PhotoBulletGlobalState->photoCaptureInputMode != 0)
        {
            goto enqueueBullet;
        }

        switch (bullet->state)
        {
        case 2:
            bullet->position +=
                bullet->velocity * g_AnmGameSpeed / 2.0f;
            if (bullet->vm.intVar0 == 0)
            {
                break;
            }
            bullet->state = 1;

        case 1:
            bullet->AdvanceTransformProgram();
            if (bullet->activeTransformFlags != 0)
            {
                if ((bullet->activeTransformFlags & 0x000001) != 0)
                    bullet->UpdateDeceleration();
                if ((bullet->activeTransformFlags & 0x000010) != 0)
                    bullet->UpdateVectorAcceleration();
                if ((bullet->activeTransformFlags & 0x000020) != 0)
                    bullet->UpdatePolarAcceleration();
                if ((bullet->activeTransformFlags & 0x000040) != 0)
                    bullet->UpdateRelativeDirectionChange();
                if ((bullet->activeTransformFlags & 0x000100) != 0)
                    bullet->UpdateAbsoluteDirectionChange();
                if ((bullet->activeTransformFlags & 0x000080) != 0)
                    bullet->UpdateAimedDirectionChange();
                if ((bullet->activeTransformFlags & 0x000c00) != 0)
                    bullet->UpdateBoundaryBounce();
                if ((bullet->activeTransformFlags & 0x100000) != 0)
                    bullet->UpdateHorizontalWrap();
                if ((bullet->activeTransformFlags & 0x200000) != 0)
                    bullet->UpdateVerticalWrap();
                if ((bullet->activeTransformFlags & 0x008000) != 0)
                {
                    if (bullet->waitTimer <= 0)
                        bullet->activeTransformFlags ^= 0x008000;
                    else
                        bullet->waitTimer--;
                }
            }

            bullet->position += bullet->velocity * g_AnmGameSpeed;
            if (bullet->collidable != 0)
            {
                if (g_PhotoBulletPlayer->CheckBulletCollision(
                        &bullet->position, &bullet->collisionSize) != 0)
                {
                    bullet->state = 3;
                    bullet->vm.pendingInterrupt = 1;
                    break;
                }
            }
            break;

        case 3:
            bullet->position +=
                bullet->velocity * g_AnmGameSpeed / 2.0f;
            break;
        }

        if (bullet->vm.loadedSprite != NULL)
        {
            if (PhotoBulletIsOutsidePlayfield(
                    &bullet->position,
                    bullet->vm.loadedSprite->widthPx,
                    bullet->vm.loadedSprite->heightPx))
            {
                bullet->Deactivate();
                continue;
            }
        }

        if (AnmManager::ExecuteScript(&bullet->vm) != 0)
        {
            bullet->Deactivate();
            continue;
        }

    enqueueBullet:
        if (bulletManager->drawBucketHeads[bullet->drawBucketIndex] != NULL)
        {
            bulletManager->drawBucketTails[bullet->drawBucketIndex]
                ->nextInDrawBucket = bullet;
        }
        else
        {
            bulletManager->drawBucketHeads[bullet->drawBucketIndex] = bullet;
        }
        bulletManager->drawBucketTails[bullet->drawBucketIndex] = bullet;
        bullet->nextInDrawBucket = NULL;
        bulletManager->activeBulletCount++;
        bullet->stateTimer.Tick();
    }
    return 1;
}

} // namespace th095
