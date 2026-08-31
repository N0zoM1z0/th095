#define TH095_DECLARE_ANM_LOADED_INITIALIZE_VM
#include "AnmManager.hpp"
#include "AnmVmId.hpp"

namespace th095
{

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

struct AnmVmLifecycleView
{
    AnmVmLifecycleView *next;
    u8 unknown004[4];
    AnmVmLifecycleView *previous;
    u32 renderMode;
    struct Id
    {
        i32 value;

        Id()
        {
            this->value = 0;
        }

        i32 operator==(Id other)
        {
            return this->value == other.value;
        }

        i32 operator++(int)
        {
            i32 previous = this->value;
            this->value++;
            return previous;
        }

    } id;
};

struct AnmManagerVmLifecycleView
{
    u8 unknown000[0x381814];
    AnmVmLifecycleView *vmListHead;
    AnmVmLifecycleView *vmListTail;
    u8 unknown38181c[0x192c];
    AnmVmLifecycleView::Id nextVmId;

    AnmVmId AddVm(AnmVmLifecycleView *vm);
    i32 RemoveVm(struct AnmVmDeleteView *vm);
};

struct AnmVmDeleteView
{
    AnmVmDeleteView *next;
    u8 unknown004[4];
    AnmVmDeleteView *previous;
    u8 unknown00c[8];
    void *generatedVertices;

    ~AnmVmDeleteView()
    {
        if (this->generatedVertices != NULL)
        {
            void *generatedVertices = this->generatedVertices;
            free(generatedVertices);
        }
    }
};

struct AnmLoadedPositionView
{
    i32 anmIdx;
    void *rawData;
    i32 totalEntries;
    AnmLoadedSprite *sprites;
    AnmRawInstr **scripts;
    void *textures;
    i32 numberEntriesToBeLoaded;

    AnmVmId CreateVmAtScreen(i32 scriptIndex, Float3 *position);
    AnmVmId CreateVmAtWorld(i32 scriptIndex, Float3 *position);
};

struct AnmVmDrawNodeView
{
    u8 unknown000[4];
    AnmVmDrawNodeView *nextInDrawLayer;
    u8 unknown008[0x220];
    union
    {
        u32 flagsWord;
        struct
        {
            u32 unknownFlags00 : 26;
            u32 flag26 : 1;
            u32 flag27 : 1;
            u32 flag28 : 1;
            u32 unknownFlags29 : 3;
        };
    };
    u8 unknown22c[0xa0];
};

struct AnmManagerDrawLayerView
{
    u8 unknown000[0x38181c];
    AnmVmDrawNodeView drawLayerHeads[9];

    i32 DrawLayer(i32 layer);
};

struct PhotoGameTaskDrawGateView
{
    u8 unknown000[0xfc];
    union
    {
        u32 flags;
        struct
        {
            u32 flag0 : 1;
            u32 flag1 : 1;
            u32 drawVms : 1;
            u32 unknownFlags3 : 7;
            u32 flag10 : 1;
            u32 unknownFlags11 : 21;
        };
    };

};

extern PhotoGameTaskDrawGateView *g_PhotoGameTask;
extern f32 g_ScreenEffectShakeX;
extern f32 g_ScreenEffectShakeY;

struct AnmVmUpdateView
{
    AnmVmUpdateView *next;
    AnmVmUpdateView *nextInDrawLayer;
    AnmVmUpdateView *previous;
    i32 renderMode;
    u8 unknown010[0x218];
    union
    {
        u32 flagsWord;
        struct
        {
            u32 unknownFlags00 : 26;
            u32 flag26 : 1;
            u32 flag27 : 1;
            u32 flag28 : 1;
            u32 unknownFlags29 : 3;
        };
    };
    u8 unknown22c[0xa0];
};

struct AnmManagerUpdateView
{
    u8 unknown000[0x28];
    i32 vmsProcessedThisFrame;
    u8 unknown02c[0x381814 - 0x2c];
    AnmVmUpdateView *vmListHead;
    AnmVmUpdateView *vmListTail;
    AnmVmUpdateView drawLayerHeads[9];

