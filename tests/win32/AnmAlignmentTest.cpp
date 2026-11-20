// Actual normal VM producers, physical fields and geometry consumers.
#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ViewportDeviceFixture.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(v) do { if (!(v)) { fprintf(stderr, "ANM alignment line %d: %s\n", __LINE__, #v); fflush(stderr); abort(); } } while (0)
using namespace th095;

static unsigned int contextMethod, contextTurn, contextX, contextY, contextSigns, contextPosition;
static void Near(float actual, float expected)
{
    if (fabs(actual - expected) >= 0.0001f)
        fprintf(stderr,"geometry method=%u turn=%u x=%u y=%u signs=%u position=%u actual=%.9g expected=%.9g\n",contextMethod,contextTurn,contextX,contextY,contextSigns,contextPosition,actual,expected);
    CHECK(fabs(actual - expected) < 0.0001f);
}

static unsigned int CheckPhysicalFields()
{
    unsigned int cases = 0;
    AnmVm vm;
    CHECK(vm.flagsWord == 0 && vm.alignmentX == 0 && vm.alignmentY == 0);
    vm.flagsWord = 0xffffffff;
    vm.Initialize();
    CHECK(vm.flagsWord == 7 && vm.alignmentX == 0 && vm.alignmentY == 0);
    cases += 2;
    for (unsigned int bit = 0; bit < 32; ++bit)
    for (unsigned int complement = 0; complement < 2; ++complement)
    for (unsigned int x = 0; x < 4; ++x)
    for (unsigned int y = 0; y < 4; ++y)
    {
        const unsigned int seed = complement ? ~(1u << bit) : 1u << bit;
        vm.flagsWord = seed;
        vm.alignmentX = x;
        const unsigned int afterX = (seed & ~0x000c0000u) | (x << 18);
        CHECK(vm.flagsWord == afterX);
        vm.alignmentY = y;
        CHECK(vm.flagsWord == ((afterX & ~0x00300000u) | (y << 20)));
        CHECK(vm.alignmentX == x && vm.alignmentY == y);
        ++cases;
    }
    return cases;
}

static unsigned int CheckScriptProducer()
{
    const unsigned short arguments[] = {0,1,2,3,4,5,6,7,0x7fff,0x8000,0xfffc,0xfffd,0xfffe,0xffff,0xd8f0,0xd8f1};
    const unsigned short masks[] = {0,3,0xffff};
    const unsigned int seeds[] = {7,0xffffffff};
    unsigned int cases = 0;
    const float savedSpeed = g_AnmGameSpeed;
    for (unsigned int x = 0; x < 16; ++x)
    for (unsigned int y = 0; y < 16; ++y)
    for (unsigned int mask = 0; mask < 3; ++mask)
    for (unsigned int seed = 0; seed < 2; ++seed)
    {
        AnmVm vm;
        AnmRawInstr script[2]; memset(script,0,sizeof(script));
        script[0].opcode = ANM_OP_ALIGNMENT;
        script[0].instructionSize = sizeof(AnmRawInstr);
        script[0].shortArgs[0] = arguments[x]; script[0].shortArgs[1] = arguments[y];
        script[0].varMask = masks[mask];
        script[1].opcode = ANM_OP_STATIC; script[1].instructionSize = sizeof(AnmRawInstr);
        vm.flagsWord = seeds[seed]; vm.scriptDisabled = false;
        const unsigned int before = vm.flagsWord;
        vm.intVar0 = 2; vm.intVar1 = 1;
        vm.beginningOfScript = vm.currentInstruction = script;
        g_AnmGameSpeed = 0.625f;
        CHECK(AnmManager::ExecuteScript(&vm) == 1 && vm.currentInstruction == NULL);
        CHECK(vm.flagsWord == ((before & ~0x003c0000u) | ((arguments[x] & 3u) << 18) | ((arguments[y] & 3u) << 20)));
        CHECK(g_AnmGameSpeed == 0.625f);
        ++cases;
    }
    g_AnmGameSpeed = savedSpeed;
    return cases;
}

