#include "TextRenderer.hpp"

#include <string.h>

namespace th095
{

extern HFONT g_TextFont19;
extern HFONT g_TextFont20;
extern HFONT g_TextFont17;
extern HFONT g_TextFont18;
extern TextRenderBufferView g_TextRenderBuffer;

bool TextRenderBufferView::InvertAlpha(i32 rowCount, BOOL unused)
{
    struct
    {
        i32 imageWidthInBytes;
        PixelArgb1555 *bufferCursor;
        i32 regionByteCount;
        i32 byteOffset;
        u8 *bufferRegion;
    } locals;

    (void)unused;
    locals.imageWidthInBytes = this->imageWidthInBytes;
    locals.regionByteCount = locals.imageWidthInBytes * rowCount;
    locals.bufferRegion = this->buffer;

    switch (this->format)
    {
    case D3DFMT_A8R8G8B8:
        for (locals.byteOffset = 3;
             locals.byteOffset < locals.regionByteCount;
             locals.byteOffset += 4)
        {
            locals.bufferRegion[locals.byteOffset] ^= 0xff;
        }
        break;

    case D3DFMT_A1R5G5B5:
    {
        locals.bufferCursor =
            reinterpret_cast<PixelArgb1555 *>(locals.bufferRegion);
        for (locals.byteOffset = 0;
             locals.byteOffset < locals.regionByteCount;
             locals.byteOffset += 2, locals.bufferCursor++)
        {
            locals.bufferCursor->alpha ^= 1;
            if (locals.bufferCursor->alpha != 0)
            {
            }
            else
            {
                locals.bufferCursor->red = 0;
                locals.bufferCursor->green = 0;
                locals.bufferCursor->blue = 0;
            }
        }
        break;
    }

    case D3DFMT_A4R4G4B4:
        for (locals.byteOffset = 1;
             locals.byteOffset < locals.regionByteCount;
             locals.byteOffset += 2)
        {
            locals.bufferRegion[locals.byteOffset] ^= 0xf0;
        }
        break;

    default:
        return false;
    }
    return true;
}

bool TextRenderBufferView::ApplyAlphaBleed(i32 rowCount)
{
    i32 imageWidthInBytes;
    i32 regionByteCount;
    u8 *bufferRegion;
    TextRenderBufferView *self;
    bool unusedFlag;
    i32 argb8888UpperPitch;
    i32 argb8888LowerPitch;
    i32 argb4444UpperPitch;
    i32 argb4444LowerPitch;

    imageWidthInBytes = this->imageWidthInBytes;
    regionByteCount = imageWidthInBytes * rowCount;
    bufferRegion = this->buffer;
    self = this;
    unusedFlag = false;
    (void)regionByteCount;
    (void)bufferRegion;
    (void)self;
    (void)unusedFlag;

    switch (this->format)
    {
    case D3DFMT_A8R8G8B8:
    {
        u32 *pixel = reinterpret_cast<u32 *>(this->buffer);
        for (u32 y = 0; y < static_cast<u32>(rowCount); y++)
        {
            for (u32 x = 0; x < static_cast<u32>(this->width); x++)
            {
                if (reinterpret_cast<u8 *>(pixel)[3] == 0)
                {
                    u32 sums[3];
                    u32 neighborCount;

                    sums[2] = 0;
                    sums[1] = sums[2];
                    sums[0] = sums[1];
                    neighborCount = 0;
                    if (x > 0)
                    {
                        AccumulateArgb8888Neighbor(
                            sums, reinterpret_cast<u8 *>(pixel - 1),
                                                   &neighborCount);
                    }
                    if (x < static_cast<u32>(this->width - 1))
                    {
                        AccumulateArgb8888Neighbor(
                            sums, reinterpret_cast<u8 *>(pixel + 1),
                                                   &neighborCount);
                    }
                    if (y > 0)
                    {
                        argb8888UpperPitch = this->imageWidthInBytes;
                        AccumulateArgb8888Neighbor(
                            sums, reinterpret_cast<u8 *>(
                                      pixel + (-argb8888UpperPitch) / 4),
                            &neighborCount);
                    }
                    if (y < static_cast<u32>(this->height - 1))
                    {
                        argb8888LowerPitch = this->imageWidthInBytes;
                        AccumulateArgb8888Neighbor(
                            sums, reinterpret_cast<u8 *>(
                                      pixel + argb8888LowerPitch / 4),
                            &neighborCount);
                    }
                    if (neighborCount > 1)
                    {
                        sums[0] /= neighborCount;
                        sums[1] /= neighborCount;
                        sums[2] /= neighborCount;
                    }
                    reinterpret_cast<u8 *>(pixel)[2] =
                        static_cast<u8>(sums[0]);
                    reinterpret_cast<u8 *>(pixel)[1] =
                        static_cast<u8>(sums[1]);
                    reinterpret_cast<u8 *>(pixel)[0] =
                        static_cast<u8>(sums[2]);
                }
                pixel++;
            }
        }
        break;
    }

    case D3DFMT_A4R4G4B4:
    {
        PixelArgb4444 *pixel =
            reinterpret_cast<PixelArgb4444 *>(this->buffer);
        for (u32 y = 0; y < static_cast<u32>(rowCount); y++)
        {
            for (u32 x = 0; x < static_cast<u32>(this->width); x++)
            {
                if (pixel->alpha == 0)
                {
                    u32 sums[3];
                    u32 neighborCount;

                    sums[2] = 0;
                    sums[1] = sums[2];
                    sums[0] = sums[1];
                    neighborCount = 0;
                    if (x > 0)
                    {
                        AccumulateArgb4444Neighbor(sums, pixel - 1,
                                                   &neighborCount);
                    }
                    if (x < static_cast<u32>(this->width - 1))
                    {
                        AccumulateArgb4444Neighbor(sums, pixel + 1,
                                                   &neighborCount);
                    }
                    if (y > 0)
                    {
                        argb4444UpperPitch = this->imageWidthInBytes;
                        AccumulateArgb4444Neighbor(
                            sums,
                            pixel + (-argb4444UpperPitch) / 2,
                            &neighborCount);
                    }
                    if (y < static_cast<u32>(this->height - 1))
                    {
                        argb4444LowerPitch = this->imageWidthInBytes;
                        AccumulateArgb4444Neighbor(
                            sums,
                            pixel + argb4444LowerPitch / 2,
                            &neighborCount);
                    }
                    if (neighborCount > 1)
                    {
                        sums[0] /= neighborCount;
                        sums[1] /= neighborCount;
                        sums[2] /= neighborCount;
                    }
                    pixel->red = static_cast<u8>(sums[0]);
                    pixel->green = static_cast<u8>(sums[1]);
                    pixel->blue = static_cast<u8>(sums[2]);
                }
                pixel++;
            }
        }
        break;
    }
    }
    return true;
}

void TextHelperView::RenderTextToTextureBold(
    i32 x, i32 y, i32 width, i32 height, i32 glyphWidth,
    i32 glyphHeight, COLORREF textColor, COLORREF shadowColor,
    const char *text, IDirect3DTexture8 *texture)
{
    HFONT font;
    HDC hdc;
    HGDIOBJ previousFont;
    i32 textLength;
    RECT destination;
    RECT source;
    IDirect3DSurface8 *destinationSurface;
    u8 *sourceBits;
    D3DFORMAT sourceFormat;
    i32 sourcePitch;

    font = glyphWidth <= 17 ? g_TextFont17
           : glyphWidth <= 18 ? g_TextFont18
           : glyphWidth <= 19 ? g_TextFont19
                              : g_TextFont20;

    memset(g_TextRenderBuffer.buffer, 0,
           g_TextRenderBuffer.imageSizeInBytes);
    hdc = g_TextRenderBuffer.hdc;
    previousFont = SelectObject(hdc, font);
    g_TextRenderBuffer.InvertAlpha(glyphWidth * 2 + 6, FALSE);
    SetBkMode(hdc, TRANSPARENT);

    textLength = strlen(text);
    SetTextColor(hdc, 0);
    TextOutA(hdc, x * 2 + 3, 3, text, textLength);
    SetTextColor(hdc, textColor);
    TextOutA(hdc, x * 2, 0, text, textLength);

    SelectObject(hdc, previousFont);
    g_TextRenderBuffer.InvertAlpha(
        glyphWidth * 2 + 6, shadowColor == 0xffffffff);
    g_TextRenderBuffer.ApplyAlphaBleed(glyphWidth * 2 + 6);
    SelectObject(hdc, previousFont);

    destination.left = 0;
    destination.top = y;
    destination.right = width;
    destination.bottom = y + glyphHeight;
    source.left = 0;
    source.top = 0;
    source.right = width * 2;
    source.bottom = glyphWidth * 2;
    if (source.right > 1024)
    {
        source.right = 1024;
    }

    texture->GetSurfaceLevel(0, &destinationSurface);
    sourcePitch = g_TextRenderBuffer.imageWidthInBytes;
    sourceFormat = g_TextRenderBuffer.format;
    sourceBits = g_TextRenderBuffer.buffer;
    D3DXLoadSurfaceFromMemory(
        destinationSurface, NULL, &destination, sourceBits, sourceFormat,
        sourcePitch, NULL, &source, D3DX_FILTER_TRIANGLE, 0);
    if (destinationSurface != NULL)
    {
        destinationSurface->Release();
        destinationSurface = NULL;
    }
}

} // namespace th095
