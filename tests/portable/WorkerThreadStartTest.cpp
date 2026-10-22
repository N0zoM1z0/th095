#include "WorkerThreadStart.hpp"
#include <errno.h>
#include <memory>
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <type_traits>
#include <vector>
using namespace th095;
static void Check(bool ok, int line) { if (!ok) { fprintf(stderr,"worker entry failed at %d\n",line); exit(1); } }
#define CHECK(x) Check((x),__LINE__)
static_assert(sizeof(void *)==8,"native x64 required");
static_assert(sizeof(WorkerThreadStart)==16,"two native addresses");
enum Mode { ALLOCATION_FAILURE, BEGIN_FAILURE, DEFERRED, EARLY, NULL_CALLBACK };
struct UserData { int value; };
struct Model {
    Mode mode;
    WorkerThreadStart *live;
    CrtWorkerEntry entry;
    void *packet;
    void *expectedArgument;
    uintptr_t handle;
    unsigned allocations, releases, begins, callbacks, waits, closes, result;
    int expectedValue;
    bool completed;
    std::vector<char> events;
    Model(Mode m, void *argument, int value):mode(m),live(0),entry(0),packet(0),expectedArgument(argument),
      handle(0x123456789abcdef0ULL),allocations(0),releases(0),begins(0),callbacks(0),waits(0),closes(0),
      result(99),expectedValue(value),completed(false) {}
};
static Model *model;
static void Invoke() {
    // Join establishes visibility for this fixture's bookkeeping/user result.
    // The callback never accesses worker fields; this is not a worker-race test.
    std::thread host([]{ model->result=model->entry(model->packet); });
    host.join();
    model->completed=true;
}
namespace th095 {
static WorkerThreadStart *AllocateWorkerThreadStart() {
    ++model->allocations; model->events.push_back('A');
    if (model->mode==ALLOCATION_FAILURE) return NULL;
    model->live=new (std::nothrow) WorkerThreadStart;
    CHECK(model->live && reinterpret_cast<uintptr_t>(model->live)>0xffffffffu);
    return model->live;
}
static void ReleaseWorkerThreadStart(WorkerThreadStart *start) {
    CHECK(start && model->live==start && model->releases==0);
    ++model->releases; model->events.push_back('F'); model->live=NULL;
    // Expose an entry that rereads packet fields after releasing ownership.
    memset(start,0xa5,sizeof(*start)); delete start;
}
static uintptr_t BeginBoundWorkerThread(CrtWorkerEntry entry,void *argument,u32 *threadId) {
    ++model->begins; model->events.push_back('B');
    CHECK(threadId);
    if (entry == NULL) {
        CHECK(model->mode==NULL_CALLBACK && argument==model->expectedArgument);
        CHECK(!model->live && !model->allocations);
        errno=EINVAL; return 0;
    }
    CHECK(argument==model->live);
    CHECK(model->live->argument==model->expectedArgument);
    *threadId=0x89abcdefu;
    if (model->mode==BEGIN_FAILURE) return 0;
    model->entry=entry; model->packet=argument;
    if (model->mode==EARLY) Invoke();
    return model->handle;
}
#include "WorkerThreadStart.inl"
}
static_assert(std::is_same<decltype(&th095::RunWorkerThread),CrtWorkerEntry>::value,"typed CRT slot");
typedef void *HANDLE;
const u32 WAIT_TIMEOUT=0x102;
static u32 WaitForSingleObject(HANDLE handle,u32 timeout) {
    CHECK(model->completed && reinterpret_cast<uintptr_t>(handle)==model->handle && timeout==200);
    ++model->waits; model->events.push_back('W'); return 0;
}
static i32 CloseHandle(HANDLE handle) {
    CHECK(reinterpret_cast<uintptr_t>(handle)==model->handle); ++model->closes; model->events.push_back('H'); return 1;
}
static void Sleep(u32) { CHECK(false); }
namespace th095 {
#include "ReplayScanWorkerBehavior.inl"
}
static void RunCallback(void *argument,int value) {
    CHECK(argument==model->expectedArgument && model->live==NULL && model->releases==1);
    CHECK(model->expectedValue==value); ++model->callbacks; model->events.push_back('C');
    if (argument) static_cast<UserData *>(argument)->value=value;
}
static void TH095_FASTCALL First(void *argument) { RunCallback(argument,7); }
static void TH095_FASTCALL Second(void *argument) { RunCallback(argument,11); }
int main() {
    unsigned cases=0;
    for (int mode=0;mode<5;++mode) for (int nullArgument=0;nullArgument<2;++nullArgument) for (int second=0;second<(mode==NULL_CALLBACK?1:2);++second) {
        std::unique_ptr<UserData> data(new UserData{0});
        CHECK(reinterpret_cast<uintptr_t>(data.get())>0xffffffffu);
        Model state(static_cast<Mode>(mode),nullArgument?NULL:data.get(),second?11:7); model=&state;
        ReplayScanWorker worker;
        memset(worker.unknown010,0x5d,sizeof(worker.unknown010));
        worker.threadId=0x12345678; worker.active=-3; worker.exitSignal=-4;
        WorkerCallback callback=mode==NULL_CALLBACK?NULL:(second?Second:First);
        errno=0;
        worker.Start(callback,state.expectedArgument);
        CHECK(worker.active==1 && worker.exitSignal==0 && worker.threadProc==callback);
        if (mode==NULL_CALLBACK) {
            CHECK(!state.allocations && !state.releases && !state.callbacks && state.begins==1);
            CHECK(!worker.threadHandle && worker.threadId==0x12345678 && errno==EINVAL);
            CHECK(state.events==std::vector<char>{'B'});
        } else if (mode==ALLOCATION_FAILURE) {
            CHECK(state.allocations==1);
            CHECK(!worker.threadHandle && worker.threadId==0x12345678 && !state.begins && !state.releases && !state.callbacks);
            CHECK(state.events==std::vector<char>{'A'});
        } else if (mode==BEGIN_FAILURE) {
            CHECK(state.allocations==1);
            CHECK(!worker.threadHandle && worker.threadId==0x89abcdef && state.begins==1 && state.releases==1 && !state.callbacks);
            CHECK((state.events==std::vector<char>{'A','B','F'}));
        } else {
            CHECK(state.allocations==1);
            CHECK(worker.threadHandle==state.handle && worker.threadId==0x89abcdef && state.begins==1);
            if (mode==DEFERRED) {
                CHECK(!state.releases && !state.callbacks && state.live);
                // The transport owns a launch-time snapshot, independently of
                // later edits to the worker's callback slot.
                worker.threadProc=second?First:Second;
                Invoke();
            }
            CHECK(state.completed && state.result==0 && state.releases==1 && state.callbacks==1);
            CHECK((state.events==std::vector<char>{'A','B','F','C'}));
            CHECK(data->value==(nullArgument?0:state.expectedValue));
            worker.Stop();
            CHECK(!worker.threadHandle && !worker.threadProc && worker.active==0 && worker.exitSignal==1);
            CHECK(state.waits==1 && state.closes==1);
        }
        for (unsigned i=0;i<sizeof(worker.unknown010);++i) CHECK(worker.unknown010[i]==0x5d);
        CHECK(!state.live); ++cases;
    }
    CHECK(cases==18);
    puts("worker entry: typed native callback, high addresses, early/deferred host dispatch null-callback validation and failure ownership passed");
}
