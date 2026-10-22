#include "ReplayBrowser.hpp"

#include "ReplayScanWorkerWin32.hpp"

#define ZUN_SUCCESS TH095_LEGACY_ZUN_SUCCESS
#define ZUN_ERROR TH095_LEGACY_ZUN_ERROR

namespace th095
{

#include "ReplayScanWorkerBehavior.inl"

::ZunResult SceneSupervisorView::StartReplayScan(
    void (__fastcall *callback)(void *), void *argument)
{
    utils::DebugPrint(
        "FillBufferWithSound in HandleWaveStreamNotification\r\n");
    this->EnterCriticalSectionWrapper(6);
    this->lockCounts[6]++;
    this->replayScanWorker.Start(callback, argument);
    this->LeaveCriticalSectionWrapper(6);
    this->lockCounts[6]--;
    return ZUN_SUCCESS;
}

void SceneSupervisorView::StopReplayScan()
{
    this->replayScanWorker.Stop();
}

} // namespace th095
