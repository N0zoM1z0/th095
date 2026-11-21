#include "AnmManager.hpp"
#include "FrontEndController.hpp"
#include "SceneSelect.hpp"
#include "FrontEndGlobals.hpp"
#include "ReplayBrowser.hpp"
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

#ifdef TH095_MATCH_EXACT
#define CreateVmAtScreen CreateVm
#endif

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

static __forceinline void FrontEndDrainQueueValue(ScenePreviewDataQueue *queue)
{
    u32 compilerStorage;
    FrontEndFreePoppedValue(reinterpret_cast<void *>(queue->Pop()));
}

static __forceinline void FrontEndCreateSceneVm(
    SceneSelectControllerView *controller,
    i32 scriptIndex)
{
    controller->vmIds[scriptIndex] =
        controller->sceneAnm->CreateVm(scriptIndex, 7);
}

ChainCallbackResult FrontEndControllerView::Update()
{
// Scene Select still supplies the VM overlay. Expand the cast at each use so
// VC7.1 keeps the existing frame and accesses the fields through this.
#define sceneMenu (reinterpret_cast<SceneSelectControllerView *>(this))
    FrontEndUpdateLocals locals;

    switch (this->requestedState)
    {
    case FRONT_END_REQUESTED_STATE_INITIALIZE:
    {
        if (this->flagBits.titleLoadFailed)
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
        this->sceneAnm->textures[1].texture->GetSurfaceLevel(0, &locals.surface);
        locals.surface->LockRect(&locals.lockedRect, NULL, 0);
        if (this->sceneAnm->textures[1].bytesPerPixel == 4)
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
            this->entryMode = FRONT_END_ENTRY_MAIN_MENU;
            g_ReplayUsesArchive = REPLAY_PLAYBACK_SOURCE_LOOSE_FILE;
        }

        switch (this->entryMode)
        {
        case FRONT_END_ENTRY_MAIN_MENU:
        {
            this->requestedState = FRONT_END_REQUESTED_STATE_MAIN_MENU;
            FrontEndResetTimer(&this->stateTimer);
            this->state = FRONT_END_MAIN_MENU_STATE_INITIALIZE;
            FrontEndCreateSceneVm(sceneMenu, 0x66);
            FrontEndCreateSceneVm(sceneMenu, 0x67);
            FrontEndCreateSceneVm(sceneMenu, 0x19);
            FrontEndCreateSceneVm(sceneMenu, 0x1a);
            sceneMenu->transitionVm = this->transitionAnm->CreateVm(0, 7);
            FrontEndCreateSceneVm(sceneMenu, 0x1b);
            FrontEndCreateSceneVm(sceneMenu, 0x64);
            FrontEndCreateSceneVm(sceneMenu, 0x65);
            break;
        }
        case FRONT_END_ENTRY_SCENE_SELECT:
        {
            this->requestedState = FRONT_END_REQUESTED_STATE_SCENE_SELECT;
            this->state = SCENE_SELECT_STATE_INITIALIZE;
            FrontEndResetTimer(&this->stateTimer);
            FrontEndCreateSceneVm(sceneMenu, 0x19);
            FrontEndCreateSceneVm(sceneMenu, 0x1a);
            sceneMenu->transitionVm = this->transitionAnm->CreateVm(0, 7);
            FrontEndCreateSceneVm(sceneMenu, 0x1b);
            FrontEndCreateSceneVm(sceneMenu, 0x64);
            FrontEndCreateSceneVm(sceneMenu, 0x65);
            this->entryMode = FRONT_END_ENTRY_MAIN_MENU;
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        case FRONT_END_ENTRY_REPLAY_BROWSER:
        {
            this->requestedState = FRONT_END_REQUESTED_STATE_REPLAY_BROWSER;
            this->state = REPLAY_BROWSER_STATE_INITIALIZE;
            FrontEndResetTimer(&this->stateTimer);
            this->groupCursor.Set(1);
            FrontEndCreateSceneVm(sceneMenu, 0x19);
            FrontEndCreateSceneVm(sceneMenu, 0x1a);
            sceneMenu->transitionVm = this->transitionAnm->CreateVm(0, 7);
            FrontEndCreateSceneVm(sceneMenu, 0x1b);
            FrontEndCreateSceneVm(sceneMenu, 0x64);
            FrontEndCreateSceneVm(sceneMenu, 0x65);
            this->entryMode = FRONT_END_ENTRY_MAIN_MENU;
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
        this->UpdateReplayBrowser();
        break;

    case FRONT_END_REQUESTED_STATE_OPTIONS:
        if (this->UpdateOptionsMenu() ==
            CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR)
        {
            return CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR;
        }
        break;

    case FRONT_END_REQUESTED_STATE_MUSIC_ROOM:
        this->UpdateMusicRoom();
        break;

    case FRONT_END_REQUESTED_STATE_HELP:
        this->UpdateHelpMenu();
        break;

    case FRONT_END_REQUESTED_STATE_START_GAME:
        if ((this->stateTimer.current == 1) != 0)
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
        if ((this->stateTimer.current < 40) != 0)
        {
            break;
        }
        if (this->transitionReady != 0)
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
                    sceneMenu->vmIds[locals.gameInterruptIndex], 1);
            }
            sceneMenu->transitionVm.SetInterrupt(1);
            g_Supervisor.requestedSceneState = SUPERVISOR_STATE_PHOTO_GAME;
            if (g_ReplayUsesArchive == REPLAY_PLAYBACK_SOURCE_LOOSE_FILE)
            {
                g_Supervisor.PlayMusic(0, 0);
            }
            break;
        }
        break;

    case FRONT_END_REQUESTED_STATE_START_REPLAY:
        if ((this->stateTimer.current == 1) != 0)
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
        if ((this->stateTimer.current < 40) != 0)
        {
            break;
        }
        if (this->transitionReady != 0)
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
                    sceneMenu->vmIds[locals.replayInterruptIndex], 1);
            }
            sceneMenu->transitionVm.SetInterrupt(1);
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
        reinterpret_cast<AnmVmId *>(&sceneMenu->vmIds.values[0x66])->GetVm();
    locals.second67 =
        reinterpret_cast<AnmVmId *>(&sceneMenu->vmIds.values[0x67])->GetVm();
    if (locals.first66 != NULL && locals.second67 != NULL)
    {
        locals.second67->position = locals.first66->position;
        locals.second67->position.x += 512.0f;
        locals.second67->color1.color = locals.first66->color1.color;
    }

    locals.first68 =
        reinterpret_cast<AnmVmId *>(&sceneMenu->vmIds.values[0x68])->GetVm();
    locals.second69 =
        reinterpret_cast<AnmVmId *>(&sceneMenu->vmIds.values[0x69])->GetVm();
    if (locals.first68 != NULL && locals.second69 != NULL)
    {
        locals.second69->position = locals.first68->position;
        locals.second69->position.x += 512.0f;
        locals.second69->color1.color = locals.first68->color1.color;
    }

    if (this->animationTimer.current % 5 == 0)
    {
        this->sceneAnm->CreateVm(0x1c, 0);
    }
    this->stateTimer.Tick();
    this->animationTimer.Tick();