static unsigned int CheckAsciiProducers()
{
    const unsigned int seeds[] = {0,0xffffffff,0xa5a5a5a5,0x5a5a5a5a};
    unsigned int cases = 0;
    g_AsciiManager.numStrings = g_AsciiManager.numGuiStrings = 0;
    for (unsigned int gui = 0; gui < 2; ++gui)
    for (unsigned int seed = 0; seed < 4; ++seed)
    {
        g_AsciiManager.largeText.flagsWord = seeds[seed];
        const unsigned int other = g_AsciiManager.smallText.flagsWord;
        ResetViewportDeviceCalls(S_OK);
        if (gui) g_AsciiManager.DrawGuiStrings(); else g_AsciiManager.DrawStrings();
        CHECK(g_AsciiManager.largeText.flagsWord == ((seeds[seed] & ~0x003c0000u) | 0x00140001u));
        CHECK(g_AsciiManager.smallText.flagsWord == other);
        CHECK(g_AsciiManager.numStrings == 0 && g_AsciiManager.numGuiStrings == 0);
        ++cases;
    }
    return cases;
}

static float RoundEven(float value)
{
    const float low = (float)floor(value), fraction = value - low;
    return fraction < 0.5f ? low : fraction > 0.5f ? low + 1 : ((int)low & 1) ? low + 1 : low;
}

static void SeedVm(AnmVm &vm, AnmLoadedSprite &sprite)
{
    vm.visible = vm.drawEnabled = true;
    vm.color1.color = 0xffffffff;
    vm.loadedSprite = &sprite; vm.blendMode = ANM_BLEND_ALPHA;
    vm.spriteSize.x = 13.5f; vm.spriteSize.y = 11.25f;
    vm.positionOffset = Float3(3.125f,-2.25f,0.125f);
    vm.unknownFlag14 = true; vm.updateRotation = vm.updateScale = false;
}

static void SeedManager(AnmManager *manager, AnmLoadedSprite &sprite)
{
    manager->ClearVertexBuffer(); manager->cachedDrawPath = ANM_DRAW_PATH_BUFFERED_QUADS;
    manager->cachedBlendMode = ANM_BLEND_ALPHA; manager->currentTexture = sprite.texture;
    manager->currentTextureFactor = 0xffffffff; manager->currentSprite = &sprite;
    manager->useMixColor = 0;
    manager->screenShakeOffset.x = 0.375f; manager->screenShakeOffset.y = 0.125f;
    for (unsigned int vertex = 0; vertex < 4; ++vertex)
    {
        g_AnmTexturedVertices[vertex].position = Float3(vertex & 1 ? 135.75f : 122.25f, vertex & 2 ? 181.5f : 171.5f, 0);
        g_AnmTexturedVertices[vertex].rhw = 1;
    }
    ResetViewportDeviceCalls(S_OK);
}

