#ifdef TH095_MATCH_EXACT
#include "PhotoGameTaskExact.inl"
#else
#include "AnmManager.hpp"
#include "AnmVmId.hpp"
#include "AsciiManager.hpp"
#include "FrontEndGlobals.hpp"
#include "GameplayGlobals.hpp"
#include "InputRuntime.hpp"
#include "Main.hpp"
#include "PhotoGameTask.hpp"
#include "PhotoEffectRuntime.hpp"
#include "ReplayManager.hpp"
#include "ResultScreen.hpp"
#include "ScoreData.hpp"
#include "SceneData.hpp"
#include "SoundPlayer.hpp"

#include <stdio.h>
#include <string.h>

namespace th095
{

#if defined(TH095_MATCH_EXACT)
#define PHOTO_GAME_TASK_GAMEPLAY_LOAD_ACTIVE 4
#else
enum PhotoGameTaskGlobalFlags
{
    PHOTO_GAME_TASK_GAMEPLAY_LOAD_ACTIVE = 1 << 2,
};
#endif

struct Background
{
    ~Background();
    static Background *__fastcall Create();
};

struct PhotoFrontManagerView
{
    static PhotoFrontManagerView *Create();
    void Destroy();
};

struct PhotoBulletManagerView
{
    static PhotoBulletManagerView *__fastcall Create();
    void Destroy();
};

struct PhotoOverlayManagerView
{
    static PhotoOverlayManagerView *Create();
    void Destroy();
};

struct PhotoGameUpdateView
{
    static PhotoGameUpdateView *__fastcall Create();
    void Destroy();
};

struct PhotoEnemyManagerTaskView
{
    static PhotoEnemyManagerTaskView *Create();
};

struct PhotoEnemyManagerView
{
    void Destroy();
    static void __fastcall RestartPhotoTargetEcls(
        PhotoEnemyManagerView *enemyManager);
};

struct PhotoItemManagerView
{
    static PhotoItemManagerView *__fastcall Create();
    void Destroy();
};


struct PhotoCardInfoView
{
    void Destroy();
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
    (sizeof(GameConfiguration) == 0xc8) ? 1 : -1];

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
#ifndef DIFFBUILD
// Target 0x004B1FEC is g_AsciiManager @ 0x004A9F80 + 0x806C, the
// canonical ZunColor storage used by every following AddFormatText call.
#define g_PhotoAsciiTextColor (g_AsciiManager.color.color)
#endif
extern i32 g_PhotoNextState;
#ifndef DIFFBUILD
#define g_PhotoNextState (g_Supervisor.currentState)
#endif
extern i32 g_ReplayUsesArchive;
// Target 0x004C6E70 is the zero-initialized load barrier consumed by this
// task's loading loop.
DIFFABLE_STATIC(i32, g_PhotoLoadWaitFlag);
#ifndef DIFFBUILD
#define g_PhotoGameRuntime \
    TH095_RUNTIME_GLOBAL_PTR(PhotoGameRuntimeTaskView, g_RuntimePlayerOwner)
#define g_PhotoGameTask \
    TH095_RUNTIME_GLOBAL_PTR(PhotoGameTaskView, g_RuntimeGlobalStateOwner)
#endif

PhotoGameTaskView::PhotoGameTaskView()
{
    this->runtimeConfig.Initialize();
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
        ((RuntimeInputCurrent() & TH_BUTTON_DEMO_INTERRUPT) != 0 ||
         ((this->flags >> 4) & 1) != 0 ||
         this->playerDeathTransitionComplete != 0 ||
         this->photoLimitTransitionComplete != 0))
    {
        g_PhotoNextState = 2;
    }

    if (((this->flags >> 4) & 1) != 0)
    {
        if (this->photoLimitTransitionComplete != 0)
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

    if (this->playerDeathTransitionComplete != 0)
    {
        return 3;
    }

    if (this->photoLimitTransitionComplete != 0)
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
            PhotoEnemyManagerView::RestartPhotoTargetEcls(
                reinterpret_cast<PhotoEnemyManagerView *>(
                    g_PhotoEnemyManagerTask));
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
    locals.task->flags = locals.task->flags | PHOTO_GAME_TASK_GAMEPLAY_LOAD_ACTIVE;

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
    task->flags = task->flags | PHOTO_GAME_TASK_GAMEPLAY_LOAD_ACTIVE;

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

    if (g_Supervisor.flags.resultRestartActive == 0)
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
    task->flags = task->flags & ~PHOTO_GAME_TASK_GAMEPLAY_LOAD_ACTIVE;
    g_Supervisor.flags.resultRestartActive = 0;
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

    this->runtimeConfig = g_Supervisor.config;
    this->replay = ReplayManager::Create(
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

    if (!FileSystem::CheckIfFileAlreadyExists(bestShotPath))
    {
        g_ResultSaveData->scoreEntries[this->bestShotIndex].captureTime = 0;
        g_ResultSaveData->scoreEntries[this->bestShotIndex].detailScore = 0;
    }

    this->background = Background::Create();
    if (this->background == NULL)
    {
        return ZUN_ERROR;
    }
    this->front = PhotoFrontManagerView::Create();
    if (this->front == NULL)
    {
        return ZUN_ERROR;
    }
    this->bullets = PhotoBulletManagerView::Create();
    if (this->bullets == NULL)
    {
        return ZUN_ERROR;
    }
    this->photoOverlay = PhotoOverlayManagerView::Create();
    if (this->photoOverlay == NULL)
    {
        return ZUN_ERROR;
    }
    this->player = PhotoGameUpdateView::Create();
    if (this->player == NULL)
    {
        return ZUN_ERROR;
    }
    this->enemies = PhotoEnemyManagerTaskView::Create();
    if (this->enemies == NULL)
    {
        return ZUN_ERROR;
    }
    this->items = PhotoItemManagerView::Create();
    if (this->items == NULL)
    {
        return ZUN_ERROR;
    }
    this->pause = ResultScreen::Create();
    if (this->pause == NULL)
    {
        return ZUN_ERROR;
    }
    this->lasers = PhotoEffectManagerView::Create();
    if (this->lasers == NULL)
    {
        return ZUN_ERROR;
    }

    if (g_Supervisor.flags.resultRestartActive == 0 &&
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
    delete this->background;
    this->front->Destroy();
    this->bullets->Destroy();
    this->player->Destroy();
    ReplayManager::Destroy(this->replay);
    reinterpret_cast<PhotoEnemyManagerView *>(this->enemies)->Destroy();
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

    if (g_Supervisor.flags.resultRestartActive == 0 &&
        g_ReplayUsesArchive == 0)
    {
        g_Supervisor.StopAudio();
    }
    if (g_Supervisor.flags.resultRestartActive != 0)
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
