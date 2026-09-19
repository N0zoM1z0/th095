// VC7 emission adapter for EclExtended callbacks only.  Normal callbacks use
// the canonical PhotoBulletView/PhotoBulletManagerView owner declarations.
struct ExtendedBulletView
{
    u32 flags;
    AnmVm vm;
    ExtendedVector position;
    ExtendedVector velocity;
    ExtendedVector acceleration;
    f32 speed;
    u32 unknown2f8[2];
    f32 angle;
    u32 unknown304[2];
    ExtendedVector collisionSize;
    ZunTimer stateTimer;
    ZunTimer activeTimer;
    i32 ownerTag;
    u8 unknown334[0x14];
    union
    {
        i32 field348;
        u32 activeTransformFlags;
    };
    union
    {
        i32 field34c;
        u32 transformFlags;
    };
    i16 unknown350;
    u16 state;
    u16 offscreenFrames;
    u16 unknown356;
    ExtendedBulletView *nextInDrawBucket;
    i32 field35c;
    i32 field360;
    i32 transformSound;
    i32 transformIndex;
    i32 drawBucketIndex;
    u8 unknown370[0x2e4];
    i8 collisionDisabled;
    u8 unknown655;
    i16 bulletType;
    i16 color;
    u8 trailingAlignment65A[2];

    void ReinitializeDirect();
    void ReinitializeShifted();
};

typedef char ExtendedBulletSize65C[
    (sizeof(ExtendedBulletView) == 0x65c) ? 1 : -1];
typedef char ExtendedBulletPositionAt2D0[
    (offsetof(ExtendedBulletView, position) == 0x2d0) ? 1 : -1];
typedef char ExtendedBulletVelocityAt2DC[
    (offsetof(ExtendedBulletView, velocity) == 0x2dc) ? 1 : -1];
typedef char ExtendedBulletSpeedAt2F4[
    (offsetof(ExtendedBulletView, speed) == 0x2f4) ? 1 : -1];
typedef char ExtendedBulletAngleAt300[
    (offsetof(ExtendedBulletView, angle) == 0x300) ? 1 : -1];
typedef char ExtendedBulletOwnerAt330[
    (offsetof(ExtendedBulletView, ownerTag) == 0x330) ? 1 : -1];
typedef char ExtendedBulletField348At348[
    (offsetof(ExtendedBulletView, field348) == 0x348) ? 1 : -1];
typedef char ExtendedBulletField34CAt34C[
    (offsetof(ExtendedBulletView, field34c) == 0x34c) ? 1 : -1];

struct ExtendedBulletManager
{
    u8 unknown000[0x4c];
    ExtendedBulletView bullets[0x641];
    u8 unknown27C5A8[8];
    ExtendedAnmSpawner *bulletAnm;
};

typedef char ExtendedBulletManagerBulletsAt4C[
    (offsetof(ExtendedBulletManager, bullets) == 0x4c) ? 1 : -1];
typedef char ExtendedBulletManagerAnmAt27C5B0[
    (offsetof(ExtendedBulletManager, bulletAnm) == 0x27c5b0) ? 1 : -1];
