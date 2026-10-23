// Execute the shared sampler/producer with explicit clock, task and locking
// fixtures. These bindings do not declare the native Supervisor/GameWindow ABI.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include "inttypes.hpp"
#include "SupervisorFpsState.hpp"

#define CHECK(condition) do { if (!(condition)) { \
    std::fprintf(stderr, "FPS check failed at line %d: %s\n", __LINE__, #condition); \
    std::abort(); } } while (0)
#define __fastcall

namespace th095
{
struct FpsTaskFixture
{
    u32 captureActive : 1;
    u32 capturedPhotoActive : 1;
    u32 gameplayLoadActive : 1;
    u32 gameplayLoadFailed : 1;
    u32 resultScreenActive : 1;
    u32 playerDeathTransitionComplete : 1;
    u32 photoLimitTransitionComplete : 1;
    u32 resetFpsSample : 1;
    u32 unknownFlags8_31 : 24;
};

struct FpsWindowFixture
{
    struct CounterFixture { long long QuadPart; } performanceFrequency;
    f64 lastTimestamp, currentTimestamp, lastFrameTime, timeOrigin;
    f64 times[3];
    long long observedFrequency[3];
    i32 nextTime, timeCount;

    f64 GetTimestamp()
    {
        CHECK(nextTime < timeCount);
        observedFrequency[nextTime] = performanceFrequency.QuadPart;
        return times[nextTime++];
    }
};

struct FpsSupervisorFixture
{
    struct ConfigurationFixture { u8 frameskipConfig; } config;
    u32 fpsFrameCount;
    f32 currentFps;
    f64 lagNumerator, lagDenominator;
    u8 criticalSectionLockCounts[7];
    i32 lockDepth;
    void CalculateFps();
    static i32 __fastcall FinalizeFrame(FpsSupervisorFixture *s);
    void EnterCriticalSectionWrapper(i32 id)
    {
        CHECK(id == 5 && lockDepth == 0);
        CHECK(criticalSectionLockCounts[id] == 0);
        lockDepth = 1;
    }
    void LeaveCriticalSectionWrapper(i32 id)
    {
        CHECK(id == 5 && lockDepth == 1);
        CHECK(criticalSectionLockCounts[id] == 1);
        lockDepth = 0;
    }
};

struct FpsAnmFixture
{
    i32 flushCount;
    void FlushVertexBuffer() { ++flushCount; }
};

FpsSupervisorFixture g_Supervisor;
FpsWindowFixture g_GameWindow;
FpsTaskFixture *g_SupervisorGameTask;
FpsAnmFixture anm;
FpsAnmFixture *g_AnmManager = &anm;
i32 g_FpsClockAnomalyCount;
f64 g_LastFpsTimestamp;

#define Supervisor FpsSupervisorFixture
#include "SupervisorCalculateFps.inl"
#include "SupervisorFinalizeFrame.inl"
#undef Supervisor
}

using namespace th095;

static FpsTaskFixture task;
static i32 cases;

static void Reset(f64 previous, f64 now, u32 frames)
{
    std::memset(&g_Supervisor, 0, sizeof(g_Supervisor));
    std::memset(&g_GameWindow, 0, sizeof(g_GameWindow));
    std::memset(&task, 0, sizeof(task));
    g_SupervisorGameTask = &task;
    g_Supervisor.fpsFrameCount = frames;
    g_Supervisor.currentFps = -9.0f;
    g_Supervisor.lagNumerator = 2.0;
    g_Supervisor.lagDenominator = 3.0;
    g_FpsClockAnomalyCount = 0;
    g_LastFpsTimestamp = previous;
    g_GameWindow.performanceFrequency.QuadPart = 1LL << 40;
    g_GameWindow.lastTimestamp = -1.0;
    g_GameWindow.currentTimestamp = -2.0;
    g_GameWindow.lastFrameTime = -3.0;
    g_GameWindow.timeOrigin = -4.0;
    g_GameWindow.times[0] = now;
    g_GameWindow.times[1] = now + 10.0;
    g_GameWindow.timeCount = 2;
    anm.flushCount = 0;
}

