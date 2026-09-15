#pragma once

#include "inttypes.hpp"

namespace th095
{

typedef u16 PhotoBulletState;
// Target-observed lifecycle values; value 4 remains unclassified.
enum PhotoBulletStateValue
{
    PHOTO_BULLET_STATE_INACTIVE = 0,
    PHOTO_BULLET_STATE_ACTIVE = 1,
    PHOTO_BULLET_STATE_SPAWN_TRANSITION = 2,
    PHOTO_BULLET_STATE_DESPAWN_TRANSITION = 3,
    PHOTO_BULLET_STATE_CURSOR_SENTINEL = 5,
};
typedef char PhotoBulletStateSizeIs2[
    (sizeof(PhotoBulletState) == sizeof(u16)) ? 1 : -1];

} // namespace th095
