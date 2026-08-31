#define WIN32_LEAN_AND_MEAN
#include "ScreenEffect.hpp"

#include <windows.h>
#include <d3d8.h>
#include <stddef.h>

namespace th095
{

struct ScreenEffectAnmManagerView
{
    void FlushVertexBuffer();
};
extern ScreenEffectAnmManagerView *g_AnmManager;

struct ChainElem;
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
typedef char ScreenEffectChainElemSizeIs20[(sizeof(ChainElem) == 0x20) ? 1 : -1];

class Chain
{
  public:
    void Cut(ChainElem *elem);
};
extern Chain g_Chain;

class Rng
{
  public:
    unsigned int GetRandomU32();
    __forceinline unsigned int GetRandomU32InRange(unsigned int range)
    {
        return range != 0 ? GetRandomU32() % range : 0;
    }
};
extern Rng g_Rng;

extern int g_ScreenEffectCounter;
extern float g_ScreenEffectShakeX;
extern float g_ScreenEffectShakeY;

struct ScreenEffectPhotoGlobalStateView
{
    unsigned char unknown000[0xfc];
    union
    {
        unsigned int flags;
        struct
        {
            unsigned int flag0 : 1;
            unsigned int flag1 : 1;
            unsigned int flag2 : 1;
            unsigned int flag3 : 1;
            unsigned int flag4 : 1;
            unsigned int flag5 : 1;
            unsigned int flag6 : 1;
            unsigned int remaining : 25;
        };
    };
};
typedef char ScreenEffectPhotoFlagsAtFC[
    (offsetof(ScreenEffectPhotoGlobalStateView, flags) == 0xfc) ? 1 : -1];
extern ScreenEffectPhotoGlobalStateView *g_PhotoGlobalState;

static __forceinline int ScreenEffectEitherFlag(int first, int second)
{
    return first | second;
}

struct ScreenEffectSupervisorView
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice;
    unsigned char unknown00C[0xCC - 0x0C];
    D3DVIEWPORT8 viewport;
    D3DPRESENT_PARAMETERS presentParameters;
};
extern ScreenEffectSupervisorView g_Supervisor;

typedef char ScreenEffectSupervisorDeviceAt08[(offsetof(ScreenEffectSupervisorView, d3dDevice) == 0x08) ? 1 : -1];
typedef char ScreenEffectSupervisorViewportAtCC[(offsetof(ScreenEffectSupervisorView, viewport) == 0xcc) ? 1 : -1];
typedef char ScreenEffectSupervisorPresentAtE4[(offsetof(ScreenEffectSupervisorView, presentParameters) == 0xe4) ? 1 : -1];

__forceinline ScreenEffectTimer::operator int()
{
    return this->current;
}

__forceinline ScreenEffectTimer::operator float()
{
    return this->subFrame;
}

__forceinline void ScreenEffectTimer::operator++(int)
{
    this->Tick();
}

__forceinline void ScreenEffectTimer::operator=(int value)
{
    this->current = value;
    this->subFrame = (float)value;
    this->previous = -999999;
}

__forceinline unsigned int ScreenEffectTimer::operator<(int value)
{
    return this->current < value;
}

__forceinline unsigned int ScreenEffectTimer::operator<=(int value)
{
    return this->current <= value;
}

__forceinline unsigned int ScreenEffectTimer::operator>=(int value)
{
    return this->current >= value;
}

// FUNCTION: TH095 0x00436760; TH08 0x0045B020 is the source-shape oracle.
void ScreenEffect::Clear(unsigned int color)
{
    g_Supervisor.d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
    if (g_Supervisor.d3dDevice->Present(NULL, NULL, NULL, NULL) < 0)
        g_Supervisor.d3dDevice->Reset(&g_Supervisor.presentParameters);
    g_Supervisor.d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
    if (g_Supervisor.d3dDevice->Present(NULL, NULL, NULL, NULL) < 0)
        g_Supervisor.d3dDevice->Reset(&g_Supervisor.presentParameters);
}

// FUNCTION: TH095 0x00436820; TH08 0x0045B0E0 is the source-shape oracle.
void ScreenEffect::SetViewport(unsigned int clearColor)
{
    if (g_AnmManager != NULL)
        g_AnmManager->FlushVertexBuffer();
    g_Supervisor.viewport.X = 0;
    g_Supervisor.viewport.Y = 0;
    g_Supervisor.viewport.Width = 640;
    g_Supervisor.viewport.Height = 480;
    g_Supervisor.viewport.MinZ = 0.0f;
    g_Supervisor.viewport.MaxZ = 1.0f;
    g_Supervisor.d3dDevice->SetViewport(&g_Supervisor.viewport);
    ScreenEffect::Clear(clearColor);
}

