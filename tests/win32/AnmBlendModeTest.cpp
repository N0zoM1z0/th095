// Actual normal VM producers and destination-blend cache transitions.
#include "AnmManager.hpp"
#include "ViewportDeviceFixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(v) do { if (!(v)) { fprintf(stderr, "ANM blend line %d: %s\n", __LINE__, #v); fflush(stderr); abort(); } } while (0)
using namespace th095;

static unsigned int CheckScriptProducer()
{
    const int arguments[] = {-2147483647-1,-10001,-10000,-9999,-3,-2,-1,0,1,2,3,4,5,6,7,2147483647};
    const unsigned short masks[] = {0,1,0xffff};
    const unsigned int seeds[] = {0x00000007,0xa5a5a5a5,0x5a5a5a5a,0xffffffff};
    const float savedSpeed = g_AnmGameSpeed;
    unsigned int cases = 0;
    for (unsigned int arg = 0; arg < 16; ++arg)
    for (unsigned int mask = 0; mask < 3; ++mask)
    for (unsigned int old = 0; old < 4; ++old)
    {
        AnmVm vm; CHECK(vm.blendMode == 0);
        AnmRawInstr script[2]; memset(script,0,sizeof(script));
        script[0].opcode = ANM_OP_BLEND_MODE;
        script[0].instructionSize = sizeof(AnmRawInstr);
        script[0].varMask = masks[mask]; script[0].intArgs[0] = arguments[arg];
        script[1].opcode = ANM_OP_STATIC;
        script[1].instructionSize = sizeof(AnmRawInstr);
        vm.flagsWord = (seeds[old] & ~0x30u) | old << 4;
        vm.scriptDisabled = false;
        const unsigned int before = vm.flagsWord;
        // -10000 normally denotes I0; opcode 66 consumes the literal even
        // when varMask requests indirection, so this value must not win.
        vm.intVar0 = 3; vm.beginningOfScript = vm.currentInstruction = script;
        g_AnmGameSpeed = 0.625f;
        CHECK(AnmManager::ExecuteScript(&vm) == 1);
        const unsigned int value = static_cast<unsigned int>(arguments[arg]) & 3;
        CHECK(vm.flagsWord == ((before & ~0x30u) | value << 4));
        CHECK(vm.blendMode == value && vm.currentInstruction == NULL);
        CHECK(g_AnmGameSpeed == 0.625f);
        ++cases;
    }
    g_AnmGameSpeed = savedSpeed;
    return cases;
}

static unsigned int CheckAdditiveProducer()
{
    const unsigned int seeds[] = {0,0xa5a5a5a5,0x5a5a5a5a,0xffffffff};
    unsigned int cases = 0;
    for (unsigned int seed = 0; seed < 4; ++seed)
    for (unsigned int old = 0; old < 4; ++old)
    {
        AnmVm vm; vm.flagsWord = (seeds[seed] & ~0x30u) | old << 4;
        const unsigned int before = vm.flagsWord;
        vm.SetBlendModeAdditive();
        CHECK(vm.flagsWord == ((before & ~0x30u) | 0x10u));
        ++cases;
    }
    return cases;
}

static void SeedPending(AnmManager *manager, unsigned int pending)
{
    manager->ClearVertexBuffer(); manager->flushesThisFrame = 30;
    if (pending)
    {
        AnmVertex vertices[4]; memset(vertices,0,sizeof(vertices));
        for (unsigned int i = 0; i < 4; ++i)
        { vertices[i].rhw = 1; vertices[i].diffuse.color = 0xff123456u+i; }
        CHECK(manager->AddSpriteToDrawBuffer(vertices) == ZUN_SUCCESS);
    }
}

struct BlendObservation
{
    AnmManager *manager;
    unsigned int before, after, flushed;
};
static void __cdecl ObserveBlend(const ViewportDeviceCall *call, void *context)
{
    BlendObservation &o = *static_cast<BlendObservation *>(context);
    CHECK(o.manager->renderStateChangesThisFrame == 17);
    if (call->kind == VIEWPORT_CALL_VERTEX_SHADER || call->kind == VIEWPORT_CALL_DRAW)
    {
        CHECK(o.manager->cachedBlendMode == o.before);
        CHECK(o.manager->spritesToDraw == 1 && o.manager->flushesThisFrame == 30);
    }
    if (call->kind == VIEWPORT_CALL_RENDER_STATE)
    {
        CHECK(call->state == D3DRS_DESTBLEND && o.manager->cachedBlendMode == o.after);
        CHECK(o.manager->spritesToDraw == 0 && o.manager->flushesThisFrame == 30+(int)o.flushed);
    }
}

