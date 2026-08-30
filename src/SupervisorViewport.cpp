#include "Main.hpp"

namespace th095
{

struct GameplayViewportConfiguration
{
    u8 unknown000[0xcc];
    D3DVIEWPORT8 viewport;  // +0xcc
    u8 unknown0e4[0x0c];
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
