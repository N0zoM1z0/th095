#include "OptionsMenu.hpp"
#include "Controller.hpp"
#include "FrontEndController.hpp"
#include "SceneSelect.hpp"
#include "Main.hpp"
#include "SoundPlayer.hpp"

namespace th095
{

static const i16 OPTIONS_JOYSTICK_BUTTON_COUNT = 32;

// Target 0x004A5894 starts at the sentinel immediately beyond the 0..31
// joystick-button range; Update replaces it after each poll.
i16 g_OptionsLastJoystickButton = OPTIONS_JOYSTICK_BUTTON_COUNT;

inline u16 GetOptionsPressedButtons(u16 buttons)
{
    return g_ControllerInputSlots[0].pressed & buttons;
}

inline u16 IsOptionsMenuInputPressed(u16 buttons)
{
    return (u16)((GetOptionsPressedButtons(buttons) != 0) ||
                 ((g_ControllerInputSlots[0].repeat & buttons) != 0));
}

static __forceinline void OptionsCreateFixedVm(FrontEndControllerView *menu, i32 scriptIndex)
{
    menu->vmIds[scriptIndex] = reinterpret_cast<SceneAnmLoadedView *>(menu->sceneAnm)->CreateVm(scriptIndex, 7);
}

static __forceinline void OptionsCreateInitialVm(
    FrontEndControllerView *menu, i32 scriptIndex)
{
    menu->vmIds[scriptIndex] = reinterpret_cast<SceneAnmLoadedView *>(menu->sceneAnm)->CreateVm(scriptIndex, 7);
}


__forceinline void FrontEndControllerView::SetOptionDigitSprite(i32 vmIndex, i32 digit)
{
    reinterpret_cast<SceneAnmLoadedView *>(this->sceneAnm)->SetSprite(
        OPTIONS_LOOKUP_VM(vmIndex),
        digit + 0x66);
}

__forceinline void FrontEndControllerView::UpdateButton00Sprites()
{
    struct ButtonDigitLocals
    {
        i32 onesSprite;
        i32 tensSprite;
    } digits;
    digits.tensSprite = this->optionsButtonMapping.shotButton / 10 + 0x66;
    reinterpret_cast<SceneAnmLoadedView *>(this->sceneAnm)->SetSprite(
        OPTIONS_LOOKUP_VM(0x74), digits.tensSprite);
    digits.onesSprite = this->optionsButtonMapping.shotButton % 10 + 0x66;
    reinterpret_cast<SceneAnmLoadedView *>(this->sceneAnm)->SetSprite(
        OPTIONS_LOOKUP_VM(0x75), digits.onesSprite);
}

__forceinline void FrontEndControllerView::UpdateButton02Sprites()
{
    struct ButtonDigitLocals
    {
        i32 onesSprite;
        i32 tensSprite;
    } digits;
    digits.tensSprite = this->optionsButtonMapping.bombButton / 10 + 0x66;
    reinterpret_cast<SceneAnmLoadedView *>(this->sceneAnm)->SetSprite(
        OPTIONS_LOOKUP_VM(0x72), digits.tensSprite);
    digits.onesSprite = this->optionsButtonMapping.bombButton % 10 + 0x66;
    reinterpret_cast<SceneAnmLoadedView *>(this->sceneAnm)->SetSprite(
        OPTIONS_LOOKUP_VM(0x73), digits.onesSprite);
}

__forceinline void FrontEndControllerView::UpdateButton06Sprites()
{
    struct ButtonDigitLocals
    {
        i32 onesSprite;
        i32 tensSprite;
    } digits;
    digits.tensSprite = this->optionsButtonMapping.menuButton / 10 + 0x66;
    reinterpret_cast<SceneAnmLoadedView *>(this->sceneAnm)->SetSprite(
        OPTIONS_LOOKUP_VM(0x76), digits.tensSprite);
    digits.onesSprite = this->optionsButtonMapping.menuButton % 10 + 0x66;
    reinterpret_cast<SceneAnmLoadedView *>(this->sceneAnm)->SetSprite(
        OPTIONS_LOOKUP_VM(0x77), digits.onesSprite);
}

__forceinline void FrontEndControllerView::UpdateBgmVolumeSprites(i32 value)
{
    if (value >= 100)
    {
        OPTIONS_SHOW_VM(0x7a);
        this->SetOptionDigitSprite(0x7a, (value / 100) % 10);
    }
    else
    {
        OPTIONS_HIDE_VM(0x7a);
    }
    if (value >= 10)
    {
        OPTIONS_SHOW_VM(0x7b);
        this->SetOptionDigitSprite(0x7b, (value / 10) % 10);
    }
    else
    {
        OPTIONS_HIDE_VM(0x7b);
    }
    this->SetOptionDigitSprite(0x7c, value % 10);
}

__forceinline void FrontEndControllerView::UpdateSfxVolumeSprites(i32 value)
{
    if (value >= 100)
    {
        OPTIONS_SHOW_VM(0x7e);
        this->SetOptionDigitSprite(0x7e, (value / 100) % 10);
    }
    else
    {
        OPTIONS_HIDE_VM(0x7e);
    }
    if (value >= 10)
    {
        OPTIONS_SHOW_VM(0x7f);
        this->SetOptionDigitSprite(0x7f, (value / 10) % 10);
    }
    else
    {
        OPTIONS_HIDE_VM(0x7f);
    }
    this->SetOptionDigitSprite(0x80, value % 10);
}

__forceinline void FrontEndControllerView::UpdateWindowModeSprites()
{
    if (g_Supervisor.config.windowed == 0)
    {
        this->vmIds.SetInterrupt(0x78, 4);
        this->vmIds.SetInterrupt(0x79, 5);
    }
    else
    {
        this->vmIds.SetInterrupt(0x78, 5);
        this->vmIds.SetInterrupt(0x79, 4);
    }
}

__forceinline void FrontEndControllerView::UpdateOptionsSelectionSprites()
{
    this->vmIds.SetInterrupt(0x6b, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BUTTON02_BINDING) + 2);
    this->vmIds.SetInterrupt(0x6c, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BUTTON00_BINDING) + 2);
    this->vmIds.SetInterrupt(0x6d, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BUTTON06_BINDING) + 2);
    this->vmIds.SetInterrupt(0x6e, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_WINDOW_MODE) + 2);
    this->vmIds.SetInterrupt(0x6f, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BGM_VOLUME) + 2);
    this->vmIds.SetInterrupt(0x70, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_SFX_VOLUME) + 2);
    this->vmIds.SetInterrupt(0x71, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_EXIT) + 2);
    this->vmIds.SetInterrupt(0x72, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BUTTON02_BINDING) + 2);
    this->vmIds.SetInterrupt(0x73, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BUTTON02_BINDING) + 2);
    this->vmIds.SetInterrupt(0x74, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BUTTON00_BINDING) + 2);
    this->vmIds.SetInterrupt(0x75, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BUTTON00_BINDING) + 2);
    this->vmIds.SetInterrupt(0x76, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BUTTON06_BINDING) + 2);
    this->vmIds.SetInterrupt(0x77, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BUTTON06_BINDING) + 2);
    this->vmIds.SetInterrupt(0x78, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_WINDOW_MODE) + 2);
    this->vmIds.SetInterrupt(0x79, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_WINDOW_MODE) + 2);
    this->vmIds.SetInterrupt(0x7a, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BGM_VOLUME) + 2);
    this->vmIds.SetInterrupt(0x7b, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BGM_VOLUME) + 2);
    this->vmIds.SetInterrupt(0x7c, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BGM_VOLUME) + 2);
    this->vmIds.SetInterrupt(0x7d, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_BGM_VOLUME) + 2);
    this->vmIds.SetInterrupt(0x7e, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_SFX_VOLUME) + 2);
    this->vmIds.SetInterrupt(0x7f, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_SFX_VOLUME) + 2);
    this->vmIds.SetInterrupt(0x80, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_SFX_VOLUME) + 2);
    this->vmIds.SetInterrupt(0x81, (this->groupCursor.GetCurrent() != OPTIONS_MENU_ITEM_SFX_VOLUME) + 2);
}

