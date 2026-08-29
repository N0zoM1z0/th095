#ifndef TH095_RESULT_SCREEN_HPP
#define TH095_RESULT_SCREEN_HPP

#include "Global.hpp"
#include "ReplayManager.hpp"

namespace th095
{

struct ResultScreenTimer
{
    i32 previous;
    f32 subFrame;
    i32 current;

    u32 operator==(i32 value) { return this->current == value; }
    u32 operator<(i32 value) { return this->current < value; }
};

typedef char ResultScreenTimerSizeIsC[
    (sizeof(ResultScreenTimer) == 0x0c) ? 1 : -1];

struct ResultScreenReplayCursor
{
    i32 current;
    i32 previous;
    i32 count;
    u8 unknown00c[0x90 - 0x0c];
    i32 disabledEntries[16];
    i32 wraps;
    i32 disabledEntryCount;

    i32 Move(i32 amount);

    i32 GetCurrent() { return this->current; }
    i32 GetCount() { return this->count; }
    void SaveCurrent() { this->previous = this->current; }
    u32 HasChanged() { return this->previous != this->current; }

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

struct ResultScreenAnmVm
{
    u8 unknown000[0x22e];
    i16 pendingInterrupt;
    u8 unknown230[0x2cc - 0x230];

    void SetInterrupt(i32 interrupt)
    {
        this->pendingInterrupt = interrupt;
    }
};

typedef char ResultScreenAnmVmSizeIs2CC[
    (sizeof(ResultScreenAnmVm) == 0x2cc) ? 1 : -1];

struct ResultScreen
{
    u8 unknown0000[8];
    ResultScreenTimer stateTimer;         // +0x0008
    u8 unknown0014[4];
    ResultScreenAnmVm vms[25];            // +0x0018
    ResultScreenReplayCursor replayCursor; // +0x4604
    u8 unknown46dc[0x6ce8 - 0x46dc];
    ReplayManager *replays[20];            // +0x6ce8

    i32 UpdateCursor(i32 firstVm);
    ZunResult LoadReplays();
};

typedef char ResultScreenVmsAt18[
    (offsetof(ResultScreen, vms) == 0x18) ? 1 : -1];
typedef char ResultScreenReplayCursorAt4604[
    (offsetof(ResultScreen, replayCursor) == 0x4604) ? 1 : -1];
typedef char ResultScreenReplayCursorSizeIsD8[
    (sizeof(ResultScreenReplayCursor) == 0xd8) ? 1 : -1];
typedef char ResultScreenReplaysAt6CE8[
    (offsetof(ResultScreen, replays) == 0x6ce8) ? 1 : -1];

} // namespace th095

#endif
