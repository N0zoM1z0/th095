#include "AnmManager.hpp"

#include <string.h>

namespace th095
{

struct PhotoFrontGameTaskView
{
    u8 unknown000[0xfc];
    u32 flags;                 // +0x0fc
    i32 bestShotIndex;         // +0x100
    i32 completionActive;       // +0x104
    ZunTimer completionTimer;   // +0x108
};

struct PhotoFrontRuntimeView
{
    u8 unknown000[0x1e34];
    f32 hudFade;                // +0x1e34
};

struct PhotoFrontStageStateView
{
    u8 unknown000[0x25720];
    u32 flags;                  // +0x25720
};

struct PhotoFrontSceneDefinitionView
{
    u8 unknown000[0x1c];
    i32 frontScriptIndex;       // +0x01c
};

struct PhotoFrontVmIdView
{
    i32 value;

    void SetInterrupt(i32 interrupt);
};

struct PhotoFrontAnmLoadedView
{
    i32 anmIdx;
    void *rawData;
    i32 totalEntries;
    AnmLoadedSprite *sprites;
    AnmRawInstr **scripts;
    void *textures;
    i32 numberEntriesToBeLoaded;

    ZunResult SetSprite(AnmVm *vm, i32 spriteIndex);
    void InitializeVm(AnmVm *vm, i32 scriptIndex);
    PhotoFrontVmIdView CreateVm(i32 scriptIndex, i32 renderMode);
};

struct PhotoFrontManagerView
{
    AnmVm vms[6];               // +0x0000
    PhotoFrontAnmLoadedView *frontAnm; // +0x10c8
    ChainElem *calcChain;              // +0x10cc
    ChainElem *drawChain;              // +0x10d0

    PhotoFrontManagerView();
    ~PhotoFrontManagerView();

    static PhotoFrontManagerView *Create();
    void Destroy();
    i32 Initialize();
    i32 Update();
    i32 Draw();
    static i32 __fastcall OnUpdate(PhotoFrontManagerView *front);
    static i32 __fastcall OnDraw(PhotoFrontManagerView *front);
};

typedef char PhotoFrontManagerSizeIs10D4[
    (sizeof(PhotoFrontManagerView) == 0x10d4) ? 1 : -1];
typedef char PhotoFrontManagerAnmAt10C8[
    (offsetof(PhotoFrontManagerView, frontAnm) == 0x10c8) ? 1 : -1];

struct PhotoFrontUpdateLocals
{
    i32 nextSecond;
    u32 vmIndex;
    i32 displayedTime;
    i32 alpha;
};

extern PhotoFrontGameTaskView *g_PhotoFrontGameTask;
extern PhotoFrontRuntimeView *g_PhotoFrontRuntime;
extern PhotoFrontStageStateView *g_PhotoFrontStageState;
extern PhotoFrontSceneDefinitionView *g_PhotoFrontSceneDefinition;
extern PhotoFrontManagerView *g_PhotoFrontManager;
extern u32 g_PhotoFrontControllerFlags;
extern i32 g_PhotoFrontReplayUsesArchive;

i32 LoadPhotoFrontAnm()
{
    if (g_AnmManager->LoadAnm(5, "front.anm") == NULL)
    {
        g_GameErrorContext.Log(
            "\x89\xe6\x96\xca\x8d\x5c\x90\xac"
            "\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8c\xa9\x82\xc2\x82\xa9\x82\xe8"
            "\x82\xdc\x82\xb9\x82\xf1\x81\x42"
            "\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x89\xf3\x82\xea\x82\xc4\x82\xa2"
            "\x82\xdc\x82\xb7\r\n");
        return ZUN_ERROR;
    }
    return ZUN_SUCCESS;
}

i32 ReleasePhotoFrontAnm()
{
    g_AnmManager->ReleaseAnm(5);
    return ZUN_SUCCESS;
}

PhotoFrontManagerView::PhotoFrontManagerView()
{
    utils::DebugPrint("initialize FrontInf\n");
    memset(this, 0, sizeof(PhotoFrontManagerView));
    g_PhotoFrontManager = this;
}

PhotoFrontManagerView::~PhotoFrontManagerView()
{
    utils::DebugPrint("shitdown FrontInf\n");
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);
    g_AnmManager->MarkVmsForDeletion(
        reinterpret_cast<AnmLoaded *>(this->frontAnm));
    g_PhotoFrontManager = NULL;
}

