#include "ReplayScanWorker.hpp"

#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <type_traits>
#include <vector>

using th095::ReplayScanWorker;
typedef void *HANDLE;
typedef void (TH095_FASTCALL *WorkerCallback)(void *);
const u32 WAIT_TIMEOUT = 0x102;

static void Check(bool condition, int line)
{
    if (!condition) { fprintf(stderr, "worker oracle failed at %d\n", line); exit(1); }
}
#define CHECK(c) Check((c), __LINE__)

static_assert(sizeof(void *) == 8, "Run the worker oracle as native x86-64");
static_assert(sizeof(ReplayScanWorker) == 0x20, "One native layout");
static_assert(sizeof(ReplayScanWorker::threadId) == 4, "Thread ID stays narrow");
static_assert(sizeof(ReplayScanWorker::exitSignal) == 4, "Signal stays narrow");
static_assert(sizeof(ReplayScanWorker::active) == 4, "Active stays narrow");

struct Event
{
    std::string operation;
    uintptr_t handle;
    u32 delay;
    i32 exitSignal;
    i32 active;
};
struct Fixture
{
    ReplayScanWorker *worker;
    std::vector<u32> waits;
    size_t nextWait;
    std::vector<Event> events;
    uintptr_t newHandle;
    WorkerCallback expectedCallback;
    void *expectedArgument;
    bool completeBeforeReturn;
    bool mutateOnTimeout;
    i32 closeResult;
    Fixture() : worker(0), nextWait(0), newHandle(0), expectedCallback(0),
        expectedArgument(0), completeBeforeReturn(false), mutateOnTimeout(false), closeResult(1) {}
    void Record(const char *operation, uintptr_t handle, u32 delay)
    {
        Event event = {operation, handle, delay, worker->exitSignal, worker->active};
        events.push_back(event);
    }
};
static Fixture *fixture;

static u32 WaitForSingleObject(HANDLE handle, u32 delay)
{
    CHECK(fixture && fixture->nextWait < fixture->waits.size());
    fixture->Record("wait", reinterpret_cast<uintptr_t>(handle), delay);
    u32 result = fixture->waits[fixture->nextWait++];
    if (result == WAIT_TIMEOUT && fixture->mutateOnTimeout)
    {
        fixture->worker->exitSignal = 0;
        fixture->worker->active = 1;
    }
    return result;
}
static void Sleep(u32 delay) { fixture->Record("sleep", 0, delay); }
static i32 CloseHandle(HANDLE handle)
{
    fixture->Record("close", reinterpret_cast<uintptr_t>(handle), 0);
    return fixture->closeResult;
}
static uintptr_t BeginWorkerThread(ReplayScanWorker *worker, void *argument)
{
    CHECK(fixture && worker == fixture->worker);
    CHECK(worker->threadProc == fixture->expectedCallback);
    CHECK(argument == fixture->expectedArgument);
    CHECK(worker->threadHandle == 0);
    fixture->Record("begin", 0, 0);
    worker->threadId = 0xabcdef01u;
    // Model completion publication during the API call without invoking the
    // historical Win32 callback cast or creating an OS thread.
    if (fixture->completeBeforeReturn)
    {
        worker->active = 0;
        worker->exitSignal = 1;
    }
    return fixture->newHandle;
}

namespace th095
{
#include "ReplayScanWorkerBehavior.inl"
}

