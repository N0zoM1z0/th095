#ifndef TH095_PIXEL_FORMATS_HPP
#define TH095_PIXEL_FORMATS_HPP

#include "inttypes.hpp"

namespace th095
{

struct PixelArgb1555
{
    u16 blue : 5;
    u16 green : 5;
    u16 red : 5;
    u16 alpha : 1;
};

struct PixelArgb4444
{
    u16 blue : 4;
    u16 green : 4;
    u16 red : 4;
    u16 alpha : 4;
};

void __fastcall AccumulateArgb8888Neighbor(u32 *sums, u8 *pixel,
                                           u32 *count);
void __fastcall AccumulateArgb1555Neighbor(u32 *sums, PixelArgb1555 *pixel,
                                           u32 *count);
void __fastcall AccumulateArgb4444Neighbor(u32 *sums, PixelArgb4444 *pixel,
                                           u32 *count);

} // namespace th095

#endif
