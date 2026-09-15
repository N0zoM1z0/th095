#ifndef TH095_FILE_SYSTEM_HPP
#define TH095_FILE_SYSTEM_HPP

#include "Global.hpp"

namespace th095
{
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
extern i32 g_ReplayUsesArchive;
#define REPLAY_PLAYBACK_SOURCE_LOOSE_FILE 0
#define REPLAY_PLAYBACK_SOURCE_ARCHIVE 1
#else
enum ReplayPlaybackSource
{
    REPLAY_PLAYBACK_SOURCE_LOOSE_FILE = 0,
    REPLAY_PLAYBACK_SOURCE_ARCHIVE = 1,
};
extern ReplayPlaybackSource g_ReplayUsesArchive;
#endif
}

#endif
