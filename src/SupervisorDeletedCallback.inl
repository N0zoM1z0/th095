namespace th095
{
void __fastcall ReleaseFrontEndAnms();
}

// Keep the real version-data ownership local inside its teardown phase so
// VC7.1 allocates it with the target destructor chronology.
static __forceinline void ReleaseSupervisorVersionData()
{
    if (g_Supervisor.versionData != NULL)
    {
        void *versionData = g_Supervisor.versionData;
        free(versionData);
        g_Supervisor.versionData = NULL;
    }
}

// The ANM vertex-buffer owner has a separate lifetime from later shutdown
// delete expressions; a bounded inline helper preserves that live local phase.
static __forceinline void ReleaseSupervisorAnmVertexBuffer()
{
    AnmManager *anmManager = g_AnmManager;
    if (anmManager->quadVertexBuffer != NULL)
    {
        anmManager->quadVertexBuffer->Release();
        anmManager->quadVertexBuffer = NULL;
    }
}

// FUNCTION: TH095 0x004244D0.
i32 __fastcall Supervisor::DeletedCallback(void *arg)
{
    g_SupervisorInputWorker.Stop();
    g_SoundPlayer.RequestThreadStop();
    g_Supervisor.StopReplayScan();

    ReleaseSupervisorVersionData();

    ((Supervisor *)arg)->ReleaseGameManagers();
    ReleasePhotoBulletAnm();
    ReleasePhotoAnm();
    ResultScreen::ReleaseAnm();
    ReleasePhotoFrontAnm();
    ReleaseFrontEndAnms();
    ReleasePhotoPlayerAnm();
    ReleaseScoreData();

    ReleaseSupervisorAnmVertexBuffer();
    g_AnmManager->ReleaseAnm(0);
    g_AnmManager->ReleaseAnm(2);
    g_AnmManager->ReleaseSurface(SUPERVISOR_STARTUP_BACKGROUND_SURFACE);
    AsciiManager::CutChain();
    g_SoundPlayer.QueueCommand(SOUNDPLAYER_COMMAND_RELEASE_BGM, 0, "dummy");
    TextHelperView::ReleaseTextBuffer();

    if (((Supervisor *)arg)->keyboard != NULL)
    {
        utils::DebugPrint("DirectInput Release\n");
        ((Supervisor *)arg)->keyboard->Unacquire();
        if (((Supervisor *)arg)->keyboard != NULL)
        {
            ((Supervisor *)arg)->keyboard->Release();
            ((Supervisor *)arg)->keyboard = NULL;
        }
    }

    if (((Supervisor *)arg)->controllerDevices[0] != NULL)
    {
        utils::DebugPrint("DirectInput(Pad) Release\n");
        ((Supervisor *)arg)->controllerDevices[0]->Unacquire();
        if (((Supervisor *)arg)->controllerDevices[0] != NULL)
        {
            ((Supervisor *)arg)->controllerDevices[0]->Release();
            ((Supervisor *)arg)->controllerDevices[0] = NULL;
        }
    }

    if (((Supervisor *)arg)->directInput != NULL)
    {
        ((Supervisor *)arg)->directInput->Release();
        ((Supervisor *)arg)->directInput = NULL;
    }

    g_PbgArchive.Release();
    if (g_Supervisor.dummyMidiTimer != NULL)
    {
        g_Supervisor.dummyMidiTimer->StopTimer();
        delete g_Supervisor.dummyMidiTimer;
        g_Supervisor.dummyMidiTimer = NULL;
    }
    return 0;
}
