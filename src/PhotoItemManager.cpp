#ifdef TH095_MATCH_EXACT
#include "PhotoItemManagerExact.inl"
#else
#include "PhotoItemManager.hpp"
#include "PhotoBulletManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoGameTask.hpp"
#include "PhotoGamePlayer.hpp"
#include "SoundPlayer.hpp"

namespace th095
{

#ifndef DIFFBUILD
#define g_ItemManager \
    TH095_RUNTIME_GLOBAL_PTR(PhotoItemManagerView, g_RuntimeItemManagerOwner)
#endif

#define g_PhotoBulletManager \
    TH095_RUNTIME_GLOBAL_PTR(PhotoBulletManagerView, g_RuntimeBulletManagerOwner)


Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

static inline i32 ItemEitherFlag(i32 first, i32 second)
{
    return first | second;
}

PhotoItemView::PhotoItemView()
{
}

PhotoItemView::~PhotoItemView()
{
}

PhotoItemManagerView::PhotoItemManagerView()
{
    utils::DebugPrint("initialize ItemInf\n");
    memset(this, 0, sizeof(*this));
    g_ItemManager = this;
}

PhotoItemManagerView::~PhotoItemManagerView()
{
    utils::DebugPrint("shutdown ItemInf\n");
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);
    g_ItemManager = NULL;
}

i32 PhotoItemManagerView::Initialize()
{
    return 0;
}

PhotoItemManagerView *PhotoItemManagerView::Create()
{
    struct
    {
        PhotoItemManagerView *manager;
        ChainElem *elem;
    } locals;

#define manager locals.manager
#define elem locals.elem

    manager = new PhotoItemManagerView();
    if (manager->Initialize() != 0)
    {
        goto failure;
    }

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoItemManagerView::OnUpdate));
    elem->arg = manager;
    g_Chain.AddToCalcChain(elem, 0xf);
    manager->calcChain = elem;

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoItemManagerView::OnDraw));
    elem->arg = manager;
    g_Chain.AddToDrawChain(elem, 0x10);
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

void PhotoItemManagerView::Destroy()
{
    PhotoItemManagerView *manager = this;
    if (manager != NULL)
    {
        delete manager;
        manager = NULL;
    }
}

static __forceinline Float3 ScaleItemVector(f32 scalar, const Float3 &value)
{
    return Float3(scalar * value.x, scalar * value.y, scalar * value.z);
}

static __forceinline void NormalizeAndScaleItemVelocity(
    const Float3 &direction, Float3 *velocity, f32 acceleration)
{
    D3DXVec3Normalize(
        reinterpret_cast<D3DXVECTOR3 *>(velocity),
        reinterpret_cast<const D3DXVECTOR3 *>(&direction));
    *velocity *= acceleration;
}

static __forceinline void AddIndexedItemCameraCharge(
    PhotoGameUpdateView *game, i32 photoIndex)
{
    game->camera.charge += static_cast<f32>(photoIndex) * 0.0002f + 0.0016f;
    if (game->camera.charge > 1.0f)
        game->camera.charge = 1.0f;
}

static __forceinline void AddFixedItemCameraCharge(PhotoGameUpdateView *game)
{
    game->camera.charge += 0.004f;
    if (game->camera.charge > 1.0f)
        game->camera.charge = 1.0f;
}

