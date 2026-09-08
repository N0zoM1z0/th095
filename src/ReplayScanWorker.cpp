#include "ReplayBrowser.hpp"

#include <process.h>

namespace th095
{

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
