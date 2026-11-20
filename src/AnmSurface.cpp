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
typedef char AnmSurfaceRestoreCopyOffset[(offsetof(AnmSurfaceStorageView, surfacesBis) == 0x125c) ? 1 : -1];
typedef char AnmSurfaceDataOffset[(offsetof(AnmSurfaceStorageView, surfaceData) == 0x12dc) ? 1 : -1];
typedef char AnmSurfaceDataSizeOffset[(offsetof(AnmSurfaceStorageView, surfaceDataSizes) == 0x135c) ? 1 : -1];
typedef char AnmSurfaceInfoOffset[(offsetof(AnmSurfaceStorageView, surfaceInfo) == 0x13dc) ? 1 : -1];

#define surfaceStorage reinterpret_cast<AnmSurfaceStorageView *>(this)
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
#define TH095_ANM_SURFACE_RESTORE_COPY(owner, index) \
    (reinterpret_cast<AnmSurfaceStorageView *>(owner)->surfacesBis[(index)])
#define TH095_ANM_SURFACE_INFO(owner, index) \
    (reinterpret_cast<AnmSurfaceStorageView *>(owner)->surfaceInfo[(index)])
#else
#define TH095_ANM_SURFACE_RESTORE_COPY(owner, index) \
    (reinterpret_cast<AnmManager *>(owner)->surfaceRestoreCopies[(index)])
#define TH095_ANM_SURFACE_INFO(owner, index) \
    (reinterpret_cast<AnmManager *>(owner)->surfaceInfo[(index)])