static __forceinline void PhotoFrontInitializeFirstVmPhase(PhotoFrontManagerView *front)
{
    u8 compilerStorage[0x108];
    front->frontAnm->InitializeVm(&front->vms[0], 4);
}

i32 PhotoFrontManagerView::Initialize()
{
    PhotoFrontVmIdView vmIds[4];

    this->frontAnm = reinterpret_cast<PhotoFrontAnmLoadedView *>(
        g_AnmManager->LoadAnm(5, "front.anm"));
    if (this->frontAnm == NULL)
    {
        g_GameErrorContext.Log(
            "\x89\xe6\x96\xca\x8d\x5c\x90\xac"
            "\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8c\xa9\x82\xc2\x82\xa9\x82\xe8"
            "\x82\xdc\x82\xb9\x82\xf1\x81\x42"
            "\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x89\xf3\x82\xea\x82Ä\x82\xa2"
            "\x82\xdc\x82\xb7\r\n");
        return ZUN_ERROR;
    }

    vmIds[0] = this->frontAnm->CreateVm(0, 7);
    vmIds[1] = this->frontAnm->CreateVm(1, 7);
    vmIds[2] = this->frontAnm->CreateVm(2, 7);
    vmIds[3] = this->frontAnm->CreateVm(3, 7);

    if (((g_PhotoFrontControllerFlags >> 9) & 1) != 0)
    {
        vmIds[0].SetInterrupt(2);
        vmIds[1].SetInterrupt(2);
        vmIds[2].SetInterrupt(2);
        vmIds[3].SetInterrupt(2);
    }
    else
    {
        this->frontAnm->CreateVm(
            g_PhotoFrontSceneDefinition->frontScriptIndex + 0xd, 7);
    }

    if (g_PhotoFrontReplayUsesArchive != 0)
    {
        this->frontAnm->CreateVm(0x12, 7);
    }

    PhotoFrontInitializeFirstVmPhase(this);
    this->frontAnm->InitializeVm(&this->vms[1], 5);
    this->frontAnm->InitializeVm(&this->vms[2], 6);
    this->frontAnm->InitializeVm(&this->vms[3], 7);
    this->frontAnm->InitializeVm(&this->vms[4], 8);
    this->frontAnm->InitializeVm(&this->vms[5], 9);
    return ZUN_SUCCESS;
}

PhotoFrontManagerView *PhotoFrontManagerView::Create()
{
    struct
    {
        PhotoFrontManagerView *front;
        ChainElem *elem;
    } locals;

#define front locals.front
#define elem locals.elem

    front = new PhotoFrontManagerView();
    if (front->Initialize() != ZUN_SUCCESS)
    {
        goto failure;
    }

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoFrontManagerView::OnUpdate));
    elem->arg = front;
    g_Chain.AddToCalcChain(elem, 0x11);
    front->calcChain = elem;

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoFrontManagerView::OnDraw));
    elem->arg = front;
    g_Chain.AddToDrawChain(elem, 0x14);
    front->drawChain = elem;
    return front;

failure:
    if (front != NULL)
    {
        delete front;
        front = NULL;
    }
#undef elem
#undef front
    return NULL;
}

void PhotoFrontManagerView::Destroy()
{
    PhotoFrontManagerView *front = this;
    if (front != NULL)
    {
        delete front;
        front = NULL;
    }
}

