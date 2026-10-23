// Actual normal ScreenEffect, RNG, timer, Chain, Supervisor, ANM and D3DX.
// Typed SDK bindings record only the external D3D device boundary.
#include "AnmManager.hpp"
#include "Chain.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoGameTask.hpp"
#include "ScreenEffect.hpp"
#include "ViewportDeviceFixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(value) do { if (!(value)) { fprintf(stderr, "shake check line %d: %s\n", __LINE__, #value); fflush(stderr); abort(); } } while (0)
namespace th095 { extern i32 g_ScreenEffectCounter; }
using namespace th095;
static unsigned int cases;
static AnmManager *globalManager;
static AnmManager *drawReceiver;
static PhotoGameTaskView *task;
static u16 directionSeeds[3][3];
static bool calcObserved, beforeResetObserved, afterResetObserved;

static Float2 Pair(float x, float y) { Float2 p = {x, y}; return p; }
static bool Same(const Float2 &a, const Float2 &b) { return memcmp(&a, &b, sizeof(a)) == 0; }
static Float2 &Published() { return g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset; }
static Float2 &OtherSlot() { return g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW].screenShakeOffset; }
static float Direction(u32 direction, float magnitude) { return direction == 0 ? 0.0f : direction == 1 ? magnitude : -magnitude; }
static int Calculate(ScreenEffect *effect) { return effect->type == SCREEN_EFFECT_SHAKE ? ScreenEffect::CalcShake(effect) : ScreenEffect::CalcShakeEnvelope(effect); }

static void Prepare(ScreenEffect *effect, int current, float subFrame)
{
    effect->timer.previous = 912;
    effect->timer.current = current;
    effect->timer.subFrame = subFrame;
    Published() = Pair(71.0f, -83.0f);
    OtherSlot() = Pair(27.0f, -35.0f);
    globalManager->screenShakeOffset = Pair(101.0f, -109.0f);
    g_ScreenEffectCounter = 0;
    g_RuntimeGlobalStateOwner = task;
    task->flags = 0;
    g_AnmGameSpeed = 1.0f;
    g_Rng.seed = 1234;
    g_Rng.generationCount = 0;
}

static void FindDirectionSeeds()
{
    bool seen[3][3] = {};
    unsigned int count = 0;
    for (u32 seed = 0; seed < 65536 && count < 9; ++seed)
    {
        Rng expected = g_Rng;
        expected.seed = (u16)seed;
        const u32 x = expected.GetRandomU32() % 3;
        const u32 y = expected.GetRandomU32() % 3;
        if (!seen[x][y]) { seen[x][y] = true; directionSeeds[x][y] = (u16)seed; ++count; }
    }
    CHECK(count == 9);
}

static void CheckNoPublication(ScreenEffect *effect, int result, bool ticks)
{
    const Rng rng = g_Rng;
    const int previous = effect->timer.current;
    const float subFrame = effect->timer.subFrame;
    CHECK(Calculate(effect) == result);
    CHECK(memcmp(&rng, &g_Rng, sizeof(rng)) == 0);
    CHECK(Same(Published(), Pair(71.0f, -83.0f)));
    CHECK(Same(OtherSlot(), Pair(27.0f, -35.0f)));
    CHECK(Same(globalManager->screenShakeOffset, Pair(101.0f, -109.0f)));
    CHECK(effect->timer.previous == (ticks ? previous : 912));
    CHECK(effect->timer.current == previous + (ticks ? 1 : 0));
    CHECK(effect->timer.subFrame == subFrame + (ticks ? 1.0f : 0.0f));
    ++cases;
}

