#include "FrontEndController.hpp"
#ifdef TH095_MATCH_EXACT
#include "AnmManager.hpp"
#endif
#include "SceneSelect.hpp"
#include "FrontEndGlobals.hpp"
#include "ReplayBrowser.hpp"
#include "OptionsMenu.hpp"
#include "MusicRoom.hpp"
#include "HelpMenu.hpp"
#include "SoundPlayer.hpp"
#include "ZunMath.hpp"
#include "GameplayGlobals.hpp"
#ifndef DIFFBUILD
#include "InputRuntime.hpp"
#endif
#include "FileSystem.hpp"
#include "PhotoGameTask.hpp"

#include <d3d8.h>
#include <stdio.h>
#include <stdlib.h>

namespace th095
{

#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
#define TH095_MAIN_MENU_STATE_INITIALIZE 0
#define TH095_MAIN_MENU_STATE_ACTIVE 1
#define TH095_SCENE_SELECT_STATE_INITIALIZE 0
#else
enum FrontEndMainMenuStateValue
{
    FRONT_END_MAIN_MENU_STATE_INITIALIZE = 0,
    FRONT_END_MAIN_MENU_STATE_ACTIVE = 1,
};
#define TH095_MAIN_MENU_STATE_INITIALIZE FRONT_END_MAIN_MENU_STATE_INITIALIZE
#define TH095_MAIN_MENU_STATE_ACTIVE FRONT_END_MAIN_MENU_STATE_ACTIVE
#define TH095_SCENE_SELECT_STATE_INITIALIZE SCENE_SELECT_STATE_INITIALIZE
#endif

typedef ZunTimer FrontEndControllerTimer;

#ifdef TH095_MATCH_EXACT
#define CreateVmAtScreen CreateVm
#endif

struct FrontEndControllerUpdateView
{
    SceneAnmLoadedView *sceneAnm;
    AnmLoaded *transitionAnm;
    FrontEndControllerTimer stateTimer;
    FrontEndControllerTimer animationTimer;
    ResultScreenReplayCursor cursor;
    ResultScreenReplayCursor secondaryCursor;
    u8 unknown01d0[0xa24];
    SceneAnmVmIdArray vmIds;
    u8 unknown0e88[0x5278];
    AnmVmId transitionVm;
    u8 unknown6104[4];
    i32 transitionReady;
    i32 state;
    FrontEndRequestedState requestedState;
    u8 unknown6114[0x0c];
    union
    {
        u32 flags;
        FrontEndControllerFlagBits flagBits;
    };
    i32 entryMode;
};

struct FrontEndUpdateLocals
{
    AnmVm *second69;
    AnmVm *first68;
    AnmVm *second67;
    AnmVm *first66;
    i32 replayInterruptIndex;
    i32 gameInterruptIndex;
    u8 *rgb16Pixel;
    i32 rgb16X;
    i32 rgb16Y;
    u8 *argb32Pixel;
    i32 argb32X;
    i32 argb32Y;
    D3DLOCKED_RECT lockedRect;
    IDirect3DSurface8 *surface;
};

struct MainMenuVmPositions
{
    Float3 position11;
    Float3 position10;
    Float3 position9;
    Float3 position8;
    Float3 position7;
    Float3 position6;
    Float3 position17;
    Float3 position16;
    Float3 position15;
    Float3 position14;
    Float3 position13;
    Float3 position12;
    Float3 position5;
    Float3 position4;
    Float3 position3;
    Float3 position2;
    Float3 position1;
    Float3 position0;
};

typedef char FrontEndUpdateCursorAt20[
    (offsetof(FrontEndControllerUpdateView, cursor) == 0x20) ? 1 : -1];
typedef char FrontEndUpdateSecondaryCursorAtF8[
    (offsetof(FrontEndControllerUpdateView, secondaryCursor) == 0xf8) ? 1 : -1];
typedef char FrontEndUpdateVmIdsAtBF4[
    (offsetof(FrontEndControllerUpdateView, vmIds) == 0xbf4) ? 1 : -1];
typedef char FrontEndUpdateTransitionVmAt6100[
    (offsetof(FrontEndControllerUpdateView, transitionVm) == 0x6100)
        ? 1
        : -1];
typedef char FrontEndUpdateStateAt610C[
    (offsetof(FrontEndControllerUpdateView, state) == 0x610c) ? 1 : -1];
typedef char FrontEndUpdateRequestedStateAt6110[
    (offsetof(FrontEndControllerUpdateView, requestedState) == 0x6110)
        ? 1
        : -1];
typedef char FrontEndUpdateFlagsAt6120[
    (offsetof(FrontEndControllerUpdateView, flags) == 0x6120) ? 1 : -1];
typedef char FrontEndUpdateEntryModeAt6124[
    (offsetof(FrontEndControllerUpdateView, entryMode) == 0x6124) ? 1 : -1];
typedef char FrontEndUpdateLocalsSizeIs3C[
    (sizeof(FrontEndUpdateLocals) == 0x3c) ? 1 : -1];

#ifdef TH095_MATCH_EXACT
#define TH095_FRONT_ANM_MANAGER g_SceneAnmManager
#else
#define TH095_FRONT_ANM_MANAGER g_AnmManager
#endif

DIFFABLE_STATIC(i32, g_MainMenuDemoWaitFrames); // 0x004CA2FC
extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;
extern u16 g_FrontEndCurrentInput;
#ifndef DIFFBUILD
#define g_ResultMenuInput (RuntimeResultMenuInput())
#define g_PressedButtons (RuntimePressedButtons())
#define g_FrontEndCurrentInput (RuntimeInputCurrent())
#endif
DIFFABLE_STATIC(i32, g_DemoReplayIndex);

static __forceinline u16 FrontEndInputAnd(u16 input, u16 mask)
{
    return input & mask;
}

static __forceinline u16 FrontEndUpInputMask()
{
    return TH_BUTTON_UP;
}

static __forceinline u16 FrontEndDownInputMask()
{
    return TH_BUTTON_DOWN;
}

#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
#define TH095_FRONT_DEMO_INTERRUPT_MASK 0x160b
#else
#define TH095_FRONT_DEMO_INTERRUPT_MASK TH_BUTTON_DEMO_INTERRUPT
#endif

static __forceinline i32 PrimaryWorkerActiveSnapshot()
{
    i32 active;
    ReadWorkerSignal(active, g_Supervisor.replayScanWorker.active);
    return active;
}

static __forceinline void FrontEndFreePoppedValue(void *block)
{
    free(block);
}

static __forceinline void FrontEndResetTimer(FrontEndControllerTimer *timer)
{
    timer->current = 0;
    timer->subFrame = 0.0f;
    timer->previous = -999999;
}

static __forceinline void FrontEndDrainQueueValue(ScenePreviewDataQueue *queue)
{
    u32 compilerStorage;
    FrontEndFreePoppedValue(reinterpret_cast<void *>(queue->Pop()));
}

static __forceinline void FrontEndCreateSceneVm(
    FrontEndControllerUpdateView *view,
    i32 scriptIndex)
{
    view->vmIds[scriptIndex] =
        view->sceneAnm->CreateVm(scriptIndex, 7);
}

#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
#define TH095_MUSIC_ROOM_STATE_INITIALIZE 0
#define TH095_REPLAY_BROWSER_STATE_INITIALIZE 0
#define TH095_OPTIONS_MENU_STATE_INITIALIZE 0
#else
#define TH095_MUSIC_ROOM_STATE_INITIALIZE MUSIC_ROOM_STATE_INITIALIZE
#define TH095_REPLAY_BROWSER_STATE_INITIALIZE REPLAY_BROWSER_STATE_INITIALIZE
#define TH095_OPTIONS_MENU_STATE_INITIALIZE OPTIONS_MENU_STATE_INITIALIZE
#endif

ChainCallbackResult SceneSelectControllerView::Update()
{
#define view (reinterpret_cast<FrontEndControllerUpdateView *>(this))
    FrontEndUpdateLocals locals;

    switch (view->requestedState)
    {
    case FRONT_END_REQUESTED_STATE_INITIALIZE:
    {
        if (view->flagBits.titleLoadFailed)
        {
            g_Supervisor.StopReplayScan();
            g_Supervisor.requestedSceneState = SUPERVISOR_STATE_ERROR;
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        if (PrimaryWorkerActiveSnapshot() != 0)
        {
            return CHAIN_CALLBACK_RESULT_BREAK;
        }

        g_Supervisor.StopReplayScan();
        locals.surface = NULL;
        view->sceneAnm->textures[1].texture->GetSurfaceLevel(0, &locals.surface);
        locals.surface->LockRect(&locals.lockedRect, NULL, 0);
        if (view->sceneAnm->textures[1].bytesPerPixel == 4)
        {
            for (locals.argb32Y = 0; locals.argb32Y < 192;
                 locals.argb32Y++)
            {
                locals.argb32Pixel =
                    reinterpret_cast<u8 *>(locals.lockedRect.pBits) +
                    locals.argb32Y * locals.lockedRect.Pitch;
                for (locals.argb32X = 0; locals.argb32X < 256;
                     locals.argb32X++)
                {
                    locals.argb32Pixel[0] = 0;
                    locals.argb32Pixel[1] = 0;
                    locals.argb32Pixel[2] = 0;
                    locals.argb32Pixel[3] = 0xff;
                    locals.argb32Pixel += 4;
                }
            }
        }
        else
        {
            for (locals.rgb16Y = 0; locals.rgb16Y < 192;
                 locals.rgb16Y++)
            {
                locals.rgb16Pixel =
                    reinterpret_cast<u8 *>(locals.lockedRect.pBits) +
                    locals.rgb16Y * locals.lockedRect.Pitch;
                for (locals.rgb16X = 0; locals.rgb16X < 256;
                     locals.rgb16X++)
                {
                    locals.rgb16Pixel[0] = 0;
                    locals.rgb16Pixel[1] = 0xf0;
                    locals.rgb16Pixel += 2;
                }
            }
        }
        locals.surface->UnlockRect();
        locals.surface->Release();
        g_MainMenuDemoWaitFrames = 0;

        if (g_ReplayUsesArchive == REPLAY_PLAYBACK_SOURCE_LOOSE_FILE)
        {
            g_Supervisor.LoadMusic(0, "bgm/th09_00.wav");
            g_Supervisor.PlayMusic(0, 0);
        }
        else
        {
            view->entryMode = FRONT_END_ENTRY_MAIN_MENU;
            g_ReplayUsesArchive = REPLAY_PLAYBACK_SOURCE_LOOSE_FILE;
        }

        switch (view->entryMode)
        {
        case FRONT_END_ENTRY_MAIN_MENU:
        {
            view->requestedState = FRONT_END_REQUESTED_STATE_MAIN_MENU;
            FrontEndResetTimer(&view->stateTimer);
            view->state = TH095_MAIN_MENU_STATE_INITIALIZE;
            FrontEndCreateSceneVm(view, 0x66);
            FrontEndCreateSceneVm(view, 0x67);
            FrontEndCreateSceneVm(view, 0x19);
            FrontEndCreateSceneVm(view, 0x1a);
            view->transitionVm = view->transitionAnm->CreateVm(0, 7);
            FrontEndCreateSceneVm(view, 0x1b);
            FrontEndCreateSceneVm(view, 0x64);
            FrontEndCreateSceneVm(view, 0x65);
            break;
        }
        case FRONT_END_ENTRY_SCENE_SELECT:
        {
            view->requestedState = FRONT_END_REQUESTED_STATE_SCENE_SELECT;
            view->state = TH095_SCENE_SELECT_STATE_INITIALIZE;
            FrontEndResetTimer(&view->stateTimer);
            FrontEndCreateSceneVm(view, 0x19);
            FrontEndCreateSceneVm(view, 0x1a);
            view->transitionVm = view->transitionAnm->CreateVm(0, 7);
            FrontEndCreateSceneVm(view, 0x1b);
            FrontEndCreateSceneVm(view, 0x64);
            FrontEndCreateSceneVm(view, 0x65);
            view->entryMode = FRONT_END_ENTRY_MAIN_MENU;
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        case FRONT_END_ENTRY_REPLAY_BROWSER:
        {
            view->requestedState = FRONT_END_REQUESTED_STATE_REPLAY_BROWSER;
            view->state = TH095_REPLAY_BROWSER_STATE_INITIALIZE;
            FrontEndResetTimer(&view->stateTimer);
            view->cursor.Set(1);
            FrontEndCreateSceneVm(view, 0x19);
            FrontEndCreateSceneVm(view, 0x1a);
            view->transitionVm = view->transitionAnm->CreateVm(0, 7);
            FrontEndCreateSceneVm(view, 0x1b);
            FrontEndCreateSceneVm(view, 0x64);
            FrontEndCreateSceneVm(view, 0x65);
            view->entryMode = FRONT_END_ENTRY_MAIN_MENU;
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        default:
            break;
        }
    }
    case FRONT_END_REQUESTED_STATE_MAIN_MENU:
        this->UpdateMainMenu();
        break;

    case FRONT_END_REQUESTED_STATE_SCENE_SELECT:
        this->UpdateSceneSelect();
        break;

    case FRONT_END_REQUESTED_STATE_REPLAY_BROWSER:
        reinterpret_cast<ReplayBrowserView *>(this)->Update();
        break;

    case FRONT_END_REQUESTED_STATE_OPTIONS:
        if (reinterpret_cast<OptionsMenuView *>(this)->Update() ==
            CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR)
        {
            return CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR;
        }
        break;

    case FRONT_END_REQUESTED_STATE_MUSIC_ROOM:
        reinterpret_cast<MusicRoomView *>(this)->UpdateMusicRoom();
        break;

    case FRONT_END_REQUESTED_STATE_HELP:
        reinterpret_cast<HelpMenuView *>(this)->UpdateHelpMenu();
        break;

    case FRONT_END_REQUESTED_STATE_START_GAME:
        if ((view->stateTimer.current == 1) != 0)
        {
            g_Supervisor.FadeOutMusic(2.0f);
            if (PrimaryWorkerActiveSnapshot() != 0)
            {
                return CHAIN_CALLBACK_RESULT_CONTINUE;
            }
            g_Supervisor.StopReplayScan();
            g_Supervisor.photoGameTask =
                PhotoGameTaskView::Create(REPLAY_MANAGER_RECORD);
            if (g_Supervisor.photoGameTask == NULL)
            {
                g_Supervisor.requestedSceneState = SUPERVISOR_STATE_EXIT;
            }
        }
        if ((view->stateTimer.current < 40) != 0)
        {
            break;
        }
        if (view->transitionReady != 0)
        {
            if (g_RuntimeGlobalStateOwner->gameplayLoadFailed)
            {
                g_Supervisor.requestedSceneState = SUPERVISOR_STATE_EXIT;
                break;
            }
            for (locals.gameInterruptIndex = 0;
                 locals.gameInterruptIndex < 0x9a;
                 locals.gameInterruptIndex++)
            {
                TH095_FRONT_ANM_MANAGER->SetInterrupt(
                    view->vmIds[locals.gameInterruptIndex], 1);
            }
            view->transitionVm.SetInterrupt(1);
            g_Supervisor.requestedSceneState = SUPERVISOR_STATE_PHOTO_GAME;
            if (g_ReplayUsesArchive == REPLAY_PLAYBACK_SOURCE_LOOSE_FILE)
            {
                g_Supervisor.PlayMusic(0, 0);
            }
            break;
        }
        break;

    case FRONT_END_REQUESTED_STATE_START_REPLAY:
        if ((view->stateTimer.current == 1) != 0)
        {
            if (g_ReplayUsesArchive == REPLAY_PLAYBACK_SOURCE_LOOSE_FILE)
            {
                g_Supervisor.FadeOutMusic(2.0f);
            }
            if (PrimaryWorkerActiveSnapshot() != 0)
            {
                return CHAIN_CALLBACK_RESULT_CONTINUE;
            }
            g_Supervisor.StopReplayScan();
            g_Supervisor.photoGameTask =
                PhotoGameTaskView::Create(REPLAY_MANAGER_PLAYBACK);
            if (g_Supervisor.photoGameTask == NULL)
            {
                g_Supervisor.requestedSceneState = SUPERVISOR_STATE_EXIT;
            }
        }
        if ((view->stateTimer.current < 40) != 0)
        {
            break;
        }
        if (view->transitionReady != 0)
        {
            if (g_RuntimeGlobalStateOwner->gameplayLoadFailed)
            {
                g_Supervisor.requestedSceneState = SUPERVISOR_STATE_EXIT;
                break;
            }
            for (locals.replayInterruptIndex = 0;
                 locals.replayInterruptIndex < 0x9a;
                 locals.replayInterruptIndex++)
            {
                TH095_FRONT_ANM_MANAGER->SetInterrupt(
                    view->vmIds[locals.replayInterruptIndex], 1);
            }
            view->transitionVm.SetInterrupt(1);
            g_Supervisor.requestedSceneState = SUPERVISOR_STATE_START_REPLAY;
            if (g_ReplayUsesArchive == REPLAY_PLAYBACK_SOURCE_LOOSE_FILE)
            {
                g_Supervisor.PlayMusic(0, 0);
            }
            break;
        }
        break;

    case FRONT_END_REQUESTED_STATE_EXIT:
        if (PrimaryWorkerActiveSnapshot() != 0)
        {
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        g_Supervisor.StopReplayScan();
        g_Supervisor.requestedSceneState = SUPERVISOR_STATE_EXIT;
        break;
    }

    // The controller overlays these VM ids as POD storage; keep the exact
    // AnmVmId::GetVm ABI while naming their canonical array ownership.
    locals.first66 =
        reinterpret_cast<AnmVmId *>(&view->vmIds.values[0x66])->GetVm();
    locals.second67 =
        reinterpret_cast<AnmVmId *>(&view->vmIds.values[0x67])->GetVm();
    if (locals.first66 != NULL && locals.second67 != NULL)
    {
        locals.second67->position = locals.first66->position;
        locals.second67->position.x += 512.0f;
        locals.second67->color1.color = locals.first66->color1.color;
    }

    locals.first68 =
        reinterpret_cast<AnmVmId *>(&view->vmIds.values[0x68])->GetVm();
    locals.second69 =
        reinterpret_cast<AnmVmId *>(&view->vmIds.values[0x69])->GetVm();
    if (locals.first68 != NULL && locals.second69 != NULL)
    {
        locals.second69->position = locals.first68->position;
        locals.second69->position.x += 512.0f;
        locals.second69->color1.color = locals.first68->color1.color;
    }

    if (view->animationTimer.current % 5 == 0)
    {
        view->sceneAnm->CreateVm(0x1c, 0);
    }
    view->stateTimer.Tick();
    view->animationTimer.Tick();
#undef view
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

void SceneSelectControllerView::CloseMainMenu()
{
    this->vmIds.SetInterrupt(0, 1);
    this->vmIds.SetInterrupt(1, 1);
    this->vmIds.SetInterrupt(2, 1);
    this->vmIds.SetInterrupt(3, 1);
    this->vmIds.SetInterrupt(5, 1);
    this->vmIds.SetInterrupt(6, 1);
    this->vmIds.SetInterrupt(7, 1);
    this->vmIds.SetInterrupt(8, 1);
    this->vmIds.SetInterrupt(9, 1);
    this->vmIds.SetInterrupt(11, 1);
    this->vmIds.SetInterrupt(12, 1);
    this->vmIds.SetInterrupt(13, 1);
    this->vmIds.SetInterrupt(14, 1);
    this->vmIds.SetInterrupt(15, 1);
    this->vmIds.SetInterrupt(17, 1);
    this->vmIds.SetInterrupt(102, 1);
    this->vmIds.SetInterrupt(103, 1);
    this->vmIds.SetInterrupt(4, 1);
    this->vmIds.SetInterrupt(10, 1);
    this->vmIds.SetInterrupt(16, 1);
}

void SceneSelectControllerView::UpdateMainMenuSelection()
{
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[0], (this->GetSelectedGroup() != 0) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[6], (this->GetSelectedGroup() != 0) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[1], (this->GetSelectedGroup() != 1) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[7], (this->GetSelectedGroup() != 1) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[2], (this->GetSelectedGroup() != 2) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[8], (this->GetSelectedGroup() != 2) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[3], (this->GetSelectedGroup() != 3) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[9], (this->GetSelectedGroup() != 3) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[4], (this->GetSelectedGroup() != 4) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[10], (this->GetSelectedGroup() != 4) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[5], (this->GetSelectedGroup() != 5) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[11], (this->GetSelectedGroup() != 5) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[12], (this->GetSelectedGroup() != 0) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[13], (this->GetSelectedGroup() != 1) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[14], (this->GetSelectedGroup() != 2) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[15], (this->GetSelectedGroup() != 3) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[16], (this->GetSelectedGroup() != 4) + 2);
    TH095_FRONT_ANM_MANAGER->SetInterrupt(
        this->vmIds.values[17], (this->GetSelectedGroup() != 5) + 2);
}

ChainCallbackResult SceneSelectControllerView::UpdateMainMenu()
{
#define view (reinterpret_cast<FrontEndControllerUpdateView *>(this))
    MainMenuVmPositions positions;

    switch (view->state)
    {
    case TH095_MAIN_MENU_STATE_INITIALIZE:
    {
        view->cursor.count = 6;
        view->cursor.wraps = 1;
        view->state = TH095_MAIN_MENU_STATE_ACTIVE;

#define CREATE_MAIN_MENU_VM(position, index, yValue)                           \
    positions.position.x = 64.0f;                                             \
    positions.position.y = yValue;                                            \
    positions.position.z = 0.0f;                                              \
    view->vmIds[index] =                                                      \
        view->sceneAnm->CreateVmAtScreen(index, &positions.position)
        CREATE_MAIN_MENU_VM(position0, 0, 130.0f);
        CREATE_MAIN_MENU_VM(position1, 1, 168.0f);
        CREATE_MAIN_MENU_VM(position2, 2, 206.0f);
        CREATE_MAIN_MENU_VM(position3, 3, 244.0f);
        CREATE_MAIN_MENU_VM(position4, 4, 282.0f);
        CREATE_MAIN_MENU_VM(position5, 5, 320.0f);
        CREATE_MAIN_MENU_VM(position12, 12, 130.0f);
        CREATE_MAIN_MENU_VM(position13, 13, 168.0f);
        CREATE_MAIN_MENU_VM(position14, 14, 206.0f);
        CREATE_MAIN_MENU_VM(position15, 15, 244.0f);
        CREATE_MAIN_MENU_VM(position16, 16, 282.0f);
        CREATE_MAIN_MENU_VM(position17, 17, 320.0f);
        CREATE_MAIN_MENU_VM(position6, 6, 130.0f);
        CREATE_MAIN_MENU_VM(position7, 7, 168.0f);
        CREATE_MAIN_MENU_VM(position8, 8, 206.0f);
        CREATE_MAIN_MENU_VM(position9, 9, 244.0f);
        CREATE_MAIN_MENU_VM(position10, 10, 282.0f);
        CREATE_MAIN_MENU_VM(position11, 11, 320.0f);
#undef CREATE_MAIN_MENU_VM
        FrontEndResetTimer(&view->stateTimer);
    }
    case TH095_MAIN_MENU_STATE_ACTIVE:
    if ((view->stateTimer.current < 30) != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if ((view->stateTimer.current == 30) != 0)
    {
        this->UpdateMainMenuSelection();
    }

    view->cursor.SaveCurrent();
    if ((u16)(FrontEndInputAnd(g_PressedButtons, TH_BUTTON_UP) != 0 ||
              (g_ResultMenuInput & FrontEndUpInputMask()) != 0) != 0)
    {
        view->cursor.Move(-1);
    }
    if ((u16)(FrontEndInputAnd(g_PressedButtons, TH_BUTTON_DOWN) != 0 ||
              (g_ResultMenuInput & FrontEndDownInputMask()) != 0) != 0)
    {
        view->cursor.Move(1);
    }
    if (view->cursor.HasChanged())
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        this->UpdateMainMenuSelection();
    }

    if (FrontEndInputAnd(g_FrontEndCurrentInput, TH095_FRONT_DEMO_INTERRUPT_MASK) != 0)
    {
        g_MainMenuDemoWaitFrames = 0;
    }
    else
    {
        g_MainMenuDemoWaitFrames++;
        if (g_MainMenuDemoWaitFrames >= 1800)
        {
            g_MainMenuDemoWaitFrames = 0;
            g_ReplayUsesArchive = REPLAY_PLAYBACK_SOURCE_ARCHIVE;
            sprintf(g_SelectedReplayPath, "demo/demo%d.rpy", g_DemoReplayIndex);
            g_DemoReplayIndex++;
            g_DemoReplayIndex %= 3;
            view->requestedState = FRONT_END_REQUESTED_STATE_START_REPLAY;
            FrontEndResetTimer(&view->stateTimer);
            view->state = 0;
            break;
        }
    }

    if (FrontEndInputAnd(g_PressedButtons, 0x1002) != 0)
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
        switch (view->cursor.GetCurrent())
        {
        case 0:
        {
            this->CloseMainMenu();
            view->requestedState = FRONT_END_REQUESTED_STATE_SCENE_SELECT;
            view->state = TH095_SCENE_SELECT_STATE_INITIALIZE;
            FrontEndResetTimer(&view->stateTimer);
            while (PrimaryWorkerActiveSnapshot() != 0)
            {
                Sleep(1);
            }
            while (this->groupPreviewDataQueue.Size() != 0)
            {
                FrontEndDrainQueueValue(&this->groupPreviewDataQueue);
            }
            while (this->scenePreviewDataQueue.Size() != 0)
            {
                FrontEndDrainQueueValue(&this->scenePreviewDataQueue);
            }
            WriteSceneQueueWord32(this->groupPreviewSizeQueue.words[SCENE_QUEUE_COUNT_WORD].value, 0);
            WriteSceneQueueWord32(this->scenePreviewSizeQueue.words[SCENE_QUEUE_COUNT_WORD].value, 0);
            WriteSceneQueueWord32(this->groupPreviewQueue.words[SCENE_QUEUE_COUNT_WORD].value, 0);
            WriteSceneQueueWord32(this->scenePreviewQueue.words[SCENE_QUEUE_COUNT_WORD].value, 0);
            WriteSceneQueueWord32(this->loadedGroupQueue.words[SCENE_QUEUE_COUNT_WORD].value, 0);
            WriteSceneQueueWord32(this->selectionQueue.words[SCENE_QUEUE_COUNT_WORD].value, 0);
            WriteSceneQueueWord32(this->loadedSceneQueue.words[SCENE_QUEUE_COUNT_WORD].value, 0);
            this->pendingTextures.count = 0;
            this->stateHistory.count = 0;
            this->currentDisplayState = -1;
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        case 1:
            this->CloseMainMenu();
            view->requestedState = FRONT_END_REQUESTED_STATE_REPLAY_BROWSER;
            view->state = TH095_REPLAY_BROWSER_STATE_INITIALIZE;
            FrontEndResetTimer(&view->stateTimer);
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        case 3:
            this->CloseMainMenu();
            view->requestedState = FRONT_END_REQUESTED_STATE_OPTIONS;
            view->state = TH095_OPTIONS_MENU_STATE_INITIALIZE;
            FrontEndResetTimer(&view->stateTimer);
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        case 2:
            // The target emits the state-7 block before state 8, while its
            // adjacent switch table routes menu row 2 to state 8 and row 3
            // to state 7.  Keeping the cases in this source order preserves
            // both the target block chronology and the real menu semantics;
            // ordering these labels numerically makes a relocation-normalized
            // COFF comparison look exact while swapping Music Room/Options in
            // the fully linked executable.
            this->CloseMainMenu();
            view->requestedState = FRONT_END_REQUESTED_STATE_MUSIC_ROOM;
            view->state = TH095_MUSIC_ROOM_STATE_INITIALIZE;
            FrontEndResetTimer(&view->stateTimer);
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        case 4:
            this->CloseMainMenu();
            view->requestedState = FRONT_END_REQUESTED_STATE_HELP;
            view->state = TH095_HELP_STATE_INITIALIZE;
            FrontEndResetTimer(&view->stateTimer);
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        case 5:
        exitMainMenu:
            view->requestedState = FRONT_END_REQUESTED_STATE_EXIT;
            view->state = 0;
            FrontEndResetTimer(&view->stateTimer);
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
    }

    if (FrontEndInputAnd(g_PressedButtons, 9) != 0)
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
        if (view->cursor.GetCurrent() == 5)
        {
            goto exitMainMenu;
        }
        view->cursor.Set(5);
        this->UpdateMainMenuSelection();
    }
    break;
    default:
        break;
    }
#undef view
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

} // namespace th095
