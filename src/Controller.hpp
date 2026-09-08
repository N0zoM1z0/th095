#ifndef TH095_CONTROLLER_HPP
#define TH095_CONTROLLER_HPP

#define DIRECTINPUT_VERSION 0x0800

#include "Global.hpp"

#include <dinput.h>

namespace th095
{

#ifdef TH095_MATCH_EXACT
extern u8 g_ControllerButtons;
extern LPDIRECTINPUTDEVICE8A g_ControllerDevices;
extern u32 g_ControllerRuntimeFlags;
extern JOYCAPSA g_JoystickCaps;
extern i16 g_ControllerPadXAxis;
extern i16 g_ControllerPadYAxis;
extern i32 g_ControllerInputEnabled;
extern LPDIRECTINPUTDEVICE8A g_KeyboardDevice;
extern u8 g_ControllerAssignments;
#else
DIFFABLE_EXTERN(i32, g_ControllerInputEnabled);
DIFFABLE_EXTERN_ARRAY(u8, 128, g_ControllerButtons);
DIFFABLE_EXTERN_ARRAY(JOYCAPSA, 2, g_JoystickCaps);
DIFFABLE_EXTERN_ARRAY(u8, 2, g_ControllerAssignments);
#endif

struct ControllerInputSlotView
{
    ControllerInputSlotView();

    u16 current;
    u16 previous;
    u16 repeat;
    u16 pressed;
    u16 released;
    u16 heldFrames[16];
    u8 unknown02a[0x2e];
    ControllerButtonMapping mappings[3];
};

#ifdef TH095_MATCH_EXACT
extern ControllerInputSlotView g_ControllerInputSlots;
#else
DIFFABLE_EXTERN_ARRAY(ControllerInputSlotView, 3, g_ControllerInputSlots);
#endif

namespace Controller
{
u16 GetJoystickCaps();
u32 SetButtonFromDirectInputJoystate(u16 *outButtons,
                                     i16 controllerButtonToTest,
                                     u16 touhouButton, u8 *inputButtons);
u32 SetButtonFromControllerInputs(u16 *outButtons,
                                  i16 controllerButtonToTest,
                                  u16 touhouButton, u32 inputButtons);
u8 *GetControllerState(i32 deviceIndex);
u16 GetControllerInput(i32 controllerIndex, i32 joystickIndex, u16 buttons);
u16 GetInput(i32 inputIndex);
void ResetKeyboard();
}

} // namespace th095

#endif
