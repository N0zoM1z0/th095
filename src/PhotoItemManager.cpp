#include "PhotoItemManager.hpp"

namespace th095
{

struct ItemAnmSpawnerView
{
    void InitializeVm(AnmVm *vm, i32 scriptIndex);
};

struct ItemBulletManagerView
{
    u8 unknown000000[0x27c5b0];
    ItemAnmSpawnerView *anmSpawner;
};

extern ItemBulletManagerView *g_PhotoBulletManager;

struct ItemPhotoGameView
{
    u8 unknown0000[0x1e30];
    Float3 playerPosition;            // +0x1e30
    u8 unknown1e3c[0x29bc - 0x1e3c];
    f32 cameraCharge;                 // +0x29bc
    u8 unknown29c0[0x29e4 - 0x29c0];
    i32 photoIndex;                   // +0x29e4
    u8 unknown29e8[0x29f0 - 0x29e8];
    u32 cameraFlags;                  // +0x29f0
    u8 unknown29f4[0x2a28 - 0x29f4];
    Float3 photoTargetBoundsMin;      // +0x2a28
    Float3 photoTargetBoundsMax;      // +0x2a34
};

struct ItemGlobalStateView
{
    u8 unknown000[0xfc];
    union
    {
        u32 flags;
        struct
        {
            u32 blockItemUpdate0 : 1;
            u32 unknownFlag1 : 1;
            u32 blockItemUpdateAndDraw : 1;
            u32 unknownFlags3 : 7;
            u32 blockItemUpdate1 : 1;
            u32 unknownFlags11 : 21;
        };
    };
};

struct ItemSoundPlayerView
{
    void PlaySoundPositionedByIdx(i32 soundIndex, f32 pan);
};

extern ItemPhotoGameView *g_PhotoGame;
extern ItemGlobalStateView *g_PhotoGlobalState;

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
    ItemPhotoGameView *game, i32 photoIndex)
{
    game->cameraCharge += static_cast<f32>(photoIndex) * 0.0002f + 0.0016f;
    if (game->cameraCharge > 1.0f)
        game->cameraCharge = 1.0f;
}

static __forceinline void AddFixedItemCameraCharge(ItemPhotoGameView *game)
{
    game->cameraCharge += 0.004f;
    if (game->cameraCharge > 1.0f)
        game->cameraCharge = 1.0f;
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
            locals.direction = g_PhotoGame->playerPosition - locals.item->position;
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
        if (!(g_PhotoGame->photoTargetBoundsMin.x > locals.boundsMax.x ||
              g_PhotoGame->photoTargetBoundsMin.y > locals.boundsMax.y ||
              g_PhotoGame->photoTargetBoundsMax.x < locals.boundsMin.x ||
              g_PhotoGame->photoTargetBoundsMax.y < locals.boundsMin.y))
        {
            locals.item->active = 0;
            if ((g_PhotoGame->cameraFlags & 1) != 0)
            {
                AddIndexedItemCameraCharge(
                    g_PhotoGame, g_PhotoGame->photoIndex);
            }
            else
            {
                AddFixedItemCameraCharge(g_PhotoGame);
            }
            if (((g_PhotoGlobalState->flags >> 9) & 1) == 0)
            {
                reinterpret_cast<ItemSoundPlayerView *>(&g_SoundPlayer)
                    ->PlaySoundPositionedByIdx(0x14, locals.item->position.x);
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
            g_PhotoGlobalState->blockItemUpdate0,
            g_PhotoGlobalState->blockItemUpdateAndDraw) != 0)
    {
        return 1;
    }
    if (g_PhotoGlobalState->blockItemUpdate1 != 0)
    {
        return 1;
    }
    return manager->Update();
}

i32 __fastcall PhotoItemManagerView::OnDraw(PhotoItemManagerView *manager)
{
    if (g_PhotoGlobalState->blockItemUpdateAndDraw != 0)
    {
        return 1;
    }
    return manager->Draw();
}

i32 PhotoItemManagerView::Spawn(i32 type, Float3 *position, u32 color)
{
    PhotoItemView *item = &this->items[0];
    i32 index = 0;

    goto checkSlot;
nextSlot:
    index++;
    item++;
checkSlot:
    if (index >= 150)
    {
        goto finished;
    }
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
    g_PhotoBulletManager->anmSpawner->InitializeVm(&item->vm, 0x120);
    item->vm.color1.color = color;
finished:
    return 0;
}

} // namespace th095