i32 PhotoItemManagerView::Update()
{
    struct UpdateLocals
    {
        Float3 boundsMin;
        Float3 boundsMax;
        i32 index;
        PhotoItemView *item;
        Float3 direction;
    } locals;
    locals.item = &this->items[0];
    for (locals.index = 0; locals.index < 150; locals.index++, locals.item++)
    {
        if (locals.item->active == 0)
        {
            continue;
        }
        if (locals.item->timer < 4)
        {
            goto tick;
        }

        if (locals.item->timer < 20)
        {
            locals.item->position +=
                ScaleItemVector(
                    20.0f - static_cast<f32>(locals.item->timer),
                    locals.item->velocity) / 20.0f;
        }
        else
        {
            locals.direction = g_RuntimePlayerOwner->playerPosition - locals.item->position;
            NormalizeAndScaleItemVelocity(
                locals.direction, &locals.item->velocity,
                locals.item->acceleration);
            locals.item->position += locals.item->velocity;
            if (locals.item->acceleration < 8.0f)
            {
                locals.item->acceleration += 0.1f;
            }
        }

        locals.boundsMin.x = locals.item->position.x - 0.0f;
        locals.boundsMin.y = locals.item->position.y - 0.0f;
        locals.boundsMax.x = locals.item->position.x + 0.0f;
        locals.boundsMax.y = locals.item->position.y + 0.0f;
        if (!(g_RuntimePlayerOwner->photoTargetBoundsMin.x > locals.boundsMax.x ||
              g_RuntimePlayerOwner->photoTargetBoundsMin.y > locals.boundsMax.y ||
              g_RuntimePlayerOwner->photoTargetBoundsMax.x < locals.boundsMin.x ||
              g_RuntimePlayerOwner->photoTargetBoundsMax.y < locals.boundsMin.y))
        {
            locals.item->active = 0;
            if (g_RuntimePlayerOwner->camera.alternateCapture != 0)
            {
                AddIndexedItemCameraCharge(
                    g_RuntimePlayerOwner, g_RuntimePlayerOwner->camera.photoIndex);
            }
            else
            {
                AddFixedItemCameraCharge(g_RuntimePlayerOwner);
            }
            if (g_RuntimeGlobalStateOwner->photoSoundSuppressed == 0)
            {
                g_SoundPlayer.PlaySoundPositionedByIdx(
                    static_cast<SoundIdx>(0x14), locals.item->position.x);
            }
            continue;
        }

        AnmManager::ExecuteScript(&locals.item->vm);
tick:
        locals.item->timer.Tick();
    }
    return 1;
}

i32 PhotoItemManagerView::Draw()
{
    PhotoItemView *item = &this->items[0];
    for (i32 index = 0; index < 150; index++, item++)
    {
        if (item->active == 0)
        {
            continue;
        }
        if (item->timer < 4)
        {
            continue;
        }
        PhotoToScreen(&item->vm.positionOffset, &item->position);
        item->vm.Draw();
    }
    return 1;
}

i32 __fastcall PhotoItemManagerView::OnUpdate(PhotoItemManagerView *manager)
{
    if (ItemEitherFlag(
            g_RuntimeGlobalStateOwner->captureActive,
            g_RuntimeGlobalStateOwner->gameplayLoadActive) != 0)
    {
        return 1;
    }
    if (g_RuntimeGlobalStateOwner->photoTransitionActive != 0)
    {
        return 1;
    }
    return manager->Update();
}

i32 __fastcall PhotoItemManagerView::OnDraw(PhotoItemManagerView *manager)
{
    if (g_RuntimeGlobalStateOwner->gameplayLoadActive != 0)
    {
        return 1;
    }
    return manager->Draw();
}

static __forceinline void PhotoItemSpawnVmSetupPhase(PhotoItemView *item, u32 color)
{
    u8 compilerStorage[0x2c];
    g_PhotoBulletManager->bulletAnm->InitializeVm(&item->vm, 0x120);
    item->vm.color1.color = color;
}

#define item averagedPanLocal12
i32 PhotoItemManagerView::Spawn(i32 type, Float3 *position, u32 color)
{
    PhotoItemView *item = &this->items[0];
    i32 index = 0;

    goto checkSlot;
nextSlot:
    index++;
    item++;
checkSlot:
    if (index < 150)
    {
        if (item->active != 0)
        {
            goto nextSlot;
        }

        item->active = 1;
        item->position = *position;
        item->velocity.x = g_Rng.GetRandomF32Signed() * 1.0f;
        item->velocity.y = g_Rng.GetRandomF32() * 2.0f + 2.0f;
        item->velocity.z = 0.0f;
        item->timer = 0;
        item->acceleration = 0.0f;
        PhotoItemSpawnVmSetupPhase(item, color);
    }
finished:
    return 0;
}
#undef item

} // namespace th095

#endif // TH095_MATCH_EXACT
