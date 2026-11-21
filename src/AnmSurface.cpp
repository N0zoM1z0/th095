#include "AnmManager.hpp"

#include <stdlib.h>

namespace th095
{

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

    if (this->deferredSurfaceData[surfaceIndex] == NULL)
    {
        locals.fileData = FileSystem::OpenFile(path, &locals.fileSize, 0);
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
        locals.fileData = this->deferredSurfaceData[surfaceIndex];
        locals.fileSize = this->deferredSurfaceDataSizes[surfaceIndex];
        this->deferredSurfaceData[surfaceIndex] = NULL;
    }

    if (g_Supervisor.d3dDevice->CreateImageSurface(
            640, 1024, g_Supervisor.presentParameters.BackBufferFormat,
            &locals.surface) != D3D_OK)
    {
        return ZUN_ERROR;
    }

    if (D3DXLoadSurfaceFromFileInMemory(
            locals.surface, NULL, NULL, locals.fileData, locals.fileSize, NULL, 1, 0,
            &this->surfaceInfo[surfaceIndex]) != D3D_OK)
    {
        goto error;
    }

    if (g_Supervisor.d3dDevice->CreateRenderTarget(
            this->surfaceInfo[surfaceIndex].Width,
            this->surfaceInfo[surfaceIndex].Height,
            g_Supervisor.presentParameters.BackBufferFormat,
            D3DMULTISAMPLE_NONE, 1, &this->surfaces[surfaceIndex]) != D3D_OK)
    {
        if (g_Supervisor.d3dDevice->CreateImageSurface(
                this->surfaceInfo[surfaceIndex].Width,
                this->surfaceInfo[surfaceIndex].Height,
                g_Supervisor.presentParameters.BackBufferFormat,
                &this->surfaces[surfaceIndex]) != D3D_OK)
        {
            goto error;
        }
    }

    if (g_Supervisor.d3dDevice->CreateImageSurface(
            this->surfaceInfo[surfaceIndex].Width,
            this->surfaceInfo[surfaceIndex].Height,
            g_Supervisor.presentParameters.BackBufferFormat,
            &this->surfaceRestoreCopies[surfaceIndex]) != D3D_OK)
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
            this->surfaceRestoreCopies[surfaceIndex], NULL, NULL, locals.surface, NULL, NULL,
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
    if (this->surfaceRestoreCopies[surfaceIndex] != NULL)
    {
        this->surfaceRestoreCopies[surfaceIndex]->Release();
        this->surfaceRestoreCopies[surfaceIndex] = NULL;
    }
    if (this->deferredSurfaceData[surfaceIndex] != NULL)
    {
        data = this->deferredSurfaceData[surfaceIndex];
        free(data);
    }
    this->deferredSurfaceData[surfaceIndex] = NULL;
}

// FUNCTION: TH095 0x00444460.
void AnmManager::CopySurfaceToBackbuffer(
    i32 surfaceIndex, i32 left, i32 top, i32 x, i32 y)
{
    if (this->surfaceRestoreCopies[surfaceIndex] == NULL)
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
                this->surfaceInfo[surfaceIndex].Width,
                this->surfaceInfo[surfaceIndex].Height,
                g_Supervisor.presentParameters.BackBufferFormat,
                D3DMULTISAMPLE_NONE, TRUE,
                &this->surfaces[surfaceIndex]) != D3D_OK)
        {
            if (g_Supervisor.d3dDevice->CreateImageSurface(
                    this->surfaceInfo[surfaceIndex].Width,
                    this->surfaceInfo[surfaceIndex].Height,
                    g_Supervisor.presentParameters.BackBufferFormat,
                    &this->surfaces[surfaceIndex]) != D3D_OK)
            {
                destSurface->Release();
                return;
            }
        }

        if (D3DXLoadSurfaceFromSurface(
                this->surfaces[surfaceIndex], NULL, NULL,
                this->surfaceRestoreCopies[surfaceIndex], NULL, NULL,
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
    sourceRect.right = this->surfaceInfo[surfaceIndex].Width;
    sourceRect.bottom = this->surfaceInfo[surfaceIndex].Height;
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

    this->surfaceInfo[surfaceIndex].Width = dstW;
    this->surfaceInfo[surfaceIndex].Height = dstH;

    if (g_Supervisor.d3dDevice->CreateRenderTarget(
            this->surfaceInfo[surfaceIndex].Width,
            this->surfaceInfo[surfaceIndex].Height,
            g_Supervisor.presentParameters.BackBufferFormat,
            D3DMULTISAMPLE_NONE, TRUE,
            &this->surfaces[surfaceIndex]) != D3D_OK)
    {
        if (g_Supervisor.d3dDevice->CreateImageSurface(
                this->surfaceInfo[surfaceIndex].Width,
                this->surfaceInfo[surfaceIndex].Height,
                g_Supervisor.presentParameters.BackBufferFormat,
                &this->surfaces[surfaceIndex]) != D3D_OK)
        {
            goto out;
        }
    }

    if (g_Supervisor.d3dDevice->CreateImageSurface(
            this->surfaceInfo[surfaceIndex].Width,
            this->surfaceInfo[surfaceIndex].Height,
            g_Supervisor.presentParameters.BackBufferFormat,
            &this->surfaceRestoreCopies[surfaceIndex]) != D3D_OK)
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
        this->surfaceRestoreCopies[surfaceIndex], NULL, NULL,
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

} // namespace th095
