// Use the SDK's C COM declaration to bind the real IDirect3DDevice8 vtable.
// Production C++ objects still execute their ordinary SDK virtual calls.
#define CINTERFACE
#include "ViewportDeviceFixture.h"

#include <stdlib.h>
#include <string.h>

struct FixtureDevice
{
    IDirect3DDevice8 sdkDevice;
    int index;
};

static IDirect3DDevice8Vtbl vtable;
static IDirect3DTexture8 textures[2];
static ViewportDeviceObserver observer;
static void *observerContext;
static struct FixtureDevice devices[2];
static struct ViewportDeviceCall calls[32];
static UINT callCount;
static HRESULT callResult;

static struct ViewportDeviceCall *RecordCall(
    IDirect3DDevice8 *device, enum ViewportDeviceCallKind kind)
{
    struct ViewportDeviceCall *call;
    if (callCount == sizeof(calls) / sizeof(calls[0]))
        abort();
    call = &calls[callCount++];
    memset(call, 0, sizeof(*call));
    call->device = ((struct FixtureDevice *)device)->index;
    call->kind = kind;
    return call;
}

static void Notify(struct ViewportDeviceCall *call)
{
    if (observer != NULL)
        observer(call, observerContext);
}

static HRESULT WINAPI CaptureTransform(
    IDirect3DDevice8 *device, D3DTRANSFORMSTATETYPE state, CONST D3DMATRIX *matrix)
{
    struct ViewportDeviceCall *call = RecordCall(device, VIEWPORT_CALL_TRANSFORM);
    call->state = state;
    call->matrix = *matrix;
    Notify(call);
    return callResult;
}

static HRESULT WINAPI CaptureViewport(IDirect3DDevice8 *device, CONST D3DVIEWPORT8 *viewport)
{
    struct ViewportDeviceCall *call = RecordCall(device, VIEWPORT_CALL_VIEWPORT);
    call->viewport = *viewport;
    Notify(call);
    return callResult;
}

static HRESULT WINAPI CaptureTextureState(
    IDirect3DDevice8 *device, DWORD stage, D3DTEXTURESTAGESTATETYPE state, DWORD value)
{
    struct ViewportDeviceCall *call = RecordCall(device, VIEWPORT_CALL_TEXTURE_STATE);
    call->state = state;
    call->value = value;
    call->count = stage;
    Notify(call);
    return callResult;
}

static HRESULT WINAPI CaptureVertexShader(IDirect3DDevice8 *device, DWORD shader)
{
    struct ViewportDeviceCall *call = RecordCall(device, VIEWPORT_CALL_VERTEX_SHADER);
    call->value = shader;
    Notify(call);
    return callResult;
}

static HRESULT WINAPI CaptureDraw(
    IDirect3DDevice8 *device, D3DPRIMITIVETYPE primitive, UINT count,
    CONST void *vertices, UINT stride)
{
    struct ViewportDeviceCall *call = RecordCall(device, VIEWPORT_CALL_DRAW);
    call->state = primitive;
    call->vertices = vertices;
    call->count = count;
    call->stride = stride;
    // DrawPrimitiveUP consumes caller memory synchronously. Preserve its bytes
    // while stack-based quad vertices are alive; the raw pointer is identity only.
    if (primitive == D3DPT_TRIANGLESTRIP)
        call->capturedVertexBytes = (count + 2) * stride;
    else if (primitive == D3DPT_TRIANGLELIST)
        call->capturedVertexBytes = count * 3 * stride;
    else
        abort();
    if (call->capturedVertexBytes > sizeof(call->capturedVertices))
        abort();
    memcpy(call->capturedVertices, vertices, call->capturedVertexBytes);
    Notify(call);
    return callResult;
}

static HRESULT WINAPI CaptureRenderState(
    IDirect3DDevice8 *device, D3DRENDERSTATETYPE state, DWORD value)
{
    struct ViewportDeviceCall *call = RecordCall(device, VIEWPORT_CALL_RENDER_STATE);
    call->state = state;
    call->value = value;
    Notify(call);
    return callResult;
}

static HRESULT WINAPI CaptureTexture(
    IDirect3DDevice8 *device, DWORD stage, IDirect3DBaseTexture8 *texture)
{
    struct ViewportDeviceCall *call = RecordCall(device, VIEWPORT_CALL_TEXTURE);
    call->count = stage;
    call->texture = texture;
    Notify(call);
    return callResult;
}

IDirect3DTexture8 *ViewportTestTexture(int index)
{
    if (index < 0 || index >= 2)
        abort();
    return &textures[index];
}

void SetViewportDeviceObserver(ViewportDeviceObserver value, void *context)
{
    observer = value;
    observerContext = context;
}

IDirect3DDevice8 *ViewportTestDevice(int index)
{
    if (index < 0 || index >= 2)
        abort();
    vtable.SetTransform = CaptureTransform;
    vtable.SetViewport = CaptureViewport;
    vtable.SetTextureStageState = CaptureTextureState;
    vtable.SetVertexShader = CaptureVertexShader;
    vtable.DrawPrimitiveUP = CaptureDraw;
    vtable.SetRenderState = CaptureRenderState;
    vtable.SetTexture = CaptureTexture;
    devices[index].sdkDevice.lpVtbl = &vtable;
    devices[index].index = index;
    return &devices[index].sdkDevice;
}

void ResetViewportDeviceCalls(HRESULT result)
{
    callCount = 0;
    callResult = result;
}

UINT ViewportDeviceCallCount(void)
{
    return callCount;
}

const struct ViewportDeviceCall *ViewportDeviceCallAt(UINT index)
{
    if (index >= callCount)
        abort();
    return &calls[index];
}
