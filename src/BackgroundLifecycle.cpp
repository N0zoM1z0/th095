#include "Chain.hpp"
#ifndef TH095_MATCH_EXACT
#include "ZunTimer.hpp"
#endif
#include "inttypes.hpp"
#include "utils.hpp"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

namespace th095
{

#ifdef TH095_MATCH_EXACT
// This lifecycle TU was independently matched with a bounded timer view. The
// target distinguishes member construction order from the post-memset reset:
// ctor writes current/previous/subFrame, while Initialize writes
// current/subFrame/previous. Production uses the shared ZunTimer definition.
struct ZunTimer
{
    i32 previous;
    f32 subFrame;
    i32 current;

    ZunTimer()
    {
        current = 0;
        previous = -999999;
        subFrame = 0.0f;
    }

    void Initialize()
    {
        current = 0;
        subFrame = 0.0f;
        previous = -999999;
    }
};
#endif

struct AnmLoaded;
struct AnmVm
{
    u8 bytes[0x2cc];
    AnmVm();
    ~AnmVm();
};

struct AnmManager
{
    void ReleaseAnm(i32 anmIdx);
    void MarkVmsForDeletion(AnmLoaded *anmFile);
};
struct BackgroundSupervisorFlagsView
{
    u32 unknown00 : 9;
#if defined(TH095_MATCH_EXACT)
    u32 disableResourceReload : 1;
#else
    u32 resultRestartActive : 1;
#endif
    u32 unknown10 : 22;
};
struct Supervisor
{
    u8 unknown000[0x444];
    u32 flags;
};

extern AnmManager *g_AnmManager;
extern Supervisor g_Supervisor;

// Bounded lifecycle-only view. Background.cpp intentionally keeps a storage
// view for the TH095-specific runtime fields; this TU gives VC7 ownership of the
// real non-trivial members and therefore the target vector ctor/dtor iterators.
struct BackgroundStageHeader;
struct BackgroundStageObject;
struct BackgroundStageObjectInstance;
struct BackgroundStageInstruction;
#ifndef TH095_MATCH_EXACT
// These POD projections mirror the already-proven Background.cpp runtime view
// without adding Float3/AnmVmId constructors to the real lifecycle object.
struct BackgroundLifecycleFloat3
{
    f32 x;
    f32 y;
    f32 z;
};
struct BackgroundLifecyclePhotoBlend
{
    f32 x;
    f32 y;
    u32 color;
};
#endif
struct Background
{
#ifdef TH095_MATCH_EXACT
    void *stageData;
    u8 unknown004[0x0c];
#else
    BackgroundStageHeader *stageData;
    BackgroundStageObject **stageObjects;
    BackgroundStageObjectInstance *stageObjectInstances;
    u8 *stageScript;
#endif
    ZunTimer stageScriptTimer;
    BackgroundStageInstruction *stageInstruction;
    ZunTimer interpolationCurrentTimers[4];
    ZunTimer interpolationEndTimers[4];
#ifdef TH095_MATCH_EXACT
    u8 unknown080[0x70];
#else
    u16 interpolationModes[4];
    BackgroundLifecycleFloat3 cameraLookAtFinal;
    BackgroundLifecycleFloat3 cameraLookAtInitial;
    BackgroundLifecycleFloat3 cameraLookAtTangentFinal;
    BackgroundLifecycleFloat3 cameraLookAtTangentInitial;
    BackgroundLifecycleFloat3 cameraPositionFinal;
    BackgroundLifecycleFloat3 cameraPositionInitial;
    BackgroundLifecycleFloat3 cameraPositionTangentFinal;
    BackgroundLifecycleFloat3 cameraPositionTangentInitial;
    u8 cameraMotionMode;
    u8 unknown00e9[7];
#endif
    AnmLoaded *anm;
    AnmVm *stageObjectVms;
    AnmVm stageVms[8];
#ifdef TH095_MATCH_EXACT
    u8 unknown1758[0x28];
#else
    f32 cullingDistanceSq;
    i32 spellBackgroundFrameCounter;
    u32 photoColor;
    i32 photoAreaActive;
    BackgroundLifecycleFloat3 photoAreaPosition;
    BackgroundLifecycleFloat3 photoAreaSize;
#endif
    AnmVm photoAreaVms[3];
#ifdef TH095_MATCH_EXACT
    u8 unknown1fe4[0x2c];
#else
    i32 spellBackgroundVms[2];
    BackgroundLifecyclePhotoBlend photoBlendCurrent;
    BackgroundLifecyclePhotoBlend photoBlendInitial;
    BackgroundLifecyclePhotoBlend photoBlendFinal;
#endif
    ChainElem *calcChain;
    ChainElem *drawHighChain;
    ChainElem *drawLowChain;