#endif

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
            &TH095_ANM_SURFACE_INFO(this, surfaceIndex)) != D3D_OK)
    {
        goto error;
    }

    if (g_Supervisor.d3dDevice->CreateRenderTarget(
            TH095_ANM_SURFACE_INFO(this, surfaceIndex).Width,
            TH095_ANM_SURFACE_INFO(this, surfaceIndex).Height,
            g_Supervisor.presentParameters.BackBufferFormat,
            D3DMULTISAMPLE_NONE, 1, &this->surfaces[surfaceIndex]) != D3D_OK)
    {
        if (g_Supervisor.d3dDevice->CreateImageSurface(
                TH095_ANM_SURFACE_INFO(this, surfaceIndex).Width,
                TH095_ANM_SURFACE_INFO(this, surfaceIndex).Height,
                g_Supervisor.presentParameters.BackBufferFormat,
                &this->surfaces[surfaceIndex]) != D3D_OK)
        {
            goto error;
        }
    }

    if (g_Supervisor.d3dDevice->CreateImageSurface(
            TH095_ANM_SURFACE_INFO(this, surfaceIndex).Width,
            TH095_ANM_SURFACE_INFO(this, surfaceIndex).Height,
            g_Supervisor.presentParameters.BackBufferFormat,
            &TH095_ANM_SURFACE_RESTORE_COPY(this, surfaceIndex)) != D3D_OK)
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
            TH095_ANM_SURFACE_RESTORE_COPY(this, surfaceIndex), NULL, NULL, locals.surface, NULL, NULL,
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
    if (TH095_ANM_SURFACE_RESTORE_COPY(this, surfaceIndex) != NULL)
    {
        TH095_ANM_SURFACE_RESTORE_COPY(this, surfaceIndex)->Release();
        TH095_ANM_SURFACE_RESTORE_COPY(this, surfaceIndex) = NULL;
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
    if (TH095_ANM_SURFACE_RESTORE_COPY(this, surfaceIndex) == NULL)
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
                TH095_ANM_SURFACE_INFO(this, surfaceIndex).Width,
                TH095_ANM_SURFACE_INFO(this, surfaceIndex).Height,
                g_Supervisor.presentParameters.BackBufferFormat,
                D3DMULTISAMPLE_NONE, TRUE,
                &this->surfaces[surfaceIndex]) != D3D_OK)
        {
            if (g_Supervisor.d3dDevice->CreateImageSurface(
                    TH095_ANM_SURFACE_INFO(this, surfaceIndex).Width,
                    TH095_ANM_SURFACE_INFO(this, surfaceIndex).Height,
                    g_Supervisor.presentParameters.BackBufferFormat,
                    &this->surfaces[surfaceIndex]) != D3D_OK)
            {
                destSurface->Release();
                return;
            }
        }

        if (D3DXLoadSurfaceFromSurface(
                this->surfaces[surfaceIndex], NULL, NULL,
                TH095_ANM_SURFACE_RESTORE_COPY(this, surfaceIndex), NULL, NULL,
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
    sourceRect.right = TH095_ANM_SURFACE_INFO(this, surfaceIndex).Width;
    sourceRect.bottom = TH095_ANM_SURFACE_INFO(this, surfaceIndex).Height;
    destPoint.x = x;
    destPoint.y = y;
    g_Supervisor.d3dDevice->CopyRects(
        this->surfaces[surfaceIndex], &sourceRect, 1, destSurface, &destPoint);
    destSurface->Release();
}

// FUNCTION: TH095 0x00444620.
void AnmManager::CaptureToTexture(
    i32 anmIndex, i32 entryIndex, i32 srcX, i32 srcY, i32 srcW, i32 srcH,
    i32 dstX, i32 dstY, i32 dstW, i32 dstH)
{
    struct CaptureToTextureLocals
    {
        RECT dstRect;
        IDirect3DSurface8 *backbuffer;
        IDirect3DSurface8 *textureSurface;
        RECT srcRect;
    } locals;

    if (this->slots[anmIndex].loaded.textures[entryIndex].texture == NULL)
    {
        return;
    }

    this->FlushVertexBuffer();

    if (g_Supervisor.d3dDevice->GetBackBuffer(
            0, D3DBACKBUFFER_TYPE_MONO, &locals.backbuffer) != D3D_OK)
    {
        return;
    }

    if (this->slots[anmIndex].loaded.textures[entryIndex]
            .texture->GetSurfaceLevel(0, &locals.textureSurface) != D3D_OK)
    {
        locals.backbuffer->Release();
        return;
    }

    locals.srcRect.left = srcX;
    locals.srcRect.top = srcY;
    locals.srcRect.right = srcX + srcW;
    locals.srcRect.bottom = srcY + srcH;
    locals.dstRect.left = dstX;
    locals.dstRect.top = dstY;
    locals.dstRect.right = dstX + dstW;
    locals.dstRect.bottom = dstY + dstH;

    if (D3DXLoadSurfaceFromSurface(
            locals.textureSurface, NULL, &locals.dstRect, locals.backbuffer,
            NULL, &locals.srcRect, D3DX_FILTER_POINT, 0) != D3D_OK)
    {
        locals.textureSurface->Release();
        locals.backbuffer->Release();
        return;
    }

    locals.textureSurface->Release();
    locals.backbuffer->Release();
}

// FUNCTION: TH095 0x00444760.
void AnmManager::CaptureToSurface(
    i32 surfaceIndex, i32 srcX, i32 srcY, i32 srcW, i32 srcH, i32 dstX,
    i32 dstY, i32 dstW, i32 dstH)
{
    struct CaptureToSurfaceLocals
    {
        RECT dstRect;
        IDirect3DSurface8 *backbuffer;
        RECT srcRect;
    } locals;

    this->FlushVertexBuffer();

    if (this->surfaces[surfaceIndex] != NULL)
    {
        this->ReleaseSurface(surfaceIndex);
    }

    locals.srcRect.left = srcX;
    locals.srcRect.top = srcY;
    locals.srcRect.right = srcX + srcW;
    locals.srcRect.bottom = srcY + srcH;
    locals.dstRect.left = dstX;
    locals.dstRect.top = dstY;
    locals.dstRect.right = dstX + dstW;
    locals.dstRect.bottom = dstY + dstH;

    if (g_Supervisor.d3dDevice->GetBackBuffer(
            0, D3DBACKBUFFER_TYPE_MONO, &locals.backbuffer) != D3D_OK)
    {
        return;
    }

    TH095_ANM_SURFACE_INFO(this, surfaceIndex).Width = dstW;
    TH095_ANM_SURFACE_INFO(this, surfaceIndex).Height = dstH;

    if (g_Supervisor.d3dDevice->CreateRenderTarget(
            TH095_ANM_SURFACE_INFO(this, surfaceIndex).Width,
            TH095_ANM_SURFACE_INFO(this, surfaceIndex).Height,
            g_Supervisor.presentParameters.BackBufferFormat,
            D3DMULTISAMPLE_NONE, TRUE,
            &this->surfaces[surfaceIndex]) != D3D_OK)
    {
        if (g_Supervisor.d3dDevice->CreateImageSurface(
                TH095_ANM_SURFACE_INFO(this, surfaceIndex).Width,
                TH095_ANM_SURFACE_INFO(this, surfaceIndex).Height,
                g_Supervisor.presentParameters.BackBufferFormat,
                &this->surfaces[surfaceIndex]) != D3D_OK)
        {
            goto out;
        }
    }

    if (g_Supervisor.d3dDevice->CreateImageSurface(
            TH095_ANM_SURFACE_INFO(this, surfaceIndex).Width,
            TH095_ANM_SURFACE_INFO(this, surfaceIndex).Height,
            g_Supervisor.presentParameters.BackBufferFormat,
            &TH095_ANM_SURFACE_RESTORE_COPY(this, surfaceIndex)) != D3D_OK)
    {
        goto out;
    }

    if (D3DXLoadSurfaceFromSurface(
            this->surfaces[surfaceIndex], NULL,
            &locals.dstRect, locals.backbuffer, NULL, &locals.srcRect,
            D3DX_DEFAULT, 0) != D3D_OK)
    {
        goto out;
    }

    D3DXLoadSurfaceFromSurface(
        TH095_ANM_SURFACE_RESTORE_COPY(this, surfaceIndex), NULL, NULL,
        this->surfaces[surfaceIndex], NULL,
        NULL, D3DX_DEFAULT, 0);

out:
    if (locals.backbuffer != NULL)
    {
        locals.backbuffer->Release();
        locals.backbuffer = NULL;
    }
}

// FUNCTION: TH095 0x00421D00.
void AnmManager::TakeScreenshots()
{
    if (this->captureAnmIdx >= 0)
    {
        this->CaptureToTexture(
            this->captureAnmIdx, this->textureCaptureEntryIndex,
            this->captureSourceX,
            this->captureSourceY,
            this->captureSourceWidth,
            this->captureSourceHeight,
            this->captureDestinationX,
            this->captureDestinationY,
            this->captureDestinationWidth,
            this->captureDestinationHeight);
        this->captureAnmIdx = -1;
    }

    if (this->captureSurfaceIdx >= 0)
    {
        this->CaptureToSurface(
            this->captureSurfaceIdx, this->surfaceCaptureSourceX,
            this->surfaceCaptureSourceY,
            this->surfaceCaptureSourceWidth,
            this->surfaceCaptureSourceHeight,
            this->surfaceCaptureDestinationX,
            this->surfaceCaptureDestinationY,
            this->surfaceCaptureDestinationWidth,
            this->surfaceCaptureDestinationHeight);
        this->captureSurfaceIdx = -1;
    }
}

#undef TH095_ANM_SURFACE_INFO
#undef TH095_ANM_SURFACE_RESTORE_COPY
#undef surfaceStorage

} // namespace th095
