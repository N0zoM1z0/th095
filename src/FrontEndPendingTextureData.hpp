#ifndef TH095_FRONT_END_PENDING_TEXTURE_DATA_HPP
#define TH095_FRONT_END_PENDING_TEXTURE_DATA_HPP

#include "inttypes.hpp"
#include <stddef.h>

namespace th095
{

// Three pending fcNN.anm/fcNNb.anm buffer pairs in the front-end allocation.
// FileSystem publishes byte buffers; Scene Select uploads only the newest
// remaining pair, frees each consumed pair, then shifts the parallel columns.
// Sizes/counts are four-byte game values. Data slots are host-native pointers,
// not serialized offsets or the integer payloads of the mission-preview queues.
// Publication, consumption, and teardown keep their existing external locks
// and explicit free calls; this storage type does not release memory itself.
struct FrontEndPendingTextureData
{
    i32 count;
    u8 *primaryData[3];
    i32 primarySize[3];
    u8 *secondaryData[3];
    i32 secondarySize[3];
};

typedef char FrontEndPendingTextureCountAtZero[
    (offsetof(FrontEndPendingTextureData, count) == 0) ? 1 : -1];
typedef char FrontEndPendingTextureScalarWidth[
    (sizeof(i32) == 4) ? 1 : -1];

} // namespace th095

#endif
