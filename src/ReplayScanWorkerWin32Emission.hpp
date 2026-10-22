#pragma once

#include "ReplayScanWorker.hpp"
#include <process.h>

namespace th095
{

// Target 0x0041BBDF..0x0041BBE4 passes the stored callback directly to the
// CRT's unsigned/stdcall entry slot. Keep that historical conversion inside
// the Win32 API binding; it is not a portable callback-dispatch contract.
// SEM-336's minimal clean typed transport changes Start from target 87 bytes
// to 68 plus separate 168-byte launch / 61-byte CRT-entry functions under
// pinned VC7.1 build 3077. Its allocation, ownership and typed dispatch cannot
// reproduce the target's direct callback handoff. Keep this conversion only
// at the existing exact emission include route; normal code uses the typed
// WorkerThreadStart transport. No worker layout or new selector lives here.
static __forceinline uintptr_t BeginWorkerThread(ReplayScanWorker *worker,
                                                void *argument)
{
    return _beginthreadex(
        NULL, 0, (unsigned (TH095_STDCALL *)(void *))worker->threadProc,
        argument, 0, &worker->threadId);
}

} // namespace th095
