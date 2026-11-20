#ifndef TH095_PHOTO_SCORE_BREAKDOWN_HPP
#define TH095_PHOTO_SCORE_BREAKDOWN_HPP

#include "inttypes.hpp"
#include <stddef.h>

namespace th095
{

struct PhotoScoreBreakdownView
{
    i32 finalScore;
    i32 baseScore;
    i32 capturedBulletCount;
    i32 nearbyTargetCount;
    i32 nearbyTargetBonus;
    f32 enemyDistanceMultiplier;
    f32 bossRateMultiplier;
    union
    {
        u32 scoringFlags;
        struct
        {
            u32 enemyPhotographed : 1;
            u32 otherFlags1_2 : 2;
            u32 bossRateApplied : 1;
            u32 otherFlags4_31 : 28;
        };
    };
};

typedef char PhotoScoreBreakdownSizeIs20[
    (sizeof(PhotoScoreBreakdownView) == 0x20) ? 1 : -1];
typedef char PhotoScoreBreakdownMultipliersAt14[
    (offsetof(PhotoScoreBreakdownView, enemyDistanceMultiplier) == 0x14 &&
     offsetof(PhotoScoreBreakdownView, bossRateMultiplier) == 0x18)
        ? 1
        : -1];
typedef char PhotoScoreBreakdownFlagsAt1C[
    (offsetof(PhotoScoreBreakdownView, scoringFlags) == 0x1c) ? 1 : -1];

} // namespace th095

#endif