static void CheckDrawConsumer(const Float2 &offset, bool culled)
{
    AnmVm vm;
    AnmLoadedSprite sprite;
    memset(&sprite, 0, sizeof(sprite));
    sprite.uvStart = Pair(0.125f, 0.25f);
    sprite.uvEnd = Pair(0.75f, 0.875f);
    vm.loadedSprite = &sprite;
    vm.color1.color = 0xff123456;
    vm.blendMode = ANM_BLEND_ALPHA;
    globalManager->useMixColor = 0;
    globalManager->currentTexture = NULL;
    globalManager->cachedBlendMode = ANM_BLEND_ALPHA;
    globalManager->cachedDrawPath = ANM_DRAW_PATH_BUFFERED_QUADS;
    globalManager->spritesToDraw = 0;
    globalManager->vertexBufferStartPtr = globalManager->vertexBuffer;
    globalManager->vertexBufferEndPtr = globalManager->vertexBuffer;
    // Keep visible geometry inside the real inset viewport (PLAYFIELD X=128).
    const D3DVIEWPORT8 &viewport = g_Supervisor.currentViewportConfiguration->viewport;
    const float originX = culled ? -1024.0f : (float)viewport.X + 64.0f;
    const float originY = culled ? -1024.0f : (float)viewport.Y + 64.0f;
    for (unsigned int i = 0; i < 4; ++i)
    {
        g_AnmTexturedVertices[i].position.x = originX + (i & 1 ? 20.0f : 0.0f);
        g_AnmTexturedVertices[i].position.y = originY + (i & 2 ? 20.0f : 0.0f);
        g_AnmTexturedVertices[i].position.z = 0.25f;
        g_AnmTexturedVertices[i].rhw = 1.0f;
    }
    ResetViewportDeviceCalls(S_OK);
    CHECK(globalManager->DrawInner(&vm, 0) == ZUN_SUCCESS);
    for (unsigned int i = 0; i < 4; ++i)
    {
        CHECK(g_AnmTexturedVertices[i].position.x == originX + (i & 1 ? 20.0f : 0.0f) + offset.x);
        CHECK(g_AnmTexturedVertices[i].position.y == originY + (i & 2 ? 20.0f : 0.0f) + offset.y);
        CHECK(g_AnmTexturedVertices[i].position.z == 0.25f && g_AnmTexturedVertices[i].rhw == 1.0f);
        CHECK(g_AnmTexturedVertices[i].uv.x == (i & 1 ? 0.75f : 0.125f));
        CHECK(g_AnmTexturedVertices[i].uv.y == (i & 2 ? 0.875f : 0.25f));
        if (!culled) CHECK(g_AnmTexturedVertices[i].diffuse.color == 0xff123456);
    }
    CHECK(globalManager->spritesToDraw == (culled ? 0 : 1));
    CHECK(ViewportDeviceCallCount() == 0); // Ordinary cached/buffered draw path.
    if (!culled)
    {
        CHECK(globalManager->vertexBufferEndPtr == globalManager->vertexBuffer + 6);
        for (unsigned int i = 0; i < 6; ++i)
        {
            bool found = false;
            for (unsigned int j = 0; j < 4; ++j)
                if (memcmp(&globalManager->vertexBuffer[i], &g_AnmTexturedVertices[j], sizeof(AnmVertex)) == 0) found = true;
            CHECK(found);
        }
    }
}

static void CheckActive(ScreenEffect *effect, float magnitude, int expectedCurrent, float expectedSubFrame, u16 seed)
{
    const int previous = effect->timer.current;
    g_Rng.seed = seed;
    g_Rng.generationCount = 0;
    Rng expected = g_Rng;
    const u32 x = expected.GetRandomU32() % 3;
    const u32 y = expected.GetRandomU32() % 3;
    const Float2 offset = Pair(Direction(x, magnitude), Direction(y, magnitude));
    CHECK(Calculate(effect) == 1);
    CHECK(effect->timer.previous == previous && effect->timer.current == expectedCurrent);
    CHECK(effect->timer.subFrame == expectedSubFrame);
    CHECK(memcmp(&g_Rng, &expected, sizeof(expected)) == 0);
    CHECK(Same(Published(), offset) && Same(OtherSlot(), Pair(27.0f, -35.0f)));
    CHECK(Same(globalManager->screenShakeOffset, Pair(101.0f, -109.0f)));
    globalManager->spritesToDraw = 0;
    g_Supervisor.ConfigureGameplayViewport(SUPERVISOR_VIEWPORT_PLAYFIELD);
    CHECK(Same(globalManager->screenShakeOffset, offset));
    CheckDrawConsumer(offset, false);
    CheckDrawConsumer(offset, true);
    ++cases;
}

