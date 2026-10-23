// Actual normal DrawSquare/Flush/DrawInner with SDK C COM boundary capture.
#include "AnmManager.hpp"
#include "ScreenEffect.hpp"
#include "ViewportDeviceFixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(value) do { if (!(value)) { fprintf(stderr, "quad check line %d: %s\n", __LINE__, #value); fflush(stderr); abort(); } } while (0)
using namespace th095;

// Independent decoding of the external FVF_XYZRHW | FVF_DIFFUSE packet.
struct CapturedQuadVertex { float x, y, z, rhw; u32 diffuse; };
typedef char CapturedQuadVertexIs14[sizeof(CapturedQuadVertex) == 20 ? 1 : -1];
struct Observation
{
    AnmManager *manager;
    AnmLoadedSprite *sprite;
    IDirect3DTexture8 *texture;
    bool quadSeen;
    bool restoredAfterInvalidation;
    unsigned int pending;
};

static bool SameFloat(float a, float b) { return memcmp(&a, &b, sizeof(a)) == 0; }
static void CheckInvalidated(AnmManager *manager)
{
    CHECK(manager->currentVertexShader == 0xff && manager->currentSprite == NULL);
    CHECK(manager->currentTexture == NULL && manager->currentColorOp == 0xff);
    CHECK(manager->currentBlendMode == 3 && manager->disableZWrite == 0xff);
    CHECK(manager->currentTextureFactor == 0x11223344u);
}
static void CheckInitialCache(const Observation &o)
{
    CHECK(o.manager->currentVertexShader == 1 && o.manager->currentSprite == o.sprite);
    CHECK(o.manager->currentTexture == o.texture && o.manager->currentColorOp == 1);
    CHECK(o.manager->currentBlendMode == 1 && o.manager->disableZWrite == 0);
    CHECK(o.manager->currentTextureFactor == 0x11223344u);
}
static void __cdecl ObserveDevice(const ViewportDeviceCall *call, void *context)
{
    Observation &o = *static_cast<Observation *>(context);
    if (call->kind == VIEWPORT_CALL_DRAW && call->state == D3DPT_TRIANGLESTRIP)
    {
        CheckInitialCache(o);
        CHECK(o.manager->spritesToDraw == 0);
        CHECK(o.manager->flushesThisFrame == 42 + o.pending);
        o.quadSeen = true;
    }
    else if (o.quadSeen && call->kind == VIEWPORT_CALL_TEXTURE_STATE)
    {
        CheckInvalidated(o.manager);
        o.restoredAfterInvalidation = true;
    }
}
static void CheckTextureState(UINT at, D3DTEXTURESTAGESTATETYPE state, DWORD value)
{
    const ViewportDeviceCall *call = ViewportDeviceCallAt(at);
    CHECK(call->kind == VIEWPORT_CALL_TEXTURE_STATE && call->device == 0 && call->count == 0);
    CHECK(call->state == (DWORD)state && call->value == value);
}
static void CheckAnmAfterQuad(AnmManager *manager, AnmLoadedSprite &sprite)
{
    AnmVm vm;
    vm.loadedSprite = &sprite;
    vm.color1.color = 0xff876543;
    vm.blendMode = 1;
    manager->useMixColor = 0;
    const D3DVIEWPORT8 &viewport = g_Supervisor.currentViewportConfiguration->viewport;
    for (unsigned int i = 0; i < 4; ++i)
    {
        g_AnmTexturedVertices[i].position.x = (float)viewport.X + 64.0f + (i & 1 ? 20.0f : 0.0f);
        g_AnmTexturedVertices[i].position.y = (float)viewport.Y + 64.0f + (i & 2 ? 20.0f : 0.0f);
        g_AnmTexturedVertices[i].position.z = 0.25f;
        g_AnmTexturedVertices[i].rhw = 1.0f;
    }
    // Invalidation must force a bind even though this is the same texture and
    // blend mode cached before the overlay. The real ANM batch then flushes.
    ResetViewportDeviceCalls(S_OK);
    CHECK(manager->DrawInner(&vm, 0) == ZUN_SUCCESS);
    CHECK(ViewportDeviceCallCount() == 2);
    const ViewportDeviceCall *texture = ViewportDeviceCallAt(0);
    CHECK(texture->kind == VIEWPORT_CALL_TEXTURE && texture->device == 0 && texture->count == 0);
    CHECK(texture->texture == static_cast<IDirect3DBaseTexture8 *>(sprite.texture));
    const ViewportDeviceCall *blend = ViewportDeviceCallAt(1);
    CHECK(blend->kind == VIEWPORT_CALL_RENDER_STATE && blend->state == D3DRS_DESTBLEND && blend->value == D3DBLEND_ONE);
    CHECK(manager->currentTexture == sprite.texture && manager->currentBlendMode == 1 && manager->currentVertexShader == 1);
    CHECK(manager->spritesToDraw == 1);
    manager->FlushVertexBuffer();
    CHECK(ViewportDeviceCallCount() == 6 && manager->spritesToDraw == 0);
    CHECK(ViewportDeviceCallAt(4)->kind == VIEWPORT_CALL_VERTEX_SHADER);
    CHECK(ViewportDeviceCallAt(4)->value == (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1));
    const ViewportDeviceCall *draw = ViewportDeviceCallAt(5);
    CHECK(draw->kind == VIEWPORT_CALL_DRAW && draw->state == D3DPT_TRIANGLELIST && draw->count == 2);
    CHECK(draw->stride == sizeof(AnmVertex) && draw->capturedVertexBytes == 6 * sizeof(AnmVertex));
}

