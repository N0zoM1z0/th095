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
    PhotoEffectBaseView()
    {
        memset(this, 0, sizeof(*this));
    }

    virtual i32 Initialize(void *args) = 0;
    virtual i32 Update() = 0;
    virtual i32 Draw() = 0;
    virtual void Cleanup();
    virtual i32 DrawSecondary() = 0;
    virtual i32 CheckCollisionA(
        Float3 *position, Float3 *size, i32 capture) = 0;
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
    u8 deletionCounter;                    // +0x48
    u8 unknown49[3];                        // +0x49
    i32 id;                                 // +0x4c
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

    PhotoEffectArgsSmallView()
    {
        memset(this, 0, sizeof(*this));
    }
};

typedef char PhotoEffectArgsSmallSizeIs28[
    (sizeof(PhotoEffectArgsSmallView) == 0x28) ? 1 : -1];

struct PhotoStraightLaserView : PhotoEffectBaseView
{
    PhotoEffectArgsSmallView spawn;          // +0x050
    AnmVm bodyVm;                            // +0x078
    AnmVm tailVm;                            // +0x344

    PhotoStraightLaserView();
    i32 Initialize(void *args);
    i32 Update();
    i32 Draw();
    i32 DrawSecondary();
    i32 CheckCollisionA(Float3 *position, Float3 *size, i32 capture);
    i32 CheckCollision(Float3 *position, Float3 *size, i32 capture);
    i32 Unknown1C(Float3 *position, Float3 *size);
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

    PhotoEffectArgsView()
    {
        memset(this, 0, sizeof(*this));
        this->speed = 8.0f;
    }
};

typedef char PhotoEffectArgsSizeIs48[
    (sizeof(PhotoEffectArgsView) == 0x48) ? 1 : -1];

struct PhotoRotatingLaserView : PhotoEffectBaseView
{
    PhotoEffectArgsView spawn;              // +0x050
    AnmVm bodyVm;                            // +0x098
    AnmVm tailVm;                            // +0x364

