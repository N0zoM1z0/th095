#define TH095_DECLARE_ANM_LOADED_INITIALIZE_VM
#include "AnmManager.hpp"
#include "AnmVmId.hpp"

namespace th095
{

struct Background
{
    i32 RunStageScript();
    void SetPhotoArea(const Float3 *position, const Float3 *size);
    void StartSpellBackground();
    void StopSpellBackground();
};

struct BackgroundStageInstruction
{
    i32 time;
    i16 opcode;
    i16 size;
    i32 args[1];
};

struct BackgroundPhotoBlend
{
    f32 x;
    f32 y;
    ZunColor color;
};

struct BackgroundStateView
{
    u8 unknown0000[0x0c];
    u8 *stageScript;                            // +0x000c
    ZunTimer stageScriptTimer;                  // +0x0010
    BackgroundStageInstruction *stageInstruction; // +0x001c
    ZunTimer interpolationCurrentTimers[4];     // +0x0020
    ZunTimer interpolationEndTimers[4];         // +0x0050
    u16 interpolationModes[4];                  // +0x0080
    Float3 cameraLookAtFinal;                   // +0x0088
    Float3 cameraLookAtInitial;                 // +0x0094
    Float3 cameraLookAtTangentFinal;            // +0x00a0
    Float3 cameraLookAtTangentInitial;          // +0x00ac
    Float3 cameraPositionFinal;                 // +0x00b8
    Float3 cameraPositionInitial;               // +0x00c4
    Float3 cameraPositionTangentFinal;           // +0x00d0
    Float3 cameraPositionTangentInitial;         // +0x00dc
    u8 cameraMotionMode;                         // +0x00e8
    u8 unknown00e9[7];
    AnmLoaded *anm;                              // +0x00f0
    u8 unknown00f4[4];
    AnmVm stageVms[8];                           // +0x00f8
    u8 unknown1758[4];
    i32 spellBackgroundState;       // +0x175c
    ZunColor photoColor;            // +0x1760
    i32 photoAreaActive;            // +0x1764
    Float3 photoAreaPosition;       // +0x1768
    Float3 photoAreaSize;           // +0x1774
    AnmVm photoAreaVms[3];          // +0x1780
    AnmVmId spellBackgroundVms[2];  // +0x1fe4
    BackgroundPhotoBlend photoBlendCurrent; // +0x1fec
    BackgroundPhotoBlend photoBlendInitial; // +0x1ff8
    BackgroundPhotoBlend photoBlendFinal;   // +0x2004
    u8 unknown2010[0x0c];
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
typedef char BackgroundStageScriptAtC[
    (offsetof(BackgroundStateView, stageScript) == 0x0c) ? 1 : -1];
typedef char BackgroundStageInstructionAt1C[
    (offsetof(BackgroundStateView, stageInstruction) == 0x1c) ? 1 : -1];
typedef char BackgroundInterpolationModesAt80[
    (offsetof(BackgroundStateView, interpolationModes) == 0x80) ? 1 : -1];
typedef char BackgroundCameraMotionModeAtE8[
    (offsetof(BackgroundStateView, cameraMotionMode) == 0xe8) ? 1 : -1];
typedef char BackgroundAnmAtF0[
    (offsetof(BackgroundStateView, anm) == 0xf0) ? 1 : -1];
typedef char BackgroundStageVmsAtF8[
    (offsetof(BackgroundStateView, stageVms) == 0xf8) ? 1 : -1];
typedef char BackgroundPhotoAreaAt1764[
    (offsetof(BackgroundStateView, photoAreaActive) == 0x1764) ? 1 : -1];
typedef char BackgroundPhotoVmsAt1780[
    (offsetof(BackgroundStateView, photoAreaVms) == 0x1780) ? 1 : -1];
typedef char BackgroundSpellVmsAt1FE4[
    (offsetof(BackgroundStateView, spellBackgroundVms) == 0x1fe4) ? 1 : -1];
typedef char BackgroundPhotoBlendAt1FEC[
    (offsetof(BackgroundStateView, photoBlendCurrent) == 0x1fec) ? 1 : -1];
typedef char BackgroundStateSizeIs201C[
    (sizeof(BackgroundStateView) == 0x201c) ? 1 : -1];

extern BackgroundStageStateView *g_BackgroundStageState;
extern BackgroundRuntimeView *g_BackgroundRuntime;
extern Float3 g_BackgroundCameraPosition;
extern Float3 g_BackgroundCameraLookAt;
extern f32 g_BackgroundCameraValue0;
extern f32 g_BackgroundCameraValue1;
extern f32 g_BackgroundCameraValue2;
extern f32 g_BackgroundWaveX;
extern f32 g_BackgroundWaveY;
extern i32 g_BackgroundModeValue;
extern ZunColor g_PhotoScreenFadeColor;

f32 __stdcall CubicHermiteInterpolate(
    f32 startValue, f32 endValue, f32 startTangent, f32 endTangent, f32 time);

// FUNCTION: TH095 0x00403440. Variable-size stage-script interpreter and
// TH095-specific photograph-mask/camera interpolation owner.
i32 Background::RunStageScript()
{
    BackgroundStateView *background =
        reinterpret_cast<BackgroundStateView *>(this);
    BackgroundStageInstruction *instruction;
    i32 interpolationIndex;
    i32 colorIndex;
    f32 interpolationTime;
    f32 angle;

read_instruction:
    instruction = background->stageInstruction;
    if (background->stageScriptTimer >= instruction->time)
    {
        switch (instruction->opcode)
        {
        case 0:
            break;

        case 1:
            background->stageScriptTimer = instruction->args[1];
            background->stageInstruction =
                reinterpret_cast<BackgroundStageInstruction *>(
                    background->stageScript + instruction->args[0]);
            goto read_instruction;

        case 2:
            g_BackgroundCameraPosition =
                *reinterpret_cast<Float3 *>(&instruction->args[0]);
            break;

        case 3:
            background->interpolationCurrentTimers[0].Initialize();
            background->interpolationEndTimers[0] = instruction->args[0];
            background->interpolationModes[0] = (u16)instruction->args[1];
            background->cameraPositionInitial = g_BackgroundCameraPosition;
            background->cameraPositionFinal =
                *reinterpret_cast<Float3 *>(&instruction->args[2]);
            break;

        case 4:
            g_BackgroundCameraLookAt =
                *reinterpret_cast<Float3 *>(&instruction->args[0]);
            break;

        case 5:
            background->interpolationCurrentTimers[1].Initialize();
            background->interpolationEndTimers[1] = instruction->args[0];
            background->interpolationModes[1] = (u16)instruction->args[1];
            background->cameraLookAtInitial = g_BackgroundCameraLookAt;
            background->cameraLookAtFinal =
                *reinterpret_cast<Float3 *>(&instruction->args[2]);
            break;

        case 6:
            g_BackgroundCameraValue0 =
                *reinterpret_cast<f32 *>(&instruction->args[0]);
            g_BackgroundCameraValue1 =
                *reinterpret_cast<f32 *>(&instruction->args[1]);
            g_BackgroundCameraValue2 =
                *reinterpret_cast<f32 *>(&instruction->args[2]);
            break;

        case 7:
            g_BackgroundModeValue = instruction->args[0];
            break;

        case 8:
            background->photoBlendCurrent.color.color = instruction->args[0];
            background->photoBlendCurrent.x =
                *reinterpret_cast<f32 *>(&instruction->args[1]);
            background->photoBlendCurrent.y =
                *reinterpret_cast<f32 *>(&instruction->args[2]);
            background->photoBlendFinal = background->photoBlendCurrent;
            g_PhotoScreenFadeColor = background->photoBlendCurrent.color;
            break;

        case 9:
            background->interpolationCurrentTimers[2].Initialize();
            background->interpolationEndTimers[2] = instruction->args[0];
            background->interpolationModes[2] = (u16)instruction->args[1];
            background->photoBlendInitial = background->photoBlendCurrent;
            background->photoBlendFinal.color.color = instruction->args[2];
            background->photoBlendFinal.x =
                *reinterpret_cast<f32 *>(&instruction->args[3]);
            background->photoBlendFinal.y =
                *reinterpret_cast<f32 *>(&instruction->args[4]);
            break;

        case 10:
            background->interpolationCurrentTimers[0].Initialize();
            background->interpolationEndTimers[0] = instruction->args[0];
            background->interpolationModes[0] =
                (u16)instruction->args[1] | 0x800;
            background->cameraPositionInitial = g_BackgroundCameraPosition;
            background->cameraPositionTangentInitial =
                *reinterpret_cast<Float3 *>(&instruction->args[2]);
            background->cameraPositionFinal =
                *reinterpret_cast<Float3 *>(&instruction->args[5]);
            background->cameraPositionTangentFinal =
                *reinterpret_cast<Float3 *>(&instruction->args[8]);
            break;

        case 11:
            background->interpolationCurrentTimers[1].Initialize();
            background->interpolationEndTimers[1] = instruction->args[0];
            background->interpolationModes[1] =
                (u16)instruction->args[1] | 0x800;
            background->cameraLookAtInitial = g_BackgroundCameraLookAt;
            background->cameraLookAtTangentInitial =
                *reinterpret_cast<Float3 *>(&instruction->args[2]);
            background->cameraLookAtFinal =
                *reinterpret_cast<Float3 *>(&instruction->args[5]);
            background->cameraLookAtTangentFinal =
                *reinterpret_cast<Float3 *>(&instruction->args[8]);
            break;

        case 12:
            background->cameraMotionMode =
                *reinterpret_cast<u8 *>(&instruction->args[0]);
            break;

        case 13:
            g_PhotoScreenFadeColor.color = instruction->args[0];
            break;

        case 14:
            if (instruction->args[1] < 0)
            {
                background->stageVms[instruction->args[0]].flagsWord &= ~1;
            }
            else
            {
                background->anm->InitializeVm(
                    &background->stageVms[instruction->args[0]],
                    instruction->args[1]);
            }
            break;
        }

        background->stageInstruction =
            reinterpret_cast<BackgroundStageInstruction *>(
                reinterpret_cast<u8 *>(instruction) + instruction->size);
        goto read_instruction;
    }

    background->stageScriptTimer.Tick();

    for (interpolationIndex = 0; interpolationIndex < 4;
         interpolationIndex++)
    {
        if (background->interpolationEndTimers[interpolationIndex].current > 0)
        {
            background->interpolationCurrentTimers[interpolationIndex].Tick();
            if (background->interpolationCurrentTimers[interpolationIndex] <
                background->interpolationEndTimers[interpolationIndex].current)
            {
                interpolationTime =
                    background->interpolationCurrentTimers[interpolationIndex]
                        .subFrame /
                    background->interpolationEndTimers[interpolationIndex]
                        .subFrame;
            }
            else
            {
                interpolationTime = 1.0f;
                background->interpolationEndTimers[interpolationIndex]
                    .Initialize();
            }

            switch (background->interpolationModes[interpolationIndex] & 0xff)
            {
            case 1:
                interpolationTime *= interpolationTime;
                break;
            case 2:
                interpolationTime *= interpolationTime * interpolationTime;
                break;
            case 3:
                interpolationTime *= interpolationTime * interpolationTime *
                                     interpolationTime;
                break;
            case 4:
                interpolationTime = 1.0f - interpolationTime;
                interpolationTime =
                    1.0f - interpolationTime * interpolationTime;
                break;
            case 5:
                interpolationTime = 1.0f - interpolationTime;
                interpolationTime =
                    1.0f - interpolationTime * interpolationTime *
                               interpolationTime;
                break;
            case 6:
                interpolationTime = 1.0f - interpolationTime;
                interpolationTime *= interpolationTime;
                interpolationTime =
                    1.0f - interpolationTime * interpolationTime;
                break;
            }

            if ((background->interpolationModes[interpolationIndex] >> 8) == 0)
            {
                if (interpolationIndex == 0)
                {
                    g_BackgroundCameraPosition =
                        (background->cameraPositionFinal -
                         background->cameraPositionInitial) *
                            interpolationTime +
                        background->cameraPositionInitial;
                }
                else if (interpolationIndex == 1)
                {
                    g_BackgroundCameraLookAt =
                        (background->cameraLookAtFinal -
                         background->cameraLookAtInitial) *
                            interpolationTime +
                        background->cameraLookAtInitial;
                }
                else if (interpolationIndex == 2)
                {
                    for (colorIndex = 0; colorIndex < 4; colorIndex++)
                    {
                        reinterpret_cast<u8 *>(
                            &background->photoBlendCurrent.color)[colorIndex] =
                            (u8)(((f32)reinterpret_cast<u8 *>(
                                      &background->photoBlendFinal.color)
                                      [colorIndex] -
                                  (f32)reinterpret_cast<u8 *>(
                                      &background->photoBlendInitial.color)
                                      [colorIndex]) *
                                     interpolationTime +
                                 (f32)reinterpret_cast<u8 *>(
                                     &background->photoBlendInitial.color)
                                     [colorIndex]);
                    }
                    background->photoBlendCurrent.x =
                        (background->photoBlendFinal.x -
                         background->photoBlendInitial.x) *
                            interpolationTime +
                        background->photoBlendInitial.x;
                    background->photoBlendCurrent.y =
                        (background->photoBlendFinal.y -
                         background->photoBlendInitial.y) *
                            interpolationTime +
                        background->photoBlendInitial.y;
                    g_PhotoScreenFadeColor =
                        background->photoBlendCurrent.color;
                }
            }
            else if (interpolationIndex == 0)
            {
                g_BackgroundCameraPosition.x = CubicHermiteInterpolate(
                    background->cameraPositionInitial.x,
                    background->cameraPositionFinal.x,
                    background->cameraPositionTangentInitial.x,
                    background->cameraPositionTangentFinal.x,
                    interpolationTime);
                g_BackgroundCameraPosition.y = CubicHermiteInterpolate(
                    background->cameraPositionInitial.y,
                    background->cameraPositionFinal.y,
                    background->cameraPositionTangentInitial.y,
                    background->cameraPositionTangentFinal.y,
                    interpolationTime);
                g_BackgroundCameraPosition.z = CubicHermiteInterpolate(
                    background->cameraPositionInitial.z,
                    background->cameraPositionFinal.z,
                    background->cameraPositionTangentInitial.z,
                    background->cameraPositionTangentFinal.z,
                    interpolationTime);
            }
            else if (interpolationIndex == 1)
            {
                g_BackgroundCameraLookAt.x = CubicHermiteInterpolate(
                    background->cameraLookAtInitial.x,
                    background->cameraLookAtFinal.x,
                    background->cameraLookAtTangentInitial.x,
                    background->cameraLookAtTangentFinal.x,
                    interpolationTime);
                g_BackgroundCameraLookAt.y = CubicHermiteInterpolate(
                    background->cameraLookAtInitial.y,
                    background->cameraLookAtFinal.y,
                    background->cameraLookAtTangentInitial.y,
                    background->cameraLookAtTangentFinal.y,
                    interpolationTime);
                g_BackgroundCameraLookAt.z = CubicHermiteInterpolate(
                    background->cameraLookAtInitial.z,
                    background->cameraLookAtFinal.z,
                    background->cameraLookAtTangentInitial.z,
                    background->cameraLookAtTangentFinal.z,
                    interpolationTime);
            }
        }
    }

    if (background->cameraMotionMode != 0)
    {
        switch (background->cameraMotionMode)
        {
        case 1:
            angle = (f32)background->interpolationCurrentTimers[3] *
                        3.1415927f * 2.0f / 480.0f -
                    3.1415927f;
            g_BackgroundWaveX = sinf(angle) * 40.0f;
            background->interpolationCurrentTimers[3].Tick();
            if (background->interpolationCurrentTimers[3] >= 480)
                background->interpolationCurrentTimers[3].Initialize();
            break;

        case 2:
            angle = (f32)background->interpolationCurrentTimers[3] *
                        3.1415927f * 2.0f / 480.0f -
                    3.1415927f;
            g_BackgroundWaveX = sinf(angle) * 70.0f;
            g_BackgroundCameraValue0 = -sinf(angle) * 0.1f;
            background->interpolationCurrentTimers[3].Tick();
            if (background->interpolationCurrentTimers[3] >= 480)
                background->interpolationCurrentTimers[3].Initialize();
            break;

        case 3:
            angle = (f32)background->interpolationCurrentTimers[3] *
                        3.1415927f * 2.0f / 2048.0f -
                    3.1415927f;
            g_BackgroundWaveX = sinf(angle) * 30.0f;
            g_BackgroundWaveY = cosf(angle) * 30.0f;
            g_BackgroundCameraValue0 = -sinf(angle) * 0.1f;
            background->interpolationCurrentTimers[3].Tick();
            if (background->interpolationCurrentTimers[3] >= 2048)
                background->interpolationCurrentTimers[3].Initialize();
            break;

        case 4:
            angle = (f32)background->interpolationCurrentTimers[3] *
                        3.1415927f * 2.0f / 4800.0f -
                    3.1415927f;
            g_BackgroundCameraValue0 = sinf(angle) * 1.0f;
            g_BackgroundCameraValue2 = cosf(angle) * 1.0f;
            background->interpolationCurrentTimers[3].Tick();
            if (background->interpolationCurrentTimers[3] >= 4800)
                background->interpolationCurrentTimers[3].Initialize();
            break;
        }
    }

    return 0;
}

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
