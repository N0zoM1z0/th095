#include "AnmVmInterpolation.hpp"

namespace th095
{

void AnmVmColorInterpolationView::SetColor1Interpolation(
    i32 duration, u8 mode, u32 initial, u32 final)
{
    this->interpCurrentTimers[1].SetCurrent(0);
    this->interpEndTimers[1].SetCurrent(duration);
    this->interpModes[1] = mode;
    this->color1Initial.r = (u8)((initial >> 16) & 0xff);
    this->color1Initial.g = (u8)((initial >> 8) & 0xff);
    this->color1Initial.b = (u8)(initial & 0xff);
    this->color1Final.r = (u8)((final >> 16) & 0xff);
    this->color1Final.g = (u8)((final >> 8) & 0xff);
    this->color1Final.b = (u8)(final & 0xff);
}

} // namespace th095
