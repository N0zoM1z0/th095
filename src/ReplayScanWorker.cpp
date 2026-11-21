#include "ReplayScanWorker.hpp"
#include "Global.hpp"
#include "Main.hpp"

#ifdef TH095_MATCH_EXACT
#include "ReplayScanWorkerWin32Emission.hpp"
#else
#include "ReplayScanWorkerWin32.hpp"
#endif

namespace th095
{

#include "ReplayScanWorkerBehavior.inl"

i32 Supervisor::StartReplayScan(
    void (__fastcall *callback)(void *), void *argument)
{
    utils::DebugPrint("StartSubThread\n");
    this->EnterCriticalSectionWrapper(6);
    this->criticalSectionLockCounts[6]++;
    this->replayScanWorker.Start(callback, argument);
    this->LeaveCriticalSectionWrapper(6);
    this->criticalSectionLockCounts[6]--;
    return ZUN_SUCCESS;
}

void Supervisor::StopReplayScan()
{
    this->replayScanWorker.Stop();
}

} // namespace th095
