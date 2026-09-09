#pragma once

// The retail ECL RunEcl/CallEclSub ABI uses a global ZunResult return type.
// Keep that target type available in production as well as exact replay; its
// enumerator names remain distinct from the canonical th095::ZunResult values.
enum ZunResult
{
    TH095_LEGACY_ZUN_SUCCESS = 0,
    TH095_LEGACY_ZUN_ERROR = -1
};

namespace th095
{
enum ZunResult
{
    ZUN_SUCCESS = 0,
    ZUN_ERROR = -1
};
} // namespace th095
