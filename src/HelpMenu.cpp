#include "HelpMenu.hpp"
#include "FileSystem.hpp"
#include "SoundPlayer.hpp"

#include <stdio.h>
#include <stdlib.h>

namespace th095
{

extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;

struct HelpTextureEntryView
{
    IDirect3DTexture8 *texture;
    u8 unknown004[0x0c];

    void Clear();
};

struct HelpAnmStorageView
{
    u8 unknown000[0x14];
    HelpTextureEntryView *textures;
};

static __forceinline u16 GetHelpPressedButtons(u16 buttons)
{
    return g_PressedButtons & buttons;
}

static __forceinline u16 IsHelpMenuInputPressed(u16 buttons)
{
    return (u16)((GetHelpPressedButtons(buttons) != 0) ||
                 ((g_ResultMenuInput & buttons) != 0));
}

void __fastcall LoadHelpAnm(void *unused)
{
    HelpMenuView *helpMenu = g_HelpMenu;

    helpMenu->helpAnmData = FileSystem::OpenFile(
        helpMenu->helpAnmPath, &helpMenu->helpAnmSize, FALSE);
    helpMenu->state = 3;
    g_HelpLoadActive = 0;
    g_HelpLoadComplete = 1;
}

i32 HelpMenuView::UpdateHelpMenu()
{
    i32 i;

    switch (this->state)
    {
    case 0:
        this->cursor.Push();
        this->vmIds.SetInterrupt(0x66, 1);
        this->vmIds.SetInterrupt(0x67, 1);
        this->vmIds[0x68] = this->sceneAnm->CreateVm(0x68, 7);
        this->vmIds[0x69] = this->sceneAnm->CreateVm(0x69, 7);
        this->vmIds[0x18] = this->sceneAnm->CreateVm(0x18, 7);
        this->vmIds.SetInterrupt(0x19, 3);
        this->vmIds.SetInterrupt(0x1a, 3);
        this->transitionVm.SetInterrupt(3);
        this->vmIds.SetInterrupt(0x1b, 3);
        this->state = 1;
        this->stateTimer.Reset();
        this->cursor.count = 9;
        this->cursor.Set(0);

        ((HelpAnmStorageView *)this->sceneAnm)->textures[13].Clear();
        for (i = 0; i < 9; i++)
        {
            this->vmIds[0x91 + i] =
                this->sceneAnm->CreateVm(0x91 + i, 7);
            if (this->cursor.current == i)
            {
                this->vmIds.SetInterrupt(0x91 + i, 2);
            }
            else
            {
                this->vmIds.SetInterrupt(0x91 + i, 3);
            }
        }
        return 0;

    case 1:
        if (this->stateTimer.current < 20)
        {
            return 0;
        }

        this->cursor.SaveCurrent();
        if (IsHelpMenuInputPressed(TH_BUTTON_UP))
        {
            this->cursor.Move(-1);
        }
        if (IsHelpMenuInputPressed(TH_BUTTON_DOWN))
        {
            this->cursor.Move(1);
        }
        if (this->cursor.HasChanged())
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
            for (i = 0; i < 9; i++)
            {
                if (this->cursor.current == i)
                {
                    this->vmIds.SetInterrupt(0x91 + i, 2);
                }
                else
                {
                    this->vmIds.SetInterrupt(0x91 + i, 3);
                }
            }
        }

    load_page:
        if (GetHelpPressedButtons(0x1002) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
            this->state = 2;
            this->stateTimer.Reset();
            sprintf(this->helpAnmPath, "help_%.2d.anm",
                    this->cursor.current);
            g_SceneSupervisor.StartReplayScan(LoadHelpAnm, NULL);
            for (i = 0; i < 9; i++)
            {
                this->vmIds.SetInterrupt(0x91 + i, 1);
            }
            return 0;
        }

        if (GetHelpPressedButtons(9) != 0)
        {
            this->cursor.Pop();
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            this->vmIds.SetInterrupt(0x68, 1);
            this->vmIds.SetInterrupt(0x69, 1);
            this->vmIds[0x66] = this->sceneAnm->CreateVm(0x66, 7);
            this->vmIds[0x67] = this->sceneAnm->CreateVm(0x67, 7);
            this->vmIds.SetInterrupt(0x18, 1);
            this->vmIds.SetInterrupt(0x19, 2);
            this->vmIds.SetInterrupt(0x1a, 2);
            this->transitionVm.SetInterrupt(2);
            this->vmIds.SetInterrupt(0x1b, 2);
            for (i = 0; i < 9; i++)
            {
                this->vmIds.SetInterrupt(0x91 + i, 1);
            }
            this->requestedState = 1;
            this->state = 0;
            this->stateTimer.Reset();
        }
        return 0;

    case 2:
        return 0;

    case 3:
        g_SceneAnmManager->LoadTexture(
            reinterpret_cast<SceneTextureEntryView *>(
                &((HelpAnmStorageView *)this->sceneAnm)->textures[13]),
            this->helpAnmData, this->helpAnmSize, 1, 0, 1);
        free(this->helpAnmData);
        this->helpAnmData = NULL;
        ((HelpAnmStorageView *)this->sceneAnm)->textures[13]
            .texture->PreLoad();
        this->vmIds[0x90] = this->sceneAnm->CreateVm(0x90, 7);
        this->state = 4;

    case 4:
        if (this->stateTimer.current < 20)
        {
            return 0;
        }

        if (GetHelpPressedButtons(TH_BUTTON_RIGHT) != 0 &&
            this->cursor.current < 8)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_DAMAGE_LOW_HEALTH, 0);
            this->cursor.Move(1);
            this->vmIds.SetInterrupt(0x90, 1);
            goto load_page;
        }
        if (GetHelpPressedButtons(TH_BUTTON_LEFT) != 0 &&
            this->cursor.current > 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_DAMAGE_LOW_HEALTH, 0);
            this->cursor.Move(-1);
            this->vmIds.SetInterrupt(0x90, 1);
            goto load_page;
        }

        if (GetHelpPressedButtons(0x1002) != 0 ||
            GetHelpPressedButtons(9) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            this->state = 1;
            this->stateTimer.Reset();
            this->vmIds.SetInterrupt(0x90, 1);
            for (i = 0; i < 9; i++)
            {
                this->vmIds[0x91 + i] =
                    this->sceneAnm->CreateVm(0x91 + i, 7);
                if (this->cursor.current == i)
                {
                    this->vmIds.SetInterrupt(0x91 + i, 2);
                }
                else
                {
                    this->vmIds.SetInterrupt(0x91 + i, 3);
                }
            }
        }
        return 0;
    }

    return 0;
}

} // namespace th095
