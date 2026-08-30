#include "AnmManager.hpp"
#include "AnmVmId.hpp"
#include "PhotoItemManager.hpp"

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

    PhotoBulletVector operator+(PhotoBulletVector other) const
    {
        return PhotoBulletVector(
            this->x + other.x,
            this->y + other.y,
            this->z + other.z);
    }

    PhotoBulletVector operator-(PhotoBulletVector other) const
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
    PHOTO_BULLET_TRANSFORM_SET_FIELD_330 = 0x01000000,
    PHOTO_BULLET_TRANSFORM_JUMP = 0x02000000,
};

enum PhotoBulletAimMode
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
    u16 aimMode;
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
    i32 field330;
    u8 unknown334[0x344 - 0x334];
    i32 offscreenCullDelayFrames;      // +0x344
    u32 activeTransformFlags;          // +0x348
    u32 transformFlags;                // +0x34c
    i16 unknown350;
    u16 state;                          // +0x352
    u16 offscreenFrames;
    u16 unknown356;
    PhotoBulletView *nextInDrawBucket; // +0x358
    union
    {
        i32 zoneTransitionCooldownFrames;
        PhotoBulletView *nextCaptured;  // +0x35c
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

struct PhotoBulletAnmLoadedView;

struct PhotoBulletManagerView
{
    PhotoBulletView *bulletCursor;       // +0x00
    PhotoBulletView *drawBucketHeads[6]; // +0x04
    PhotoBulletView *drawBucketTails[6]; // +0x1c
    PhotoBulletVector capturePosition;    // +0x34
    PhotoBulletVector captureSize;        // +0x40
    PhotoBulletView bullets[0x641];      // +0x4c
    ChainElem *calcChain;                 // +0x27c5a8
    ChainElem *drawChain;                 // +0x27c5ac
    PhotoBulletAnmLoadedView *anmSpawner; // +0x27c5b0
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
            u32 suppressesBulletCallbacks : 1;
            u32 unknownFlags3 : 6;
            u32 suppressesPhotoSound : 1;
            u32 photoCaptureInputMode : 1;
            u32 unknownFlags11 : 21;
        };
    };
};

struct PhotoBulletPlayerView
{
    f32 AngleFromPoint(PhotoBulletVector *position);
    i32 CheckBulletCollision(
        PhotoBulletVector *position, PhotoBulletVector *size);
};

struct PhotoBulletAnmLoadedView : AnmLoaded
{
    void InitializeVm(AnmVm *vm, i32 scriptIndex);
    AnmVmId CreateVm(i32 scriptIndex, PhotoBulletVector *position);
};

struct PhotoBulletSoundPlayerView
{
    void PlaySoundByIdx(i32 soundIndex, i32 pan);
    void PlaySoundPositionedByIdx(i32 soundIndex, f32 positionX);
};

extern PhotoBulletGlobalStateView *g_PhotoBulletGlobalState;
extern PhotoBulletPlayerView *g_PhotoBulletPlayer;
extern PhotoBulletManagerView *g_PhotoBulletManager;
extern PhotoBulletSoundPlayerView g_PhotoBulletSoundPlayer;
extern i32 g_PhotoBulletScriptBases[];
extern f32 g_PhotoBulletCollisionSizes[];
extern i32 g_PhotoBulletDrawBucketIndices[];
extern u32 g_PhotoBulletColors16[];
extern u32 g_PhotoBulletColors8[];
extern u32 g_PhotoBulletColors4[];

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

// FUNCTION: TH095 0x00404C60.
i32 __fastcall GetPhotoBulletScriptBase(i32 bulletType)
{
    return g_PhotoBulletScriptBases[bulletType];
}

// FUNCTION: TH095 0x00404D00.
PhotoBulletView::PhotoBulletView()
{
}

// FUNCTION: TH095 0x00404DC0.
PhotoBulletView::~PhotoBulletView()
{
}

// FUNCTION: TH095 0x00404C80.
PhotoBulletManagerView::PhotoBulletManagerView()
{
    utils::DebugPrint("@@initialize BulletInf\n");
    memset(this, 0, sizeof(PhotoBulletManagerView));
    g_PhotoBulletManager = this;
}

