#include "FrontEndPendingTextureData.hpp"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <type_traits>

using th095::FrontEndPendingTextureData;

static_assert(sizeof(void *) == 8, "This oracle must execute as a 64-bit process");
static_assert(std::is_same<decltype(FrontEndPendingTextureData::primaryData),
                           u8 *[3]>::value, "Primary slots must retain pointers");
static_assert(std::is_same<decltype(FrontEndPendingTextureData::secondaryData),
                           u8 *[3]>::value, "Secondary slots must retain pointers");
static_assert(sizeof(i32) == 4, "Lengths and counts remain four-byte values");
static_assert(sizeof(FrontEndPendingTextureData) == 0x58,
              "Verify the native layout independently of the target i386 layout");

static void Check(bool condition, int line)
{
    if (!condition)
    {
        fprintf(stderr, "pending texture oracle failed at line %d\n", line);
        exit(1);
    }
}

#define CHECK(condition) Check((condition), __LINE__)

struct Consumer
{
    FrontEndPendingTextureData pendingTextures;
};

static i32 Advance(Consumer *view)
{
    i32 i;
#include "FrontEndPendingTextureAdvance.inl"
    return i;
}

static u8 *Allocate(u8 marker)
{
    u8 *data = static_cast<u8 *>(malloc(4));
    CHECK(data != NULL);
    // Require actual live buffers above 4 GB, not synthetic integer addresses.
    CHECK(reinterpret_cast<uintptr_t>(data) > UINT32_MAX);
    data[0] = marker;
    return data;
}

static void CheckDrain(i32 initialCount, bool missingSecondary)
{
    Consumer consumer = {};
    FrontEndPendingTextureData &pending = consumer.pendingTextures;
    u8 *primary[3] = {};
    u8 *secondary[3] = {};
    for (i32 slot = 0; slot < initialCount; ++slot)
    {
        primary[slot] = Allocate(static_cast<u8>(10 + slot));
        pending.primaryData[slot] = primary[slot];
        pending.primarySize[slot] = 100 + slot;
        if (!(missingSecondary && slot == 0))
        {
            secondary[slot] = Allocate(static_cast<u8>(20 + slot));
            pending.secondaryData[slot] = secondary[slot];
            pending.secondarySize[slot] = 200 + slot;
        }
        // Publication follows both pointer/size columns, as in the worker.
        pending.count++;
    }

    for (i32 consumed = 0; consumed < initialCount; ++consumed)
    {
        CHECK(pending.count == initialCount - consumed);
        CHECK(pending.primaryData[0] == primary[consumed]);
        CHECK(pending.primaryData[0][0] == 10 + consumed);
        CHECK(pending.primarySize[0] == 100 + consumed);
        CHECK(pending.secondaryData[0] == secondary[consumed]);
        if (pending.secondaryData[0] != NULL)
        {
            CHECK(pending.secondaryData[0][0] == 20 + consumed);
            CHECK(pending.secondarySize[0] == 200 + consumed);
        }
        else
            CHECK(pending.secondarySize[0] == 0);

        // Production consumes/frees slot zero before entering its shift block.
        free(pending.primaryData[0]);
        pending.primaryData[0] = NULL;
        free(pending.secondaryData[0]);
        pending.secondaryData[0] = NULL;
        CHECK(Advance(&consumer) == 2);

        i32 remaining = initialCount - consumed - 1;
        CHECK(pending.count == remaining);
        for (i32 slot = remaining; slot < 3; ++slot)
        {
            CHECK(pending.primaryData[slot] == NULL);
            CHECK(pending.secondaryData[slot] == NULL);
            CHECK(pending.primarySize[slot] == 0);
            CHECK(pending.secondarySize[slot] == 0);
        }
    }
}

int main()
{
    for (i32 count = 1; count <= 3; ++count)
    {
        CheckDrain(count, false);
        CheckDrain(count, true);
    }
    puts("pending textures: native 64-bit pointers above 4 GB and shared shift passed");
    return 0;
}
