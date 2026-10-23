#pragma once

#include <d3d8.h>

// Real SDK COM bindings, with only the viewport client's API calls implemented.
// Unused vtable entries stay null so an unexpected API dependency fails the test.
enum ViewportDeviceCallKind
{
    VIEWPORT_CALL_TRANSFORM,
    VIEWPORT_CALL_VIEWPORT,
    VIEWPORT_CALL_TEXTURE_STATE,
    VIEWPORT_CALL_VERTEX_SHADER,
    VIEWPORT_CALL_DRAW
};

struct ViewportDeviceCall
{
    int device;
    enum ViewportDeviceCallKind kind;
    DWORD state;
    DWORD value;
    D3DMATRIX matrix;
    D3DVIEWPORT8 viewport;
    const void *vertices;
    UINT count;
    UINT stride;
};

#ifdef __cplusplus
extern "C" {
#endif

IDirect3DDevice8 *ViewportTestDevice(int index);
void ResetViewportDeviceCalls(HRESULT result);
UINT ViewportDeviceCallCount(void);
const struct ViewportDeviceCall *ViewportDeviceCallAt(UINT index);

#ifdef __cplusplus
}
#endif
