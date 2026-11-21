#include "AnmManager.hpp"
#include "Global.hpp"
#include "FrontEndGlobals.hpp"
#include "FrontEndPendingTextureData.hpp"
#include "SceneValueQueue.hpp"
#include "ScenePreviewDataQueue.hpp"
#include "Main.hpp"
#include "ResultScreen.hpp"
#include "ScoreData.hpp"
#include "SceneData.hpp"
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


struct FrontEndTimerView
{
    i32 previous;
    f32 subFrame;
    i32 current;

    FrontEndTimerView()
    {
        this->current = 0;
        this->previous = -999999;
        this->subFrame = 0.0f;
    }
};

struct FrontEndLifecycleView
{
    AnmLoaded *sceneAnm;
    AnmLoaded *transitionAnm;
    FrontEndTimerView stateTimer;
    FrontEndTimerView transitionTimer;
    ResultScreenReplayCursor groupCursor;
    ResultScreenReplayCursor secondaryCursor;
    ResultScreenReplayCursor sceneCursors[12];
    u8 unknown0bf0[0x26c];
    u8 *ownedMissionMessageData;
    SceneEncodedText specialEncodedText[10];
    i8 specialDisplayStates[10];
    i8 currentDisplayState;
    u8 unknown0e93[0x15];
    ReplayManager *replays[80];
    u8 unknown0fe8[0x14];
    void *replayListData;
    u8 unknown1000[0x5120];
    FrontEndControllerFlagBits flagBits;
    i32 entryMode;
    SceneValueQueue selectionQueue;
    SceneValueQueue loadedSceneQueue;
    ScenePreviewDataQueue groupPreviewDataQueue;
    SceneValueQueue groupPreviewSizeQueue;
    ScenePreviewDataQueue scenePreviewDataQueue;
    SceneValueQueue scenePreviewSizeQueue;
    SceneValueQueue groupPreviewQueue;
    SceneValueQueue scenePreviewQueue;
    SceneValueQueue loadedGroupQueue;
    u8 unknown63b0[0x1c];
    FrontEndPendingTextureData pendingTextures;
    ChainElem *calcChain;
    ChainElem *drawChain;
    u8 unknown6408[0x10c];

    FrontEndLifecycleView();
    ~FrontEndLifecycleView();
    i32 Initialize();
    static void __fastcall LoadResources(void *unused);
    static void __fastcall LoadThread(void *unused);
    static FrontEndLifecycleView *__fastcall Create(i32 entryMode);
    void Destroy();
};


typedef char FrontEndLifecyclePendingTextureDataLayout[
    (sizeof(FrontEndPendingTextureData) == 0x34 &&
     offsetof(FrontEndLifecycleView, pendingTextures) == 0x63cc &&
     offsetof(FrontEndPendingTextureData, primaryData) == 0x04 &&
     offsetof(FrontEndPendingTextureData, primarySize) == 0x10 &&
     offsetof(FrontEndPendingTextureData, secondaryData) == 0x1c &&
     offsetof(FrontEndPendingTextureData, secondarySize) == 0x28) ? 1 : -1];
typedef char FrontEndLifecycleOwnedMissionDataAtE5C[
    (offsetof(FrontEndLifecycleView, ownedMissionMessageData) == 0xe5c)
        ? 1
        : -1];
typedef char FrontEndLifecycleSpecialEncodedTextAtE60[
    (offsetof(FrontEndLifecycleView, specialEncodedText) == 0xe60) ? 1 : -1];
typedef char FrontEndLifecycleDisplayStateAtE92[
    (offsetof(FrontEndLifecycleView, currentDisplayState) == 0xe92) ? 1
                                                                   : -1];
typedef char FrontEndLifecycleReplaysAtEA8[
    (offsetof(FrontEndLifecycleView, replays) == 0xea8) ? 1 : -1];
typedef char FrontEndLifecycleReplayListDataAtFFC[
    (offsetof(FrontEndLifecycleView, replayListData) == 0xffc) ? 1 : -1];
typedef char FrontEndLifecycleGroupDataQueueAt61B8[
    (offsetof(FrontEndLifecycleView, groupPreviewDataQueue) == 0x61b8)
        ? 1
        : -1];
typedef char FrontEndLifecycleFlagsAt6120[
    (offsetof(FrontEndLifecycleView, flagBits) == 0x6120) ? 1 : -1];
typedef char FrontEndLifecycleEntryModeAt6124[
    (offsetof(FrontEndLifecycleView, entryMode) == 0x6124) ? 1 : -1];
