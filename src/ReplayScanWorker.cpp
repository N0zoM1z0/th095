#include "ReplayScanWorker.hpp"

#ifdef TH095_MATCH_EXACT
#include "ReplayScanWorkerExact.inl"
#else
#include "Main.hpp"
#include "Global.hpp"

#include "ReplayScanWorkerWin32.hpp"

namespace th095
{

#ifndef DIFFBUILD
// Historical completion/active aliases refer to the primary Supervisor worker,
// target 0x004C4CB8. Field-based binding follows its native layout as well.
i32 &g_HelpLoadComplete = g_Supervisor.replayScanWorker.exitSignal;
i32 &g_HelpLoadActive = g_Supervisor.replayScanWorker.active;
#endif

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
