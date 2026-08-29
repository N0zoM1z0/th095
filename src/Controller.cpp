#include "Controller.hpp"

#include <mmsystem.h>
#include <string.h>

namespace th095
{

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
    (offsetof(ControllerInputSlotView, shootButton) == 0x58 &&
     offsetof(ControllerInputSlotView, bombButton) == 0x5a &&
     offsetof(ControllerInputSlotView, menuButton) == 0x5e) ? 1 : -1];
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

    joyGetDevCapsA(0, &g_JoystickCaps, sizeof(g_JoystickCaps));
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

    locals.inputSlot = (&g_ControllerInputSlots) + controllerIndex;
    if (((g_ControllerRuntimeFlags >> 11) & 1) == 0)
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
            &buttons, locals.inputSlot->shootButton,
            TH_BUTTON_SHOOT, locals.joystickInfo.dwButtons);
        SetButtonFromControllerInputs(
            &buttons, locals.inputSlot->bombButton,
            TH_BUTTON_BOMB, locals.joystickInfo.dwButtons);
        SetButtonFromControllerInputs(
            &buttons, locals.inputSlot->menuButton,
            TH_BUTTON_MENU, locals.joystickInfo.dwButtons);

        locals.axisDeadzone =
            ((&g_JoystickCaps)[joystickIndex].wXmax -
             (&g_JoystickCaps)[joystickIndex].wXmin) /
            2 / 2;
        buttons |= locals.joystickInfo.dwXpos >
                           (((&g_JoystickCaps)[joystickIndex].wXmin +
                             (&g_JoystickCaps)[joystickIndex].wXmax) /
                                2 +
                            locals.axisDeadzone)
                       ? TH_BUTTON_RIGHT
                       : 0;
        buttons |= locals.joystickInfo.dwXpos <
                           (((&g_JoystickCaps)[joystickIndex].wXmin +
                             (&g_JoystickCaps)[joystickIndex].wXmax) /
                                2 -
                            locals.axisDeadzone)
                       ? TH_BUTTON_LEFT
                       : 0;

        locals.axisDeadzone =
            ((&g_JoystickCaps)[joystickIndex].wYmax -
             (&g_JoystickCaps)[joystickIndex].wYmin) /
            2 / 2;
        buttons |= locals.joystickInfo.dwYpos >
                           (((&g_JoystickCaps)[joystickIndex].wYmin +
                             (&g_JoystickCaps)[joystickIndex].wYmax) /
                                2 +
                            locals.axisDeadzone)
                       ? TH_BUTTON_DOWN
                       : 0;
        buttons |= locals.joystickInfo.dwYpos <
                           (((&g_JoystickCaps)[joystickIndex].wYmin +
                             (&g_JoystickCaps)[joystickIndex].wYmax) /
                                2 -
                            locals.axisDeadzone)
                       ? TH_BUTTON_UP
                       : 0;
        return buttons;
    }

    locals.result = (&g_ControllerDevices)[joystickIndex]->Poll();
    if (locals.result < 0)
    {
        acquireAttempts = 0;
        utils::DebugPrint("error : DIERR_INPUTLOST\r\n");
        locals.result = (&g_ControllerDevices)[joystickIndex]->Acquire();
        while (locals.result == DIERR_INPUTLOST)
        {
            locals.result = (&g_ControllerDevices)[joystickIndex]->Acquire();
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
    locals.result = (&g_ControllerDevices)[joystickIndex]->GetDeviceState(
        sizeof(locals.joystickState), &locals.joystickState);
    if (locals.result < 0)
    {
        return buttons;
    }

    locals.directInputShootPressed = SetButtonFromDirectInputJoystate(
        &buttons, locals.inputSlot->shootButton,
        TH_BUTTON_SHOOT, locals.joystickState.rgbButtons);
    SetButtonFromDirectInputJoystate(
        &buttons, locals.inputSlot->bombButton,
        TH_BUTTON_BOMB, locals.joystickState.rgbButtons);
    SetButtonFromDirectInputJoystate(
        &buttons, locals.inputSlot->menuButton,
        TH_BUTTON_MENU, locals.joystickState.rgbButtons);

    buttons |= locals.joystickState.lX > g_ControllerPadXAxis
                   ? TH_BUTTON_RIGHT
                   : 0;
    buttons |= locals.joystickState.lX < -g_ControllerPadXAxis
                   ? TH_BUTTON_LEFT
                   : 0;
    buttons |= locals.joystickState.lY > g_ControllerPadYAxis
                   ? TH_BUTTON_DOWN
                   : 0;
    buttons |= locals.joystickState.lY < -g_ControllerPadYAxis
                   ? TH_BUTTON_UP
                   : 0;
    return buttons;
}

u8 *GetControllerState(i32 deviceIndex)
{
    i32 acquireAttempts;
    ControllerStateLocals locals;

    memset(&g_ControllerButtons, 0, 128);
    if (((g_ControllerRuntimeFlags >> 11) & 1) == 0)
    {
        memset(&locals.joystickInfo, 0, sizeof(locals.joystickInfo));
        locals.joystickInfo.dwSize = sizeof(locals.joystickInfo);
        locals.joystickInfo.dwFlags = JOY_RETURNALL;
        if (joyGetPosEx(0, &locals.joystickInfo) != JOYERR_NOERROR)
        {
            return &g_ControllerButtons;
        }

        locals.buttons = locals.joystickInfo.dwButtons;
        for (locals.buttonIndex = 0; locals.buttonIndex < 32;
             locals.buttonIndex++, locals.buttons >>= 1)
        {
            if ((locals.buttons & 1) != 0)
            {
                (&g_ControllerButtons)[locals.buttonIndex] = 0x80;
            }
        }
        return &g_ControllerButtons;
    }

    locals.result = (&g_ControllerDevices)[deviceIndex]->Poll();
    if (locals.result < 0)
    {
        acquireAttempts = 0;
        utils::DebugPrint("error : DIERR_INPUTLOST\r\n");
        locals.result = (&g_ControllerDevices)[deviceIndex]->Acquire();
        while (locals.result == DIERR_INPUTLOST)
        {
            locals.result = (&g_ControllerDevices)[deviceIndex]->Acquire();
            acquireAttempts++;
            if (acquireAttempts >= 400)
            {
                utils::DebugPrint(
                    "error : DIERR_INPUTLOST %d\r\n", acquireAttempts);
                return &g_ControllerButtons;
            }
        }

        return &g_ControllerButtons;
    }

    (&g_ControllerDevices)[deviceIndex]->GetDeviceState(
        sizeof(locals.joystickState), &locals.joystickState);
    if (locals.result < 0)
    {
        return &g_ControllerButtons;
    }
    memcpy(&g_ControllerButtons, locals.joystickState.rgbButtons, 128);
    return &g_ControllerButtons;
}

#define KEYBOARD_KEY_PRESSED(button, key) \
    keyboardState[key] & 0x80 ? button : 0

u16 GetInput(i32 inputIndex)
{
    u8 keyboardState[256];
    u16 buttons;
    ControllerInputSlotView *inputSlot;
    HRESULT result;
    i32 bitIndex;
    u16 repeatMask;
    u16 currentBits;

    buttons = 0;
    inputSlot = (&g_ControllerInputSlots) + inputIndex;
    if (g_ControllerInputEnabled != 0)
    {
        if (((g_ControllerRuntimeFlags >> 10) & 1) == 0)
        {
            GetKeyboardState(keyboardState);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, VK_UP);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, VK_DOWN);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, VK_LEFT);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, VK_RIGHT);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, 'Z');
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, 'X');
            buttons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_SHOOT | TH_BUTTON_FOCUS, VK_SHIFT);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, VK_NUMPAD8);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, VK_NUMPAD2);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, VK_NUMPAD4);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, VK_NUMPAD6);
            buttons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_UP_LEFT, VK_NUMPAD7);
            buttons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_UP_RIGHT, VK_NUMPAD9);
            buttons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_DOWN_LEFT, VK_NUMPAD1);
            buttons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_DOWN_RIGHT, VK_NUMPAD3);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SKIP, VK_CONTROL);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_MENU, VK_ESCAPE);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_ENTER, VK_RETURN);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, VK_HOME);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, 'P');
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_D, 'D');
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_Q, 'Q');
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_S, 'S');
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RESET, 'R');
            buttons |= KEYBOARD_KEY_PRESSED(0x8000, 'L');
        }
        else
        {
            result = g_KeyboardDevice->GetDeviceState(
                sizeof(keyboardState), keyboardState);
            buttons = 0;
            if (result == DIERR_INPUTLOST)
            {
                g_KeyboardDevice->Acquire();
            }
            else if (result != S_OK)
            {
                g_KeyboardDevice->Acquire();
            }
            else
            {
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_UP);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_DOWN);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_LEFT);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_RIGHT);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, DIK_Z);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, DIK_X);
                buttons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_SHOOT | TH_BUTTON_FOCUS, DIK_LSHIFT);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_NUMPAD8);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_NUMPAD2);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_NUMPAD4);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_NUMPAD6);
                buttons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_UP_LEFT, DIK_NUMPAD7);
                buttons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_UP_RIGHT, DIK_NUMPAD9);
                buttons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_DOWN_LEFT, DIK_NUMPAD1);
                buttons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_DOWN_RIGHT, DIK_NUMPAD3);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SKIP, DIK_LCONTROL);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_MENU, DIK_ESCAPE);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_ENTER, DIK_RETURN);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, DIK_HOME);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, DIK_P);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_D, DIK_D);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_Q, DIK_Q);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_S, DIK_S);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RESET, DIK_R);
                buttons |= KEYBOARD_KEY_PRESSED(0x8000, DIK_L);
            }
        }
    }

    if (inputIndex >= 2)
    {
        if ((&g_ControllerAssignments)[0] == 0 ||
            (&g_ControllerAssignments)[0] == 1)
        {
            buttons = GetControllerInput(
                0, (&g_ControllerAssignments)[0] != 0, buttons);
        }
        if ((&g_ControllerAssignments)[1] == 0 ||
            (&g_ControllerAssignments)[1] == 1)
        {
            buttons = GetControllerInput(
                1, (&g_ControllerAssignments)[1] != 0, buttons);
        }
    }
    else if ((&g_ControllerAssignments)[inputIndex] == 0 ||
             (&g_ControllerAssignments)[inputIndex] == 1)
    {
        buttons = GetControllerInput(
            inputIndex, (&g_ControllerAssignments)[inputIndex] != 0,
            buttons);
    }

    inputSlot->previous = inputSlot->current;
    inputSlot->current = buttons;
    repeatMask = 1;
    currentBits = buttons;
    inputSlot->repeat = 0;
    for (bitIndex = 0; bitIndex < 16;
         bitIndex++, currentBits >>= 1, repeatMask <<= 1)
    {
        if ((currentBits & 1) != 0)
        {
            inputSlot->heldFrames[bitIndex]++;
            if (inputSlot->heldFrames[bitIndex] >= 26)
            {
                inputSlot->repeat |= repeatMask;
                inputSlot->heldFrames[bitIndex] -= 8;
            }
        }
        else
        {
            inputSlot->heldFrames[bitIndex] = 0;
        }
    }
    inputSlot->pressed =
        (inputSlot->current ^ inputSlot->previous) & inputSlot->current;
    inputSlot->released =
        (inputSlot->current ^ inputSlot->previous) & ~inputSlot->current;
    return buttons;
}

#undef KEYBOARD_KEY_PRESSED

} // namespace Controller
} // namespace th095