// FUNCTION: TH095 0x004368A0; TH08 0x0045B160 is the source-shape oracle.
int ScreenEffect::CalcFadeIn(ScreenEffect *screenEffect)
{
    if (screenEffect->duration != 0)
    {
        // Operand order is target-visible in x87 codegen under VC7.1.
        screenEffect->overlayAlpha =
            (int)(255.0f - (((float)screenEffect->timer * 255.0f) / screenEffect->duration));
        if (screenEffect->overlayAlpha < 0)
            screenEffect->overlayAlpha = 0;
    }
    if (screenEffect->timer >= screenEffect->duration)
        return 0;
    screenEffect->timer++;
    return 1;
}

// FUNCTION: TH095 0x00436D20; TH08 0x0045B800 is the source-shape oracle.
int ScreenEffect::CalcFadeHold(ScreenEffect *screenEffect)
{
    if (screenEffect->fadeReleaseRequested == 0)
    {
        if (screenEffect->duration != 0 && screenEffect->timer <= screenEffect->duration)
            screenEffect->overlayAlpha =
                (int)(((float)screenEffect->timer * 128.0f) / screenEffect->duration);
    }
    else
    {
        if (screenEffect->timer <= 8)
            screenEffect->overlayAlpha =
                128 - (int)(((float)screenEffect->timer * 128.0f) / 8.0f);
        else
            return 0;
    }
    screenEffect->timer++;
    return 1;
}

// FUNCTION: TH095 0x00437050; TH08 0x0045BB50 is the source-shape oracle.
int ScreenEffect::DrawFullFade(ScreenEffect *screenEffect)
{
    ScreenEffectRect rect = {0.0f, 0.0f, 640.0f, 480.0f};
    g_AnmManager->FlushVertexBuffer();
    g_Supervisor.viewport.X = 0;
    g_Supervisor.viewport.Y = 0;
    g_Supervisor.viewport.Width = 640;
    g_Supervisor.viewport.Height = 480;
    g_Supervisor.d3dDevice->SetViewport(&g_Supervisor.viewport);
    ScreenEffect::DrawSquare(&rect, (screenEffect->overlayAlpha << 24) | screenEffect->rawParameter0);
    return 1;
}

// FUNCTION: TH095 0x004370F0; TH08 0x0045BBF0 is the source-shape oracle.
int ScreenEffect::DrawPartialFade(ScreenEffect *screenEffect)
{
    ScreenEffectRect rect = {0.0f, 0.0f, 640.0f, 480.0f};
    ScreenEffect::DrawSquare(&rect, (screenEffect->overlayAlpha << 24) | screenEffect->rawParameter0);
    return 1;
}

// FUNCTION: TH095 0x00437140; TH08 0x0045BC40 is the source-shape oracle.
int ScreenEffect::DrawArcadeFade(ScreenEffect *screenEffect)
{
    ScreenEffectRect rect = {32.0f, 16.0f, 416.0f, 464.0f};
    ScreenEffect::DrawSquare(&rect, (screenEffect->overlayAlpha << 24) | screenEffect->rawParameter0);
    return 1;
}

// FUNCTION: TH095 0x00437280; TH08 0x0045BD70 is the source-shape oracle.
int ScreenEffect::DrawArcadePulse(ScreenEffect *screenEffect)
{
    // TH095 deliberately shifts this rectangle right relative to TH08.
    ScreenEffectRect rect = {128.0f, 16.0f, 512.0f, 464.0f};
    ScreenEffect::DrawSquare(
        &rect, (screenEffect->overlayAlpha << 24) |
                   ((unsigned int)screenEffect->rawParameter1 & 0x00ffffff));
    return 1;
}


// FUNCTION: TH095 0x00436CA0. TH095 removes TH08's menu/retry gate and
// leaves timer advancement to the owner; this callback only computes alpha.
int ScreenEffect::CalcFadeOut(ScreenEffect *screenEffect)
{
    if (g_ScreenEffectCounter != 0)
        return 0;

    if (screenEffect->duration != 0)
    {
        screenEffect->overlayAlpha =
            (int)(((float)screenEffect->timer * 255.0f) /
                  screenEffect->duration);
        if (screenEffect->overlayAlpha < 0)
            screenEffect->overlayAlpha = 0;
    }

    if (screenEffect->timer >= screenEffect->duration)
        return 0;
    return 1;
}

// FUNCTION: TH095 0x00437190.
int ScreenEffect::CalcArcadePulse(ScreenEffect *screenEffect)
{
    unsigned int alpha =
        ((unsigned int)screenEffect->rawParameter1 >> 24) & 0xff;

    if (g_ScreenEffectCounter != 0)
        return 0;

    if (screenEffect->timer < screenEffect->duration)
    {
        screenEffect->overlayAlpha =
            alpha - (int)(((float)screenEffect->timer * alpha) /
                          screenEffect->duration);
        if (screenEffect->overlayAlpha < 0)
            screenEffect->overlayAlpha = 0;
    }
    else
    {
        screenEffect->overlayAlpha = 0;
        screenEffect->rawParameter0--;
        if (screenEffect->rawParameter0 <= 0)
            return 0;
        screenEffect->timer = 0;
    }

    screenEffect->timer++;
    return 1;
}

