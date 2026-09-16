#pragma once

#include "inttypes.hpp"

namespace th095
{

typedef u8 GameMusicMode;

enum GameMusicModeValue
{
    GAME_MUSIC_MODE_DISABLED = 0,
    GAME_MUSIC_MODE_WAV = 1,
    GAME_MUSIC_MODE_MIDI = 2,
    GAME_MUSIC_MODE_COUNT = 3,
};

typedef char GameMusicModeSizeIs1[(sizeof(GameMusicMode) == 1) ? 1 : -1];

} // namespace th095
