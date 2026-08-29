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
        AnmTextRendererView::RenderTextToTextureBold(
            x, y, width, height, glyphWidth * scaleX, glyphHeight * scaleY,
            textColor, shadowColor, text, texture);
    }
}

void AnmTextManagerView::DrawTextLeft(AnmTextVmView *vm, COLORREF textColor,
                                     COLORREF shadowColor,
                                     const char *format, ...)
{
    i32 glyphWidth;
    char buffer[128];
    va_list args;

    glyphWidth = vm->glyphWidth;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    this->DrawTextInner(
        vm->loadedSprite->texture,
        vm->loadedSprite->startPixelInclusive.x,
        vm->loadedSprite->startPixelInclusive.y,
        vm->loadedSprite->width,
        vm->loadedSprite->height,
        glyphWidth,
        vm->glyphHeight,
        textColor,
        shadowColor,
        buffer,
        vm->loadedSprite->scaleFactor.x,
        vm->loadedSprite->scaleFactor.y);

    vm->flagsWord |= 1;
}

void AnmTextManagerView::DrawTextRight(AnmTextVmView *vm,
                                       COLORREF textColor,
                                       COLORREF shadowColor,
                                       const char *format, ...)
{
    char buffer[128];
    i32 x;
    i32 glyphWidth;
    va_list args;

    glyphWidth = vm->glyphWidth <= 0 ? 15 : vm->glyphWidth;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    x = vm->loadedSprite->startPixelInclusive.x +
        vm->loadedSprite->widthPx * vm->loadedSprite->scaleFactor.x -
        (f32)strlen(buffer) * glyphWidth *
            vm->loadedSprite->scaleFactor.x / 2.0f;
    this->DrawTextInner(
        vm->loadedSprite->texture, x,
        vm->loadedSprite->startPixelInclusive.y,
        vm->loadedSprite->width, vm->loadedSprite->height,
        glyphWidth, vm->glyphHeight, textColor, shadowColor, buffer,
        vm->loadedSprite->scaleFactor.x,
        vm->loadedSprite->scaleFactor.y);

    vm->flagsWord |= 1;
}

void AnmTextManagerView::DrawTextCentered(AnmTextVmView *vm,
                                          COLORREF textColor,
                                          COLORREF shadowColor,
                                          const char *format, ...)
{
    char buffer[72];
    i32 x;
    i32 glyphWidth;
    va_list args;

    glyphWidth = vm->glyphWidth <= 0 ? 15 : vm->glyphWidth;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    x = vm->loadedSprite->startPixelInclusive.x +
        vm->loadedSprite->widthPx * vm->loadedSprite->scaleFactor.x / 2.0f -
        (f32)strlen(buffer) * glyphWidth *
            vm->loadedSprite->scaleFactor.x / 4.0f;
    this->DrawTextInner(
        vm->loadedSprite->texture, x,
        vm->loadedSprite->startPixelInclusive.y,
        vm->loadedSprite->width, vm->loadedSprite->height,
        glyphWidth, vm->glyphHeight, textColor, shadowColor, buffer,
        vm->loadedSprite->scaleFactor.x,
        vm->loadedSprite->scaleFactor.y);

    vm->flagsWord |= 1;
}

} // namespace th095