// FUNCTION: TH095 0x00404E00.
i32 PhotoBulletManagerView::Initialize()
{
    this->anmSpawner = reinterpret_cast<PhotoBulletAnmLoadedView *>(
        g_AnmManager->LoadAnm(6, "bullet.anm"));
    if (this->anmSpawner == NULL)
    {
        g_GameErrorContext.Log(
            "\x93\x47\x92\x65\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x8c\xa9\x82\xc2"
            "\x82\xa9\x82\xe8\x82\xdc\x82\xb9"
            "\x82\xf1\x81\x42\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x89\xf3\x82\xea"
            "\x82\xc4\x82\xa2\x82\xdc\x82\xb7"
            "\r\n");
        return ZUN_ERROR;
    }
    this->bulletCursor = &this->bullets[0];
    this->bullets[0x640].state = 5;
    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00404E70.
i32 LoadPhotoBulletAnm()
{
    if (g_AnmManager->LoadAnm(6, "bullet.anm") == NULL)
    {
        g_GameErrorContext.Log(
            "\x93\x47\x92\x65\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x8c\xa9\x82\xc2"
            "\x82\xa9\x82\xe8\x82\xdc\x82\xb9"
            "\x82\xf1\x81\x42\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x89\xf3\x82\xea"
            "\x82\xc4\x82\xa2\x82\xdc\x82\xb7"
            "\r\n");
        return ZUN_ERROR;
    }
    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00404EB0.
i32 ReleasePhotoBulletAnm()
{
    g_AnmManager->ReleaseAnm(6);
    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00404ED0.
PhotoBulletManagerView::~PhotoBulletManagerView()
{
    utils::DebugPrint("shitdown BulletInf\n");
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);
    g_AnmManager->MarkVmsForDeletion(
        reinterpret_cast<AnmLoaded *>(this->anmSpawner));
    g_PhotoBulletManager = NULL;
}

// FUNCTION: TH095 0x00404F80.
PhotoBulletManagerView *__fastcall PhotoBulletManagerView::Create()
{
    struct
    {
        PhotoBulletManagerView *manager;
        ChainElem *elem;
    } locals;

#define manager locals.manager
#define elem locals.elem

    manager = new PhotoBulletManagerView();
    if (manager->Initialize() != ZUN_SUCCESS)
    {
        goto failure;
    }

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoBulletManagerView::OnUpdate));
    elem->arg = manager;
    g_Chain.AddToCalcChain(elem, 0x0e);
    manager->calcChain = elem;

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoBulletManagerView::OnDraw));
    elem->arg = manager;
    g_Chain.AddToDrawChain(elem, 0x0e);
    manager->drawChain = elem;
    return manager;

failure:
    if (manager != NULL)
    {
        delete manager;
        manager = NULL;
    }
#undef elem
#undef manager
    return NULL;
}

// FUNCTION: TH095 0x004050C0.
void PhotoBulletManagerView::Destroy()
{
    PhotoBulletManagerView *manager = this;
    if (manager != NULL)
    {
        delete manager;
        manager = NULL;
    }
}

