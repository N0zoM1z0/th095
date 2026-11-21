#include "AnmManager.hpp"
#include "ControllerInputSlot.hpp"
#include "FileSystem.hpp"
#include "FrontEndController.hpp"
#include "SceneSelect.hpp"
#include "SoundPlayer.hpp"

#include <stdio.h>
#include <stdlib.h>

namespace th095
{

static __forceinline void HelpMenuCreateVmAt(SceneSelectControllerView *view, i32 scriptIndex)
{
    view->vmIds[scriptIndex] =
        view->sceneAnm->CreateVm(scriptIndex, 7);
}

static __forceinline void HelpMenuFreeAnmData(FrontEndControllerView *view)
{
    u8 *data = view->helpAnmData;
    free(data);
    view->helpAnmData = NULL;
}

static __forceinline u16 GetHelpPressedButtons(u16 buttons)
{
    return g_ControllerInputSlots[0].pressed & buttons;
}

static __forceinline u16 IsHelpMenuInputPressed(u16 buttons)
{
    return (u16)((GetHelpPressedButtons(buttons) != 0) ||
                 ((g_ControllerInputSlots[0].repeat & buttons) != 0));
}

void __fastcall LoadHelpAnm(void *unused)
{
    FrontEndControllerView *helpMenu =
        static_cast<FrontEndControllerView *>(g_ActiveMenuController);

    helpMenu->helpAnmData = FileSystem::OpenFile(
        helpMenu->helpAnmPath, &helpMenu->helpAnmSize, FALSE);
    helpMenu->state = HELP_MENU_PAGE_DATA_READY;
    WriteWorkerSignal(g_Supervisor.replayScanWorker.active, 0);
    WriteWorkerSignal(g_Supervisor.replayScanWorker.exitSignal, 1);
}

i32 FrontEndControllerView::UpdateHelpMenu()
{
// The animation slots still use Scene Select's POD handle overlay.
#define sceneMenu (reinterpret_cast<SceneSelectControllerView *>(this))
    switch (this->state)
    {
    case HELP_MENU_INITIALIZE:
        this->groupCursor.Push();
        sceneMenu->vmIds.SetInterrupt(0x66, 1);
        sceneMenu->vmIds.SetInterrupt(0x67, 1);
        HelpMenuCreateVmAt(sceneMenu, 0x68);
        HelpMenuCreateVmAt(sceneMenu, 0x69);
        HelpMenuCreateVmAt(sceneMenu, 0x18);
        sceneMenu->vmIds.SetInterrupt(0x19, 3);
        sceneMenu->vmIds.SetInterrupt(0x1a, 3);
        sceneMenu->transitionVm.SetInterrupt(3);
        sceneMenu->vmIds.SetInterrupt(0x1b, 3);
        this->state = HELP_MENU_PAGE_SELECT;
        FrontEndResetTimer(&this->stateTimer);
        this->groupCursor.count = 9;
        this->groupCursor.Set(0);

        this->sceneAnm->textures[13].Clear();
        for (i32 i = 0; i < 9; i++)
        {
            HelpMenuCreateVmAt(sceneMenu, 0x91 + i);
            if (this->groupCursor.GetCurrent() == i)
            {
                sceneMenu->vmIds.SetInterrupt(0x91 + i, 2);
            }
            else
            {
                sceneMenu->vmIds.SetInterrupt(0x91 + i, 3);
            }
        }
        break;

    case HELP_MENU_PAGE_SELECT:
        if (this->stateTimer < 20)
        {
            break;
        }

        this->groupCursor.SaveCurrent();
        if (IsHelpMenuInputPressed(TH_BUTTON_UP))
        {
            this->groupCursor.Move(-1);
        }
        if (IsHelpMenuInputPressed(TH_BUTTON_DOWN))
        {
            this->groupCursor.Move(1);
        }
        if (this->groupCursor.HasChanged())
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
            for (i32 i = 0; i < 9; i++)
            {
                if (this->groupCursor.GetCurrent() == i)
                {
                    sceneMenu->vmIds.SetInterrupt(0x91 + i, 2);
                }
                else
                {
                    sceneMenu->vmIds.SetInterrupt(0x91 + i, 3);
                }
            }
        }

        if (GetHelpPressedButtons(TH_BUTTON_ENTER | TH_BUTTON_BOMB) != 0)
        {
        load_page:
            g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
            this->state = HELP_MENU_PAGE_LOADING;
            FrontEndResetTimer(&this->stateTimer);
            sprintf(this->helpAnmPath, "help_%.2d.anm",
                    this->groupCursor.GetCurrent());
            g_Supervisor.StartReplayScan(LoadHelpAnm, NULL);
            for (i32 i = 0; i < 9; i++)
            {
                sceneMenu->vmIds.SetInterrupt(0x91 + i, 1);
            }
            break;
        }

        if (GetHelpPressedButtons(TH_BUTTON_MENU | TH_BUTTON_SHOOT) != 0)
        {
            this->groupCursor.Pop();
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            sceneMenu->vmIds.SetInterrupt(0x68, 1);
            sceneMenu->vmIds.SetInterrupt(0x69, 1);
            HelpMenuCreateVmAt(sceneMenu, 0x66);
            HelpMenuCreateVmAt(sceneMenu, 0x67);
            sceneMenu->vmIds.SetInterrupt(0x18, 1);
            sceneMenu->vmIds.SetInterrupt(0x19, 2);
            sceneMenu->vmIds.SetInterrupt(0x1a, 2);
            sceneMenu->transitionVm.SetInterrupt(2);
            sceneMenu->vmIds.SetInterrupt(0x1b, 2);
            for (i32 i = 0; i < 9; i++)
            {
                sceneMenu->vmIds.SetInterrupt(0x91 + i, 1);
            }
            this->requestedState = FRONT_END_REQUESTED_STATE_MAIN_MENU;
            this->state = HELP_MENU_INITIALIZE;
            FrontEndResetTimer(&this->stateTimer);
            break;
        }
        break;

    case HELP_MENU_PAGE_LOADING:
        break;

    case HELP_MENU_PAGE_DATA_READY:
    {
        g_AnmManager->LoadTexture(
            &this->sceneAnm->textures[13],
            this->helpAnmData, this->helpAnmSize, 1, 0, 1);
        HelpMenuFreeAnmData(this);
        this->sceneAnm->textures[13].texture->PreLoad();
        HelpMenuCreateVmAt(sceneMenu, 0x90);
        this->state = HELP_MENU_PAGE_VIEW;
    }

    case HELP_MENU_PAGE_VIEW:
        if (this->stateTimer < 20)
        {
            break;
        }

        if (GetHelpPressedButtons(TH_BUTTON_RIGHT) != 0 &&
            this->groupCursor.GetCurrent() < 8)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_TAKE_PHOTO, 0);
            this->groupCursor.Move(1);
            sceneMenu->vmIds.SetInterrupt(0x90, 1);
            goto load_page;
        }
        if (GetHelpPressedButtons(TH_BUTTON_LEFT) != 0 &&
            this->groupCursor.GetCurrent() > 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_TAKE_PHOTO, 0);
            this->groupCursor.Move(-1);
            sceneMenu->vmIds.SetInterrupt(0x90, 1);
            goto load_page;
        }

        if (GetHelpPressedButtons(TH_BUTTON_ENTER | TH_BUTTON_BOMB) != 0 ||
            GetHelpPressedButtons(TH_BUTTON_MENU | TH_BUTTON_SHOOT) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            this->state = HELP_MENU_PAGE_SELECT;
            FrontEndResetTimer(&this->stateTimer);
            sceneMenu->vmIds.SetInterrupt(0x90, 1);
            for (i32 i = 0; i < 9; i++)
            {
                HelpMenuCreateVmAt(sceneMenu, 0x91 + i);
                if (this->groupCursor.GetCurrent() == i)
                {
                    sceneMenu->vmIds.SetInterrupt(0x91 + i, 2);
                }
                else
                {
                    sceneMenu->vmIds.SetInterrupt(0x91 + i, 3);
                }
            }
        }
        break;
    }

#undef sceneMenu
    return 0;
}

} // namespace th095