static void SetFlags(u32 word)
{
    static_assert(sizeof(task) == sizeof(word), "fixture word width");
    std::memcpy(&task, &word, sizeof(word));
}

static u32 Flags()
{
    u32 word;
    std::memcpy(&word, &task, sizeof(word));
    return word;
}

static void TestSampleBoundaries()
{
    // A backwards clock moves the sample origin but leaves the pending sample.
    Reset(10.0, 9.0, 17); task.resetFpsSample = 1;
    g_Supervisor.CalculateFps(); ++cases;
    CHECK(g_LastFpsTimestamp == 9.0 && g_Supervisor.fpsFrameCount == 17);
    CHECK(g_Supervisor.currentFps == -9.0f && task.resetFpsSample == 1);
    CHECK(g_GameWindow.nextTime == 1);
    Reset(0.0, 0.499, 30); task.resetFpsSample = 1;
    g_Supervisor.CalculateFps(); ++cases;
    CHECK(g_LastFpsTimestamp == 0.0 && g_Supervisor.fpsFrameCount == 30);
    CHECK(task.resetFpsSample == 1 && g_Supervisor.lagDenominator == 3.0);
    Reset(0.0, 0.5, 30);
    g_Supervisor.CalculateFps(); ++cases;
    CHECK(g_LastFpsTimestamp == 0.5 && g_Supervisor.currentFps == 60.0f);
    CHECK(g_Supervisor.fpsFrameCount == 0);
    CHECK(g_Supervisor.lagNumerator == 62.0 && g_Supervisor.lagDenominator == 63.0);
    // Sampling also clears a pending reset when the frame count is zero.
    Reset(0.0, 1.0, 0); task.resetFpsSample = 1;
    g_Supervisor.CalculateFps(); ++cases;
    CHECK(g_Supervisor.currentFps == 0.0f && task.resetFpsSample == 0);
    CHECK(g_Supervisor.lagNumerator == 2.0 && g_Supervisor.lagDenominator == 3.0);
}

static void TestTaskGateAndLagThreshold()
{
    // Enumerate all 256 low-bit combinations, preserving every upper bit.
    for (u32 low = 0; low < 256; ++low)
    {
        Reset(0.0, 1.0, 58);
        const u32 original = 0xa5a55a00u | low;
        SetFlags(original);
        g_Supervisor.CalculateFps(); ++cases;
        const bool allowed = (low & 0xf5u) == 0;
        CHECK(g_Supervisor.lagNumerator == (allowed ? 62.0 : 2.0));
        CHECK(g_Supervisor.lagDenominator == (allowed ? 63.0 : 3.0));
        CHECK(Flags() == (original & ~0x80u));
        CHECK(g_Supervisor.currentFps == 58.0f && g_Supervisor.fpsFrameCount == 0);
    }
    for (u32 frames = 0; frames <= 65; ++frames)
    {
        Reset(0.0, 1.0, frames);
        g_Supervisor.CalculateFps(); ++cases;
        CHECK(g_Supervisor.lagNumerator == 2.0 + (frames > 57 ? 60.0 : frames));
        CHECK(g_FpsClockAnomalyCount == 0 && g_GameWindow.nextTime == 1);
    }
    Reset(0.0, 1.0, 60); g_SupervisorGameTask = NULL;
    g_Supervisor.CalculateFps(); ++cases;
    CHECK(g_Supervisor.lagNumerator == 2.0 && g_Supervisor.lagDenominator == 3.0);
    CHECK(g_Supervisor.fpsFrameCount == 0 && g_LastFpsTimestamp == 1.0);
}

