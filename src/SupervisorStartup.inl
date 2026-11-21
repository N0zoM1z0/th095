// Stock VC7.1 hashes local identifiers when assigning /Od stack homes. Keep
// the semantic fileSize spelling while using the target-proven shallow bucket.
#define fileSize averagedPanLocal12

// FUNCTION: TH095 0x00423FB0.
i32 Supervisor::LoadDat()
{
    i32 fileSize;
    char versionFileName[128];

    if (g_PbgArchive.Load("th095.dat"))
    {
        sprintf(versionFileName, "th095_%.4x%c.ver", 0x102, 'a');
        g_Supervisor.versionData =
            FileSystem::OpenFile(versionFileName, &fileSize, 0);
        g_Supervisor.versionDataSize = fileSize;
        if (g_Supervisor.versionData == NULL)
        {
            g_GameErrorContext.Fatal(
                "error : \x83\x66\x81\x5b\x83\x5e\x82\xcc\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93\x82\xaa\x88\xe1\x82\xa2\x82\xdc\x82\xb7\r\n");
            return -1;
        }
    }
    else
    {
        g_GameErrorContext.Fatal(
            "error : \x83\x66\x81\x5b\x83\x5e\x83\x74\x83\x40\x83\x43\x83\x8b\x82\xaa\x91\xb6\x8d\xdd\x82\xb5\x82\xdc\x82\xb9\x82\xf1\r\n");
        return -1;
    }
    return 0;
}
#undef fileSize

// Stock VC7.1 allocates locals through identifier hash chains. These scoped
// backing names reproduce the target order without adding inert locals.
#define frameIndex restartCommandProcessingLocal05
#define framesInWindow averagedPanLocal12
#define lastTime iLocal11
#define samples commandCursorLocal02
#define sampleCount soundIndexLocal01
#define currentTime jLocal00
#define deltaTime preloadBufferLocal03
#define fps bgmPathLocal18
#define averageIndex bgmFormatIndexLocal05
#define average reopenedBufferLocal01
#pragma var_order(frameIndex, framesInWindow, lastTime, samples, sampleCount, currentTime, deltaTime, fps,            \
                  averageIndex, average)
// FUNCTION: TH095 0x00424050.
i32 Supervisor::CheckFps()
{
    i32 frameIndex;
    i32 framesInWindow;
    f64 lastTime;
    f32 samples[30];
    i32 sampleCount;
    f64 currentTime;
    f64 deltaTime;
    f64 fps;
    i32 averageIndex;
    f32 average;

    frameIndex = 0;
    framesInWindow = 0;
    lastTime = 0.0;
    sampleCount = 0;
    lastTime = g_GameWindow.GetTimestamp();

    while (frameIndex < 600 && sampleCount < 8)
    {
        g_Supervisor.d3dDevice->BeginScene();
        g_AnmManager->CopySurfaceToBackbuffer(SUPERVISOR_STARTUP_BACKGROUND_SURFACE, 0, 0, 0, 0);
        g_Supervisor.d3dDevice->EndScene();
        if (g_Supervisor.d3dDevice->Present(NULL, NULL, NULL, NULL) < 0)
        {
            g_Supervisor.d3dDevice->Reset(&g_Supervisor.presentParameters);
        }

        frameIndex++;
        currentTime = g_GameWindow.GetTimestamp();
        framesInWindow++;
        deltaTime = currentTime - lastTime;
        if (deltaTime >= 0.7)
        {
            lastTime = currentTime;
            framesInWindow = 0;
        }
        else if (deltaTime >= 0.5)
        {
            fps = (f64)framesInWindow / deltaTime;
            if (fps >= 57.0)
            {
                samples[sampleCount] = (f32)fps;
                sampleCount++;
            }
            lastTime = currentTime;
            framesInWindow = 0;
        }
    }

    if (!g_Supervisor.config.options.disableVsync)
    {
        average = 0.0f;
        if (sampleCount >= 2)
        {
            for (averageIndex = 0; averageIndex < sampleCount; averageIndex++)
            {
                average += samples[averageIndex];
            }
            average /= averageIndex;
        }
        else
        {
            average = 1000.0f;
        }

        if (average >= 65.0f)
        {
            g_GameErrorContext.Log(
                "\x90\x82\x92\xbc\x93\xaf\x8a\xfa\x82\xaa\x8e\xe6\x82\xea\x82\xc4\x82\xc8"
                "\x82\xa2\x82\xa9\x81\x41\x83\x8a\x83\x74\x83\x8c\x83\x62\x83\x56\x83\x85"
                "\x83\x8c\x81\x5b\x83\x67\x82\xaa\x8d\x82\x82\xb7\x82\xac\x82\xdc\x82\xb7"
                "\x81\x42\r\n");
            g_GameErrorContext.Log(
                "\x8b\xad\x90\xa7\x82\x55\x82\x4f\x83\x74\x83\x8c\x81\x5b\x83\x80\x83\x82"
                "\x81\x5b\x83\x68\x82\xc5\x93\xae\x8d\xec\x82\xb5\x82\xdc\x82\xb7\r\n");
            g_Supervisor.disableVsync = 1;
            return -2;
        }
    }
    return 0;
}
#undef frameIndex
#undef framesInWindow
#undef lastTime
#undef samples
#undef sampleCount
#undef currentTime
#undef deltaTime
#undef fps
#undef averageIndex
#undef average

