static __forceinline void ResetAnmFrameCounters(AnmManager *manager)
{
    manager->scriptsExecutedThisFrame = 0;
    manager->renderStateChangesThisFrame = 0;
    manager->scriptsStartedThisFrame = 0;
    manager->flushesThisFrame = 0;
}

// FUNCTION: TH095 0x00423440.
i32 __fastcall Supervisor::OnUpdate(void *arg)
{
    struct OnUpdateLocals
    {
        i32 replayScanActive;
        i32 result;
    } locals;

#define supervisor reinterpret_cast<Supervisor *>(arg)
    if (supervisor->flags.receivedCloseMsg)
    {
        ReadWorkerSignal(locals.replayScanActive, supervisor->replayScanWorker.active);
        if (locals.replayScanActive == 0)
            return 4;
    }

    g_SoundPlayer.UpdateFades();
    Controller::GetInput(0);

    g_AnmManager->ClearSprite();
    g_AnmManager->ClearTexture();
    g_AnmManager->ClearColorOp();
    g_AnmManager->InvalidateBlendMode();
    g_AnmManager->ClearZWrite();
    ResetAnmFrameCounters(g_AnmManager);
    g_AnmManager->ClearCameraSettings();
    g_AnmManager->SetMixColorDefault();
    g_AnmManager->screenShakeOffset.x =
        g_AnmManager->screenShakeOffset.y = 0.0f;

    if (g_AnmManager->ServicePreloadedAnims() != ZUN_SUCCESS)
        return 4;

    g_AnmManager->InvalidateDrawPath();
    if (supervisor->startupThreadState != SUPERVISOR_STARTUP_PHASE_IDLE)
    {
        if (supervisor->startupThreadState == SUPERVISOR_STARTUP_PHASE_FAILED)
            return 4;
        return 1;
    }

    locals.result = supervisor->UpdateSceneState();
    if (locals.result != 1)
        return locals.result;
    return 1;
#undef supervisor
}
