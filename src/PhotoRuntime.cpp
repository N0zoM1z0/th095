#include "AnmManager.hpp"

namespace th095
{
struct PhotoEnemyEclContextView;
struct PhotoEnemyEclManagerView
{
    i32 InitializeContext(PhotoEnemyEclContextView *context, i16 subroutineId);
};

struct PhotoItemManagerView
{
    i32 Spawn(i32 type, Float3 *position, u32 color);
};
extern PhotoItemManagerView *g_PhotoItemManager;

struct PhotoTargetEnemyView
{
    u8 unknown0000[0x2dc];
    u8 mainEclContext[0x285a - 0x2dc];
    i16 pendingEclSubroutineId;
    u8 unknown285c[0x28a0 - 0x285c];
    Float3 position;
    u8 unknown28ac[0x28dc - 0x28ac];
    Float3 collisionSize;
    u8 unknown28e8[0x2bf4 - 0x28e8];
    u32 flags1;
    u32 flags2;
    u8 trailing[0x4cc0 - 0x2bfc];
};
typedef char PhotoTargetEnemySize[(sizeof(PhotoTargetEnemyView) == 0x4cc0) ? 1 : -1];
typedef char PhotoTargetEnemyPosition[(offsetof(PhotoTargetEnemyView, position) == 0x28a0) ? 1 : -1];
typedef char PhotoTargetEnemyCollision[(offsetof(PhotoTargetEnemyView, collisionSize) == 0x28dc) ? 1 : -1];
typedef char PhotoTargetEnemyFlags1[(offsetof(PhotoTargetEnemyView, flags1) == 0x2bf4) ? 1 : -1];
typedef char PhotoTargetEnemyFlags2[(offsetof(PhotoTargetEnemyView, flags2) == 0x2bf8) ? 1 : -1];

struct PhotoRuntimeView
{
    u8 unknown0000[0x4df4];
    PhotoEnemyEclManagerView *eclManager;
    u8 unknown4df8[8];
    PhotoTargetEnemyView enemies[128];

    i32 CountPhotoTargets(const Float3 *position, const Float3 *size);
};

// Keep the TH08 Float3 divide body in this call-site allocation phase.
static __forceinline Float3 DividePhotoVector(
    const Float3 &value, f32 scalar)
{
    f32 inverse;
    inverse = 1.0f / scalar;
    return Float3(
        value.x * inverse,
        value.y * inverse,
        value.z * inverse);
}

// FUNCTION: TH095 0x004168D0.
int PhotoRuntimeView::CountPhotoTargets(
    const Float3 *position, const Float3 *size)
{
    struct CountPhotoTargetLocals
    {
        i32 i;
        PhotoTargetEnemyView *enemy;
        Float3 captureMaximum;
        Float3 enemyMaximum;
        Float3 enemyMinimum;
        Float3 captureMinimum;
        i32 count;
    } locals;

    locals.enemy = &this->enemies[0];
    locals.count = 0;

    locals.captureMaximum = DividePhotoVector(*size, 2.0f);
    locals.captureMinimum = *position - locals.captureMaximum;
    // Do not fold this to +=: target VC7.1 keeps two Float3 return/copy objects.
    locals.captureMaximum = locals.captureMaximum + *position;

    for (locals.i = 0; locals.i < 128; ++locals.i, ++locals.enemy)
    {
        if ((locals.enemy->flags1 & 1U) == 0)
            continue;
        if (((locals.enemy->flags1 >> 1) & 1U) != 0)
            continue;
        if (((locals.enemy->flags1 >> 8) & 3U) != 0)
            continue;
        if (((locals.enemy->flags1 >> 4) & 1U) != 0 ||
            ((locals.enemy->flags1 >> 5) & 1U) != 0 ||
            ((locals.enemy->flags2 >> 6) & 1U) != 0)
            continue;

        locals.enemyMinimum =
            locals.enemy->position -
            DividePhotoVector(locals.enemy->collisionSize, 2.0f);
        locals.enemyMaximum =
            DividePhotoVector(locals.enemy->collisionSize, 2.0f) +
            locals.enemy->position;

        if (locals.enemyMaximum.x < locals.captureMinimum.x ||
            locals.enemyMinimum.x > locals.captureMaximum.x ||
            locals.enemyMaximum.y < locals.captureMinimum.y ||
            locals.enemyMinimum.y > locals.captureMaximum.y)
            continue;

        if (locals.enemy->pendingEclSubroutineId < 0)
        {
            locals.enemy->flags1 =
                (locals.enemy->flags1 & ~0x300U) | 0x100U;
        }
        else
        {
            this->eclManager->InitializeContext(
                reinterpret_cast<PhotoEnemyEclContextView *>(
                    locals.enemy->mainEclContext),
                locals.enemy->pendingEclSubroutineId);
        }

        g_PhotoItemManager->Spawn(
            0, &locals.enemy->position, 0xffffffffU);
        g_PhotoItemManager->Spawn(
            0, &locals.enemy->position, 0xffff0000U);
        g_PhotoItemManager->Spawn(
            0, &locals.enemy->position, 0xff00ff00U);
        g_PhotoItemManager->Spawn(
            0, &locals.enemy->position, 0xff0000ffU);
        g_PhotoItemManager->Spawn(
            0, &locals.enemy->position, 0xff00ffffU);
        ++locals.count;
    }
    return locals.count;
}
} // namespace th095