int __cdecl main()
{
    CHECK(g_AnmManager == NULL);
    g_Supervisor.InitializeCriticalSections();
    g_Supervisor.InitializeViewports();
    g_Supervisor.d3dDevice = ViewportTestDevice(0);
    AnmManager *manager = new AnmManager;
    g_AnmManager = manager;
    g_Supervisor.ConfigureGameplayViewport(SUPERVISOR_VIEWPORT_PLAYFIELD);
    const ScreenEffectRect rectangles[] = {
        {0, 0, 640, 480}, {128, 16, 512, 464}, {0.25f, -0.5f, 13.75f, 20.5f},
        {7, 9, 7, 9}, {30, 40, -10, -20}, {-100, -200, -1, -0.25f},
        {10000, -10000, 1000000, -1000000}, {-0.0f, 0.0f, -0.0f, 0.0f}
    };
    const u32 colors[] = {0, 0x01020304, 0x80abcdef, 0xffffffff};
    AnmLoadedSprite sprite;
    memset(&sprite, 0, sizeof(sprite));
    sprite.texture = ViewportTestTexture(0);
    sprite.uvStart.x = 0.125f; sprite.uvStart.y = 0.25f;
    sprite.uvEnd.x = 0.75f; sprite.uvEnd.y = 0.875f;
    unsigned int cases = 0;
    for (unsigned int r = 0; r < 8; ++r)
    for (unsigned int color = 0; color < 4; ++color)
    for (unsigned int pending = 0; pending < 2; ++pending)
    for (unsigned int failure = 0; failure < 2; ++failure)
    {
        ScreenEffectRect rect = rectangles[r];
        const ScreenEffectRect before = rect;
        manager->currentSprite = &sprite;
        manager->currentTexture = sprite.texture;
        manager->currentBlendMode = 1;
        manager->currentColorOp = 1;
        manager->currentVertexShader = 1;
        manager->disableZWrite = 0;
        manager->currentTextureFactor = 0x11223344u;
        manager->screenShakeOffset.x = 7.25f;
        manager->screenShakeOffset.y = -3.5f;
        manager->captureSurfaceIdx = 19;
        manager->spritesToDraw = pending;
        manager->flushesThisFrame = 42;
        manager->vertexBufferStartPtr = manager->vertexBuffer;
        manager->vertexBufferEndPtr = manager->vertexBuffer + pending * 6;
        for (unsigned int i = 0; i < 6; ++i)
        {
            manager->vertexBuffer[i].position.x = (float)i;
            manager->vertexBuffer[i].position.y = 30.0f;
            manager->vertexBuffer[i].position.z = 0.5f;
            manager->vertexBuffer[i].rhw = 1.0f;
            manager->vertexBuffer[i].diffuse.color = 0xff998877;
            manager->vertexBuffer[i].uv.x = 0.125f;
            manager->vertexBuffer[i].uv.y = 0.25f;
        }
        Observation o = {manager, &sprite, sprite.texture, false, false, pending};
        ResetViewportDeviceCalls(failure ? E_FAIL : S_OK);
        SetViewportDeviceObserver(ObserveDevice, &o);
        ScreenEffect::DrawSquare(&rect, colors[color]);
        SetViewportDeviceObserver(NULL, NULL);
        CHECK(o.quadSeen && o.restoredAfterInvalidation);
        CHECK(memcmp(&rect, &before, sizeof(rect)) == 0);
        CheckInvalidated(manager);
        CHECK(manager->captureSurfaceIdx == 19 && manager->screenShakeOffset.x == 7.25f && manager->screenShakeOffset.y == -3.5f);
        CHECK(manager->spritesToDraw == 0 && manager->vertexBufferStartPtr == manager->vertexBufferEndPtr);
        CHECK(manager->flushesThisFrame == 42 + pending);
        CHECK(ViewportDeviceCallCount() == 11 + 4 * pending);
        const UINT at = 4 * pending;
        if (pending)
        {
            CheckTextureState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
            CheckTextureState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
            CHECK(ViewportDeviceCallAt(2)->kind == VIEWPORT_CALL_VERTEX_SHADER);
            CHECK(ViewportDeviceCallAt(2)->value == (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1));
            CHECK(ViewportDeviceCallAt(3)->kind == VIEWPORT_CALL_DRAW && ViewportDeviceCallAt(3)->state == D3DPT_TRIANGLELIST);
        }
        CheckTextureState(at, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        CheckTextureState(at + 1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        CheckTextureState(at + 2, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        CheckTextureState(at + 3, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        const ViewportDeviceCall *blend = ViewportDeviceCallAt(at + 4);
        CHECK(blend->kind == VIEWPORT_CALL_RENDER_STATE && blend->device == 0 && blend->state == D3DRS_DESTBLEND && blend->value == D3DBLEND_INVSRCALPHA);
        const ViewportDeviceCall *shader = ViewportDeviceCallAt(at + 5);
        CHECK(shader->kind == VIEWPORT_CALL_VERTEX_SHADER && shader->device == 0 && shader->value == (D3DFVF_XYZRHW | D3DFVF_DIFFUSE));
        const ViewportDeviceCall *draw = ViewportDeviceCallAt(at + 6);
        CHECK(draw->kind == VIEWPORT_CALL_DRAW && draw->device == 0 && draw->state == D3DPT_TRIANGLESTRIP && draw->count == 2);
        CHECK(draw->stride == sizeof(CapturedQuadVertex) && draw->capturedVertexBytes == 4 * sizeof(CapturedQuadVertex));
        CapturedQuadVertex vertices[4]; memcpy(vertices, draw->capturedVertices, sizeof(vertices));
        for (unsigned int i = 0; i < 4; ++i)
        {
            CHECK(SameFloat(vertices[i].x, i & 1 ? rect.right : rect.left));
            CHECK(SameFloat(vertices[i].y, i & 2 ? rect.bottom : rect.top));
            CHECK(SameFloat(vertices[i].z, 0.0f) && vertices[i].rhw == 1.0f && vertices[i].diffuse == colors[color]);
        }
        CheckTextureState(at + 7, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        CheckTextureState(at + 8, D3DTSS_COLOROP, D3DTOP_MODULATE);
        CheckTextureState(at + 9, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        CheckTextureState(at + 10, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        CheckAnmAfterQuad(manager, sprite);
        ++cases;
    }
    g_AnmManager = NULL;
    g_Supervisor.d3dDevice = NULL;
    delete manager;
    printf("canonical quad: %u packet/flush/cache/API-failure/restoration/next-ANM cases passed\n", cases);
    return 0;
}
