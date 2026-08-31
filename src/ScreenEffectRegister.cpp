#include "ScreenEffect.hpp"

#include <string.h>

namespace th095
{

typedef int (__fastcall *ChainCallback)(void *arg);
typedef int (__fastcall *ChainLifetimeCallback)(void *arg);

struct ChainElem
{
    short priority;
    unsigned short isHeapAllocated : 1;
    ChainCallback callback;
    ChainLifetimeCallback addedCallback;
    ChainLifetimeCallback deletedCallback;
    ChainElem *prev;
    ChainElem *next;
    ChainElem *releaseTarget;
    void *arg;
};
typedef char ScreenEffectRegisterChainElemSizeIs20[
    (sizeof(ChainElem) == 0x20) ? 1 : -1];

class Chain
{
  public:
    int AddToCalcChain(ChainElem *elem, int priority);
    int AddToDrawChain(ChainElem *elem, int priority);
    ChainElem *CreateElem(ChainCallback callback);
};
extern Chain g_Chain;

// The implicit ScreenEffect default constructor calls this member constructor
// before RegisterChain immediately clears the whole 0x34-byte allocation.
// Keeping it force-inlined reproduces the original new-expression machinery.
__forceinline ScreenEffectTimer::ScreenEffectTimer()
{
    current = 0;
    previous = -999999;
    subFrame = 0.0f;
}

// FUNCTION: TH095 0x00436DD0; TH08 0x0045B8B0 is the source-shape oracle.
// Authored body: 598 bytes. VC7.1 owns a trailing 32-byte eight-entry switch
// table, so canonical comparison covers 630 bytes without crediting the table.
ScreenEffect *ScreenEffect::RegisterChain(
    ScreenEffectType effect, int durationFrames, int primaryParameter,
    int secondaryParameter, int tertiaryParameter, int drawPriority)
{
    ChainElem *calcChain = NULL;
    ChainElem *drawChain = NULL;
    ScreenEffect *screenEffect = new ScreenEffect;

    if (screenEffect == NULL)
        return NULL;

    memset(screenEffect, 0, sizeof(ScreenEffect));

    switch (effect)
    {
    case SCREEN_EFFECT_FULL_FADE_IN:
        calcChain =
            g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeIn);
        drawChain =
            g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawFullFade);
        break;
    case SCREEN_EFFECT_SHAKE:
        calcChain = g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcShake);
        break;
    case SCREEN_EFFECT_ARCADE_FADE_OUT:
        calcChain =
            g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeOut);
        drawChain =
            g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawArcadeFade);
        break;
    case SCREEN_EFFECT_FULL_FADE_OUT:
        calcChain =
            g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeOut);
        drawChain =
            g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawFullFade);
        break;
    case SCREEN_EFFECT_ARCADE_PULSE:
        calcChain =
            g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcArcadePulse);
        drawChain =
            g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawArcadePulse);
        break;
    case SCREEN_EFFECT_FULL_FADE_HOLD:
        calcChain =
            g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeHold);
        drawChain =
            g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawPartialFade);
        break;
    case SCREEN_EFFECT_ARCADE_FADE_HOLD:
        calcChain =
            g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcFadeHold);
        drawChain =
            g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawArcadeFade);
        break;
    case SCREEN_EFFECT_SHAKE_ENVELOPE:
        calcChain =
            g_Chain.CreateElem((ChainCallback)ScreenEffect::CalcShakeEnvelope);
        break;
    }

    calcChain->addedCallback =
        (ChainLifetimeCallback)ScreenEffect::InitializeTimer;
    calcChain->deletedCallback =
        (ChainLifetimeCallback)ScreenEffect::DeleteScreenEffect;
    calcChain->arg = screenEffect;

    screenEffect->type = effect;
    screenEffect->duration = durationFrames;
    screenEffect->rawParameter0 = primaryParameter;
    screenEffect->rawParameter1 = secondaryParameter;
    screenEffect->rawParameter2 = tertiaryParameter;

    if (g_Chain.AddToCalcChain(calcChain, 2) != 0)
        return NULL;

    if (drawChain != NULL)
    {
        drawChain->arg = screenEffect;
        g_Chain.AddToDrawChain(drawChain, drawPriority);
    }

    screenEffect->calcChainElement = calcChain;
    screenEffect->drawChainElement = drawChain;
    return screenEffect;
}

} // namespace th095
