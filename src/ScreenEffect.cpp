#define WIN32_LEAN_AND_MEAN
#include "AnmManager.hpp"
#include "Chain.hpp"
#include "diffbuild.hpp"
#include "Rng.hpp"
#include "PhotoGameTask.hpp"
#include "ScreenEffect.hpp"
#include "ZunTimer.hpp"

#include <windows.h>
#include <d3d8.h>
#include <stddef.h>

namespace th095
{

DIFFABLE_STATIC(int, g_ScreenEffectCounter);

// Target 0x004BDEC8 is the task's standalone publication slot, distinct
// from Supervisor::photoGameTask. The full task owner supplies its flag word.
extern void *g_RuntimeGlobalStateOwner;
static __forceinline PhotoGameTaskView *ScreenEffectPhotoTask()
{
    return static_cast<PhotoGameTaskView *>(g_RuntimeGlobalStateOwner);
}

static __forceinline int ScreenEffectEitherFlag(int first, int second)
{
    return first | second;
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
                   (screenEffect->arcadePulseColor & 0x00ffffff));
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
        (screenEffect->arcadePulseColor >> 24) & 0xff;

    if (g_ScreenEffectCounter != 0)
        return 0;

    if (screenEffect->timer < screenEffect->arcadePulseFadeFrames)
    {
        screenEffect->overlayAlpha =
            alpha - (int)(((float)screenEffect->timer * alpha) /
                          screenEffect->arcadePulseFadeFrames);
        if (screenEffect->overlayAlpha < 0)
            screenEffect->overlayAlpha = 0;
    }
    else
    {
        screenEffect->overlayAlpha = 0;
        screenEffect->arcadePulseRepeatCount--;
        if (screenEffect->arcadePulseRepeatCount <= 0)
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

    if (ScreenEffectPhotoTask() != NULL)
    {
        if (ScreenEffectEitherFlag(ScreenEffectPhotoTask()->captureActive,
                                   ScreenEffectPhotoTask()->gameplayLoadActive) != 0 ||
            ScreenEffectPhotoTask()->capturedPhotoActive != 0 ||
            ScreenEffectPhotoTask()->resultScreenActive != 0 ||
            ScreenEffectPhotoTask()->playerDeathTransitionComplete != 0 ||
            ScreenEffectPhotoTask()->photoLimitTransitionComplete != 0)
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
        g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.x = 0.0f;
        break;
    case 1:
        g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.x = shakeAmount;
        break;
    case 2:
        g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.x = -shakeAmount;
        break;
    }

    switch (g_Rng.GetRandomU32InRange(3))
    {
    case 0:
        g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.y = 0.0f;
        break;
    case 1:
        g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.y = shakeAmount;
        break;
    case 2:
        g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.y = -shakeAmount;
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

    if (ScreenEffectPhotoTask() != NULL)
    {
        if (ScreenEffectEitherFlag(ScreenEffectPhotoTask()->captureActive,
                                   ScreenEffectPhotoTask()->gameplayLoadActive) != 0 ||
            ScreenEffectPhotoTask()->capturedPhotoActive != 0 ||
            ScreenEffectPhotoTask()->resultScreenActive != 0 ||
            ScreenEffectPhotoTask()->playerDeathTransitionComplete != 0 ||
            ScreenEffectPhotoTask()->photoLimitTransitionComplete != 0)
            return 1;
    }
    else
    {
        return 1;
    }

    screenEffect->timer++;
    if (screenEffect->timer < screenEffect->shakeEnvelopeRampUpFrames)
    {
        shakeAmount =
            (float)screenEffect->timer / screenEffect->shakeEnvelopeRampUpFrames;
    }
    else if (screenEffect->timer <
             screenEffect->shakeEnvelopeRampUpFrames + screenEffect->shakeEnvelopeHoldFrames)
    {
        shakeAmount = 1.0f;
    }
    else
    {
        if (screenEffect->timer < screenEffect->shakeEnvelopeRampUpFrames +
                                      screenEffect->shakeEnvelopeHoldFrames +
                                      screenEffect->shakeEnvelopeRampDownFrames)
        {
            shakeAmount =
                ((float)(unsigned int)(screenEffect->shakeEnvelopeRampUpFrames +
                                       screenEffect->shakeEnvelopeHoldFrames +
                                       screenEffect->shakeEnvelopeRampDownFrames) -
                 (float)screenEffect->timer) /
                (unsigned int)screenEffect->shakeEnvelopeRampDownFrames;
        }
        else
        {
            return 0;
        }
    }

    shakeAmount = (float)screenEffect->shakeEnvelopeAmplitude * shakeAmount;

    switch (g_Rng.GetRandomU32InRange(3))
    {
    case 0:
        g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.x = 0.0f;
        break;
    case 1:
        g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.x = shakeAmount;
        break;
    case 2:
        g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.x = -shakeAmount;
        break;
    }

    switch (g_Rng.GetRandomU32InRange(3))
    {
    case 0:
        g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.y = 0.0f;
        break;
    case 1:
        g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.y = shakeAmount;
        break;
    case 2:
        g_Supervisor.viewportConfigurations[TH095_SUPERVISOR_VIEWPORT_PLAYFIELD].screenShakeOffset.y = -shakeAmount;
        break;
    }
    return 1;
}

// FUNCTION: TH095 0x00437700.
int ScreenEffect::InitializeTimer(ScreenEffect *screenEffect)
{
    ZunTimer *timer = &screenEffect->timer;
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
