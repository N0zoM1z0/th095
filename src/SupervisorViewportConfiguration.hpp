#pragma once

#include <d3d8.h>
#include <d3dx8.h>

#include "SupervisorViewportSlot.hpp"
#include "ZunMath.hpp"

namespace th095
{

// Canonical storage for the two 0xF0-byte configurations embedded in the
// TH095 Supervisor at +0x1E4.  The dword at +0xE4 has only initializer writes
// in the verified target, so it deliberately remains Unknown.
struct SupervisorViewportConfiguration
{
    Float3 cameraPosition;          // +0x000
    Float3 cameraLookAtOffset;      // +0x00c
    Float3 cameraUp;                // +0x018
    Float3 cameraForward;           // +0x024
    Float3 cameraRight;             // +0x030
    Float3 cameraPositionOffset;    // +0x03c
    f32 fieldOfView;                // +0x048
    D3DXMATRIX viewMatrix;          // +0x04c
    D3DXMATRIX projectionMatrix;    // +0x08c
    D3DVIEWPORT8 viewport;          // +0x0cc
    i32 unknown0e4;                 // +0x0e4
    Float2 screenShakeOffset;       // +0x0e8
};

} // namespace th095