static int __fastcall ObserveCalc(void *)
{
    CHECK(Same(Published(), Pair(10.0f, -10.0f)));
    g_Supervisor.ConfigureGameplayViewport(SUPERVISOR_VIEWPORT_PLAYFIELD);
    calcObserved = true;
    return 1;
}
static int __fastcall ObserveBeforeReset(void *)
{
    CHECK(calcObserved && Same(Published(), Pair(10.0f, -10.0f)));
    CHECK(Same(globalManager->screenShakeOffset, Published()));
    beforeResetObserved = true;
    return 1;
}
static int __fastcall ObserveAfterReset(void *)
{
    CHECK(beforeResetObserved && Same(Published(), Pair(0.0f, 0.0f)));
    CHECK(Same(globalManager->screenShakeOffset, Pair(0.0f, 0.0f)));
    CHECK(Same(OtherSlot(), Pair(27.0f, -35.0f)));
    afterResetObserved = true;
    return 1;
}

int __cdecl main()
{
    CHECK(g_AnmManager == NULL && g_RuntimeGlobalStateOwner == NULL);
    g_Supervisor.InitializeCriticalSections();
    g_Supervisor.InitializeViewports();
    g_Supervisor.d3dDevice = ViewportTestDevice(0);
    globalManager = new AnmManager;
    drawReceiver = new AnmManager;
    g_AnmManager = globalManager;
    // Construct the real owner. Its destructive game teardown requires fully
    // loaded subsystems, outside this fixture; keep this 0x124 owner until exit.
    task = new PhotoGameTaskView;
    g_RuntimeGlobalStateOwner = task;
    ScreenEffect *linear = ScreenEffect::RegisterChain(SCREEN_EFFECT_SHAKE, 6, 4, 10, 0, 0);
    ScreenEffect *envelope = ScreenEffect::RegisterChain(SCREEN_EFFECT_SHAKE_ENVELOPE, 8, 4, 3, 4, 0);
    CHECK(linear && envelope && !linear->drawChainElement && !envelope->drawChainElement);
    CHECK(linear->calcChainElement->priority == 2 && envelope->calcChainElement->priority == 2);
    CHECK(linear->timer.current == 0 && linear->timer.subFrame == 0.0f && linear->timer.previous == -999999);
    FindDirectionSeeds();
    ScreenEffect *effects[] = {linear, envelope};
    const int gates[] = {1, -1, (-2147483647 - 1)};
    const u32 holdFlags[] = {1, 2, 4, 16, 32, 64, 0x77};
    for (unsigned int e = 0; e < 2; ++e)
    {
        for (unsigned int i = 0; i < 3; ++i) { Prepare(effects[e], 2, 2.0f); g_ScreenEffectCounter = gates[i]; CheckNoPublication(effects[e], 0, false); }
        Prepare(effects[e], 2, 2.0f); g_RuntimeGlobalStateOwner = NULL; CheckNoPublication(effects[e], 1, false);
        for (unsigned int i = 0; i < 7; ++i) { Prepare(effects[e], 2, 2.0f); task->flags = holdFlags[i]; CheckNoPublication(effects[e], 1, false); }
    }
    for (unsigned int x = 0; x < 3; ++x)
    for (unsigned int y = 0; y < 3; ++y)
    {
        for (int frame = 1; frame < 6; ++frame) { Prepare(linear, frame - 1, (float)(frame - 1)); CheckActive(linear, (float)(4 + frame), frame, (float)frame, directionSeeds[x][y]); }
        // Independent envelope timeline: four ramp frames, three hold frames,
        // four release frames; expiration occurs at 11 after ticking.
        const float magnitudes[] = {2, 4, 6, 8, 8, 8, 8, 6, 4, 2};
        for (int frame = 1; frame <= 10; ++frame) { Prepare(envelope, frame - 1, (float)(frame - 1)); CheckActive(envelope, magnitudes[frame - 1], frame, (float)frame, directionSeeds[x][y]); }
    }
    Prepare(linear, 5, 5.0f); CheckNoPublication(linear, 0, true);
    Prepare(envelope, 10, 10.0f); CheckNoPublication(envelope, 0, true);
    Prepare(linear, 0, 0.0f); g_AnmGameSpeed = 0.5f; CheckActive(linear, 4.5f, 0, 0.5f, directionSeeds[1][2]);
    Prepare(envelope, 7, 7.0f); g_AnmGameSpeed = 0.5f; CheckActive(envelope, 7.0f, 7, 7.5f, directionSeeds[2][1]);
    Prepare(linear, 0, 0.0f); g_AnmGameSpeed = 0.0f; CheckActive(linear, 4.0f, 0, 0.0f, directionSeeds[1][2]);
    Prepare(envelope, 0, 0.0f); g_AnmGameSpeed = 0.0f; CheckActive(envelope, 0.0f, 0, 0.0f, directionSeeds[2][1]);
    const u32 nonHoldFlags[] = {8, 128, 256, 512, 1024, 0x80000000u};
    for (unsigned int i = 0; i < 6; ++i) { Prepare(linear, 0, 0.0f); task->flags = nonHoldFlags[i]; CheckActive(linear, 5.0f, 1, 1.0f, directionSeeds[1][2]); }

    Prepare(linear, 0, 0.0f);
    drawReceiver->screenShakeOffset = Pair(-0.0f, 22.0f);
    u8 parentBefore[sizeof(Supervisor)]; memcpy(parentBefore, &g_Supervisor, sizeof(parentBefore));
    u8 *managerBefore = (u8 *)malloc(sizeof(AnmManager));
    CHECK(managerBefore != NULL); memcpy(managerBefore, globalManager, sizeof(AnmManager));
    AnmManager::DrawLayer6(drawReceiver);
    CHECK(Same(Published(), Pair(0.0f, 0.0f)) && Same(globalManager->screenShakeOffset, Pair(0.0f, 0.0f)));
    CHECK(Same(drawReceiver->screenShakeOffset, Pair(-0.0f, 22.0f)) && Same(OtherSlot(), Pair(27.0f, -35.0f)));
    CHECK(memcmp(parentBefore, &g_Supervisor, 0x2cc) == 0);
    CHECK(memcmp(parentBefore + 0x2d4, (const u8 *)&g_Supervisor + 0x2d4, sizeof(Supervisor) - 0x2d4) == 0);
    CHECK(memcmp(managerBefore, globalManager, 0x20) == 0);
    CHECK(memcmp(managerBefore + 0x28, (const u8 *)globalManager + 0x28, sizeof(AnmManager) - 0x28) == 0);
    free(managerBefore); ++cases;

    Prepare(linear, 0, 0.0f); linear->duration = 20; linear->rawParameter0 = linear->rawParameter1 = 10;
    envelope->shakeEnvelopeRampUpFrames = envelope->shakeEnvelopeHoldFrames = envelope->shakeEnvelopeRampDownFrames = 0;
    envelope->timer.SetCurrent(0);
    g_Rng.seed = directionSeeds[1][2];
    ChainElem *observer = g_Chain.CreateElem((ChainCallback)ObserveCalc); g_Chain.AddToCalcChain(observer, 3);
    observer = g_Chain.CreateElem((ChainCallback)ObserveBeforeReset); g_Chain.AddToDrawChain(observer, 0x10);
    observer = g_Chain.CreateElem((ChainCallback)ObserveAfterReset); g_Chain.AddToDrawChain(observer, 0x12);
    CHECK(g_Chain.RunCalcChain() > 0 && calcObserved);
    CHECK(g_Chain.RunDrawChain() > 0 && beforeResetObserved && afterResetObserved);
    CHECK(Same(Published(), Pair(0.0f, 0.0f)) && Same(OtherSlot(), Pair(27.0f, -35.0f)));
    CHECK(Same(globalManager->screenShakeOffset, OtherSlot())); // Actual layers 7/8 select FULL_WINDOW later.
    ++cases;
    g_RuntimeGlobalStateOwner = NULL;
    g_AnmManager = NULL;
    g_Supervisor.d3dDevice = NULL;
    delete drawReceiver;
    delete globalManager;
    printf("canonical shake: %u gate/timer/RNG/publication/draw/reset/actual-chain cases passed\n", cases);
    return 0;
}
