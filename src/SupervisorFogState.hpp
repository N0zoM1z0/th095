#ifndef TH095_SUPERVISOR_FOG_STATE_HPP
#define TH095_SUPERVISOR_FOG_STATE_HPP

namespace th095
{

enum SupervisorFogCacheState
{
    SUPERVISOR_FOG_CACHE_DISABLED = 0,
    SUPERVISOR_FOG_CACHE_ENABLED = 1,
    SUPERVISOR_FOG_CACHE_INVALID = 0xff,
};

typedef char SupervisorFogCacheStateSizeIs4[
    (sizeof(SupervisorFogCacheState) == 4) ? 1 : -1];

} // namespace th095

#endif