// FUNCTION: TH095 0x004372D0.
int ScreenEffect::CalcShake(ScreenEffect *screenEffect)
{
    float shakeAmount;

    if (g_ScreenEffectCounter != 0)
        return 0;

    if (g_PhotoGlobalState != NULL)
    {
        if (ScreenEffectEitherFlag(g_PhotoGlobalState->flag0,
                                   g_PhotoGlobalState->flag2) != 0 ||
            g_PhotoGlobalState->flag1 != 0 ||
            g_PhotoGlobalState->flag4 != 0 ||
            g_PhotoGlobalState->flag5 != 0 ||
            g_PhotoGlobalState->flag6 != 0)
            return 1;
    }
    else
    {
        return 1;
    }

    screenEffect->timer++;
    if (screenEffect->timer >= screenEffect->duration)
        return 0;

    shakeAmount =
        (float)(screenEffect->rawParameter1 - screenEffect->rawParameter0) *
        (float)screenEffect->timer;
    shakeAmount = shakeAmount / screenEffect->duration;
    shakeAmount = (float)screenEffect->rawParameter0 + shakeAmount;

    switch (g_Rng.GetRandomU32InRange(3))
    {
    case 0:
        g_ScreenEffectShakeX = 0.0f;
        break;
    case 1:
        g_ScreenEffectShakeX = shakeAmount;
        break;
    case 2:
        g_ScreenEffectShakeX = -shakeAmount;
        break;
    }

    switch (g_Rng.GetRandomU32InRange(3))
    {
    case 0:
        g_ScreenEffectShakeY = 0.0f;
        break;
    case 1:
        g_ScreenEffectShakeY = shakeAmount;
        break;
    case 2:
        g_ScreenEffectShakeY = -shakeAmount;
        break;
    }
    return 1;
}

// FUNCTION: TH095 0x004374B0.
int ScreenEffect::CalcShakeEnvelope(ScreenEffect *screenEffect)
{
    float shakeAmount;

    if (g_ScreenEffectCounter != 0)
        return 0;

    if (g_PhotoGlobalState != NULL)
    {
        if (ScreenEffectEitherFlag(g_PhotoGlobalState->flag0,
                                   g_PhotoGlobalState->flag2) != 0 ||
            g_PhotoGlobalState->flag1 != 0 ||
            g_PhotoGlobalState->flag4 != 0 ||
            g_PhotoGlobalState->flag5 != 0 ||
            g_PhotoGlobalState->flag6 != 0)
            return 1;
    }
    else
    {
        return 1;
    }

    screenEffect->timer++;
    if (screenEffect->timer < screenEffect->rawParameter0)
    {
        shakeAmount =
            (float)screenEffect->timer / screenEffect->rawParameter0;
    }
    else if (screenEffect->timer <
             screenEffect->rawParameter0 + screenEffect->rawParameter1)
    {
        shakeAmount = 1.0f;
    }
    else
    {
        if (screenEffect->timer < screenEffect->rawParameter0 +
                                      screenEffect->rawParameter1 +
                                      screenEffect->rawParameter2)
        {
            shakeAmount =
                ((float)(unsigned int)(screenEffect->rawParameter0 +
                                       screenEffect->rawParameter1 +
                                       screenEffect->rawParameter2) -
                 (float)screenEffect->timer) /
                (unsigned int)screenEffect->rawParameter2;
        }
        else
        {
            return 0;
        }
    }

    shakeAmount = (float)screenEffect->duration * shakeAmount;

    switch (g_Rng.GetRandomU32InRange(3))
    {
    case 0:
        g_ScreenEffectShakeX = 0.0f;
        break;
    case 1:
        g_ScreenEffectShakeX = shakeAmount;
        break;
    case 2:
        g_ScreenEffectShakeX = -shakeAmount;
        break;
    }

    switch (g_Rng.GetRandomU32InRange(3))
    {
    case 0:
        g_ScreenEffectShakeY = 0.0f;
        break;
    case 1:
        g_ScreenEffectShakeY = shakeAmount;
        break;
    case 2:
        g_ScreenEffectShakeY = -shakeAmount;
        break;
    }
    return 1;
}

// FUNCTION: TH095 0x00437700.
int ScreenEffect::InitializeTimer(ScreenEffect *screenEffect)
{
    ScreenEffectTimer *timer = &screenEffect->timer;
    timer->current = 0;
    timer->subFrame = 0.0f;
    timer->previous = -999999;
    return 0;
}

// FUNCTION: TH095 0x00437740.
int ScreenEffect::DeleteScreenEffect(ScreenEffect *screenEffect)
{
    screenEffect->calcChainElement->deletedCallback = NULL;
    g_Chain.Cut(screenEffect->drawChainElement);
    screenEffect->drawChainElement = NULL;
    delete screenEffect;
    screenEffect = NULL;
    return 0;
}

} // namespace th095
