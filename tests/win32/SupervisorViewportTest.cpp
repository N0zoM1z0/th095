// Actual normal Supervisor/ANM owners and D3DX math; the D3D API boundary uses
// typed real-SDK C COM bindings. No game entry point or graphics device runs.
#include "SupervisorRuntime.hpp"
#include "AnmManager.hpp"
#include "Background.hpp"
#include "ViewportDeviceFixture.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(value) do { if (!(value)) { fprintf(stderr, "viewport check line %d: %s\n", __LINE__, #value); abort(); } } while (0)

static bool Near(float actual, float expected)
{
    return fabs(actual - expected) <= 0.00002f * (1.0f + fabs(expected));
}

static void CheckProjection(const th095::SupervisorViewportConfiguration &configuration)
{
    // Analytic 2D basis: right +X, up -Y, forward -Z; near/far 1/10000.
    const D3DVIEWPORT8 &viewport = configuration.viewport;
    const D3DXMATRIX &view = configuration.viewMatrix;
    const D3DXMATRIX &projection = configuration.projectionMatrix;
    const float cotangent = 1.0f / (float)tan((D3DX_PI / 10.0f) / 2.0f);
    CHECK(Near(view._11, 1.0f) && Near(view._22, -1.0f));
    CHECK(Near(view._33, -1.0f) && Near(view._44, 1.0f));
    CHECK(Near(view._41, -(float)viewport.Width / 2.0f));
    CHECK(Near(view._42, (float)viewport.Height / 2.0f));
    CHECK(Near(view._43, (float)viewport.Height / 2.0f * cotangent));
    CHECK(Near(projection._11, cotangent / ((float)viewport.Width / viewport.Height)));
    CHECK(Near(projection._22, cotangent));
    CHECK(Near(projection._33, 10000.0f / 9999.0f));
    CHECK(projection._34 == 1.0f && projection._44 == 0.0f);
    CHECK(Near(projection._43, -10000.0f / 9999.0f));
}

static void PrepareBackground(th095::SupervisorViewportConfiguration &configuration, unsigned int scenario)
{
    using namespace th095;
    const float cameras[4][13] = {
        {10,20,30, 0,0,1, 0,1,0, 2,3,-4, 0.5f},
        {-80,140,1000, 4,-2,-7, 0,1,0, 1,0,3, 0.7f},
        {-3,8,15, 2,5,1, -1,2,0.5f, -4,7,3, 0.9f},
        {0,0,-600, 0,0,1, 0,1,0, 0,0,0, 0.6f}
    };
    const float *c = cameras[scenario];
    configuration.cameraPosition = Float3(c[0], c[1], c[2]);
    configuration.cameraLookAtOffset = Float3(c[3], c[4], c[5]);
    configuration.cameraUp = Float3(c[6], c[7], c[8]);
    configuration.cameraPositionOffset = Float3(c[9], c[10], c[11]);
    configuration.fieldOfView = c[12];
}

static void CheckBackgroundProjection(const th095::SupervisorViewportConfiguration &configuration)
{
    using namespace th095;
    const D3DXMATRIX &v = configuration.viewMatrix;
    const D3DXMATRIX &p = configuration.projectionMatrix;
    Float3 eye = configuration.cameraPosition + configuration.cameraPositionOffset;
    Float3 look = configuration.cameraPosition + configuration.cameraLookAtOffset;
    const float scale = 1.0f + fabs(eye.x) + fabs(eye.y) + fabs(eye.z);
    CHECK(fabs(eye.x*v._11 + eye.y*v._21 + eye.z*v._31 + v._41) < 0.0002f*scale);
    CHECK(fabs(eye.x*v._12 + eye.y*v._22 + eye.z*v._32 + v._42) < 0.0002f*scale);
    CHECK(fabs(eye.x*v._13 + eye.y*v._23 + eye.z*v._33 + v._43) < 0.0002f*scale);
    CHECK(fabs(look.x*v._11 + look.y*v._21 + look.z*v._31 + v._41) < 0.0002f*scale);
    CHECK(fabs(look.x*v._12 + look.y*v._22 + look.z*v._32 + v._42) < 0.0002f*scale);
    CHECK(look.x*v._13 + look.y*v._23 + look.z*v._33 + v._43 > 0.0f);
    CHECK(Near(v._11*v._11 + v._21*v._21 + v._31*v._31, 1.0f));
    CHECK(Near(v._12*v._12 + v._22*v._22 + v._32*v._32, 1.0f));
    CHECK(Near(v._13*v._13 + v._23*v._23 + v._33*v._33, 1.0f));
    const float cotangent = 1.0f / (float)tan(configuration.fieldOfView / 2.0f);
    CHECK(Near(p._11, cotangent / ((float)configuration.viewport.Width / configuration.viewport.Height)));
    CHECK(Near(p._22, cotangent));
    CHECK(Near(p._33, 1800.0f / 1770.0f) && Near(p._43, -30.0f * 1800.0f / 1770.0f));
    CHECK(p._34 == 1.0f && p._44 == 0.0f);
    const Float3 &right = configuration.cameraRight;
    const Float3 &direction = configuration.cameraLookAtOffset;
    const Float3 &up = configuration.cameraUp;
    CHECK(Near(right.x*right.x + right.y*right.y + right.z*right.z, 1.0f));
    CHECK(Near(right.x*direction.x + right.y*direction.y + right.z*direction.z, 0.0f));
    CHECK(Near(right.x*up.x + right.y*up.y + right.z*up.z, 0.0f));
    CHECK(right.x*(direction.y*up.z-direction.z*up.y)
        + right.y*(direction.z*up.x-direction.x*up.z)
        + right.z*(direction.x*up.y-direction.y*up.x) > 0.0f);
}

