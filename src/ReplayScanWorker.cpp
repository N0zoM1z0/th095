#include "ReplayScanWorker.hpp"

#ifdef TH095_MATCH_EXACT
#include "ReplayScanWorkerExact.inl"
#else
#include "Main.hpp"
#include "Global.hpp"

#include "ReplayScanWorkerWin32.hpp"

namespace th095
{

#include "ReplayScanWorkerBehavior.inl"

i32 Supervisor::StartReplayScan(
    void (__fastcall *callback)(void *), void *argument)
{
    utils::DebugPrint(
        "FillBufferWithSound in HandleWaveStreamNotification\r\n");
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

#endif // TH095_MATCH_EXACT
