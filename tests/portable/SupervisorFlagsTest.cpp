#include "SupervisorFlags.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <type_traits>

using th095::SupervisorFlags;
static_assert(sizeof(void *) == 8, "Require native x86-64");
static_assert(sizeof(SupervisorFlags) == 4, "Flags remain one four-byte word");
static_assert(alignof(SupervisorFlags) == alignof(u32), "Retain word alignment");
static_assert(std::is_standard_layout<SupervisorFlags>::value, "Standard layout");
static_assert(std::is_trivially_copyable<SupervisorFlags>::value, "Byte snapshots are valid");
static void Check(bool condition, int line) {
    if (!condition) { fprintf(stderr, "Supervisor flags oracle failed at %d\n", line); exit(1); }
}
#define CHECK(x) Check((x), __LINE__)

// Physical bit positions, not new acceptance of the inherited business names.
#define FLAG_FIELDS(X) \
    X(0, usingHardwareTL) \
    X(1, lockableBackbuffer) \
    X(2, using32BitGraphics) \
    X(3, speedhackDetected) \
    X(4, d3dDeviceNeedsReset) \
    X(5, forceExtraTimerStep) \
    X(6, dummyMidiTimerEnabled) \
    X(7, receivedCloseMsg) \
    X(8, unknownBit8) \
    X(9, resultRestartActive) \
    X(10, keyboardAvailable) \
    X(11, controllerAvailable) \
    X(12, restartPhotoGame)
static u32 Read(const SupervisorFlags &flags, unsigned bit) {
    switch (bit) {
#define READ_CASE(index, field) case index: return flags.field;
        FLAG_FIELDS(READ_CASE)
#undef READ_CASE
    }
    CHECK(false); return 0;
}
static void Write(SupervisorFlags &flags, unsigned bit, u32 value) {
    switch (bit) {
#define WRITE_CASE(index, field) case index: flags.field=value; return;
        FLAG_FIELDS(WRITE_CASE)
#undef WRITE_CASE
    }
    CHECK(false);
}
#undef FLAG_FIELDS
static u32 Snapshot(const SupervisorFlags &flags) {
    u32 value=0;
    memcpy(&value, &flags, sizeof(value));
    return value;
}
int main() {
    const u32 upperWords[] = {0, 1, 0x7ffff};
    unsigned states=0, writes=0;
    for (unsigned upperIndex=0; upperIndex<3; ++upperIndex) {
        for (u32 low=0; low<0x2000; ++low) {
            const u32 word=(upperWords[upperIndex] << 13) | low;
            SupervisorFlags flags={};
            memcpy(&flags, &word, sizeof(word));
            CHECK(Snapshot(flags)==word);
            CHECK(flags.unknown13==upperWords[upperIndex]);
            for (unsigned bit=0; bit<13; ++bit) {
                CHECK(Read(flags,bit)==((word >> bit) & 1));
                for (u32 value=0; value<2; ++value) {
                    SupervisorFlags changed=flags;
                    Write(changed,bit,value);
                    CHECK(Snapshot(changed)==((word & ~(1u << bit)) | (value << bit)));
                    ++writes;
                }
            }
            SupervisorFlags changed=flags;
            changed.unknown13=0;
            CHECK(Snapshot(changed)==low);
            changed.unknown13=0x7ffff;
            CHECK(Snapshot(changed)==(0xffffe000u | low));
            ++states;
        }
    }
    CHECK(states==24576 && writes==638976);
    puts("Supervisor flags: standard native layout, 24576 word states and 638976 neighbor-preserving writes passed");
}
