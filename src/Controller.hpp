#ifndef TH095_CONTROLLER_HPP
#define TH095_CONTROLLER_HPP

#define DIRECTINPUT_VERSION 0x0800

#include "ControllerInputSlot.hpp"
#include "Global.hpp"

#include <dinput.h>

namespace th095
{

DIFFABLE_EXTERN(i32, g_ControllerInputEnabled);
DIFFABLE_EXTERN_ARRAY(u8, 128, g_ControllerButtons);
DIFFABLE_EXTERN_ARRAY(JOYCAPSA, 2, g_JoystickCaps);

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