static unsigned int CheckSoftwareGeometry(AnmManager *manager, AnmLoadedSprite &sprite)
{
    const Float3 positions[] = {Float3(173.25f,199.75f,0.25f),Float3(-20.375f,199.75f,0.25f),Float3(636.875f,199.75f,0.25f),Float3(173.25f,-6.3125f,0.25f),Float3(173.25f,482.5f,0.25f)};
    unsigned int cases = 0;
    for (unsigned int method = 0; method < 3; ++method)
    for (unsigned int turn = 0; turn < (method == 2 ? 2u : 1u); ++turn)
    for (unsigned int signs = 0; signs < 4; ++signs)
    for (unsigned int x = 0; x < (method == 2 ? 3u : 4u); ++x)
    for (unsigned int y = 0; y < (method == 2 ? 3u : 4u); ++y)
    for (unsigned int position = 0; position < 5; ++position)
    {
        contextMethod = method; contextTurn = turn; contextX = x; contextY = y; contextSigns = signs; contextPosition = position;
        AnmVm vm; SeedVm(vm,sprite); SeedManager(manager,sprite);
        vm.position = positions[position]; vm.scale.x = signs & 1 ? -1.25f : 1.25f; vm.scale.y = signs & 2 ? -0.75f : 0.75f;
        vm.alignmentX = x; vm.alignmentY = y; vm.rotation.z = turn ? D3DX_PI * 0.5f : 0;
        const float width = vm.spriteSize.x * vm.scale.x, height = vm.spriteSize.y * vm.scale.y;
        const float originX = vm.position.x + vm.positionOffset.x, originY = vm.position.y + vm.positionOffset.y;
        float xs[4],ys[4];
        for (unsigned int seed = 0; seed < 4; ++seed)
        { xs[seed] = g_AnmTexturedVertices[seed].position.x; ys[seed] = g_AnmTexturedVertices[seed].position.y; }
        if (turn)
        {
            const float left = x == 0 ? -width / 2 : x == 1 ? 0 : -width;
            const float top = y == 0 ? -height / 2 : y == 1 ? 0 : -height;
            for (unsigned int rotated = 0; rotated < 4; ++rotated)
            { xs[rotated] = originX - (top + (rotated & 2 ? height : 0)); ys[rotated] = originY + left + (rotated & 1 ? width : 0); }
        }
        else
        {
            if (x != 3)
            {
                float left = originX - (x == 0 ? width / 2 : x == 1 ? 0 : width);
                if (method == 0 && x == 0) left = (float)floor(left);
                xs[0] = xs[2] = left; xs[1] = xs[3] = left + width;
            }
            if (y != 3)
            {
                float top = originY - (y == 0 ? height / 2 : y == 1 ? 0 : height);
                if (method == 0 && y == 0) top = (float)floor(top);
                ys[0] = ys[1] = top; ys[2] = ys[3] = top + height;
            }
        }
        float minX = 1e6f,maxX = -1e6f,minY = 1e6f,maxY = -1e6f;
        for (unsigned int vertex = 0; vertex < 4; ++vertex)
        {
            xs[vertex] += 0.375f; ys[vertex] += 0.125f;
            if (method == 0) { xs[vertex] = RoundEven(xs[vertex]) - 0.5f; ys[vertex] = RoundEven(ys[vertex]) - 0.5f; }
            if (xs[vertex] < minX) minX = xs[vertex]; if (xs[vertex] > maxX) maxX = xs[vertex];
            if (ys[vertex] < minY) minY = ys[vertex]; if (ys[vertex] > maxY) maxY = ys[vertex];
        }
        const bool culled = maxX < 0 || maxY < 0 || minX > 640 || minY > 480;
        const th095::ZunResult result = method == 0 ? manager->DrawNoRotation(&vm) : method == 1 ? manager->DrawNoRotationNoRound(&vm) : manager->Draw2D(&vm);
        CHECK(result == ZUN_SUCCESS && ViewportDeviceCallCount() == 0);
        CHECK(manager->spritesToDraw == (culled ? 0u : 1u));
        const float expectedZ = turn ? vm.position.z : vm.position.z + vm.positionOffset.z;
        for (unsigned int check = 0; check < 4; ++check)
        { Near(g_AnmTexturedVertices[check].position.x,xs[check]); Near(g_AnmTexturedVertices[check].position.y,ys[check]); Near(g_AnmTexturedVertices[check].position.z,expectedZ); }
        if (!culled)
        {
            const unsigned int order[] = {0,1,2,1,2,3};
            const AnmVertex *queued = reinterpret_cast<const AnmVertex *>(manager->vertexBufferStartPtr);
            for (unsigned int packet = 0; packet < 6; ++packet)
            { Near(queued[packet].position.x,xs[order[packet]]); Near(queued[packet].position.y,ys[order[packet]]); }
        }
        ++cases;
    }
    return cases;
}

