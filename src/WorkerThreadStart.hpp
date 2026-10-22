#pragma once

#include "ReplayScanWorker.hpp"

namespace th095
{

// Normal/native launch transport. It owns a callback/argument snapshot until
// the CRT entry consumes it; this is not a target worker overlay or wire data.
struct WorkerThreadStart
{
    WorkerCallback callback;
    void *argument;
};

typedef unsigned (TH095_STDCALL *CrtWorkerEntry)(void *);

} // namespace th095
