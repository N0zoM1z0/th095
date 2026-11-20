#ifdef TH095_MATCH_EXACT
#include "OptionsMenuExact.inl"
#else
#include "OptionsMenu.hpp"
#include "Controller.hpp"
#include "Global.hpp"
#include "InputRuntime.hpp"
#include "Main.hpp"
#include "SoundPlayer.hpp"

namespace th095
{

#ifdef DIFFBUILD
#define TH095_OPTIONS_MENU_STATE_INITIALIZE 0
#define TH095_OPTIONS_MENU_STATE_ACTIVE 1
#else
#define TH095_OPTIONS_MENU_STATE_INITIALIZE OPTIONS_MENU_STATE_INITIALIZE
#define TH095_OPTIONS_MENU_STATE_ACTIVE OPTIONS_MENU_STATE_ACTIVE
#endif

#ifndef DIFFBUILD
// Target 0x004A5894 starts at the sentinel immediately beyond the 0..31
// joystick-button range; Update replaces it after each poll.
i16 g_OptionsLastJoystickButton = 0x20;
#endif

extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;
#define g_ResultMenuInput (RuntimeResultMenuInput())
#define g_PressedButtons (RuntimePressedButtons())

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
    case TH095_OPTIONS_MENU_STATE_INITIALIZE:
        g_Supervisor.StopReplayScan();
        this->stateTimer.Reset();
        this->cursor.Push();
        this->cursor.Set(OPTIONS_MENU_ITEM_BUTTON02_BINDING);
        this->cursor.count = OPTIONS_MENU_ITEM_COUNT;
        this->cursor.wraps = 1;
        this->state = TH095_OPTIONS_MENU_STATE_ACTIVE;

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
        this->UpdateWindowModeSprites(g_Supervisor.config.windowed);
        this->controllerBinding = g_Supervisor.config.controllerMapping.bindings[0];
        this->UpdateButton00Sprites();
        this->UpdateButton02Sprites();
        this->UpdateButton06Sprites();
        shallow.initialBgmVolume = g_Supervisor.config.musicVolume;
        this->UpdateBgmVolumeSprites(shallow.initialBgmVolume);
        shallow.initialSfxVolume = g_Supervisor.config.sfxVolume;
        this->UpdateSfxVolumeSprites(shallow.initialSfxVolume);
        this->outerFlags &= ~8u;

    case TH095_OPTIONS_MENU_STATE_ACTIVE:
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
            case OPTIONS_MENU_ITEM_WINDOW_MODE:
                g_Supervisor.config.windowed =
                    1 - g_Supervisor.config.windowed;
                this->UpdateWindowModeSprites(g_Supervisor.config.windowed);
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
            switch (this->cursor.GetCurrent())
            {
            case OPTIONS_MENU_ITEM_WINDOW_MODE:
                g_Supervisor.config.windowed =
                    1 - g_Supervisor.config.windowed;
                this->UpdateWindowModeSprites(g_Supervisor.config.windowed);
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
        if (this->cursor.GetCurrent() == OPTIONS_MENU_ITEM_SFX_VOLUME && this->animationTimer.current % 40 == 0)
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
            case OPTIONS_MENU_ITEM_BUTTON02_BINDING:
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

            case OPTIONS_MENU_ITEM_BUTTON00_BINDING:
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

            case OPTIONS_MENU_ITEM_BUTTON06_BINDING:
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
            if (this->cursor.GetCurrent() == OPTIONS_MENU_ITEM_EXIT)
            {
options_finish:
            this->cursor.Pop();
            this->requestedState = FRONT_END_REQUESTED_STATE_MAIN_MENU;
            this->state = TH095_OPTIONS_MENU_STATE_INITIALIZE;
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
                if (this->cursor.GetCurrent() == OPTIONS_MENU_ITEM_EXIT)
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

#endif // TH095_MATCH_EXACT
