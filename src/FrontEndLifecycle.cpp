#ifdef TH095_MATCH_EXACT
#define TH095_MATCH_GAME_ERROR_CONTEXT_AS_CLASS
#endif
#include "Global.hpp"
#include "FrontEndGlobals.hpp"
#include "Main.hpp"
#include "ResultScreen.hpp"
#include "ScoreData.hpp"
#include "SceneData.hpp"
#include "SoundPlayer.hpp"
#if !defined(DIFFBUILD) && !defined(TH095_MATCH_EXACT)
#include "SceneSelect.hpp"
#endif
#include "utils.hpp"

namespace th095
{

DIFFABLE_STATIC(void *, g_ActiveMenuController);

i32 LoadPhotoBulletAnm();
i32 LoadPhotoAnm();
i32 LoadPhotoFrontAnm();
i32 LoadPhotoPlayerAnm();

// Target 0x004CA2F8 is the zero-initialized completion flag written by this
// resource-loader lifecycle.
DIFFABLE_STATIC(i32, g_FrontEndLoadActive);
extern i32 g_HelpLoadComplete;
extern i32 g_HelpLoadActive;

struct FrontEndLifecycleView;

#ifdef TH095_MATCH_EXACT
extern FrontEndLifecycleView *g_FrontEndController;
extern ResultSaveDataView *g_FrontEndResultSaveData;

struct FrontEndAnmManagerView
{
    u8 unknown000[8];
    i32 surfaceCaptureIndex;
    i32 textureCaptureIndex;

    void *PreloadAnm(i32 anmIndex, const char *path);
    void ReleaseAnm(i32 anmIndex);
};
extern FrontEndAnmManagerView *g_FrontEndAnmManager;

struct FrontEndSupervisorView
{
    i32 StartReplayScan(void (__fastcall *callback)(void *), void *argument);
    void HideLoadingVms();
    void BeginLoadingCompletion();
};
extern FrontEndSupervisorView g_FrontEndSupervisor;
extern u32 g_FrontEndSupervisorFlags;
extern i32 g_FrontEndLoadFinished;
extern i32 g_FrontEndLoadInProgress;

struct FrontEndSceneDefinitionView
{
    u8 unknown000[0x24];
    i32 textId;
    char *text;
    u8 displayState;
    u8 unknown02d[3];
};
extern FrontEndSceneDefinitionView *g_FrontEndSceneGroups[12];

#define TH095_FRONT_CONTROLLER_STORAGE g_FrontEndController
#define TH095_FRONT_CONTROLLER_VALUE g_FrontEndController
#define TH095_FRONT_ANM_PRELOAD(index, path) g_FrontEndAnmManager->PreloadAnm((index), (path))
#define TH095_FRONT_ANM_RELEASE(index) g_FrontEndAnmManager->ReleaseAnm(index)
#define TH095_FRONT_SURFACE_CAPTURE_INDEX g_FrontEndAnmManager->surfaceCaptureIndex
#define TH095_FRONT_TEXTURE_CAPTURE_INDEX g_FrontEndAnmManager->textureCaptureIndex
#define TH095_FRONT_SUPERVISOR_FLAGS g_FrontEndSupervisorFlags
#define TH095_FRONT_HIDE_LOADING() g_FrontEndSupervisor.HideLoadingVms()
#define TH095_FRONT_BEGIN_LOADING_COMPLETION() g_FrontEndSupervisor.BeginLoadingCompletion()
#define TH095_FRONT_START_REPLAY_SCAN(callback, argument) g_FrontEndSupervisor.StartReplayScan((callback), (argument))
#define TH095_FRONT_LOAD_FINISHED g_FrontEndLoadFinished
#define TH095_FRONT_LOAD_IN_PROGRESS g_FrontEndLoadInProgress
#define TH095_FRONT_RESULT_SAVE_DATA g_FrontEndResultSaveData
#define TH095_FRONT_SCENE_GROUPS g_FrontEndSceneGroups
#else
#define TH095_FRONT_CONTROLLER_STORAGE g_ActiveMenuController
#define TH095_FRONT_CONTROLLER_VALUE reinterpret_cast<FrontEndLifecycleView *>(g_ActiveMenuController)
#define TH095_FRONT_ANM_PRELOAD(index, path) g_AnmManager->PreloadAnm((index), (path))
#define TH095_FRONT_ANM_RELEASE(index) g_AnmManager->ReleaseAnm(index)
#define TH095_FRONT_SURFACE_CAPTURE_INDEX g_AnmManager->captureSurfaceIdx
#define TH095_FRONT_TEXTURE_CAPTURE_INDEX g_AnmManager->captureAnmIdx
#define TH095_FRONT_SUPERVISOR_FLAGS g_Supervisor.flags.raw
#define TH095_FRONT_HIDE_LOADING() g_Supervisor.HideLoadingVms()
#define TH095_FRONT_BEGIN_LOADING_COMPLETION() g_Supervisor.BeginLoadingCompletion()
#define TH095_FRONT_START_REPLAY_SCAN(callback, argument) g_Supervisor.StartReplayScan((callback), (argument))
#define TH095_FRONT_LOAD_FINISHED g_HelpLoadComplete
#define TH095_FRONT_LOAD_IN_PROGRESS g_HelpLoadActive
#define TH095_FRONT_RESULT_SAVE_DATA g_ResultSaveData
#define TH095_FRONT_SCENE_GROUPS g_SceneGroups
#endif

extern i32 g_SoundInitializationComplete;
extern i32 g_MusicArchiveBaseOffset;
extern u32 g_FrontEndConfigurationFlags;

struct FrontEndMissionEntryView
{
    u16 group;
    u16 scene;
    u8 displayState;
    u8 unknown005[3];
    i32 textId;
    char text[1];
};

struct FrontEndInitializeLocals
{
    i32 i;
    FrontEndMissionEntryView *entry;
    i32 *offset;
    i32 count;
    i32 missionSize;
};

struct FrontEndPointerQueueView
{
    i32 values[16];
    i32 count;
    i32 capacity;

