#pragma once

#include "ZunTimer.hpp"

namespace th095
{

// Front-end transitions clear the fractional frame before the previous frame.
static __forceinline void FrontEndResetTimer(ZunTimer *timer)
{
    timer->current = 0;
    timer->subFrame = 0.0f;
    timer->previous = -999999;
}

} // namespace th095
