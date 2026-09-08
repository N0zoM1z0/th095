#ifdef TH095_MATCH_EXACT
#include "PhotoGameTaskExact.inl"
#else
#include "AnmManager.hpp"
#include "AnmVmId.hpp"
#include "AsciiManager.hpp"
#include "FrontEndGlobals.hpp"
#include "GameplayGlobals.hpp"
#include "Main.hpp"
#include "ResultScreen.hpp"
#include "ScoreData.hpp"
#include "SceneData.hpp"
#include "SoundPlayer.hpp"

#include <stdio.h>
#include <string.h>

namespace th095
{

struct PhotoBackgroundManagerView
{
    static PhotoBackgroundManagerView *Create();
    void Destroy();
};

struct PhotoFrontManagerView
{
    static PhotoFrontManagerView *Create();
    void Destroy();
};

struct PhotoBulletManagerTaskView
{
    static PhotoBulletManagerTaskView *Create();
    void Destroy();
};

struct PhotoOverlayManagerView
{
    static PhotoOverlayManagerView *Create();
    void Destroy();
};

struct PhotoPlayerManagerView
{
    static PhotoPlayerManagerView *Create();
    void Destroy();
};

struct PhotoEnemyManagerTaskView
{
    static PhotoEnemyManagerTaskView *Create();
    void Destroy();
    void RestartPhotoTargetEcls();
};

struct PhotoItemManagerTaskView
{
    static PhotoItemManagerTaskView *Create();
    void Destroy();
};

struct PhotoLaserManagerTaskView
{
    static PhotoLaserManagerTaskView *Create();
    void Destroy();
};

struct ReplayManagerTaskView
{
    static ReplayManagerTaskView *__fastcall Create(i32 mode, char *path);
    void Destroy();
};

struct PhotoCardInfoView
{
    void Destroy();
};

struct PhotoGameFileSystemView
{
    static BOOL CheckIfFileAlreadyExists(LPCSTR path);
};

struct PhotoHelpMenuTaskView
{
    u8 unknown000[0x6108];
    i32 closeRequested;
};

struct PhotoStageStateTaskView
{
    u8 unknown000[0x17720];
    AnmVmId capturedPhotoVms[11];
    u8 unknown1774c[0x25720 - 0x1774c];
    u32 flags;
};

struct PhotoCapacityCounterTaskView
{
    i32 capturedPhotoCount;
    u8 unknown004[4];
    i32 photoCapacity;

    operator i32()
    {
        return this->photoCapacity;
    }
};

struct PhotoReplayInputButtonsTaskView
{
    u16 current;

    u16 operator&(u16 mask)
    {
        return this->current & mask;
    }
};

struct PhotoGameRuntimeTaskView
{
    u8 unknown000[0x1e34];
    f32 hudFade;
    u8 unknown1e38[0x29e4 - 0x1e38];
    PhotoCapacityCounterTaskView photoCounter;
};

struct PhotoAsciiManagerTaskView
{
    i32 AddFormatText(Float3 *position, const char *format, ...);
};

struct PhotoRuntimeConfigView
{
    u32 values[50];

    PhotoRuntimeConfigView()
    {
        this->Initialize();
    }

