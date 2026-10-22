#include "ScenePreviewDataQueue.hpp"
#include <memory>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

namespace th095 {
#include "SceneValueQueue.inl"
#include "ScenePreviewDataQueue.inl"
}
using namespace th095;
static void Check(bool ok, int line)
{
    if (!ok) { fprintf(stderr, "queue publication failed at %d\n", line); exit(1); }
}
#define CHECK(x) Check((x), __LINE__)
static_assert(sizeof(void *) == 8, "native x86-64 oracle");
static_assert(sizeof(SceneValueQueue) == 72, "narrow value words");
static_assert(sizeof(ScenePreviewDataQueue) == 144, "native address words");

static void Publish(ScenePreviewDataQueue *queue, u8 **value)
{
#include "ScenePreviewDataPublish.inl"
}
static void PublishNull(ScenePreviewDataQueue *queue)
{
#include "ScenePreviewDataPublishNull.inl"
}
struct Packet
{
    u32 sequence, checksum;
    u8 bytes[32];
};
struct Handoff
{
    ScenePreviewDataQueue primary, secondary;
    SceneValueQueue primarySize, secondarySize, identifier;
    std::mutex lane4;
};
static void Fill(Packet *packet, u32 sequence)
{
    packet->sequence = sequence;
    packet->checksum = sequence ^ 0x5a39c7e1u;
    for (unsigned i = 0; i < sizeof(packet->bytes); ++i)
        packet->bytes[i] = static_cast<u8>(sequence + i);
}
static void Verify(const Packet *packet, u32 sequence)
{
    CHECK(packet && reinterpret_cast<uintptr_t>(packet) > 0xffffffffu);
    CHECK(packet->sequence == sequence && packet->checksum == (sequence ^ 0x5a39c7e1u));
    for (unsigned i = 0; i < sizeof(packet->bytes); ++i)
        CHECK(packet->bytes[i] == static_cast<u8>(sequence + i));
}
int main()
{
    // A controlled single producer/consumer scenario with the target's writer
    // mutex and empty-primary wait. This proves neither a global count bound
    // nor cancellation/release correctness of all game callers.
    Handoff state;
    const u32 messages = 512;
    std::thread producer([&] {
        for (u32 sequence = 1; sequence <= messages; ++sequence)
        {
            while (state.primary.Size() != 0) std::this_thread::yield();
            Packet *primary = new Packet;
            Packet *secondary = sequence % 4 ? new Packet : NULL;
            Fill(primary, sequence);
            if (secondary) Fill(secondary, sequence + messages);
            u8 *p = reinterpret_cast<u8 *>(primary);
            u8 *s = reinterpret_cast<u8 *>(secondary);
            // Packet ownership passes to the consumer. The producer never
            // touches it after publication, including while waiting for empty.
            std::lock_guard<std::mutex> lock(state.lane4);
            Publish(&state.primary, &p);
            state.primarySize.Push(sizeof(Packet));
            if (s) Publish(&state.secondary, &s);
            else PublishNull(&state.secondary);
            state.secondarySize.Push(s ? sizeof(Packet) : 0);
            state.identifier.Push(sequence % 2 ? static_cast<i32>(sequence) : -static_cast<i32>(sequence));
        }
    });
    for (u32 sequence = 1; sequence <= messages; ++sequence)
    {
        // Size is only an atomic status observation. Front's positive count
        // test and acquire fence establish publication before Packet reads.
        while (state.identifier.Size() == 0) std::this_thread::yield();
        CHECK(state.identifier.Front() == (sequence % 2 ? static_cast<i32>(sequence) : -static_cast<i32>(sequence)));
        CHECK(state.primarySize.Front() == sizeof(Packet));
        Packet *primary = reinterpret_cast<Packet *>(state.primary.Front());
        Packet *secondary = reinterpret_cast<Packet *>(state.secondary.Front());
        Verify(primary, sequence);
        if (sequence % 4)
        {
            Verify(secondary, sequence + messages);
            CHECK(state.secondarySize.Front() == sizeof(Packet));
        }
        else
        {
            CHECK(!secondary && state.secondary.Size() == 1);
            CHECK(state.secondarySize.Front() == 0);
        }
        // Preserve the UI's observed old-front release before advancing. Pop
        // returns a post-shift word, so it is deliberately not passed to delete.
        delete primary;
        delete secondary;
        std::lock_guard<std::mutex> lock(state.lane4);
        state.identifier.Pop();
        state.primary.Pop();
        state.primarySize.Pop();
        state.secondary.Pop();
        state.secondarySize.Pop();
    }
    producer.join();
    CHECK(state.primary.Size() == 0 && state.secondary.Size() == 0);
    CHECK(state.primarySize.Size() == 0 && state.secondarySize.Size() == 0);
    CHECK(state.identifier.Size() == 0);
    puts("queue publication: 512 real-thread high-address handoffs, signed identifiers, null secondary and old-front release passed");
}
