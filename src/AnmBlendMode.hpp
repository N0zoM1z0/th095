#pragma once

namespace th095
{
// Two-bit VM selectors. These names describe DESTBLEND handling on a cache
// miss, not an entire blend equation or original script-author intent.
enum AnmBlendMode
{
    ANM_BLEND_ALPHA = 0,
    ANM_BLEND_ADDITIVE = 1,
    ANM_BLEND_ADDITIVE_3D_ONLY = 2,
    ANM_BLEND_UNHANDLED = 3
};

// A separate cache operation deliberately uses the same physical value as
// the unhandled VM selector. Invalidation therefore cannot force a miss for
// every two-bit input. See docs/ANM_BLEND_MODE.md.
enum AnmBlendCacheState
{
    ANM_BLEND_CACHE_INVALIDATED = 3
};
}