// Hand-authored little-endian stage records: time i32, opcode i16, size u16,
// then raw 32-bit operands. This is serialized input, not another C++ layout.
static unsigned int AppendStageRecord(u8 *bytes, unsigned int at, u16 opcode, const u32 *args, unsigned int count)
{
    const unsigned int size = 8 + count*4;
    memset(bytes + at, 0, size);
    bytes[at+4] = (u8)opcode; bytes[at+5] = (u8)(opcode >> 8);
    bytes[at+6] = (u8)size; bytes[at+7] = (u8)(size >> 8);
    for (unsigned int i = 0; i < count; ++i)
        for (unsigned int b = 0; b < 4; ++b)
            bytes[at+8+i*4+b] = (u8)(args[i] >> (b*8));
    return at + size;
}

static void CheckStageCameraPublication(th095::AnmManager *manager)
{
    using namespace th095;
    for (u32 mode = 0; mode < 5; ++mode)
    {
        g_AnmManager = manager;
        Background background;
        u32 storage[32]; u8 *bytes = reinterpret_cast<u8 *>(storage);
        const float values[10] = {11,22,33, 3,-4,7, 0,2,1, 0.75f};
        u32 bits[10]; memcpy(bits, values, sizeof(bits));
        unsigned int at = 0;
        at = AppendStageRecord(bytes, at, 2, bits, 3);
        at = AppendStageRecord(bytes, at, 4, bits+3, 3);
        at = AppendStageRecord(bytes, at, 6, bits+6, 3);
        at = AppendStageRecord(bytes, at, 7, bits+9, 1);
        at = AppendStageRecord(bytes, at, 12, &mode, 1);
        const unsigned int halt = at;
        AppendStageRecord(bytes, at, 0, NULL, 0);
        background.stageScript = bytes;
        background.stageInstruction = reinterpret_cast<BackgroundStageInstruction *>(bytes);
        SupervisorViewportConfiguration &camera = g_Supervisor.viewportConfigurations[0];
        camera.cameraPositionOffset = Float3(9,42,77);
        unsigned char fullWindow[sizeof(SupervisorViewportConfiguration)];
        memcpy(fullWindow, &g_Supervisor.viewportConfigurations[1], sizeof(fullWindow));
        g_Supervisor.currentViewportConfiguration = &g_Supervisor.viewportConfigurations[1];
        g_Supervisor.currentViewportIndex = 1;
        CHECK(background.RunStageScript() == 0);
        CHECK(background.stageInstruction == reinterpret_cast<BackgroundStageInstruction *>(bytes+halt));
        CHECK(camera.cameraPosition.x == 11 && camera.cameraPosition.y == 22 && camera.cameraPosition.z == 33);
        CHECK(camera.cameraLookAtOffset.x == 3 && camera.cameraLookAtOffset.y == -4 && camera.cameraLookAtOffset.z == 7);
        CHECK(camera.fieldOfView == 0.75f);
        const int quarterPeriods[] = {0,120,120,1200,512};
        background.interpolationCurrentTimers[3] = quarterPeriods[mode];
        CHECK(background.RunStageScript() == 0);
        CHECK(camera.cameraPositionOffset.y == 42);
        if (mode == 0) CHECK(camera.cameraPositionOffset.x == 9 && camera.cameraPositionOffset.z == 77);
        if (mode == 1) CHECK(Near(camera.cameraPositionOffset.x, -40) && camera.cameraPositionOffset.z == 77);
        if (mode == 2) CHECK(Near(camera.cameraPositionOffset.x, -70) && Near(camera.cameraUp.x, 0.1f));
        if (mode == 3) CHECK(Near(camera.cameraUp.x, -1) && Near(camera.cameraUp.z, 0));
        if (mode == 4) CHECK(Near(camera.cameraPositionOffset.x, -30) && Near(camera.cameraPositionOffset.z, 0) && Near(camera.cameraUp.x, 0.1f));
        CHECK(memcmp(fullWindow, &g_Supervisor.viewportConfigurations[1], sizeof(fullWindow)) == 0);
        CHECK(g_Supervisor.currentViewportConfiguration == &g_Supervisor.viewportConfigurations[1]);
        CHECK(g_Supervisor.currentViewportIndex == 1);
        g_AnmManager = NULL;
        ResetViewportDeviceCalls(S_OK);
        g_Supervisor.ConfigureBackgroundViewport(0);
        CHECK(g_Supervisor.currentViewportConfiguration == &camera && g_Supervisor.currentViewportIndex == 0);
        CheckBackgroundProjection(camera);
        CHECK(ViewportDeviceCallCount() == 3);
        CHECK(memcmp(&ViewportDeviceCallAt(0)->matrix, &camera.viewMatrix, sizeof(D3DMATRIX)) == 0);
        background.stageScript = NULL; background.stageInstruction = NULL;
        g_AnmManager = manager; // The actual destructor releases its empty ANM slot.
    }
    g_Supervisor.currentViewportIndex = 91;
}

