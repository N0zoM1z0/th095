#include "FrontEndGlobals.hpp"
#include "ScenePreviewDataQueue.hpp"
#include <array>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace th095 {
#include "ScenePreviewDataQueue.inl"
}
using namespace th095;
static_assert(sizeof(void *) == 8, "Require native x86-64 addresses");

static void Check(bool condition, int line) {
    if (!condition) { fprintf(stderr, "asset wait oracle failed at %d\n", line); exit(1); }
}
#define CHECK(x) Check((x), __LINE__)

// A fixture for the bounded protocol components, not a controller ABI layout.
struct WaitController {
    FrontEndControllerFlagBits flagBits;
    ScenePreviewDataQueue groupPreviewDataQueue;
    WaitController() : flagBits() {}
};
struct WaitFixture {
    WaitController *controller;
    unsigned signalMask;
    unsigned triggerSleep;
    i32 exitValue;
    u32 closeRequested;
    i32 exitSignal;
    unsigned sleeps;
    std::vector<char> events;
    WaitFixture(WaitController &owner, unsigned mask, unsigned trigger, i32 value)
        : controller(&owner), signalMask(mask), triggerSleep(trigger), exitValue(value),
          closeRequested(0), exitSignal(0), sleeps(0) {}
};
static WaitFixture *fixture;
static i32 AssetQueueSize(ScenePreviewDataQueue *queue) {
    fixture->events.push_back('Q');
    return queue->Size();
}
static void Sleep(u32 milliseconds) {
    CHECK(milliseconds == 10);
    fixture->events.push_back('S');
    CHECK(++fixture->sleeps <= 2);
    if (fixture->sleeps != fixture->triggerSleep) return;
    fixture->closeRequested = (fixture->signalMask & 1) != 0;
    fixture->controller->flagBits.assetLoadStopRequested = (fixture->signalMask & 2) != 0;
    fixture->exitSignal = (fixture->signalMask & 4) ? fixture->exitValue : 0;
    if (fixture->signalMask == 0) {
        // Model an independent consumer; never free Pop's raw return value.
        while (fixture->controller->groupPreviewDataQueue.Size() != 0)
            fixture->controller->groupPreviewDataQueue.Pop();
    }
}
static u32 SceneSelectionAssetsSupervisorStopRequested() {
    fixture->events.push_back('C');
    return fixture->closeRequested;
}
static i32 SceneSelectionAssetsLoadComplete() {
    fixture->events.push_back('E');
    return fixture->exitSignal;
}
static void Publish(ScenePreviewDataQueue *queue, u8 **value) {
#include "ScenePreviewDataPublish.inl"
}
static void WaitThenPublish(WaitController &controller, u8 *data) {
    struct { WaitController *view; } locals = {&controller};
#include "ScenePreviewDataWait.inl"
    fixture->events.push_back('P');
    Publish(&controller.groupPreviewDataQueue, &data);
}

// Independent trace derived from the target's branch destinations. Q is a
// queue-count read, S Sleep(10), C the close read, E the exit read, P publication.
static std::vector<char> ExpectedTrace(int count, unsigned mask, unsigned trigger) {
    std::vector<char> trace(1, 'Q');
    if (count != 0) {
        for (unsigned iteration=1; iteration<=trigger; ++iteration) {
            trace.push_back('S'); trace.push_back('C');
            if (iteration == trigger && (mask & 3)) break;
            trace.push_back('E');
            if (iteration == trigger && (mask & 4)) break;
            trace.push_back('Q');
        }
    }
    trace.push_back('P');
    return trace;
}
int main() {
    std::array<std::unique_ptr<u8[]>,17> buffers;
    for (unsigned i=0; i<buffers.size(); ++i) {
        buffers[i].reset(new u8[8]);
        CHECK(reinterpret_cast<uintptr_t>(buffers[i].get()) > 0xffffffffu);
    }
    const int counts[] = {0, 1, 2, 16};
    const i32 exitValues[] = {1, -1};
    unsigned cases=0;
    for (unsigned ci=0; ci<4; ++ci) {
        const int count=counts[ci];
        for (unsigned mask=0; mask<8; ++mask) {
            for (unsigned trigger=1; trigger<=2; ++trigger) {
                for (unsigned vi=0; vi<2; ++vi) {
                    WaitController controller;
                    // Neighboring controller flags do not cancel the wait.
                    controller.flagBits.previewPending=1;
                    controller.flagBits.unknownFlagBit4=1;
                    controller.flagBits.unknownFlagBits6=0x3ffffff;
                    for (int i=0; i<count; ++i) {
                        u8 *item=buffers[i].get(); Publish(&controller.groupPreviewDataQueue, &item);
                    }
                    WaitFixture state(controller, mask, trigger, exitValues[vi]); fixture=&state;
                    WaitThenPublish(controller, buffers[16].get());
                    CHECK(state.events == ExpectedTrace(count, mask, trigger));
                    CHECK(state.sleeps == (count ? trigger : 0));
                    int prior=(count && mask) ? count : 0;
                    CHECK(controller.groupPreviewDataQueue.Size() == (prior<16 ? prior+1 : 16));
                    if (prior) {
                        for (int i=0; i<prior; ++i)
                            CHECK(controller.groupPreviewDataQueue.words[i].bits == reinterpret_cast<uintptr_t>(buffers[i].get()));
                    }
                    if (prior<16)
                        CHECK(controller.groupPreviewDataQueue.words[prior].bits == reinterpret_cast<uintptr_t>(buffers[16].get()));
                    CHECK(controller.flagBits.previewPending == 1 && controller.flagBits.unknownFlagBit4 == 1);
                    CHECK(controller.flagBits.unknownFlagBits6 == 0x3ffffff);
                    u32 rawFlags=0;
                    memcpy(&rawFlags, &controller.flagBits, sizeof(rawFlags));
                    CHECK(rawFlags == (0xffffffd4u | ((count && (mask & 2)) ? 0x20u : 0)));
                    ++cases;
                }
            }
        }
    }
    CHECK(cases == 128);
    // A flag arriving after the caller's outer check still cannot suppress
    // publication when the result queue is empty: no wait predicates run.
    WaitController empty;
    empty.flagBits.assetLoadStopRequested=1;
    WaitFixture stopped(empty, 7, 1, -1); fixture=&stopped;
    stopped.closeRequested=1; stopped.exitSignal=-1;
    WaitThenPublish(empty, buffers[16].get());
    CHECK((stopped.events == std::vector<char>{'Q','P'}));
    CHECK(empty.groupPreviewDataQueue.Size() == 1);
    u32 rawFlags=0;
    memcpy(&rawFlags, &empty.flagBits, sizeof(rawFlags));
    CHECK(rawFlags == 0x20u);
    puts("asset wait: native cancellation order, final publication and backlog traces passed");
}