#pragma var_order(speed, i, bullet, angle, transformFlags, this)
// FUNCTION: TH095 0x00405A30; TH08 0x0042F5F0 is the adjacent source oracle.
i32 PhotoBulletManagerView::SpawnSingleBullet(
    PhotoBulletSpawnDescriptor *descriptor, i32 index1, i32 index2,
    f32 angleToPlayer)
{
    f32 speed;
    i32 i;
    PhotoBulletView *bullet;
    f32 angle;
    u32 transformFlags;

    i = 0;
    bullet = this->bulletCursor;
    for (i = 0; i < 0x640; ++i)
    {
        if (bullet->state == 0)
            break;
        ++bullet;
        if (bullet->state == 5)
            bullet = &this->bullets[0];
    }
    if (i >= 0x640)
        return 1;

    angle = 0.0f;
    if (descriptor->count2 > 1)
        speed = descriptor->speed1 -
                (descriptor->speed1 - descriptor->speed2) * (f32)index2 /
                    (f32)descriptor->count2;
    else
        speed = descriptor->speed1;

    switch (descriptor->aimMode)
    {
    case PHOTO_BULLET_AIM_FAN_AIMED:
    case PHOTO_BULLET_AIM_FAN:
        if ((descriptor->count1 & 1) != 0)
            angle += (f32)((index1 + 1) / 2) * descriptor->angleStep;
        else
            angle += (f32)(index1 / 2) * descriptor->angleStep +
                     descriptor->angleStep * 0.5f;
        if ((index1 & 1) != 0)
            angle *= -1.0f;
        if (descriptor->aimMode == PHOTO_BULLET_AIM_FAN_AIMED)
            angle += angleToPlayer;
        angle += descriptor->angle;
        break;

    case PHOTO_BULLET_AIM_CIRCLE_AIMED:
        angle += angleToPlayer;
    case PHOTO_BULLET_AIM_CIRCLE:
        angle += (f32)index1 * 6.2831855f / (f32)descriptor->count1;
        angle += (f32)index2 * descriptor->angleStep + descriptor->angle;
        break;

    case PHOTO_BULLET_AIM_OFFSET_CIRCLE_AIMED:
        angle += angleToPlayer;
    case PHOTO_BULLET_AIM_OFFSET_CIRCLE:
        angle += 3.1415927f / (f32)descriptor->count1;
        angle += (f32)index1 * 6.2831855f / (f32)descriptor->count1;
        angle += descriptor->angle;
        break;

    case PHOTO_BULLET_AIM_RANDOM_ANGLE:
        angle = g_Rng.GetRandomF32InRange(
                    descriptor->angle - descriptor->angleStep) +
                descriptor->angleStep;
        break;

    case PHOTO_BULLET_AIM_RANDOM_SPEED:
        speed = g_Rng.GetRandomF32InRange(
                    descriptor->speed1 - descriptor->speed2) +
                descriptor->speed2;
        angle += (f32)index1 * 6.2831855f / (f32)descriptor->count1;
        angle += (f32)index2 * descriptor->angleStep + descriptor->angle;
        break;

    case PHOTO_BULLET_AIM_RANDOM:
        angle = g_Rng.GetRandomF32InRange(
                    descriptor->angle - descriptor->angleStep) +
                descriptor->angleStep;
        speed = g_Rng.GetRandomF32InRange(
                    descriptor->speed1 - descriptor->speed2) +
                descriptor->speed2;
        break;

    default:
        break;
    }

    bullet->state = 1;
    bullet->flags |= 1;
    bullet->stateTimer = 0;
    bullet->activeTimer = 0;
    bullet->speed = speed;
    bullet->angle = AddNormalizeAngle(angle, 0.0f);
    bullet->position = descriptor->position;
    bullet->position.z = 0.1f;
    bullet->velocity.FromAngleMagnitude(angle, speed);
    bullet->activeTransformFlags = descriptor->transformFlags;
    bullet->color = descriptor->color;
    bullet->bulletType = descriptor->bulletType;
    bullet->field360 = 0;
    bullet->flags &= ~0x00000008;
    bullet->flags &= ~0x00000004;
    bullet->flags |= 0x00000002;
    bullet->flags &= ~0x00000010;

    this->anmSpawner->InitializeVm(
        &bullet->vm,
        g_PhotoBulletScriptBases[descriptor->bulletType] + descriptor->color);
    bullet->drawBucketIndex =
        g_PhotoBulletDrawBucketIndices[descriptor->bulletType];
    bullet->transformSound = descriptor->transformSound;
    bullet->offscreenCullDelayFrames = 0;
    bullet->collisionSize.y =
        g_PhotoBulletCollisionSizes[descriptor->bulletType];
    bullet->collisionSize.x = bullet->collisionSize.y;

    transformFlags = descriptor->transformFlags;
    if ((descriptor->transformFlags & PHOTO_BULLET_TRANSFORM_SPAWN_FAST) != 0)
    {
        bullet->vm.pendingInterrupt = 7;
        bullet->state = 2;
        bullet->position -= bullet->velocity * 4.0f;
    }
    else if ((descriptor->transformFlags &
              PHOTO_BULLET_TRANSFORM_SPAWN_NORMAL) != 0)
    {
        bullet->vm.pendingInterrupt = 8;
        bullet->state = 2;
        bullet->position -= bullet->velocity * 4.0f;
    }
    else if ((descriptor->transformFlags &
              PHOTO_BULLET_TRANSFORM_SPAWN_SLOW) != 0)
    {
        bullet->vm.pendingInterrupt = 9;
        bullet->state = 2;
        bullet->position -= bullet->velocity * 4.0f;
    }
    else
    {
        bullet->vm.pendingInterrupt = 2;
    }

    memcpy(bullet->transforms, descriptor->transforms,
           sizeof(descriptor->transforms));
    bullet->transformFlags = descriptor->transformFlags;
    bullet->activeTransformFlags = 0;
    bullet->transformIndex = descriptor->transformStartIndex;
    bullet->AdvanceTransformProgram();
    AnmManager::ExecuteScript(&bullet->vm);

    ++bullet;
    if (bullet->state == 5)
        this->bulletCursor = &this->bullets[0];
    else
        this->bulletCursor = bullet;
    return 0;
}