    void Initialize();
};

struct PhotoCompletionStateTaskView
{
    i32 unknown000;
    ZunTimer timer;
};

struct PhotoGameTaskDrawHudLocals
{
    i32 capturedPhotoCount;
    i32 photoCapacity;
    Float3 extraScenePosition;
    Float3 scenePosition;
    Float3 photoCountPosition;
    Float3 scorePosition;
    Float3 highScorePosition;
    u32 alpha;
};

typedef char PhotoGameTaskDrawHudLocalsSizeIs48[
    (sizeof(PhotoGameTaskDrawHudLocals) == 0x48) ? 1 : -1];
typedef char PhotoGameTaskDrawHudAlphaAt44[
    (offsetof(PhotoGameTaskDrawHudLocals, alpha) == 0x44) ? 1 : -1];

struct PhotoGameTaskUpdateLocals
{
    PhotoHelpMenuTaskView *activeHelpMenu;
    PhotoHelpMenuTaskView *shutdownHelpMenu;
    i32 previousSecond;
    i32 j;
    i32 i;
};

typedef char PhotoGameTaskUpdateLocalsSizeIs14[
    (sizeof(PhotoGameTaskUpdateLocals) == 0x14) ? 1 : -1];
typedef char PhotoGameTaskUpdateLoopIAt10[
    (offsetof(PhotoGameTaskUpdateLocals, i) == 0x10) ? 1 : -1];

typedef char PhotoRuntimeConfigSizeIsC8[
    (sizeof(PhotoRuntimeConfigView) == 0xc8) ? 1 : -1];

struct PhotoGameTaskView
{
    PhotoBackgroundManagerView *background; // +0x000
    PhotoFrontManagerView *front;            // +0x004
    PhotoBulletManagerTaskView *bullets;     // +0x008
    PhotoPlayerManagerView *player;          // +0x00c
    ReplayManagerTaskView *replay;           // +0x010
    PhotoEnemyManagerTaskView *enemies;      // +0x014
    PhotoOverlayManagerView *photoOverlay;   // +0x018
    PhotoItemManagerTaskView *items;         // +0x01c
    ResultScreen *pause;                     // +0x020
    PhotoLaserManagerTaskView *lasers;       // +0x024
    ZunTimer stageTimer;                     // +0x028
    PhotoRuntimeConfigView runtimeConfig;    // +0x034
    u32 flags;                               // +0x0fc
    i32 bestShotIndex;                       // +0x100
    PhotoCompletionStateTaskView completion; // +0x104
    i32 score;                               // +0x114
    ChainElem *calcChain;                    // +0x118
    ChainElem *drawChain;                    // +0x11c
    i32 replayMode;                          // +0x120

    PhotoGameTaskView();
    ~PhotoGameTaskView();

