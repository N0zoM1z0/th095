#pragma once

#include "inttypes.hpp"

namespace th095
{

// A nine-button mapping stored in input slots and the packed configuration.
struct ControllerButtonMapping
{
    i16 shotButton;
    i16 bombButton;
    i16 focusButton;
    i16 menuButton;
    i16 upButton;
    i16 downButton;
    i16 leftButton;
    i16 rightButton;
    i16 skipButton;
};

typedef char ControllerButtonMappingSizeIs12[
    (sizeof(ControllerButtonMapping) == 0x12) ? 1 : -1];

} // namespace th095