// FUNCTION: TH095 0x004062B0; TH08 0x0042FFC0 is the adjacent source oracle.
void PhotoBulletView::AdvanceTransformProgram()
{
    PhotoBulletTransformRecord *record;

nextRecord:
    if (this->transformIndex >= 18)
        return;

    record = &this->transforms[this->transformIndex];
    if (record->kind == PHOTO_BULLET_TRANSFORM_NONE)
        return;
    if (record->allowWhileActive == 0 && this->activeTransformFlags != 0)
        return;
    if ((this->transformFlags & record->kind) == 0)
    {
        ++this->transformIndex;
        goto nextRecord;
    }

    switch (record->kind)
    {
    case PHOTO_BULLET_TRANSFORM_DECELERATE:
        this->activeTransformFlags |= PHOTO_BULLET_TRANSFORM_DECELERATE;
        this->exStates[0].timer = 0;
        *reinterpret_cast<i32 *>(&this->exStates[0].vector.z) = 0;
        break;

    case PHOTO_BULLET_TRANSFORM_ACCELERATE_VECTOR:
        this->activeTransformFlags |= PHOTO_BULLET_TRANSFORM_ACCELERATE_VECTOR;
        this->exStates[1].accelerationMagnitude =
            record->payload.accelerationMagnitude;
        this->exStates[1].accelerationAngle =
            record->payload.accelerationAngle > -990.0f
                ? record->payload.accelerationAngle
                : this->angle;
        this->exStates[1].timer = 0;
        this->exStates[1].durationFrames = record->payload.durationFrames;
        this->exStates[1].vector.FromAngleMagnitude(
            this->exStates[1].accelerationAngle,
            this->exStates[1].accelerationMagnitude);
        if (this->transformIndex != 0 && this->transformSound >= 0)
            g_PhotoBulletSoundPlayer.PlaySoundByIdx(
                this->transformSound, 0);
        break;

    case PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR:
        this->activeTransformFlags |= PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR;
        this->exStates[2].speedDelta = record->payload.speedDelta;
        this->exStates[2].angleDelta = record->payload.angleDelta;
        this->exStates[2].timer = 0;
        this->exStates[2].durationFrames = record->payload.durationFrames;
        if (this->transformIndex != 0 && this->transformSound >= 0)
            g_PhotoBulletSoundPlayer.PlaySoundByIdx(
                this->transformSound, 0);
        break;

    case PHOTO_BULLET_TRANSFORM_CHANGE_DIRECTION_RELATIVE:
    case PHOTO_BULLET_TRANSFORM_CHANGE_DIRECTION_AIMED:
    case PHOTO_BULLET_TRANSFORM_CHANGE_DIRECTION_ABSOLUTE:
        this->activeTransformFlags |= record->kind;
        this->exStates[3].directionChangeAngle =
            record->payload.directionChangeAngle;
        this->exStates[3].directionChangeSpeed =
            record->payload.directionChangeSpeed > -999.0f
                ? record->payload.directionChangeSpeed
                : this->speed;
        this->exStates[3].timer = 0;
        this->exStates[3].directionChangeIntervalFrames =
            record->payload.directionChangeIntervalFrames;
        this->exStates[3].directionChangeRepeatCount =
            record->payload.directionChangeRepeatCount;
        this->exStates[3].directionChangesCompleted = 0;
        break;

    case PHOTO_BULLET_TRANSFORM_BOUNCE_ALL_EDGES:
    case PHOTO_BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM:
        this->activeTransformFlags |= record->kind;
        if (record->payload.bounceSpeed >= 0.0f)
            this->exStates[4].bounceSpeed = record->payload.bounceSpeed;
        else
            this->exStates[4].bounceSpeed = this->speed;
        this->exStates[4].bounceLimit = record->payload.bounceLimit;
        this->exStates[4].bouncesCompleted = 0;
        break;

    case PHOTO_BULLET_TRANSFORM_WRAP_X:
    case PHOTO_BULLET_TRANSFORM_WRAP_Y:
        this->activeTransformFlags |= record->kind;
        this->exStates[6].timer = record->payload.durationFrames;
        break;

    case PHOTO_BULLET_TRANSFORM_WAIT:
        this->activeTransformFlags |= record->kind;
        this->exStates[5].timer = record->payload.durationFrames;
        break;

    case PHOTO_BULLET_TRANSFORM_SET_CULL_DELAY:
        this->offscreenCullDelayFrames = record->payload.durationFrames;
        ++this->transformIndex;
        goto nextRecord;

    case PHOTO_BULLET_TRANSFORM_SET_SPRITE:
        g_PhotoBulletManager->anmSpawner->InitializeVm(
            &this->vm,
            g_PhotoBulletScriptBases[record->payload.int0] +
                record->payload.int1);
        ++this->transformIndex;
        goto nextRecord;

    case PHOTO_BULLET_TRANSFORM_DESPAWN:
        this->state = 3;
        break;

    case PHOTO_BULLET_TRANSFORM_PLAY_SOUND:
        g_PhotoBulletSoundPlayer.PlaySoundPositionedByIdx(
            record->payload.soundIndex, this->position.x);
        ++this->transformIndex;
        goto nextRecord;

    case PHOTO_BULLET_TRANSFORM_SPAWN_CHILD_PATTERN:
        {
            PhotoBulletSpawnDescriptor pattern;
            i32 fadeParent;

            pattern.position = this->position;
            fadeParent = record->payload.packedChildPattern & 0x80000000;
            pattern.aimMode =
                (static_cast<u32>(record->payload.packedChildPattern) &
                 0x7f000000) >> 24;
            pattern.bulletType =
                (static_cast<u32>(record->payload.packedChildPattern) &
                 0x00ff0000) >> 16;
            pattern.color =
                (static_cast<u32>(record->payload.packedChildPattern) &
                 0x0000ff00) >> 8;
            pattern.transformStartIndex =
                record->payload.packedChildPattern & 0xff;
            pattern.count1 = static_cast<i16>(record->payload.childCount1);
            pattern.speed1 = record->payload.childSpeed1;
            pattern.speed2 = record->payload.childSpeed2;

            ++record;
            ++this->transformIndex;
            pattern.count2 = static_cast<i16>(record->payload.childCount2);
            pattern.transformFlags = record->payload.childTransformFlags;
            pattern.angle = record->payload.float0;
            pattern.angleStep = record->payload.float1;
            memcpy(pattern.transforms, this->transforms,
                   sizeof(pattern.transforms));
            g_PhotoBulletManager->SpawnBulletPattern(&pattern);
            ++this->transformIndex;
            if (fadeParent != 0)
                this->BeginDespawn();
            else
                goto nextRecord;
        }
        break;

    case PHOTO_BULLET_TRANSFORM_SET_FIELD_330:
        this->field330 = record->payload.int0;
        ++this->transformIndex;
        goto nextRecord;

    case PHOTO_BULLET_TRANSFORM_JUMP:
        this->transformIndex = record->payload.int0;
        goto nextRecord;

    default:
        break;
    }

    ++this->transformIndex;
}

