#include "ReplayBrowser.hpp"

#include <process.h>

namespace th095
{

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

ZunResult SceneSupervisorView::StartReplayScan(
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
