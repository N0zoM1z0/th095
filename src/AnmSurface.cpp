#include "AnmManager.hpp"

#include <stdlib.h>

namespace th095
{

struct AnmSurfaceStorageView
{
    u8 unconsumed000[0x125c];
    IDirect3DSurface8 *surfacesBis[32];
    u8 *surfaceData[32];
    i32 surfaceDataSizes[32];
    D3DXIMAGE_INFO surfaceInfo[32];
};

typedef char AnmSurfacePrimaryOffset[(offsetof(AnmManager, surfaces) == 0x11dc) ? 1 : -1];
typedef char AnmSurfaceSecondaryOffset[(offsetof(AnmSurfaceStorageView, surfacesBis) == 0x125c) ? 1 : -1];
typedef char AnmSurfaceDataOffset[(offsetof(AnmSurfaceStorageView, surfaceData) == 0x12dc) ? 1 : -1];
typedef char AnmSurfaceDataSizeOffset[(offsetof(AnmSurfaceStorageView, surfaceDataSizes) == 0x135c) ? 1 : -1];
typedef char AnmSurfaceInfoOffset[(offsetof(AnmSurfaceStorageView, surfaceInfo) == 0x13dc) ? 1 : -1];

#define surfaceStorage reinterpret_cast<AnmSurfaceStorageView *>(this)

// FUNCTION: TH095 0x004440F0.
i32 AnmManager::LoadSurface(i32 surfaceIndex, const char *path)
{
    struct LoadSurfaceLocals
    {
        u8 *fileData;
        i32 fileSize;
        IDirect3DSurface8 *surface;
    } locals;

    if (this->surfaces[surfaceIndex] != NULL)
    {
        this->ReleaseSurface(surfaceIndex);
    }

    if (surfaceStorage->surfaceData[surfaceIndex] == NULL)
    {
        locals.fileData = FileSystem::OpenFile((char *)path, &locals.fileSize, 0);
        if (locals.fileData == NULL)
        {
            g_GameErrorContext.Fatal(
                "%s\x82\xaa\x93\xc7\x82\xdd\x8d\x9e\x82\xdf\x82\xc8"
                "\x82\xa2\x82\xc5\x82\xb7\x81\x42\r\n",
                path);
            return ZUN_ERROR;
        }
    }
    else
    {
        locals.fileData = surfaceStorage->surfaceData[surfaceIndex];
        locals.fileSize = surfaceStorage->surfaceDataSizes[surfaceIndex];
        surfaceStorage->surfaceData[surfaceIndex] = NULL;
    }

    if (g_Supervisor.d3dDevice->CreateImageSurface(
            640, 1024, g_Supervisor.presentParameters.BackBufferFormat,
            &locals.surface) != D3D_OK)
    {
        return ZUN_ERROR;
    }

    if (D3DXLoadSurfaceFromFileInMemory(
            locals.surface, NULL, NULL, locals.fileData, locals.fileSize, NULL, 1, 0,
            &surfaceStorage->surfaceInfo[surfaceIndex]) != D3D_OK)
    {
        goto error;
    }

    if (g_Supervisor.d3dDevice->CreateRenderTarget(
            surfaceStorage->surfaceInfo[surfaceIndex].Width,
            surfaceStorage->surfaceInfo[surfaceIndex].Height,
            g_Supervisor.presentParameters.BackBufferFormat,
            D3DMULTISAMPLE_NONE, 1, &this->surfaces[surfaceIndex]) != D3D_OK)
    {
        if (g_Supervisor.d3dDevice->CreateImageSurface(
                surfaceStorage->surfaceInfo[surfaceIndex].Width,
                surfaceStorage->surfaceInfo[surfaceIndex].Height,
                g_Supervisor.presentParameters.BackBufferFormat,
                &this->surfaces[surfaceIndex]) != D3D_OK)
        {
            goto error;
        }
    }

    if (g_Supervisor.d3dDevice->CreateImageSurface(
            surfaceStorage->surfaceInfo[surfaceIndex].Width,
            surfaceStorage->surfaceInfo[surfaceIndex].Height,
            g_Supervisor.presentParameters.BackBufferFormat,
            &surfaceStorage->surfacesBis[surfaceIndex]) != D3D_OK)
    {
        goto error;
    }

    if (D3DXLoadSurfaceFromSurface(
            this->surfaces[surfaceIndex], NULL, NULL, locals.surface, NULL, NULL,
            D3DX_FILTER_NONE, 0) != D3D_OK)
    {
        goto error;
    }

    if (D3DXLoadSurfaceFromSurface(
            surfaceStorage->surfacesBis[surfaceIndex], NULL, NULL, locals.surface, NULL, NULL,
            D3DX_FILTER_NONE, 0) != D3D_OK)
    {
        goto error;
    }

    if (locals.surface != NULL)
    {
        locals.surface->Release();
        locals.surface = NULL;
    }
    free(locals.fileData);
    return ZUN_SUCCESS;

error:
    if (locals.surface != NULL)
    {
        locals.surface->Release();
        locals.surface = NULL;
    }
    free(locals.fileData);
    return ZUN_ERROR;
}

// FUNCTION: TH095 0x00444390.
void AnmManager::ReleaseSurface(i32 surfaceIndex)
{
    void *data;

    if (this->surfaces[surfaceIndex] != NULL)
    {
        this->surfaces[surfaceIndex]->Release();
        this->surfaces[surfaceIndex] = NULL;
    }
    if (surfaceStorage->surfacesBis[surfaceIndex] != NULL)
    {
        surfaceStorage->surfacesBis[surfaceIndex]->Release();
        surfaceStorage->surfacesBis[surfaceIndex] = NULL;
    }
    if (surfaceStorage->surfaceData[surfaceIndex] != NULL)
    {
        data = surfaceStorage->surfaceData[surfaceIndex];
        free(data);
    }
    surfaceStorage->surfaceData[surfaceIndex] = NULL;
}

// FUNCTION: TH095 0x00444460.
void AnmManager::CopySurfaceToBackbuffer(
    i32 surfaceIndex, i32 left, i32 top, i32 x, i32 y)
{
    if (surfaceStorage->surfacesBis[surfaceIndex] == NULL)
    {
        return;
    }

    IDirect3DSurface8 *destSurface;
    if (g_Supervisor.d3dDevice->GetBackBuffer(
            0, D3DBACKBUFFER_TYPE_MONO, &destSurface) != D3D_OK)
    {
        return;
    }

    if (this->surfaces[surfaceIndex] == NULL)
    {
        if (g_Supervisor.d3dDevice->CreateRenderTarget(
                surfaceStorage->surfaceInfo[surfaceIndex].Width,
                surfaceStorage->surfaceInfo[surfaceIndex].Height,
                g_Supervisor.presentParameters.BackBufferFormat,
                D3DMULTISAMPLE_NONE, TRUE,
                &this->surfaces[surfaceIndex]) != D3D_OK)
        {
            if (g_Supervisor.d3dDevice->CreateImageSurface(
                    surfaceStorage->surfaceInfo[surfaceIndex].Width,
                    surfaceStorage->surfaceInfo[surfaceIndex].Height,
                    g_Supervisor.presentParameters.BackBufferFormat,
                    &this->surfaces[surfaceIndex]) != D3D_OK)
            {
                destSurface->Release();
                return;
            }
        }

        if (D3DXLoadSurfaceFromSurface(
                this->surfaces[surfaceIndex], NULL, NULL,
                surfaceStorage->surfacesBis[surfaceIndex], NULL, NULL,
                D3DX_FILTER_NONE, 0) != D3D_OK)
        {
            destSurface->Release();
            return;
        }
    }

    RECT sourceRect;
    POINT destPoint;
    sourceRect.left = left;
    sourceRect.top = top;
    sourceRect.right = surfaceStorage->surfaceInfo[surfaceIndex].Width;
    sourceRect.bottom = surfaceStorage->surfaceInfo[surfaceIndex].Height;
    destPoint.x = x;
    destPoint.y = y;
    g_Supervisor.d3dDevice->CopyRects(
        this->surfaces[surfaceIndex], &sourceRect, 1, destSurface, &destPoint);
    destSurface->Release();
}

#undef surfaceStorage

} // namespace th095
