#define TH095_DECLARE_ANM_LOADED_INITIALIZE_VM
#include "AnmManager.hpp"
#include "AnmVmId.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoGameTask.hpp"
#include "SupervisorViewportSlot.hpp"
namespace th095
{

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

static __forceinline i32 AnmUpdateEitherFlag(i32 first, i32 second)
{
    return first | second;
}

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
                this->RemoveVm(vm);
                goto vmDone;
            }
            if (AnmManager::ExecuteScript(vm))
            {
                this->RemoveVm(vm);
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
AnmVmId AnmManager::AddVm(AnmVm *vm)
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

    AnmVmIdStorage *incrementReceiver = &this->nextVmId;
    (*incrementReceiver)++;
    if (this->nextVmId == AnmVmId())
    {
        this->nextVmId++;
    }
    vm->id = this->nextVmId;
    // Copy the stored four-byte handle directly into the ABI return slot.
    // A value-taking constructor introduces a second copy on VC7.1.
    return *reinterpret_cast<const AnmVmId *>(&this->nextVmId);
}

// FUNCTION: TH095 0x00444E00.
i32 AnmManager::RemoveVm(AnmVm *node)
{
    if (node == this->vmListTail)
    {
        this->vmListTail = node->previous;
    }

    if (node->previous == NULL)
    {
        this->vmListHead = node->next;
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
    return g_AnmManager->AddVm(vm);
}

// FUNCTION: TH095 0x00444FA0.
AnmVmId AnmLoaded::CreateVmAtScreen(
    i32 scriptIndex, Float3 *position)
{
    AnmVm *vm = new AnmVm;
    this->InitializeVm(vm, scriptIndex);
    vm->positionOffset = *position;
    return g_AnmManager->AddVm(vm);
}

// FUNCTION: TH095 0x00445060.
AnmVmId AnmLoaded::CreateVmAtWorld(
    i32 scriptIndex, Float3 *position)
{
    AnmVm *vm = new AnmVm;
    this->InitializeVm(vm, scriptIndex);
    PhotoToScreen(&vm->positionOffset, position);
    return g_AnmManager->AddVm(vm);
}

} // namespace th095
