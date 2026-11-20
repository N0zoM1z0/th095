static __forceinline void PhotoStagePublishCaptureRequestArgs(
    AnmManager *anmManager, i32 captureSlot, i32 left, i32 right,
    i32 top, i32 bottom)
{
    if (anmManager->captureAnmIdx >= 0)
    {
        return;
    }
    anmManager->captureAnmIdx = 9;
    anmManager->captureSourceX = left;
    anmManager->captureSourceY = top;
    anmManager->captureSourceWidth = right - left;
    anmManager->captureSourceHeight = bottom - top;
    anmManager->captureDestinationX = 3;
    anmManager->captureDestinationY = 3;
    anmManager->captureDestinationWidth = right - left;
    anmManager->captureDestinationHeight = bottom - top;
    anmManager->textureCaptureEntryIndex = captureSlot;
}
