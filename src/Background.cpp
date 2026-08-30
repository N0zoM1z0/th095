#define TH095_DECLARE_ANM_LOADED_INITIALIZE_VM
#include "AnmManager.hpp"
#include "AnmVmId.hpp"

namespace th095
{

struct Background
{
    void SetPhotoArea(const Float3 *position, const Float3 *size);
    void StartSpellBackground();
    void StopSpellBackground();
};

struct BackgroundStateView
{
    u8 unknown0000[0x175c];
    i32 spellBackgroundState;       // +0x175c
    ZunColor photoColor;            // +0x1760
    i32 photoAreaActive;            // +0x1764
    Float3 photoAreaPosition;       // +0x1768
    Float3 photoAreaSize;           // +0x1774
    AnmVm photoAreaVms[3];          // +0x1780
    AnmVmId spellBackgroundVms[2];  // +0x1fe4
};

struct BackgroundStageStateView
{
    u8 unknown00000[0x2571c];
    AnmLoaded *anm;
};

struct BackgroundAnmSpawnerView
{
    AnmVmId CreateVm(i32 scriptIndex, Float3 *position);
};

struct BackgroundRuntimeView
{
    u8 unknown0000[0x4df8];
    BackgroundAnmSpawnerView *anmSpawner;
};

typedef char BackgroundSpellStateAt175C[
    (offsetof(BackgroundStateView, spellBackgroundState) == 0x175c) ? 1 : -1];
typedef char BackgroundPhotoAreaAt1764[
    (offsetof(BackgroundStateView, photoAreaActive) == 0x1764) ? 1 : -1];
typedef char BackgroundPhotoVmsAt1780[
    (offsetof(BackgroundStateView, photoAreaVms) == 0x1780) ? 1 : -1];
typedef char BackgroundSpellVmsAt1FE4[
    (offsetof(BackgroundStateView, spellBackgroundVms) == 0x1fe4) ? 1 : -1];

extern BackgroundStageStateView *g_BackgroundStageState;
extern BackgroundRuntimeView *g_BackgroundRuntime;

// FUNCTION: TH095 0x004048B0; TH08 0x00408FC0 is the adjacent source oracle.
#pragma var_order(weight3, weight1, weight2, weight0)
f32 __stdcall CubicHermiteInterpolate(
    f32 startValue, f32 endValue, f32 startTangent, f32 endTangent, f32 time)
{
    // VC7.1 allocates these backing identifiers in the target's local order.
    // The aliases retain the mathematical role at each use site.
    f32 weight0;
    f32 weight1;
    f32 weight2;
    f32 weight3;

#define startWeight weight1
#define endWeight weight3
#define startTangentWeight weight2
#define endTangentWeight weight0
    startWeight =
        (time - 1.0f) * (time - 1.0f) * (2.0f * time + 1.0f);
    endWeight = time * time * (3.0f - 2.0f * time);
    startTangentWeight = (1.0f - time) * (1.0f - time) * time;
    endTangentWeight = (time - 1.0f) * time * time;
    return startWeight * startValue + endWeight * endValue +
           startTangentWeight * startTangent +
           endTangentWeight * endTangent;
#undef endTangentWeight
#undef startTangentWeight
#undef endWeight
#undef startWeight
}

// FUNCTION: TH095 0x00404950.
void Background::SetPhotoArea(const Float3 *position, const Float3 *size)
{
    reinterpret_cast<BackgroundStateView *>(this)->photoAreaActive = 1;
    reinterpret_cast<BackgroundStateView *>(this)->photoAreaPosition = *position;
    reinterpret_cast<BackgroundStateView *>(this)->photoAreaSize = *size;
    if ((reinterpret_cast<BackgroundStateView *>(this)
             ->photoAreaVms[0].flagsWord & 1) == 0)
    {
        g_BackgroundStageState->anm->InitializeVm(
            &reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[0],
            0x25);
        g_BackgroundStageState->anm->InitializeVm(
            &reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[1],
            0x26);
        g_BackgroundStageState->anm->InitializeVm(
            &reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[2],
            0x27);
    }
}

// FUNCTION: TH095 0x00404A30.
void Background::StartSpellBackground()
{
    reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundState = 1;
    reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundVms[0] =
        g_BackgroundRuntime->anmSpawner->CreateVm(
            0, &Float3(0.0f, 0.0f, 0.0f));

    reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundVms[1] =
        g_BackgroundRuntime->anmSpawner->CreateVm(
            1, &Float3(0.0f, 0.0f, 0.0f));
}

// FUNCTION: TH095 0x00404AC0.
void Background::StopSpellBackground()
{
    reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundState = 0;
    g_AnmManager->MarkVmForDeletion(
        reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundVms[0]);
    g_AnmManager->MarkVmForDeletion(
        reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundVms[1]);
}

} // namespace th095
