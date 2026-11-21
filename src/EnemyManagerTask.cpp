#include "PhotoEnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoGameTask.hpp"

namespace th095
{

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

static inline i32 PhotoEnemyEitherFlag(i32 first, i32 second)
{
    return first | second;
}

// FUNCTION: TH095 0x004149F0.
PhotoEnemyManagerView *PhotoEnemyManagerView::Create()
{
    struct
    {
        PhotoEnemyManagerView *manager;
        ChainElem *elem;
    } locals;

#define manager locals.manager
#define elem locals.elem

    manager = new PhotoEnemyManagerView();
    if (manager->LoadResources() != ZUN_SUCCESS)
        goto failure;

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoEnemyManagerView::OnTaskUpdate));
    elem->arg = manager;
    g_Chain.AddToCalcChain(elem, 0x0c);
    manager->calcChain = elem;

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoEnemyManagerView::OnTaskDraw));
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
i32 PhotoEnemyManagerView::Draw()
{
    for (i32 groupIndex = 0; groupIndex < 4; ++groupIndex)
        this->DrawGroup(groupIndex);
    return 1;
}

// FUNCTION: TH095 0x00416230.
i32 PhotoEnemyManagerView::DrawGroup(i32 groupIndex)
{
    PhotoEnemyView *enemy = this->drawGroupHeads[groupIndex];
    while (enemy != NULL)
    {
        PhotoToScreen(&enemy->vm.positionOffset, &enemy->position);
        g_AnmManager->Draw(&enemy->vm);
        enemy = enemy->nextInDrawGroup;
    }
    return 1;
}

// FUNCTION: TH095 0x00416290.
i32 __fastcall PhotoEnemyManagerView::OnTaskUpdate(
    PhotoEnemyManagerView *manager)
{
    // VC7.1 reads gameplayLoadActive before captureActive in this call.
    // Both branches of the gate share the return-1 tail.
    if (PhotoEnemyEitherFlag(
            g_RuntimeGlobalStateOwner->captureActive,
            g_RuntimeGlobalStateOwner->gameplayLoadActive) != 0 ||
        g_RuntimeGlobalStateOwner->capturedPhotoActive != 0)
    {
        return 1;
    }
    return PhotoEnemyManagerView::OnUpdate(manager);
}

// FUNCTION: TH095 0x004162F0.
i32 __fastcall PhotoEnemyManagerView::OnTaskDraw(
    PhotoEnemyManagerView *manager)
{
    if (g_RuntimeGlobalStateOwner->gameplayLoadActive != 0)
        return 1;
    return manager->Draw();
}

} // namespace th095
