#include "SceneSelect.hpp"
#include "ReplayBrowser.hpp"
#include "OptionsMenu.hpp"
#include "MusicRoom.hpp"
#include "HelpMenu.hpp"
#include "SoundPlayer.hpp"
#include "ZunMath.hpp"

#include <d3d8.h>
#include <stdio.h>
#include <stdlib.h>

namespace th095
{

struct FrontEndTextureEntryView
{
    IDirect3DTexture8 *texture;
    u8 unknown004[8];
    i32 bytesPerPixel;
};

struct FrontEndAnmStorageView
{
    u8 unknown000[0x14];
    FrontEndTextureEntryView *textures;
};

struct FrontEndVmUpdateView
{
    u8 unknown000[0x148];
    Float3 position;
    u8 unknown154[0xcc];
    u32 displayState;
};

struct FrontEndGameManagerView
{
    u8 unknown000[0xfc];
    union
    {
        u32 flags;
        struct
        {
            u32 unknownFlags0 : 3;
            u32 transitionBlocked : 1;
            u32 unknownFlags4 : 28;
        };
    };
};

struct FrontEndControllerUpdateView
{
    SceneAnmLoadedView *sceneAnm;
    SceneAnmLoadedView *transitionAnm;
    ResultScreenTimer stateTimer;
    ResultScreenTimer animationTimer;
    ResultScreenReplayCursor cursor;
    u8 unknown00f8[0xafc];
    SceneAnmVmIdArray vmIds;
    u8 unknown0e88[0x5278];
    AnmVmId transitionVm;
    u8 unknown6104[4];
    i32 transitionReady;
    i32 state;
    i32 requestedState;
    u8 unknown6114[0x0c];
    union
    {
        u32 flags;
        struct
        {
            u32 unknownFlag0 : 1;
            u32 exitToResult : 1;
            u32 unknownFlags2 : 30;
        };
    };
    i32 entryMode;
};

struct FrontEndUpdateLocals
{
    FrontEndVmUpdateView *second69;
    FrontEndVmUpdateView *first68;
    FrontEndVmUpdateView *second67;
    FrontEndVmUpdateView *first66;
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

typedef char FrontEndTextureEntrySizeIs10[
    (sizeof(FrontEndTextureEntryView) == 0x10) ? 1 : -1];
typedef char FrontEndTextureFormatAtC[
    (offsetof(FrontEndTextureEntryView, bytesPerPixel) == 0x0c) ? 1 : -1];
typedef char FrontEndVmPositionAt148[
    (offsetof(FrontEndVmUpdateView, position) == 0x148) ? 1 : -1];
typedef char FrontEndVmDisplayStateAt220[
    (offsetof(FrontEndVmUpdateView, displayState) == 0x220) ? 1 : -1];
typedef char FrontEndUpdateCursorAt20[
    (offsetof(FrontEndControllerUpdateView, cursor) == 0x20) ? 1 : -1];
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

extern i32 g_FrontEndSupervisorState;
extern i32 g_FrontEndUiState;
extern FrontEndGameManagerView *g_FrontEndGameManager;
extern FrontEndGameManagerView *g_FrontEndGlobalState;
extern i32 g_ReplayUsesArchive;
extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;
extern u16 g_FrontEndCurrentInput;
extern i32 g_DemoReplayIndex;

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

static __forceinline i32 FrontEndHelpLoadSnapshot()
{
    i32 active = g_HelpLoadActive;
    return active;
}

static __forceinline void FrontEndFreePoppedValue(void *block)
{
    free(block);
}

static __forceinline void FrontEndDrainQueueValue(SceneValueQueue *queue)
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

FrontEndGameManagerView *__fastcall CreateFrontEndGameManager(i32 mode);

struct FrontEndSupervisorAudioView
{
    ZunResult LoadMusic(i32 slot, const char *path);
    ZunResult PlayMusic(i32 slot, i32 unknown);
    ZunResult FadeOutMusic(f32 durationSeconds);
};

extern FrontEndSupervisorAudioView g_FrontEndSupervisorAudio;

ChainCallbackResult SceneSelectControllerView::Update()
{
#define view (reinterpret_cast<FrontEndControllerUpdateView *>(this))
    FrontEndUpdateLocals locals;

    switch (view->requestedState)
    {
    case 0:
    {
        if (view->exitToResult)
        {
            g_SceneSupervisor.StopReplayScan();
            g_FrontEndSupervisorState = 6;
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        if (FrontEndHelpLoadSnapshot() != 0)
        {
            return CHAIN_CALLBACK_RESULT_BREAK;
        }

        g_SceneSupervisor.StopReplayScan();
        locals.surface = NULL;
        reinterpret_cast<FrontEndAnmStorageView *>(view->sceneAnm)
            ->textures[1]
            .texture->GetSurfaceLevel(0, &locals.surface);
        locals.surface->LockRect(&locals.lockedRect, NULL, 0);
        if (reinterpret_cast<FrontEndAnmStorageView *>(view->sceneAnm)
                ->textures[1]
                .bytesPerPixel == 4)
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
        g_FrontEndUiState = 0;

        if (g_ReplayUsesArchive == 0)
        {
            g_FrontEndSupervisorAudio.LoadMusic(0, "bgm/th09_00.wav");
            g_FrontEndSupervisorAudio.PlayMusic(0, 0);
        }
        else
        {
            view->entryMode = 0;
            g_ReplayUsesArchive = 0;
        }

        switch (view->entryMode)
        {
        case 0:
        {
            view->requestedState = 1;
            view->stateTimer.Reset();
            view->state = 0;
            FrontEndCreateSceneVm(view, 0x66);
            FrontEndCreateSceneVm(view, 0x67);
            FrontEndCreateSceneVm(view, 0x19);
            FrontEndCreateSceneVm(view, 0x1a);
            // Both target handles are four-byte VM ids; preserving the
            // returned wrapper as a whole is codegen-significant here.
            *reinterpret_cast<SceneAnmVmId *>(&view->transitionVm) =
                view->transitionAnm->CreateVm(0, 7);
            FrontEndCreateSceneVm(view, 0x1b);
            FrontEndCreateSceneVm(view, 0x64);
            FrontEndCreateSceneVm(view, 0x65);
            break;
        }
        case 1:
        {
            view->requestedState = 2;
            view->state = 0;
            view->stateTimer.Reset();
            FrontEndCreateSceneVm(view, 0x19);
            FrontEndCreateSceneVm(view, 0x1a);
            *reinterpret_cast<SceneAnmVmId *>(&view->transitionVm) =
                view->transitionAnm->CreateVm(0, 7);
            FrontEndCreateSceneVm(view, 0x1b);
            FrontEndCreateSceneVm(view, 0x64);
            FrontEndCreateSceneVm(view, 0x65);
            view->entryMode = 0;
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        case 2:
        {
            view->requestedState = 3;
            view->state = 0;
            view->stateTimer.Reset();
            view->cursor.Set(1);
            FrontEndCreateSceneVm(view, 0x19);
            FrontEndCreateSceneVm(view, 0x1a);
            *reinterpret_cast<SceneAnmVmId *>(&view->transitionVm) =
                view->transitionAnm->CreateVm(0, 7);
            FrontEndCreateSceneVm(view, 0x1b);
            FrontEndCreateSceneVm(view, 0x64);
            FrontEndCreateSceneVm(view, 0x65);
            view->entryMode = 0;
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        default:
            break;
        }
    }
    case 1:
        this->UpdateMainMenu();
        break;

    case 2:
        this->UpdateSceneSelect();
        break;

    case 3:
        reinterpret_cast<ReplayBrowserView *>(this)->Update();
        break;

    case 7:
        if (reinterpret_cast<OptionsMenuView *>(this)->Update() ==
            CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR)
        {
            return CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR;
        }
        break;

    case 8:
        reinterpret_cast<MusicRoomView *>(this)->UpdateMusicRoom();
        break;

    case 9:
        reinterpret_cast<HelpMenuView *>(this)->UpdateHelpMenu();
        break;

    case 5:
        if ((view->stateTimer.current == 1) != 0)
        {
            g_FrontEndSupervisorAudio.FadeOutMusic(2.0f);
            if (FrontEndHelpLoadSnapshot() != 0)
            {
                return CHAIN_CALLBACK_RESULT_CONTINUE;
            }
            g_SceneSupervisor.StopReplayScan();
            g_FrontEndGameManager = CreateFrontEndGameManager(0);
            if (g_FrontEndGameManager == NULL)
            {
                g_FrontEndSupervisorState = 1;
            }
        }
        if ((view->stateTimer.current < 40) != 0)
        {
            break;
        }
        if (view->transitionReady != 0)
        {
            if (g_FrontEndGlobalState->transitionBlocked)
            {
                g_FrontEndSupervisorState = 1;
                break;
            }
            for (locals.gameInterruptIndex = 0;
                 locals.gameInterruptIndex < 0x9a;
                 locals.gameInterruptIndex++)
            {
                g_SceneAnmManager->SetInterrupt(
                    view->vmIds[locals.gameInterruptIndex], 1);
            }
            view->transitionVm.SetInterrupt(1);
            g_FrontEndSupervisorState = 3;
            if (g_ReplayUsesArchive == 0)
            {
                g_FrontEndSupervisorAudio.PlayMusic(0, 0);
            }
            break;
        }
        break;

    case 6:
        if ((view->stateTimer.current == 1) != 0)
        {
            if (g_ReplayUsesArchive == 0)
            {
                g_FrontEndSupervisorAudio.FadeOutMusic(2.0f);
            }
            if (FrontEndHelpLoadSnapshot() != 0)
            {
                return CHAIN_CALLBACK_RESULT_CONTINUE;
            }
            g_SceneSupervisor.StopReplayScan();
            g_FrontEndGameManager = CreateFrontEndGameManager(1);
            if (g_FrontEndGameManager == NULL)
            {
                g_FrontEndSupervisorState = 1;
            }
        }
        if ((view->stateTimer.current < 40) != 0)
        {
            break;
        }
        if (view->transitionReady != 0)
        {
            if (g_FrontEndGlobalState->transitionBlocked)
            {
                g_FrontEndSupervisorState = 1;
                break;
            }
            for (locals.replayInterruptIndex = 0;
                 locals.replayInterruptIndex < 0x9a;
                 locals.replayInterruptIndex++)
            {
                g_SceneAnmManager->SetInterrupt(
                    view->vmIds[locals.replayInterruptIndex], 1);
            }
            view->transitionVm.SetInterrupt(1);
            g_FrontEndSupervisorState = 7;
            if (g_ReplayUsesArchive == 0)
            {
                g_FrontEndSupervisorAudio.PlayMusic(0, 0);
            }
            break;
        }
        break;

    case 4:
        if (FrontEndHelpLoadSnapshot() != 0)
        {
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        g_SceneSupervisor.StopReplayScan();
        g_FrontEndSupervisorState = 1;
        break;
    }

    locals.first66 =
        reinterpret_cast<FrontEndVmUpdateView *>(
            reinterpret_cast<AnmVmId *>(
                reinterpret_cast<u8 *>(this) + 0xd8c)
                ->GetVm());
    locals.second67 =
        reinterpret_cast<FrontEndVmUpdateView *>(
            reinterpret_cast<AnmVmId *>(
                reinterpret_cast<u8 *>(this) + 0xd90)
                ->GetVm());
    if (locals.first66 != NULL && locals.second67 != NULL)
    {
        locals.second67->position = locals.first66->position;
        locals.second67->position.x += 512.0f;
        locals.second67->displayState = locals.first66->displayState;
    }

    locals.first68 = reinterpret_cast<FrontEndVmUpdateView *>(
        reinterpret_cast<AnmVmId *>(
            reinterpret_cast<u8 *>(this) + 0xd94)
            ->GetVm());
    locals.second69 = reinterpret_cast<FrontEndVmUpdateView *>(
        reinterpret_cast<AnmVmId *>(
            reinterpret_cast<u8 *>(this) + 0xd98)
            ->GetVm());
    if (locals.first68 != NULL && locals.second69 != NULL)
    {
        locals.second69->position = locals.first68->position;
        locals.second69->position.x += 512.0f;
        locals.second69->displayState = locals.first68->displayState;
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
#define MAIN_MENU_VM(offset)                                                   \
    (*reinterpret_cast<SceneAnmVmId *>(reinterpret_cast<u8 *>(this) + offset))
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xbf4), (this->GetSelectedGroup() != 0) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc0c), (this->GetSelectedGroup() != 0) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xbf8), (this->GetSelectedGroup() != 1) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc10), (this->GetSelectedGroup() != 1) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xbfc), (this->GetSelectedGroup() != 2) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc14), (this->GetSelectedGroup() != 2) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc00), (this->GetSelectedGroup() != 3) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc18), (this->GetSelectedGroup() != 3) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc04), (this->GetSelectedGroup() != 4) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc1c), (this->GetSelectedGroup() != 4) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc08), (this->GetSelectedGroup() != 5) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc20), (this->GetSelectedGroup() != 5) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc24), (this->GetSelectedGroup() != 0) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc28), (this->GetSelectedGroup() != 1) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc2c), (this->GetSelectedGroup() != 2) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc30), (this->GetSelectedGroup() != 3) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc34), (this->GetSelectedGroup() != 4) + 2);
    g_SceneAnmManager->SetInterrupt(
        MAIN_MENU_VM(0xc38), (this->GetSelectedGroup() != 5) + 2);