    PhotoRotatingLaserView();
    i32 Initialize(void *args);
    i32 Update();
    i32 Draw();
    i32 DrawSecondary();
    i32 CheckCollisionA(Float3 *position, Float3 *size, i32 capture);
    i32 CheckCollision(Float3 *position, Float3 *size, i32 capture);
    i32 Unknown1C(Float3 *position, Float3 *size);
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

struct PhotoEffectAnmView
{
    void SetAndExecuteScript(AnmVm *vm, i32 scriptIndex);
};

struct PhotoEffectManagerResourcesView
{
    u8 unknown00[0x74];
    PhotoEffectAnmView *anm;
};

extern PhotoEffectManagerResourcesView *g_PhotoEffectManager;

struct PhotoEffectGlobalStateView
{
    u8 unknown000[0xfc];
    union
    {
        u32 flags;
        struct
        {
            u32 unknownFlag0 : 1;
            u32 freezeEffects : 1;
            u32 suppressEffects : 1;
            u32 unknownFlags3 : 7;
            u32 blockEffectUpdate : 1;
            u32 unknownFlags11 : 21;
        };
    };
};

extern PhotoEffectGlobalStateView *g_PhotoGlobalState;

static inline i32 PhotoEffectEitherFlag(i32 first, i32 second)
{
    return first | second;
}

extern i32 __fastcall GetPhotoEffectScriptBase(i32 type);
Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

struct PhotoEffectManagerView
{
    u8 listRootVtable[4];                   // +0x00
    PhotoEffectBaseView *listRootPrevious;  // +0x04
    PhotoEffectBaseView *first;             // +0x08
    u8 listRootTail[0x50 - 0x0c];
    PhotoEffectBaseView *last;              // +0x50
    i32 effectCount;                        // +0x54
    i32 nextId;                             // +0x58
    PhotoEffectVector collisionPosition;    // +0x5c
    PhotoEffectVector collisionSize;        // +0x68
    u8 unknown74[0x0c];

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
    i32 CheckCollisionA(Float3 *position, Float3 *size);
    static i32 __fastcall CheckCollisionStored(
        PhotoEffectManagerView *manager);
    static i32 __fastcall DrawSecondary(PhotoEffectManagerView *manager);
    i32 CheckCollisionB(Float3 *position, Float3 *size);
};

typedef char PhotoEffectManagerSizeIs80[
    (sizeof(PhotoEffectManagerView) == 0x80) ? 1 : -1];

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

i32 PhotoStraightLaserView::Initialize(void *args)
{
    this->spawn = *static_cast<PhotoEffectArgsSmallView *>(args);
    this->state = 2;

    g_PhotoEffectManager->anm->SetAndExecuteScript(
        &this->bodyVm,
        GetPhotoEffectScriptBase(this->spawn.type) + this->spawn.color);
    this->bodyVm.pendingInterrupt = 2;
    AnmManager::ExecuteScript(&this->bodyVm);
    this->bodyVm.SetBlendModeAdditive();
    this->bodyVm.renderModeBits = 1;
    this->bodyVm.renderStateA = 0;
    this->bodyVm.renderStateB = 2;

    g_PhotoEffectManager->anm->SetAndExecuteScript(
        &this->tailVm, this->spawn.color + 0xc2);
    this->tailVm.pendingInterrupt = 2;
    AnmManager::ExecuteScript(&this->tailVm);
    this->tailVm.SetBlendModeAdditive();
    this->tailVm.renderModeBits = 1;

    this->position = this->spawn.position;
    *reinterpret_cast<i32 *>(&this->length) = this->spawn.initialLength;
    this->width = this->spawn.width;
    this->speed = this->spawn.speed;
    this->angle = this->spawn.angle;
    if (this->length > 0.0f)
    {
        this->tailOffset = 0.01f;
    }
    else
    {
        this->tailOffset = 0.0f;
    }
    reinterpret_cast<Float3 *>(&this->velocity)
        ->FromAngleMagnitude(this->angle, this->speed);
    return 0;
}

i32 PhotoStraightLaserView::Draw()
{
    PhotoToScreen(
        &this->bodyVm.position,
        reinterpret_cast<const Float3 *>(&this->position));
    this->bodyVm.rotation.z =
        AddNormalizeAngle(this->angle, 1.5707964f);
    this->bodyVm.Draw();
    if (this->tailOffset == 0.0f)
    {
        PhotoToScreen(
            &this->tailVm.position,
            reinterpret_cast<const Float3 *>(&this->position));
        this->tailVm.Draw();
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

i32 PhotoRotatingLaserView::Initialize(void *args)
{
    this->spawn = *static_cast<PhotoEffectArgsView *>(args);
    this->state = 3;

    g_PhotoEffectManager->anm->SetAndExecuteScript(
        &this->bodyVm,
        GetPhotoEffectScriptBase(this->spawn.type) + this->spawn.color);
    this->bodyVm.pendingInterrupt = 2;
    AnmManager::ExecuteScript(&this->bodyVm);
    this->bodyVm.SetBlendModeAdditive();
    this->bodyVm.renderModeBits = 1;
    this->bodyVm.renderStateA = 0;
    this->bodyVm.renderStateB = 2;

    g_PhotoEffectManager->anm->SetAndExecuteScript(
        &this->tailVm, this->spawn.color + 0xc2);
    this->tailVm.pendingInterrupt = 2;
    AnmManager::ExecuteScript(&this->tailVm);
    this->tailVm.SetBlendModeAdditive();
    this->tailVm.renderModeBits = 1;

    this->position = this->spawn.position;
    this->length = this->spawn.initialLength;
    this->width = 2.0f;
    this->speed = this->spawn.speed;
    this->angle = this->spawn.angle;
    return 0;
}

i32 PhotoRotatingLaserView::Draw()
{
    PhotoToScreen(
        &this->bodyVm.position,
        reinterpret_cast<const Float3 *>(&this->position));
    this->bodyVm.rotation.z =
        AddNormalizeAngle(this->angle, 1.5707964f);
    this->bodyVm.Draw();
    if (this->tailOffset == 0.0f)
    {
        PhotoToScreen(
            &this->tailVm.position,
            reinterpret_cast<const Float3 *>(&this->position));
        this->tailVm.Draw();
    }
    return 0;
}

void PhotoEffectManagerView::Remove(PhotoEffectBaseView *effect)
{
    this->effectCount--;
    effect->previous->next = effect->next;
    if (effect->next != NULL)
    {
        effect->next->previous = effect->previous;
    }
    if (this->last == effect)
    {
        this->last = effect->previous;
    }
}

i32 __fastcall PhotoEffectManagerView::Update(
    PhotoEffectManagerView *manager)
{
    PhotoEffectBaseView *effect = manager->first;
    while (effect != NULL)
    {
        PhotoEffectBaseView *next = effect->next;

        if (effect->deletionCounter != 0)
        {
            effect->deletionCounter++;
            if (effect->deletionCounter >= 2)
            {
                effect->Cleanup();
                manager->Remove(effect);
                delete effect;
                effect = NULL;
                goto advanceEffect;
            }
        }

        if (effect->state == 1)
        {
            effect->Cleanup();
            manager->Remove(effect);
            delete effect;
            effect = NULL;
        }
        else if (effect->Update() != 0)
        {
            effect->Cleanup();
            manager->Remove(effect);
            delete effect;
            effect = NULL;
        }
        else
        {
            effect->timer.Tick();
        }

    advanceEffect:
        effect = next;
    }
    return 1;
}

i32 __fastcall PhotoEffectManagerView::Draw(
    PhotoEffectManagerView *manager)
{
    PhotoEffectBaseView *effect = manager->first;
    while (effect != NULL)
    {
        PhotoEffectBaseView *next = effect->next;
        if (effect->state != 1)
        {
            effect->Draw();
        }
        effect = next;
    }
    return 1;
}

i32 __fastcall PhotoEffectManagerView::OnUpdate(
    PhotoEffectManagerView *manager)
{
    if (PhotoEffectEitherFlag(
            g_PhotoGlobalState->unknownFlag0,
            g_PhotoGlobalState->suppressEffects) != 0)
    {
        return 1;
    }
    if (g_PhotoGlobalState->blockEffectUpdate != 0)
    {
        return 1;
    }
    if (g_PhotoGlobalState->freezeEffects != 0)
    {
        f32 gameSpeed = g_GameSpeed;
        g_GameSpeed = 0.0f;
        i32 result = Update(manager);
        g_GameSpeed = gameSpeed;
        return result;
    }
    return Update(manager);
}

i32 __fastcall PhotoEffectManagerView::OnDraw(
    PhotoEffectManagerView *manager)
{
    if (g_PhotoGlobalState->suppressEffects != 0)
    {
        return 1;
    }
    return Draw(manager);
}

i32 PhotoEffectManagerView::Spawn(i32 type, void *args)
{
    if (this->effectCount >= 0x100)
    {
        return 0;
    }

    this->nextId++;
    if (this->nextId == 0)
    {
        this->nextId++;
    }

    switch (type)
    {
    case 0:
    {
        PhotoStraightLaserView *effect = new PhotoStraightLaserView;
        effect->id = this->nextId;
        this->Append(effect);
        this->effectCount++;
        effect->Initialize(args);
        break;
    }
    case 1:
    {
        PhotoRotatingLaserView *effect = new PhotoRotatingLaserView;
        effect->id = this->nextId;
        this->Append(effect);
        this->effectCount++;
        effect->Initialize(args);
        break;
    }
    }
    return this->nextId;
}

i32 PhotoEffectManagerView::CheckCollisionA(
    Float3 *position, Float3 *size)
{
    PhotoEffectBaseView *effect = this->first;
    i32 count = 0;
    this->collisionPosition =
        *reinterpret_cast<PhotoEffectVector *>(position);
    this->collisionSize =
        *reinterpret_cast<PhotoEffectVector *>(size);

    while (effect != NULL)
    {
        PhotoEffectBaseView *next = effect->next;
        if (effect->state != 1)
        {
            count += effect->CheckCollisionA(position, size, 1);
        }
        effect = next;
    }
    return count;
}

i32 __fastcall PhotoEffectManagerView::CheckCollisionStored(
    PhotoEffectManagerView *manager)
{
    PhotoEffectBaseView *effect = manager->first;
    i32 count = 0;
    while (effect != NULL)
    {
        PhotoEffectBaseView *next = effect->next;
        if (effect->state != 1)
        {
            count += effect->CheckCollision(
                reinterpret_cast<Float3 *>(&manager->collisionPosition),
                reinterpret_cast<Float3 *>(&manager->collisionSize),
                1);
        }
        effect = next;
    }
    return count;
}

i32 __fastcall PhotoEffectManagerView::DrawSecondary(
    PhotoEffectManagerView *manager)
{
    PhotoEffectBaseView *effect = manager->first;
    while (effect != NULL)
    {
        PhotoEffectBaseView *next = effect->next;
        if (effect->state != 1)
        {
            effect->DrawSecondary();
        }
        effect = next;
    }
    return 1;
}

i32 PhotoEffectManagerView::CheckCollisionB(
    Float3 *position, Float3 *size)
{
    PhotoEffectBaseView *effect = this->first;
    i32 count = 0;
    while (effect != NULL)
    {
        PhotoEffectBaseView *next = effect->next;
        if (effect->state != 1)
        {
            count += effect->Unknown1C(position, size);
        }
        effect = next;
    }
    return count;
}

PhotoStraightLaserView::PhotoStraightLaserView()
{
}

PhotoRotatingLaserView::PhotoRotatingLaserView()
{
}

}
