#include "WorkerThreadStart.hpp"
#include "ScenePreviewDataQueue.hpp"
#include "GameWindowRenderFixture.hpp"
#include <atomic>
#include <string.h>
#include <thread>
using namespace th095;
static_assert(sizeof(void *)==8,"native x86-64 oracle");
static_assert(sizeof(RenderResult)==4,"canonical result width");
struct View {
    ScenePreviewDataQueue groupPreviewDataQueue;
    struct {u32 assetLoadStopRequested;} flagBits;
};
struct Model {
    View view;
    std::thread host;
    std::atomic<bool> ready,allowReturn,finished;
    bool early,fail;
    u8 *oldBuffer,*stagedBuffer;
    unsigned waits,closes,result;
    std::atomic<unsigned> allocations,releases;
    Model(bool e,bool f):ready(false),allowReturn(false),finished(false),early(e),fail(f),
        oldBuffer(0),stagedBuffer(0),waits(0),closes(0),result(99),allocations(0),releases(0) {view.flagBits.assetLoadStopRequested=0;}
};
static Model *model;
static const uintptr_t handleValue=0x123456789abcdef0ULL;
namespace th095 {
static WorkerThreadStart *AllocateWorkerThreadStart() {++model->allocations;return new WorkerThreadStart;}
static void ReleaseWorkerThreadStart(WorkerThreadStart *start) {++model->releases;delete start;}
static uintptr_t BeginBoundWorkerThread(CrtWorkerEntry entry,void *argument,u32 *id) {
    if(model->fail)return 0;
    *id=0x89abcdef;
    model->host=std::thread([entry,argument]{model->result=entry(argument);});
    if(model->early)while(!model->ready.load())std::this_thread::yield();
    return handleValue;
}
#include "WorkerThreadStart.inl"
}
typedef void *HANDLE;
static const u32 WAIT_TIMEOUT=0x102;
static u32 WaitForSingleObject(HANDLE handle,u32 timeout) {
    RENDER_CHECK(reinterpret_cast<uintptr_t>(handle)==handleValue && timeout==200);
    ++model->waits;model->allowReturn.store(true);model->host.join();
    RENDER_CHECK(model->finished.load() && model->result==0);return 0;
}
static i32 CloseHandle(HANDLE handle) {RENDER_CHECK(reinterpret_cast<uintptr_t>(handle)==handleValue);++model->closes;return 1;}
namespace th095 {
void RenderFixtureSleep(u32 delay) {
    if(delay==0)++renderTrace.sleeps;
    else if(delay==10)model->ready.store(true);
    else RENDER_CHECK(false);
    std::this_thread::yield();
}
#define Sleep RenderFixtureSleep
#include "ReplayScanWorkerBehavior.inl"
#undef Sleep
#include "ScenePreviewDataQueue.inl"
static i32 AssetQueueSize(ScenePreviewDataQueue *queue) {return queue->Size();}
static u32 SceneSelectionAssetsSupervisorStopRequested() {return 0;}
static i32 SceneSelectionAssetsExitRequested() {i32 value;ReadWorkerSignal(value,g_Supervisor.replayScanWorker.exitSignal);return value;}
static void Publish(ScenePreviewDataQueue *queue,u8 **value) {
#include "ScenePreviewDataPublish.inl"
}
static void TH095_FASTCALL StagedAsset(void *argument) {
    RENDER_CHECK(argument==model && model->releases.load()==1);
    model->stagedBuffer=new u8[8];model->stagedBuffer[0]=0x22;
    struct {View *view;} locals={&model->view};
#define Sleep RenderFixtureSleep
#include "ScenePreviewDataWait.inl"
#undef Sleep
    // One staged load still publishes after the shared wait's exit break.
    Publish(&model->view.groupPreviewDataQueue,&model->stagedBuffer);
    WriteWorkerSignal(g_Supervisor.replayScanWorker.active,0);
    WriteWorkerSignal(g_Supervisor.replayScanWorker.exitSignal,1);
    if(model->early)model->ready.store(true);
    while(!model->allowReturn.load())std::this_thread::yield();
    model->finished.store(true);
}
}
static void Run(i32 calc,u8 skip,i8 initialFrames,int clock,bool early,bool fail) {
    Model state(early,fail);model=&state;renderTrace=RenderTrace();renderTrace.calcResult=calc;
    if(!early){state.oldBuffer=new u8[8];state.oldBuffer[0]=0x11;Publish(&state.view.groupPreviewDataQueue,&state.oldBuffer);}
    RenderDeviceFixture device;g_Supervisor.d3dDevice=&device;g_Supervisor.config.frameskipConfig=skip;
    g_Supervisor.replayScanWorker.Start(StagedAsset,&state);
    if(!fail)while(!state.ready.load())std::this_thread::yield();
    RENDER_CHECK(reinterpret_cast<uintptr_t>(&state)>0xffffffffu);
    if(early){i32 active;RENDER_CHECK(ReadWorkerSignal(active,g_Supervisor.replayScanWorker.active)==0);RENDER_CHECK(!state.finished.load());}
    RenderWindowFixture window;window.framesSinceRedraw=initialFrames;
    if(clock==1)window.lastFrameTime=1.2;
    if(clock==2)window.lastTimestamp=2.0;
    RenderResult result=window.Render();
    bool terminal=clock==0 && (calc==0 || calc==-1);
    RENDER_CHECK(result==(terminal?(calc==0?RENDER_RESULT_EXIT_SUCCESS:RENDER_RESULT_RESTART):RENDER_RESULT_KEEP_RUNNING));
    RENDER_CHECK(renderTrace.stops==(terminal?1u:0u));
    if(terminal){
        RENDER_CHECK(renderTrace.timestamps==1 && !renderTrace.presents && !renderTrace.draws && !renderTrace.sleeps);
        RENDER_CHECK(state.waits==(fail?0u:1u));
        if(!fail)RENDER_CHECK(state.finished.load() && !g_Supervisor.replayScanWorker.threadHandle);
    } else {
        RENDER_CHECK(!state.waits && !state.finished.load());
        if(clock){RENDER_CHECK(renderTrace.sleeps==1 && !renderTrace.audio && !renderTrace.flushes);}
        else {
            bool draw=skip+1<=initialFrames+1;
            RENDER_CHECK(renderTrace.timestamps==2 && renderTrace.presents==1 && renderTrace.audio==1 && renderTrace.viewports==1);
            RENDER_CHECK(renderTrace.draws==(draw?1u:0u) && renderTrace.flushes==(draw?2u:1u));
            RENDER_CHECK(renderTrace.begins==(draw?1u:0u) && renderTrace.textures==(draw?1u:0u) && renderTrace.ends==(draw?1u:0u));
            RENDER_CHECK(window.framesSinceRedraw==(draw?0:initialFrames+1));
        }
        // Fixture cleanup is separate from the observed Render call.
        g_Supervisor.replayScanWorker.Stop();
    }
    if(fail){
        i32 active,exit;RENDER_CHECK(ReadWorkerSignal(active,g_Supervisor.replayScanWorker.active)==1 && ReadWorkerSignal(exit,g_Supervisor.replayScanWorker.exitSignal)==0);
        RENDER_CHECK(state.releases.load()==1 && !state.waits && !state.closes);
    } else {
        RENDER_CHECK(state.waits==1 && state.closes==1 && state.releases.load()==1);
        RENDER_CHECK(reinterpret_cast<uintptr_t>(state.stagedBuffer)>0xffffffffu && state.stagedBuffer[0]==0x22);
        RENDER_CHECK(state.view.groupPreviewDataQueue.Size()==(early?1:2));
        // Capture the post-shift release argument without pretending Pop owns
        // the removed buffer. This is a constructed backlog, not game reachability.
        uintptr_t after=state.view.groupPreviewDataQueue.Pop();
        RENDER_CHECK(after==(early?0:reinterpret_cast<uintptr_t>(state.stagedBuffer)));
        while(state.view.groupPreviewDataQueue.Size())state.view.groupPreviewDataQueue.Pop();
    }
    delete[] state.oldBuffer;delete[] state.stagedBuffer;
}
int main() {
    const i32 results[]={0,-1,1,-2};
    for(unsigned i=0;i<4;++i){Run(results[i],0,0,0,false,false);Run(results[i],2,0,0,false,false);Run(results[i],2,2,0,false,false);}
    Run(0,0,0,1,false,false);Run(-1,0,0,2,false,false);
    Run(0,0,0,0,false,true);Run(-1,0,0,0,false,true);
    Run(0,0,0,0,true,false);Run(-1,0,0,0,true,false);
    puts("shared Render: 18 real-thread stop/publication/result/clock/frame-skip scenarios passed");
}
