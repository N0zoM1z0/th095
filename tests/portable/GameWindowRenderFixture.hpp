#pragma once
#include "GameWindowRenderResult.hpp"
#include "ReplayScanWorker.hpp"
#include "SupervisorViewportSlot.hpp"
#include "SupervisorFogState.hpp"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Test-only bindings for the shared method, not a native GameWindow,
// Supervisor, D3D device or whole-game ABI declaration.
namespace th095 {
static void RenderCheck(bool ok,int line) {
    if (!ok) { fprintf(stderr,"render fixture failed at %d\n",line); exit(1); }
}
#define RENDER_CHECK(x) RenderCheck((x),__LINE__)
struct RenderTrace {
    i32 calcResult;
    unsigned timestamps,presents,sleeps,flushes,clears,viewports,audio,draws;
    unsigned begins,textures,ends,fogCalls,stops;
    RenderTrace() : calcResult(1),timestamps(0),presents(0),sleeps(0),flushes(0),
        clears(0),viewports(0),audio(0),draws(0),begins(0),textures(0),ends(0),fogCalls(0),stops(0) {}
};
static RenderTrace renderTrace;
void RenderFixtureSleep(u32 delay);
struct RenderDeviceFixture {
    i32 BeginScene() {++renderTrace.begins;return 0;}
    i32 SetTexture(u32 stage,void *texture) {RENDER_CHECK(!stage && !texture);++renderTrace.textures;return 0;}
    i32 EndScene() {++renderTrace.ends;return 0;}
};
struct RenderSupervisorFixture {
    struct Config {u8 frameskipConfig;} config;
    ReplayScanWorker replayScanWorker;
    RenderDeviceFixture *d3dDevice;
    SupervisorFogCacheState fogState;
    void ConfigureGameplayViewport(i32 slot) {RENDER_CHECK(slot==SUPERVISOR_VIEWPORT_FULL_WINDOW);++renderTrace.viewports;}
    i32 DisableFog() {RENDER_CHECK(fogState==SUPERVISOR_FOG_CACHE_INVALID);++renderTrace.fogCalls;return 0;}
    void StopReplayScan() {++renderTrace.stops;replayScanWorker.Stop();}
    void ThreadClose() {RENDER_CHECK(false);}
};
struct RenderAnmFixture {
    void FlushVertexBuffer() {++renderTrace.flushes;}
    void ClearVertexBuffer() {++renderTrace.clears;}
};
struct RenderChainFixture {
    i32 RunCalcChain() {return renderTrace.calcResult;}
    i32 RunDrawChain() {++renderTrace.draws;return 0;}
};
struct RenderAudioFixture {i32 ProcessQueues() {++renderTrace.audio;return 0;}};
static RenderSupervisorFixture g_Supervisor;
static RenderAnmFixture renderAnm;
static RenderAnmFixture *g_AnmManager=&renderAnm;
static RenderChainFixture g_Chain;
static RenderAudioFixture g_SoundPlayer;
struct RenderWindowFixture {
    f64 currentTimestamp,lastTimestamp,lastFrameTime;
    i8 framesSinceRedraw;
    RenderWindowFixture() : currentTimestamp(0),lastTimestamp(0.8),lastFrameTime(0.9),framesSinceRedraw(0) {}
    f64 GetTimestamp() {++renderTrace.timestamps;return renderTrace.timestamps==1?1.0:1.02;}
    static void Present() {++renderTrace.presents;}
    RenderResult Render();
};
#define GameWindow RenderWindowFixture
#define Sleep RenderFixtureSleep
#include "GameWindowRender.inl"
#undef Sleep
#undef GameWindow
} // namespace th095