    static PhotoGameTaskView *__fastcall Create(i32 replayMode);
    void Destroy();
    static void __fastcall Load(void *argument);
    static i32 __fastcall OnUpdate(PhotoGameTaskView *task);
    static i32 __fastcall OnDraw(PhotoGameTaskView *task);
    i32 InitializeSubsystems();
    i32 Update();
    i32 DrawHud();
};

typedef char PhotoGameTaskSizeIs124[
    (sizeof(PhotoGameTaskView) == 0x124) ? 1 : -1];
typedef char PhotoGameTaskConfigAt34[
    (offsetof(PhotoGameTaskView, runtimeConfig) == 0x34) ? 1 : -1];
typedef char PhotoGameTaskCompletionAt104[
    (offsetof(PhotoGameTaskView, completion) == 0x104) ? 1 : -1];
typedef char PhotoGameTaskCompletionTimerAt108[
    (offsetof(PhotoGameTaskView, completion.timer) == 0x108) ? 1 : -1];
typedef char PhotoGameTaskChainsAt118[
    (offsetof(PhotoGameTaskView, calcChain) == 0x118) ? 1 : -1];

extern PhotoRuntimeConfigView g_PhotoRuntimeConfig;
extern char g_ReplayPath[];
#ifndef DIFFBUILD
extern char g_SelectedReplayPath[0x100];
#define g_ReplayPath g_SelectedReplayPath
#endif
extern PhotoCardInfoView *g_PhotoCardInfo;
extern PhotoGameTaskView *g_PhotoGameTask;
extern PhotoStageStateTaskView *g_PhotoStageState;
#define g_PhotoStageState \
    TH095_RUNTIME_GLOBAL_PTR(PhotoStageStateTaskView, g_RuntimeStageStateOwner)
extern PhotoGameRuntimeTaskView *g_PhotoGameRuntime;
extern PhotoEnemyManagerTaskView *g_PhotoEnemyManagerTask;
#define g_PhotoEnemyManagerTask \
    TH095_RUNTIME_GLOBAL_PTR(PhotoEnemyManagerTaskView, g_RuntimeEnemyManagerOwner)
extern u32 g_PhotoAsciiTextColor;
extern PhotoReplayInputButtonsTaskView g_ReplayInputButtons;
extern i32 g_PhotoNextState;
extern i32 g_ReplayUsesArchive;
extern i32 g_PhotoLoadWaitFlag;
extern i32 g_HelpLoadComplete;
extern i32 g_HelpLoadActive;

#ifndef DIFFBUILD
#define g_PhotoGameRuntime \
    TH095_RUNTIME_GLOBAL_PTR(PhotoGameRuntimeTaskView, g_RuntimePlayerOwner)
#define g_PhotoGameTask \
    TH095_RUNTIME_GLOBAL_PTR(PhotoGameTaskView, g_RuntimeGameTaskOwner)
#endif

PhotoGameTaskView::PhotoGameTaskView()
{
    utils::DebugPrint("pBinitialize GameTaskInf\n");
    memset(this, 0, sizeof(PhotoGameTaskView));
}

i32 PhotoGameTaskView::Update()
{
    PhotoGameTaskUpdateLocals locals;

    if (((this->flags >> 3) & 1) != 0)
    {
        g_Supervisor.StopReplayScan();
        g_PhotoNextState = 6;
        if (g_ActiveMenuController != NULL)
        {
            locals.shutdownHelpMenu = reinterpret_cast<PhotoHelpMenuTaskView *>(
                g_ActiveMenuController);
            locals.shutdownHelpMenu->closeRequested = 1;
        }
        return 1;
    }

    if (((this->flags >> 2) & 1) != 0)
    {
        this->flags = this->flags | 0x80;
        return 1;
    }

    g_Supervisor.StopReplayScan();
    if (g_ActiveMenuController != NULL)
    {
        locals.activeHelpMenu = reinterpret_cast<PhotoHelpMenuTaskView *>(
            g_ActiveMenuController);
        locals.activeHelpMenu->closeRequested = 1;
        return 1;
    }

    if (g_ReplayUsesArchive != 0 &&
        ((g_ReplayInputButtons & 0x160b) != 0 ||
         ((this->flags >> 4) & 1) != 0 ||
         ((this->flags >> 5) & 1) != 0 ||
         ((this->flags >> 6) & 1) != 0))
    {
        g_PhotoNextState = 2;
    }

    if (((this->flags >> 4) & 1) != 0)
    {
        if (((this->flags >> 6) & 1) != 0)
        {
            for (locals.i = 0;
                 locals.i < g_PhotoGameRuntime->photoCounter;
                 locals.i++)
            {
                AnmManager::ExecuteScript(
                    g_PhotoStageState
                        ->capturedPhotoVms[locals.i]
                        .GetVm());
            }
        }
        return 3;
    }

    if (((this->flags >> 5) & 1) != 0)
    {
        return 3;
    }

    if (((this->flags >> 6) & 1) != 0)
    {
        for (locals.j = 0;
             locals.j < g_PhotoGameRuntime->photoCounter;
             locals.j++)
        {
            AnmManager::ExecuteScript(
                g_PhotoStageState
                    ->capturedPhotoVms[locals.j]
                    .GetVm());
        }
        return 3;
    }

    if (this->completion.timer > 0)
    {
        locals.previousSecond =
            static_cast<i32>(this->completion.timer) / 60;
        this->completion.timer--;
        if (static_cast<i32>(this->completion.timer) / 60 <= 5 &&
            locals.previousSecond !=
                static_cast<i32>(this->completion.timer) / 60)
        {
            g_SoundPlayer.PlaySoundByIdx(static_cast<SoundIdx>(0x24), 0);
        }
        else if (static_cast<i32>(this->completion.timer) / 60 <= 10 &&
                 locals.previousSecond !=
                     static_cast<i32>(this->completion.timer) / 60)
        {
            g_SoundPlayer.PlaySoundByIdx(static_cast<SoundIdx>(0x1b), 0);
        }

        if (this->completion.timer <= 0)
        {
            g_PhotoEnemyManagerTask->RestartPhotoTargetEcls();
        }
    }

    this->stageTimer++;
    return 1;
}

i32 PhotoGameTaskView::DrawHud()
{
    PhotoGameTaskDrawHudLocals locals;

    if (((this->flags >> 2) & 1) != 0)
    {
        return 1;
    }
    if (((g_PhotoStageState->flags >> 2) & 1) == 0)
    {
        locals.alpha = 0xff;
        if (g_PhotoGameRuntime->hudFade < 64.0f)
        {
            locals.alpha = 0x40;
        }
        else if (g_PhotoGameRuntime->hudFade < 128.0f)
        {
            locals.alpha =
                ((u32)(g_PhotoGameRuntime->hudFade - 64.0f) * 0xbf >> 6) +
                0x40;
        }

        g_PhotoAsciiTextColor = locals.alpha << 24 | 0xffffff;
        g_AsciiManager.AddFormatText(
            ((locals.highScorePosition.x = 128.0f),
             (locals.highScorePosition.y = 19.0f),
             (locals.highScorePosition.z = 0.0f),
             &locals.highScorePosition),
            "HiScore %.7d",
            g_ResultSaveData
                        ->scoreEntries[g_PhotoGameTask->bestShotIndex]
                        .score > this->score
                ? g_ResultSaveData
                      ->scoreEntries[g_PhotoGameTask->bestShotIndex]
                      .score
                : this->score);
        locals.scorePosition.x = 128.0f;
        locals.scorePosition.y = 32.0f;
        locals.scorePosition.z = 0.0f;
        g_AsciiManager.AddFormatText(
            &locals.scorePosition,
            "  Score %.7d",
            this->score);

        g_PhotoAsciiTextColor = locals.alpha << 24 | 0xdfefff;
        locals.photoCapacity =
            g_PhotoGameRuntime->photoCounter.photoCapacity;
        locals.capturedPhotoCount =
            g_PhotoGameRuntime->photoCounter.capturedPhotoCount;
        locals.photoCountPosition.x = 409.0f;
        locals.photoCountPosition.y = 19.0f;
        locals.photoCountPosition.z = 0.0f;
        g_AsciiManager.AddFormatText(
            &locals.photoCountPosition,
            "Photo %.2d/%.2d",
            locals.capturedPhotoCount,
            locals.photoCapacity);

        if (g_SelectedScene->level != 10)
        {
            locals.scenePosition.x = 472.0f;
            locals.scenePosition.y = 32.0f;
            locals.scenePosition.z = 0.0f;
            g_AsciiManager.AddFormatText(
                &locals.scenePosition,
                "%2d-%d",
                g_SelectedScene->level + 1,
                g_SelectedScene->scene + 1);
        }
        else
        {
            locals.extraScenePosition.x = 472.0f;
            locals.extraScenePosition.y = 32.0f;
            locals.extraScenePosition.z = 0.0f;
            g_AsciiManager.AddFormatText(
                &locals.extraScenePosition,
                "EX-%d",
                g_SelectedScene->scene + 1);
        }
        g_PhotoAsciiTextColor = 0xffffffff;
    }
    return 1;
}

PhotoGameTaskView *PhotoGameTaskView::Create(i32 replayMode)
{
    struct
    {
        PhotoGameTaskView *task;
        ChainElem *elem;
    } locals;

    locals.task = new PhotoGameTaskView();
    g_PhotoGameTask = locals.task;
    locals.task->replayMode = replayMode;
    locals.task->flags = locals.task->flags | 4;

    locals.elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoGameTaskView::OnUpdate));
    locals.elem->arg = locals.task;
    g_Chain.AddToCalcChain(locals.elem, 6);
    locals.task->calcChain = locals.elem;

    locals.elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoGameTaskView::OnDraw));
    locals.elem->arg = locals.task;
    g_Chain.AddToDrawChain(locals.elem, 2);
    locals.task->drawChain = locals.elem;

    g_Supervisor.StartReplayScan(PhotoGameTaskView::Load, NULL);
    return locals.task;
}

