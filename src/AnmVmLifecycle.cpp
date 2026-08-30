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
            u32 unknownFlags0 : 2;
            u32 drawVms : 1;
            u32 unknownFlags3 : 29;
        };
    };
};

extern PhotoGameTaskDrawGateView *g_PhotoGameTask;

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

// FUNCTION: TH095 0x00444C80.
i32 AnmManagerDrawLayerView::DrawLayer(i32 layer)
{
    AnmVmDrawNodeView *vm;
    vm = this->drawLayerHeads[layer].nextInDrawLayer;
    while (vm != NULL)
    {
        if (!(g_PhotoGameTask != NULL && vm->flag28 == 0 &&
              g_PhotoGameTask->drawVms != 0))
        {
            if (!vm->flag26)
            {
                reinterpret_cast<AnmManager *>(this)->Draw(
                    reinterpret_cast<AnmVm *>(vm));
            }
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
