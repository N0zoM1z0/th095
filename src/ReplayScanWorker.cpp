#include "ReplayScanWorker.hpp"

#ifdef TH095_MATCH_EXACT
#include "ReplayScanWorkerExact.inl"
#else
#include "ReplayBrowser.hpp"

#include "ReplayScanWorkerWin32.hpp"

namespace th095
{

#ifndef DIFFBUILD
// Hash-attested target xrefs place every help/front-end/replay completion write
// at 0x004C4CC0/0x004C4CC4, which are +0x08/+0x0c in the Supervisor worker
// rooted at 0x004C4CB8. ReplayBrowserExitSignal::Request likewise writes +8.
// Earlier runnable builds accidentally allocated three duplicate globals, so
// Start()/Stop() and their callbacks observed different state. Keep the exact
// probe symbols unchanged, but overlay all production views on the real owner.
i32 &g_HelpLoadComplete = g_Supervisor.replayScanWorker.exitSignal;
i32 &g_HelpLoadActive = g_Supervisor.replayScanWorker.active;
ReplayBrowserExitSignal &g_ReplayBrowserExitSignal =
    *reinterpret_cast<ReplayBrowserExitSignal *>(
        &g_Supervisor.replayScanWorker);
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
