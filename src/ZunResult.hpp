#pragma once

#ifdef TH095_MATCH_EXACT
enum ZunResult
{
    TH095_LEGACY_ZUN_SUCCESS = 0,
    TH095_LEGACY_ZUN_ERROR = -1
};
#endif

namespace th095
{
enum ZunResult
{
    ZUN_SUCCESS = 0,
    ZUN_ERROR = -1
};
} // namespace th095
