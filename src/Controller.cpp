#ifdef TH095_MATCH_EXACT
#define TH095_MATCH_GAME_ERROR_CONTEXT_AS_CLASS
#endif
#include "Controller.hpp"
#include "Main.hpp"

#include <mmsystem.h>
#include <string.h>

namespace th095
{

#ifndef TH095_MATCH_EXACT
DIFFABLE_STATIC_ARRAY(u8, 128, g_ControllerButtons);
DIFFABLE_STATIC_ARRAY(JOYCAPSA, 2, g_JoystickCaps);
DIFFABLE_STATIC_ASSIGN(i32, g_ControllerInputEnabled) = 1;
DIFFABLE_STATIC_ARRAY(ControllerInputSlotView, 3, g_ControllerInputSlots);
#endif

#ifdef TH095_MATCH_EXACT
#define TH095_CONTROLLER_INPUT_SLOT(index) ((&g_ControllerInputSlots) + (index))
#define TH095_CONTROLLER_JOYCAPS_FIRST g_JoystickCaps
#define TH095_CONTROLLER_JOYCAP(index) ((&g_JoystickCaps)[index])
#define TH095_CONTROLLER_DEVICE(index) ((&g_ControllerDevices)[index])
#define TH095_CONTROLLER_RUNTIME_FLAGS g_ControllerRuntimeFlags
#define TH095_CONTROLLER_PAD_X g_ControllerPadXAxis
#define TH095_CONTROLLER_PAD_Y g_ControllerPadYAxis
#define TH095_CONTROLLER_BUTTONS_PTR (&g_ControllerButtons)
#define TH095_CONTROLLER_BUTTON(index) ((&g_ControllerButtons)[index])
#define TH095_CONTROLLER_KEYBOARD_DEVICE g_KeyboardDevice
#define TH095_CONTROLLER_ASSIGNMENT(index) ((&g_ControllerAssignments)[index])
#else
#define TH095_CONTROLLER_INPUT_SLOT(index) (g_ControllerInputSlots + (index))
#define TH095_CONTROLLER_JOYCAPS_FIRST g_JoystickCaps[0]
#define TH095_CONTROLLER_JOYCAP(index) (g_JoystickCaps[index])
#define TH095_CONTROLLER_DEVICE(index) (g_Supervisor.controller)
#define TH095_CONTROLLER_RUNTIME_FLAGS g_Supervisor.flags.raw
#define TH095_CONTROLLER_PAD_X g_Supervisor.config.padXAxis
#define TH095_CONTROLLER_PAD_Y g_Supervisor.config.padYAxis
#define TH095_CONTROLLER_BUTTONS_PTR (g_ControllerButtons)
#define TH095_CONTROLLER_BUTTON(index) (g_ControllerButtons[index])
#define TH095_CONTROLLER_KEYBOARD_DEVICE g_Supervisor.keyboard
// GetInput's only assignment reads target 0x004C483E/0x004C483F, which are
// Supervisor::config +0xB2/+0xB3.  GameConfiguration::Initialize supplies the
// target defaults 0/1/2 there.  A former production-only array initialized to
// 0/0 split these reads from the configuration owner and made both logical
// controllers select device zero.  Exact probes retain their historical
// g_ControllerAssignments relocation spelling above.
#define TH095_CONTROLLER_ASSIGNMENT(index) \
    (g_Supervisor.config.controllerAssignments[index])
#endif

ControllerInputSlotView::ControllerInputSlotView()
{
    this->mappings[0].shotButton = 1;
    this->mappings[0].bombButton = 0;
    this->mappings[0].focusButton = 9;
    this->mappings[0].menuButton = 2;
    this->mappings[0].upButton = -1;
    this->mappings[0].downButton = -1;
    this->mappings[0].leftButton = -1;
    this->mappings[0].rightButton = -1;
    this->mappings[0].skipButton = 1;

    this->mappings[1].shotButton = 0x5a;
    this->mappings[1].bombButton = 0x58;
    this->mappings[1].focusButton = 0x10;
    this->mappings[1].menuButton = 0x1b;
    this->mappings[1].upButton = 0x26;
    this->mappings[1].downButton = 0x28;
    this->mappings[1].leftButton = 0x25;
    this->mappings[1].rightButton = 0x27;
    this->mappings[1].skipButton = 0x11;

    this->mappings[2].shotButton = 0x2c;
    this->mappings[2].bombButton = 0x2d;
    this->mappings[2].focusButton = 0x2a;
    this->mappings[2].menuButton = 1;
    this->mappings[2].upButton = 0xc8;
    this->mappings[2].downButton = 0xd0;
    this->mappings[2].leftButton = 0xcb;
    this->mappings[2].rightButton = 0xcd;
    this->mappings[2].skipButton = 0x1d;
}

struct ControllerStateLocals
{
    DIJOYSTATE2 joystickState;
    HRESULT result;
    u32 buttonIndex;
    u32 buttons;
    JOYINFOEX joystickInfo;
};

typedef char ControllerStateLocalsSizeIs150[
    (sizeof(ControllerStateLocals) == 0x150) ? 1 : -1];
typedef char ControllerStateJoystickAt00[
    (offsetof(ControllerStateLocals, joystickState) == 0) ? 1 : -1];
typedef char ControllerStateResultAt110[
    (offsetof(ControllerStateLocals, result) == 0x110) ? 1 : -1];
typedef char ControllerStateJoystickInfoAt11C[
    (offsetof(ControllerStateLocals, joystickInfo) == 0x11c) ? 1 : -1];

struct ControllerInputLocals
{
    DIJOYSTATE2 joystickState;
    u32 joystickShootPressed;
    HRESULT result;
    u32 directInputShootPressed;
    u32 axisDeadzone;
    JOYINFOEX joystickInfo;
    ControllerInputSlotView *inputSlot;
};

typedef char ControllerInputSlotSizeIs8E[
    (sizeof(ControllerInputSlotView) == 0x8e) ? 1 : -1];
typedef char ControllerInputSlotButtonsAt58[
    (offsetof(ControllerInputSlotView, mappings) == 0x58 &&
     offsetof(ControllerButtonMapping, shotButton) == 0x0 &&
     offsetof(ControllerButtonMapping, bombButton) == 0x2 &&
     offsetof(ControllerButtonMapping, menuButton) == 0x6) ? 1 : -1];
typedef char ControllerInputLocalsSizeIs158[
    (sizeof(ControllerInputLocals) == 0x158) ? 1 : -1];
typedef char ControllerInputStateAt00[
    (offsetof(ControllerInputLocals, joystickState) == 0) ? 1 : -1];
typedef char ControllerInputResultAt114[
    (offsetof(ControllerInputLocals, result) == 0x114) ? 1 : -1];
typedef char ControllerInputInfoAt120[
    (offsetof(ControllerInputLocals, joystickInfo) == 0x120) ? 1 : -1];
typedef char ControllerInputSlotAt154[
    (offsetof(ControllerInputLocals, inputSlot) == 0x154) ? 1 : -1];

namespace Controller
{

u16 GetJoystickCaps()
{
    JOYINFOEX joystickInfo;

    joystickInfo.dwSize = sizeof(joystickInfo);
    joystickInfo.dwFlags = JOY_RETURNALL;
    if (joyGetPosEx(0, &joystickInfo) != JOYERR_NOERROR &&
        joyGetPosEx(1, &joystickInfo) != JOYERR_NOERROR)
    {
        g_GameErrorContext.Log(
            "\x8e\x67\x82\xa6\x82\xe9\x83\x70\x83\x62\x83\x68\x82\xaa"
            "\x91\xb6\x8d\xdd\x82\xb5\x82\xc8\x82\xa2\x82\xe6\x82\xa4"
            "\x82\xc5\x82\xb7\x81\x41\x8e\x63\x94\x4f\r\n");
        return 1;
    }

    joyGetDevCapsA(0, &TH095_CONTROLLER_JOYCAPS_FIRST, sizeof(TH095_CONTROLLER_JOYCAPS_FIRST));
    return 0;
}

u32 SetButtonFromDirectInputJoystate(u16 *outButtons,
                                     i16 controllerButtonToTest,
                                     u16 touhouButton, u8 *inputButtons)
{
    if (controllerButtonToTest < 0)
    {
        return 0;
    }

    *outButtons |= inputButtons[controllerButtonToTest] & 0x80
                       ? touhouButton
                       : 0;
    return inputButtons[controllerButtonToTest] & 0x80 ? touhouButton : 0;
}

u32 SetButtonFromControllerInputs(u16 *outButtons,
                                  i16 controllerButtonToTest,
                                  u16 touhouButton, u32 inputButtons)
{
    u32 mask;

    if (controllerButtonToTest < 0)
    {
        return 0;
    }

    mask = 1 << controllerButtonToTest;
    *outButtons |= inputButtons & mask ? touhouButton : 0;
    return inputButtons & mask ? touhouButton : 0;
}

u16 GetControllerInput(i32 controllerIndex, i32 joystickIndex, u16 buttons)
{
    i32 acquireAttempts;
    ControllerInputLocals locals;

    locals.inputSlot = TH095_CONTROLLER_INPUT_SLOT(controllerIndex);
    if (((TH095_CONTROLLER_RUNTIME_FLAGS >> 11) & 1) == 0)
    {
        memset(&locals.joystickInfo, 0, sizeof(locals.joystickInfo));
        locals.joystickInfo.dwSize = sizeof(locals.joystickInfo);
        locals.joystickInfo.dwFlags = JOY_RETURNALL;
        if (joyGetPosEx(joystickIndex != 0, &locals.joystickInfo) !=
            JOYERR_NOERROR)
        {
            return buttons;
        }

        SetButtonFromControllerInputs(
            &buttons, locals.inputSlot->mappings[0].shotButton,
            TH_BUTTON_SHOOT, locals.joystickInfo.dwButtons);
        SetButtonFromControllerInputs(
            &buttons, locals.inputSlot->mappings[0].bombButton,
            TH_BUTTON_BOMB, locals.joystickInfo.dwButtons);
        SetButtonFromControllerInputs(
            &buttons, locals.inputSlot->mappings[0].menuButton,
            TH_BUTTON_MENU, locals.joystickInfo.dwButtons);

        locals.axisDeadzone =
            (TH095_CONTROLLER_JOYCAP(joystickIndex).wXmax -
             TH095_CONTROLLER_JOYCAP(joystickIndex).wXmin) /
            2 / 2;
        buttons |= locals.joystickInfo.dwXpos >
                           ((TH095_CONTROLLER_JOYCAP(joystickIndex).wXmin +
                             TH095_CONTROLLER_JOYCAP(joystickIndex).wXmax) /
                                2 +
                            locals.axisDeadzone)
                       ? TH_BUTTON_RIGHT
                       : 0;
        buttons |= locals.joystickInfo.dwXpos <
                           ((TH095_CONTROLLER_JOYCAP(joystickIndex).wXmin +
                             TH095_CONTROLLER_JOYCAP(joystickIndex).wXmax) /
                                2 -
                            locals.axisDeadzone)
                       ? TH_BUTTON_LEFT
                       : 0;

        locals.axisDeadzone =
            (TH095_CONTROLLER_JOYCAP(joystickIndex).wYmax -
             TH095_CONTROLLER_JOYCAP(joystickIndex).wYmin) /
            2 / 2;
        buttons |= locals.joystickInfo.dwYpos >
                           ((TH095_CONTROLLER_JOYCAP(joystickIndex).wYmin +
                             TH095_CONTROLLER_JOYCAP(joystickIndex).wYmax) /
                                2 +
                            locals.axisDeadzone)
                       ? TH_BUTTON_DOWN
                       : 0;
        buttons |= locals.joystickInfo.dwYpos <
                           ((TH095_CONTROLLER_JOYCAP(joystickIndex).wYmin +
                             TH095_CONTROLLER_JOYCAP(joystickIndex).wYmax) /
                                2 -
                            locals.axisDeadzone)
                       ? TH_BUTTON_UP
                       : 0;
        return buttons;
    }

    locals.result = TH095_CONTROLLER_DEVICE(joystickIndex)->Poll();
    if (locals.result < 0)
    {
        acquireAttempts = 0;
        utils::DebugPrint("error : DIERR_INPUTLOST\r\n");
        locals.result = TH095_CONTROLLER_DEVICE(joystickIndex)->Acquire();
        while (locals.result == DIERR_INPUTLOST)
        {
            locals.result = TH095_CONTROLLER_DEVICE(joystickIndex)->Acquire();
            utils::DebugPrint(
                "error : DIERR_INPUTLOST %d\r\n", acquireAttempts);
            acquireAttempts++;
            if (acquireAttempts >= 400)
            {
                return buttons;
            }
        }
        return buttons;
    }

    memset(&locals.joystickState, 0, sizeof(locals.joystickState));
    locals.result = TH095_CONTROLLER_DEVICE(joystickIndex)->GetDeviceState(
        sizeof(locals.joystickState), &locals.joystickState);
    if (locals.result < 0)
    {
        return buttons;
    }

    locals.directInputShootPressed = SetButtonFromDirectInputJoystate(
        &buttons, locals.inputSlot->mappings[0].shotButton,
        TH_BUTTON_SHOOT, locals.joystickState.rgbButtons);
    SetButtonFromDirectInputJoystate(
        &buttons, locals.inputSlot->mappings[0].bombButton,
        TH_BUTTON_BOMB, locals.joystickState.rgbButtons);
    SetButtonFromDirectInputJoystate(
        &buttons, locals.inputSlot->mappings[0].menuButton,
        TH_BUTTON_MENU, locals.joystickState.rgbButtons);

    buttons |= locals.joystickState.lX > TH095_CONTROLLER_PAD_X
                   ? TH_BUTTON_RIGHT
                   : 0;
    buttons |= locals.joystickState.lX < -TH095_CONTROLLER_PAD_X
                   ? TH_BUTTON_LEFT
                   : 0;
    buttons |= locals.joystickState.lY > TH095_CONTROLLER_PAD_Y
                   ? TH_BUTTON_DOWN
                   : 0;
    buttons |= locals.joystickState.lY < -TH095_CONTROLLER_PAD_Y
                   ? TH_BUTTON_UP
                   : 0;
    return buttons;
}

u8 *GetControllerState(i32 deviceIndex)
{
    i32 acquireAttempts;
    ControllerStateLocals locals;

    memset(TH095_CONTROLLER_BUTTONS_PTR, 0, 128);
    if (((TH095_CONTROLLER_RUNTIME_FLAGS >> 11) & 1) == 0)
    {
        memset(&locals.joystickInfo, 0, sizeof(locals.joystickInfo));
        locals.joystickInfo.dwSize = sizeof(locals.joystickInfo);
        locals.joystickInfo.dwFlags = JOY_RETURNALL;
        if (joyGetPosEx(0, &locals.joystickInfo) != JOYERR_NOERROR)
        {
            return TH095_CONTROLLER_BUTTONS_PTR;
        }

        locals.buttons = locals.joystickInfo.dwButtons;
        for (locals.buttonIndex = 0; locals.buttonIndex < 32;
             locals.buttonIndex++, locals.buttons >>= 1)
        {
            if ((locals.buttons & 1) != 0)
            {
                TH095_CONTROLLER_BUTTON(locals.buttonIndex) = 0x80;
            }
        }
        return TH095_CONTROLLER_BUTTONS_PTR;
    }

    locals.result = TH095_CONTROLLER_DEVICE(deviceIndex)->Poll();
    if (locals.result < 0)
    {
        acquireAttempts = 0;
        utils::DebugPrint("error : DIERR_INPUTLOST\r\n");
        locals.result = TH095_CONTROLLER_DEVICE(deviceIndex)->Acquire();
        while (locals.result == DIERR_INPUTLOST)
        {
            locals.result = TH095_CONTROLLER_DEVICE(deviceIndex)->Acquire();
            acquireAttempts++;
            if (acquireAttempts >= 400)
            {
                utils::DebugPrint(
                    "error : DIERR_INPUTLOST %d\r\n", acquireAttempts);
                return TH095_CONTROLLER_BUTTONS_PTR;
            }
        }

        return TH095_CONTROLLER_BUTTONS_PTR;
    }

    TH095_CONTROLLER_DEVICE(deviceIndex)->GetDeviceState(
        sizeof(locals.joystickState), &locals.joystickState);
    if (locals.result < 0)
    {
        return TH095_CONTROLLER_BUTTONS_PTR;
    }
    memcpy(TH095_CONTROLLER_BUTTONS_PTR, locals.joystickState.rgbButtons, 128);
    return TH095_CONTROLLER_BUTTONS_PTR;
}

#define KEYBOARD_KEY_PRESSED(button, key) \
    keyboardState[key] & 0x80 ? button : 0

// Stock VC7.1 ranks these six real locals by identifier hash rather than source
// declaration order.  Keep readable semantic aliases while using the
// target-proven backing buckets; this gives GetInput all 212 observed EBP homes
// exactly without changing the input logic or adding storage.
#define inputButtons repeatMask
#define inputStateSlot buttons
#define inputResult bitIndex
#define inputBitIndex currentBits
#define inputCurrentBits result
#define inputRepeatMask inputSlot
u16 GetInput(i32 inputIndex)
{
    u8 keyboardState[256];
    u16 inputButtons;
    ControllerInputSlotView *inputStateSlot;
    HRESULT inputResult;
    i32 inputBitIndex;
    u16 inputCurrentBits;
    u16 inputRepeatMask;

    inputButtons = 0;
    inputStateSlot = TH095_CONTROLLER_INPUT_SLOT(inputIndex);
    if (g_ControllerInputEnabled != 0)
    {
        if (((TH095_CONTROLLER_RUNTIME_FLAGS >> 10) & 1) == 0)
        {
            GetKeyboardState(keyboardState);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, VK_UP);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, VK_DOWN);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, VK_LEFT);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, VK_RIGHT);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, 'Z');
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, 'X');
            inputButtons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_SHOOT | TH_BUTTON_FOCUS, VK_SHIFT);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, VK_NUMPAD8);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, VK_NUMPAD2);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, VK_NUMPAD4);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, VK_NUMPAD6);
            inputButtons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_UP_LEFT, VK_NUMPAD7);
            inputButtons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_UP_RIGHT, VK_NUMPAD9);
            inputButtons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_DOWN_LEFT, VK_NUMPAD1);
            inputButtons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_DOWN_RIGHT, VK_NUMPAD3);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SKIP, VK_CONTROL);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_MENU, VK_ESCAPE);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_ENTER, VK_RETURN);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, VK_HOME);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, 'P');
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_D, 'D');
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_Q, 'Q');
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_S, 'S');
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RESET, 'R');
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_L, 'L');
        }
        else
        {
            inputResult = TH095_CONTROLLER_KEYBOARD_DEVICE->GetDeviceState(
                sizeof(keyboardState), keyboardState);
            inputButtons = 0;
            if (inputResult == DIERR_INPUTLOST)
            {
                TH095_CONTROLLER_KEYBOARD_DEVICE->Acquire();
            }
            else if (inputResult != S_OK)
            {
                TH095_CONTROLLER_KEYBOARD_DEVICE->Acquire();
            }
            else
            {
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_UP);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_DOWN);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_LEFT);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_RIGHT);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, DIK_Z);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, DIK_X);
                inputButtons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_SHOOT | TH_BUTTON_FOCUS, DIK_LSHIFT);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_NUMPAD8);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_NUMPAD2);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_NUMPAD4);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_NUMPAD6);
                inputButtons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_UP_LEFT, DIK_NUMPAD7);
                inputButtons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_UP_RIGHT, DIK_NUMPAD9);
                inputButtons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_DOWN_LEFT, DIK_NUMPAD1);
                inputButtons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_DOWN_RIGHT, DIK_NUMPAD3);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SKIP, DIK_LCONTROL);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_MENU, DIK_ESCAPE);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_ENTER, DIK_RETURN);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, DIK_HOME);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, DIK_P);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_D, DIK_D);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_Q, DIK_Q);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_S, DIK_S);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RESET, DIK_R);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_L, DIK_L);
            }
        }
    }

    if (inputIndex >= 2)
    {
        if (TH095_CONTROLLER_ASSIGNMENT(0) == 0 ||
            TH095_CONTROLLER_ASSIGNMENT(0) == 1)
        {
            inputButtons = GetControllerInput(
                0, TH095_CONTROLLER_ASSIGNMENT(0) != 0, inputButtons);
        }
        if (TH095_CONTROLLER_ASSIGNMENT(1) == 0 ||
            TH095_CONTROLLER_ASSIGNMENT(1) == 1)
        {
            inputButtons = GetControllerInput(
                1, TH095_CONTROLLER_ASSIGNMENT(1) != 0, inputButtons);
        }
    }
    else if (TH095_CONTROLLER_ASSIGNMENT(inputIndex) == 0 ||
             TH095_CONTROLLER_ASSIGNMENT(inputIndex) == 1)
    {
        inputButtons = GetControllerInput(
            inputIndex, TH095_CONTROLLER_ASSIGNMENT(inputIndex) != 0,
            inputButtons);
    }

    inputStateSlot->previous = inputStateSlot->current;
    inputStateSlot->current = inputButtons;
    inputRepeatMask = 1;
    inputCurrentBits = inputButtons;
    inputStateSlot->repeat = 0;
    for (inputBitIndex = 0; inputBitIndex < 16;
         inputBitIndex++, inputCurrentBits >>= 1, inputRepeatMask <<= 1)
    {
        if ((inputCurrentBits & 1) != 0)
        {
            inputStateSlot->heldFrames[inputBitIndex]++;
            if (inputStateSlot->heldFrames[inputBitIndex] >= 26)
            {
                inputStateSlot->repeat |= inputRepeatMask;
                inputStateSlot->heldFrames[inputBitIndex] -= 8;
            }
        }
        else
        {
            inputStateSlot->heldFrames[inputBitIndex] = 0;
        }
    }
    inputStateSlot->pressed =
        (inputStateSlot->current ^ inputStateSlot->previous) & inputStateSlot->current;
    inputStateSlot->released =
        (inputStateSlot->current ^ inputStateSlot->previous) & ~inputStateSlot->current;
    return inputButtons;
}
#undef inputRepeatMask
#undef inputCurrentBits
#undef inputBitIndex
#undef inputResult
#undef inputStateSlot
#undef inputButtons

void ResetKeyboard()
{
    u8 keyboardState[256];

    GetKeyboardState(keyboardState);
    for (i32 index = 0; index < 256; index++)
    {
        keyboardState[index] &= 0x7f;
    }
    SetKeyboardState(keyboardState);
}

#undef KEYBOARD_KEY_PRESSED

} // namespace Controller
} // namespace th095
