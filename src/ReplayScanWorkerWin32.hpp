#pragma once

#include "ReplayScanWorker.hpp"
#include <process.h>

namespace th095
{

// Target 0x0041BBDF..0x0041BBE4 passes the stored callback directly to the
// CRT's unsigned/stdcall entry slot. Keep that historical conversion inside
// the Win32 API binding; it is not a portable callback-dispatch contract.
// A minimal pinned-3077 probe proves this typed function inlines without
// changing Start's 87 bytes or its two relocation positions/destinations.
static __forceinline uintptr_t BeginWorkerThread(ReplayScanWorker *worker,
                                                void *argument)
{
    return _beginthreadex(
        NULL, 0, (unsigned (TH095_STDCALL *)(void *))worker->threadProc,
        argument, 0, &worker->threadId);
}

} // namespace th095
