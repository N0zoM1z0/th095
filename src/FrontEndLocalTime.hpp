#pragma once

#include "inttypes.hpp"

#include <time.h>

namespace th095
{

#if defined(_MSC_VER) && _MSC_VER == 1310
// VC7.1's x86 CRT reads the stored signed 32-bit timestamp in place.
typedef char StoredTimestampMatchesCrtTime[
    (sizeof(i32) == sizeof(time_t)) ? 1 : -1];
#define LocalTimeFromStoredTimestamp(timestamp) localtime(reinterpret_cast<const time_t *>(timestamp))
#define StoreCurrentTimestamp(timestamp) time(reinterpret_cast<time_t *>(timestamp))
#else
static __forceinline tm *LocalTimeFromStoredTimestamp(const i32 *timestamp)
{
    time_t timeValue = static_cast<time_t>(*timestamp);
    return localtime(&timeValue);
}

static __forceinline void StoreCurrentTimestamp(i32 *timestamp)
{
    *timestamp = static_cast<i32>(time(NULL));
}
#endif

} // namespace th095
