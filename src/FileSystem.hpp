#ifndef TH095_FILE_SYSTEM_HPP
#define TH095_FILE_SYSTEM_HPP

#include "Global.hpp"

namespace th095
{
enum ReplayPlaybackSource
{
    REPLAY_PLAYBACK_SOURCE_LOOSE_FILE = 0,
    REPLAY_PLAYBACK_SOURCE_ARCHIVE = 1,
};
DIFFABLE_EXTERN(ReplayPlaybackSource, g_ReplayUsesArchive);
}

#endif
