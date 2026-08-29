#include "SceneSelect.hpp"
#include "Supervisor.hpp"

#include <string.h>

namespace th095
{

struct AnmTextureHeaderView
{
    char magic[4];
    u16 unknown004;
    i16 format;
    i16 width;
    i16 height;
    u16 unknown00c;
    u16 unknown00e;
};

typedef char AnmTextureHeaderViewSizeIs10[
    (sizeof(AnmTextureHeaderView) == 0x10) ? 1 : -1];

struct SceneTextureLoadLocals
{
    u8 *rawEntry;
    AnmTextureHeaderView *header;
    RECT sourceRect;
    IDirect3DSurface8 *surface;
};

typedef char SceneTextureLoadLocalsSizeIs1C[
    (sizeof(SceneTextureLoadLocals) == 0x1c) ? 1 : -1];

extern D3DFORMAT g_TextureFormatD3D8Mapping[6];
extern u32 g_TextureFormatBytesPerPixel[6];

void __fastcall AccumulateArgb8888Neighbor(u32 *sums, u8 *pixel,
                                           u32 *count)
{
    if (pixel[3] != 0)
    {
        sums[0] += pixel[2];
        sums[1] += pixel[1];
        sums[2] += pixel[0];
        *count = *count + 1;
    }
}

void __fastcall AccumulateArgb1555Neighbor(u32 *sums, PixelArgb1555 *pixel,
                                           u32 *count)
{
    if (pixel->alpha != 0)
    {
        sums[0] += pixel->red;
        sums[1] += pixel->green;
        sums[2] += pixel->blue;
        *count = *count + 1;
    }
}

void __fastcall AccumulateArgb4444Neighbor(u32 *sums, PixelArgb4444 *pixel,
                                           u32 *count)
{
    if (pixel->alpha != 0)
    {
        sums[0] += pixel->red;
        sums[1] += pixel->green;
        sums[2] += pixel->blue;
        *count = *count + 1;
    }
}

i32 __fastcall GetAnmFormat(i32 format)
{
    if ((*reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(&g_Supervisor) +
                                  0x1e0) &
         1) != 0)
    {
        if (g_TextureFormatD3D8Mapping[format] == D3DFMT_A8R8G8B8 ||
            g_TextureFormatD3D8Mapping[format] == D3DFMT_UNKNOWN)
        {
            format = 5;
        }
        else if (g_TextureFormatD3D8Mapping[format] == D3DFMT_R8G8B8)
        {
            format = 3;
        }
    }
    return format;
}