static void CheckGlobalInitializer()
{
    using namespace th095;
    Supervisor receiver;
    unsigned char before[sizeof(receiver)];
    memcpy(before, &receiver, sizeof(receiver));
    g_Supervisor.viewportConfigurations[0].screenShakeOffset.x = 19.0f;
    g_Supervisor.viewportConfigurations[1].screenShakeOffset.y = -23.0f;
    g_Supervisor.currentViewportIndex = 91;
    g_Supervisor.currentViewportConfiguration = &g_Supervisor.viewportConfigurations[0];
    receiver.InitializeViewports();
    CHECK(memcmp(before, &receiver, sizeof(receiver)) == 0);
    const SupervisorViewportConfiguration &window =
        g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW];
    const SupervisorViewportConfiguration &playfield =
        g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD];
    CHECK(window.viewport.X == 0 && window.viewport.Y == 0);
    CHECK(window.viewport.Width == 640 && window.viewport.Height == 480);
    CHECK(playfield.viewport.X == 128 && playfield.viewport.Y == 16);
    CHECK(playfield.viewport.Width == 384 && playfield.viewport.Height == 448);
    CHECK(window.viewport.MinZ == 0.0f && playfield.viewport.MaxZ == 1.0f);
    CHECK(window.cameraPosition.z == 1000.0f && playfield.cameraPosition.z == 1000.0f);
    CHECK(window.cameraUp.y == 1.0f && playfield.cameraUp.y == 1.0f);
    CHECK(window.unknown0e4 == 1 && playfield.unknown0e4 == 0);
    CHECK(playfield.screenShakeOffset.x == 19.0f && window.screenShakeOffset.y == -23.0f);
    CHECK(g_Supervisor.currentViewportIndex == 91);
}

