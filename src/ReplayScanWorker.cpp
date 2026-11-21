#include "ReplayScanWorker.hpp"
#include "Global.hpp"
#include "Main.hpp"

#ifdef TH095_MATCH_EXACT
#include "ReplayScanWorkerExact.inl"
#else

#include "ReplayScanWorkerWin32.hpp"

namespace th095
{

#include "ReplayScanWorkerBehavior.inl"

} // namespace th095

#endif // TH095_MATCH_EXACT

namespace th095
{

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
