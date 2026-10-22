#pragma once

#include "WorkerThreadStart.hpp"
#include <process.h>
#include <new>

namespace th095
{

static inline WorkerThreadStart *AllocateWorkerThreadStart()
{
    return new (std::nothrow) WorkerThreadStart;
}

static inline void ReleaseWorkerThreadStart(WorkerThreadStart *start)
{
    delete start;
}

static inline uintptr_t BeginBoundWorkerThread(
    CrtWorkerEntry entry, void *argument, u32 *threadId)
{
    return _beginthreadex(NULL, 0, entry, argument, 0, threadId);
}

// The shared lifecycle is unchanged. Only the normal platform binding owns
// this allocated, typed CRT entry; the exact route retains target emission.
#include "WorkerThreadStart.inl"

} // namespace th095