static void TH095_FASTCALL CallbackA(void *) {}
static void TH095_FASTCALL CallbackB(void *) {}
static void Seed(ReplayScanWorker &worker, uintptr_t handle)
{
    worker.threadHandle = handle;
    worker.threadId = 0x76543210;
    worker.exitSignal = 0;
    worker.active = 1;
    worker.threadProc = CallbackA;
    for (int i=0; i<4; ++i) worker.unknown010[i] = static_cast<u8>(0xa0+i);
}
static void CheckRetained(const ReplayScanWorker &worker)
{
    CHECK(worker.threadId == 0x76543210);
    for (int i=0; i<4; ++i) CHECK(worker.unknown010[i] == 0xa0+i);
}
int main()
{
    const uintptr_t oldHandle = 0x123456789ull, nextHandle = 0xabcdef012ull;
    {
        Fixture f; fixture=&f;
        ReplayScanWorker worker; f.worker=&worker;
        CHECK(worker.threadHandle == 0 && worker.threadId == 0);
        CHECK(worker.exitSignal == 0 && worker.active == 0);
        Seed(worker, 0);
        worker.Stop();
        CHECK(f.events.empty() && worker.threadProc == CallbackA);
        CHECK(worker.exitSignal == 0 && worker.active == 1);
        CheckRetained(worker);
    }
    const u32 terminalResults[] = {0, 0x80, 0xffffffffu, 7};
    for (int timeouts=0; timeouts<=3; ++timeouts)
    {
        for (unsigned end=0; end<sizeof(terminalResults)/sizeof(terminalResults[0]); ++end)
        {
            Fixture f; fixture=&f;
            ReplayScanWorker worker; f.worker=&worker; Seed(worker,oldHandle);
            f.waits.assign(timeouts,WAIT_TIMEOUT); f.waits.push_back(terminalResults[end]);
            f.mutateOnTimeout=true; f.closeResult=0;
            worker.Stop();
            CHECK(worker.threadHandle == 0 && worker.threadProc == 0);
            CHECK(worker.exitSignal == 1 && worker.active == 0);
            CHECK(f.nextWait == f.waits.size()); CheckRetained(worker);
            CHECK(f.events.size() == static_cast<size_t>(2*timeouts+2));
            for (size_t i=0; i<f.events.size(); ++i)
            {
                const Event &event=f.events[i];
                CHECK(event.exitSignal == 1 && event.active == 0);
                if (i+1 == f.events.size()) CHECK(event.operation == "close" && event.handle == oldHandle);
                else if (i%2 == 0) CHECK(event.operation == "wait" && event.handle == oldHandle && event.delay == 200);
                else CHECK(event.operation == "sleep" && event.delay == 1);
            }
            size_t calls=f.events.size(); worker.Stop(); CHECK(f.events.size() == calls);
        }
    }
    std::unique_ptr<u8[]> argument(new u8[1]);
    CHECK(reinterpret_cast<uintptr_t>(argument.get()) > 0xffffffffu);
    for (int mode=0; mode<3; ++mode)
    {
        Fixture f; fixture=&f;
        ReplayScanWorker worker; f.worker=&worker; Seed(worker,oldHandle);
        f.waits.push_back(WAIT_TIMEOUT); f.waits.push_back(0);
        f.expectedCallback=CallbackB; f.expectedArgument=argument.get();
        f.newHandle=mode == 0 ? 0 : nextHandle;
        f.completeBeforeReturn=mode == 2;
        worker.Start(CallbackB,argument.get());
        CHECK(f.events.size() == 5);
        CHECK(f.events[0].operation=="wait" && f.events[1].operation=="sleep" &&
              f.events[2].operation=="wait" && f.events[3].operation=="close" && f.events[4].operation=="begin");
        CHECK(f.events.back().exitSignal==0 && f.events.back().active==1);
        CHECK(worker.threadHandle==f.newHandle && worker.threadId==0xabcdef01u && worker.threadProc==CallbackB);
        CHECK(worker.exitSignal==(mode==2 ? 1 : 0) && worker.active==(mode==2 ? 0 : 1));
        for (int i=0; i<4; ++i) CHECK(worker.unknown010[i]==0xa0+i);
        if (f.newHandle)
        {
            f.waits.push_back(0); worker.Stop();
            CHECK(worker.threadHandle==0 && worker.threadProc==0);
        }
        else
        {
            size_t calls=f.events.size(); worker.Stop();
            CHECK(f.events.size()==calls && worker.active==1 && worker.exitSignal==0 && worker.threadProc==CallbackB);
        }
    }
    // Destructor dispatch uses the same Stop body and native-width token.
    {
        Fixture f; fixture=&f;
        f.waits.push_back(0);
        { ReplayScanWorker worker; f.worker=&worker; Seed(worker,oldHandle); }
        CHECK(f.events.size()==2 && f.events[0].operation=="wait" && f.events[1].operation=="close");
    }
    puts("worker lifecycle: native handles/arguments, timeout/failure order, restart and completion traces passed");
}
