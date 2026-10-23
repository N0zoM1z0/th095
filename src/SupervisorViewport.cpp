#include "SupervisorRuntime.hpp"
#include "AnmManager.hpp"
#include "SupervisorViewportConfiguration.hpp"
#include <math.h>

namespace th095
{

// FUNCTION: TH095 0x00425CC0.
void Supervisor::InitializeViewports()
{

    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW].cameraPosition =
        Float3(0.0f, 0.0f, 1000.0f);
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW].cameraLookAtOffset =
        Float3(0.0f, 0.0f, 0.0f);
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW].cameraUp =
        Float3(0.0f, 1.0f, 0.0f);
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW].fieldOfView = D3DX_PI / 6.0f;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW].viewport.X = 0;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW].viewport.Y = 0;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW].viewport.Width = 640;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW].viewport.Height = 480;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW].viewport.MinZ = 0.0f;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW].viewport.MaxZ = 1.0f;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW].unknown0e4 = 1;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_FULL_WINDOW].cameraPositionOffset =
        Float3(0.0f, 0.0f, 0.0f);

    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].cameraPosition =
        Float3(0.0f, 0.0f, 1000.0f);
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].cameraLookAtOffset =
        Float3(0.0f, 0.0f, 0.0f);
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].cameraUp =
        Float3(0.0f, 1.0f, 0.0f);
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].fieldOfView = D3DX_PI / 6.0f;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].viewport.X = 128;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].viewport.Y = 16;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].viewport.Width = 384;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].viewport.Height = 448;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].viewport.MinZ = 0.0f;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].viewport.MaxZ = 1.0f;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].unknown0e4 = 0;
    g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD].cameraPositionOffset =
        Float3(0.0f, 0.0f, 0.0f);
}

// FUNCTION: TH095 0x00425910.
#define cameraDistance soundIndexLocal01
#define viewportMiddleHeight jLocal00
#define viewportMiddleWidth preloadBufferLocal03
#define aspectRatio bgmPathLocal18
#define fov bgmFormatIndexLocal05
#pragma var_order(cameraDistance, viewportMiddleHeight, viewportMiddleWidth, aspectRatio, fov, this)
void Supervisor::ApplyGameplayViewport(
    SupervisorViewportConfiguration *configuration)
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

    // Target 0x00425A72..0x00425A93 publishes x and y separately, reloading
    // the ANM owner for each store. VC7.1 emits those exact DWORD moves for
    // these typed scalar assignments; aggregate Float2 copying emits 387
    // instead of 396 bytes and one fewer owner relocation. Keep one shared
    // source in both profiles without the old integer alias casts.
    if (g_AnmManager != NULL)
    {
        g_AnmManager->screenShakeOffset.x = configuration->screenShakeOffset.x;
        g_AnmManager->screenShakeOffset.y = configuration->screenShakeOffset.y;
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
    this->currentViewportConfiguration = &this->viewportConfigurations[index];
    this->ApplyGameplayViewport(this->currentViewportConfiguration);
    this->d3dDevice->SetViewport(&this->currentViewportConfiguration->viewport);
    this->currentViewportIndex = index;
}

} // namespace th095
