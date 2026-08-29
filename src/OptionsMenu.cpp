#include "OptionsMenu.hpp"
#include "Controller.hpp"
#include "SoundPlayer.hpp"

namespace th095
{

extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;

inline u16 GetOptionsPressedButtons(u16 buttons)
{
    return g_PressedButtons & buttons;
}

inline u16 IsOptionsMenuInputPressed(u16 buttons)
{
    return (u16)((GetOptionsPressedButtons(buttons) != 0) ||
                 ((g_ResultMenuInput & buttons) != 0));
}

ChainCallbackResult OptionsMenuView::Update()
{
    i32 i;
    i16 joystickButton;
    u8 *joystickButtons;

    switch (this->state)
    {
    case 0:
        g_SceneSupervisor.StopReplayScan();
        this->stateTimer.Reset();
        this->cursor.Push();
        this->cursor.Set(0);
        this->cursor.count = 7;
        this->cursor.wraps = 1;
        this->state = 1;

        this->vmIds[0x68] = this->sceneAnm->CreateVm(0x68, 7);
        this->vmIds[0x69] = this->sceneAnm->CreateVm(0x69, 7);
        this->vmIds.SetInterrupt(0x19, 3);
        this->vmIds.SetInterrupt(0x1a, 3);
        this->transitionVm.SetInterrupt(3);
        this->vmIds.SetInterrupt(0x1b, 3);
        this->vmIds[0x6a] = this->sceneAnm->CreateVm(0x6a, 7);
        for (i = 0; i < 0x17; i++)
        {
            this->vmIds[i + 0x6b] =
                this->sceneAnm->CreateVm(i + 0x6b, 7);
        }

        this->savedWindowed = g_OptionsGameConfig.windowed;
        this->UpdateWindowModeSprites();
        this->controllerBinding = g_OptionsGameConfig.controllerBinding;
        this->UpdateButtonSprites(0x74, this->controllerBinding.button00);
        this->UpdateButtonSprites(0x72, this->controllerBinding.button02);
        this->UpdateButtonSprites(0x76, this->controllerBinding.button06);
        this->UpdateVolumeSprites(0x7a, g_OptionsGameConfig.bgmVolume);
        this->UpdateVolumeSprites(0x7e, g_OptionsGameConfig.sfxVolume);
        this->outerFlags &= ~8u;

    case 1:
        break;

    default:
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }

    if (this->stateTimer < 30)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (this->stateTimer == 30)
    {
        this->UpdateSelectionSprites();
    }

    this->cursor.SaveCurrent();
    if (GetOptionsPressedButtons(TH_BUTTON_UP) != 0)
    {
        this->cursor.Move(-1);
    }
    if (GetOptionsPressedButtons(TH_BUTTON_DOWN) != 0)
    {
        this->cursor.Move(1);
    }

    if (!this->cursor.HasChanged())
    {
        if (IsOptionsMenuInputPressed(TH_BUTTON_LEFT))
        {
            switch (this->cursor.GetCurrent())
            {
            case 3:
                g_OptionsGameConfig.windowed =
                    1 - g_OptionsGameConfig.windowed;
                this->UpdateWindowModeSprites();
                break;

            case 4:
                if (g_OptionsGameConfig.bgmVolume > 0)
                {
                    g_OptionsGameConfig.bgmVolume -= 5;
                    if (g_OptionsGameConfig.bgmVolume < 0)
                    {
                        g_OptionsGameConfig.bgmVolume = 0;
                    }
                }
                this->UpdateVolumeSprites(
                    0x7a, g_OptionsGameConfig.bgmVolume);
                break;

            case 5:
                if (g_OptionsGameConfig.sfxVolume > 0)
                {
                    g_OptionsGameConfig.sfxVolume -= 5;
                    if (g_OptionsGameConfig.sfxVolume < 0)
                    {
                        g_OptionsGameConfig.sfxVolume = 0;
                    }
                }
                this->UpdateVolumeSprites(
                    0x7e, g_OptionsGameConfig.sfxVolume);
                break;
            }
        }

        if (IsOptionsMenuInputPressed(TH_BUTTON_RIGHT))
        {
            switch (this->cursor.GetCurrent())
            {
            case 3:
                g_OptionsGameConfig.windowed =
                    1 - g_OptionsGameConfig.windowed;
                this->UpdateWindowModeSprites();
                break;

            case 4:
                if (g_OptionsGameConfig.bgmVolume < 100)
                {
                    g_OptionsGameConfig.bgmVolume += 5;
                    if (g_OptionsGameConfig.bgmVolume > 100)
                    {
                        g_OptionsGameConfig.bgmVolume = 100;
                    }
                }
                this->UpdateVolumeSprites(
                    0x7a, g_OptionsGameConfig.bgmVolume);
                break;

            case 5:
                if (g_OptionsGameConfig.sfxVolume < 100)
                {
                    g_OptionsGameConfig.sfxVolume += 5;
                    if (g_OptionsGameConfig.sfxVolume > 100)
                    {
                        g_OptionsGameConfig.sfxVolume = 100;
                    }
                }
                this->UpdateVolumeSprites(
                    0x7e, g_OptionsGameConfig.sfxVolume);
                break;
            }
        }

        g_SoundPlayer.bgmVolume = g_OptionsGameConfig.bgmVolume;
        g_SoundPlayer.sfxVolume = g_OptionsGameConfig.sfxVolume;
        if (this->cursor.GetCurrent() == 5 && this->frameCounter % 40 == 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_TAKE_PHOTO, 0);
        }

        joystickButtons = Controller::GetControllerState(0);
        joystickButton = 0;
        while (joystickButton < 0x20)
        {
            if ((joystickButtons[joystickButton] & 0x80) != 0)
            {
                break;
            }
            joystickButton++;
        }
        if (joystickButton < 0x20 &&
            g_OptionsLastJoystickButton != joystickButton)
        {
            switch (this->cursor.GetCurrent())
            {
            case 0:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                if (this->controllerBinding.button06 == joystickButton)
                {
                    this->controllerBinding.button06 =
                        this->controllerBinding.button02;
                }
                if (this->controllerBinding.button00 == joystickButton)
                {
                    this->controllerBinding.button00 =
                        this->controllerBinding.button02;
                }
                this->controllerBinding.button02 = joystickButton;
                break;

            case 1:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                if (this->controllerBinding.button06 == joystickButton)
                {
                    this->controllerBinding.button06 =
                        this->controllerBinding.button00;
                }
                if (this->controllerBinding.button02 == joystickButton)
                {
                    this->controllerBinding.button02 =
                        this->controllerBinding.button00;
                }
                this->controllerBinding.button00 = joystickButton;
                break;

            case 2:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                if (this->controllerBinding.button00 == joystickButton)
                {
                    this->controllerBinding.button00 =
                        this->controllerBinding.button06;
                }
                if (this->controllerBinding.button02 == joystickButton)
                {
                    this->controllerBinding.button02 =
                        this->controllerBinding.button06;
                }
                this->controllerBinding.button06 = joystickButton;
                break;
            }
            this->UpdateButtonSprites(
                0x74, this->controllerBinding.button00);
            this->UpdateButtonSprites(
                0x72, this->controllerBinding.button02);
            this->UpdateButtonSprites(
                0x76, this->controllerBinding.button06);
        }
        g_OptionsLastJoystickButton = joystickButton;

        if (GetOptionsPressedButtons(TH_BUTTON_SELECTMENU) == 0)
        {
            if (GetOptionsPressedButtons(TH_BUTTON_RETURNMENU) == 0)
            {
                return CHAIN_CALLBACK_RESULT_CONTINUE;
            }
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            if (this->cursor.GetCurrent() != 6)
            {
                return CHAIN_CALLBACK_RESULT_CONTINUE;
            }
        }
        else if (this->cursor.GetCurrent() != 6)
        {
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }

        this->cursor.Pop();
        this->requestedState = 1;
        this->state = 0;
        this->stateTimer.Reset();
        this->vmIds.SetInterrupt(0x68, 1);
        this->vmIds.SetInterrupt(0x69, 1);
        this->vmIds[0x66] = this->sceneAnm->CreateVm(0x66, 7);
        this->vmIds[0x67] = this->sceneAnm->CreateVm(0x67, 7);
        this->vmIds.SetInterrupt(0x19, 2);
        this->vmIds.SetInterrupt(0x1a, 2);
        this->transitionVm.SetInterrupt(2);
        this->vmIds.SetInterrupt(0x1b, 2);
        for (i = 0; i < 0x17; i++)
        {
            this->vmIds.SetInterrupt(i + 0x6b, 1);
        }
        this->vmIds.SetInterrupt(0x6a, 1);
        g_OptionsControllerMapping.primaryBinding = this->controllerBinding;
        g_OptionsGameConfig.controllerBinding = this->controllerBinding;
        g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
        if (this->savedWindowed != g_OptionsGameConfig.windowed)
        {
            return CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR;
        }
    }
    else
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        this->UpdateSelectionSprites();
    }

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

} // namespace th095
