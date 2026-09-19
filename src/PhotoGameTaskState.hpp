#ifndef TH095_PHOTO_GAME_TASK_STATE_HPP
#define TH095_PHOTO_GAME_TASK_STATE_HPP

#include "inttypes.hpp"

namespace th095
{

// Dependency-light access to the two photography-task state bits consumed by
// the legacy ECL declaration graph.  This is not a second owner layout: the
// canonical PhotoGameTaskView pins its flags member to this offset.
enum PhotoGameTaskStateLayout
{
    PHOTO_GAME_TASK_FLAGS_OFFSET = 0xfc
};

enum PhotoGameTaskStateBit
{
    PHOTO_GAME_TASK_FLAG_PHOTO_TRANSITION_BIT = 10
};

enum PhotoGameTaskStateFlag
{
    PHOTO_GAME_TASK_FLAG_PHOTO_SOUND_SUPPRESSED = 0x00000200,
    PHOTO_GAME_TASK_FLAG_PHOTO_TRANSITION_ACTIVE = 0x00000400
};

#define TH095_PHOTO_GAME_TASK_FLAGS(task) \
    (*reinterpret_cast<u32 *>( \
        reinterpret_cast<u8 *>(task) + PHOTO_GAME_TASK_FLAGS_OFFSET))

} // namespace th095

#endif
