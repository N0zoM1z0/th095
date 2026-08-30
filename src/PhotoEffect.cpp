#include "AnmManager.hpp"

namespace th095
{

struct PhotoEffectVector
{
    f32 x;
    f32 y;
    f32 z;

    PhotoEffectVector()
    {
    }

    PhotoEffectVector(f32 x, f32 y, f32 z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    PhotoEffectVector operator*(f32 scalar) const
    {
        return PhotoEffectVector(
            this->x * scalar,
            this->y * scalar,
            this->z * scalar);
    }

    void operator+=(const PhotoEffectVector &other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
    }
};

typedef char PhotoEffectVectorSizeIsC[
    (sizeof(PhotoEffectVector) == 0x0c) ? 1 : -1];

struct PhotoEffectBaseView
{
    virtual void Unknown00() = 0;
    virtual i32 Update() = 0;
    virtual void Unknown08() = 0;
    virtual void Unknown0C() = 0;
    virtual void Unknown10() = 0;
    virtual void Unknown14() = 0;
    virtual i32 CheckCollision(
        Float3 *position, Float3 *size, i32 capture) = 0;
    virtual i32 Unknown1C(Float3 *position, Float3 *size) = 0;

    PhotoEffectBaseView *previous;          // +0x04
    PhotoEffectBaseView *next;              // +0x08
    i32 state;                              // +0x0c
    ZunTimer timer;                         // +0x10
    PhotoEffectVector position;             // +0x1c
    PhotoEffectVector velocity;             // +0x28
    f32 angle;                              // +0x34
    f32 length;                             // +0x38
    f32 width;                              // +0x3c
    f32 speed;                              // +0x40
    f32 tailOffset;                         // +0x44
    u8 lifecycleBytes[8];                   // +0x48
};

typedef char PhotoEffectBaseSizeIs50[
    (sizeof(PhotoEffectBaseView) == 0x50) ? 1 : -1];

struct PhotoEffectArgsSmallView
{
    PhotoEffectVector position;
    f32 angle;
    f32 maximumLength;
    i32 initialLength;
    f32 terminalDistance;
    f32 width;
    f32 speed;
    i16 type;
    i16 color;
};

typedef char PhotoEffectArgsSmallSizeIs28[
    (sizeof(PhotoEffectArgsSmallView) == 0x28) ? 1 : -1];

struct PhotoStraightLaserView : PhotoEffectBaseView
{
    PhotoEffectArgsSmallView spawn;          // +0x050
    AnmVm bodyVm;                            // +0x078
    AnmVm tailVm;                            // +0x344

    i32 Update();
};

typedef char PhotoStraightLaserBodyVmAt78[
    (offsetof(PhotoStraightLaserView, bodyVm) == 0x78) ? 1 : -1];
typedef char PhotoStraightLaserTailVmAt344[
    (offsetof(PhotoStraightLaserView, tailVm) == 0x344) ? 1 : -1];

struct PhotoEffectArgsView
{
    PhotoEffectVector position;
    PhotoEffectVector velocity;
    f32 angle;
    f32 angularVelocity;
    f32 maximumLength;
    f32 initialLength;
    f32 maximumWidth;
    f32 speed;
    i32 startupDuration;
    i32 growthDuration;
    i32 sustainDuration;
    i32 fadeDuration;
    i16 type;
    i16 color;
    u32 flags;
};

typedef char PhotoEffectArgsSizeIs48[
    (sizeof(PhotoEffectArgsView) == 0x48) ? 1 : -1];

struct PhotoRotatingLaserView : PhotoEffectBaseView
{
    PhotoEffectArgsView spawn;              // +0x050
    AnmVm bodyVm;                            // +0x098
    AnmVm tailVm;                            // +0x364

    i32 Update();
};

typedef char PhotoRotatingLaserBodyVmAt98[
    (offsetof(PhotoRotatingLaserView, bodyVm) == 0x98) ? 1 : -1];
typedef char PhotoRotatingLaserTailVmAt364[
    (offsetof(PhotoRotatingLaserView, tailVm) == 0x364) ? 1 : -1];

struct PhotoGameUpdateView
{
    u8 unknown0000[0x1e30];
    Float3 playerPosition;

