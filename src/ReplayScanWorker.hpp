#pragma once

#include <stddef.h>

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
    void (__fastcall *threadProc)(void *);

    ReplayScanWorker();
    ~ReplayScanWorker();
    void Stop();
    void Start(void (__fastcall *callback)(void *), void *argument);
};

typedef char ReplayScanWorkerSizeIs18[
    (sizeof(ReplayScanWorker) == 0x18) ? 1 : -1];
typedef char ReplayScanWorkerThreadHandleAt00[
    (offsetof(ReplayScanWorker, threadHandle) == 0x00) ? 1 : -1];
typedef char ReplayScanWorkerExitSignalAt08[
    (offsetof(ReplayScanWorker, exitSignal) == 0x08) ? 1 : -1];
typedef char ReplayScanWorkerActiveAt0C[
    (offsetof(ReplayScanWorker, active) == 0x0c) ? 1 : -1];

} // namespace th095
