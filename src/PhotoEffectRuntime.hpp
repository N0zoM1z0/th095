#pragma once

#include "ZunTimer.hpp"
#include "inttypes.hpp"

#include <stddef.h>
#include <string.h>

namespace th095
{

struct Float3;
struct AnmLoaded;
class ChainElem;

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
    PhotoEffectBaseView()
    {
        memset(this, 0, sizeof(*this));
    }

    virtual i32 Initialize(void *args);
    virtual i32 Update();
    virtual i32 Draw();
    virtual i32 Cleanup();
    virtual i32 DrawSecondary();
    virtual i32 CountPhotoTargets(
        Float3 *position, Float3 *size, i32 capture);
    virtual i32 CheckCollision(
        Float3 *position, Float3 *size, i32 capture);
    virtual i32 CountNearbyTargets(Float3 *position, f32 radius);

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
    u8 deletionCounter;                    // +0x48
    u8 unknown49[3];                        // +0x49
    i32 id;                                 // +0x4c
};

typedef char PhotoEffectBaseSizeIs50[
    (sizeof(PhotoEffectBaseView) == 0x50) ? 1 : -1];

struct PhotoEffectManagerView
{
    PhotoEffectBaseView listRoot;           // +0x00
    PhotoEffectBaseView *last;              // +0x50
    i32 effectCount;                        // +0x54
    i32 nextId;                             // +0x58
    PhotoEffectVector collisionPosition;    // +0x5c
    PhotoEffectVector collisionSize;        // +0x68
    AnmLoaded *anm;                         // +0x74
    ChainElem *calcChain;                   // +0x78
    ChainElem *drawChain;                   // +0x7c

    PhotoEffectManagerView();
    ~PhotoEffectManagerView();

    static PhotoEffectManagerView *Create();
    void Destroy();
    i32 Initialize();

    void Remove(PhotoEffectBaseView *effect);
    static i32 __fastcall Update(PhotoEffectManagerView *manager);
    static i32 __fastcall Draw(PhotoEffectManagerView *manager);
    static i32 __fastcall OnUpdate(PhotoEffectManagerView *manager);
    static i32 __fastcall OnDraw(PhotoEffectManagerView *manager);
    i32 Spawn(i32 type, void *args);
    void Append(PhotoEffectBaseView *effect)
    {
        PhotoEffectBaseView *previous = this->last;
        effect->previous = previous;
        previous->next = effect;
        this->last = effect;
    }
    i32 CountPhotoTargets(Float3 *position, Float3 *size);
    static i32 __fastcall CheckCollisionStored(
        PhotoEffectManagerView *manager);
    static i32 __fastcall DrawSecondary(PhotoEffectManagerView *manager);
    i32 CountNearbyTargets(Float3 *position, f32 radius);
};

typedef char PhotoEffectManagerSizeIs80[
    (sizeof(PhotoEffectManagerView) == 0x80) ? 1 : -1];
typedef char PhotoEffectManagerLastAt50[
    (offsetof(PhotoEffectManagerView, last) == 0x50) ? 1 : -1];
typedef char PhotoEffectManagerAnmAt74[
    (offsetof(PhotoEffectManagerView, anm) == 0x74) ? 1 : -1];


} // namespace th095