// FUNCTION: TH095 0x00406CC0; TH08 0x00430E10 is the adjacent source oracle.
i32 PhotoBulletManagerView::SpawnBulletPattern(
    PhotoBulletSpawnDescriptor *descriptor)
{
    i32 index2;
    i32 index1;
    f32 angleToPlayer;

    angleToPlayer = g_PhotoBulletPlayer->AngleFromPoint(&descriptor->position);
    for (index2 = 0; index2 < descriptor->count2; ++index2)
    {
        for (index1 = 0; index1 < descriptor->count1; ++index1)
        {
            if (this->SpawnSingleBullet(
                    descriptor, index1, index2, angleToPlayer) != 0)
                goto doneSpawning;
        }
    }

doneSpawning:
    if ((descriptor->transformFlags &
         PHOTO_BULLET_TRANSFORM_PLAY_SPAWN_SOUND) != 0)
    {
        g_PhotoBulletSoundPlayer.PlaySoundPositionedByIdx(
            descriptor->spawnSound, descriptor->position.x);
    }
    return 0;
}

// FUNCTION: TH095 0x004077A0.
i32 PhotoBulletView::BeginDespawn()
{
    if (this->state == 2 || this->state == 1)
    {
        this->state = 3;
        this->vm.pendingInterrupt = 1;
        this->stateTimer = 0;
        return 1;
    }
    return 0;
}

// FUNCTION: TH095 0x00405850.
void PhotoBulletView::Deactivate()
{
    this->state = 0;
    this->stateTimer = 0;
    this->activeTimer = 0;
}

static inline i32 PhotoBulletIsOutsidePlayfield(
    PhotoBulletVector *position, f32 width, f32 height)
{
    return position->x + width <= -192.0f ||
           position->x - width >= 192.0f ||
           position->y + height <= 0.0f ||
           position->y - height >= 448.0f;
}

// FUNCTION: TH095 0x00406D80; TH08 0x00432210 is the adjacent source oracle.
#pragma var_order(magnitude, this)
void PhotoBulletView::UpdateDeceleration()
{
    f32 magnitude;

    if (this->exStates[0].timer <= 16)
    {
        magnitude =
            5.0f - (5.0f * (f32)this->exStates[0].timer) / 16.0f;
        this->velocity.FromAngleMagnitude(
            this->angle, magnitude + this->speed);
    }
    else
    {
        this->activeTransformFlags ^= PHOTO_BULLET_TRANSFORM_DECELERATE;
    }

    this->exStates[0].timer++;
}

// FUNCTION: TH095 0x00406E20; TH08 0x004322B0 is the adjacent source oracle.
#pragma var_order(delta, this)
void PhotoBulletView::UpdateVectorAcceleration()
{
    if (this->exStates[1].timer >= this->exStates[1].durationFrames)
    {
        this->activeTransformFlags &=
            ~PHOTO_BULLET_TRANSFORM_ACCELERATE_VECTOR;
    }
    else
    {
        this->speed +=
            g_AnmGameSpeed * this->exStates[1].accelerationMagnitude;
        this->velocity += this->exStates[1].vector * g_AnmGameSpeed;

        if (fabsf(this->velocity.x) > 0.0001f ||
            fabsf(this->velocity.y) > 0.0001f)
        {
            this->angle =
                (f32)atan2(this->velocity.y, this->velocity.x);
        }
    }

    this->exStates[1].timer++;
}

// FUNCTION: TH095 0x00406F90; TH08 0x00432390 is the adjacent source oracle.
void PhotoBulletView::UpdatePolarAcceleration()
{
    if (this->exStates[2].timer >= this->exStates[2].durationFrames)
    {
        this->activeTransformFlags &=
            ~PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR;
    }
    else
    {
        this->angle = AddNormalizeAngle(
            this->angle, g_AnmGameSpeed * this->exStates[2].angleDelta);
        this->speed += g_AnmGameSpeed * this->exStates[2].speedDelta;
        this->velocity.FromAngleMagnitude(this->angle, this->speed);
    }

    this->exStates[2].timer++;
}

// FUNCTION: TH095 0x00407050; TH08 0x00432460 is the adjacent source oracle.
#pragma var_order(magnitude, this)
void PhotoBulletView::UpdateRelativeDirectionChange()
{
    f32 magnitude;

    if (this->exStates[3].timer >=
        this->exStates[3].directionChangeIntervalFrames)
    {
        if (this->transformSound >= 0)
            g_PhotoBulletSoundPlayer.PlaySoundByIdx(this->transformSound, 0);
        this->exStates[3].directionChangesCompleted += 1;
        if (this->exStates[3].directionChangesCompleted >=
            this->exStates[3].directionChangeRepeatCount)
        {
            this->activeTransformFlags &=
                ~PHOTO_BULLET_TRANSFORM_CHANGE_DIRECTION_RELATIVE;
        }
        this->angle += this->exStates[3].directionChangeAngle;
        *reinterpret_cast<i32 *>(&this->speed) =
            *reinterpret_cast<i32 *>(&this->exStates[3].directionChangeSpeed);
        magnitude = this->speed;
        this->exStates[3].timer = 0;
    }
    else
    {
        magnitude =
            this->speed -
            ((f32)this->exStates[3].timer * this->speed) /
                this->exStates[3].directionChangeIntervalFrames;
    }

    this->velocity.FromAngleMagnitude(this->angle, magnitude);
    this->exStates[3].timer++;
}