i32 PhotoFrontManagerView::Update()
{
    PhotoFrontUpdateLocals locals;

    if (g_PhotoFrontGameTask->completionActive != 0)
    {
        locals.displayedTime =
            static_cast<i32>(g_PhotoFrontGameTask->completionTimer) / 60;
        if (locals.displayedTime >= 1000)
        {
            locals.displayedTime = 999;
        }

        this->frontAnm->SetSprite(
            &this->vms[0], locals.displayedTime / 100 % 10 + 4);
        this->frontAnm->SetSprite(
            &this->vms[1], locals.displayedTime / 10 % 10 + 4);
        this->frontAnm->SetSprite(
            &this->vms[2], locals.displayedTime % 10 + 4);

        if (locals.displayedTime >= 1000)
        {
            locals.displayedTime = 99;
        }
        else
        {
            locals.displayedTime = static_cast<i32>(
                (static_cast<f32>(g_PhotoFrontGameTask->completionTimer) -
                 locals.displayedTime * 60.0f) *
                100.0f / 60.0f);
        }

        this->frontAnm->SetSprite(
            &this->vms[4], locals.displayedTime / 10 % 10 + 4);
        this->frontAnm->SetSprite(
            &this->vms[5], locals.displayedTime % 10 + 4);

        for (locals.vmIndex = 0; locals.vmIndex < 6; locals.vmIndex++)
        {
            AnmManager::ExecuteScript(&this->vms[locals.vmIndex]);
        }

        locals.alpha = 0xff;
        if (g_PhotoFrontRuntime->hudFade < 64.0f)
        {
            locals.alpha = 0x40;
        }
        else if (g_PhotoFrontRuntime->hudFade < 128.0f)
        {
            locals.alpha =
                (static_cast<u32>(g_PhotoFrontRuntime->hudFade - 64.0f) *
                 0xbf >> 6) +
                0x40;
        }

        if (this->vms[0].interpEndTimers[ANM_INTERP_ALPHA1] == 0)
        {
            this->vms[0].color1.a = (u8)locals.alpha;
            this->vms[1].color1.a = (u8)locals.alpha;
            this->vms[2].color1.a = (u8)locals.alpha;
            this->vms[3].color1.a = (u8)locals.alpha;
            this->vms[4].color1.a = (u8)locals.alpha;
            this->vms[5].color1.a = (u8)locals.alpha;
        }

        if (g_PhotoFrontGameTask->completionTimer > 0)
        {
            locals.nextSecond =
                (static_cast<i32>(g_PhotoFrontGameTask->completionTimer) + 1) /
                60;
            if (static_cast<i32>(g_PhotoFrontGameTask->completionTimer) / 60 <
                    10 &&
                locals.nextSecond >= 10)
            {
                this->vms[0].pendingInterrupt = 7;
                this->vms[1].pendingInterrupt = 7;
                this->vms[2].pendingInterrupt = 7;
                this->vms[3].pendingInterrupt = 7;
                this->vms[4].pendingInterrupt = 7;
                this->vms[5].pendingInterrupt = 7;
            }
            else if (
                static_cast<i32>(g_PhotoFrontGameTask->completionTimer) / 60 <
                    5 &&
                locals.nextSecond >= 5)
            {
                this->vms[0].pendingInterrupt = 8;
                this->vms[1].pendingInterrupt = 8;
                this->vms[2].pendingInterrupt = 8;
                this->vms[3].pendingInterrupt = 8;
                this->vms[4].pendingInterrupt = 8;
                this->vms[5].pendingInterrupt = 8;
            }
        }
    }

    return 1;
}

i32 PhotoFrontManagerView::Draw()
{
    u32 vmIndex;

    if (((g_PhotoFrontStageState->flags >> 2) & 1) == 0)
    {
        for (vmIndex = 0; vmIndex < 6; vmIndex++)
        {
            this->vms[vmIndex].Draw();
        }
    }
    return 1;
}

i32 __fastcall PhotoFrontManagerView::OnUpdate(PhotoFrontManagerView *front)
{
    if (((g_PhotoFrontGameTask->flags >> 2) & 1) != 0)
    {
        return 1;
    }
    return front->Update();
}

i32 __fastcall PhotoFrontManagerView::OnDraw(PhotoFrontManagerView *front)
{
    if (((g_PhotoFrontGameTask->flags >> 2) & 1) != 0)
    {
        return 1;
    }
    return front->Draw();
}

} // namespace th095
