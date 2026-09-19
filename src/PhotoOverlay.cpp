#ifdef TH095_MATCH_EXACT
#include "PhotoOverlayExact.inl"
#else
#include "Chain.hpp"
#include "GameErrorContext.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoStage.hpp"
#include "ScoreData.hpp"
#include "SupervisorViewportSlot.hpp"
#include "inttypes.hpp"
#include <windows.h>
#include <stddef.h>
#include <string.h>
namespace th095
{

namespace utils
{
void DebugPrint(char *format, ...);
}


struct PhotoStageGlobalStateView
{
    u8 unknown000[0xfc];
#ifdef DIFFBUILD
    u32 flags;
#else
    union
    {
        u32 flags;
        struct
        {
            u32 unknownFlags0_1 : 2;
            u32 gameplayLoadActive : 1;
            u32 unknownFlags3_31 : 29;
        };
    };
#endif
    i32 bestShotIndex;
};
typedef char PhotoOverlayBestShotIndexAt100[
    (offsetof(PhotoStageGlobalStateView, bestShotIndex) == 0x100) ? 1 : -1];
extern PhotoStageGlobalStateView *g_PhotoStageGlobalState;

#ifndef DIFFBUILD
#define g_PhotoStageGlobalState \
    TH095_RUNTIME_GLOBAL_PTR(PhotoStageGlobalStateView, g_RuntimeGlobalStateOwner)
#endif

i32 __fastcall UpdatePhotoStage(PhotoStageStateView *stage);

extern PhotoStageStateView *g_PhotoStageState;
#define g_PhotoStageState \
    TH095_RUNTIME_GLOBAL_PTR(PhotoStageStateView, g_RuntimeStageStateOwner)

// FUNCTION: TH095 0x0042A8A0.
PhotoStageStateView::PhotoStageStateView()
{
    utils::DebugPrint("initialize PhotoInf\n");
    memset(this, 0, sizeof(*this));
    g_PhotoStageState = this;
}

// FUNCTION: TH095 0x0042AAF0.
PhotoStageStateView::~PhotoStageStateView()
{
    utils::DebugPrint("shutdown PhotoInf\n");
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);
    g_AnmManager->MarkVmsForDeletion(this->anm);
    g_PhotoStageState = NULL;
}

// FUNCTION: TH095 0x0042AA30.
i32 PhotoStageStateView::Initialize()
{
    this->anm = g_AnmManager->PreloadAnm(9, "photo.anm");
    if (this->anm == NULL)
    {
        g_GameErrorContext.Log("\x8e\xca\x90\x5e\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81\x42" "\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
        return -1;
    }
    this->scoreMultiplier = 1.0f;
    return 0;
}

// FUNCTION: TH095 0x0042C220.
i32 PhotoStageStateView::Draw()
{
    struct DrawLocals
    {
        i32 vmIndex;
        i32 slotIndex;
        i32 displayVmIndex;
        i32 scoreScanIndex;
        i32 bestSlot;
        i32 bestScore;
    } locals;

    locals.bestSlot = 0;
    locals.bestScore = 0;
    for (locals.scoreScanIndex = 0; locals.scoreScanIndex < 11;
         ++locals.scoreScanIndex)
    {
        if (locals.bestScore <
            this->slots[locals.scoreScanIndex].display.score)
        {
            locals.bestScore =
                this->slots[locals.scoreScanIndex].display.score;
            locals.bestSlot = locals.scoreScanIndex;
        }
    }

    for (locals.displayVmIndex = 0; locals.displayVmIndex < 80;
         ++locals.displayVmIndex)
        this->displayVms[locals.displayVmIndex].Draw();

    for (locals.slotIndex = 0; locals.slotIndex < 11; ++locals.slotIndex)
    {
        for (locals.vmIndex = 0; locals.vmIndex < 6; ++locals.vmIndex)
        {
            if (locals.bestSlot != locals.slotIndex)
                this->slots[locals.slotIndex]
                    .display.overlayVms[locals.vmIndex].color1.color =
                    0xffffffff;
            else if (this->slots[locals.slotIndex].display.score >=
                     g_ResultSaveData->scoreEntries[
                         g_PhotoStageGlobalState->bestShotIndex].detailScore)
                this->slots[locals.slotIndex]
                    .display.overlayVms[locals.vmIndex].color1.color =
                    0xffffff00;
            else
                this->slots[locals.slotIndex]
                    .display.overlayVms[locals.vmIndex].color1.color =
                    0xffffe080;

            // Target source keeps the repeated indexed expression. Hoisting a
            // VM pointer shortens the body and creates a seventh local.
            this->slots[locals.slotIndex]
                .display.overlayVms[locals.vmIndex].Draw();
        }
    }
    return 1;
}

// FUNCTION: TH095 0x0042C410.
i32 __fastcall DrawPhotoStage(PhotoStageStateView *manager)
{
    g_Supervisor.ConfigureGameplayViewport(TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW);
#ifdef DIFFBUILD
    if (((g_PhotoStageGlobalState->flags >> 2) & 1) != 0)
#else
    if (g_PhotoStageGlobalState->gameplayLoadActive != 0)
#endif
    {
        return 1;
    }
    return manager->Draw();
}

// FUNCTION: TH095 0x0042ABC0.
PhotoStageStateView *PhotoStageStateView::Create()
{
    struct CreateLocals
    {
        PhotoStageStateView *manager;
        ChainElem *chain;
    } locals;

    locals.manager = new PhotoStageStateView();
    if (locals.manager->Initialize() != 0)
    {
        goto create_error;
    }

    locals.chain = g_Chain.CreateElem((ChainCallback)UpdatePhotoStage);
    locals.chain->arg = locals.manager;
    g_Chain.AddToCalcChain(locals.chain, 8);
    locals.manager->calcChain = locals.chain;

    locals.chain = g_Chain.CreateElem((ChainCallback)DrawPhotoStage);
    locals.chain->arg = locals.manager;
    g_Chain.AddToDrawChain(locals.chain, 0x1a);
    locals.manager->drawChain = locals.chain;
    return locals.manager;

create_error:
    if (locals.manager != NULL)
    {
        delete locals.manager;
        locals.manager = NULL;
    }
    return NULL;
}

// FUNCTION: TH095 0x0042AD00.
void PhotoStageStateView::Destroy()
{
    PhotoStageStateView *manager = this;
    if (manager != NULL)
    {
        delete manager;
        manager = NULL;
    }
}

// FUNCTION: TH095 0x0042AA90.
i32 LoadPhotoAnm()
{
    if (g_AnmManager->PreloadAnm(9, "photo.anm") == NULL)
    {
        g_GameErrorContext.Log("\x8e\xca\x90\x5e\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81\x42" "\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
        return -1;
    }
    return 0;
}

// FUNCTION: TH095 0x0042AAD0.
i32 ReleasePhotoAnm()
{
    g_AnmManager->ReleaseAnm(9);
    return 0;
}

} // namespace th095

#endif // TH095_MATCH_EXACT