ChainCallbackResult FrontEndControllerView::UpdateOptionsMenu()
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
    case OPTIONS_MENU_STATE_INITIALIZE:
        g_Supervisor.StopReplayScan();
        FrontEndResetTimer(&this->stateTimer);
        this->groupCursor.Push();
        this->groupCursor.Set(OPTIONS_MENU_ITEM_BUTTON02_BINDING);
        this->groupCursor.count = OPTIONS_MENU_ITEM_COUNT;
        this->groupCursor.wraps = 1;
        this->state = OPTIONS_MENU_STATE_ACTIVE;

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

        this->optionsSavedWindowed = g_Supervisor.config.windowed;
        this->UpdateWindowModeSprites();
        this->optionsButtonMapping = g_Supervisor.config.controllerMapping.bindings[0];
        this->UpdateButton00Sprites();
        this->UpdateButton02Sprites();
        this->UpdateButton06Sprites();
        shallow.initialBgmVolume = g_Supervisor.config.musicVolume;
        this->UpdateBgmVolumeSprites(shallow.initialBgmVolume);
        shallow.initialSfxVolume = g_Supervisor.config.sfxVolume;
        this->UpdateSfxVolumeSprites(shallow.initialSfxVolume);
        this->flagBits.showRates = 0;

    case OPTIONS_MENU_STATE_ACTIVE:
    if (this->stateTimer < 30)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (this->stateTimer == 30)
    {
        this->UpdateOptionsSelectionSprites();
    }

    this->groupCursor.SaveCurrent();
    if (GetOptionsPressedButtons(TH_BUTTON_UP) != 0)
    {
        this->groupCursor.Move(-1);
    }
    if (GetOptionsPressedButtons(TH_BUTTON_DOWN) != 0)
    {
        this->groupCursor.Move(1);
    }

    if (this->groupCursor.HasChanged())
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        this->UpdateOptionsSelectionSprites();
    }
    else
    {
        if (IsOptionsMenuInputPressed(TH_BUTTON_LEFT))
        {
            switch (this->groupCursor.GetCurrent())
            {
            case OPTIONS_MENU_ITEM_WINDOW_MODE:
                g_Supervisor.config.windowed =
                    1 - g_Supervisor.config.windowed;
                this->UpdateWindowModeSprites();
                break;

            case OPTIONS_MENU_ITEM_BGM_VOLUME:
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

            case OPTIONS_MENU_ITEM_SFX_VOLUME:
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
            switch (this->groupCursor.GetCurrent())
            {
            case OPTIONS_MENU_ITEM_WINDOW_MODE:
                g_Supervisor.config.windowed =
                    1 - g_Supervisor.config.windowed;
                this->UpdateWindowModeSprites();
                break;

            case OPTIONS_MENU_ITEM_BGM_VOLUME:
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

            case OPTIONS_MENU_ITEM_SFX_VOLUME:
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
        if (this->groupCursor.GetCurrent() == OPTIONS_MENU_ITEM_SFX_VOLUME && this->animationTimer.current % 40 == 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_TAKE_PHOTO, 0);
        }

        shallow.joystickButtons = Controller::GetControllerState(0);
        for (shallow.joystickButton = 0;
             shallow.joystickButton < OPTIONS_JOYSTICK_BUTTON_COUNT;
             shallow.joystickButton++)
        {
            if ((shallow.joystickButtons[shallow.joystickButton] & 0x80) != 0)
            {
                break;
            }
        }
        if (shallow.joystickButton < OPTIONS_JOYSTICK_BUTTON_COUNT &&
            g_OptionsLastJoystickButton != shallow.joystickButton)
        {
            switch (this->groupCursor.GetCurrent())
            {
            case OPTIONS_MENU_ITEM_BUTTON02_BINDING:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                if (this->optionsButtonMapping.menuButton == shallow.joystickButton)
                {
                    this->optionsButtonMapping.menuButton =
                        this->optionsButtonMapping.bombButton;
                }
                if (this->optionsButtonMapping.shotButton == shallow.joystickButton)
                {
                    this->optionsButtonMapping.shotButton =
                        this->optionsButtonMapping.bombButton;
                }
                this->optionsButtonMapping.bombButton = shallow.joystickButton;
                break;

            case OPTIONS_MENU_ITEM_BUTTON00_BINDING:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                if (this->optionsButtonMapping.menuButton == shallow.joystickButton)
                {
                    this->optionsButtonMapping.menuButton =
                        this->optionsButtonMapping.shotButton;
                }
                if (this->optionsButtonMapping.bombButton == shallow.joystickButton)
                {
                    this->optionsButtonMapping.bombButton =
                        this->optionsButtonMapping.shotButton;
                }
                this->optionsButtonMapping.shotButton = shallow.joystickButton;
                break;

            case OPTIONS_MENU_ITEM_BUTTON06_BINDING:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                if (this->optionsButtonMapping.shotButton == shallow.joystickButton)
                {
                    this->optionsButtonMapping.shotButton =
                        this->optionsButtonMapping.menuButton;
                }
                if (this->optionsButtonMapping.bombButton == shallow.joystickButton)
                {
                    this->optionsButtonMapping.bombButton =
                        this->optionsButtonMapping.menuButton;
                }
                this->optionsButtonMapping.menuButton = shallow.joystickButton;
                break;
            }
            this->UpdateButton00Sprites();
            this->UpdateButton02Sprites();
            this->UpdateButton06Sprites();
        }
        g_OptionsLastJoystickButton = shallow.joystickButton;

        if (GetOptionsPressedButtons(TH_BUTTON_ENTER | TH_BUTTON_BOMB) != 0)
        {
            if (this->groupCursor.GetCurrent() == OPTIONS_MENU_ITEM_EXIT)
            {
options_finish:
            this->groupCursor.Pop();
            this->requestedState = FRONT_END_REQUESTED_STATE_MAIN_MENU;
            this->state = OPTIONS_MENU_STATE_INITIALIZE;
            FrontEndResetTimer(&this->stateTimer);
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
            g_Supervisor.config.controllerMapping.bindings[0] = this->optionsButtonMapping;
            g_ControllerInputSlots[0].mappings[0] = g_Supervisor.config.controllerMapping.bindings[0];
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            if (this->optionsSavedWindowed != g_Supervisor.config.windowed)
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
                if (this->groupCursor.GetCurrent() == OPTIONS_MENU_ITEM_EXIT)
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

#undef OPTIONS_HIDE_VM
#undef OPTIONS_SHOW_VM
#undef OPTIONS_LOOKUP_VM

} // namespace th095
