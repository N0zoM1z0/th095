#pragma once

#include "InputRuntime.hpp"

namespace th095
{

#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
extern ReplayInputSource g_ReplayInputSource;
#endif

} // namespace th095
