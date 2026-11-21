#include "AnmManager.hpp"
#include "FrontEndController.hpp"
#include "Global.hpp"
#include "Main.hpp"
#include "ResultScreen.hpp"
#include "ScoreData.hpp"
#include "SoundPlayer.hpp"
#include "SceneSelect.hpp"
#include "utils.hpp"

namespace th095
{

enum TitleAnmSlot
{
    TITLE_ANM_SLOT = 11,
    TITLE_TRANSITION_ANM_SLOT = 12,
};

DIFFABLE_STATIC(void *, g_ActiveMenuController);

i32 LoadPhotoBulletAnm();
i32 LoadPhotoAnm();
i32 LoadPhotoFrontAnm();
i32 LoadPhotoPlayerAnm();

// Target 0x004CA2F8 is the zero-initialized completion flag written by this
// resource-loader lifecycle.
DIFFABLE_STATIC(i32, g_FrontEndLoadActive);
struct FrontEndMissionEntryView
{
    u16 group;
    u16 scene;
    u8 displayState;
    u8 unknown005[3];
    i32 scoreRequirement;
    u8 encodedText[1];
};

typedef char FrontEndMissionEntryFields[
    (offsetof(FrontEndMissionEntryView, scoreRequirement) == 0x08 &&
     offsetof(FrontEndMissionEntryView, encodedText) == 0x0c) ? 1 : -1];

struct FrontEndInitializeLocals
{
    i32 i;
    FrontEndMissionEntryView *entry;
    i32 *offset;
    i32 count;
    i32 missionSize;
};


// FUNCTION: TH095 0x00445440.
FrontEndControllerView::FrontEndControllerView()
{
    utils::DebugPrint("initialize TitleTaskInf\n");
    memset(this, 0, sizeof(*this));
    g_ActiveMenuController = this;
}

// FUNCTION: TH095 0x004456F0.
i32 FrontEndControllerView::Initialize()
{
    FrontEndInitializeLocals locals;

    this->sceneAnm =
        g_AnmManager->PreloadAnm(TITLE_ANM_SLOT, "title.anm");
    if (this->sceneAnm == NULL)
    {
        g_GameErrorContext.Log("title data is corrupt\r\n");
        return -1;
    }

    this->transitionAnm =
        g_AnmManager->PreloadAnm(TITLE_TRANSITION_ANM_SLOT, "title_v.anm");
    if (this->transitionAnm == NULL)
    {
        g_GameErrorContext.Log("title data is corrupt\r\n");
        return -1;
    }

    this->ownedMissionMessageData =
        FileSystem::OpenFile("sprt/mission.msg", &locals.missionSize, FALSE);
    if (this->ownedMissionMessageData == NULL)
    {
        g_GameErrorContext.Log("mission.msg data is corrupt\r\n");
        return -1;
    }

    locals.count = *reinterpret_cast<i32 *>(this->ownedMissionMessageData);
    locals.offset = reinterpret_cast<i32 *>(this->ownedMissionMessageData) + 1;
    for (locals.i = 0; locals.i < locals.count; locals.i++)
    {
        // Keep offset-first addition for VC7.1's register order. The file
        // stores a relative offset and the base retains the full pointer width.
        locals.entry = reinterpret_cast<FrontEndMissionEntryView *>(
            *locals.offset +
            reinterpret_cast<uintptr_t>(this->ownedMissionMessageData));
        if (locals.entry->group < 12)
        {
            g_SceneGroups[locals.entry->group][locals.entry->scene]
                .encodedTitleText = locals.entry->encodedText;
            g_SceneGroups[locals.entry->group][locals.entry->scene]
                .displayState = locals.entry->displayState;
            g_SceneGroups[locals.entry->group][locals.entry->scene]
                .scoreRequirement = locals.entry->scoreRequirement;
        }
        else
        {
            this->specialEncodedText[locals.entry->scene] =
                locals.entry->encodedText;
            this->specialDisplayStates[locals.entry->scene] =
                locals.entry->displayState;
        }
        locals.offset++;
    }

    this->currentDisplayState = -1;
    this->flagBits.unknownFlagBit4 = 1;
    if (g_SoundPlayer.initializationComplete == 0)
    {
        g_SoundPlayer.InitSoundBuffers();
        if (g_SoundPlayer.bgmFileBaseOffset == 0)
        {
            if (g_Supervisor.config.options.preloadMusic == 0)
                g_SoundPlayer.StartBGM("thbgm.dat");
            else
                strcpy(g_SoundPlayer.currentBgmFileName, "thbgm.dat");
        }
        else
        {
            if (g_Supervisor.config.options.preloadMusic == 0)
                g_SoundPlayer.StartBGM("th095.dat");
            else
                strcpy(g_SoundPlayer.currentBgmFileName, "th095.dat");
        }
    }

    FrontEndControllerView::LoadResources(NULL);
    return 0;
}

// FUNCTION: TH095 0x00445A50.
void __fastcall FrontEndControllerView::LoadResources(void *)
{
    if (LoadPhotoBulletAnm() < 0)
        goto loadDone;
    if (LoadPhotoAnm() < 0)
        goto loadDone;
    if (ResultScreen::LoadAnm() < 0)
        goto loadDone;
    if (LoadPhotoFrontAnm() < 0)
        goto loadDone;
    LoadPhotoPlayerAnm();

loadDone:
    g_FrontEndLoadActive = 0;
}

// FUNCTION: TH095 0x00445CA0.
void __fastcall ReleaseFrontEndAnms()
{
    g_AnmManager->ReleaseAnm(TITLE_ANM_SLOT);
    g_AnmManager->ReleaseAnm(TITLE_TRANSITION_ANM_SLOT);
}

// FUNCTION: TH095 0x00445980.
void __fastcall FrontEndControllerView::LoadThread(void *)
{
    FrontEndControllerView *controller =
        static_cast<FrontEndControllerView *>(g_ActiveMenuController);

    while (g_AnmManager->captureSurfaceIdx >= 0 ||
           g_AnmManager->captureAnmIdx >= 0)
    {
        if (g_Supervisor.flags.receivedCloseMsg != 0)
            goto loadFailed;
        Sleep(1);
    }

    if (controller->Initialize() != 0)
        goto loadFailed;

    g_Supervisor.HideLoadingVms();
    controller->flagBits.titleLoadIncomplete = 0;
    utils::DebugPrint("Title Load Thread Finish\n");
    WriteWorkerSignal(g_Supervisor.replayScanWorker.active, 0);
    WriteWorkerSignal(g_Supervisor.replayScanWorker.exitSignal, 1);
    goto loadDone;

loadFailed:
    controller->flagBits.titleLoadFailed = 1;
    g_Supervisor.BeginLoadingCompletion();
    WriteWorkerSignal(g_Supervisor.replayScanWorker.active, 0);
    WriteWorkerSignal(g_Supervisor.replayScanWorker.exitSignal, 1);

loadDone:
    return;
}

// FUNCTION: TH095 0x00445CC0.
FrontEndControllerView *__fastcall FrontEndControllerView::Create(i32 mode)
{
    FrontEndControllerView *controller = new FrontEndControllerView();
    ChainElem *elem;

    controller->flagBits.titleLoadIncomplete = 1;
    controller->entryMode = mode;

    elem = g_Chain.CreateElem(SceneSelectControllerView::OnUpdate);
    elem->arg = controller;
    g_Chain.AddToCalcChain(elem, 4);
    controller->calcChain = elem;

    elem = g_Chain.CreateElem(FrontEndControllerView::OnDraw);
    elem->arg = controller;
    g_Chain.AddToDrawChain(elem, 1);
    controller->drawChain = elem;

    g_Supervisor.StartReplayScan(
        FrontEndControllerView::LoadThread, controller);
    return controller;
}

static __forceinline void FrontEndFreeReplayListData(FrontEndControllerView *view)
{
    if (view->replayListData != NULL)
    {
        void *data = view->replayListData;
        free(data);
    }
}
static __forceinline void FrontEndFreeMissionMessageData(FrontEndControllerView *view)
{
    if (view->ownedMissionMessageData != NULL)
    {
        void *data = view->ownedMissionMessageData;
        free(data);
    }
}
static __forceinline void FrontEndFreePoppedValue(void *data)
{
    free(data);
}
static __forceinline void FrontEndFreeGroupQueuePop(FrontEndControllerView *view)
{
    u32 compilerStorage;
    FrontEndFreePoppedValue(
        reinterpret_cast<void *>(view->groupPreviewDataQueue.Pop()));
}
static __forceinline void FrontEndFreeSceneQueuePop(FrontEndControllerView *view)
{
    u32 compilerStorage;
    FrontEndFreePoppedValue(
        reinterpret_cast<void *>(view->scenePreviewDataQueue.Pop()));
}
static __forceinline void FrontEndFreePendingPrimary(FrontEndControllerView *view, i32 index)
{
    if (view->pendingTextures.primaryData[index] != NULL)
    {
        void *data = view->pendingTextures.primaryData[index];
        free(data);
    }
}
static __forceinline void FrontEndFreePendingSecondary(FrontEndControllerView *view, i32 index)
{
    if (view->pendingTextures.secondaryData[index] != NULL)
    {
        void *data = view->pendingTextures.secondaryData[index];
        free(data);
    }
}

// FUNCTION: TH095 0x00445AA0.
FrontEndControllerView::~FrontEndControllerView()
{
    i32 replayIndex;
    i32 pendingIndex;

    g_ResultSaveData->WriteBestShotData();
    for (replayIndex = 0; replayIndex < 80; replayIndex++)
    {
        if (this->replays[replayIndex] != NULL)
        {
            delete this->replays[replayIndex];
            this->replays[replayIndex] = NULL;
        }
    }

    utils::DebugPrint("shutdown TitleTaskInf\n");
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);
    g_ActiveMenuController = NULL;

    FrontEndFreeReplayListData(this);
    FrontEndFreeMissionMessageData(this);

    while (this->groupPreviewDataQueue.Size() != 0)
    {
        FrontEndFreeGroupQueuePop(this);
    }
    while (this->scenePreviewDataQueue.Size() != 0)
    {
        FrontEndFreeSceneQueuePop(this);
    }

    for (pendingIndex = 0; pendingIndex < 3; pendingIndex++)
    {
        FrontEndFreePendingPrimary(this, pendingIndex);
        FrontEndFreePendingSecondary(this, pendingIndex);
    }
}

// FUNCTION: TH095 0x00445DE0.
void FrontEndControllerView::Destroy()
{
    FrontEndControllerView *controller = this;
    if (controller != NULL)
    {
        delete controller;
        controller = NULL;
    }
}

}
