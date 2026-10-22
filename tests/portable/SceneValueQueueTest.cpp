#include "SceneValueQueue.hpp"

#include <array>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <type_traits>

namespace th095
{
#include "SceneValueQueue.inl"
}

using th095::SceneValueQueue;
typedef std::array<i32, 18> TargetWords;

static_assert(sizeof(void *) == 8, "Run the raw-word oracle in a 64-bit process");
static_assert(sizeof(SceneValueQueue) == 72, "Raw words retain the target width");
static_assert(std::is_trivially_copyable<th095::SceneQueueWord>::value,
              "Every copied word must be a real, trivially copyable object");

static void Check(bool condition, int line)
{
    if (!condition)
    {
        fprintf(stderr, "raw queue oracle failed at line %d\n", line);
        exit(1);
    }
}

#define CHECK(condition) Check((condition), __LINE__)

static TargetWords Snapshot(const SceneValueQueue &queue)
{
    TargetWords result;
    for (int i = 0; i < 18; ++i)
        result[i] = queue.words[i].value;
    return result;
}

static void CheckState(SceneValueQueue &queue, const TargetWords &expected)
{
    CHECK(Snapshot(queue) == expected);
    CHECK(queue.Size() == expected[16]);
    CHECK(queue.Front() == (expected[16] > 0 ? expected[0] : 0));
}

static i32 ModelPush(TargetWords &words, i32 value)
{
    i32 slot = words[16] < words[17] ? words[16] : words[17] - 1;
    words[slot] = value;
    words[16] = slot + 1;
    return words[16];
}

static i32 ModelPop(TargetWords &words)
{
    if (words[16] <= 0)
        return 0;
    TargetWords before = words;
    for (int i = 0; i < 15; ++i)
        words[i] = before[i + 1];
    // Independent target model: the final payload word receives metadata,
    // while count is decremented and capacity is left unchanged.
    words[15] = before[16] - 1;
    words[16] = before[16] - 1;
    return before[1];
}

static void Seed(SceneValueQueue &queue, i32 count, i32 capacity)
{
    for (int i = 0; i < 16; ++i)
        queue.words[i].value = i % 2 == 0 ? 1000 + i : -1000 - i;
    queue.words[16].value = count;
    queue.words[17].value = capacity;
}

int main()
{
    SceneValueQueue initialized;
    for (int i = 0; i < 17; ++i)
        CHECK(initialized.words[i].value == 0);
    CHECK(initialized.words[17].value == 16);
    // Front-end construction later clears the complete outer allocation.
    memset(initialized.words, 0, sizeof(initialized.words));
    TargetWords cleared = Snapshot(initialized);
    CHECK(initialized.Pop() == 0);
    CheckState(initialized, cleared);

    const i32 payloads[] = {INT_MIN, -1, 0, 0x12345678, INT_MAX};
    for (i32 count = 0; count <= 16; ++count)
    {
        for (i32 capacity = 1; capacity <= 16; ++capacity)
        {
            for (unsigned int p = 0; p < sizeof(payloads) / sizeof(payloads[0]); ++p)
            {
                SceneValueQueue queue;
                Seed(queue, count, capacity);
                TargetWords expected = Snapshot(queue);
                i32 result = ModelPush(expected, payloads[p]);
                CHECK(queue.Push(payloads[p]) == result);
                CheckState(queue, expected);
            }
        }
        SceneValueQueue queue;
        Seed(queue, count, 16);
        TargetWords expected = Snapshot(queue);
        i32 result = ModelPop(expected);
        CHECK(queue.Pop() == result);
        CheckState(queue, expected);
    }

    SceneValueQueue sequence;
    TargetWords expected = Snapshot(sequence);
    for (i32 value = 0; value < 40; ++value)
    {
        CHECK(sequence.Push(value) == ModelPush(expected, value));
        CheckState(sequence, expected);
    }
    for (int i = 0; i < 16; ++i)
    {
        CHECK(sequence.Pop() == ModelPop(expected));
        CheckState(sequence, expected);
    }
    CHECK(sequence.Size() == 0);
    CHECK(sequence.words[0].value == 15); // Metadata became the last returned front.
    CHECK(sequence.Front() == 0);
    CHECK(sequence.Pop() == 0);
    CheckState(sequence, expected);
    puts("raw 32-bit queue: native x64 full-state model and metadata transfer passed");
    return 0;
}
