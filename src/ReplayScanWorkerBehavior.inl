// Included inside namespace th095 by both worker emission sites.
ReplayScanWorker::ReplayScanWorker()
{
    // The target constructor initializes only these four observed fields.
    // Start() installs threadProc before use; unknown010 remains opaque.
    this->threadHandle = 0;
    this->threadId = 0;
    this->exitSignal = 0;
    this->active = 0;
}

ReplayScanWorker::~ReplayScanWorker()
{
    this->Stop();
}

// Target 0x0041BB00: both scene and replay callers address the primary worker.
// Request only publishes exitSignal; it neither clears active nor waits/closes.
void ReplayScanWorker::RequestExit()
{
    this->exitSignal = 1;
}

void ReplayScanWorker::Stop()
{
    // A missing handle is a complete no-op, including after a failed launch.
    if (this->threadHandle != 0)
    {
        this->exitSignal = 1;
        this->active = 0;
        // Only WAIT_TIMEOUT repeats. The target also closes/clears after any
        // other status, including a wait failure; do not infer a successful join.
        while (WaitForSingleObject((HANDLE)this->threadHandle, 200) == WAIT_TIMEOUT)
        {
            this->exitSignal = 1;
            this->active = 0;
            Sleep(1);
        }
        CloseHandle((HANDLE)this->threadHandle);
        this->threadHandle = 0;
        this->threadProc = NULL;
    }
}

void ReplayScanWorker::Start(void (TH095_FASTCALL *callback)(void *),
                             void *argument)
{
    this->Stop();
    this->threadProc = callback;
    this->active = 1;
    this->exitSignal = 0;
    this->threadHandle = BeginWorkerThread(this, argument);
}
