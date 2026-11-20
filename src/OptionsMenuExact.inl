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

static __forceinline void OptionsCreateFixedVm(OptionsMenuView *menu, i32 scriptIndex)
{
    menu->vmIds[scriptIndex] = menu->sceneAnm->CreateVm(scriptIndex, 7);
}

static __forceinline void OptionsCreateInitialVm(
    OptionsMenuView *menu, i32 scriptIndex)
{
    menu->vmIds[scriptIndex] = menu->sceneAnm->CreateVm(scriptIndex, 7);
}

ChainCallbackResult OptionsMenuView::Update()
{
    struct ShallowOptionLocals
    {
        i32 teardownIndex;
        u8 *joystickButtons;
        i16 joystickButton;
        i32 rightSfxVolume;
        i32 rightBgmVolume;
        i32 leftSfxVolume;
        i32 leftBgmVolume;
        i32 initialSfxVolume;
        i32 initialBgmVolume;
        i32 initialIndex;
    } shallow;


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

        OptionsCreateInitialVm(this, 0x68);
        OptionsCreateInitialVm(this, 0x69);
        this->vmIds.SetInterrupt(0x19, 3);
        this->vmIds.SetInterrupt(0x1a, 3);
        this->transitionVm.SetInterrupt(3);
        this->vmIds.SetInterrupt(0x1b, 3);
        OptionsCreateInitialVm(this, 0x6a);
        for (shallow.initialIndex = 0; shallow.initialIndex < 0x17;
             shallow.initialIndex++)
        {
            OptionsCreateInitialVm(this, shallow.initialIndex + 0x6b);
        }

        this->savedWindowed = g_Supervisor.config.windowed;
        this->UpdateWindowModeSprites();
        this->controllerBinding = g_Supervisor.config.controllerMapping.bindings[0];
        this->UpdateButton00Sprites();
        this->UpdateButton02Sprites();
        this->UpdateButton06Sprites();
        shallow.initialBgmVolume = g_Supervisor.config.musicVolume;
        this->UpdateBgmVolumeSprites(shallow.initialBgmVolume);
        shallow.initialSfxVolume = g_Supervisor.config.sfxVolume;
        this->UpdateSfxVolumeSprites(shallow.initialSfxVolume);
        this->outerFlags &= ~8u;

    case 1:
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

    if (this->cursor.HasChanged())
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        this->UpdateSelectionSprites();
    }
    else
    {
        if (IsOptionsMenuInputPressed(TH_BUTTON_LEFT))
        {
            switch (this->cursor.GetCurrent())
            {
            case 3:
                g_Supervisor.config.windowed =
                    1 - g_Supervisor.config.windowed;
                this->UpdateWindowModeSprites();
                break;

            case 4:
                if (g_Supervisor.config.musicVolume > 0)
                {
                    g_Supervisor.config.musicVolume -= 5;
                    if (g_Supervisor.config.musicVolume < 0)
                    {
                        g_Supervisor.config.musicVolume = 0;
                    }
                }
                shallow.leftBgmVolume = g_Supervisor.config.musicVolume;
                this->UpdateBgmVolumeSprites(shallow.leftBgmVolume);
                break;

            case 5:
                if (g_Supervisor.config.sfxVolume > 0)
                {
                    g_Supervisor.config.sfxVolume -= 5;
                    if (g_Supervisor.config.sfxVolume < 0)
                    {
                        g_Supervisor.config.sfxVolume = 0;
                    }
                }
                shallow.leftSfxVolume = g_Supervisor.config.sfxVolume;
                this->UpdateSfxVolumeSprites(shallow.leftSfxVolume);
                break;
            }
        }

        if (IsOptionsMenuInputPressed(TH_BUTTON_RIGHT))
        {
            switch (this->cursor.GetCurrent())
            {
            case 3:
                g_Supervisor.config.windowed =
                    1 - g_Supervisor.config.windowed;
                this->UpdateWindowModeSprites();
                break;

            case 4:
                if (g_Supervisor.config.musicVolume < 100)
                {
                    g_Supervisor.config.musicVolume += 5;
                    if (g_Supervisor.config.musicVolume > 100)
                    {
                        g_Supervisor.config.musicVolume = 100;
                    }
                }
                shallow.rightBgmVolume = g_Supervisor.config.musicVolume;
                this->UpdateBgmVolumeSprites(shallow.rightBgmVolume);
                break;

            case 5:
                if (g_Supervisor.config.sfxVolume < 100)
                {
                    g_Supervisor.config.sfxVolume += 5;
                    if (g_Supervisor.config.sfxVolume > 100)
                    {
                        g_Supervisor.config.sfxVolume = 100;
                    }
                }
                shallow.rightSfxVolume = g_Supervisor.config.sfxVolume;
                this->UpdateSfxVolumeSprites(shallow.rightSfxVolume);
                break;
            }
        }

        g_SoundPlayer.bgmVolume = g_Supervisor.config.musicVolume;
        g_SoundPlayer.sfxVolume = g_Supervisor.config.sfxVolume;
        if (this->cursor.GetCurrent() == 5 && this->frameCounter % 40 == 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_TAKE_PHOTO, 0);
        }

        shallow.joystickButtons = Controller::GetControllerState(0);
        for (shallow.joystickButton = 0;
             shallow.joystickButton < 0x20;
             shallow.joystickButton++)
        {
            if ((shallow.joystickButtons[shallow.joystickButton] & 0x80) != 0)
            {
                break;
            }
        }
        if (shallow.joystickButton < 0x20 &&
            g_OptionsLastJoystickButton != shallow.joystickButton)
        {
            switch (this->cursor.GetCurrent())
            {
            case 0:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                if (this->controllerBinding.menuButton == shallow.joystickButton)
                {
                    this->controllerBinding.menuButton =
                        this->controllerBinding.bombButton;
                }
                if (this->controllerBinding.shotButton == shallow.joystickButton)
                {
                    this->controllerBinding.shotButton =
                        this->controllerBinding.bombButton;
                }
                this->controllerBinding.bombButton = shallow.joystickButton;
                break;

            case 1:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                if (this->controllerBinding.menuButton == shallow.joystickButton)
                {
                    this->controllerBinding.menuButton =
                        this->controllerBinding.shotButton;
                }
                if (this->controllerBinding.bombButton == shallow.joystickButton)
                {
                    this->controllerBinding.bombButton =
                        this->controllerBinding.shotButton;
                }
                this->controllerBinding.shotButton = shallow.joystickButton;
                break;

            case 2:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                if (this->controllerBinding.shotButton == shallow.joystickButton)
                {
                    this->controllerBinding.shotButton =
                        this->controllerBinding.menuButton;
                }
                if (this->controllerBinding.bombButton == shallow.joystickButton)
                {
                    this->controllerBinding.bombButton =
                        this->controllerBinding.menuButton;
                }
                this->controllerBinding.menuButton = shallow.joystickButton;
                break;
            }
            this->UpdateButton00Sprites();
            this->UpdateButton02Sprites();
            this->UpdateButton06Sprites();
        }
        g_OptionsLastJoystickButton = shallow.joystickButton;

        if (GetOptionsPressedButtons(TH_BUTTON_ENTER | TH_BUTTON_BOMB) != 0)
        {
            if (this->cursor.GetCurrent() == 6)
            {
options_finish:
            this->cursor.Pop();
            this->requestedState = 1;
            this->state = 0;
            this->stateTimer.Reset();
            this->vmIds.SetInterrupt(0x68, 1);
            this->vmIds.SetInterrupt(0x69, 1);
            OptionsCreateFixedVm(this, 0x66);
            OptionsCreateFixedVm(this, 0x67);
            this->vmIds.SetInterrupt(0x19, 2);
            this->vmIds.SetInterrupt(0x1a, 2);
            this->transitionVm.SetInterrupt(2);
            this->vmIds.SetInterrupt(0x1b, 2);
            for (shallow.teardownIndex = 0;
                 shallow.teardownIndex < 0x17;
                 shallow.teardownIndex++)
            {
                this->vmIds.SetInterrupt(shallow.teardownIndex + 0x6b, 1);
            }
            this->vmIds.SetInterrupt(0x6a, 1);
            g_Supervisor.config.controllerMapping.bindings[0] = this->controllerBinding;
            g_ControllerInputSlots[0].mappings[0] = g_Supervisor.config.controllerMapping.bindings[0];
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            if (this->savedWindowed != g_Supervisor.config.windowed)
            {
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR;
            }
                break;
            }
            else
            {
                return CHAIN_CALLBACK_RESULT_CONTINUE;
            }
        }
        else
        {
            if (GetOptionsPressedButtons(TH_BUTTON_MENU | TH_BUTTON_SHOOT) != 0)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                if (this->cursor.GetCurrent() == 6)
                {
                    goto options_finish;
                }
                return CHAIN_CALLBACK_RESULT_CONTINUE;
            }
            break;
        }
    }

    }

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

} // namespace th095
