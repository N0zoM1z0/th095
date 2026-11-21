#define TH095_DECLARE_ANM_LOADED_INITIALIZE_VM
#include "AnmManager.hpp"
#include "AnmVmId.hpp"
#include "AnmVmLifecycle.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoGameTask.hpp"
#include "SupervisorViewportSlot.hpp"
namespace th095
{

#ifdef TH095_MATCH_EXACT
// The two positional creator entries were originally proven through this
// target-facing partial view. Production canonicalizes them onto AnmLoaded,
// but exact replay must retain the historical receiver decoration.
struct AnmLoadedPositionView
{
    i32 anmIdx;
    void *rawData;
    i32 totalEntries;
    AnmLoadedSprite *sprites;
    AnmRawInstr **scripts;
    void *textures;
    i32 postloadEntryNumber;

    AnmVmId CreateVmAtScreen(i32 scriptIndex, Float3 *position);
    AnmVmId CreateVmAtWorld(i32 scriptIndex, Float3 *position);
};
#define TH095_ANM_POSITION_RECEIVER AnmLoadedPositionView
#else
#define TH095_ANM_POSITION_RECEIVER AnmLoaded
#endif


Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

static __forceinline i32 AnmUpdateEitherFlag(i32 first, i32 second)
{
    return first | second;
}

typedef char AnmVmLifecycleNextAt0[
    (offsetof(AnmVmLifecycleView, next) == 0x0) ? 1 : -1];
#ifndef TH095_MATCH_EXACT
typedef char AnmVmLifecycleNextInDrawLayerAt4[
    (offsetof(AnmVmLifecycleView, nextInDrawLayer) == 0x4) ? 1 : -1];
#endif
typedef char AnmVmLifecyclePreviousAt8[
    (offsetof(AnmVmLifecycleView, previous) == 0x8) ? 1 : -1];
typedef char AnmVmLifecycleDrawLayerAtC[
    (offsetof(AnmVmLifecycleView, drawLayer) == 0xc) ? 1 : -1];
typedef char AnmVmLifecycleIdAt10[
    (offsetof(AnmVmLifecycleView, id) == 0x10) ? 1 : -1];
typedef char AnmManagerVmLifecycleIdAt383148[
    (offsetof(AnmManagerVmLifecycleView, nextVmId) == 0x383148) ? 1 : -1];
// FUNCTION: TH095 0x00444980.
void __fastcall AnmManager::OnUpdate(void *arg)
{
    static_cast<AnmManager *>(arg)->UpdateVms();
}

// FUNCTION: TH095 0x004449A0.
void __fastcall AnmManager::DrawLayer0(void *arg)
{
    static_cast<AnmManager *>(arg)->DrawLayer(0);
}

// FUNCTION: TH095 0x004449C0.
void __fastcall AnmManager::DrawLayer1(void *arg)
{
    static_cast<AnmManager *>(arg)->DrawLayer(1);
}

// FUNCTION: TH095 0x004449E0.
void __fastcall AnmManager::DrawLayer2(void *arg)
{
    static_cast<AnmManager *>(arg)->DrawLayer(2);
}

// FUNCTION: TH095 0x00444A00.
void __fastcall AnmManager::DrawLayer3(void *arg)
{
    static_cast<AnmManager *>(arg)->DrawLayer(3);
}

// FUNCTION: TH095 0x00444A20.
void __fastcall AnmManager::DrawLayer4(void *arg)
{
    static_cast<AnmManager *>(arg)->DrawLayer(4);
}

// FUNCTION: TH095 0x00444A40.
void __fastcall AnmManager::DrawLayer5(void *arg)
{
    static_cast<AnmManager *>(arg)->DrawLayer(5);
}

// FUNCTION: TH095 0x00444A60.
void __fastcall AnmManager::DrawLayer6(void *arg)
{
    g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.x = 0.0f;
    g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.y = 0.0f;
    g_AnmManager->screenShakeOffset.x = 0.0f;
    g_AnmManager->screenShakeOffset.y = 0.0f;
    static_cast<AnmManager *>(arg)->DrawLayer(6);
}

// FUNCTION: TH095 0x00444AB0.
void __fastcall AnmManager::DrawLayer7(void *arg)
{
    g_Supervisor.ConfigureGameplayViewport(TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW);
    static_cast<AnmManager *>(arg)->DrawLayer(7);
}

// FUNCTION: TH095 0x00444AE0.
void __fastcall AnmManager::DrawLayer8(void *arg)
{
    g_Supervisor.ConfigureGameplayViewport(TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW);
    static_cast<AnmManager *>(arg)->DrawLayer(8);
}

// FUNCTION: TH095 0x00444B10.
i32 AnmManager::UpdateVms()
{
    i32 layer;
    AnmVm *layerTails[ANM_VM_DRAW_LAYER_COUNT];
    AnmVm *vm;
    AnmVm *next;

    for (layer = 0; layer < ANM_VM_DRAW_LAYER_COUNT; layer++)
    {
        this->drawLayerHeads[layer].nextInDrawLayer = NULL;
        layerTails[layer] = &this->drawLayerHeads[layer];
    }

    this->vmsProcessedThisFrame = 0;
    vm = this->vmListHead;
    while (vm != NULL)
    {
        next = vm->next;
        if (g_RuntimeGlobalStateOwner != NULL && !vm->bypassPhotoGameSuppression &&
            (AnmUpdateEitherFlag((i32)g_RuntimeGlobalStateOwner->captureActive,
                                 g_RuntimeGlobalStateOwner->gameplayLoadActive) != 0 ||
             g_RuntimeGlobalStateOwner->capturedPhotoActive != 0 ||
             g_RuntimeGlobalStateOwner->photoTransitionActive != 0))
        {
            goto addToDrawLayer;
        }
        {
            if (vm->pendingDeletion)
            {
                reinterpret_cast<AnmManagerVmLifecycleView *>(this)->RemoveVm(
                    reinterpret_cast<AnmVmDeleteView *>(vm));
                goto vmDone;
            }
            if (AnmManager::ExecuteScript(vm))
            {
                reinterpret_cast<AnmManagerVmLifecycleView *>(this)->RemoveVm(
                    reinterpret_cast<AnmVmDeleteView *>(vm));
                goto vmDone;
            }
            goto addToDrawLayer;
        }
addToDrawLayer:
        layerTails[vm->drawLayer]->nextInDrawLayer = vm;
        layerTails[vm->drawLayer] = vm;
        vm->nextInDrawLayer = NULL;

vmDone:
        this->vmsProcessedThisFrame++;
        vm = next;
    }
    return 1;
}

// FUNCTION: TH095 0x00444C80.
i32 AnmManager::DrawLayer(i32 layer)
{
    AnmVm *vm;
    vm = this->drawLayerHeads[layer].nextInDrawLayer;
    while (vm != NULL)
    {
        if (g_RuntimeGlobalStateOwner != NULL &&
            vm->bypassPhotoGameSuppression == 0 &&
            g_RuntimeGlobalStateOwner->gameplayLoadActive != 0)
        {
        }
        else if (!vm->pendingDeletion)
        {
            this->Draw(vm);
        }
        vm = vm->nextInDrawLayer;
    }
    return 1;
}

// FUNCTION: TH095 0x00444D10.
AnmVmId AnmManagerVmLifecycleView::AddVm(AnmVmLifecycleView *vm)
{
    vm->next = NULL;

    if (this->vmListHead == NULL)
    {
        vm->previous = NULL;
        this->vmListHead = vm;
        this->vmListTail = vm;
    }
    else
    {
        vm->previous = this->vmListTail;
        this->vmListTail->next = vm;
        this->vmListTail = vm;
    }

    AnmVmLifecycleView::Id *incrementReceiver = &this->nextVmId;
    (*incrementReceiver)++;
    if (this->nextVmId == AnmVmLifecycleView::Id())
    {
        this->nextVmId++;
    }
    vm->id = this->nextVmId;
    return *reinterpret_cast<AnmVmId *>(&this->nextVmId);
}

// FUNCTION: TH095 0x00444E00.
i32 AnmManagerVmLifecycleView::RemoveVm(AnmVmDeleteView *node)
{
    if (node == reinterpret_cast<AnmVmDeleteView *>(this->vmListTail))
    {
        this->vmListTail =
            reinterpret_cast<AnmVmLifecycleView *>(node->previous);
    }

    if (node->previous == NULL)
    {
        this->vmListHead =
            reinterpret_cast<AnmVmLifecycleView *>(node->next);
        if (this->vmListHead != NULL)
        {
            this->vmListHead->previous = NULL;
        }
    }
    else
    {
        node->previous->next = node->next;
        if (node->next != NULL)
        {
            node->next->previous = node->previous;
        }
    }

    delete node;
    node = NULL;
    return 0;
}

// FUNCTION: TH095 0x00444EF0.
AnmVmId AnmLoaded::CreateVm(i32 scriptIndex, i32 drawLayer)
{
    AnmVm *vm = new AnmVm;
    vm->drawLayer = drawLayer;
    this->InitializeVm(vm, scriptIndex);
    return reinterpret_cast<AnmManagerVmLifecycleView *>(g_AnmManager)
        ->AddVm(reinterpret_cast<AnmVmLifecycleView *>(vm));
}

// FUNCTION: TH095 0x00444FA0.
AnmVmId TH095_ANM_POSITION_RECEIVER::CreateVmAtScreen(
    i32 scriptIndex, Float3 *position)
{
    AnmVm *vm = new AnmVm;
    reinterpret_cast<AnmLoaded *>(this)->InitializeVm(vm, scriptIndex);
    vm->positionOffset = *position;
    return reinterpret_cast<AnmManagerVmLifecycleView *>(g_AnmManager)
        ->AddVm(reinterpret_cast<AnmVmLifecycleView *>(vm));
}

// FUNCTION: TH095 0x00445060.
AnmVmId TH095_ANM_POSITION_RECEIVER::CreateVmAtWorld(
    i32 scriptIndex, Float3 *position)
{
    AnmVm *vm = new AnmVm;
    reinterpret_cast<AnmLoaded *>(this)->InitializeVm(vm, scriptIndex);
    PhotoToScreen(&vm->positionOffset, position);
    return reinterpret_cast<AnmManagerVmLifecycleView *>(g_AnmManager)
        ->AddVm(reinterpret_cast<AnmVmLifecycleView *>(vm));
}

} // namespace th095
