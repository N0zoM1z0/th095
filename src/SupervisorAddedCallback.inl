// FUNCTION: TH095 0x00423E70.
i32 __fastcall Supervisor::AddedCallback(Supervisor *s)
{
    g_AnmGameSpeed = 1.0f;
    g_Supervisor.InitializeViewports();
    g_Supervisor.totalPlayTime = timeGetTime();
    g_Rng.seed = (u16)g_Supervisor.totalPlayTime;
    g_AnmAlternateRng.seed = (u16)g_Supervisor.totalPlayTime;

    if (Supervisor::LoadDat() != 0)
    {
        return -1;
    }

    g_Supervisor.backbufferClearColor = 0xff000000;
    InitializeScoreData();
    g_AnmManager->LoadSurface(SUPERVISOR_STARTUP_BACKGROUND_SURFACE, "title/th08logo.jpg");
    g_Supervisor.suppressFpsDisplay = 1;

    if (!g_Supervisor.disableVsync && Supervisor::CheckFps() != 0)
    {
        g_AnmManager->ReleaseSurface(0);
        return -2;
    }

    StartSoundLoadThread();
    Supervisor::StartInputWorker();
    s->loadingAnm = g_AnmManager->LoadAnm(2, "nowloading.anm");
    if (s->loadingAnm == NULL)
    {
        g_AnmManager->ReleaseSurface(0);
        return -1;
    }

    g_AnmManager->SetupVertexBuffer();
    TextHelperView::CreateTextBuffer();

    Float3 position(500.0f, 440.0f, 0.0f);
    g_Supervisor.SetupLoadingVms(&position);
    g_Supervisor.startupThreadState = SUPERVISOR_STARTUP_PHASE_RUNNING;
    g_Supervisor.StartReplayScan(
        Supervisor::StartupThread, s);
    return 0;
}
