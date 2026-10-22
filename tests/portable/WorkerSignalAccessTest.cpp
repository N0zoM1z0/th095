#include "WorkerThreadStart.hpp"
#include <atomic>
#include <limits.h>
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>

using namespace th095;
static void Check(bool ok, int line)
{
    if (!ok) { fprintf(stderr, "worker signal failed at %d\n", line); exit(1); }
}
#define CHECK(x) Check((x), __LINE__)
static_assert(sizeof(void *) == 8, "native x86-64 oracle");
static_assert(alignof(ReplayScanWorker) >= 4, "aligned signal storage");

struct Fixture
{
    ReplayScanWorker *worker;
    std::thread host;
    std::atomic<bool> entered, observedRequest, allowCompletion, allowReturn, finished;
    int payload, expected;
    unsigned waits, closes;
    Fixture(ReplayScanWorker *w, int value) : worker(w), entered(false),
        observedRequest(false), allowCompletion(false), allowReturn(false),
        finished(false), payload(0), expected(value), waits(0), closes(0) {}
};
static Fixture *fixture;
static const uintptr_t handleValue = 0x123456789abcdef0ULL;

namespace th095
{
static WorkerThreadStart *AllocateWorkerThreadStart() { return new WorkerThreadStart; }
static void ReleaseWorkerThreadStart(WorkerThreadStart *start) { delete start; }
static uintptr_t BeginBoundWorkerThread(CrtWorkerEntry entry, void *argument, u32 *id)
{
    *id = 0x89abcdef;
    fixture->host = std::thread([entry, argument] { CHECK(entry(argument) == 0); });
    return handleValue;
}
#include "WorkerThreadStart.inl"
}
typedef void *HANDLE;
static const u32 WAIT_TIMEOUT = 0x102;
static u32 WaitForSingleObject(HANDLE handle, u32 timeout)
{
    CHECK(reinterpret_cast<uintptr_t>(handle) == handleValue && timeout == 200);
    i32 exit, active;
    CHECK(ReadWorkerSignal(exit, fixture->worker->exitSignal) == 1);
    CHECK(ReadWorkerSignal(active, fixture->worker->active) == 0);
    ++fixture->waits;
    // This fixture models a successful wait only. Existing lifecycle tests
    // separately exercise timeout/failure. Join protects teardown bookkeeping.
    fixture->host.join();
    return 0;
}
static i32 CloseHandle(HANDLE handle)
{
    CHECK(reinterpret_cast<uintptr_t>(handle) == handleValue);
    CHECK(fixture->finished.load());
    ++fixture->closes;
    return 1;
}
static void Sleep(u32) { CHECK(false); }
namespace th095
{
#include "ReplayScanWorkerBehavior.inl"
}

static void Await(const std::atomic<bool> &gate)
{
    while (!gate.load()) std::this_thread::yield();
}
static void TH095_FASTCALL Cancel(void *argument)
{
    Fixture *state = static_cast<Fixture *>(argument);
    CHECK(state == fixture);
    state->entered.store(true);
    i32 exit, active;
    while (ReadWorkerSignal(exit, state->worker->exitSignal) == 0)
        std::this_thread::yield();
    CHECK(exit == 1 && ReadWorkerSignal(active, state->worker->active) == 1);
    state->observedRequest.store(true);
    Await(state->allowCompletion);
    state->payload = state->expected;
    WriteWorkerSignal(state->worker->active, 0);
    WriteWorkerSignal(state->worker->exitSignal, 1);
    Await(state->allowReturn);
    state->finished.store(true);
}
static void TH095_FASTCALL Publish(void *argument)
{
    Fixture *state = static_cast<Fixture *>(argument);
    CHECK(state == fixture);
    state->entered.store(true);
    state->payload = state->expected;
    WriteWorkerSignal(state->worker->active, 0);
    Await(state->allowCompletion);
    WriteWorkerSignal(state->worker->exitSignal, 1);
    Await(state->allowReturn);
    state->finished.store(true);
}

int main()
{
    // Full signed words, rather than bool conversion, with neighboring owner
    // fields retained. These initializations/assertions are single-threaded.
    {
        ReplayScanWorker worker;
        worker.threadProc = Publish;
        worker.threadId = 0xfedcba98;
        memset(worker.unknown010, 0xa7, sizeof(worker.unknown010));
        const i32 values[] = {0, 1, -1, INT_MIN, INT_MAX, 0x12345678};
        for (unsigned i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
        {
            i32 snapshot;
            WriteWorkerSignal(worker.exitSignal, values[i]);
            WriteWorkerSignal(worker.active, values[i]);
            CHECK(ReadWorkerSignal(snapshot, worker.exitSignal) == values[i]);
            CHECK(snapshot == values[i]);
            CHECK(ReadWorkerSignal(snapshot, worker.active) == values[i]);
            CHECK(worker.threadId == 0xfedcba98 && worker.threadProc == Publish);
            CHECK(worker.threadHandle == 0);
            for (unsigned j = 0; j < sizeof(worker.unknown010); ++j)
                CHECK(worker.unknown010[j] == 0xa7);
        }
    }
    for (int iteration = 0; iteration < 64; ++iteration)
        for (int cancel = 0; cancel < 2; ++cancel)
        {
            ReplayScanWorker worker;
            Fixture state(&worker, 1000 + iteration);
            fixture = &state;
            worker.Start(cancel ? Cancel : Publish, &state);
            Await(state.entered);
            i32 exit, active;
            if (cancel)
            {
                worker.RequestExit();
                Await(state.observedRequest);
                CHECK(ReadWorkerSignal(exit, worker.exitSignal) == 1);
                CHECK(ReadWorkerSignal(active, worker.active) == 1);
                CHECK(state.payload == 0 && !state.finished.load());
                state.allowCompletion.store(true);
            }
            // Only the callback writes active=0 until this read. Thus the
            // acquire that observes 0 sees its ordinary payload write. Stop
            // or a prior run's value would invalidate that inference.
            while (ReadWorkerSignal(active, worker.active) != 0)
                std::this_thread::yield();
            CHECK(state.payload == state.expected && !state.finished.load());
            if (!cancel)
            {
                CHECK(ReadWorkerSignal(exit, worker.exitSignal) == 0);
                state.allowCompletion.store(true);
                while (ReadWorkerSignal(exit, worker.exitSignal) != 1)
                    std::this_thread::yield();
                CHECK(state.payload == state.expected && !state.finished.load());
            }
            state.allowReturn.store(true);
            worker.Stop();
            CHECK(state.waits == 1 && state.closes == 1 && state.finished.load());
            CHECK(worker.threadHandle == 0 && worker.threadProc == NULL);
        }
    puts("worker signals: signed words and 128 real-thread request/publication/join scenarios passed");
}
