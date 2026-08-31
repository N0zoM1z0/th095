#include "EnemyManager.hpp"

namespace th095
{

struct PhotoEnemyAnmManagerDrawView
{
    ZunResult Draw(AnmVm *vm);
};

extern PhotoEnemyAnmManagerDrawView *g_PhotoEnemyAnmManager;

struct PhotoEnemyTaskGlobalStateView
{
    u8 unknown000[0xfc];
    union
    {
        u32 flags;
        struct
        {
            u32 unknownFlag0 : 1;
            u32 blockEnemyUpdate : 1;
            u32 blockEnemyUpdateAndDraw : 1;
            u32 unknownFlags3 : 29;
        };
    };
};

extern PhotoEnemyTaskGlobalStateView *g_PhotoEnemyGlobalState;

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

static inline i32 PhotoEnemyEitherFlag(i32 first, i32 second)
{
    return first | second;
}

struct PhotoEnemyManagerTaskView
{
    u8 unknown000000[0x4dc0];
    Enemy *drawGroupHeads[4];          // +0x004DC0
    u8 unknown004dd0[0x26ae20 - 0x4dd0];
    ChainElem *calcChain;              // +0x26AE20
    ChainElem *drawChain;              // +0x26AE24
    u8 unknown26ae28[8];

    PhotoEnemyManagerTaskView();
    ~PhotoEnemyManagerTaskView();
    i32 LoadResources();
    i32 Update();

    static PhotoEnemyManagerTaskView *Create();
    i32 Draw();
    i32 DrawGroup(i32 groupIndex);
    static i32 __fastcall OnUpdate(PhotoEnemyManagerTaskView *manager);
    static i32 __fastcall OnDraw(PhotoEnemyManagerTaskView *manager);
};

typedef char PhotoEnemyManagerTaskSizeIs26AE30[
    (sizeof(PhotoEnemyManagerTaskView) == 0x26ae30) ? 1 : -1];
typedef char PhotoEnemyManagerTaskDrawGroupsAt4DC0[
    (offsetof(PhotoEnemyManagerTaskView, drawGroupHeads) == 0x4dc0) ? 1 : -1];
typedef char PhotoEnemyManagerTaskChainsAt26AE20[
    (offsetof(PhotoEnemyManagerTaskView, calcChain) == 0x26ae20 &&
     offsetof(PhotoEnemyManagerTaskView, drawChain) == 0x26ae24) ? 1 : -1];

// FUNCTION: TH095 0x004149F0.
PhotoEnemyManagerTaskView *PhotoEnemyManagerTaskView::Create()
{
    struct
    {
        PhotoEnemyManagerTaskView *manager;
        ChainElem *elem;
    } locals;

#define manager locals.manager
#define elem locals.elem

    manager = new PhotoEnemyManagerTaskView();
    if (manager->LoadResources() != ZUN_SUCCESS)
        goto failure;

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoEnemyManagerTaskView::OnUpdate));
    elem->arg = manager;
    g_Chain.AddToCalcChain(elem, 0x0c);
    manager->calcChain = elem;

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoEnemyManagerTaskView::OnDraw));
    elem->arg = manager;
    g_Chain.AddToDrawChain(elem, 0x0a);
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

// FUNCTION: TH095 0x004161F0.
i32 PhotoEnemyManagerTaskView::Draw()
{
    for (i32 groupIndex = 0; groupIndex < 4; ++groupIndex)
        this->DrawGroup(groupIndex);
    return 1;
}

// FUNCTION: TH095 0x00416230.
i32 PhotoEnemyManagerTaskView::DrawGroup(i32 groupIndex)
{
    Enemy *enemy = this->drawGroupHeads[groupIndex];
    while (enemy != NULL)
    {
        PhotoToScreen(&enemy->vm.positionOffset, &enemy->position);
        g_PhotoEnemyAnmManager->Draw(&enemy->vm);
        enemy = enemy->nextInDrawGroup;
    }
    return 1;
}

// FUNCTION: TH095 0x00416290.
i32 __fastcall PhotoEnemyManagerTaskView::OnUpdate(
    PhotoEnemyManagerTaskView *manager)
{
    // Source argument order matters under VC7.1: fastcall/right-to-left
    // evaluation loads the draw-suppression bit before flag 0, as the target
    // does, while the logical short-circuit shares one return-1 tail.
    if (PhotoEnemyEitherFlag(
            g_PhotoEnemyGlobalState->unknownFlag0,
            g_PhotoEnemyGlobalState->blockEnemyUpdateAndDraw) != 0 ||
        g_PhotoEnemyGlobalState->blockEnemyUpdate != 0)
    {
        return 1;
    }
    return manager->Update();
}

// FUNCTION: TH095 0x004162F0.
i32 __fastcall PhotoEnemyManagerTaskView::OnDraw(
    PhotoEnemyManagerTaskView *manager)
{
    if (g_PhotoEnemyGlobalState->blockEnemyUpdateAndDraw != 0)
        return 1;
    return manager->Draw();
}

} // namespace th095