#undef sceneMenu
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

ChainCallbackResult FrontEndControllerView::UpdateMainMenu()
{
#define sceneMenu (reinterpret_cast<SceneSelectControllerView *>(this))
    MainMenuVmPositions positions;

    switch (this->state)
    {
    case FRONT_END_MAIN_MENU_STATE_INITIALIZE:
    {
        this->groupCursor.count = 6;
        this->groupCursor.wraps = 1;
        this->state = FRONT_END_MAIN_MENU_STATE_ACTIVE;

#define CREATE_MAIN_MENU_VM(position, index, yValue)                           \
    positions.position.x = 64.0f;                                             \
    positions.position.y = yValue;                                            \
    positions.position.z = 0.0f;                                              \
    sceneMenu->vmIds[index] =                                                      \
        sceneMenu->sceneAnm->CreateVmAtScreen(index, &positions.position)
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
        FrontEndResetTimer(&this->stateTimer);
    }
    case FRONT_END_MAIN_MENU_STATE_ACTIVE:
    if ((this->stateTimer.current < 30) != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if ((this->stateTimer.current == 30) != 0)
    {
        sceneMenu->UpdateMainMenuSelection();
    }

    this->groupCursor.SaveCurrent();
    if ((u16)(FrontEndInputAnd(g_PressedButtons, TH_BUTTON_UP) != 0 ||
              (g_ResultMenuInput & FrontEndUpInputMask()) != 0) != 0)
    {
        this->groupCursor.Move(-1);
    }
    if ((u16)(FrontEndInputAnd(g_PressedButtons, TH_BUTTON_DOWN) != 0 ||
              (g_ResultMenuInput & FrontEndDownInputMask()) != 0) != 0)
    {
        this->groupCursor.Move(1);
    }
    if (this->groupCursor.HasChanged())
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        sceneMenu->UpdateMainMenuSelection();
    }

    if (FrontEndInputAnd(g_FrontEndCurrentInput, TH_BUTTON_DEMO_INTERRUPT) != 0)
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
            this->requestedState = FRONT_END_REQUESTED_STATE_START_REPLAY;
            FrontEndResetTimer(&this->stateTimer);
            this->state = 0;
            break;
        }
    }

    if (FrontEndInputAnd(g_PressedButtons, TH_BUTTON_ENTER | TH_BUTTON_BOMB) != 0)
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
        switch (this->groupCursor.GetCurrent())
        {
        case 0:
        {
            sceneMenu->CloseMainMenu();
            this->requestedState = FRONT_END_REQUESTED_STATE_SCENE_SELECT;
            this->state = SCENE_SELECT_STATE_INITIALIZE;
            FrontEndResetTimer(&this->stateTimer);
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
            sceneMenu->CloseMainMenu();
            this->requestedState = FRONT_END_REQUESTED_STATE_REPLAY_BROWSER;
            this->state = REPLAY_BROWSER_STATE_INITIALIZE;
            FrontEndResetTimer(&this->stateTimer);
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        case 3:
            sceneMenu->CloseMainMenu();
            this->requestedState = FRONT_END_REQUESTED_STATE_OPTIONS;
            this->state = OPTIONS_MENU_STATE_INITIALIZE;
            FrontEndResetTimer(&this->stateTimer);
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        case 2:
            // The target emits the state-7 block before state 8, while its
            // adjacent switch table routes menu row 2 to state 8 and row 3
            // to state 7.  Keeping the cases in this source order preserves
            // both the target block chronology and the real menu semantics;
            // ordering these labels numerically makes a relocation-normalized
            // COFF comparison look exact while swapping Music Room/Options in
            // the fully linked executable.
            sceneMenu->CloseMainMenu();
            this->requestedState = FRONT_END_REQUESTED_STATE_MUSIC_ROOM;
            this->state = MUSIC_ROOM_STATE_INITIALIZE;
            FrontEndResetTimer(&this->stateTimer);
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        case 4:
            sceneMenu->CloseMainMenu();
            this->requestedState = FRONT_END_REQUESTED_STATE_HELP;
            this->state = HELP_MENU_INITIALIZE;
            FrontEndResetTimer(&this->stateTimer);
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        case 5:
        exitMainMenu:
            this->requestedState = FRONT_END_REQUESTED_STATE_EXIT;
            this->state = 0;
            FrontEndResetTimer(&this->stateTimer);
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
    }

    if (FrontEndInputAnd(g_PressedButtons, TH_BUTTON_MENU | TH_BUTTON_SHOOT) != 0)
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
        if (this->groupCursor.GetCurrent() == 5)
        {
            goto exitMainMenu;
        }
        this->groupCursor.Set(5);
        sceneMenu->UpdateMainMenuSelection();
    }
    break;
    default:
        break;
    }
#undef sceneMenu
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

} // namespace th095
