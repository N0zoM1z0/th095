#include "Background.hpp"
#include "utils.hpp"

#include <stdlib.h>
#include <string.h>

namespace th095
{

struct BackgroundSupervisorFlagsView
{
    u32 unknown00 : 9;
    u32 resourceReloadDisabled : 1;
    u32 unknown10 : 22;
};
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
extern u8 *g_BackgroundStageDataCache;
#define TH095_BACKGROUND_STAGE_DATA_CACHE g_BackgroundStageDataCache
#else
extern u8 *g_OwnedBackgroundStageDataCache;
#define TH095_BACKGROUND_STAGE_DATA_CACHE g_OwnedBackgroundStageDataCache
#endif

static __forceinline void FreeBackgroundOwned(void *owned)
{
    free(owned);
}

// VC7 preserves this post-memset reset as a distinct source-shape from the
// ordinary ZunTimer initialization used by other translation units.
static __forceinline void InitializeBackgroundTimerAfterClear(ZunTimer *timer)
{
    timer->current = 0;
    timer->subFrame = 0.0f;
    timer->previous = -999999;
}

Background::Background()
{
    utils::DebugPrint("initialize BackGroundInf\n");
    memset(this, 0, sizeof(*this));
    InitializeBackgroundTimerAfterClear(&stageScriptTimer);
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
            &g_Supervisor.flags)->resourceReloadDisabled == 0)
    {
        if (TH095_BACKGROUND_STAGE_DATA_CACHE != NULL)
            FreeBackgroundOwned(TH095_BACKGROUND_STAGE_DATA_CACHE);
        TH095_BACKGROUND_STAGE_DATA_CACHE = NULL;
    }

    if (stageObjectVms != NULL)
        FreeBackgroundOwned(stageObjectVms);

    if (reinterpret_cast<BackgroundSupervisorFlagsView *>(
            &g_Supervisor.flags)->resourceReloadDisabled != 0)
        g_AnmManager->MarkVmsForDeletion(anm);
    else
        g_AnmManager->ReleaseAnm(4);

    g_Background = NULL;
}

} // namespace th095