// FUNCTION: TH095 0x004242B0.
void __fastcall Supervisor::StartupThread(void *s)
{
    f32 volume;

    g_AnmGameSpeed = 1.0f;
    g_Supervisor.suppressFpsDisplay = 0;
    g_Supervisor.screenTransitionCountdown = 0;
    g_Supervisor.textAnm = g_AnmManager->PreloadAnm(0, "text.anm");
    if (g_Supervisor.textAnm == NULL)
    {
        goto error;
    }

    if (AsciiManager::RegisterChain() != 0)
    {
        g_GameErrorContext.Log(
            "error : \x95\xb6\x8e\x9a\x82\xcc\x8f\x89\x8a\xfa\x89\xbb\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\r\n");
        goto error;
    }

    if (g_SoundPlayer.LoadFmt("bgm/thbgm.fmt") != 0)
    {
        g_GameErrorContext.Log(
            "error : BGM \x82\xcc\x8f\x89\x8a\xfa\x89\xbb\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\r\n");
    }

    g_SoundPlayer.bgmVolume = g_Supervisor.config.musicVolume;
    g_SoundPlayer.sfxVolume = g_Supervisor.config.sfxVolume;
    volume = (f32)g_SoundPlayer.bgmVolume / 100.0f;
    if (g_SoundPlayer.sfxVolume != 0)
    {
        volume = 1.0f - volume;
        volume *= volume;
        volume *= volume;
        volume = 1.0f - volume;
        g_SoundPlayer.unconsumedBgmAttenuation =
            (i32)(5000.0f * volume) - 5000;
    }
    else
    {
        g_SoundPlayer.unconsumedBgmAttenuation = -10000;
    }

    if (g_Supervisor.flags.dummyMidiTimerEnabled)
    {
        g_Supervisor.dummyMidiTimer = new DummyMidiTimer(0);
        if (g_Supervisor.dummyMidiTimer != NULL)
        {
            g_Supervisor.dummyMidiTimer->StartTimer();
        }
    }

    g_Supervisor.ThreadClose();
    g_Supervisor.startupThreadState = SUPERVISOR_STARTUP_PHASE_IDLE;
    g_Supervisor.flags.unknownBit8 = 0;
    WriteWorkerSignal(g_Supervisor.replayScanWorker.active, 0);
    WriteWorkerSignal(g_Supervisor.replayScanWorker.exitSignal, 1);
    return;

error:
    g_Supervisor.ThreadClose();
    g_Supervisor.startupThreadState = SUPERVISOR_STARTUP_PHASE_FAILED;
    g_Supervisor.flags.receivedCloseMsg = 1;
    WriteWorkerSignal(g_Supervisor.replayScanWorker.active, 0);
    WriteWorkerSignal(g_Supervisor.replayScanWorker.exitSignal, 1);
}
