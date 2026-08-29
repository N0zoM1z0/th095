#include "SceneSelect.hpp"
#include "ReplayBrowser.hpp"
#include "OptionsMenu.hpp"
#include "MusicRoom.hpp"
#include "HelpMenu.hpp"
#include "ZunMath.hpp"

#include <d3d8.h>

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

} // namespace th095