// FUNCTION: TH095 0x004071A0; TH08 0x004325A0 is the adjacent source oracle.
#pragma var_order(magnitude, this)
void PhotoBulletView::UpdateAbsoluteDirectionChange()
{
    f32 magnitude;

    if (this->exStates[3].timer >=
        this->exStates[3].directionChangeIntervalFrames)
    {
        if (this->transformSound >= 0)
            g_PhotoBulletSoundPlayer.PlaySoundByIdx(this->transformSound, 0);
        this->exStates[3].directionChangesCompleted += 1;
        if (this->exStates[3].directionChangesCompleted >=
            this->exStates[3].directionChangeRepeatCount)
        {
            this->activeTransformFlags &=
                ~PHOTO_BULLET_TRANSFORM_CHANGE_DIRECTION_ABSOLUTE;
        }
        *reinterpret_cast<i32 *>(&this->angle) =
            *reinterpret_cast<i32 *>(&this->exStates[3].directionChangeAngle);
        *reinterpret_cast<i32 *>(&this->speed) =
            *reinterpret_cast<i32 *>(&this->exStates[3].directionChangeSpeed);
        magnitude = this->speed;
        this->exStates[3].timer = 0;
    }
    else
    {
        magnitude =
            this->speed -
            ((f32)this->exStates[3].timer * this->speed) /
                this->exStates[3].directionChangeIntervalFrames;
    }

    this->velocity.FromAngleMagnitude(this->angle, magnitude);
    this->exStates[3].timer++;
}

// FUNCTION: TH095 0x004072E0; TH08 0x004326E0 is the adjacent source oracle.
#pragma var_order(magnitude, this)
void PhotoBulletView::UpdateAimedDirectionChange()
{
    f32 magnitude;

    if (this->exStates[3].timer >=
        this->exStates[3].directionChangeIntervalFrames)
    {
        if (this->transformSound >= 0)
            g_PhotoBulletSoundPlayer.PlaySoundByIdx(this->transformSound, 0);
        this->exStates[3].directionChangesCompleted += 1;
        if (this->exStates[3].directionChangesCompleted >=
            this->exStates[3].directionChangeRepeatCount)
        {
            this->activeTransformFlags &=
                ~PHOTO_BULLET_TRANSFORM_CHANGE_DIRECTION_AIMED;
        }
        this->angle = AddNormalizeAngle(
            g_PhotoBulletPlayer->AngleFromPoint(&this->position),
            this->exStates[3].directionChangeAngle);
        *reinterpret_cast<i32 *>(&this->speed) =
            *reinterpret_cast<i32 *>(&this->exStates[3].directionChangeSpeed);
        magnitude = this->speed;
        this->exStates[3].timer = 0;
    }
    else
    {
        magnitude =
            this->speed -
            ((f32)this->exStates[3].timer * this->speed) /
                this->exStates[3].directionChangeIntervalFrames;
    }

    this->velocity.FromAngleMagnitude(this->angle, magnitude);
    this->exStates[3].timer++;
}

// FUNCTION: TH095 0x00407440; TH08 0x00432830 is the adjacent source oracle.
#pragma var_order(magnitude, this)
void PhotoBulletView::UpdateBoundaryBounce()
{
    f32 magnitude;

    if (PhotoBulletIsOutsidePlayfield(&this->position, 0.0f, 0.0f))
    {
        if (this->transformSound >= 0)
            g_PhotoBulletSoundPlayer.PlaySoundByIdx(this->transformSound, 0);

        if (this->position.x < -192.0f || this->position.x >= 192.0f)
        {
            this->angle = -this->angle - 3.1415927f;
            this->angle = AddNormalizeAngle(this->angle, 0.0f);
        }

        if (this->position.y < 0.0f ||
            (this->position.y >= 448.0f &&
             (this->activeTransformFlags &
              PHOTO_BULLET_TRANSFORM_BOUNCE_ALL_EDGES) != 0))
        {
            this->angle = -this->angle;
        }

        *reinterpret_cast<i32 *>(&magnitude) =
            *reinterpret_cast<i32 *>(&this->exStates[4].bounceSpeed);
        this->velocity.FromAngleMagnitude(this->angle, magnitude);
        this->exStates[4].bouncesCompleted += 1;
        if (this->exStates[4].bouncesCompleted >=
            this->exStates[4].bounceLimit)
        {
            this->activeTransformFlags &=
                ~(PHOTO_BULLET_TRANSFORM_BOUNCE_ALL_EDGES |
                  PHOTO_BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM);
        }
    }
}

// FUNCTION: TH095 0x00407620; TH08 0x004329F0 is the adjacent source oracle.
void PhotoBulletView::UpdateHorizontalWrap()
{
    if (this->position.x < -192.0f)
        this->position.x += 384.0f;
    else if (this->position.x > 192.0f)
        this->position.x -= 384.0f;

    if (this->exStates[6].timer <= 0)
        this->activeTransformFlags ^= PHOTO_BULLET_TRANSFORM_WRAP_X;
    else
        this->exStates[6].timer--;
}