    i32 UpdateVms();
};

typedef char AnmVmLifecycleIdAt10[
    (offsetof(AnmVmLifecycleView, id) == 0x10) ? 1 : -1];
typedef char AnmManagerVmLifecycleIdAt383148[
    (offsetof(AnmManagerVmLifecycleView, nextVmId) == 0x383148) ? 1 : -1];
typedef char AnmVmDrawNodeViewSizeIs2CC[
    (sizeof(AnmVmDrawNodeView) == 0x2cc) ? 1 : -1];
typedef char AnmVmDrawNodeFlagsAt228[
    (offsetof(AnmVmDrawNodeView, flagsWord) == 0x228) ? 1 : -1];
typedef char AnmManagerDrawLayerHeadsAt38181C[
    (offsetof(AnmManagerDrawLayerView, drawLayerHeads) == 0x38181c) ? 1 : -1];
typedef char AnmVmUpdateViewSizeIs2CC[
    (sizeof(AnmVmUpdateView) == 0x2cc) ? 1 : -1];
typedef char AnmManagerUpdateVmListAt381814[
    (offsetof(AnmManagerUpdateView, vmListHead) == 0x381814) ? 1 : -1];
typedef char AnmManagerUpdateDrawLayersAt38181C[
    (offsetof(AnmManagerUpdateView, drawLayerHeads) == 0x38181c) ? 1 : -1];

// FUNCTION: TH095 0x00444980.
void __fastcall AnmManager::OnUpdate(void *arg)
{
    reinterpret_cast<AnmManagerUpdateView *>(arg)->UpdateVms();
}

// FUNCTION: TH095 0x004449A0.
void __fastcall AnmManager::DrawLayer0(void *arg)
{
    reinterpret_cast<AnmManagerDrawLayerView *>(arg)->DrawLayer(0);
}

// FUNCTION: TH095 0x004449C0.
void __fastcall AnmManager::DrawLayer1(void *arg)
{
    reinterpret_cast<AnmManagerDrawLayerView *>(arg)->DrawLayer(1);
}

// FUNCTION: TH095 0x004449E0.
void __fastcall AnmManager::DrawLayer2(void *arg)
{
    reinterpret_cast<AnmManagerDrawLayerView *>(arg)->DrawLayer(2);
}

// FUNCTION: TH095 0x00444A00.
void __fastcall AnmManager::DrawLayer3(void *arg)
{
    reinterpret_cast<AnmManagerDrawLayerView *>(arg)->DrawLayer(3);
}

// FUNCTION: TH095 0x00444A20.
void __fastcall AnmManager::DrawLayer4(void *arg)
{
    reinterpret_cast<AnmManagerDrawLayerView *>(arg)->DrawLayer(4);
}

// FUNCTION: TH095 0x00444A40.
void __fastcall AnmManager::DrawLayer5(void *arg)
{
    reinterpret_cast<AnmManagerDrawLayerView *>(arg)->DrawLayer(5);
}

// FUNCTION: TH095 0x00444A60.
void __fastcall AnmManager::DrawLayer6(void *arg)
{
    g_ScreenEffectShakeX = 0.0f;
    g_ScreenEffectShakeY = 0.0f;
    g_AnmManager->unknown020 = 0;
    g_AnmManager->unknown024 = 0;
    reinterpret_cast<AnmManagerDrawLayerView *>(arg)->DrawLayer(6);
}

// FUNCTION: TH095 0x00444AB0.
void __fastcall AnmManager::DrawLayer7(void *arg)
{
    g_Supervisor.ConfigureGameplayViewport(1);
    reinterpret_cast<AnmManagerDrawLayerView *>(arg)->DrawLayer(7);
}

// FUNCTION: TH095 0x00444AE0.
void __fastcall AnmManager::DrawLayer8(void *arg)
{
    g_Supervisor.ConfigureGameplayViewport(1);
    reinterpret_cast<AnmManagerDrawLayerView *>(arg)->DrawLayer(8);
}

// FUNCTION: TH095 0x00444B10.
i32 AnmManagerUpdateView::UpdateVms()
{
    i32 layer;
    AnmVmUpdateView *layerTails[9];
    AnmVmUpdateView *vm;
    AnmVmUpdateView *next;

    for (layer = 0; layer < 9; layer++)
    {
        this->drawLayerHeads[layer].nextInDrawLayer = NULL;
        layerTails[layer] = &this->drawLayerHeads[layer];
    }

    this->vmsProcessedThisFrame = 0;
    vm = this->vmListHead;
    while (vm != NULL)
    {
        next = vm->next;
        if (g_PhotoGameTask == NULL || vm->flag28 ||
            ((g_PhotoGameTask->drawVms | g_PhotoGameTask->flag0) == 0 &&
             g_PhotoGameTask->flag1 == 0 &&
             g_PhotoGameTask->flag10 == 0))
        {
            if (vm->flag26)
            {
                reinterpret_cast<AnmManagerVmLifecycleView *>(this)->RemoveVm(
                    reinterpret_cast<AnmVmDeleteView *>(vm));
                goto vmDone;
            }
            if (AnmManager::ExecuteScript(reinterpret_cast<AnmVm *>(vm)))
            {
                reinterpret_cast<AnmManagerVmLifecycleView *>(this)->RemoveVm(
                    reinterpret_cast<AnmVmDeleteView *>(vm));
                goto vmDone;
            }
            goto addToDrawLayer;
        }
addToDrawLayer:
        layerTails[vm->renderMode]->nextInDrawLayer = vm;
        layerTails[vm->renderMode] = vm;
        vm->nextInDrawLayer = NULL;

vmDone:
        this->vmsProcessedThisFrame++;
        vm = next;
    }
    return 1;
}

// FUNCTION: TH095 0x00444C80.
i32 AnmManagerDrawLayerView::DrawLayer(i32 layer)
{
    AnmVmDrawNodeView *vm;
    vm = this->drawLayerHeads[layer].nextInDrawLayer;
    while (vm != NULL)
    {
        if (g_PhotoGameTask != NULL && vm->flag28 == 0 &&
            g_PhotoGameTask->drawVms != 0)
        {
        }
        else if (!vm->flag26)
        {
            reinterpret_cast<AnmManager *>(this)->Draw(
                reinterpret_cast<AnmVm *>(vm));
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

    this->nextVmId++;
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
AnmVmId AnmLoaded::CreateVm(i32 scriptIndex, i32 renderMode)
{
    AnmVm *vm = new AnmVm;
    vm->renderMode = renderMode;
    this->InitializeVm(vm, scriptIndex);
    return reinterpret_cast<AnmManagerVmLifecycleView *>(g_AnmManager)
        ->AddVm(reinterpret_cast<AnmVmLifecycleView *>(vm));
}

// FUNCTION: TH095 0x00444FA0.
AnmVmId AnmLoadedPositionView::CreateVmAtScreen(
    i32 scriptIndex, Float3 *position)
{
    AnmVm *vm = new AnmVm;
    reinterpret_cast<AnmLoaded *>(this)->InitializeVm(vm, scriptIndex);
    vm->positionOffset = *position;
    return reinterpret_cast<AnmManagerVmLifecycleView *>(g_AnmManager)
        ->AddVm(reinterpret_cast<AnmVmLifecycleView *>(vm));
}

// FUNCTION: TH095 0x00445060.
AnmVmId AnmLoadedPositionView::CreateVmAtWorld(
    i32 scriptIndex, Float3 *position)
{
    AnmVm *vm = new AnmVm;
    reinterpret_cast<AnmLoaded *>(this)->InitializeVm(vm, scriptIndex);
    PhotoToScreen(&vm->positionOffset, position);
    return reinterpret_cast<AnmManagerVmLifecycleView *>(g_AnmManager)
        ->AddVm(reinterpret_cast<AnmVmLifecycleView *>(vm));
}

} // namespace th095