static unsigned int CheckTransitions(AnmManager *manager)
{
    unsigned int cases = 0;
    for (unsigned int incoming = 0; incoming < 4; ++incoming)
    for (unsigned int selector = 0; selector < 4; ++selector)
    for (unsigned int threeD = 0; threeD < 2; ++threeD)
    for (unsigned int pending = 0; pending < 2; ++pending)
    for (unsigned int failure = 0; failure < 2; ++failure)
    {
        AnmVm vm; vm.blendMode = selector; vm.useSecondaryColor = false;
        vm.color1.color = 0xff123456;
        SeedPending(manager,pending); manager->cachedBlendMode = incoming;
        manager->currentTextureFactor = vm.color1.color; manager->useMixColor = 0;
        manager->renderStateChangesThisFrame = 17;
        const unsigned int changed = incoming != selector;
        const unsigned int flushed = changed && pending;
        const unsigned int writes = changed && (selector < 2 || (threeD && selector == 2));
        ResetViewportDeviceCalls(failure ? E_FAIL : S_OK);
        BlendObservation observation = {manager,incoming,selector,flushed};
        SetViewportDeviceObserver(ObserveBlend,&observation);
        if (threeD) manager->SetRenderStateForVm3D(&vm);
        else manager->SetRenderStateForVm(&vm);
        SetViewportDeviceObserver(NULL,NULL);
        CHECK(manager->cachedBlendMode == selector && manager->renderStateChangesThisFrame == 18);
        CHECK(manager->spritesToDraw == (pending && !changed));
        CHECK(manager->flushesThisFrame == 30+(int)flushed);
        CHECK(ViewportDeviceCallCount() == flushed*4+writes);
        if (flushed)
        {
            CHECK(ViewportDeviceCallAt(0)->kind == VIEWPORT_CALL_TEXTURE_STATE);
            CHECK(ViewportDeviceCallAt(1)->kind == VIEWPORT_CALL_TEXTURE_STATE);
            CHECK(ViewportDeviceCallAt(2)->kind == VIEWPORT_CALL_VERTEX_SHADER);
            const ViewportDeviceCall *draw = ViewportDeviceCallAt(3);
            CHECK(draw->kind == VIEWPORT_CALL_DRAW && draw->state == D3DPT_TRIANGLELIST);
            CHECK(draw->count == 2 && draw->stride == 28 && draw->capturedVertexBytes == 168);
        }
        if (writes)
        {
            const ViewportDeviceCall *call = ViewportDeviceCallAt(flushed*4);
            CHECK(call->kind == VIEWPORT_CALL_RENDER_STATE && call->state == D3DRS_DESTBLEND);
            CHECK(call->value == (selector == 0 ? D3DBLEND_INVSRCALPHA : D3DBLEND_ONE));
        }
        ++cases;
    }
    return cases;
}

static unsigned int CheckInvalidation(AnmManager *manager)
{
    unsigned int cases = 0;
    for (unsigned int incoming = 0; incoming < 4; ++incoming)
    for (unsigned int pending = 0; pending < 2; ++pending)
    {
        SeedPending(manager,pending); manager->cachedBlendMode = incoming;
        unsigned char expected[16];
        memcpy(expected,reinterpret_cast<unsigned char *>(manager)+0x1760,16);
        expected[4] = 3;
        AnmVertex *end = manager->vertexBufferEndPtr;
        ResetViewportDeviceCalls(S_OK); manager->InvalidateBlendMode();
        CHECK(ViewportDeviceCallCount() == 0 && manager->cachedBlendMode == 3);
        CHECK(memcmp(expected,reinterpret_cast<unsigned char *>(manager)+0x1760,16) == 0);
        CHECK(manager->spritesToDraw == pending && manager->flushesThisFrame == 30);
        CHECK(manager->vertexBufferStartPtr == manager->vertexBuffer && manager->vertexBufferEndPtr == end);
        ++cases;
    }
    return cases;
}

static unsigned int CheckCrossedPaths(AnmManager *manager)
{
    unsigned int cases = 0;
    for (unsigned int first3D = 0; first3D < 2; ++first3D)
    for (unsigned int failure = 0; failure < 2; ++failure)
    {
        AnmVm vm; vm.blendMode = 2; vm.useSecondaryColor = false;
        SeedPending(manager,0); manager->cachedBlendMode = 3;
        manager->currentTextureFactor = vm.color1.color; manager->useMixColor = 0;
        ResetViewportDeviceCalls(failure ? E_FAIL : S_OK);
        if (first3D) manager->SetRenderStateForVm3D(&vm);
        else manager->SetRenderStateForVm(&vm);
        CHECK(ViewportDeviceCallCount() == first3D && manager->cachedBlendMode == 2);
        ResetViewportDeviceCalls(failure ? E_FAIL : S_OK);
        if (first3D) manager->SetRenderStateForVm(&vm);
        else manager->SetRenderStateForVm3D(&vm);
        CHECK(ViewportDeviceCallCount() == 0 && manager->cachedBlendMode == 2);
        ++cases;
    }
    return cases;
}

int __cdecl main()
{
    g_Supervisor.InitializeCriticalSections(); g_Supervisor.InitializeViewports();
    g_Supervisor.d3dDevice = ViewportTestDevice(0);
    AnmManager *manager = new AnmManager; g_AnmManager = manager;
    CHECK(manager->cachedBlendMode == 0);
    const unsigned int script = CheckScriptProducer(), additive = CheckAdditiveProducer();
    const unsigned int transitions = CheckTransitions(manager), invalidation = CheckInvalidation(manager), crossed = CheckCrossedPaths(manager);
    CHECK(script == 192 && additive == 16 && transitions == 128 && invalidation == 8 && crossed == 4);
    printf("ANM blend: %u script, %u additive, %u transition, %u invalidation and %u crossed-path cases passed\n",script,additive,transitions,invalidation,crossed);
    return 0;
}
