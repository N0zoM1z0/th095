#include "TextRenderer.hpp"

#include <string.h>

namespace th095
{

extern HFONT g_TextFont19;
extern HFONT g_TextFont20;
extern HFONT g_TextFont17;
extern HFONT g_TextFont18;
extern TextRenderBufferView g_TextRenderBuffer;

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