void SceneAnmManagerView::ApplyTextureAlphaBleed(
    SceneTextureEntryView *entry)
{
    IDirect3DSurface8 *surface = NULL;
    D3DLOCKED_RECT lockedRect;
    D3DSURFACE_DESC description;

    entry->texture->GetSurfaceLevel(0, &surface);
    surface->GetDesc(&description);
    surface->LockRect(&lockedRect, NULL, 0);

    switch (description.Format)
    {
    case D3DFMT_UNKNOWN:
    case D3DFMT_A8R8G8B8:
        for (u32 y = 0; y < description.Height; y++)
        {
            u32 *pixel = reinterpret_cast<u32 *>(
                reinterpret_cast<u8 *>(lockedRect.pBits) +
                lockedRect.Pitch * y);
            for (u32 x = 0; x < description.Width; x++)
            {
                if (reinterpret_cast<u8 *>(pixel)[3] == 0)
                {
                    u32 sums[3];
                    sums[2] = 0;
                    sums[1] = sums[2];
                    sums[0] = sums[1];
                    u32 neighborCount = 0;
                    if (x > 0)
                    {
                        AccumulateArgb8888Neighbor(
                            sums, reinterpret_cast<u8 *>(pixel - 1),
                            &neighborCount);
                    }
                    if (x < description.Width - 1)
                    {
                        AccumulateArgb8888Neighbor(
                            sums, reinterpret_cast<u8 *>(pixel + 1),
                            &neighborCount);
                    }
                    if (y > 0)
                    {
                        AccumulateArgb8888Neighbor(
                            sums,
                            reinterpret_cast<u8 *>(
                                pixel - lockedRect.Pitch / 4),
                            &neighborCount);
                    }
                    if (y < description.Height - 1)
                    {
                        AccumulateArgb8888Neighbor(
                            sums,
                            reinterpret_cast<u8 *>(
                                pixel + lockedRect.Pitch / 4),
                            &neighborCount);
                    }
                    if (neighborCount > 1)
                    {
                        sums[0] /= neighborCount;
                        sums[1] /= neighborCount;
                        sums[2] /= neighborCount;
                    }
                    reinterpret_cast<u8 *>(pixel)[2] = (u8)sums[0];
                    reinterpret_cast<u8 *>(pixel)[1] = (u8)sums[1];
                    reinterpret_cast<u8 *>(pixel)[0] = (u8)sums[2];
                }
                pixel++;
            }
        }
        break;

    case D3DFMT_A1R5G5B5:
        for (u32 y = 0; y < description.Height; y++)
        {
            PixelArgb1555 *pixel = reinterpret_cast<PixelArgb1555 *>(
                reinterpret_cast<u8 *>(lockedRect.pBits) +
                lockedRect.Pitch * y);
            for (u32 x = 0; x < description.Width; x++)
            {
                if (pixel->alpha == 0)
                {
                    u32 sums[3];
                    sums[2] = 0;
                    sums[1] = sums[2];
                    sums[0] = sums[1];
                    u32 neighborCount = 0;
                    if (x > 0)
                    {
                        AccumulateArgb1555Neighbor(sums, pixel - 1,
                                                   &neighborCount);
                    }
                    if (x < description.Width - 1)
                    {
                        AccumulateArgb1555Neighbor(sums, pixel + 1,
                                                   &neighborCount);
                    }
                    if (y > 0)
                    {
                        AccumulateArgb1555Neighbor(
                            sums, pixel - lockedRect.Pitch / 2,
                            &neighborCount);
                    }
                    if (y < description.Height - 1)
                    {
                        AccumulateArgb1555Neighbor(
                            sums, pixel + lockedRect.Pitch / 2,
                            &neighborCount);
                    }
                    if (neighborCount > 1)
                    {
                        sums[0] /= neighborCount;
                        sums[1] /= neighborCount;
                        sums[2] /= neighborCount;
                    }
                    pixel->red = (u16)sums[0];
                    pixel->green = (u16)sums[1];
                    pixel->blue = (u16)sums[2];
                }
                pixel++;
            }
        }
        break;

    case D3DFMT_A4R4G4B4:
        for (u32 y = 0; y < description.Height; y++)
        {
            PixelArgb4444 *pixel = reinterpret_cast<PixelArgb4444 *>(
                reinterpret_cast<u8 *>(lockedRect.pBits) +
                lockedRect.Pitch * y);
            for (u32 x = 0; x < description.Width; x++)
            {
                if (pixel->alpha == 0)
                {
                    u32 sums[3];
                    sums[2] = 0;
                    sums[1] = sums[2];
                    sums[0] = sums[1];
                    u32 neighborCount = 0;
                    if (x > 0)
                    {
                        AccumulateArgb4444Neighbor(sums, pixel - 1,
                                                   &neighborCount);
                    }
                    if (x < description.Width - 1)
                    {
                        AccumulateArgb4444Neighbor(sums, pixel + 1,
                                                   &neighborCount);
                    }
                    if (y > 0)
                    {
                        AccumulateArgb4444Neighbor(
                            sums, pixel - lockedRect.Pitch / 2,
                            &neighborCount);
                    }
                    if (y < description.Height - 1)
                    {
                        AccumulateArgb4444Neighbor(
                            sums, pixel + lockedRect.Pitch / 2,
                            &neighborCount);
                    }
                    if (neighborCount > 1)
                    {
                        sums[0] /= neighborCount;
                        sums[1] /= neighborCount;
                        sums[2] /= neighborCount;
                    }
                    pixel->red = (u16)sums[0];
                    pixel->green = (u16)sums[1];
                    pixel->blue = (u16)sums[2];
                }
                pixel++;
            }
        }
        break;
    }

    surface->UnlockRect();
    surface->Release();
}