static unsigned int CheckDirectGeometry(AnmManager *manager, AnmLoadedSprite &sprite)
{
    unsigned int cases = 0;
    for (unsigned int x = 0; x < 4; ++x)
    for (unsigned int y = 0; y < 4; ++y)
    for (unsigned int signs = 0; signs < 4; ++signs)
    for (unsigned int failure = 0; failure < 2; ++failure)
    {
        AnmVm vm; SeedVm(vm,sprite); SeedManager(manager,sprite);
        vm.position = Float3(173.25f,199.75f,0.25f); vm.scale.x = signs & 1 ? -1.25f : 1.25f; vm.scale.y = signs & 2 ? -0.75f : 0.75f;
        vm.alignmentX = x; vm.alignmentY = y;
        D3DXMatrixIdentity(&vm.matrix2); vm.matrix2._11 = 0.125f; vm.matrix2._22 = -0.25f;
        vm.matrix2._41 = 33.25f; vm.matrix2._42 = -44.5f; vm.matrix2._43 = 0.125f;
        const D3DXMATRIX before = vm.matrix2;
        D3DXMATRIX expected = before;
        const float offsets[] = {0,-8.4375f,8.4375f};
        const float vertical[] = {0,-4.21875f,4.21875f};
        if (x != 3) expected._41 = 176.375f + offsets[x];
        if (y != 3) expected._42 = 197.5f + vertical[y];
        expected._43 = 0.375f;
        manager->cachedDrawPath = ANM_DRAW_PATH_STREAMED_QUAD;
        ResetViewportDeviceCalls(failure ? E_FAIL : S_OK);
        CHECK(manager->Draw3D(&vm) == ZUN_SUCCESS && ViewportDeviceCallCount() == 2);
        CHECK(ViewportDeviceCallAt(0)->kind == VIEWPORT_CALL_TRANSFORM && ViewportDeviceCallAt(0)->state == D3DTS_WORLD);
        CHECK(memcmp(&ViewportDeviceCallAt(0)->matrix,&expected,sizeof(expected)) == 0);
        CHECK(memcmp(&vm.matrix2,&before,sizeof(before)) == 0);
        CHECK(ViewportDeviceCallAt(1)->kind == VIEWPORT_CALL_BUFFER_DRAW);
        ++cases;
    }
    for (unsigned int rejected = 0; rejected < 3; ++rejected)
    for (unsigned int horizontal = 0; horizontal < 4; ++horizontal)
    for (unsigned int vertical = 0; vertical < 4; ++vertical)
    {
        AnmVm vm; SeedVm(vm,sprite); SeedManager(manager,sprite);
        vm.alignmentX = horizontal; vm.alignmentY = vertical;
        if (rejected == 0) vm.visible = false; else if (rejected == 1) vm.drawEnabled = false; else vm.color1.a = 0;
        AnmVertex pending[4]; memset(pending,0,sizeof(pending)); manager->AddSpriteToDrawBuffer(pending);
        CHECK(manager->Draw3D(&vm) == ZUN_ERROR && ViewportDeviceCallCount() == 0 && manager->spritesToDraw == 1);
        ++cases;
    }
    return cases;
}

int __cdecl main()
{
    CHECK((_control87(0,0) & _MCW_RC) == _RC_NEAR);
    g_Supervisor.InitializeCriticalSections(); g_Supervisor.InitializeViewports();
    g_Supervisor.d3dDevice = ViewportTestDevice(0);
    AnmManager *manager = new AnmManager; g_AnmManager = manager;
    manager->quadVertexBuffer = ViewportTestVertexBuffer(0);
    const unsigned int physical = CheckPhysicalFields(), scripts = CheckScriptProducer(), ascii = CheckAsciiProducers();
    SupervisorViewportConfiguration &configuration = g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD];
    configuration.viewport.X = configuration.viewport.Y = 0; configuration.viewport.Width = 640; configuration.viewport.Height = 480;
    g_Supervisor.currentViewportConfiguration = &configuration;
    AnmLoadedSprite sprite; memset(&sprite,0,sizeof(sprite)); sprite.texture = ViewportTestTexture(0); sprite.uvEnd.x = sprite.uvEnd.y = 1;
    const unsigned int software = CheckSoftwareGeometry(manager,sprite), direct = CheckDirectGeometry(manager,sprite);
    CHECK(physical == 1026 && scripts == 1536 && ascii == 8 && software == 1000 && direct == 176);
    printf("ANM alignment: %u physical/default, %u literal-script, %u actual ASCII, %u software/boundary and %u direct-world/gate cases passed\n",physical,scripts,ascii,software,direct);
    return 0;
}