// FUNCTION: TH095 0x004076E0; TH08 0x00432AA0 is the adjacent source oracle.
void PhotoBulletView::UpdateVerticalWrap()
{
    if (this->position.y < 0.0)
        this->position.y += 448.0f;
    else if (this->position.y > 448.0f)
        this->position.y -= 448.0f;

    if (this->exStates[6].timer <= 0)
        this->activeTransformFlags ^= PHOTO_BULLET_TRANSFORM_WRAP_Y;
    else
        this->exStates[6].timer--;
}

// FUNCTION: TH095 0x00407820.
#pragma var_order(index, first, previous, bullet, this)
PhotoBulletView *PhotoBulletManagerView::CapturePhotoTargets(
    PhotoBulletVector *position, PhotoBulletVector *size)
{
    PhotoBulletView *bullet = &this->bullets[0];
    PhotoBulletView *first = NULL;
    PhotoBulletView *previous;
    i32 index;

    this->capturePosition = *position;
    this->captureSize = *size;

    PhotoBulletVector halfSize = *size / 2.0f;
    PhotoBulletVector minimum = *position - halfSize;
    PhotoBulletVector maximum = *position + halfSize;

    for (index = 0; index < 0x640; ++index, ++bullet)
    {
        if (bullet->state == 0 || bullet->state == 3)
            continue;
        if (bullet->captureDisabled != 0)
            continue;

        PhotoBulletVector bulletMinimum =
            bullet->position - bullet->collisionSize / 2.0f;
        PhotoBulletVector bulletMaximum =
            bullet->position + bullet->collisionSize / 2.0f;
        if (minimum.x > bulletMaximum.x || bulletMinimum.x > maximum.x ||
            minimum.y > bulletMaximum.y || bulletMinimum.y > maximum.y)
        {
            continue;
        }
        if (first == NULL)
            first = bullet;
        else
            previous->nextCaptured = bullet;
        bullet->nextCaptured = NULL;
        previous = bullet;
    }

    if (g_PhotoBulletGlobalState->suppressesPhotoSound == 0)
        g_PhotoBulletSoundPlayer.PlaySoundByIdx(0x0f, 0);
    return first;
}

// FUNCTION: TH095 0x00407C90.
#pragma var_order(vmId, maximum, minimum, bulletMaximum, bulletMinimum, halfSize, vm, index, bullet, this)
i32 PhotoBulletManagerView::ClearCapturedBullets()
{
    PhotoBulletView *bullet = &this->bullets[0];
    i32 index;
    AnmVm *vm;

    PhotoBulletVector halfSize = this->captureSize / 2.0f;
    PhotoBulletVector minimum = this->capturePosition - halfSize;
    PhotoBulletVector maximum = this->capturePosition + halfSize;

    for (index = 0; index < 0x640; ++index, ++bullet)
    {
        if (bullet->state == 0 || bullet->state == 3)
            continue;
        if (bullet->captureDisabled != 0)
            continue;

        PhotoBulletVector bulletMinimum =
            bullet->position - bullet->collisionSize / 2.0f;
        PhotoBulletVector bulletMaximum =
            bullet->position + bullet->collisionSize / 2.0f;

        if (minimum.x > bulletMaximum.x || bulletMinimum.x > maximum.x ||
            minimum.y > bulletMaximum.y || bulletMinimum.y > maximum.y)
        {
            continue;
        }

        bullet->Deactivate();
        AnmVmId vmId = this->anmSpawner->CreateVm(
            0x126, &bullet->position);
        vm = g_AnmManager->GetVm(vmId);
        if (bullet->vm.loadedSprite != NULL)
        {
            if (bullet->vm.loadedSprite->widthPx <= 16.0f)
                vm->color1.color =
                    g_PhotoBulletColors16[bullet->color];
            else if (bullet->vm.loadedSprite->widthPx <= 32.0f)
                vm->color1.color =
                    g_PhotoBulletColors8[bullet->color];
            else
                vm->color1.color =
                    g_PhotoBulletColors4[bullet->color];
        }
        bullet->nextCaptured = NULL;
        g_ItemManager->Spawn(
            0, reinterpret_cast<Float3 *>(&bullet->position),
            vm->color1.color);
    }
    return 0;
}

// FUNCTION: TH095 0x004081B0.
void PhotoBulletManagerView::DespawnAllBullets()
{
    PhotoBulletView *bullet = &this->bullets[0];
    for (i32 index = 0; index < 0x640; ++index, ++bullet)
    {
        if (bullet->state == 0 || bullet->state == 3)
            continue;
        bullet->BeginDespawn();
    }
}

