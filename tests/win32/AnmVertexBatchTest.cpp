// Actual normal batch producer, shader/color/cull transitions and SDK I/O.
#include "AnmManager.hpp"
#include "ViewportDeviceFixture.h"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(v) do { if (!(v)) { fprintf(stderr, "ANM batch line %d: %s\n", __LINE__, #v); fflush(stderr); abort(); } } while (0)
using namespace th095;

// Independent FVF_XYZRHW | FVF_DIFFUSE | FVF_TEX1 decoding, not a source view.
struct WireVertex { float x, y, z, rhw; unsigned int color; float u, v; };
typedef char WireIs28[sizeof(WireVertex) == 28 ? 1 : -1];
static const unsigned int triangleOrder[6] = {0,1,2,1,2,3};

static void FillWire(AnmVertex *quad, unsigned int seed)
{
    // Include signed zero, infinities and NaN payloads only in byte-copy tests.
    // No real driver or arithmetic on these diagnostic packets is exercised.
    const unsigned int samples[] = {0,0x80000000,0x3f800000,0xbf800000,
        0x7f800000,0xff800000,0x7fc12345,0x3e800000};
    for (unsigned int i = 0; i < 4; ++i)
    {
        unsigned int words[7];
        for (unsigned int j = 0; j < 7; ++j) words[j] = samples[(seed+i+j)%8];
        words[4] = 0x10203040u ^ (seed * 0x01010101u) ^ (i * 0x11223344u);
        memcpy(&quad[i], words, sizeof(words));
    }
}
static void Expand(unsigned char *output, const AnmVertex *quad)
{
    for (unsigned int i = 0; i < 6; ++i) memcpy(output+i*28, &quad[triangleOrder[i]], 28);
}
static void CheckFlushCalls(AnmManager *manager, const void *first,
    unsigned int sprites, const unsigned char *bytes)
{
    CHECK(ViewportDeviceCallCount() == 4);
    CHECK(ViewportDeviceCallAt(0)->kind == VIEWPORT_CALL_TEXTURE_STATE);
    CHECK(ViewportDeviceCallAt(0)->state == D3DTSS_ALPHAARG2 && ViewportDeviceCallAt(0)->value == D3DTA_DIFFUSE);
    CHECK(ViewportDeviceCallAt(1)->kind == VIEWPORT_CALL_TEXTURE_STATE);
    CHECK(ViewportDeviceCallAt(1)->state == D3DTSS_COLORARG2 && ViewportDeviceCallAt(1)->value == D3DTA_DIFFUSE);
    CHECK(ViewportDeviceCallAt(2)->kind == VIEWPORT_CALL_VERTEX_SHADER);
    CHECK(ViewportDeviceCallAt(2)->value == (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1));
    const ViewportDeviceCall *draw = ViewportDeviceCallAt(3);
    CHECK(draw->kind == VIEWPORT_CALL_DRAW && draw->device == 0 && draw->state == D3DPT_TRIANGLELIST);
    CHECK(draw->vertices == first && draw->count == sprites*2 && draw->stride == 28);
    CHECK(draw->capturedVertexBytes == sprites*168 && memcmp(draw->capturedVertices, bytes, sprites*168) == 0);
    CHECK(manager->spritesToDraw == 0 && manager->vertexBufferStartPtr == manager->vertexBufferEndPtr);
}
struct FlushObservation
{
    AnmManager *manager;
    AnmVertex *first;
    unsigned int sprites;
    int flushes;
    unsigned int draws;
};
static void __cdecl ObserveFlush(const ViewportDeviceCall *call, void *context)
{
    if (call->kind != VIEWPORT_CALL_DRAW) return;
    FlushObservation &o = *static_cast<FlushObservation *>(context);
    CHECK(o.manager->spritesToDraw == o.sprites && o.manager->flushesThisFrame == o.flushes);
    CHECK(o.manager->vertexBufferStartPtr == o.first);
    CHECK(o.manager->vertexBufferEndPtr == o.first+o.sprites*6);
    ++o.draws;
}
static unsigned int CheckQueue(AnmManager *manager)
{
    unsigned int cases = 0;
    for (unsigned int seed = 0; seed < 4; ++seed)
    for (unsigned int failure = 0; failure < 2; ++failure)
    {
        manager->ClearVertexBuffer(); manager->flushesThisFrame = 100;
        ResetViewportDeviceCalls(failure ? E_FAIL : S_OK);
        manager->FlushVertexBuffer();
        CHECK(ViewportDeviceCallCount() == 0 && manager->flushesThisFrame == 100);
        AnmVertex a[4], b[4], c[4]; FillWire(a,seed); FillWire(b,seed+1); FillWire(c,seed+2);
        unsigned char expected[336]; Expand(expected,a); Expand(expected+168,b);
        CHECK(manager->AddSpriteToDrawBuffer(a) == ZUN_SUCCESS);
        CHECK(manager->AddSpriteToDrawBuffer(b) == ZUN_SUCCESS);
        CHECK(manager->spritesToDraw == 2 && manager->vertexBufferEndPtr == manager->vertexBuffer+12);
        CHECK(memcmp(manager->vertexBuffer,expected,sizeof(expected)) == 0 && ViewportDeviceCallCount() == 0);
        FlushObservation observation = {manager,manager->vertexBuffer,2,100,0};
        SetViewportDeviceObserver(ObserveFlush,&observation); manager->FlushVertexBuffer(); SetViewportDeviceObserver(NULL,NULL);
        CHECK(observation.draws == 1 && manager->flushesThisFrame == 101);
        CheckFlushCalls(manager,manager->vertexBuffer,2,expected);
        ResetViewportDeviceCalls(failure ? E_FAIL : S_OK); manager->FlushVertexBuffer();
        CHECK(ViewportDeviceCallCount() == 0 && manager->vertexBufferEndPtr == manager->vertexBuffer+12);
        CHECK(manager->flushesThisFrame == 101);
        unsigned char next[168]; Expand(next,c);
        CHECK(manager->AddSpriteToDrawBuffer(c) == ZUN_SUCCESS);
        CHECK(manager->vertexBufferStartPtr == manager->vertexBuffer+12 && manager->vertexBufferEndPtr == manager->vertexBuffer+18);
        manager->FlushVertexBuffer(); CheckFlushCalls(manager,manager->vertexBuffer+12,1,next);
        CHECK(memcmp(manager->vertexBuffer,expected,sizeof(expected)) == 0 && manager->flushesThisFrame == 102);
        ResetViewportDeviceCalls(S_OK); manager->ClearVertexBuffer();
        CHECK(manager->spritesToDraw == 0 && manager->vertexBufferStartPtr == manager->vertexBuffer && manager->vertexBufferEndPtr == manager->vertexBuffer);
        CHECK(manager->flushesThisFrame == 102 && ViewportDeviceCallCount() == 0);
        ++cases;
    }
    return cases;
}
static unsigned int CheckLastFittingBatch(AnmManager *manager)
{
    unsigned int cases = 0;
    for (unsigned int count = 1; count <= 2; ++count)
    for (unsigned int failure = 0; failure < 2; ++failure)
    {
        manager->ClearVertexBuffer(); manager->flushesThisFrame = 200;
        AnmVertex *first = manager->vertexBuffer+0x20000-count*6;
        manager->vertexBufferStartPtr = manager->vertexBufferEndPtr = first;
        unsigned char expected[336];
        for (unsigned int i = 0; i < count; ++i)
        { AnmVertex q[4]; FillWire(q,i+4); Expand(expected+i*168,q); CHECK(manager->AddSpriteToDrawBuffer(q) == ZUN_SUCCESS); }
        CHECK(manager->vertexBufferEndPtr == manager->vertexBuffer+0x20000);
        ResetViewportDeviceCalls(failure ? E_FAIL : S_OK); manager->FlushVertexBuffer(); CheckFlushCalls(manager,first,count,expected);
        CHECK(manager->vertexBufferStartPtr == manager->vertexBuffer+0x20000 && manager->flushesThisFrame == 201);
        manager->ClearVertexBuffer(); CHECK(manager->vertexBufferEndPtr == manager->vertexBuffer);
        ++cases;
    }
    return cases;
}
static unsigned int Channel(unsigned int word,unsigned int shift) { return (word>>shift)&255; }
static unsigned int Mixed(unsigned int input,unsigned int color)
{
    unsigned int word = 0;
    for (unsigned int shift = 0; shift < 32; shift+=8)
    { unsigned int c = Channel(input,shift)*Channel(color,shift)/128; if (c>255) c=255; word |= c<<shift; }
    return word;
}
static void SeedGeometry(float left,float top,float right,float bottom)
{
    for (unsigned int i = 0; i < 4; ++i)
    {
        g_AnmTexturedVertices[i].position = Float3(i&1?right:left,i&2?bottom:top,0.5f);
        g_AnmTexturedVertices[i].rhw = 1;
        g_AnmTexturedVertices[i].diffuse.color = 0x10203040u ^ (i*0x11223344u);
        g_AnmTexturedVertices[i].uv.x = g_AnmTexturedVertices[i].uv.y = -9;
    }
}
static void SeedVm(AnmVm &vm, AnmLoadedSprite &sprite)
{
    vm.loadedSprite = &sprite; vm.blendMode = 0; vm.useSecondaryColor = false;
    vm.color1.color = 0xff102030; vm.color2.color = 0xffa0b0c0;
    vm.uvScrollPos.x = 0.0625f; vm.uvScrollPos.y = -0.125f;
}
static unsigned int CheckDrawColors(AnmManager *manager,AnmLoadedSprite &sprite)
{
    unsigned int cases = 0; const unsigned int values[] = {0,1,127,128,129,255};
    unsigned int control = _controlfp(0,0); _controlfp(_RC_NEAR,_MCW_RC);
    for (unsigned int color = 0; color < 6; ++color)
    for (unsigned int secondary = 0; secondary < 2; ++secondary)
    for (unsigned int mix = 0; mix < 2; ++mix)
    for (unsigned int preserve = 0; preserve < 2; ++preserve)
    for (unsigned int rounded = 0; rounded < 2; ++rounded)
    {
        AnmVm vm; SeedVm(vm,sprite); vm.useSecondaryColor = secondary != 0;
        vm.color1.color = values[color] | values[(color+1)%6]<<8 | values[(color+2)%6]<<16 | values[(color+3)%6]<<24;
        vm.color2.color = values[(color+5)%6] | values[(color+4)%6]<<8 | values[(color+3)%6]<<16 | values[(color+2)%6]<<24;
        manager->color.color = 0xff80817f; manager->useMixColor = mix;
        manager->ClearVertexBuffer(); manager->currentTexture = sprite.texture;
        manager->currentVertexShader = 1; manager->currentBlendMode = 0;
        manager->screenShakeOffset.x = 0.25f; manager->screenShakeOffset.y = -0.25f;
        SeedGeometry(10.25f,20.75f,40.25f,50.75f); ResetViewportDeviceCalls(S_OK);
        CHECK(manager->DrawInner(&vm,(preserve?2:0)|(rounded?1:0)) == ZUN_SUCCESS);
        CHECK(manager->spritesToDraw == 1 && ViewportDeviceCallCount() == 0);
        WireVertex expected[4];
        for (unsigned int i = 0; i < 4; ++i)
        {
            expected[i].x = (i&1?40.5f:10.5f) - (rounded?1.0f:0.0f);
            expected[i].y = (i&2?50.5f:20.5f) - (rounded?1.0f:0.0f);
            expected[i].z = 0.5f; expected[i].rhw = 1;
            unsigned int word = secondary?vm.color2.color:vm.color1.color;
            expected[i].color = preserve ? (0x10203040u ^ (i*0x11223344u)) : (mix?Mixed(word,manager->color.color):word);
            expected[i].u = i&1?0.8125f:0.1875f; expected[i].v = i&2?0.75f:0.125f;
            CHECK(memcmp(&expected[i],&g_AnmTexturedVertices[i],28) == 0);
        }
        unsigned char packet[168]; for (unsigned int j=0;j<6;++j) memcpy(packet+j*28,&expected[triangleOrder[j]],28);
        manager->FlushVertexBuffer(); CheckFlushCalls(manager,manager->vertexBuffer,1,packet); ++cases;
    }
    _controlfp(control,_MCW_RC); return cases;
}
struct TransitionObservation { AnmManager *manager; unsigned int mode,pending,draws; IDirect3DTexture8 *texture; };
static void __cdecl ObserveTransition(const ViewportDeviceCall *call,void *context)
{
    TransitionObservation &o = *static_cast<TransitionObservation *>(context);
    if (call->kind == VIEWPORT_CALL_DRAW)
    {
        CHECK(o.pending == 1 && o.manager->spritesToDraw == 1);
        if (o.mode==0) CHECK(o.manager->currentTexture == o.texture);
        if (o.mode==1) CHECK(o.manager->currentBlendMode == 0);
        ++o.draws;
    }
    if (call->kind == VIEWPORT_CALL_TEXTURE || call->kind == VIEWPORT_CALL_RENDER_STATE)
        CHECK(o.manager->spritesToDraw == 0 && o.manager->vertexBufferStartPtr == o.manager->vertexBufferEndPtr);
}
static unsigned int CheckTransitions(AnmManager *manager,AnmLoadedSprite &sprite)
{
    unsigned int cases=0;
    for (unsigned int mode=0;mode<3;++mode)
    for (unsigned int pending=0;pending<(mode==2?1u:2u);++pending)
    for (unsigned int failure=0;failure<2;++failure)
    {
        AnmVm vm; SeedVm(vm,sprite); if (mode==1) vm.blendMode=1;
        manager->ClearVertexBuffer();manager->flushesThisFrame=300;
        manager->currentTexture = mode==0?ViewportTestTexture(1):sprite.texture;
        manager->currentBlendMode=0;manager->currentVertexShader=mode==2?2:1;manager->useMixColor=0;
        manager->screenShakeOffset.x=manager->screenShakeOffset.y=0;
        AnmVertex old[4];FillWire(old,5);unsigned char oldPacket[168];Expand(oldPacket,old);
        if (pending) CHECK(manager->AddSpriteToDrawBuffer(old)==ZUN_SUCCESS);
        SeedGeometry(10,20,40,50);ResetViewportDeviceCalls(failure?E_FAIL:S_OK);
        TransitionObservation observation={manager,mode,pending,0,sprite.texture};
        SetViewportDeviceObserver(ObserveTransition,&observation);
        CHECK(manager->DrawInner(&vm,0)==ZUN_SUCCESS);SetViewportDeviceObserver(NULL,NULL);
        CHECK(observation.draws==pending && ViewportDeviceCallCount()==pending*4+(mode==2?0:1));
        if (pending) CHECK(memcmp(ViewportDeviceCallAt(3)->capturedVertices,oldPacket,168)==0);
        if (mode==0) CHECK(ViewportDeviceCallAt(pending*4)->kind==VIEWPORT_CALL_TEXTURE && ViewportDeviceCallAt(pending*4)->texture==static_cast<IDirect3DBaseTexture8 *>(sprite.texture));
        if (mode==1) CHECK(ViewportDeviceCallAt(pending*4)->kind==VIEWPORT_CALL_RENDER_STATE && ViewportDeviceCallAt(pending*4)->state==D3DRS_DESTBLEND && ViewportDeviceCallAt(pending*4)->value==D3DBLEND_ONE);
        CHECK(manager->spritesToDraw==1 && manager->currentVertexShader==1 && manager->flushesThisFrame==300+pending);
        CHECK(manager->vertexBufferStartPtr==manager->vertexBuffer+pending*6 && manager->vertexBufferEndPtr==manager->vertexBuffer+(pending+1)*6);
        unsigned char expected[168];Expand(expected,g_AnmTexturedVertices);
        ResetViewportDeviceCalls(failure?E_FAIL:S_OK);manager->FlushVertexBuffer();CheckFlushCalls(manager,manager->vertexBuffer+pending*6,1,expected);
        CHECK(manager->flushesThisFrame==301+pending);++cases;
    }
    return cases;
}
static unsigned int CheckCull(AnmManager *manager,AnmLoadedSprite &sprite)
{
    const float rectangles[4][4]={{-100,10,-50,20},{650,10,700,20},{10,-80,20,-20},{10,490,20,520}};
    for (unsigned int i=0;i<4;++i)
    {
        manager->ClearVertexBuffer();manager->flushesThisFrame=400;
        manager->currentTexture=ViewportTestTexture(1);manager->currentVertexShader=2;manager->currentBlendMode=0;
        manager->screenShakeOffset.x=manager->screenShakeOffset.y=0;
        AnmVertex q[4];FillWire(q,i);unsigned char bytes[168];Expand(bytes,q);CHECK(manager->AddSpriteToDrawBuffer(q)==ZUN_SUCCESS);
        AnmVm vm;SeedVm(vm,sprite);vm.blendMode=1;
        SeedGeometry(rectangles[i][0],rectangles[i][1],rectangles[i][2],rectangles[i][3]);ResetViewportDeviceCalls(S_OK);
        CHECK(manager->DrawInner(&vm,0)==ZUN_SUCCESS && ViewportDeviceCallCount()==0);
        CHECK(manager->spritesToDraw==1 && manager->vertexBufferEndPtr==manager->vertexBuffer+6 && manager->vertexBufferStartPtr==manager->vertexBuffer);
        CHECK(manager->flushesThisFrame==400 && manager->currentTexture==ViewportTestTexture(1) && manager->currentVertexShader==2 && manager->currentBlendMode==0);
        CHECK(memcmp(manager->vertexBuffer,bytes,sizeof(bytes))==0);
        for (unsigned int j=0;j<4;++j)
        {
            CHECK(g_AnmTexturedVertices[j].diffuse.color==(0x10203040u ^ (j*0x11223344u)));
            CHECK(g_AnmTexturedVertices[j].uv.x==(j&1?0.8125f:0.1875f));
            CHECK(g_AnmTexturedVertices[j].uv.y==(j&2?0.75f:0.125f));
        }
    }
    return 4;
}
int __cdecl main()
{
    g_Supervisor.InitializeCriticalSections();g_Supervisor.InitializeViewports();g_Supervisor.d3dDevice=ViewportTestDevice(0);
    AnmManager *manager=new AnmManager;g_AnmManager=manager;
    SupervisorViewportConfiguration &configuration=g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD];
    g_Supervisor.currentViewportConfiguration=&configuration;
    configuration.viewport.X=configuration.viewport.Y=0;configuration.viewport.Width=640;configuration.viewport.Height=480;
    AnmLoadedSprite sprite;memset(&sprite,0,sizeof(sprite));sprite.texture=ViewportTestTexture(0);
    sprite.uvStart.x=0.125f;sprite.uvStart.y=0.25f;sprite.uvEnd.x=0.75f;sprite.uvEnd.y=0.875f;
    unsigned int queue=CheckQueue(manager),boundary=CheckLastFittingBatch(manager),colors=CheckDrawColors(manager,sprite),transitions=CheckTransitions(manager,sprite),cull=CheckCull(manager,sprite);
    CHECK(queue==8 && boundary==4 && colors==96 && transitions==10 && cull==4);
    printf("ANM batch: %u queue, %u last-fit, %u color/round/UV, %u transition and %u cull cases passed\n",queue,boundary,colors,transitions,cull);
    return 0;
}
