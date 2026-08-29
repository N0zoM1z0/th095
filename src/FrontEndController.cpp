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

extern i32 g_FrontEndSupervisorState;
extern i32 g_FrontEndUiState;
extern FrontEndGameManagerView *g_FrontEndGameManager;
extern FrontEndGameManagerView *g_FrontEndGlobalState;
extern i32 g_ReplayUsesArchive;
extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;
extern u16 g_FrontEndCurrentInput;
extern i32 g_DemoReplayIndex;

FrontEndGameManagerView *__fastcall CreateFrontEndGameManager(i32 mode);

struct FrontEndSupervisorAudioView
{
    ZunResult LoadMusic(i32 slot, const char *path);
    ZunResult PlayMusic(i32 slot, i32 unknown);
    ZunResult FadeOutMusic();
};

extern FrontEndSupervisorAudioView g_FrontEndSupervisorAudio;

ChainCallbackResult SceneSelectControllerView::Update()
{
    FrontEndControllerUpdateView *view =
        reinterpret_cast<FrontEndControllerUpdateView *>(this);

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
        if (g_HelpLoadActive != 0)
        {
            return CHAIN_CALLBACK_RESULT_BREAK;
        }

        g_SceneSupervisor.StopReplayScan();
        FrontEndTextureEntryView *textureEntry =
            &reinterpret_cast<FrontEndAnmStorageView *>(view->sceneAnm)
                 ->textures[1];
        IDirect3DSurface8 *surface = NULL;
        textureEntry->texture->GetSurfaceLevel(0, &surface);
        D3DLOCKED_RECT lockedRect;
        surface->LockRect(&lockedRect, NULL, 0);
        if (textureEntry->bytesPerPixel == 4)
        {
            for (i32 y = 0; y < 192; y++)
            {
                u8 *pixel =
                    reinterpret_cast<u8 *>(lockedRect.pBits) + y * lockedRect.Pitch;
                for (i32 x = 0; x < 256; x++)
                {
                    pixel[0] = 0;
                    pixel[1] = 0;
                    pixel[2] = 0;
                    pixel[3] = 0xff;
                    pixel += 4;
                }
            }
        }
        else
        {
            for (i32 y = 0; y < 192; y++)
            {
                u8 *pixel =
                    reinterpret_cast<u8 *>(lockedRect.pBits) + y * lockedRect.Pitch;
                for (i32 x = 0; x < 256; x++)
                {
                    pixel[0] = 0;
                    pixel[1] = 0xf0;
                    pixel += 2;
                }
            }
        }
        surface->UnlockRect();
        surface->Release();
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

        if (view->entryMode == 0)
        {
            view->requestedState = 1;
            view->stateTimer.Reset();
            view->state = 0;
            view->vmIds[0x66] = view->sceneAnm->CreateVm(0x66, 7);
            view->vmIds[0x67] = view->sceneAnm->CreateVm(0x67, 7);
            view->vmIds[0x19] = view->sceneAnm->CreateVm(0x19, 7);
            view->vmIds[0x1a] = view->sceneAnm->CreateVm(0x1a, 7);
            view->transitionVm.value =
                view->transitionAnm->CreateVm(0, 7).value;
            view->vmIds[0x1b] = view->sceneAnm->CreateVm(0x1b, 7);
            view->vmIds[0x64] = view->sceneAnm->CreateVm(0x64, 7);
            view->vmIds[0x65] = view->sceneAnm->CreateVm(0x65, 7);
        }
        else if (view->entryMode == 1)
        {
            view->requestedState = 2;
            view->state = 0;
            view->stateTimer.Reset();
            view->vmIds[0x19] = view->sceneAnm->CreateVm(0x19, 7);
            view->vmIds[0x1a] = view->sceneAnm->CreateVm(0x1a, 7);
            view->transitionVm.value =
                view->transitionAnm->CreateVm(0, 7).value;
            view->vmIds[0x1b] = view->sceneAnm->CreateVm(0x1b, 7);
            view->vmIds[0x64] = view->sceneAnm->CreateVm(0x64, 7);
            view->vmIds[0x65] = view->sceneAnm->CreateVm(0x65, 7);
            view->entryMode = 0;
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        else if (view->entryMode == 2)
        {
            view->requestedState = 3;
            view->state = 0;
            view->stateTimer.Reset();
            view->cursor.Set(1);
            view->vmIds[0x19] = view->sceneAnm->CreateVm(0x19, 7);
            view->vmIds[0x1a] = view->sceneAnm->CreateVm(0x1a, 7);
            view->transitionVm.value =
                view->transitionAnm->CreateVm(0, 7).value;
            view->vmIds[0x1b] = view->sceneAnm->CreateVm(0x1b, 7);
            view->vmIds[0x64] = view->sceneAnm->CreateVm(0x64, 7);
            view->vmIds[0x65] = view->sceneAnm->CreateVm(0x65, 7);
            view->entryMode = 0;
            return CHAIN_CALLBACK_RESULT_CONTINUE;
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

    case 4:
        if (g_HelpLoadActive != 0)
        {
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        g_SceneSupervisor.StopReplayScan();
        g_FrontEndSupervisorState = 1;
        break;

    case 5:
        if (view->stateTimer.current == 1)
        {
            g_FrontEndSupervisorAudio.FadeOutMusic();
            if (g_HelpLoadActive != 0)
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
        if (view->stateTimer.current > 39 && view->transitionReady != 0)
        {
            if (!g_FrontEndGlobalState->transitionBlocked)
            {
                for (i32 i = 0; i < 0x9a; i++)
                {
                    g_SceneAnmManager->SetInterrupt(view->vmIds[i], 1);
                }
                view->transitionVm.SetInterrupt(1);
                g_FrontEndSupervisorState = 3;
                if (g_ReplayUsesArchive == 0)
                {
                    g_FrontEndSupervisorAudio.PlayMusic(0, 0);
                }
            }
            else
            {
                g_FrontEndSupervisorState = 1;
            }
        }
        break;

    case 6:
        if (view->stateTimer.current == 1)
        {
            if (g_ReplayUsesArchive == 0)
            {
                g_FrontEndSupervisorAudio.FadeOutMusic();
            }
            if (g_HelpLoadActive != 0)
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
        if (view->stateTimer.current > 39 && view->transitionReady != 0)
        {
            if (!g_FrontEndGlobalState->transitionBlocked)
            {
                for (i32 i = 0; i < 0x9a; i++)
                {
                    g_SceneAnmManager->SetInterrupt(view->vmIds[i], 1);
                }
                view->transitionVm.SetInterrupt(1);
                g_FrontEndSupervisorState = 7;
                if (g_ReplayUsesArchive == 0)
                {
                    g_FrontEndSupervisorAudio.PlayMusic(0, 0);
                }
            }
            else
            {
                g_FrontEndSupervisorState = 1;
            }
        }
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
    }

    FrontEndVmUpdateView *first =
        reinterpret_cast<FrontEndVmUpdateView *>(view->vmIds[0x66].GetVm());
    FrontEndVmUpdateView *second =
        reinterpret_cast<FrontEndVmUpdateView *>(view->vmIds[0x67].GetVm());
    if (first != NULL && second != NULL)
    {
        second->position = first->position;
        second->position.x += 512.0f;
        second->displayState = first->displayState;
    }

    first = reinterpret_cast<FrontEndVmUpdateView *>(
        view->vmIds[0x68].GetVm());
    second = reinterpret_cast<FrontEndVmUpdateView *>(
        view->vmIds[0x69].GetVm());
    if (first != NULL && second != NULL)
    {
        second->position = first->position;
        second->position.x += 512.0f;
        second->displayState = first->displayState;
    }

    if (view->animationTimer.current % 5 == 0)
    {
        view->sceneAnm->CreateVm(0x1c, 0);
    }
    view->stateTimer.Tick();
    view->animationTimer.Tick();
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
    FrontEndControllerUpdateView *view =
        reinterpret_cast<FrontEndControllerUpdateView *>(this);

    if (view->state == 0)
    {
        view->cursor.count = 6;
        view->cursor.wraps = 1;
        view->state = 1;

        Float3 position0(64.0f, 130.0f, 0.0f);
        view->vmIds[0] = view->sceneAnm->CreateVm(0, &position0);
        Float3 position1(64.0f, 168.0f, 0.0f);
        view->vmIds[1] = view->sceneAnm->CreateVm(1, &position1);
        Float3 position2(64.0f, 206.0f, 0.0f);
        view->vmIds[2] = view->sceneAnm->CreateVm(2, &position2);
        Float3 position3(64.0f, 244.0f, 0.0f);
        view->vmIds[3] = view->sceneAnm->CreateVm(3, &position3);
        Float3 position4(64.0f, 282.0f, 0.0f);
        view->vmIds[4] = view->sceneAnm->CreateVm(4, &position4);
        Float3 position5(64.0f, 320.0f, 0.0f);
        view->vmIds[5] = view->sceneAnm->CreateVm(5, &position5);

        Float3 position12(64.0f, 130.0f, 0.0f);
        view->vmIds[12] = view->sceneAnm->CreateVm(12, &position12);
        Float3 position13(64.0f, 168.0f, 0.0f);
        view->vmIds[13] = view->sceneAnm->CreateVm(13, &position13);
        Float3 position14(64.0f, 206.0f, 0.0f);
        view->vmIds[14] = view->sceneAnm->CreateVm(14, &position14);
        Float3 position15(64.0f, 244.0f, 0.0f);
        view->vmIds[15] = view->sceneAnm->CreateVm(15, &position15);
        Float3 position16(64.0f, 282.0f, 0.0f);
        view->vmIds[16] = view->sceneAnm->CreateVm(16, &position16);
        Float3 position17(64.0f, 320.0f, 0.0f);
        view->vmIds[17] = view->sceneAnm->CreateVm(17, &position17);

        Float3 position6(64.0f, 130.0f, 0.0f);
        view->vmIds[6] = view->sceneAnm->CreateVm(6, &position6);
        Float3 position7(64.0f, 168.0f, 0.0f);
        view->vmIds[7] = view->sceneAnm->CreateVm(7, &position7);
        Float3 position8(64.0f, 206.0f, 0.0f);
        view->vmIds[8] = view->sceneAnm->CreateVm(8, &position8);
        Float3 position9(64.0f, 244.0f, 0.0f);
        view->vmIds[9] = view->sceneAnm->CreateVm(9, &position9);
        Float3 position10(64.0f, 282.0f, 0.0f);
        view->vmIds[10] = view->sceneAnm->CreateVm(10, &position10);
        Float3 position11(64.0f, 320.0f, 0.0f);
        view->vmIds[11] = view->sceneAnm->CreateVm(11, &position11);
        view->stateTimer.Reset();
    }
    else if (view->state != 1)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }

    if (view->stateTimer.current < 30)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (view->stateTimer.current == 30)
    {
        this->UpdateMainMenuSelection();
    }

    view->cursor.SaveCurrent();
    if ((g_PressedButtons & TH_BUTTON_UP) != 0 ||
        (g_ResultMenuInput & TH_BUTTON_UP) != 0)
    {
        view->cursor.Move(-1);
    }
    if ((g_PressedButtons & TH_BUTTON_DOWN) != 0 ||
        (g_ResultMenuInput & TH_BUTTON_DOWN) != 0)
    {
        view->cursor.Move(1);
    }
    if (view->cursor.HasChanged())
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        this->UpdateMainMenuSelection();
    }

    if ((g_FrontEndCurrentInput & 0x160b) == 0)
    {
        g_FrontEndUiState++;
        if (g_FrontEndUiState > 1799)
        {
            g_FrontEndUiState = 0;
            g_ReplayUsesArchive = 1;
            sprintf(g_SelectedReplayPath, "demo/demo%d.rpy", g_DemoReplayIndex);
            g_DemoReplayIndex = (g_DemoReplayIndex + 1) % 3;
            view->requestedState = 6;
            view->stateTimer.Reset();
            view->state = 0;
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
    }
    else
    {
        g_FrontEndUiState = 0;
    }

    if ((g_PressedButtons & 0x1002) != 0)
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
        switch (view->cursor.current)
        {
        case 0:
        {
            this->CloseMainMenu();
            view->requestedState = 2;
            view->state = 0;
            view->stateTimer.Reset();
            while (g_HelpLoadActive != 0)
            {
                Sleep(1);
            }
            SceneValueQueue *firstQueue = reinterpret_cast<SceneValueQueue *>(
                reinterpret_cast<u8 *>(this) + 0x61b8);
            while (firstQueue->count != 0)
            {
                free(reinterpret_cast<void *>(firstQueue->Pop()));
            }
            SceneValueQueue *secondQueue = reinterpret_cast<SceneValueQueue *>(
                reinterpret_cast<u8 *>(this) + 0x6248);
            while (secondQueue->count != 0)
            {
                free(reinterpret_cast<void *>(secondQueue->Pop()));
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
            break;
        }
        case 1:
            this->CloseMainMenu();
            view->requestedState = 3;
            view->state = 0;
            view->stateTimer.Reset();
            break;
        case 2:
            this->CloseMainMenu();
            view->requestedState = 8;
            view->state = 0;
            view->stateTimer.Reset();
            break;
        case 3:
            this->CloseMainMenu();
            view->requestedState = 7;
            view->state = 0;
            view->stateTimer.Reset();
            break;
        case 4:
            this->CloseMainMenu();
            view->requestedState = 9;
            view->state = 0;
            view->stateTimer.Reset();
            break;
        case 5:
            view->requestedState = 4;
            view->state = 0;
            view->stateTimer.Reset();
            break;
        }
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }

    if ((g_PressedButtons & 9) != 0)
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
        if (view->cursor.current != 5)
        {
            view->cursor.Set(5);
            this->UpdateMainMenuSelection();
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
        view->requestedState = 4;
        view->state = 0;
        view->stateTimer.Reset();
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

} // namespace th095