int __cdecl main()
{
    using namespace th095;
    CHECK(g_AnmManager == NULL);
    CheckGlobalInitializer();
    g_Supervisor.InitializeCriticalSections();
    AnmManager *manager = new AnmManager;
    IDirect3DDevice8 *globalDevice = ViewportTestDevice(0);
    IDirect3DDevice8 *receiverDevice = ViewportTestDevice(1);
    g_Supervisor.d3dDevice = globalDevice;
    const DWORD dimensions[][2] = {{640, 480}, {384, 448}, {641, 479}, {0xfffffff0u, 4000000000u}};
    const u32 shake[][2] = {{0x80000000u, 0x3f800000u}, {0x7fc12345u, 0xff800000u}, {0x3f000001u, 0xbf000001u}};
    unsigned int cases = 0;
    for (int backgroundMode = 0; backgroundMode < 2; ++backgroundMode)
    for (unsigned int geometry = 0; geometry < 4; ++geometry)
    for (int slot = 0; slot < SUPERVISOR_VIEWPORT_SLOT_COUNT; ++slot)
    for (int failure = 0; failure < 2; ++failure)
    for (int anmState = 0; anmState < 3; ++anmState)
    {
        Supervisor receiver;
        receiver.d3dDevice = receiverDevice;
        receiver.currentViewportIndex = -7;
        receiver.viewportConfigurations[slot] = g_Supervisor.viewportConfigurations[slot];
        SupervisorViewportConfiguration &configuration = receiver.viewportConfigurations[slot];
        configuration.viewport.Width = dimensions[geometry][0];
        configuration.viewport.Height = dimensions[geometry][1];
        if (backgroundMode) PrepareBackground(configuration, geometry);
        memcpy(&configuration.screenShakeOffset, shake[anmState], sizeof(configuration.screenShakeOffset));
        g_AnmManager = anmState ? manager : NULL;
        manager->spritesToDraw = anmState == 2 ? 1 : 0;
        manager->vertexBufferStartPtr = manager->vertexBuffer;
        manager->vertexBufferEndPtr = manager->vertexBuffer + 6;
        const i32 previousFlushes = manager->flushesThisFrame;
        const i32 previousCapture = manager->captureSurfaceIdx;
        ResetViewportDeviceCalls(failure ? E_FAIL : S_OK);
        if (backgroundMode) receiver.ConfigureBackgroundViewport(slot);
        else receiver.ConfigureGameplayViewport(slot);
        CHECK(receiver.currentViewportConfiguration == &configuration);
        CHECK(receiver.currentViewportIndex == slot);
        CHECK(g_Supervisor.currentViewportIndex == 91);
        CHECK(g_Supervisor.currentViewportConfiguration == &g_Supervisor.viewportConfigurations[0]);
        if (backgroundMode) CheckBackgroundProjection(configuration);
        else CheckProjection(configuration);
        UINT offset = 0;
        if (anmState == 2)
        {
            CHECK(ViewportDeviceCallAt(0)->kind == VIEWPORT_CALL_TEXTURE_STATE);
            CHECK(ViewportDeviceCallAt(1)->kind == VIEWPORT_CALL_TEXTURE_STATE);
            CHECK(ViewportDeviceCallAt(2)->kind == VIEWPORT_CALL_VERTEX_SHADER);
            const ViewportDeviceCall *draw = ViewportDeviceCallAt(3);
            CHECK(draw->kind == VIEWPORT_CALL_DRAW && draw->device == 0);
            CHECK(draw->state == D3DPT_TRIANGLELIST && draw->count == 2);
            CHECK(draw->vertices == manager->vertexBuffer && draw->stride == sizeof(VertexTex1DiffuseXyzrhw));
            CHECK(manager->spritesToDraw == 0 && manager->flushesThisFrame == previousFlushes + 1);
            CHECK(manager->vertexBufferStartPtr == manager->vertexBufferEndPtr);
            offset = 4;
        }
        else
            CHECK(manager->flushesThisFrame == previousFlushes);
        CHECK(ViewportDeviceCallCount() == offset + 3);
        const ViewportDeviceCall *view = ViewportDeviceCallAt(offset);
        const ViewportDeviceCall *projection = ViewportDeviceCallAt(offset + 1);
        const ViewportDeviceCall *viewport = ViewportDeviceCallAt(offset + 2);
        CHECK(view->device == 0 && view->kind == VIEWPORT_CALL_TRANSFORM && view->state == D3DTS_VIEW);
        CHECK(projection->device == 0 && projection->kind == VIEWPORT_CALL_TRANSFORM && projection->state == D3DTS_PROJECTION);
        CHECK(memcmp(&view->matrix, &configuration.viewMatrix, sizeof(D3DMATRIX)) == 0);
        CHECK(memcmp(&projection->matrix, &configuration.projectionMatrix, sizeof(D3DMATRIX)) == 0);
        CHECK(viewport->device == 1 && viewport->kind == VIEWPORT_CALL_VIEWPORT);
        CHECK(memcmp(&viewport->viewport, &configuration.viewport, sizeof(D3DVIEWPORT8)) == 0);
        if (anmState)
            CHECK(memcmp(&manager->screenShakeOffset, &configuration.screenShakeOffset, sizeof(Float2)) == 0);
        CHECK(manager->captureSurfaceIdx == previousCapture);
        ++cases;
    }
    CheckStageCameraPublication(manager);
    g_AnmManager = NULL;
    g_Supervisor.d3dDevice = NULL;
    delete manager;
    // The real global Chain owns its registrations and releases them at exit.
    printf("canonical Supervisor viewport: %u real-owner/D3DX/SDK-binding cases and five stage-camera handoffs passed\n", cases);
    return 0;
}
