#pragma once

#include "ControllerButtonMapping.hpp"
#include "diffbuild.hpp"

namespace th095
{

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

DIFFABLE_EXTERN_ARRAY(ControllerInputSlotView, 3, g_ControllerInputSlots);

} // namespace th095