i32 SceneAnmManagerView::LoadTexture(SceneTextureEntryView *entry, u8 *data,
                                     i32 size, i32 format, i32, i32 hasData)
{
    SceneTextureLoadLocals locals;

    format = GetAnmFormat(format);
    entry->rawDataSize = size;

    locals.surface = NULL;
    entry->texture->GetSurfaceLevel(0, &locals.surface);
    if (hasData == 0)
    {
        D3DXLoadSurfaceFromFileInMemory(locals.surface, NULL, NULL, data,
                                        size, NULL, D3DX_FILTER_NONE, 0, NULL);
    }
    else
    {
        locals.rawEntry = data;
        locals.header = reinterpret_cast<AnmTextureHeaderView *>(
            locals.rawEntry + reinterpret_cast<u32 *>(locals.rawEntry)[12]);
        locals.sourceRect.left = 0;
        locals.sourceRect.top = 0;
        locals.sourceRect.right = locals.header->width;
        locals.sourceRect.bottom = locals.header->height;
        D3DXLoadSurfaceFromMemory(
            locals.surface, NULL, NULL,
            locals.rawEntry + reinterpret_cast<u32 *>(locals.rawEntry)[12] +
                sizeof(AnmTextureHeaderView),
            g_TextureFormatD3D8Mapping[locals.header->format],
            g_TextureFormatBytesPerPixel[locals.header->format] *
                locals.header->width,
            NULL, &locals.sourceRect, D3DX_FILTER_NONE, 0);
    }
    locals.surface->Release();
    this->ApplyTextureAlphaBleed(entry);
    entry->bytesPerPixel = g_TextureFormatBytesPerPixel[format];
    return 0;
}

i32 SceneAnmManagerView::LoadTextureRegion(SceneTextureEntryView *entry,
                                           u8 *data, i32 size, i32 format,
                                           i32, i32 hasData, i32 top)
{
    IDirect3DSurface8 *surface;
    RECT fileDestinationRect;
    D3DSURFACE_DESC description;
    RECT sourceRect;
    RECT dataDestinationRect;
    AnmTextureHeaderView *header;
    u8 *rawEntry;

    format = GetAnmFormat(format);
    entry->rawDataSize = size;

    surface = NULL;
    entry->texture->GetSurfaceLevel(0, &surface);
    if (hasData == 0)
    {
        surface->GetDesc(&description);
        fileDestinationRect.left = 0;
        fileDestinationRect.top = top;
        fileDestinationRect.right = description.Width;
        fileDestinationRect.bottom = description.Height;
        D3DXLoadSurfaceFromFileInMemory(
            surface, NULL, &fileDestinationRect, data, size, NULL,
            D3DX_FILTER_NONE, 0, NULL);
    }
    else
    {
        rawEntry = data;
        header = reinterpret_cast<AnmTextureHeaderView *>(
            rawEntry + reinterpret_cast<u32 *>(rawEntry)[12]);
        sourceRect.left = 0;
        sourceRect.top = 0;
        sourceRect.right = header->width;
        sourceRect.bottom = header->height;
        dataDestinationRect.left = 0;
        dataDestinationRect.top = top;
        dataDestinationRect.right = header->width;
        dataDestinationRect.bottom = header->height + top;
        D3DXLoadSurfaceFromMemory(
            surface, NULL, &dataDestinationRect,
            rawEntry + reinterpret_cast<u32 *>(rawEntry)[12] +
                sizeof(AnmTextureHeaderView),
            g_TextureFormatD3D8Mapping[header->format],
            g_TextureFormatBytesPerPixel[header->format] * header->width,
            NULL, &sourceRect, D3DX_FILTER_NONE, 0);
    }
    surface->Release();
    this->ApplyTextureAlphaBleed(entry);
    entry->bytesPerPixel = g_TextureFormatBytesPerPixel[format];
    return 0;
}

struct SceneTextureClearLocals
{
    D3DSURFACE_DESC description;
    D3DLOCKED_RECT lockedRect;
    IDirect3DSurface8 *surface;
};

typedef char SceneTextureClearLocalsSizeIs2C[
    (sizeof(SceneTextureClearLocals) == 0x2c) ? 1 : -1];

void SceneTextureEntryView::Clear()
{
    SceneTextureClearLocals locals;

    this->texture->GetSurfaceLevel(0, &locals.surface);
    locals.surface->GetDesc(&locals.description);
    locals.surface->LockRect(&locals.lockedRect, NULL, 0);
    memset(locals.lockedRect.pBits, 0,
           locals.lockedRect.Pitch * locals.description.Height);
    locals.surface->UnlockRect();
    if (locals.surface != NULL)
    {
        locals.surface->Release();
        locals.surface = NULL;
    }
}

} // namespace th095
