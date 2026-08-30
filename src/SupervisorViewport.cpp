#include "Main.hpp"
#include "AnmManager.hpp"

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
    i32 anmViewportValue0;        // +0x0e8
    i32 anmViewportValue1;        // +0x0ec
};

struct SupervisorViewportView
{
    u8 unknown000[8];
    IDirect3DDevice8 *d3dDevice;                       // +0x008
    u8 unknown00c[0x1e4 - 0x00c];
    GameplayViewportConfiguration configurations[2];  // +0x1e4
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

    supervisor->configurations[1].cameraPosition =
        Float3(0.0f, 0.0f, 1000.0f);
    supervisor->configurations[1].cameraLookAtOffset =
        Float3(0.0f, 0.0f, 0.0f);
    supervisor->configurations[1].cameraUp =
        Float3(0.0f, 1.0f, 0.0f);
    supervisor->configurations[1].fieldOfView = D3DX_PI / 6.0f;
    supervisor->configurations[1].viewport.X = 0;
    supervisor->configurations[1].viewport.Y = 0;
    supervisor->configurations[1].viewport.Width = 640;
    supervisor->configurations[1].viewport.Height = 480;
    supervisor->configurations[1].viewport.MinZ = 0.0f;
    supervisor->configurations[1].viewport.MaxZ = 1.0f;
    supervisor->configurations[1].viewportMode = 1;
    supervisor->configurations[1].cameraPositionOffset =
        Float3(0.0f, 0.0f, 0.0f);

    supervisor->configurations[0].cameraPosition =
        Float3(0.0f, 0.0f, 1000.0f);
    supervisor->configurations[0].cameraLookAtOffset =
        Float3(0.0f, 0.0f, 0.0f);
    supervisor->configurations[0].cameraUp =
        Float3(0.0f, 1.0f, 0.0f);
    supervisor->configurations[0].fieldOfView = D3DX_PI / 6.0f;
    supervisor->configurations[0].viewport.X = 128;
    supervisor->configurations[0].viewport.Y = 16;
    supervisor->configurations[0].viewport.Width = 384;
    supervisor->configurations[0].viewport.Height = 448;
    supervisor->configurations[0].viewport.MinZ = 0.0f;
    supervisor->configurations[0].viewport.MaxZ = 1.0f;
    supervisor->configurations[0].viewportMode = 0;
    supervisor->configurations[0].cameraPositionOffset =
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
        g_AnmManager->unknown020 = configuration->anmViewportValue0;
        g_AnmManager->unknown024 = configuration->anmViewportValue1;
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
