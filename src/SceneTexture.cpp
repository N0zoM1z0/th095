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

struct SceneAlphaArgb8888Locals
{
    u32 sums[3];
    u32 *pixel;
    u32 neighborCount;
    u32 x;
    u32 y;
};

struct SceneAlphaArgb1555Locals
{
    u32 sums[3];
    PixelArgb1555 *pixel;
    u32 neighborCount;
    u32 x;
    u32 y;
};

struct SceneAlphaArgb4444Locals
{
    u32 sums[3];
    PixelArgb4444 *pixel;
    u32 neighborCount;
    u32 x;
    u32 y;
};

struct SceneTextureAlphaBleedLocals
{
    SceneAlphaArgb4444Locals argb4444;
    SceneAlphaArgb1555Locals argb1555;
    SceneAlphaArgb8888Locals argb8888;
    D3DSURFACE_DESC description;
    D3DLOCKED_RECT lockedRect;
    IDirect3DSurface8 *surface;
};

typedef char SceneAlphaCaseSizeIs1C[
    (sizeof(SceneAlphaArgb8888Locals) == 0x1c &&
     sizeof(SceneAlphaArgb1555Locals) == 0x1c &&
     sizeof(SceneAlphaArgb4444Locals) == 0x1c) ? 1 : -1];
typedef char SceneTextureAlphaBleedLocalsSizeIs80[
    (sizeof(SceneTextureAlphaBleedLocals) == 0x80) ? 1 : -1];
typedef char SceneTextureAlphaBleedDescriptionAt54[
    (offsetof(SceneTextureAlphaBleedLocals, description) == 0x54) ? 1 : -1];
typedef char SceneTextureAlphaBleedLockedAt74[
    (offsetof(SceneTextureAlphaBleedLocals, lockedRect) == 0x74) ? 1 : -1];
typedef char SceneTextureAlphaBleedSurfaceAt7C[
    (offsetof(SceneTextureAlphaBleedLocals, surface) == 0x7c) ? 1 : -1];

