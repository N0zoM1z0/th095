// Actual normal producer/consumers, with typed SDK bindings only at COM I/O.
#include "AnmManager.hpp"
#include "Background.hpp"
#include "ViewportDeviceFixture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(v) do { if (!(v)) { fprintf(stderr, "ANM quad line %d: %s\n", __LINE__, #v); fflush(stderr); abort(); } } while (0)
using namespace th095;
namespace th095 { extern AnmQuadVertexCopy g_BackgroundQuadVertices[4]; }

// Decode the external XYZ/TEX1 stream independently of its production owner.
struct WireVertex { float x, y, z, u, v; };
typedef char WireVertexIs20[sizeof(WireVertex) == 20 ? 1 : -1];
struct Observation { AnmManager *manager; unsigned int seed; bool published; };

static void CheckPublished(AnmManager *manager, unsigned int seed)
{
    WireVertex decoded[4];
    memcpy(decoded, manager->quadVertices, sizeof(decoded));
    for (unsigned int i = 0; i < 4; ++i)
    {
        CHECK(decoded[i].x == (i & 1 ? 128.0f : -128.0f));
        CHECK(decoded[i].y == (i & 2 ? 128.0f : -128.0f));
        CHECK(decoded[i].z == 0.0f && decoded[i].u == (i & 1 ? 1.0f : 0.0f));
        CHECK(decoded[i].v == (i & 2 ? 1.0f : 0.0f));
        const AnmQuadVertexCopy &copy = g_BackgroundQuadVertices[i];
        CHECK(memcmp(&copy.position, &decoded[i], 12) == 0);
        CHECK(memcmp(&copy.textureUV, &decoded[i].u, 8) == 0);
        CHECK(copy.unknown00c == seed + i);
    }
}

static void __cdecl Observe(const ViewportDeviceCall *call, void *context)
{
    Observation &o = *static_cast<Observation *>(context);
    if (call->kind == VIEWPORT_CALL_CREATE_BUFFER)
    {
        CheckPublished(o.manager, o.seed);
        o.published = true;
    }
}

static unsigned int CheckSetup(AnmManager *receiver, AnmManager *other)
{
    unsigned int cases = 0;
    for (unsigned int device = 0; device < 2; ++device)
    for (unsigned int separate = 0; separate < 2; ++separate)
    for (unsigned int failure = 0; failure < 2; ++failure)
    for (unsigned int seed = 0; seed < 2; ++seed)
    {
        g_Supervisor.d3dDevice = ViewportTestDevice(device);
        g_AnmManager = separate ? other : receiver;
        other->quadVertexBuffer = ViewportTestVertexBuffer(1);
        memset(other->quadVertices, 0x35, sizeof(other->quadVertices));
        unsigned char untouched[80]; memcpy(untouched, other->quadVertices, 80);
        receiver->quadVertexBuffer = ViewportTestVertexBuffer(1);
        memset(receiver->quadVertices, 0x79, sizeof(receiver->quadVertices));
        receiver->currentTextureFactor = 0x12345678;
        receiver->spritesToDraw = 0;
        for (unsigned int i = 0; i < 4; ++i)
        {
            g_BackgroundQuadVertices[i].unknown00c = seed + i;
            g_BackgroundQuadVertices[i].position = Float3(11,22,33);
            g_BackgroundQuadVertices[i].textureUV.x = 44;
            g_BackgroundQuadVertices[i].textureUV.y = 55;
        }
        Observation observation = {receiver, seed, false};
        SetViewportDeviceObserver(Observe, &observation);
        // Failure injection retains valid API output storage: test the target's
        // unchecked HRESULT continuation, not a null-buffer recovery contract.
        ResetViewportDeviceCalls(failure ? E_FAIL : S_OK);
        receiver->SetupVertexBuffer();
        SetViewportDeviceObserver(NULL, NULL);
        CHECK(observation.published && ViewportDeviceCallCount() == 4);
        CheckPublished(receiver, seed);
        CHECK(receiver->currentTextureFactor == 0x12345678 && receiver->spritesToDraw == 0);
        CHECK(receiver->quadVertexBuffer == ViewportTestVertexBuffer(0));
        if (separate) CHECK(memcmp(untouched, other->quadVertices, 80) == 0);
        const ViewportDeviceCall *create = ViewportDeviceCallAt(0);
        CHECK(create->kind == VIEWPORT_CALL_CREATE_BUFFER && create->device == device);
        CHECK(create->count == 80 && create->value == 0);
        CHECK(create->state == (D3DFVF_XYZ | D3DFVF_TEX1) && create->stride == D3DPOOL_MANAGED);
        const ViewportDeviceCall *lock = ViewportDeviceCallAt(1);
        CHECK(lock->kind == VIEWPORT_CALL_LOCK_BUFFER && lock->buffer == receiver->quadVertexBuffer);
        CHECK(lock->state == 0 && lock->count == 0 && lock->value == 0);
        const ViewportDeviceCall *unlock = ViewportDeviceCallAt(2);
        CHECK(unlock->kind == VIEWPORT_CALL_UNLOCK_BUFFER && unlock->buffer == receiver->quadVertexBuffer);
        CHECK(unlock->capturedVertexBytes == 80 && memcmp(unlock->capturedVertices, receiver->quadVertices, 80) == 0);
        const ViewportDeviceCall *stream = ViewportDeviceCallAt(3);
        CHECK(stream->kind == VIEWPORT_CALL_STREAM && stream->device == device);
        CHECK(stream->state == 0 && stream->stride == 20 && stream->buffer == g_AnmManager->quadVertexBuffer);
        CHECK((stream->buffer == receiver->quadVertexBuffer) == !separate);
        ++cases;
    }
    CHECK(cases == 16);
    return cases;
}

