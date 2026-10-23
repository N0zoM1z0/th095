// Actual normal Supervisor/ANM owners and D3DX math; the D3D API boundary uses
// typed real-SDK C COM bindings. No game entry point or graphics device runs.
#include "SupervisorRuntime.hpp"
#include "AnmManager.hpp"
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
        memcpy(&configuration.screenShakeOffset, shake[anmState], sizeof(configuration.screenShakeOffset));
        g_AnmManager = anmState ? manager : NULL;
        manager->spritesToDraw = anmState == 2 ? 1 : 0;
        manager->vertexBufferStartPtr = manager->vertexBuffer;
        manager->vertexBufferEndPtr = manager->vertexBuffer + 6;
        const i32 previousFlushes = manager->flushesThisFrame;
        const i32 previousCapture = manager->captureSurfaceIdx;
        ResetViewportDeviceCalls(failure ? E_FAIL : S_OK);
        receiver.ConfigureGameplayViewport(slot);
        CHECK(receiver.currentViewportConfiguration == &configuration);
        CHECK(receiver.currentViewportIndex == slot);
        CHECK(g_Supervisor.currentViewportIndex == 91);
        CHECK(g_Supervisor.currentViewportConfiguration == &g_Supervisor.viewportConfigurations[0]);
        CheckProjection(configuration);
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
    g_AnmManager = NULL;
    g_Supervisor.d3dDevice = NULL;
    delete manager;
    // The real global Chain owns its registrations and releases them at exit.
    printf("canonical Supervisor viewport: %u real-owner/D3DX/SDK-binding cases passed\n", cases);
    return 0;
}
