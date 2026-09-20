#include "AnmManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoEnemy.hpp"
#include "PhotoEnemyManager.hpp"

namespace th095
{
struct PhotoEnemyEclContextView;
struct PhotoEnemyEclManagerView
{
    i32 InitializeContext(PhotoEnemyEclContextView *context, i16 subroutineId);
};
#ifndef DIFFBUILD
struct EnemyEclContext;
struct EclManager
{
    ::ZunResult CallEclSub(EnemyEclContext *context, i16 subId);
};
#define TH095_PHOTO_RUNTIME_ECL_INIT(manager, context, subroutineId) \
    reinterpret_cast<EclManager *>(manager)->CallEclSub( \
        reinterpret_cast<EnemyEclContext *>(context), (subroutineId))
#else
#define TH095_PHOTO_RUNTIME_ECL_INIT(manager, context, subroutineId) \
    (manager)->InitializeContext((context), (subroutineId))
#endif

struct PhotoItemManagerView
{
    i32 Spawn(i32 type, Float3 *position, u32 color);
};
extern PhotoItemManagerView *g_PhotoItemManager;
#ifndef DIFFBUILD
#define g_PhotoItemManager \
    TH095_RUNTIME_GLOBAL_PTR(PhotoItemManagerView, g_RuntimeItemManagerOwner)
#endif

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
int PhotoEnemyManagerView::CountPhotoTargets(
    const Float3 *position, const Float3 *size)
{
    struct CountPhotoTargetLocals
    {
        i32 i;
        PhotoEnemyView *enemy;
        Float3 captureMaximum;
        Float3 enemyMaximum;
        Float3 enemyMinimum;
        Float3 captureMinimum;
        i32 count;
    } locals;

    locals.enemy = &this->enemyPool[0];
    locals.count = 0;

    locals.captureMaximum = DividePhotoVector(*size, 2.0f);
    locals.captureMinimum = *position - locals.captureMaximum;
    // Do not fold this to +=: target VC7.1 keeps two Float3 return/copy objects.
    locals.captureMaximum = locals.captureMaximum + *position;

    for (locals.i = 0; locals.i < 128; ++locals.i, ++locals.enemy)
    {
        if (locals.enemy->active == 0)
            continue;
        if (locals.enemy->photoTarget != 0)
            continue;
        if (locals.enemy->lifecycleState != 0)
            continue;
        if (locals.enemy->hiddenFromDrawGroups != 0 ||
            ((locals.enemy->flags1 >> 5) & 1U) != 0 ||
            locals.enemy->showPhotoMarker != 0)
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

        if (locals.enemy->photoCaptureEclSubroutineId < 0)
        {
            locals.enemy->lifecycleState = 1;
        }
        else
        {
            TH095_PHOTO_RUNTIME_ECL_INIT(
                this->eclManager,
                &locals.enemy->mainEclContext,
                locals.enemy->photoCaptureEclSubroutineId);
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
