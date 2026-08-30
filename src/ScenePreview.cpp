#include "SceneSelect.hpp"

#include <string.h>

namespace th095
{

struct ScenePreviewLocals
{
    u8 *clearRow;
    i32 clearY;
    u8 *pixel;
    RECT sourceRect;
    i32 x;
    i32 y;
    D3DLOCKED_RECT lockedRect;
    IDirect3DSurface8 *surface;
};

typedef char ScenePreviewLocalsSizeIs30[
    (sizeof(ScenePreviewLocals) == 0x30) ? 1 : -1];

ZunResult SceneSaveDataView::LoadScenePreviewTexture(
    SceneAnmLoadedView *anm, i32 textureIndex, i32 sceneIndex)
{
    ScenePreviewLocals locals;

    locals.surface = NULL;
    anm->textures[textureIndex].texture->GetSurfaceLevel(0,
                                                         &locals.surface);
    if (this->bestShotRecords[sceneIndex].componentsLoaded != 0)
    {
        locals.sourceRect.left = 0;
        locals.sourceRect.top = 0;
        locals.sourceRect.right =
            this->bestShotRecords[sceneIndex].width;
        locals.sourceRect.bottom =
            this->bestShotRecords[sceneIndex].height;
        D3DXLoadSurfaceFromMemory(
            locals.surface, NULL, &locals.sourceRect,
            g_SceneSaveData->bestShotRecords[sceneIndex].pixelData,
            this->bestShotRecords[sceneIndex].componentCount == 3
                ? D3DFMT_R8G8B8
                : D3DFMT_A4R4G4B4,
            this->bestShotRecords[sceneIndex].width *
                this->bestShotRecords[sceneIndex].componentCount,
            NULL, &locals.sourceRect, D3DX_FILTER_POINT, 0);

        locals.surface->LockRect(&locals.lockedRect, NULL, 0);
        for (locals.y = 0; locals.y < 192; locals.y++)
        {
            locals.pixel =
                reinterpret_cast<u8 *>(locals.lockedRect.pBits) +
                locals.y * locals.lockedRect.Pitch;
            if (locals.lockedRect.Pitch == 4)
            {
                for (locals.x = 0; locals.x < 256; locals.x++)
                {
                    locals.pixel[3] = 0xff;
                    locals.pixel += 4;
                }
            }
            else
            {
                for (locals.x = 0; locals.x < 256; locals.x++)
                {
                    locals.pixel[3] |= 0xf0;
                    locals.pixel += 4;
                }
            }
        }
        locals.surface->UnlockRect();
    }
    else
    {
        locals.surface->LockRect(&locals.lockedRect, NULL, 0);
        for (locals.clearY = 0; locals.clearY < 192; locals.clearY++)
        {
            locals.clearRow =
                reinterpret_cast<u8 *>(locals.lockedRect.pBits) +
                locals.clearY * locals.lockedRect.Pitch;
            memset(locals.clearRow, 0,
                   anm->textures[textureIndex].bytesPerPixel * 256);
        }
        locals.surface->UnlockRect();
    }
    locals.surface->Release();
    return ZUN_SUCCESS;
}

} // namespace th095
