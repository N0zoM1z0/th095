#include "AnmText.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

namespace th095
{

void AnmTextManagerView::DrawTextInner(
    IDirect3DTexture8 *texture, i32 x, i32 y, i32 width, i32 height,
    i32 glyphWidth, i32 glyphHeight, COLORREF textColor,
    COLORREF shadowColor, const char *text, f32 scaleX, f32 scaleY)
{
    if (glyphWidth <= 0)
    {
        glyphWidth = 15;
    }
    if (glyphHeight <= 0)
    {
        glyphHeight = 15;
    }

    if (glyphWidth > 8)
    {
        TextHelperView::RenderTextToTextureBold(
            x, y, width, height, glyphWidth * scaleX, glyphHeight * scaleY,
            textColor, shadowColor, text, texture);
    }
}

// TH08 documents #pragma var_order(buf, fontWidth) for all three formatted
// text helpers. Stock VC7.1 ignores that patched pragma, so use target-proven
// identifier hash buckets for the same real locals. The unlisted x local in
// the right/centered helpers must sort ahead of the ordered buffer/font pair.
#define textBuffer restartCommandProcessingLocal05
#define textGlyphWidth averagedPanLocal12
#define textX textXLocal00
#pragma var_order(textBuffer, textGlyphWidth)
void AnmTextManagerView::DrawTextLeft(AnmTextVmView *vm, COLORREF textColor,
                                     COLORREF shadowColor,
                                     const char *format, ...)
{
    char textBuffer[128];
    i32 textGlyphWidth;
    va_list args;

    textGlyphWidth = vm->glyphWidth;
    va_start(args, format);
    vsprintf(textBuffer, format, args);
    va_end(args);

    this->DrawTextInner(
        vm->loadedSprite->texture,
        vm->loadedSprite->startPixelInclusive.x,
        vm->loadedSprite->startPixelInclusive.y,
        vm->loadedSprite->width,
        vm->loadedSprite->height,
        textGlyphWidth,
        vm->glyphHeight,
        textColor,
        shadowColor,
        textBuffer,
        vm->loadedSprite->scaleFactor.x,
        vm->loadedSprite->scaleFactor.y);

    vm->flagsWord |= 1;
}

#pragma var_order(textBuffer, textGlyphWidth)
void AnmTextManagerView::DrawTextRight(AnmTextVmView *vm,
                                       COLORREF textColor,
                                       COLORREF shadowColor,
                                       const char *format, ...)
{
    char textBuffer[128];
    i32 textX;
    i32 textGlyphWidth;
    va_list args;

    textGlyphWidth = vm->glyphWidth <= 0 ? 15 : vm->glyphWidth;
    va_start(args, format);
    vsprintf(textBuffer, format, args);
    va_end(args);

    textX = vm->loadedSprite->startPixelInclusive.x +
        vm->loadedSprite->widthPx * vm->loadedSprite->scaleFactor.x -
        (f32)strlen(textBuffer) * textGlyphWidth *
            vm->loadedSprite->scaleFactor.x / 2.0f;
    this->DrawTextInner(
        vm->loadedSprite->texture, textX,
        vm->loadedSprite->startPixelInclusive.y,
        vm->loadedSprite->width, vm->loadedSprite->height,
        textGlyphWidth, vm->glyphHeight, textColor, shadowColor, textBuffer,
        vm->loadedSprite->scaleFactor.x,
        vm->loadedSprite->scaleFactor.y);

    vm->flagsWord |= 1;
}

#pragma var_order(textBuffer, textGlyphWidth)
void AnmTextManagerView::DrawTextCentered(AnmTextVmView *vm,
                                          COLORREF textColor,
                                          COLORREF shadowColor,
                                          const char *format, ...)
{
    char textBuffer[72];
    i32 textX;
    i32 textGlyphWidth;
    va_list args;

    textGlyphWidth = vm->glyphWidth <= 0 ? 15 : vm->glyphWidth;
    va_start(args, format);
    vsprintf(textBuffer, format, args);
    va_end(args);

    textX = vm->loadedSprite->startPixelInclusive.x +
        vm->loadedSprite->widthPx * vm->loadedSprite->scaleFactor.x / 2.0f -
        (f32)strlen(textBuffer) * textGlyphWidth *
            vm->loadedSprite->scaleFactor.x / 4.0f;
    this->DrawTextInner(
        vm->loadedSprite->texture, textX,
        vm->loadedSprite->startPixelInclusive.y,
        vm->loadedSprite->width, vm->loadedSprite->height,
        textGlyphWidth, vm->glyphHeight, textColor, shadowColor, textBuffer,
        vm->loadedSprite->scaleFactor.x,
        vm->loadedSprite->scaleFactor.y);

    vm->flagsWord |= 1;
}
#undef textBuffer
#undef textGlyphWidth
#undef textX

} // namespace th095
