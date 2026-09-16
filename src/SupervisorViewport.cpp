#include "Main.hpp"
#include "AnmManager.hpp"
#ifdef TH095_MATCH_EXACT
#define TH095_SUPERVISOR_VIEWPORT_PLAYFIELD 0
#define TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW 1
#define TH095_SUPERVISOR_VIEWPORT_SLOT_COUNT 2
#else
#include "SupervisorViewportSlot.hpp"
#endif
#include <math.h>

namespace th095
{

struct GameplayViewportConfiguration
{
    Float3 cameraPosition;         // +0x000
    Float3 cameraLookAtOffset;     // +0x00c
    Float3 cameraUp;               // +0x018
    Float3 cameraForward;          // +0x024
    Float3 cameraRight;            // +0x030
    Float3 cameraPositionOffset;   // +0x03c
    f32 fieldOfView;               // +0x048
    D3DXMATRIX viewMatrix;        // +0x04c
    D3DXMATRIX projectionMatrix;  // +0x08c
    D3DVIEWPORT8 viewport;        // +0x0cc
    i32 viewportMode;              // +0x0e4
#if defined(TH095_MATCH_EXACT)
    i32 anmViewportValue0;        // +0x0e8
    i32 anmViewportValue1;        // +0x0ec
#else
    Float2 screenShakeOffset;     // +0x0e8
#endif
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

typedef char GameplayViewportConfigurationSizeIsF0[
    (sizeof(GameplayViewportConfiguration) == 0xf0) ? 1 : -1];
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
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_FULL_WINDOW].viewportMode = 1;
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
    supervisor->configurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].viewportMode = 0;
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
        g_AnmManager->unknown020 = configuration->anmViewportValue0;
        g_AnmManager->unknown024 = configuration->anmViewportValue1;
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
