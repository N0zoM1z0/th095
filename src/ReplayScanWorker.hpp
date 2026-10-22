#pragma once

#include "RuntimeAbi.hpp"

#include "inttypes.hpp"

namespace th095
{

// Shared worker protocol used by the standalone Supervisor input worker and
// the two distinct ReplayScanWorker instances embedded in Supervisor. This
// type owns their common layout and behavior, but not any of those storages.
struct ReplayScanWorker
{
    uintptr_t threadHandle;
    u32 threadId;
    i32 exitSignal;
    i32 active;
    u8 unknown010[4];
    void (TH095_FASTCALL *threadProc)(void *);

    ReplayScanWorker();
    ~ReplayScanWorker();
    void Stop();
    void Start(void (TH095_FASTCALL *callback)(void *), void *argument);
};

// Handle/callback storage follows native pointer width; the three scalar
// fields and the four opaque bytes retain their target widths. These relations
// give target size 0x18 and native x86-64 size 0x20 without a selected layout.
typedef char ReplayScanWorkerNativeLayout[
    (sizeof(i32) == 4 && sizeof(u32) == 4 &&
     sizeof(uintptr_t) == sizeof(void *) &&
     sizeof(ReplayScanWorker) == 2 * sizeof(uintptr_t) + 16 &&
     offsetof(ReplayScanWorker, threadHandle) == 0 &&
     offsetof(ReplayScanWorker, threadId) == sizeof(uintptr_t) &&
     offsetof(ReplayScanWorker, exitSignal) == sizeof(uintptr_t) + 4 &&
     offsetof(ReplayScanWorker, active) == sizeof(uintptr_t) + 8 &&
     offsetof(ReplayScanWorker, unknown010) == sizeof(uintptr_t) + 12 &&
     offsetof(ReplayScanWorker, threadProc) == sizeof(uintptr_t) + 16) ? 1 : -1];

} // namespace th095
