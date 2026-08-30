#ifndef TH095_TEXT_RENDERER_HPP
#define TH095_TEXT_RENDERER_HPP

#include "PixelFormats.hpp"
#include "inttypes.hpp"

#include <d3dx8.h>
#include <stddef.h>
#include <windows.h>

namespace th095
{

struct TextRenderFormatInfo
{
    D3DFORMAT format;
    i32 bitCount;
    u32 alphaMask;
    u32 redMask;
    u32 greenMask;
    u32 blueMask;
};

struct TextRenderBufferView
{
    TextRenderBufferView();
    ~TextRenderBufferView();

    u8 unknown000[0x100];
    D3DFORMAT format;
    i32 width;
    i32 height;
    u32 imageSizeInBytes;
    i32 imageWidthInBytes;
    HDC hdc;
    HGDIOBJ originalBitmap;
    HGDIOBJ bitmap;
    u8 *buffer;

    bool ReleaseBuffer();
    bool AllocateBufferWithFallback(i32 width, i32 height,
                                    D3DFORMAT format);
    bool TryAllocateBuffer(i32 width, i32 height, D3DFORMAT format);
    TextRenderFormatInfo *GetFormatInfo(D3DFORMAT format);
    bool InvertAlpha(i32 rowCount, BOOL unused);
    bool ApplyAlphaBleed(i32 rowCount);
};

struct TextHelperView
{
    static void CreateTextBuffer();
    static void ReleaseTextBuffer();
    static void RenderTextToTextureBold(
        i32 x, i32 y, i32 width, i32 height, i32 glyphWidth,
        i32 glyphHeight, COLORREF textColor, COLORREF shadowColor,
        const char *text, IDirect3DTexture8 *texture);
};

typedef char TextRenderBufferFormatAt100[
    (offsetof(TextRenderBufferView, format) == 0x100) ? 1 : -1];
typedef char TextRenderBufferImageSizeAt10C[
    (offsetof(TextRenderBufferView, imageSizeInBytes) == 0x10c) ? 1 : -1];
typedef char TextRenderBufferHdcAt114[
    (offsetof(TextRenderBufferView, hdc) == 0x114) ? 1 : -1];
typedef char TextRenderBufferDataAt120[
    (offsetof(TextRenderBufferView, buffer) == 0x120) ? 1 : -1];
typedef char TextRenderFormatInfoSizeIs18[
    (sizeof(TextRenderFormatInfo) == 0x18) ? 1 : -1];

} // namespace th095

#endif