void SceneAnmManagerView::ApplyTextureAlphaBleed(
    SceneTextureEntryView *entry)
{
    SceneTextureAlphaBleedLocals locals;

    locals.surface = NULL;
    entry->texture->GetSurfaceLevel(0, &locals.surface);
    locals.surface->GetDesc(&locals.description);
    locals.surface->LockRect(&locals.lockedRect, NULL, 0);

    switch (locals.description.Format)
    {
    case D3DFMT_UNKNOWN:
    case D3DFMT_A8R8G8B8:
        for (locals.argb8888.y = 0;
             locals.argb8888.y < locals.description.Height;
             locals.argb8888.y++)
        {
            locals.argb8888.pixel = reinterpret_cast<u32 *>(
                reinterpret_cast<u8 *>(locals.lockedRect.pBits) +
                locals.lockedRect.Pitch * locals.argb8888.y);
            for (locals.argb8888.x = 0;
                 locals.argb8888.x < locals.description.Width;
                 locals.argb8888.x++)
            {
                if (reinterpret_cast<u8 *>(locals.argb8888.pixel)[3] == 0)
                {
                    locals.argb8888.sums[2] = 0;
                    locals.argb8888.sums[1] = locals.argb8888.sums[2];
                    locals.argb8888.sums[0] = locals.argb8888.sums[1];
                    locals.argb8888.neighborCount = 0;
                    if (locals.argb8888.x > 0)
                        AccumulateArgb8888Neighbor(
                            locals.argb8888.sums,
                            reinterpret_cast<u8 *>(locals.argb8888.pixel - 1),
                            &locals.argb8888.neighborCount);
                    if (locals.argb8888.x < locals.description.Width - 1)
                        AccumulateArgb8888Neighbor(
                            locals.argb8888.sums,
                            reinterpret_cast<u8 *>(locals.argb8888.pixel + 1),
                            &locals.argb8888.neighborCount);
                    if (locals.argb8888.y > 0)
                        AccumulateArgb8888Neighbor(
                            locals.argb8888.sums,
                            reinterpret_cast<u8 *>(
                                locals.argb8888.pixel +
                                (-locals.lockedRect.Pitch / 4)),
                            &locals.argb8888.neighborCount);
                    if (locals.argb8888.y < locals.description.Height - 1)
                        AccumulateArgb8888Neighbor(
                            locals.argb8888.sums,
                            reinterpret_cast<u8 *>(
                                locals.argb8888.pixel +
                                locals.lockedRect.Pitch / 4),
                            &locals.argb8888.neighborCount);
                    if (locals.argb8888.neighborCount > 1)
                    {
                        locals.argb8888.sums[0] /= locals.argb8888.neighborCount;
                        locals.argb8888.sums[1] /= locals.argb8888.neighborCount;
                        locals.argb8888.sums[2] /= locals.argb8888.neighborCount;
                    }
                    reinterpret_cast<u8 *>(locals.argb8888.pixel)[2] =
                        (u8)locals.argb8888.sums[0];
                    reinterpret_cast<u8 *>(locals.argb8888.pixel)[1] =
                        (u8)locals.argb8888.sums[1];
                    reinterpret_cast<u8 *>(locals.argb8888.pixel)[0] =
                        (u8)locals.argb8888.sums[2];
                }
                locals.argb8888.pixel++;
            }
        }
        break;

    case D3DFMT_A1R5G5B5:
        for (locals.argb1555.y = 0;
             locals.argb1555.y < locals.description.Height;
             locals.argb1555.y++)
        {
            locals.argb1555.pixel = reinterpret_cast<PixelArgb1555 *>(
                reinterpret_cast<u8 *>(locals.lockedRect.pBits) +
                locals.lockedRect.Pitch * locals.argb1555.y);
            for (locals.argb1555.x = 0;
                 locals.argb1555.x < locals.description.Width;
                 locals.argb1555.x++)
            {
                if (locals.argb1555.pixel->alpha == 0)
                {
                    locals.argb1555.sums[2] = 0;
                    locals.argb1555.sums[1] = locals.argb1555.sums[2];
                    locals.argb1555.sums[0] = locals.argb1555.sums[1];
                    locals.argb1555.neighborCount = 0;
                    if (locals.argb1555.x > 0)
                        AccumulateArgb1555Neighbor(
                            locals.argb1555.sums, locals.argb1555.pixel - 1,
                            &locals.argb1555.neighborCount);
                    if (locals.argb1555.x < locals.description.Width - 1)
                        AccumulateArgb1555Neighbor(
                            locals.argb1555.sums, locals.argb1555.pixel + 1,
                            &locals.argb1555.neighborCount);
                    if (locals.argb1555.y > 0)
                        AccumulateArgb1555Neighbor(
                            locals.argb1555.sums,
                            locals.argb1555.pixel +
                                (-locals.lockedRect.Pitch / 2),
                            &locals.argb1555.neighborCount);
                    if (locals.argb1555.y < locals.description.Height - 1)
                        AccumulateArgb1555Neighbor(
                            locals.argb1555.sums,
                            locals.argb1555.pixel + locals.lockedRect.Pitch / 2,
                            &locals.argb1555.neighborCount);
                    if (locals.argb1555.neighborCount > 1)
                    {
                        locals.argb1555.sums[0] /= locals.argb1555.neighborCount;
                        locals.argb1555.sums[1] /= locals.argb1555.neighborCount;
                        locals.argb1555.sums[2] /= locals.argb1555.neighborCount;
                    }
                    locals.argb1555.pixel->red = (u8)locals.argb1555.sums[0];
                    locals.argb1555.pixel->green = (u8)locals.argb1555.sums[1];
                    locals.argb1555.pixel->blue = (u8)locals.argb1555.sums[2];
                }
                locals.argb1555.pixel++;
            }
        }
        break;

    case D3DFMT_A4R4G4B4:
        for (locals.argb4444.y = 0;
             locals.argb4444.y < locals.description.Height;
             locals.argb4444.y++)
        {
            locals.argb4444.pixel = reinterpret_cast<PixelArgb4444 *>(
                reinterpret_cast<u8 *>(locals.lockedRect.pBits) +
                locals.lockedRect.Pitch * locals.argb4444.y);
            for (locals.argb4444.x = 0;
                 locals.argb4444.x < locals.description.Width;
                 locals.argb4444.x++)
            {
                if (locals.argb4444.pixel->alpha == 0)
                {
                    locals.argb4444.sums[2] = 0;
                    locals.argb4444.sums[1] = locals.argb4444.sums[2];
                    locals.argb4444.sums[0] = locals.argb4444.sums[1];
                    locals.argb4444.neighborCount = 0;
                    if (locals.argb4444.x > 0)
                        AccumulateArgb4444Neighbor(
                            locals.argb4444.sums, locals.argb4444.pixel - 1,
                            &locals.argb4444.neighborCount);
                    if (locals.argb4444.x < locals.description.Width - 1)
                        AccumulateArgb4444Neighbor(
                            locals.argb4444.sums, locals.argb4444.pixel + 1,
                            &locals.argb4444.neighborCount);
                    if (locals.argb4444.y > 0)
                        AccumulateArgb4444Neighbor(
                            locals.argb4444.sums,
                            locals.argb4444.pixel +
                                (-locals.lockedRect.Pitch / 2),
                            &locals.argb4444.neighborCount);
                    if (locals.argb4444.y < locals.description.Height - 1)
                        AccumulateArgb4444Neighbor(
                            locals.argb4444.sums,
                            locals.argb4444.pixel + locals.lockedRect.Pitch / 2,
                            &locals.argb4444.neighborCount);
                    if (locals.argb4444.neighborCount > 1)
                    {
                        locals.argb4444.sums[0] /= locals.argb4444.neighborCount;
                        locals.argb4444.sums[1] /= locals.argb4444.neighborCount;
                        locals.argb4444.sums[2] /= locals.argb4444.neighborCount;
                    }
                    locals.argb4444.pixel->red = (u8)locals.argb4444.sums[0];
                    locals.argb4444.pixel->green = (u8)locals.argb4444.sums[1];
                    locals.argb4444.pixel->blue = (u8)locals.argb4444.sums[2];
                }
                locals.argb4444.pixel++;
            }
        }
        break;
    }

    locals.surface->UnlockRect();
    locals.surface->Release();
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

#define regionSurface restartCommandProcessingLocal05
#define regionFileDestinationRect averagedPanLocal12
#define regionDescription iLocal11
#define regionSourceRect commandCursorLocal02
#define regionDataDestinationRect soundIndexLocal01
#define regionHeader jLocal00
#define regionRawEntry preloadBufferLocal03
#pragma var_order(regionSurface, regionFileDestinationRect, regionDescription, \
                  regionSourceRect, regionDataDestinationRect, regionHeader, \
                  regionRawEntry, this)
