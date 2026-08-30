#include "TextRenderer.hpp"

#include "Global.hpp"

#include <string.h>

namespace th095
{

extern HFONT g_TextFont19;
extern HFONT g_TextFont20;
extern HFONT g_TextFont17;
extern HFONT g_TextFont18;
extern TextRenderBufferView g_TextRenderBuffer;

DIFFABLE_STATIC_ARRAY_ASSIGN(TextRenderFormatInfo, 7,
                             g_TextRenderFormatInfoArray) = {
    {D3DFMT_X8R8G8B8, 32, 0x00000000, 0x00ff0000, 0x0000ff00,
     0x000000ff},
    {D3DFMT_A8R8G8B8, 32, 0xff000000, 0x00ff0000, 0x0000ff00,
     0x000000ff},
    {D3DFMT_X1R5G5B5, 16, 0x00000000, 0x00007c00, 0x000003e0,
     0x0000001f},
    {D3DFMT_R5G6B5, 16, 0x00000000, 0x0000f800, 0x000007e0,
     0x0000001f},
    {D3DFMT_A1R5G5B5, 16, 0x00008000, 0x00007c00, 0x000003e0,
     0x0000001f},
    {D3DFMT_A4R4G4B4, 16, 0x0000f000, 0x00000f00, 0x000000f0,
     0x0000000f},
    {(D3DFORMAT)-1, 0, 0, 0, 0, 0},
};

TextRenderBufferView::TextRenderBufferView()
{
    this->format = (D3DFORMAT)-1;
    this->width = 0;
    this->height = 0;
    this->hdc = 0;
    this->bitmap = 0;
    this->originalBitmap = 0;
    this->buffer = NULL;
}

TextRenderBufferView::~TextRenderBufferView()
{
    this->ReleaseBuffer();
}

bool TextRenderBufferView::ReleaseBuffer()
{
    if (this->hdc)
    {
        SelectObject(this->hdc, this->originalBitmap);
        DeleteDC(this->hdc);
        DeleteObject(this->bitmap);
        this->format = (D3DFORMAT)-1;
        this->width = 0;
        this->height = 0;
        this->hdc = 0;
        this->bitmap = 0;
        this->originalBitmap = 0;
        this->buffer = NULL;
        return true;
    }
    else
    {
        return false;
    }
}

bool TextRenderBufferView::AllocateBufferWithFallback(
    i32 width, i32 height, D3DFORMAT format)
{
    if (this->TryAllocateBuffer(width, height, format))
    {
        return true;
    }

    if (format == D3DFMT_A1R5G5B5 || format == D3DFMT_A4R4G4B4)
    {
        return this->TryAllocateBuffer(width, height, D3DFMT_A8R8G8B8);
    }
    if (format == D3DFMT_R5G6B5)
    {
        return this->TryAllocateBuffer(width, height, D3DFMT_X8R8G8B8);
    }
    return false;
}

struct TextBitmapInfo
{
    BITMAPINFOHEADER header;
    RGBQUAD colors[17];
};

typedef char TextBitmapInfoSizeIs6C[
    (sizeof(TextBitmapInfo) == 0x6c) ? 1 : -1];

bool TextRenderBufferView::TryAllocateBuffer(i32 width, i32 height,
                                             D3DFORMAT format)
{
    HGDIOBJ originalBitmap;
    u8 *bitmapData;
    HBITMAP bitmap;
    TextRenderFormatInfo *formatInfo;
    TextBitmapInfo bitmapInfo;
    HDC deviceContext;
    i32 imageWidthInBytes;

    this->ReleaseBuffer();
    memset(&bitmapInfo, 0, sizeof(TextBitmapInfo));
    formatInfo = this->GetFormatInfo(format);
    if (formatInfo == NULL)
    {
        return false;
    }
    imageWidthInBytes =
        ((((width * formatInfo->bitCount) / 8) + 3) / 4) * 4;
    bitmapInfo.header.biSize = sizeof(TextBitmapInfo);
    bitmapInfo.header.biWidth = width;
    bitmapInfo.header.biHeight = -(height + 1);
    bitmapInfo.header.biPlanes = 1;
    bitmapInfo.header.biBitCount = formatInfo->bitCount;
    bitmapInfo.header.biSizeImage = height * imageWidthInBytes;
    if (format != D3DFMT_X1R5G5B5 && format != D3DFMT_X8R8G8B8)
    {
        bitmapInfo.header.biCompression = BI_BITFIELDS;
        reinterpret_cast<u32 *>(bitmapInfo.colors)[0] = formatInfo->redMask;
        reinterpret_cast<u32 *>(bitmapInfo.colors)[1] = formatInfo->greenMask;
        reinterpret_cast<u32 *>(bitmapInfo.colors)[2] = formatInfo->blueMask;
        reinterpret_cast<u32 *>(bitmapInfo.colors)[3] = formatInfo->alphaMask;
    }
    bitmap = CreateDIBSection(NULL, reinterpret_cast<BITMAPINFO *>(&bitmapInfo),
                              DIB_RGB_COLORS,
                              reinterpret_cast<void **>(&bitmapData), NULL, 0);
    if (bitmap == NULL)
    {
        return false;
    }
    memset(bitmapData, 0, bitmapInfo.header.biSizeImage);
    deviceContext = CreateCompatibleDC(NULL);
    originalBitmap = SelectObject(deviceContext, bitmap);
    this->hdc = deviceContext;
    this->bitmap = bitmap;
    this->buffer = bitmapData;
    this->imageSizeInBytes = bitmapInfo.header.biSizeImage;
    this->originalBitmap = originalBitmap;
    this->width = width;
    this->height = height;
    this->format = format;
    this->imageWidthInBytes = imageWidthInBytes;
    return true;
}

TextRenderFormatInfo *TextRenderBufferView::GetFormatInfo(D3DFORMAT format)
{
    i32 formatIndex;

    for (formatIndex = 0;
         g_TextRenderFormatInfoArray[formatIndex].format != -1 &&
         g_TextRenderFormatInfoArray[formatIndex].format != format;
         formatIndex++)
    {
    }
    if (format == -1)
    {
        return NULL;
    }
    return &g_TextRenderFormatInfoArray[formatIndex];
}

void TextHelperView::CreateTextBuffer()
{
    u32 index;

    g_TextRenderBuffer.AllocateBufferWithFallback(1024, 64,
                                                   D3DFMT_A4R4G4B4);
    for (index = 0; index < 256; index++)
    {
        g_TextRenderBuffer.unknown000[index] =
            static_cast<u8>((g_Rng.GetRandomU16() >> 8) / 2);
    }
    g_TextFont17 = CreateFontA(
        30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN,
        "\x82\x6c\x82\x72\x20\x83\x53\x83\x56\x83\x62\x83\x4e");
    g_TextFont18 = CreateFontA(
        34, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN,
        "\x82\x6c\x82\x72\x20\x83\x53\x83\x56\x83\x62\x83\x4e");
    g_TextFont19 = CreateFontA(
        36, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN,
        "\x82\x6c\x82\x72\x20\x83\x53\x83\x56\x83\x62\x83\x4e");
    g_TextFont20 = CreateFontA(
        38, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        FIXED_PITCH | FF_ROMAN,
        "\x82\x6c\x82\x72\x20\x83\x53\x83\x56\x83\x62\x83\x4e");
}

void TextHelperView::ReleaseTextBuffer()
{
    g_TextRenderBuffer.ReleaseBuffer();
    DeleteObject(g_TextFont17);
    DeleteObject(g_TextFont18);
    DeleteObject(g_TextFont19);
    DeleteObject(g_TextFont20);
}

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
