#pragma once

#include <stddef.h>

namespace th095
{

struct ScreenEffectTimer
{
    int previous;
    float subFrame;
    int current;

    __forceinline operator int();
    __forceinline operator float();
    int Tick();
    __forceinline void operator++(int);
    __forceinline unsigned int operator<=(int value);
    __forceinline unsigned int operator>=(int value);
};

typedef char ScreenEffectTimerSizeIs0C[(sizeof(ScreenEffectTimer) == 0x0c) ? 1 : -1];

struct ScreenEffectRect
{
    float left;
    float top;
    float right;
    float bottom;
};

struct ScreenEffect
{
    int type;
    void *calcChainElement;
    void *drawChainElement;
    int unconsumedDword0C;
    int overlayAlpha;
    int duration;
    unsigned int rawParameter0;
    int rawParameter1;
    int rawParameter2;
    int fadeReleaseRequested;
    ScreenEffectTimer timer;

    static void Clear(unsigned int color);
    static void SetViewport(unsigned int clearColor);
    static int CalcFadeIn(ScreenEffect *screenEffect);
    static int CalcFadeHold(ScreenEffect *screenEffect);
    static void DrawSquare(ScreenEffectRect *rect, unsigned int color);
    static int DrawFullFade(ScreenEffect *screenEffect);
    static int DrawPartialFade(ScreenEffect *screenEffect);
    static int DrawArcadeFade(ScreenEffect *screenEffect);
    static int DrawArcadePulse(ScreenEffect *screenEffect);
};

typedef char ScreenEffectSizeIs34[(sizeof(ScreenEffect) == 0x34) ? 1 : -1];
typedef char ScreenEffectAlphaAt10[(offsetof(ScreenEffect, overlayAlpha) == 0x10) ? 1 : -1];
typedef char ScreenEffectDurationAt14[(offsetof(ScreenEffect, duration) == 0x14) ? 1 : -1];
typedef char ScreenEffectRawAt18[(offsetof(ScreenEffect, rawParameter0) == 0x18) ? 1 : -1];
typedef char ScreenEffectReleaseAt24[(offsetof(ScreenEffect, fadeReleaseRequested) == 0x24) ? 1 : -1];
typedef char ScreenEffectTimerAt28[(offsetof(ScreenEffect, timer) == 0x28) ? 1 : -1];

} // namespace th095