#undef MAIN_MENU_VM
}

ChainCallbackResult SceneSelectControllerView::UpdateMainMenu()
{
#define view (reinterpret_cast<FrontEndControllerUpdateView *>(this))
    MainMenuVmPositions positions;

    switch (view->state)
    {
    case 0:
    {
        view->cursor.count = 6;
        view->cursor.wraps = 1;
        view->state = 1;

#define CREATE_MAIN_MENU_VM(position, index, yValue)                           \
    positions.position.x = 64.0f;                                             \
    positions.position.y = yValue;                                            \
    positions.position.z = 0.0f;                                              \
    view->vmIds[index] =                                                      \
        view->sceneAnm->CreateVm(index, &positions.position)
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
        view->stateTimer.Reset();
    }
    case 1:
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

    if (FrontEndInputAnd(g_FrontEndCurrentInput, 0x160b) != 0)
    {
        g_FrontEndUiState = 0;
    }
    else
    {
        g_FrontEndUiState++;
        if (g_FrontEndUiState >= 1800)
        {
            g_FrontEndUiState = 0;
            g_ReplayUsesArchive = 1;
            sprintf(g_SelectedReplayPath, "demo/demo%d.rpy", g_DemoReplayIndex);
            g_DemoReplayIndex++;
            g_DemoReplayIndex %= 3;
            view->requestedState = 6;
            view->stateTimer.Reset();
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
            view->requestedState = 2;
            view->state = 0;
            view->stateTimer.Reset();
            while (FrontEndHelpLoadSnapshot() != 0)
            {
                Sleep(1);
            }
            while (reinterpret_cast<SceneValueQueue *>(
                       reinterpret_cast<u8 *>(this) + 0x61b8)
                       ->Size() != 0)
            {
                FrontEndDrainQueueValue(reinterpret_cast<SceneValueQueue *>(
                    reinterpret_cast<u8 *>(this) + 0x61b8));
            }
            while (reinterpret_cast<SceneValueQueue *>(
                       reinterpret_cast<u8 *>(this) + 0x6248)
                       ->Size() != 0)
            {
                FrontEndDrainQueueValue(reinterpret_cast<SceneValueQueue *>(
                    reinterpret_cast<u8 *>(this) + 0x6248));
            }
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x6240) = 0;
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x62d0) = 0;
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x6318) = 0;
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x6360) = 0;
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x63a8) = 0;
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x6168) = 0;
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x61b0) = 0;
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x63cc) = 0;
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x63bc) = 0;
            *reinterpret_cast<i8 *>(reinterpret_cast<u8 *>(this) + 0xe92) = -1;
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        case 1:
            this->CloseMainMenu();
            view->requestedState = 3;
            view->state = 0;
            view->stateTimer.Reset();
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        case 2:
            this->CloseMainMenu();
            view->requestedState = 7;
            view->state = 0;
            view->stateTimer.Reset();
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        case 3:
            this->CloseMainMenu();
            view->requestedState = 8;
            view->state = 0;
            view->stateTimer.Reset();
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        case 4:
            this->CloseMainMenu();
            view->requestedState = 9;
            view->state = 0;
            view->stateTimer.Reset();
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        case 5:
        exitMainMenu:
            view->requestedState = 4;
            view->state = 0;
            view->stateTimer.Reset();
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
