// Actual normal Background/ANM state publication and downstream quad colors.
#include "AnmManager.hpp"
#include "Background.hpp"
#include "ViewportDeviceFixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(v) do { if (!(v)) { fprintf(stderr, "Background ANM line %d: %s\n", __LINE__, #v); fflush(stderr); abort(); } } while (0)
using namespace th095;
static const unsigned int colors[] = {0,0x80808080,0xffffffff,0xff404040,0xff80817f,0x01020304};

static void SeedPending(AnmManager *manager, unsigned int pending)
{
    manager->ClearVertexBuffer(); manager->flushesThisFrame = 30;
    if (pending)
    {
        AnmVertex vertices[4]; memset(vertices,0,sizeof(vertices));
        for (unsigned int i=0;i<4;++i) vertices[i].rhw=1;
        CHECK(manager->AddSpriteToDrawBuffer(vertices)==ZUN_SUCCESS);
    }
}

static unsigned int CheckMixWriters(AnmManager *receiver, AnmManager *other)
{
    g_AnmManager=other;
    unsigned int cases=0;
    for (unsigned int color=0;color<6;++color)
    for (unsigned int pending=0;pending<2;++pending)
    {
        SeedPending(receiver,pending);
        unsigned int expected[4]; memcpy(expected,receiver,sizeof(expected));
        unsigned char otherPrefix[16]; memcpy(otherPrefix,other,sizeof(otherPrefix));
        AnmVertex *end=receiver->vertexBufferEndPtr;
        ResetViewportDeviceCalls(E_FAIL);
        receiver->SetMixColor(colors[color]);
        expected[0]=colors[color];expected[1]=1;
        CHECK(memcmp(receiver,expected,sizeof(expected))==0);
        CHECK(memcmp(other,otherPrefix,sizeof(otherPrefix))==0);
        CHECK(receiver->spritesToDraw==pending && receiver->vertexBufferEndPtr==end);
        CHECK(receiver->vertexBufferStartPtr==receiver->vertexBuffer && receiver->flushesThisFrame==30);
        CHECK(ViewportDeviceCallCount()==0);
        receiver->SetMixColorDefault();expected[0]=0x80808080;expected[1]=0;
        CHECK(memcmp(receiver,expected,sizeof(expected))==0);
        CHECK(memcmp(other,otherPrefix,sizeof(otherPrefix))==0);
        CHECK(receiver->spritesToDraw==pending && receiver->vertexBufferEndPtr==end && receiver->flushesThisFrame==30);
        CHECK(ViewportDeviceCallCount()==0);++cases;
    }
    return cases;
}

static unsigned int Mix(unsigned int source,unsigned int color)
{
    unsigned int result=0;
    for (unsigned int shift=0;shift<32;shift+=8)
    {
        unsigned int channel=((source>>shift)&255)*((color>>shift)&255)/128;
        if (channel>255) channel=255;
        result|=channel<<shift;
    }
    return result;
}

static unsigned int CheckMixConsumers(AnmManager *manager)
{
    g_AnmManager=manager;
    SupervisorViewportConfiguration &configuration=g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD];
    configuration.viewport.X=configuration.viewport.Y=0;configuration.viewport.Width=640;configuration.viewport.Height=480;
    g_Supervisor.currentViewportConfiguration=&configuration;
    AnmLoadedSprite sprite;memset(&sprite,0,sizeof(sprite));sprite.texture=ViewportTestTexture(0);
    sprite.uvEnd.x=sprite.uvEnd.y=1;
    unsigned int cases=0;
    for (unsigned int color=0;color<6;++color)
    for (unsigned int useDefault=0;useDefault<2;++useDefault)
    for (unsigned int secondary=0;secondary<2;++secondary)
    {
        AnmVm vm;vm.loadedSprite=&sprite;vm.blendMode=ANM_BLEND_ALPHA;
        vm.color1.color=0xff2050c0;vm.color2.color=0x7f90a010;vm.useSecondaryColor=secondary!=0;
        manager->ClearVertexBuffer();manager->currentTexture=sprite.texture;
        manager->cachedBlendMode=ANM_BLEND_ALPHA;manager->cachedDrawPath=ANM_DRAW_PATH_BUFFERED_QUADS;
        manager->screenShakeOffset.x=manager->screenShakeOffset.y=0;
        for (unsigned int i=0;i<4;++i)
        { g_AnmTexturedVertices[i].position=Float3(i&1?40.0f:10.0f,i&2?50.0f:20.0f,0.5f);g_AnmTexturedVertices[i].rhw=1; }
        ResetViewportDeviceCalls(S_OK);manager->SetMixColor(colors[color]);
        if (useDefault) manager->SetMixColorDefault();
        CHECK(manager->DrawInner(&vm,0)==ZUN_SUCCESS && ViewportDeviceCallCount()==0);
        unsigned int expected=secondary?0x7f90a010:0xff2050c0;
        if (!useDefault) expected=Mix(expected,colors[color]);
        CHECK(manager->spritesToDraw==1 && manager->vertexBufferEndPtr==manager->vertexBuffer+6);
        for (unsigned int j=0;j<4;++j) CHECK(g_AnmTexturedVertices[j].diffuse.color==expected);
        for (unsigned int k=0;k<6;++k)
        {
            unsigned int word;
            memcpy(&word,reinterpret_cast<unsigned char *>(manager->vertexBuffer)+k*28+16,4);
            CHECK(word==expected);
        }
        ++cases;
    }
    return cases;
}

