static __forceinline void InitializeResultCapturePhase()
{
    AnmManager *anmManager = g_AnmManager;
    if (anmManager->captureAnmIdx >= 0)
    {
        return;
    }
    anmManager->captureAnmIdx = 10;
    anmManager->captureSourceX = 0x80;
    anmManager->captureSourceY = 0x10;
    anmManager->captureSourceWidth = 0x180;
    anmManager->captureSourceHeight = 0x1c0;
    anmManager->captureDestinationX = 0;
    anmManager->captureDestinationY = 0;
    anmManager->captureDestinationWidth = 0x80;
    anmManager->captureDestinationHeight = 0x80;
    anmManager->textureCaptureEntryIndex = 0;
}

static inline i32 ReadResultPhotoLimit(const PhotoCameraState *camera)
{
    return camera->photoLimit;
}