static void SeedVm(AnmVm &vm, AnmLoadedSprite &sprite)
{
    vm.visible = vm.drawEnabled = true;
    vm.color1.color = 0xffc89664;
    vm.useSecondaryColor = false;
    vm.blendMode = 0;
    vm.updateScale = vm.updateRotation = false;
    vm.unknownFlag14 = true;
    vm.loadedSprite = &sprite;
    vm.position = Float3(0.25f, 0.125f, 0.25f);
    vm.positionOffset = Float3(0,0,0);
    vm.renderStateA = vm.renderStateB = 0;
    vm.uvScrollPos.x = vm.uvScrollPos.y = 0;
    D3DXMatrixIdentity(&vm.matrix2);
    vm.matrix2._11 = vm.matrix2._22 = vm.matrix2._33 = 1.0f / 256.0f;
}

static unsigned int CheckDirectDraw(AnmManager *manager)
{
    unsigned int cases = 0;
    const u8 incoming[] = {0, 1, 2, 3, 0xff};
    AnmLoadedSprite sprite; memset(&sprite, 0, sizeof(sprite));
    sprite.texture = ViewportTestTexture(0);
    for (unsigned int state = 0; state < 5; ++state)
    for (unsigned int failure = 0; failure < 2; ++failure)
    {
        AnmVm vm; SeedVm(vm, sprite);
        manager->currentBlendMode = vm.blendMode;
        manager->currentTextureFactor = vm.color1.color;
        manager->currentTexture = sprite.texture;
        manager->currentSprite = &sprite;
        manager->cachedDrawPath = incoming[state];
        bool hit = incoming[state] == 2;
        manager->spritesToDraw = 0;
        manager->useMixColor = 0;
        ResetViewportDeviceCalls(failure ? E_FAIL : S_OK);
        CHECK(manager->Draw3D(&vm) == ZUN_SUCCESS);
        CHECK(ViewportDeviceCallCount() == (hit ? 2 : 6));
        CHECK(ViewportDeviceCallAt(0)->kind == VIEWPORT_CALL_TRANSFORM);
        CHECK(ViewportDeviceCallAt(0)->state == D3DTS_WORLD);
        if (!hit)
        {
            CHECK(ViewportDeviceCallAt(1)->kind == VIEWPORT_CALL_VERTEX_SHADER);
            CHECK(ViewportDeviceCallAt(1)->value == (D3DFVF_XYZ | D3DFVF_TEX1));
            const ViewportDeviceCall *stream = ViewportDeviceCallAt(2);
            CHECK(stream->kind == VIEWPORT_CALL_STREAM && stream->state == 0 && stream->stride == 20);
            CHECK(stream->buffer == manager->quadVertexBuffer);
            CHECK(ViewportDeviceCallAt(3)->state == D3DTSS_ALPHAARG2 && ViewportDeviceCallAt(3)->value == D3DTA_TFACTOR);
            CHECK(ViewportDeviceCallAt(4)->state == D3DTSS_COLORARG2 && ViewportDeviceCallAt(4)->value == D3DTA_TFACTOR);
        }
        const ViewportDeviceCall *draw = ViewportDeviceCallAt(hit ? 1 : 5);
        CHECK(draw->kind == VIEWPORT_CALL_BUFFER_DRAW && draw->state == D3DPT_TRIANGLESTRIP);
        CHECK(draw->value == 0 && draw->count == 2 && manager->cachedDrawPath == ANM_DRAW_PATH_STREAMED_QUAD);
        ++cases;
    }
    return cases;
}