void PhotoGameTaskView::Destroy()
{
    PhotoGameTaskView *task = this;
    if (task != NULL)
    {
        delete task;
        task = NULL;
    }
}

void __fastcall PhotoGameTaskView::Load(void *argument)
{
    PhotoGameTaskView *task = g_PhotoGameTask;
    task->flags = task->flags | 4;

    while (g_AnmManager->captureSurfaceIdx >= 0 ||
           g_AnmManager->captureAnmIdx >= 0)
    {
        if (((g_Supervisor.flags.raw >> 7) & 1) != 0)
        {
            goto failure;
        }
        Sleep(1);
    }

    if (task->InitializeSubsystems() != ZUN_SUCCESS)
    {
        goto failure;
    }

    while (g_PhotoLoadWaitFlag != 0)
    {
        Sleep(16);
    }

    if (((g_Supervisor.flags.raw >> 9) & 1) == 0)
    {
        if (((g_Supervisor.flags.raw >> 12) & 1) == 0)
        {
            task->flags = task->flags | 0x100;
        }
        else
        {
            g_Supervisor.flags.raw &= ~0x1000;
            g_Supervisor.PlayMusic(0, 0);
        }
    }

    g_Supervisor.HideLoadingVms();
    task->flags = task->flags & ~4;
    g_Supervisor.flags.raw &= ~0x200;
    g_HelpLoadActive = 0;
    g_HelpLoadComplete = 1;
    return;

failure:
    task->flags = task->flags | 8;
    g_Supervisor.BeginLoadingCompletion();
    g_HelpLoadActive = 0;
    g_HelpLoadComplete = 1;
}

