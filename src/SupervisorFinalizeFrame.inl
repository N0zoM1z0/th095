// Shared FPS frame-count producer; Main supplies the ANM/locking bindings.
// The receiver owns the count and lane-5 lock; the frameskip configuration is
// read from the global Supervisor, as in target 0x00423840.
i32 __fastcall Supervisor::FinalizeFrame(Supervisor *s)
{
    g_AnmManager->FlushVertexBuffer();
    s->EnterCriticalSectionWrapper(5);
    s->criticalSectionLockCounts[5]++;
    s->fpsFrameCount += g_Supervisor.config.frameskipConfig + 1;
    s->LeaveCriticalSectionWrapper(5);
    s->criticalSectionLockCounts[5]--;
    return 1;
}