static void TestClockAnomalySequence()
{
    for (i32 prior = 0; prior < 4; ++prior)
    {
        Reset(0.0, 1.0, 66);
        g_FpsClockAnomalyCount = prior;
        g_Supervisor.CalculateFps(); ++cases;
        const bool resync = prior == 1 || prior == 3;
        CHECK(g_FpsClockAnomalyCount == (prior == 3 ? 0 : prior + 1));
        CHECK(g_LastFpsTimestamp == 1.0); // not the second timestamp (11)
        CHECK(g_GameWindow.nextTime == (resync ? 2 : 1));
        CHECK(g_GameWindow.lastTimestamp == (resync ? 11.0 : -1.0));
        CHECK(g_GameWindow.currentTimestamp == (resync ? 11.0 : -2.0));
        CHECK(g_GameWindow.lastFrameTime == (resync ? 11.0 : -3.0));
        CHECK(g_GameWindow.timeOrigin == (resync ? 11.0 : -4.0));
        CHECK(g_GameWindow.performanceFrequency.QuadPart == (prior == 3 ? 0 : 1LL << 40));
        if (resync) CHECK(g_GameWindow.observedFrequency[1] == (prior == 3 ? 0 : 1LL << 40));
    }
    Reset(0.0, 1.0, 65); g_FpsClockAnomalyCount = 3;
    g_Supervisor.CalculateFps(); ++cases;
    CHECK(g_FpsClockAnomalyCount == 0 && g_GameWindow.nextTime == 1);
}

static void TestSamplerReceiverOwnership()
{
    Reset(0.0, 0.5, 999);
    FpsSupervisorFixture receiver = {};
    receiver.fpsFrameCount = 30;
    receiver.CalculateFps(); ++cases;
    CHECK(receiver.currentFps == 60.0f && receiver.fpsFrameCount == 0);
    CHECK(receiver.lagNumerator == 60.0 && receiver.lagDenominator == 60.0);
    CHECK(g_Supervisor.fpsFrameCount == 999 && g_Supervisor.currentFps == -9.0f);
    CHECK(g_LastFpsTimestamp == 0.5 && g_GameWindow.nextTime == 1);
}

static void TestFinalizeFrameProducer()
{
    const u32 initial[] = {0u, 11u, UINT_MAX};
    for (u32 skip = 0; skip < 256; ++skip)
        for (u32 count : initial)
        {
            Reset(0.0, 1.0, 999);
            FpsSupervisorFixture receiver = {};
            receiver.fpsFrameCount = count;
            // The global config is deliberately different from the receiver.
            g_Supervisor.config.frameskipConfig = static_cast<u8>(skip);
            receiver.config.frameskipConfig = static_cast<u8>(skip ^ 0xffu);
            CHECK(FpsSupervisorFixture::FinalizeFrame(&receiver) == 1); ++cases;
            CHECK(receiver.fpsFrameCount == count + skip + 1u);
            CHECK(receiver.lockDepth == 0 && receiver.criticalSectionLockCounts[5] == 0);
            CHECK(g_Supervisor.fpsFrameCount == 999 && anm.flushCount == 1);
        }
    // Feed the actual frame producer into the sampler for every config byte.
    for (u32 skip = 0; skip < 256; ++skip)
    {
        Reset(0.0, 1.0, 0); g_Supervisor.config.frameskipConfig = static_cast<u8>(skip);
        CHECK(FpsSupervisorFixture::FinalizeFrame(&g_Supervisor) == 1);
        g_Supervisor.CalculateFps(); ++cases;
        CHECK(g_Supervisor.currentFps == static_cast<f32>(skip + 1));
        CHECK(g_Supervisor.fpsFrameCount == 0);
    }
}

int main()
{
    static_assert(sizeof(void *) == 8, "native x86-64 component oracle");
    TestSampleBoundaries(); TestTaskGateAndLagThreshold();
    TestClockAnomalySequence(); TestSamplerReceiverOwnership(); TestFinalizeFrameProducer();
    std::printf("shared FPS sampler/producer: %d scenarios passed\n", cases);
}
