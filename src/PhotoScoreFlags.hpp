#ifndef TH095_PHOTO_SCORE_FLAGS_HPP
#define TH095_PHOTO_SCORE_FLAGS_HPP

namespace th095
{

enum PhotoScoreFlags
{
    PHOTO_SCORE_ENEMY = 1 << 0,
    PHOTO_SCORE_SELF = 1 << 1,
    PHOTO_SCORE_TWO_SHOT = 1 << 2,
    PHOTO_SCORE_BOSS_RATE = 1 << 3,
    PHOTO_SCORE_NEARBY = 1 << 4,
    PHOTO_SCORE_UNKNOWN_5 = 1 << 5,
    PHOTO_SCORE_COLOR_1 = 1 << 6,
    PHOTO_SCORE_COLOR_2 = 1 << 7,
    PHOTO_SCORE_COLOR_3 = 1 << 8,
    PHOTO_SCORE_COLOR_4 = 1 << 9,
    PHOTO_SCORE_COLOR_5 = 1 << 10,
    PHOTO_SCORE_COLOR_6 = 1 << 11,
    PHOTO_SCORE_COLOR_7 = 1 << 12,
    PHOTO_SCORE_COLORFUL = 1 << 13,
    PHOTO_SCORE_RAINBOW = 1 << 14,
    PHOTO_SCORE_EMPTY = 1 << 15,
    PHOTO_SCORE_NO_BULLETS = 1 << 16,
    PHOTO_SCORE_UNKNOWN_17 = 1 << 17,
    PHOTO_SCORE_UNKNOWN_18 = 1 << 18,
    PHOTO_SCORE_UNKNOWN_19 = 1 << 19,
};

// VC7.1 emits the retail shift-and-test sequence for division by a flag mask.
#define PHOTO_SCORE_FLAG_SET(flags, mask) ((((flags) / (mask)) & 1) != 0)

} // namespace th095

#endif