    u32 CalcLaserHitbox(Float3 *origin, f32 angle, f32 width, f32 length);
};

extern PhotoGameUpdateView *g_PhotoGame;
extern f32 g_GameSpeed;

struct PhotoEnemyView
{
    u8 unknown0000[0x28a0];
    PhotoEffectVector worldPosition;
};

struct PhotoEnemyManagerView
{
    u8 unknown000000[0x26ae00];
    PhotoEnemyView *photoTargets[8];
};

extern PhotoEnemyManagerView *g_PhotoEnemyManager;

static inline i32 PhotoEffectIsOutsidePlayfield(
    PhotoEffectVector *position, f32 width, f32 height)
{
    return width + position->x <= -192.0f ||
           position->x - width >= 192.0f ||
           height + position->y <= 0.0f ||
           position->y - height >= 448.0f;
}

static inline i32 PhotoEffectEndpointIsOutsidePlayfield(
    PhotoEffectVector *position, f32 width, f32 height)
{
    return position->x + width <= -192.0f ||
           position->x - width >= 192.0f ||
           position->y + height <= 0.0f ||
           position->y - height >= 448.0f;
}

i32 PhotoStraightLaserView::Update()
{
    if (this->length < this->spawn.maximumLength)
    {
        this->length += g_GameSpeed * this->speed;
        if (this->length > this->spawn.maximumLength)
        {
            this->length = this->spawn.maximumLength;
        }
    }
    else
    {
        this->tailOffset += g_GameSpeed * this->speed;
        this->position += this->velocity * g_GameSpeed;
        if (this->spawn.terminalDistance > 0.0f &&
            this->spawn.terminalDistance < this->tailOffset + this->length)
        {
            this->length = this->spawn.terminalDistance - this->tailOffset;
            this->spawn.maximumLength = this->length;
            if (this->length <= 0.0f)
            {
                return 1;
            }
        }
    }

    Float3 endpoint;
    endpoint.FromAngleMagnitude(this->angle, this->length);
    endpoint += *reinterpret_cast<Float3 *>(&this->position);

    if (PhotoEffectIsOutsidePlayfield(
            &this->position, this->width, this->width))
    {
        if (PhotoEffectEndpointIsOutsidePlayfield(
                reinterpret_cast<PhotoEffectVector *>(&endpoint),
                this->width,
                this->width))
        {
            return 1;
        }
    }

    if (this->length > 16.0f && this->width > 40.0f)
    {
        Float3 collisionOrigin;
        collisionOrigin.FromAngleMagnitude(this->angle, 8.0f);
        collisionOrigin += *reinterpret_cast<Float3 *>(&this->position);

        if (g_PhotoGame->CalcLaserHitbox(
                &collisionOrigin,
                this->angle,
                this->width < 32.0f
                    ? this->width * 0.5f
                    : this->width - 16.0f,
                this->length - 16.0f) != 0)
        {
            Float3 collisionSize(32.0f, 32.0f, 0.0f);
            this->CheckCollision(
                &g_PhotoGame->playerPosition,
                &collisionSize,
                0);
        }
    }

    this->bodyVm.scale.x =
        this->width / this->bodyVm.loadedSprite->widthPx;
    this->bodyVm.scale.y =
        this->length / this->bodyVm.loadedSprite->heightPx;
    AnmManager::ExecuteScript(&this->bodyVm);
    if (this->tailOffset == 0.0f)
    {
        AnmManager::ExecuteScript(&this->tailVm);
    }
    return 0;
}

i32 PhotoRotatingLaserView::Update()
{
    if (this->length < this->spawn.maximumLength)
    {
        this->length += g_GameSpeed * this->speed;
        if (this->length > this->spawn.maximumLength)
        {
            this->length = this->spawn.maximumLength;
        }
    }

    this->angle = AddNormalizeAngle(
        this->angle, g_GameSpeed * this->spawn.angularVelocity);

    if ((this->spawn.flags & 1) != 0 &&
        g_PhotoEnemyManager->photoTargets[0] != NULL)
    {
        this->position =
            g_PhotoEnemyManager->photoTargets[0]->worldPosition;
    }

    this->position += this->spawn.velocity * g_GameSpeed;

    switch (this->state)
    {
    case 3:
        if (this->timer >= this->spawn.startupDuration)
        {
            this->timer = 0;
            this->state = 4;
        }
        break;

    case 4:
        if (this->timer >= this->spawn.growthDuration)
        {
            this->timer = 0;
            this->state = 2;
            this->width = this->spawn.maximumWidth;
        }
        else
        {
            this->width =
                static_cast<f32>(this->timer) *
                this->spawn.maximumWidth /
                this->spawn.growthDuration;
            break;
        }

    case 2:
        if (this->timer >= this->spawn.sustainDuration)
        {
            this->timer = 0;
            this->state = 5;
        }
        else
        {
            break;
        }

    case 5:
        if (this->timer >= this->spawn.fadeDuration)
        {
            return 1;
        }
        this->width =
            this->spawn.maximumWidth -
            static_cast<f32>(this->timer) *
                this->spawn.maximumWidth /
                this->spawn.fadeDuration;
        break;
    }

    if ((this->state == 4 || this->state == 2) &&
        this->length > 16.0f)
    {
        Float3 collisionOrigin;
        collisionOrigin.FromAngleMagnitude(this->angle, 8.0f);
        collisionOrigin += *reinterpret_cast<Float3 *>(&this->position);

        if (g_PhotoGame->CalcLaserHitbox(
                &collisionOrigin,
                this->angle,
                this->width < 32.0f
                    ? this->width * 0.5f
                    : this->width - 16.0f,
                this->length - 16.0f) != 0)
        {
            Float3 collisionSize(32.0f, 32.0f, 0.0f);
            this->CheckCollision(
                &g_PhotoGame->playerPosition,
                &collisionSize,
                0);
        }
    }

    this->bodyVm.scale.x =
        this->width / this->bodyVm.loadedSprite->widthPx;
    this->bodyVm.scale.y =
        this->length / this->bodyVm.loadedSprite->heightPx;
    AnmManager::ExecuteScript(&this->bodyVm);
    if (this->tailOffset == 0.0f)
    {
        AnmManager::ExecuteScript(&this->tailVm);
    }
    return 0;
}

}