static u8 BlendByte(u8 source, u8 background, float amount)
{
    return source - (u8)((source - background) * amount);
}

static unsigned int CheckPhotoBlend(AnmManager *manager)
{
    g_Background = new Background;
    g_Background->photoBlendCurrent.farDistance = 2.0f;
    g_Background->photoBlendCurrent.color.color = 0xff112233;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].cameraPosition = Float3(0,0,0);
    SupervisorViewportConfiguration &configuration =
        g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD];
    g_Supervisor.currentViewportConfiguration = &configuration;
    D3DXMatrixIdentity(&configuration.viewMatrix);
    D3DXMatrixIdentity(&configuration.projectionMatrix);
    configuration.viewport.X = configuration.viewport.Y = 0;
    configuration.viewport.Width = 640; configuration.viewport.Height = 480;
    configuration.viewport.MinZ = 0; configuration.viewport.MaxZ = 1;
    AnmLoadedSprite sprite; memset(&sprite, 0, sizeof(sprite));
    sprite.texture = ViewportTestTexture(0);
    unsigned int cases = 0;
    for (unsigned int secondary = 0; secondary < 2; ++secondary)
    for (unsigned int threshold = 0; threshold < 2; ++threshold)
    {
        AnmVm vm; SeedVm(vm, sprite);
        vm.useSecondaryColor = secondary != 0;
        vm.color2.color = 0xff336699;
        const ZunColor input = secondary ? vm.color2 : vm.color1;
        const ZunColor background = g_Background->photoBlendCurrent.color;
        const float nearDistance = threshold ? 0.9f : 0.0f;
        g_Background->photoBlendCurrent.nearDistance = nearDistance;
        manager->currentBlendMode = vm.blendMode;
        manager->currentColorOp = 0;
        manager->currentTexture = sprite.texture;
        manager->currentSprite = &sprite;
        manager->cachedDrawPath = ANM_DRAW_PATH_BUFFERED_QUADS;
        manager->spritesToDraw = 0;
        manager->vertexBufferStartPtr = manager->vertexBuffer;
        manager->vertexBufferEndPtr = manager->vertexBuffer;
        manager->useMixColor = 0;
        ResetViewportDeviceCalls(S_OK);
        CHECK(manager->DrawMode7(&vm) == ZUN_SUCCESS);
        CHECK(manager->spritesToDraw == 1);
        for (unsigned int i = 0; i < 4; ++i)
        {
            // Target Vec4Transform reads the first four source floats: its W
            // is the initialized U coordinate (0/1), not an implicit 1.
            const float w = i & 1 ? 1.0f : 0.0f;
            const float x = (i & 1 ? 0.5f : -0.5f) + w * 0.25f;
            const float y = (i & 2 ? 0.5f : -0.5f) + w * 0.125f;
            const float z = w * 0.25f;
            const float distance = (float)sqrt((double)(x*x + y*y + z*z));
            ZunColor expected = input;
            if (distance > nearDistance)
            {
                const float amount = (nearDistance - distance) / (nearDistance - 2.0f);
                expected.r = BlendByte(input.r, background.r, amount);
                expected.g = BlendByte(input.g, background.g, amount);
                expected.b = BlendByte(input.b, background.b, amount);
            }
            CHECK(g_AnmTexturedVertices[i].diffuse.color == expected.color);
            CHECK(g_AnmTexturedVertices[i].rhw == 1.0f);
        }
        ++cases;
    }
    return cases;
}

int __cdecl main()
{
    unsigned char zeroCopy[96] = {0};
    CHECK(memcmp(g_BackgroundQuadVertices, zeroCopy, sizeof(zeroCopy)) == 0);
    g_Supervisor.InitializeCriticalSections();
    g_Supervisor.InitializeViewports();
    AnmManager *receiver = new AnmManager;
    AnmManager *other = new AnmManager;
    const unsigned int setup = CheckSetup(receiver, other);
    // Draw3D must use its receiver's buffer even when the global manager differs.
    g_AnmManager = other;
    const unsigned int draws = CheckDirectDraw(receiver);
    CHECK(draws == 10);
    const unsigned int photo = CheckPhotoBlend(receiver);
    CHECK(photo == 4);
    printf("ANM quad: %u setup, %u direct-draw and %u U-as-W photo-blend cases passed\n", setup, draws, photo);
    return 0;
}