i32 __fastcall PhotoGameTaskView::OnUpdate(PhotoGameTaskView *task)
{
    return task->Update();
}

i32 __fastcall PhotoGameTaskView::OnDraw(PhotoGameTaskView *task)
{
    return task->DrawHud();
}

i32 PhotoGameTaskView::InitializeSubsystems()
{
    char bestShotPath[0x108];

    this->runtimeConfig = g_PhotoRuntimeConfig;
    this->replay = ReplayManagerTaskView::Create(
        this->replayMode, g_ReplayPath);
    if (this->replay == NULL)
    {
        return ZUN_ERROR;
    }

    this->bestShotIndex = g_SelectedScene->bestShotIndex;
    utils::DebugPrint(
        "Start %d-%d\n",
        g_SelectedScene->level + 1,
        g_SelectedScene->scene + 1);

    if (g_SelectedScene->level != 10)
    {
        sprintf(
            bestShotPath,
            "bestshot/bs_%.2d_%d.dat",
            g_SelectedScene->level + 1,
            g_SelectedScene->scene + 1);
    }
    else
    {
        sprintf(
            bestShotPath,
            "bestshot/bs_ex_%d.dat",
            g_SelectedScene->scene + 1);
    }

    if (!PhotoGameFileSystemView::CheckIfFileAlreadyExists(bestShotPath))
    {
        g_ResultSaveData->scoreEntries[this->bestShotIndex].attemptCount = 0;
        g_ResultSaveData->scoreEntries[this->bestShotIndex].detailScore = 0;
    }

    this->background = PhotoBackgroundManagerView::Create();
    if (this->background == NULL)
    {
        return ZUN_ERROR;
    }
    this->front = PhotoFrontManagerView::Create();
    if (this->front == NULL)
    {
        return ZUN_ERROR;
    }
    this->bullets = PhotoBulletManagerTaskView::Create();
    if (this->bullets == NULL)
    {
        return ZUN_ERROR;
    }
    this->photoOverlay = PhotoOverlayManagerView::Create();
    if (this->photoOverlay == NULL)
    {
        return ZUN_ERROR;
    }
    this->player = PhotoPlayerManagerView::Create();
    if (this->player == NULL)
    {
        return ZUN_ERROR;
    }
    this->enemies = PhotoEnemyManagerTaskView::Create();
    if (this->enemies == NULL)
    {
        return ZUN_ERROR;
    }
    this->items = PhotoItemManagerTaskView::Create();
    if (this->items == NULL)
    {
        return ZUN_ERROR;
    }
    this->pause = ResultScreen::Create();
    if (this->pause == NULL)
    {
        return ZUN_ERROR;
    }
    this->lasers = PhotoLaserManagerTaskView::Create();
    if (this->lasers == NULL)
    {
        return ZUN_ERROR;
    }

    if (((g_Supervisor.flags.raw >> 9) & 1) == 0 &&
        g_ReplayUsesArchive == 0)
    {
        g_Supervisor.LoadMusic(
            0, g_SelectedScene->musicPath);
    }
    g_Supervisor.lagDenominator = g_Supervisor.lagNumerator = 0.0;
    return ZUN_SUCCESS;
}

PhotoGameTaskView::~PhotoGameTaskView()
{
    utils::DebugPrint("shitdown GameTaskInf\n");
    this->background->Destroy();
    this->front->Destroy();
    this->bullets->Destroy();
    this->player->Destroy();
    this->replay->Destroy();
    this->enemies->Destroy();
    this->photoOverlay->Destroy();
    this->items->Destroy();
    this->pause->Destroy();
    this->lasers->Destroy();
    if (g_PhotoCardInfo != NULL)
    {
        g_PhotoCardInfo->Destroy();
    }
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);
    g_PhotoGameTask = NULL;

    if (((g_Supervisor.flags.raw >> 9) & 1) == 0 &&
        g_ReplayUsesArchive == 0)
    {
        g_Supervisor.StopAudio();
    }
    if (((g_Supervisor.flags.raw >> 9) & 1) != 0)
    {
        g_PhotoScreenFadeColor = 0;
    }
    else
    {
        g_PhotoScreenFadeColor = 0xff000000;
    }
}

} // namespace th095

#endif // TH095_MATCH_EXACT
