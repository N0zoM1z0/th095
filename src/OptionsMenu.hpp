#pragma once

#include "inttypes.hpp"

#ifdef TH095_MATCH_EXACT
#include "OptionsMenuVmEmission.hpp"
#else
#define OPTIONS_LOOKUP_VM(index) g_AnmManager->GetVm(this->vmIds[index])
#define OPTIONS_SHOW_VM(index) OPTIONS_LOOKUP_VM(index)->drawEnabled = 1
#define OPTIONS_HIDE_VM(index) OPTIONS_LOOKUP_VM(index)->drawEnabled = 0
#endif

namespace th095
{

enum OptionsMenuItemValue
{
    OPTIONS_MENU_ITEM_BUTTON02_BINDING = 0,
    OPTIONS_MENU_ITEM_BUTTON00_BINDING = 1,
    OPTIONS_MENU_ITEM_BUTTON06_BINDING = 2,
    OPTIONS_MENU_ITEM_WINDOW_MODE = 3,
    OPTIONS_MENU_ITEM_BGM_VOLUME = 4,
    OPTIONS_MENU_ITEM_SFX_VOLUME = 5,
    OPTIONS_MENU_ITEM_EXIT = 6,
    OPTIONS_MENU_ITEM_COUNT = 7,
};
extern i16 g_OptionsLastJoystickButton;

} // namespace th095