typedef char FrontEndLifecycleSceneDataQueueAt6248[
    (offsetof(FrontEndLifecycleView, scenePreviewDataQueue) == 0x6248)
        ? 1
        : -1];
typedef char FrontEndLifecyclePendingPrimaryAt63D0[
    (offsetof(FrontEndLifecycleView, pendingTextures) +
         offsetof(FrontEndPendingTextureData, primaryData) == 0x63d0)
        ? 1
        : -1];
typedef char FrontEndLifecyclePendingSecondaryAt63E8[
    (offsetof(FrontEndLifecycleView, pendingTextures) +
         offsetof(FrontEndPendingTextureData, secondaryData) == 0x63e8)
        ? 1
        : -1];
typedef char FrontEndLifecycleChainsAt6400[
    (offsetof(FrontEndLifecycleView, calcChain) == 0x6400 &&
     offsetof(FrontEndLifecycleView, drawChain) == 0x6404)
        ? 1
        : -1];
typedef char FrontEndLifecycleSizeIs6514[
    (sizeof(FrontEndLifecycleView) == 0x6514) ? 1 : -1];

// FUNCTION: TH095 0x00445440.
FrontEndLifecycleView::FrontEndLifecycleView()
{
    utils::DebugPrint("initialize TitleTaskInf\n");
    memset(this, 0, sizeof(*this));
    g_ActiveMenuController = this;
}

// FUNCTION: TH095 0x004456F0.
i32 FrontEndLifecycleView::Initialize()
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

    FrontEndLifecycleView::LoadResources(NULL);
    return 0;
}

// FUNCTION: TH095 0x00445A50.
void __fastcall FrontEndLifecycleView::LoadResources(void *)
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
void __fastcall FrontEndLifecycleView::LoadThread(void *)
{
    FrontEndLifecycleView *controller =
        static_cast<FrontEndLifecycleView *>(g_ActiveMenuController);

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
FrontEndLifecycleView *__fastcall FrontEndLifecycleView::Create(i32 mode)
{
    FrontEndLifecycleView *controller = new FrontEndLifecycleView();
    ChainElem *elem;

    controller->flagBits.titleLoadIncomplete = 1;
    controller->entryMode = mode;

    elem = g_Chain.CreateElem(SceneSelectControllerView::OnUpdate);
    elem->arg = controller;
    g_Chain.AddToCalcChain(elem, 4);
    controller->calcChain = elem;

    elem = g_Chain.CreateElem(SceneSelectControllerView::OnDraw);
    elem->arg = controller;
    g_Chain.AddToDrawChain(elem, 1);
    controller->drawChain = elem;

    g_Supervisor.StartReplayScan(
        FrontEndLifecycleView::LoadThread, controller);
    return controller;
}

static __forceinline void FrontEndFreeReplayListData(FrontEndLifecycleView *view)
{
    if (view->replayListData != NULL)
    {
        void *data = view->replayListData;
        free(data);
    }
}
static __forceinline void FrontEndFreeMissionMessageData(FrontEndLifecycleView *view)
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
static __forceinline void FrontEndFreeGroupQueuePop(FrontEndLifecycleView *view)
{
    u32 compilerStorage;
    FrontEndFreePoppedValue(
        reinterpret_cast<void *>(view->groupPreviewDataQueue.Pop()));
}
static __forceinline void FrontEndFreeSceneQueuePop(FrontEndLifecycleView *view)
{
    u32 compilerStorage;
    FrontEndFreePoppedValue(
        reinterpret_cast<void *>(view->scenePreviewDataQueue.Pop()));
}
static __forceinline void FrontEndFreePendingPrimary(FrontEndLifecycleView *view, i32 index)
{
    if (view->pendingTextures.primaryData[index] != NULL)
    {
        void *data = view->pendingTextures.primaryData[index];
        free(data);
    }
}
static __forceinline void FrontEndFreePendingSecondary(FrontEndLifecycleView *view, i32 index)
{
    if (view->pendingTextures.secondaryData[index] != NULL)
    {
        void *data = view->pendingTextures.secondaryData[index];
        free(data);
    }
}

// FUNCTION: TH095 0x00445AA0.
FrontEndLifecycleView::~FrontEndLifecycleView()
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
void FrontEndLifecycleView::Destroy()
{
    FrontEndLifecycleView *controller = this;
    if (controller != NULL)
    {
        delete controller;
        controller = NULL;
    }
}

}
