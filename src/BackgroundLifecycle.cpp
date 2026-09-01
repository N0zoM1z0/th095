#include "inttypes.hpp"
#include "utils.hpp"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

namespace th095
{

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

struct AnmLoaded;
struct AnmVm
{
    u8 bytes[0x2cc];
    AnmVm();
    ~AnmVm();
};

struct ChainElem;
struct Chain
{
    void Cut(ChainElem *elem);
};
struct AnmManager
{
    void ReleaseAnm(i32 anmIdx);
    void MarkVmsForDeletion(AnmLoaded *anmFile);
};
struct BackgroundSupervisorFlagsView
{
    u32 unknown00 : 9;
    u32 disableResourceReload : 1;
    u32 unknown10 : 22;
};
struct Supervisor
{
    u8 unknown000[0x444];
    u32 flags;
};

extern Chain g_Chain;
extern AnmManager *g_AnmManager;
extern Supervisor g_Supervisor;

// Bounded lifecycle-only view. Background.cpp intentionally keeps a storage
// view for the TH095-specific runtime fields; this TU gives VC7 ownership of the
// real non-trivial members and therefore the target vector ctor/dtor iterators.
struct Background
{
    void *stageData;
    u8 unknown004[0x0c];
    ZunTimer stageScriptTimer;
    u8 unknown01c[4];
    ZunTimer interpolationCurrentTimers[4];
    ZunTimer interpolationEndTimers[4];
    u8 unknown080[0x70];
    AnmLoaded *anm;
    AnmVm *stageObjectVms;
    AnmVm stageVms[8];
    u8 unknown1758[0x28];
    AnmVm photoAreaVms[3];
    u8 unknown1fe4[0x2c];
    ChainElem *calcChain;
    ChainElem *drawHighChain;
    ChainElem *drawLowChain;

    Background();
    ~Background();
};

extern Background *g_Background;
extern u8 *g_BackgroundStageDataCache;

typedef char BackgroundLifecycleTimerAt10[
    (offsetof(Background, stageScriptTimer) == 0x10) ? 1 : -1];
typedef char BackgroundLifecycleTimersAt20[
    (offsetof(Background, interpolationCurrentTimers) == 0x20) ? 1 : -1];
typedef char BackgroundLifecycleTimersAt50[
    (offsetof(Background, interpolationEndTimers) == 0x50) ? 1 : -1];
typedef char BackgroundLifecycleAnmAtF0[
    (offsetof(Background, anm) == 0xf0) ? 1 : -1];
typedef char BackgroundLifecycleStageObjectVmsAtF4[
    (offsetof(Background, stageObjectVms) == 0xf4) ? 1 : -1];
typedef char BackgroundLifecycleStageVmsAtF8[
    (offsetof(Background, stageVms) == 0xf8) ? 1 : -1];
typedef char BackgroundLifecyclePhotoVmsAt1780[
    (offsetof(Background, photoAreaVms) == 0x1780) ? 1 : -1];
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
            &g_Supervisor.flags)->disableResourceReload == 0)
    {
        if (g_BackgroundStageDataCache != NULL)
            FreeBackgroundOwned(g_BackgroundStageDataCache);
        g_BackgroundStageDataCache = NULL;
    }

    if (stageObjectVms != NULL)
        FreeBackgroundOwned(stageObjectVms);

    if (reinterpret_cast<BackgroundSupervisorFlagsView *>(
            &g_Supervisor.flags)->disableResourceReload != 0)
        g_AnmManager->MarkVmsForDeletion(anm);
    else
        g_AnmManager->ReleaseAnm(4);

    g_Background = NULL;
}

} // namespace th095