// FUNCTION: TH095 0x00408220.
#pragma var_order(lowerInner, upperInner, maximum, minimum, index, score, bullet, this)
i32 PhotoBulletManagerView::CountNearbyTargets(
    PhotoBulletVector *position, f32 radius)
{
    PhotoBulletView *bullet = &this->bullets[0];
    i32 score = 0;
    i32 index;
    radius *= radius;

    for (index = 0; index < 0x640; ++index, ++bullet)
    {
        if (bullet->state == 0 || bullet->state == 3)
            continue;
        {
            PhotoBulletVector minimum =
                bullet->position - bullet->collisionSize / 2.0f;
            PhotoBulletVector maximum =
                bullet->position + bullet->collisionSize / 2.0f;
            PhotoBulletVector upperInner = minimum;
            PhotoBulletVector lowerInner = maximum;
            upperInner.y += bullet->collisionSize.y;
            lowerInner.y -= bullet->collisionSize.y;

            if ((position->x - minimum.x) * (position->x - minimum.x) +
                        (position->y - minimum.y) *
                            (position->y - minimum.y) <=
                    radius ||
                (position->x - upperInner.x) *
                            (position->x - upperInner.x) +
                        (position->y - upperInner.y) *
                            (position->y - upperInner.y) <=
                    radius ||
                (position->x - maximum.x) * (position->x - maximum.x) +
                        (position->y - maximum.y) *
                            (position->y - maximum.y) <=
                    radius ||
                (position->x - lowerInner.x) *
                            (position->x - lowerInner.x) +
                        (position->y - lowerInner.y) *
                            (position->y - lowerInner.y) <=
                    radius)
            {
                if (bullet->vm.loadedSprite != NULL)
                {
                    if (bullet->vm.loadedSprite->widthPx <= 8.0f)
                        score += 1;
                    else if (bullet->vm.loadedSprite->widthPx <= 16.0f)
                        score += 1;
                    else if (bullet->vm.loadedSprite->widthPx <= 32.0f)
                        score += 4;
                    else if (bullet->vm.loadedSprite->widthPx <= 64.0f)
                        score += 10;
                }
                else
                    utils::DebugPrint("Bullet Miss\n");
            }
        }
    }
    return score;
}

// FUNCTION: TH095 0x004058C0.
i32 PhotoBulletManagerView::Draw()
{
    for (i32 bucketIndex = 0; bucketIndex < 6; ++bucketIndex)
    {
        this->DrawBucket(bucketIndex);
    }
    return 1;
}

// FUNCTION: TH095 0x00405900.
i32 PhotoBulletManagerView::DrawBucket(i32 bucketIndex)
{
    PhotoBulletView *bullet = this->drawBucketHeads[bucketIndex];
    while (bullet != NULL)
    {
        PhotoToScreen(
            &bullet->vm.position,
            reinterpret_cast<const Float3 *>(&bullet->position));
        if (((bullet->vm.flagsWord >> 27) & 1) != 0)
        {
            f32 rotationZ =
                AddNormalizeAngle(bullet->angle, 1.5707964f);
            AnmVm *vm = &bullet->vm;
            vm->rotation.z = rotationZ;
            vm->flagsWord |= 4;
        }
        g_AnmManager->Draw(&bullet->vm);
        bullet = bullet->nextInDrawBucket;
    }
    return 1;
}

// FUNCTION: TH095 0x004059C0.
i32 __fastcall PhotoBulletManagerView::OnUpdate(
    PhotoBulletManagerView *bulletManager)
{
    if ((g_PhotoBulletGlobalState->suppressesBulletCallbacks |
         g_PhotoBulletGlobalState->unknownFlag0) != 0)
    {
        return 1;
    }
    return bulletManager->Update();
}

// FUNCTION: TH095 0x00405A00.
i32 __fastcall PhotoBulletManagerView::OnDraw(
    PhotoBulletManagerView *bulletManager)
{
    if (((g_PhotoBulletGlobalState->flags >> 2) & 1) != 0)
    {
        return 1;
    }
    return bulletManager->Draw();
}

// FUNCTION: TH095 0x00405120.
i32 PhotoBulletManagerView::Update()
{
    PhotoBulletView *bullet = &this->bullets[0];

    this->drawBucketHeads[5] = NULL;
    this->drawBucketHeads[4] = NULL;
    this->drawBucketHeads[3] = NULL;
    this->drawBucketHeads[2] = NULL;
    this->drawBucketHeads[1] = NULL;
    this->drawBucketHeads[0] = NULL;
    this->drawBucketTails[5] = NULL;
    this->drawBucketTails[4] = NULL;
    this->drawBucketTails[3] = NULL;
    this->drawBucketTails[2] = NULL;
    this->drawBucketTails[1] = NULL;
    this->drawBucketTails[0] = NULL;
    this->activeBulletCount = 0;

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
                    if (bullet->exStates[5].timer <= 0)
                        bullet->activeTransformFlags ^= 0x008000;
                    else
                        bullet->exStates[5].timer--;
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
        if (this->drawBucketHeads[bullet->drawBucketIndex] != NULL)
        {
            this->drawBucketTails[bullet->drawBucketIndex]
                ->nextInDrawBucket = bullet;
        }
        else
        {
            this->drawBucketHeads[bullet->drawBucketIndex] = bullet;
        }
        this->drawBucketTails[bullet->drawBucketIndex] = bullet;
        bullet->nextInDrawBucket = NULL;
        this->activeBulletCount++;
        bullet->stateTimer.Tick();
    }
    return 1;
}

} // namespace th095