    FrontEndPointerQueueView()
    {
        memset(this, 0, sizeof(*this));
        this->capacity = 16;
    }

    i32 Pop();
    i32 Size()
    {
        return this->count;
    }
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
    void *sceneAnm;
    void *transitionAnm;
    FrontEndTimerView stateTimer;
    FrontEndTimerView transitionTimer;
    ResultScreenReplayCursor groupCursor;
    ResultScreenReplayCursor sceneCursor;
    ResultScreenReplayCursor sceneCursors[12];
    u8 unknown0bf0[0x26c];
    void *missionMessageData;
    char *specialText[10];
    i8 specialDisplayStates[10];
    i8 currentDisplayState;
    u8 unknown0e93[0x15];
    ReplayManager *replays[80];
    u8 unknown0fe8[0x14];
    void *replayListData;
    u8 unknown1000[0x5120];
    u32 flags;
    i32 entryMode;
    FrontEndPointerQueueView selectionQueue;
    FrontEndPointerQueueView loadedSceneQueue;
    FrontEndPointerQueueView groupPreviewDataQueue;
    FrontEndPointerQueueView groupPreviewSizeQueue;
    FrontEndPointerQueueView scenePreviewDataQueue;
    FrontEndPointerQueueView scenePreviewSizeQueue;
    FrontEndPointerQueueView groupPreviewQueue;
    FrontEndPointerQueueView scenePreviewQueue;
    FrontEndPointerQueueView loadedGroupQueue;
    u8 unknown63b0[0x20];
    void *pendingPrimaryData[3];
    i32 pendingPrimarySize[3];
    void *pendingSecondaryData[3];
    i32 pendingSecondarySize[3];
    ChainElem *calcChain;
    ChainElem *drawChain;
    u8 unknown6408[0x10c];

    FrontEndLifecycleView();
    ~FrontEndLifecycleView();
    i32 Initialize();
    static void __fastcall LoadResources(void *unused);
    static void ReleaseResources();
    static void __fastcall LoadThread(void *unused);
    static void __fastcall OnUpdate(void *controller);
    static void __fastcall OnDraw(void *controller);
    static FrontEndLifecycleView *__fastcall Create(i32 entryMode);
    void Destroy();
};

typedef char FrontEndLifecycleMissionDataAtE5C[
    (offsetof(FrontEndLifecycleView, missionMessageData) == 0xe5c)
        ? 1
        : -1];
typedef char FrontEndLifecycleSpecialTextAtE60[
    (offsetof(FrontEndLifecycleView, specialText) == 0xe60) ? 1 : -1];
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
    (offsetof(FrontEndLifecycleView, flags) == 0x6120) ? 1 : -1];
