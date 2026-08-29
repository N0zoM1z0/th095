#ifndef TH095_RESULT_SCREEN_HPP
#define TH095_RESULT_SCREEN_HPP

#include "Global.hpp"
#include "ReplayManager.hpp"

namespace th095
{

struct ResultScreenReplayCursor
{
    i32 current;
    i32 previous;
    i32 count;

    void Set(i32 value)
    {
        if (this->count != 0)
        {
            this->current = value >= this->count
                                ? this->count - 1
                                : (value < 0 ? 0 : value);
        }
        else
        {
            this->current = 0;
        }
    }
};

struct ResultScreen
{
    u8 unknown0000[0x4604];
    ResultScreenReplayCursor replayCursor; // +0x4604
    u8 unknown4610[0x6ce8 - 0x4610];
    ReplayManager *replays[20];            // +0x6ce8

    ZunResult LoadReplays();
};

typedef char ResultScreenReplayCursorAt4604[
    (offsetof(ResultScreen, replayCursor) == 0x4604) ? 1 : -1];
typedef char ResultScreenReplaysAt6CE8[
    (offsetof(ResultScreen, replays) == 0x6ce8) ? 1 : -1];

} // namespace th095

#endif
