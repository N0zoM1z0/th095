#pragma once

#include "inttypes.hpp"

namespace th095
{

typedef u8 GameColorMode;

enum GameColorModeValue
{
    GAME_COLOR_MODE_32_BIT = 0,
    GAME_COLOR_MODE_16_BIT = 1,
    GAME_COLOR_MODE_COUNT = 2,
};

typedef char GameColorModeSizeIs1[(sizeof(GameColorMode) == 1) ? 1 : -1];

} // namespace th095
