#include "AnmText.hpp"
#include <stdarg.h>
#include <stdio.h>

namespace th095
{

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

} // namespace th095
