#pragma once

#include "AnmManager.hpp"
#include "Chain.hpp"
#include "ZunTimer.hpp"
#include "diffbuild.hpp"
#include "inttypes.hpp"

#include <stddef.h>

namespace th095
{

struct BackgroundStageHeader;
struct BackgroundStageObject;
struct BackgroundStageObjectInstance;
struct BackgroundStageInstruction;

typedef u16 BackgroundInterpolationMode;
typedef u8 BackgroundCameraMotionMode;

struct BackgroundPhotoBlend
{
    f32 nearDistance;
    f32 farDistance;
    ZunColor color;
};

// The target Background constructor does not run AnmVmId default constructors
// for these two slots, while StartSpellBackground copies AnmVmId return values
// into them.  This storage wrapper preserves both observed facts.
struct BackgroundVmId
{
    i32 value;
};

// Canonical TH095 Background allocation.  Create allocates exactly 0x201C
// bytes at 0x004024A0; the constructor at 0x004020C0 clears that same extent.
struct Background
{
    Background();
    ~Background();

    i32 Initialize();
    i32 Update();
    i32 UpdateStageObjectVms();
    i32 RunStageScript();
    i32 DrawHighPrio();
    i32 DrawLowPrio();
    i32 RenderObjects(i32 mode);
    i32 LoadStageData(const char *path);
    i32 LoadStageDataInner(const char *path);
    void SetPhotoArea(const Float3 *position, const Float3 *size);
    void StartSpellBackground();
    void StopSpellBackground();

    static i32 __fastcall OnUpdate(Background *background);
    static i32 __fastcall OnDrawHighPrio(Background *background);
    static i32 __fastcall OnDrawLowPrio(Background *background);
    static Background *Create();

    BackgroundStageHeader *stageData;                   // +0x0000
    BackgroundStageObject **stageObjects;                // +0x0004
    BackgroundStageObjectInstance *stageObjectInstances; // +0x0008
    u8 *stageScript;                                     // +0x000c
    ZunTimer stageScriptTimer;                           // +0x0010
    BackgroundStageInstruction *stageInstruction;       // +0x001c
    ZunTimer interpolationCurrentTimers[4];              // +0x0020
    ZunTimer interpolationEndTimers[4];                  // +0x0050
    BackgroundInterpolationMode interpolationModes[4]; // +0x0080
    Float3 cameraLookAtFinal;                           // +0x0088
    Float3 cameraLookAtInitial;                         // +0x0094
    Float3 cameraLookAtTangentFinal;                    // +0x00a0
    Float3 cameraLookAtTangentInitial;                  // +0x00ac
    Float3 cameraPositionFinal;                         // +0x00b8
    Float3 cameraPositionInitial;                       // +0x00c4
    Float3 cameraPositionTangentFinal;                  // +0x00d0
    Float3 cameraPositionTangentInitial;                // +0x00dc
    BackgroundCameraMotionMode cameraMotionMode;        // +0x00e8
    u8 unknown00e9[7];
    AnmLoaded *anm;                                     // +0x00f0
    AnmVm *stageObjectVms;                              // +0x00f4
    AnmVm stageVms[8];                                  // +0x00f8
    f32 cullingDistanceSq;                              // +0x1758
    i32 spellBackgroundFrameCounter;                   // +0x175c
    ZunColor photoColor;                                // +0x1760
    i32 photoAreaActive;                                // +0x1764
    Float3 photoAreaPosition;                           // +0x1768
    Float3 photoAreaSize;                               // +0x1774
    AnmVm photoAreaVms[3];                              // +0x1780
    BackgroundVmId spellBackgroundVmIds[2];            // +0x1fe4
    BackgroundPhotoBlend photoBlendCurrent;             // +0x1fec
    BackgroundPhotoBlend photoBlendInitial;             // +0x1ff8
    BackgroundPhotoBlend photoBlendFinal;               // +0x2004
    ChainElem *calcChain;                               // +0x2010
    ChainElem *drawHighChain;                           // +0x2014
    ChainElem *drawLowChain;                            // +0x2018
};

typedef char BackgroundStageScriptTimerAt10[
    (offsetof(Background, stageScriptTimer) == 0x10) ? 1 : -1];
typedef char BackgroundInterpolationModesAt80[
    (offsetof(Background, interpolationModes) == 0x80) ? 1 : -1];
typedef char BackgroundCameraMotionModeAtE8[
    (offsetof(Background, cameraMotionMode) == 0xe8) ? 1 : -1];
typedef char BackgroundStageVmsAtF8[
    (offsetof(Background, stageVms) == 0xf8) ? 1 : -1];
typedef char BackgroundCullingDistanceAt1758[
    (offsetof(Background, cullingDistanceSq) == 0x1758) ? 1 : -1];
typedef char BackgroundPhotoAreaVmsAt1780[
    (offsetof(Background, photoAreaVms) == 0x1780) ? 1 : -1];
typedef char BackgroundSpellVmIdsAt1FE4[
    (offsetof(Background, spellBackgroundVmIds) == 0x1fe4) ? 1 : -1];
typedef char BackgroundPhotoBlendAt1FEC[
    (offsetof(Background, photoBlendCurrent) == 0x1fec) ? 1 : -1];
typedef char BackgroundCalcChainAt2010[
    (offsetof(Background, calcChain) == 0x2010) ? 1 : -1];
typedef char BackgroundSizeIs201C[
    (sizeof(Background) == 0x201c) ? 1 : -1];

DIFFABLE_EXTERN(Background *, g_Background);
DIFFABLE_EXTERN_ARRAY(const char *, 9, g_StageEnemyAnms);
DIFFABLE_EXTERN_ARRAY(const char *, 17, g_SpellEnemyAnms);
DIFFABLE_EXTERN_ARRAY(const char *, 9, g_StageEclFiles);
DIFFABLE_EXTERN_ARRAY(const char *, 9, g_StageSpellEclFiles);
DIFFABLE_EXTERN_ARRAY(const char *, 17, g_SpellEclFiles);
DIFFABLE_EXTERN_ARRAY(const char *, 9, g_GuiStageTextAnmPaths);
DIFFABLE_EXTERN_ARRAY(const char *, 15, g_EffectAnms);
extern u8 *g_OwnedBackgroundStageDataCache;

} // namespace th095
