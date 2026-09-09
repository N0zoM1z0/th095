#ifdef TH095_MATCH_EXACT
#include "ReplayScanWorkerExact.inl"
#else
#include "ReplayBrowser.hpp"

#include <process.h>

namespace th095
{

#ifndef DIFFBUILD
typedef char ReplayScanWorkerStopAt08[
    (offsetof(ReplayScanWorker, stopRequested) == 0x08) ? 1 : -1];
typedef char ReplayScanWorkerActiveAt0C[
    (offsetof(ReplayScanWorker, active) == 0x0c) ? 1 : -1];

// Hash-attested target xrefs place every help/front-end/replay completion write
// at 0x004C4CC0/0x004C4CC4, which are +0x08/+0x0c in the Supervisor worker
// rooted at 0x004C4CB8. ReplayBrowserExitSignal::Request likewise writes +8.
// Earlier runnable builds accidentally allocated three duplicate globals, so
// Start()/Stop() and their callbacks observed different state. Keep the exact
// probe symbols unchanged, but overlay all production views on the real owner.
i32 &g_HelpLoadComplete = g_Supervisor.replayScanWorker.stopRequested;
i32 &g_HelpLoadActive = g_Supervisor.replayScanWorker.active;
ReplayBrowserExitSignal &g_ReplayBrowserExitSignal =
    *reinterpret_cast<ReplayBrowserExitSignal *>(
        &g_Supervisor.replayScanWorker);
#endif

ReplayScanWorker::ReplayScanWorker()
{
    // The target constructor initializes the four live synchronization fields.
    // Start() installs threadProc before use; unknown010 remains opaque.
    this->handle = NULL;
    this->threadId = 0;
    this->stopRequested = 0;
    this->active = 0;
}

ReplayScanWorker::~ReplayScanWorker()
{
    this->Stop();
}

void ReplayScanWorker::Stop()
{
    if (this->handle != 0)
    {
        this->stopRequested = 1;
        this->active = 0;
        while (WaitForSingleObject((HANDLE)this->handle, 200) == WAIT_TIMEOUT)
        {
            this->stopRequested = 1;
            this->active = 0;
            Sleep(1);
        }
        CloseHandle((HANDLE)this->handle);
        this->handle = 0;
        this->threadProc = NULL;
    }
}

void ReplayScanWorker::Start(void (__fastcall *callback)(void *),
                             void *argument)
{
    this->Stop();
    this->threadProc = callback;
    this->active = 1;
    this->stopRequested = 0;
    this->handle = _beginthreadex(
        NULL, 0, (unsigned (__stdcall *)(void *))this->threadProc,
        argument, 0, &this->threadId);
}

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
