#ifndef TH095_ANM_TEXT_HPP
#define TH095_ANM_TEXT_HPP

#include "AnmManager.hpp"

namespace th095
{

struct AnmTextSpriteView
{
    i32 anmIdx;
    IDirect3DTexture8 *texture;
    Float2 startPixelInclusive;
    Float2 endPixelInclusive;
    f32 height;
    f32 width;
    u8 unknown020[0x14];
    f32 widthPx;
    Float2 scaleFactor;
};

struct AnmTextVmView
{
    u8 unknown000[0x228];
    u32 flagsWord;
    u8 unknown22c[0x18];
    AnmTextSpriteView *loadedSprite;
    u8 unknown248[0x78];
    u8 glyphWidth;
    u8 glyphHeight;
};

struct AnmTextManagerView
{
    void DrawTextInner(IDirect3DTexture8 *texture, i32 x, i32 y, i32 width,
                       i32 height, i32 glyphWidth, i32 glyphHeight,
                       COLORREF textColor, COLORREF shadowColor,
                       const char *text, f32 scaleX, f32 scaleY);
    void DrawTextLeft(AnmTextVmView *vm, COLORREF textColor,
                      COLORREF shadowColor, const char *format, ...);
    void DrawTextRight(AnmTextVmView *vm, COLORREF textColor,
                       COLORREF shadowColor, const char *format, ...);
    void DrawTextCentered(AnmTextVmView *vm, COLORREF textColor,
                          COLORREF shadowColor, const char *format, ...);
};

struct AnmTextRendererView
{
    static void RenderTextToTextureBold(
        i32 x, i32 y, i32 width, i32 height, i32 glyphWidth,
        i32 glyphHeight, COLORREF textColor, COLORREF shadowColor,
        const char *text, IDirect3DTexture8 *texture);
};

typedef char AnmTextSpriteScaleAt38[
    (offsetof(AnmTextSpriteView, scaleFactor) == 0x38) ? 1 : -1];
typedef char AnmTextSpriteWidthPxAt34[
    (offsetof(AnmTextSpriteView, widthPx) == 0x34) ? 1 : -1];
typedef char AnmTextVmFlagsAt228[
    (offsetof(AnmTextVmView, flagsWord) == 0x228) ? 1 : -1];
typedef char AnmTextVmSpriteAt244[
    (offsetof(AnmTextVmView, loadedSprite) == 0x244) ? 1 : -1];
typedef char AnmTextVmGlyphSizeAt2C0[
    (offsetof(AnmTextVmView, glyphWidth) == 0x2c0 &&
     offsetof(AnmTextVmView, glyphHeight) == 0x2c1) ? 1 : -1];

} // namespace th095

#endif