i32 SceneAnmManagerView::LoadTextureRegion(SceneTextureEntryView *entry,
                                           u8 *data, i32 size, i32 format,
                                           i32, i32 hasData, i32 top)
{
    IDirect3DSurface8 *regionSurface;
    RECT regionFileDestinationRect;
    D3DSURFACE_DESC regionDescription;
    RECT regionSourceRect;
    RECT regionDataDestinationRect;
    AnmTextureHeaderView *regionHeader;
    u8 *regionRawEntry;

    format = GetAnmFormat(format);
    entry->rawDataSize = size;

    regionSurface = NULL;
    entry->texture->GetSurfaceLevel(0, &regionSurface);
    if (hasData == 0)
    {
        regionSurface->GetDesc(&regionDescription);
        regionFileDestinationRect.left = 0;
        regionFileDestinationRect.top = top;
        regionFileDestinationRect.right = regionDescription.Width;
        regionFileDestinationRect.bottom = regionDescription.Height;
        D3DXLoadSurfaceFromFileInMemory(
            regionSurface, NULL, &regionFileDestinationRect, data, size, NULL,
            D3DX_FILTER_NONE, 0, NULL);
    }
    else
    {
        regionRawEntry = data;
        regionHeader = reinterpret_cast<AnmTextureHeaderView *>(
            regionRawEntry + reinterpret_cast<u32 *>(regionRawEntry)[12]);
        regionSourceRect.left = 0;
        regionSourceRect.top = 0;
        regionSourceRect.right = regionHeader->width;
        regionSourceRect.bottom = regionHeader->height;
        regionDataDestinationRect.left = 0;
        regionDataDestinationRect.top = top;
        regionDataDestinationRect.right = regionHeader->width;
        regionDataDestinationRect.bottom = regionHeader->height + top;
        D3DXLoadSurfaceFromMemory(
            regionSurface, NULL, &regionDataDestinationRect,
            regionRawEntry + reinterpret_cast<u32 *>(regionRawEntry)[12] +
                sizeof(AnmTextureHeaderView),
            g_TextureFormatD3D8Mapping[regionHeader->format],
            g_TextureFormatBytesPerPixel[regionHeader->format] *
                regionHeader->width,
            NULL, &regionSourceRect, D3DX_FILTER_NONE, 0);
    }
    regionSurface->Release();
    this->ApplyTextureAlphaBleed(entry);
    entry->bytesPerPixel = g_TextureFormatBytesPerPixel[format];
    return 0;
}
#undef regionSurface
#undef regionFileDestinationRect
#undef regionDescription
#undef regionSourceRect
#undef regionDataDestinationRect
#undef regionHeader
#undef regionRawEntry


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