typedef char FrontEndLifecycleEntryModeAt6124[
    (offsetof(FrontEndLifecycleView, entryMode) == 0x6124) ? 1 : -1];
typedef char FrontEndLifecycleSceneDataQueueAt6248[
    (offsetof(FrontEndLifecycleView, scenePreviewDataQueue) == 0x6248)
        ? 1
        : -1];
typedef char FrontEndLifecyclePendingPrimaryAt63D0[
    (offsetof(FrontEndLifecycleView, pendingPrimaryData) == 0x63d0)
        ? 1
        : -1];
typedef char FrontEndLifecyclePendingSecondaryAt63E8[
    (offsetof(FrontEndLifecycleView, pendingSecondaryData) == 0x63e8)
        ? 1
        : -1];
typedef char FrontEndLifecycleChainsAt6400[
    (offsetof(FrontEndLifecycleView, calcChain) == 0x6400 &&
     offsetof(FrontEndLifecycleView, drawChain) == 0x6404)
        ? 1
        : -1];
typedef char FrontEndLifecycleSizeIs6514[
    (sizeof(FrontEndLifecycleView) == 0x6514) ? 1 : -1];

#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
#define TH095_FRONT_END_ON_UPDATE FrontEndLifecycleView::OnUpdate
#define TH095_FRONT_END_ON_DRAW FrontEndLifecycleView::OnDraw
#else
#define TH095_FRONT_END_ON_UPDATE SceneSelectControllerView::OnUpdate
#define TH095_FRONT_END_ON_DRAW SceneSelectControllerView::OnDraw
#endif

// FUNCTION: TH095 0x00445440.
FrontEndLifecycleView::FrontEndLifecycleView()
{
    utils::DebugPrint("initialize TitleTaskInf\n");
    memset(this, 0, sizeof(*this));
    TH095_FRONT_CONTROLLER_STORAGE = this;
}

