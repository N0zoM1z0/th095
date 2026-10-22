#include "ResultScreenReplayCursor.hpp"

#include <stdio.h>
#include <stdlib.h>

namespace th095
{
#include "ResultScreenReplayCursor.inl"
}

static_assert(sizeof(void *) == 8, "This oracle must execute as a 64-bit process");

static void Check(bool condition, int line)
{
    if (!condition)
    {
        fprintf(stderr, "cursor oracle failed at line %d\n", line);
        exit(1);
    }
}

#define CHECK(condition) Check((condition), __LINE__)

using th095::ResultScreenReplayCursor;

static void CheckInitializationAndSelection()
{
    ResultScreenReplayCursor cursor;
    CHECK(cursor.GetCurrent() == 0);
    CHECK(cursor.GetCount() == 999);
    CHECK(cursor.saveDepth == 0 && cursor.disabledEntryCount == 0);
    CHECK(cursor.wraps == 1);

    cursor.SetCount(4);
    cursor.Set(8);
    CHECK(cursor.GetCurrent() == 3);
    cursor.SaveCurrent();
    CHECK(!cursor.HasChanged());
    cursor.Set(-1);
    CHECK(cursor.GetCurrent() == 0 && cursor.HasChanged());
    CHECK(cursor.GetPrevious() == 3);

    cursor.Set(3);
    cursor.SetCount(2);
    CHECK(cursor.GetCurrent() == 3); // Count publication does not clamp.
    cursor.Set(3);
    CHECK(cursor.GetCurrent() == 1);
    cursor.SetCount(0);
    cursor.Set(-7);
    CHECK(cursor.GetCurrent() == -7); // Inactive cursor; do not call Move.
    cursor.Set(12);
    CHECK(cursor.GetCurrent() == 12);
}

static void CheckMovement()
{
    // An independent modular/clamped model covers both directions and moves
    // larger than one page, including each Scene Select group count 1..12.
    for (int count = 1; count <= 12; ++count)
    {
        for (int start = 0; start < count; ++start)
        {
            for (int amount = -25; amount <= 25; ++amount)
            {
                for (int wraps = 0; wraps <= 1; ++wraps)
                {
                    ResultScreenReplayCursor cursor;
                    cursor.SetCount(count);
                    cursor.Set(start);
                    cursor.wraps = wraps;
                    int expected = start + amount;
                    if (wraps)
                        expected = ((expected % count) + count) % count;
                    else if (expected < 0)
                        expected = 0;
                    else if (expected >= count)
                        expected = count - 1;
                    CHECK(cursor.Move(amount) == expected);
                    CHECK(cursor.GetCurrent() == expected);
                }
            }
        }
    }

    for (int direction = -1; direction <= 1; direction += 2)
    {
        ResultScreenReplayCursor cursor;
        cursor.SetCount(17);
        cursor.Set(16);
        for (int entry = 0; entry < 16; ++entry)
            cursor.Disable(entry);
        CHECK(cursor.disabledEntryCount == 16);
        CHECK(cursor.Move(direction) == 16); // All 16 disabled slots skipped.
    }

    ResultScreenReplayCursor cursor;
    cursor.SetCount(5);
    cursor.wraps = 0;
    cursor.Disable(1);
    cursor.Disable(2);
    cursor.Disable(3);
    CHECK(cursor.Move(1) == 4);
    CHECK(cursor.Move(-1) == 0);
}

static void CheckHistory()
{
    ResultScreenReplayCursor cursor;
    cursor.SetCount(12);
    cursor.Set(7);
    cursor.SaveCurrent();
    cursor.Disable(1);
    cursor.Push();
    CHECK(cursor.saveDepth == 1 && cursor.disabledEntryCount == 0);
    cursor.SetCount(4);
    cursor.Set(2);
    cursor.Push();
    cursor.SetCount(1);
    cursor.Set(0);
    cursor.Disable(0); // Cleared by Pop before another movement operation.
    cursor.Pop();
    CHECK(cursor.GetCount() == 4 && cursor.GetCurrent() == 2);
    CHECK(cursor.saveDepth == 1 && cursor.disabledEntryCount == 0);
    CHECK(cursor.GetPrevious() == 7); // History does not restore previous.
    cursor.Pop();
    CHECK(cursor.GetCount() == 12 && cursor.GetCurrent() == 7);
    CHECK(cursor.saveDepth == 0);
    cursor.Pop(); // Slot zero was initialized by the first Push.
    CHECK(cursor.GetCount() == 12 && cursor.GetCurrent() == 7);

    // Target saturation writes slot 15 repeatedly, but the next Pop reads 14.
    // Preserve that behavior rather than substituting a conventional stack.
    for (int level = 0; level < 18; ++level)
    {
        cursor.SetCount(100 + level);
        cursor.Set(level);
        cursor.Push();
    }
    CHECK(cursor.saveDepth == 15);
    for (int expected = 14; expected >= 0; --expected)
    {
        cursor.Pop();
        CHECK(cursor.GetCurrent() == expected);
        CHECK(cursor.GetCount() == 100 + expected);
    }
}

int main()
{
    CheckInitializationAndSelection();
    CheckMovement();
    CheckHistory();
    puts("shared cursor: 64-bit layout, selection, movement and history passed");
    return 0;
}
