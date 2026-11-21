#include "AnmManager.hpp"
#include "SceneData.hpp"
#include "PhotoFront.hpp"
#include "PhotoStage.hpp"
#include "PhotoGamePlayer.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoGameTask.hpp"

#include <string.h>

namespace th095
{

// VC7.1 adds default-construction code for a canonical handle array.
// This method declaration preserves the target's four returned handle stores.
struct PhotoFrontVmIdView
{
    i32 value;

    void SetInterrupt(i32 interrupt);
};

struct PhotoFrontAnmLoadedView
{
    PhotoFrontVmIdView CreateVm(i32 scriptIndex, i32 renderMode);
};

struct PhotoFrontUpdateLocals
{
    i32 nextSecond;
    u32 vmIndex;
    i32 displayedTime;
    i32 alpha;
};

extern u32 g_PhotoFrontControllerFlags;
extern i32 g_PhotoFrontReplayUsesArchive;

i32 LoadPhotoFrontAnm()
{
    if (g_AnmManager->PreloadAnm(5, "front.anm") == NULL)
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
    g_AnmManager->MarkVmsForDeletion(this->frontAnm);
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

    this->frontAnm = g_AnmManager->PreloadAnm(5, "front.anm");
    if (this->frontAnm == NULL)
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

    vmIds[0] = reinterpret_cast<PhotoFrontAnmLoadedView *>(this->frontAnm)
                       ->CreateVm(0, 7);
    vmIds[1] = reinterpret_cast<PhotoFrontAnmLoadedView *>(this->frontAnm)
                       ->CreateVm(1, 7);
    vmIds[2] = reinterpret_cast<PhotoFrontAnmLoadedView *>(this->frontAnm)
                       ->CreateVm(2, 7);
    vmIds[3] = reinterpret_cast<PhotoFrontAnmLoadedView *>(this->frontAnm)
                       ->CreateVm(3, 7);

    if (((g_PhotoFrontControllerFlags >> 9) & 1) != 0)
    {
        vmIds[0].SetInterrupt(2);
        vmIds[1].SetInterrupt(2);
        vmIds[2].SetInterrupt(2);
        vmIds[3].SetInterrupt(2);
    }
    else
    {
        reinterpret_cast<PhotoFrontAnmLoadedView *>(this->frontAnm)->CreateVm(
            g_SelectedScene->frontScriptIndex + 0xd, 7);
    }

    if (g_PhotoFrontReplayUsesArchive != 0)
    {
        reinterpret_cast<PhotoFrontAnmLoadedView *>(this->frontAnm)
            ->CreateVm(0x12, 7);
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

    if (g_RuntimeGlobalStateOwner->completion.completionActive != 0)
    {
        locals.displayedTime =
            static_cast<i32>(g_RuntimeGlobalStateOwner->completion.timer) / 60;
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
                (static_cast<f32>(g_RuntimeGlobalStateOwner->completion.timer) -
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
        if (g_RuntimePlayerOwner->playerPosition.y < 64.0f)
        {
            locals.alpha = 0x40;
        }
        else if (g_RuntimePlayerOwner->playerPosition.y < 128.0f)
        {
            locals.alpha =
                (static_cast<u32>(g_RuntimePlayerOwner->playerPosition.y - 64.0f) *
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

        if (g_RuntimeGlobalStateOwner->completion.timer > 0)
        {
            locals.nextSecond =
                (static_cast<i32>(g_RuntimeGlobalStateOwner->completion.timer) + 1) /
                60;
            if (static_cast<i32>(g_RuntimeGlobalStateOwner->completion.timer) / 60 <
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
                static_cast<i32>(g_RuntimeGlobalStateOwner->completion.timer) / 60 <
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

    if (g_PhotoStageState->firstCaptureFrame == 0)
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
    if (g_RuntimeGlobalStateOwner->gameplayLoadActive != 0)
    {
        return 1;
    }
    return front->Update();
}

i32 __fastcall PhotoFrontManagerView::OnDraw(PhotoFrontManagerView *front)
{
    if (g_RuntimeGlobalStateOwner->gameplayLoadActive != 0)
    {
        return 1;
    }
    return front->Draw();
}

} // namespace th095