// FUNCTION: TH095 0x004456F0.
i32 FrontEndLifecycleView::Initialize()
{
    FrontEndInitializeLocals locals;

    this->sceneAnm =
        TH095_FRONT_ANM_PRELOAD(11, "title.anm");
    if (this->sceneAnm == NULL)
    {
        g_GameErrorContext.Log("title data is corrupt\r\n");
        return -1;
    }

    this->transitionAnm =
        TH095_FRONT_ANM_PRELOAD(12, "title_v.anm");
    if (this->transitionAnm == NULL)
    {
        g_GameErrorContext.Log("title data is corrupt\r\n");
        return -1;
    }

    this->missionMessageData =
        FileSystem::OpenFile("sprt/mission.msg", &locals.missionSize, FALSE);
    if (this->missionMessageData == NULL)
    {
        g_GameErrorContext.Log("mission.msg data is corrupt\r\n");
        return -1;
    }

    locals.count = *reinterpret_cast<i32 *>(this->missionMessageData);
    locals.offset = reinterpret_cast<i32 *>(this->missionMessageData) + 1;
    for (locals.i = 0; locals.i < locals.count; locals.i++)
    {
        locals.entry = reinterpret_cast<FrontEndMissionEntryView *>(
            *locals.offset +
            reinterpret_cast<i32>(this->missionMessageData));
        if (locals.entry->group < 12)
        {
            TH095_FRONT_SCENE_GROUPS[locals.entry->group][locals.entry->scene]
                .text = locals.entry->text;
            TH095_FRONT_SCENE_GROUPS[locals.entry->group][locals.entry->scene]
                .displayState = locals.entry->displayState;
            TH095_FRONT_SCENE_GROUPS[locals.entry->group][locals.entry->scene]
                .textId = locals.entry->textId;
        }
        else
        {
            this->specialText[locals.entry->scene] = locals.entry->text;
            this->specialDisplayStates[locals.entry->scene] =
                locals.entry->displayState;
        }
        locals.offset++;
    }

    this->currentDisplayState = -1;
    this->flags |= 0x10;
    if (g_SoundInitializationComplete == 0)
    {
        g_SoundPlayer.InitSoundBuffers();
        if (g_MusicArchiveBaseOffset == 0)
        {
            if (((g_FrontEndConfigurationFlags >> 4) & 1) == 0)
                g_SoundPlayer.StartBGM("thbgm.dat");
            else
                strcpy(g_SoundPlayer.currentBgmFileName, "thbgm.dat");
        }
        else
        {
            if (((g_FrontEndConfigurationFlags >> 4) & 1) == 0)
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
void FrontEndLifecycleView::ReleaseResources()
{
    TH095_FRONT_ANM_RELEASE(11);
    TH095_FRONT_ANM_RELEASE(12);
}

// FUNCTION: TH095 0x00445980.
void __fastcall FrontEndLifecycleView::LoadThread(void *)
{
    FrontEndLifecycleView *controller =
        TH095_FRONT_CONTROLLER_VALUE;

    while (TH095_FRONT_SURFACE_CAPTURE_INDEX >= 0 ||
           TH095_FRONT_TEXTURE_CAPTURE_INDEX >= 0)
    {
        if (((TH095_FRONT_SUPERVISOR_FLAGS >> 7) & 1) != 0)
            goto loadFailed;
        Sleep(1);
    }

    if (controller->Initialize() != 0)
        goto loadFailed;

    TH095_FRONT_HIDE_LOADING();
    controller->flags &= ~1u;
    utils::DebugPrint("Title Load Thread Finish\n");
    TH095_FRONT_LOAD_IN_PROGRESS = 0;
    TH095_FRONT_LOAD_FINISHED = 1;
    goto loadDone;

loadFailed:
    controller->flags |= 2;
    TH095_FRONT_BEGIN_LOADING_COMPLETION();
    TH095_FRONT_LOAD_IN_PROGRESS = 0;
    TH095_FRONT_LOAD_FINISHED = 1;

loadDone:
    return;
}

// FUNCTION: TH095 0x00445CC0.
FrontEndLifecycleView *__fastcall FrontEndLifecycleView::Create(i32 mode)
{
    FrontEndLifecycleView *controller = new FrontEndLifecycleView();
    ChainElem *elem;

    controller->flags |= 1;
    controller->entryMode = mode;

    elem = g_Chain.CreateElem((ChainCallback)TH095_FRONT_END_ON_UPDATE);
    elem->arg = controller;
    g_Chain.AddToCalcChain(elem, 4);
    controller->calcChain = elem;

    elem = g_Chain.CreateElem((ChainCallback)TH095_FRONT_END_ON_DRAW);
    elem->arg = controller;
    g_Chain.AddToDrawChain(elem, 1);
    controller->drawChain = elem;

    TH095_FRONT_START_REPLAY_SCAN(
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
    if (view->missionMessageData != NULL)
    {
        void *data = view->missionMessageData;
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
#if !defined(DIFFBUILD) && !defined(TH095_MATCH_EXACT)
        reinterpret_cast<void *>(reinterpret_cast<SceneValueQueue *>(
                                      &view->groupPreviewDataQueue)
                                      ->Pop()));
#else
        reinterpret_cast<void *>(view->groupPreviewDataQueue.Pop()));
#endif
}
static __forceinline void FrontEndFreeSceneQueuePop(FrontEndLifecycleView *view)
{
    u32 compilerStorage;
    FrontEndFreePoppedValue(
#if !defined(DIFFBUILD) && !defined(TH095_MATCH_EXACT)
        reinterpret_cast<void *>(reinterpret_cast<SceneValueQueue *>(
                                      &view->scenePreviewDataQueue)
                                      ->Pop()));
#else
        reinterpret_cast<void *>(view->scenePreviewDataQueue.Pop()));
#endif
}
static __forceinline void FrontEndFreePendingPrimary(FrontEndLifecycleView *view, i32 index)
{
    if (view->pendingPrimaryData[index] != NULL)
    {
        void *data = view->pendingPrimaryData[index];
        free(data);
    }
}
static __forceinline void FrontEndFreePendingSecondary(FrontEndLifecycleView *view, i32 index)
{
    if (view->pendingSecondaryData[index] != NULL)
    {
        void *data = view->pendingSecondaryData[index];
        free(data);
    }
}

// FUNCTION: TH095 0x00445AA0.
FrontEndLifecycleView::~FrontEndLifecycleView()
{
    i32 replayIndex;
    i32 pendingIndex;

    TH095_FRONT_RESULT_SAVE_DATA->WriteBestShotData();
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
    TH095_FRONT_CONTROLLER_STORAGE = NULL;

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
