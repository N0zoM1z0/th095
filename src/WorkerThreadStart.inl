// Included inside namespace th095 after the allocation/begin API bindings.
static unsigned TH095_STDCALL RunWorkerThread(void *argument)
{
    WorkerThreadStart *start = static_cast<WorkerThreadStart *>(argument);
    WorkerCallback callback = start->callback;
    void *userArgument = start->argument;
    // Snapshot both fields and release the transport before invoking user code.
    ReleaseWorkerThreadStart(start);
    callback(userArgument);
    // The void callback has no result contract. This is the adapter's defined
    // CRT completion status, not the worker exit handshake or a recovered value.
    return 0;
}

static inline uintptr_t BeginWorkerThread(ReplayScanWorker *worker, void *argument)
{
    // Preserve the CRT's null-entry validation/error state without allocation.
    if (worker->threadProc == NULL)
        return BeginBoundWorkerThread(NULL, argument, &worker->threadId);
    WorkerThreadStart *start = AllocateWorkerThreadStart();
    if (start == NULL)
        return 0;
    start->callback = worker->threadProc;
    start->argument = argument;
    uintptr_t handle = BeginBoundWorkerThread(RunWorkerThread, start, &worker->threadId);
    // On success the entry may already have released start before Begin returns.
    // A failed begin starts no callback, so the creator retains release ownership.
    if (handle == 0)
        ReleaseWorkerThreadStart(start);
    return handle;
}
