#pragma once

#include "PhotoEnemyControl.hpp"

// Exact/DIFF access adapter for the two EclHelpers movement producers only.
// Normal source consumes the complete canonical PhotoEnemyView.
namespace th095
{

struct PhotoEnemyView
{
    u8 unknown0000[0x2bf4];
    union
    {
        u32 flags1;
        PhotoEnemyControlBits control;
    };
};

typedef char EclHelperEmissionFlagsAt2BF4[
    (offsetof(PhotoEnemyView, flags1) == 0x2bf4) ? 1 : -1];

} // namespace th095
