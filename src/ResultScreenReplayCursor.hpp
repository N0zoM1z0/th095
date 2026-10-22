#ifndef TH095_RESULT_SCREEN_REPLAY_CURSOR_HPP
#define TH095_RESULT_SCREEN_REPLAY_CURSOR_HPP

#include "inttypes.hpp"
#include <stddef.h>

namespace th095
{

// Shared by ResultScreen, Scene Select and Replay Browser. These are runtime
// selection indices and counts, never pointers or serialized file offsets.
struct ResultScreenReplayCursor
{
    i32 current;
    i32 previous;
    i32 count;
    i32 savedCurrent[16];
    i32 savedCount[16];
    i32 saveDepth;
    i32 disabledEntries[16];
    i32 wraps;
    i32 disabledEntryCount;

    ResultScreenReplayCursor()
    {
        this->saveDepth = 0;
        this->current = 0;
        this->disabledEntryCount = 0;
        this->wraps = 1;
        this->count = 999;
    }

    // Legacy ResultScreen construction-phase overload. Only the frozen exact
    // ResultScreen header defines it; ordinary callers use the default ctor.
    // Its compiler-only storage is intentionally outside this runtime owner.
    ResultScreenReplayCursor(i32);

    i32 Move(i32 amount);
    void Push();
    void Pop();

    i32 GetCurrent() { return this->current; }
    i32 GetPrevious() { return this->previous; }
    i32 GetCount() { return this->count; }
    void SetCount(i32 value) { this->count = value; }
    void SaveCurrent() { this->previous = this->current; }
    u32 HasChanged() { return this->previous != this->current; }

    void Disable(i32 value)
    {
        this->disabledEntries[this->disabledEntryCount] = value;
        this->disabledEntryCount++;
    }

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
            this->current = value;
        }
    }
};


// Target Push/Pop/Move at 0x0041B660/0x0041B6E0/0x0041B760 independently
// establish the history arrays, disabled entries, and signed four-byte slots.
typedef char ResultCursorScalarWidthIs4[(sizeof(i32) == 4) ? 1 : -1];
typedef char ResultCursorSizeIsD8[(sizeof(ResultScreenReplayCursor) == 0xd8) ? 1 : -1];
typedef char ResultCursorHistoryLayout[
    (offsetof(ResultScreenReplayCursor, current) == 0x00 &&
     offsetof(ResultScreenReplayCursor, previous) == 0x04 &&
     offsetof(ResultScreenReplayCursor, count) == 0x08 &&
     offsetof(ResultScreenReplayCursor, savedCurrent) == 0x0c &&
     offsetof(ResultScreenReplayCursor, savedCount) == 0x4c &&
     offsetof(ResultScreenReplayCursor, saveDepth) == 0x8c) ? 1 : -1];
typedef char ResultCursorDisabledLayout[
    (offsetof(ResultScreenReplayCursor, disabledEntries) == 0x90 &&
     offsetof(ResultScreenReplayCursor, wraps) == 0xd0 &&
     offsetof(ResultScreenReplayCursor, disabledEntryCount) == 0xd4) ? 1 : -1];

} // namespace th095

#endif