    Background();
    ~Background();
};

extern Background *g_Background;
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
extern u8 *g_BackgroundStageDataCache;
#define TH095_BACKGROUND_STAGE_DATA_CACHE g_BackgroundStageDataCache
#else
extern u8 *g_OwnedBackgroundStageDataCache;
#define TH095_BACKGROUND_STAGE_DATA_CACHE g_OwnedBackgroundStageDataCache
#endif

#ifndef TH095_MATCH_EXACT
typedef char BackgroundLifecycleStageObjectsAt4[
    (offsetof(Background, stageObjects) == 0x04) ? 1 : -1];
typedef char BackgroundLifecycleStageObjectInstancesAt8[
    (offsetof(Background, stageObjectInstances) == 0x08) ? 1 : -1];
typedef char BackgroundLifecycleStageScriptAtC[
    (offsetof(Background, stageScript) == 0x0c) ? 1 : -1];
#endif
typedef char BackgroundLifecycleTimerAt10[
    (offsetof(Background, stageScriptTimer) == 0x10) ? 1 : -1];
typedef char BackgroundLifecycleTimersAt20[
    (offsetof(Background, interpolationCurrentTimers) == 0x20) ? 1 : -1];
typedef char BackgroundLifecycleTimersAt50[
    (offsetof(Background, interpolationEndTimers) == 0x50) ? 1 : -1];
#ifndef TH095_MATCH_EXACT
typedef char BackgroundLifecycleInterpolationModesAt80[
    (offsetof(Background, interpolationModes) == 0x80) ? 1 : -1];
typedef char BackgroundLifecycleCameraMotionModeAtE8[
    (offsetof(Background, cameraMotionMode) == 0xe8) ? 1 : -1];
#endif
typedef char BackgroundLifecycleAnmAtF0[
    (offsetof(Background, anm) == 0xf0) ? 1 : -1];
typedef char BackgroundLifecycleStageObjectVmsAtF4[
    (offsetof(Background, stageObjectVms) == 0xf4) ? 1 : -1];
typedef char BackgroundLifecycleStageVmsAtF8[
    (offsetof(Background, stageVms) == 0xf8) ? 1 : -1];
#ifndef TH095_MATCH_EXACT
typedef char BackgroundLifecycleCullingDistanceAt1758[
    (offsetof(Background, cullingDistanceSq) == 0x1758) ? 1 : -1];
typedef char BackgroundLifecycleSpellFrameCounterAt175C[
    (offsetof(Background, spellBackgroundFrameCounter) == 0x175c) ? 1 : -1];
typedef char BackgroundLifecyclePhotoAreaAt1764[
    (offsetof(Background, photoAreaActive) == 0x1764) ? 1 : -1];
#endif
typedef char BackgroundLifecyclePhotoVmsAt1780[
    (offsetof(Background, photoAreaVms) == 0x1780) ? 1 : -1];
#ifndef TH095_MATCH_EXACT
typedef char BackgroundLifecycleSpellVmsAt1FE4[
    (offsetof(Background, spellBackgroundVms) == 0x1fe4) ? 1 : -1];
typedef char BackgroundLifecyclePhotoBlendAt1FEC[
    (offsetof(Background, photoBlendCurrent) == 0x1fec) ? 1 : -1];
typedef char BackgroundLifecyclePhotoBlendFinalAt2004[
    (offsetof(Background, photoBlendFinal) == 0x2004) ? 1 : -1];
#endif
typedef char BackgroundLifecycleChainsAt2010[
    (offsetof(Background, calcChain) == 0x2010) ? 1 : -1];
typedef char BackgroundLifecycleSizeIs201C[
    (sizeof(Background) == 0x201c) ? 1 : -1];

static __forceinline void FreeBackgroundOwned(void *owned)
{
    free(owned);
}

Background::Background()
{
    utils::DebugPrint("initialize BackGroundInf\n");
    memset(this, 0, sizeof(*this));
    stageScriptTimer.Initialize();
    g_Background = this;
}

Background::~Background()
{
    utils::DebugPrint("shutdown BackGroundInf\n");
    g_Chain.Cut(calcChain);
    g_Chain.Cut(drawHighChain);
    g_Chain.Cut(drawLowChain);

    if (stageData != NULL)
        FreeBackgroundOwned(stageData);

    if (reinterpret_cast<BackgroundSupervisorFlagsView *>(
            &g_Supervisor.flags)->
#if defined(TH095_MATCH_EXACT)
            disableResourceReload
#else
            resultRestartActive
#endif
            == 0)
    {
        if (TH095_BACKGROUND_STAGE_DATA_CACHE != NULL)
            FreeBackgroundOwned(TH095_BACKGROUND_STAGE_DATA_CACHE);
        TH095_BACKGROUND_STAGE_DATA_CACHE = NULL;
    }

    if (stageObjectVms != NULL)
        FreeBackgroundOwned(stageObjectVms);

    if (reinterpret_cast<BackgroundSupervisorFlagsView *>(
            &g_Supervisor.flags)->
#if defined(TH095_MATCH_EXACT)
            disableResourceReload
#else
            resultRestartActive
#endif
            != 0)
        g_AnmManager->MarkVmsForDeletion(anm);
    else
        g_AnmManager->ReleaseAnm(4);

    g_Background = NULL;
}

} // namespace th095