struct MarkerObservation { AnmManager *manager; unsigned int incoming; };
static void __cdecl ObserveMarker(const ViewportDeviceCall *call,void *context)
{
    MarkerObservation &o=*static_cast<MarkerObservation *>(context);
    CHECK(o.manager->cameraMode==o.incoming);
}

static unsigned int CheckEmptyStageMarker(AnmManager *manager)
{
    g_AnmManager=manager;
    Background *background=new Background;
    // The instance list is opaque loaded data in the public API. Supply its
    // aligned 16-byte terminating record without duplicating the private type.
    i32 sentinel[4]={-1,0,0,0};
    background->stageObjectInstances=reinterpret_cast<BackgroundStageObjectInstance *>(sentinel);
    SupervisorViewportConfiguration &configuration=g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD];
    configuration.cameraPosition=Float3(0,0,-600);configuration.cameraLookAtOffset=Float3(0,0,1);
    configuration.cameraUp=Float3(0,1,0);configuration.cameraPositionOffset=Float3(0,0,0);configuration.fieldOfView=0.7f;
    const unsigned char incoming[]={0,1,0x7e,0xff};
    unsigned int cases=0;
    for (unsigned int state=0;state<4;++state)
    for (unsigned int pending=0;pending<2;++pending)
    for (unsigned int failure=0;failure<2;++failure)
    {
        SeedPending(manager,pending);manager->cameraMode=incoming[state];
        unsigned char prefix[16],neighbors[8];memcpy(prefix,manager,16);
        memcpy(neighbors,reinterpret_cast<unsigned char *>(manager)+0x1764,8);neighbors[4]=1;
        ResetViewportDeviceCalls(failure?E_FAIL:S_OK);
        MarkerObservation observation={manager,incoming[state]};SetViewportDeviceObserver(ObserveMarker,&observation);
        CHECK(background->RenderObjects(state)==0);SetViewportDeviceObserver(NULL,NULL);
        CHECK(manager->cameraMode==1 && memcmp(prefix,manager,16)==0);
        CHECK(memcmp(neighbors,reinterpret_cast<unsigned char *>(manager)+0x1764,8)==0);
        CHECK(manager->spritesToDraw==0 && manager->flushesThisFrame==30+(int)pending);
        unsigned int view=0,projection=0,draws=0;
        for (unsigned int i=0;i<ViewportDeviceCallCount();++i)
        {
            const ViewportDeviceCall *call=ViewportDeviceCallAt(i);
            if (call->kind==VIEWPORT_CALL_TRANSFORM && call->state==D3DTS_VIEW) ++view;
            if (call->kind==VIEWPORT_CALL_TRANSFORM && call->state==D3DTS_PROJECTION) ++projection;
            if (call->kind==VIEWPORT_CALL_DRAW) ++draws;
        }
        CHECK(view==1 && projection==1 && draws==pending);++cases;
    }
    return cases;
}

int __cdecl main()
{
    g_Supervisor.InitializeCriticalSections();g_Supervisor.InitializeViewports();g_Supervisor.d3dDevice=ViewportTestDevice(0);
    AnmManager *manager=new AnmManager,*other=new AnmManager;
    const unsigned int writers=CheckMixWriters(manager,other),consumers=CheckMixConsumers(manager),marker=CheckEmptyStageMarker(manager);
    CHECK(writers==12 && consumers==24 && marker==16);
    printf("Background ANM: %u receiver/queue writer pairs, %u actual color consumers and %u empty-stage legacy-byte cases passed\n",writers,consumers,marker);
    return 0;
}
