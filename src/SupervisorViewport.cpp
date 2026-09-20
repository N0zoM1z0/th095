#include "Main.hpp"
#include "AnmManager.hpp"
#include "SupervisorViewportConfiguration.hpp"
#include <math.h>

namespace th095
{

// Fieldless compiler-emission adapter: the historical method decoration names
// GameplayViewportConfiguration, while storage and semantics are owned by the
// canonical base declaration.
struct GameplayViewportConfiguration : SupervisorViewportConfiguration
{
};

struct SupervisorViewportView
{
    u8 unknown000[8];
    IDirect3DDevice8 *d3dDevice;                       // +0x008
    u8 unknown00c[0x1e4 - 0x00c];
    GameplayViewportConfiguration configurations[TH095_SUPERVISOR_VIEWPORT_SLOT_COUNT];  // +0x1e4
    GameplayViewportConfiguration *current;           // +0x3c4
    i32 currentIndex;                                  // +0x3c8

    void ApplyGameplayViewport(GameplayViewportConfiguration *configuration);
};

typedef char SupervisorViewportConfigurationSizeIsF0[
    (sizeof(SupervisorViewportConfiguration) == 0xf0) ? 1 : -1];
typedef char SupervisorViewportCurrentAt3C4[
    (offsetof(SupervisorViewportView, current) == 0x3c4) ? 1 : -1];

// FUNCTION: TH095 0x00425CC0.
void Supervisor::InitializeViewports()
{
#define supervisor (reinterpret_cast<SupervisorViewportView *>(&g_Supervisor))

    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW].cameraPosition =
        Float3(0.0f, 0.0f, 1000.0f);
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW].cameraLookAtOffset =
        Float3(0.0f, 0.0f, 0.0f);
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW].cameraUp =
        Float3(0.0f, 1.0f, 0.0f);
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW].fieldOfView = D3DX_PI / 6.0f;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW].viewport.X = 0;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW].viewport.Y = 0;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW].viewport.Width = 640;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW].viewport.Height = 480;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW].viewport.MinZ = 0.0f;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW].viewport.MaxZ = 1.0f;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW].unknown0e4 = 1;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW].cameraPositionOffset =
        Float3(0.0f, 0.0f, 0.0f);

    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].cameraPosition =
        Float3(0.0f, 0.0f, 1000.0f);
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].cameraLookAtOffset =
        Float3(0.0f, 0.0f, 0.0f);
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].cameraUp =
        Float3(0.0f, 1.0f, 0.0f);
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].fieldOfView = D3DX_PI / 6.0f;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].viewport.X = 128;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].viewport.Y = 16;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].viewport.Width = 384;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].viewport.Height = 448;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].viewport.MinZ = 0.0f;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].viewport.MaxZ = 1.0f;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].unknown0e4 = 0;
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].cameraPositionOffset =
        Float3(0.0f, 0.0f, 0.0f);
#undef supervisor
}

// FUNCTION: TH095 0x00425910.
#define cameraDistance soundIndexLocal01
#define viewportMiddleHeight jLocal00
#define viewportMiddleWidth preloadBufferLocal03
#define aspectRatio bgmPathLocal18
#define fov bgmFormatIndexLocal05
#pragma var_order(cameraDistance, viewportMiddleHeight, viewportMiddleWidth, aspectRatio, fov, this)
void SupervisorViewportView::ApplyGameplayViewport(
    GameplayViewportConfiguration *configuration)
{
    f32 fov;
    f32 aspectRatio;
    f32 viewportMiddleWidth;
    f32 viewportMiddleHeight;
    f32 cameraDistance;

    if (g_AnmManager != NULL)
        g_AnmManager->FlushVertexBuffer();

    viewportMiddleWidth = (f32)configuration->viewport.Width / 2.0f;
    viewportMiddleHeight = (f32)configuration->viewport.Height / 2.0f;
    aspectRatio = (f32)configuration->viewport.Width /
                  (f32)configuration->viewport.Height;
    fov = D3DX_PI / 10.0f;
    cameraDistance = viewportMiddleHeight / (f32)tan(fov / 2.0f);

    D3DXMatrixLookAtLH(
        &configuration->viewMatrix,
        &D3DXVECTOR3(viewportMiddleWidth, viewportMiddleHeight, cameraDistance),
        &D3DXVECTOR3(viewportMiddleWidth, viewportMiddleHeight, 0.0f),
        &D3DXVECTOR3(0.0f, -1.0f, 0.0f));
    D3DXMatrixPerspectiveFovLH(
        &configuration->projectionMatrix, fov, aspectRatio, 1.0f, 10000.0f);
    g_Supervisor.d3dDevice->SetTransform(
        D3DTS_VIEW, &configuration->viewMatrix);
    g_Supervisor.d3dDevice->SetTransform(
        D3DTS_PROJECTION, &configuration->projectionMatrix);

    if (g_AnmManager != NULL)
    {
#if defined(TH095_MATCH_EXACT)
        g_AnmManager->unknown020 =
            *reinterpret_cast<i32 *>(&configuration->screenShakeOffset.x);
        g_AnmManager->unknown024 =
            *reinterpret_cast<i32 *>(&configuration->screenShakeOffset.y);
#else
        g_AnmManager->screenShakeOffset = configuration->screenShakeOffset;
#endif
    }
}
#undef cameraDistance
#undef viewportMiddleHeight
#undef viewportMiddleWidth
#undef aspectRatio
#undef fov

// FUNCTION: TH095 0x00404B10.
void Supervisor::ConfigureGameplayViewport(i32 index)
{
    reinterpret_cast<SupervisorViewportView *>(this)->current =
        &reinterpret_cast<SupervisorViewportView *>(this)
             ->configurations[index];
    reinterpret_cast<SupervisorViewportView *>(this)->ApplyGameplayViewport(
        reinterpret_cast<SupervisorViewportView *>(this)->current);
    reinterpret_cast<SupervisorViewportView *>(this)->d3dDevice->SetViewport(
        &reinterpret_cast<SupervisorViewportView *>(this)->current->viewport);
    reinterpret_cast<SupervisorViewportView *>(this)->currentIndex = index;
}

} // namespace th095
