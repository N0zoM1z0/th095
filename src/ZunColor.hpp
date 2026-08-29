#pragma once

#include "inttypes.hpp"

#define COLOR_BLACK 0xff000000
#define COLOR_LIGHT_GREY 0xffa0a0a0
#define COLOR_WHITE 0xffffffff

#define COLOR_TEXT_WHITE 0xffffff

#define COLOR_RGB_MASK 0x00FFFFFF
#define COLOR_ALPHA_MASK 0xFF000000
#define COLOR_RGB(color) ((color) & COLOR_RGB_MASK)
#define COLOR_ALPHA(color) (((color) & COLOR_ALPHA_MASK) >> 24)
#define COLOR_SET_ALPHA(color, alpha) (((alpha) << 24) | COLOR_RGB(color))
#define COLOR_SET_ALPHA2(color, alpha) (COLOR_RGB(color) | (((alpha) & 0xff) << 24))
#define COLOR_SET_ALPHA3(color, alpha) (COLOR_RGB(color) | ((alpha) << 24))
#define COLOR_COMBINE_ALPHA(color, alpha) (((alpha) & COLOR_ALPHA_MASK) | COLOR_RGB(color))

union ZunColor {
    u32 d3dColor;
    struct
    {
        u8 b;
        u8 g;
        u8 r;
        u8 a;
    };
};
