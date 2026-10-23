// Shared FPS sampler, emitted by both existing Main paths inside th095.
// The sample timestamp and anomaly count are independent Main-owned globals,
// not members beyond the constructor-proven Supervisor boundary.
void Supervisor::CalculateFps()
{
    struct CalculateFpsLocals
    {
        f64 elapsed;
        f64 currentTime;
    } locals;

    locals.currentTime = g_GameWindow.GetTimestamp();
    if (g_LastFpsTimestamp > locals.currentTime)
    {
        g_LastFpsTimestamp = locals.currentTime;
    }

    if (locals.currentTime - g_LastFpsTimestamp >= 0.5)
    {
        locals.elapsed = locals.currentTime - g_LastFpsTimestamp;
        g_LastFpsTimestamp += locals.elapsed;
        this->currentFps =
            static_cast<f64>(this->fpsFrameCount) / locals.elapsed;

        if (this->currentFps > 65.0f)
        {
            g_FpsClockAnomalyCount++;
            if (g_FpsClockAnomalyCount == 2)
            {
                g_GameWindow.lastTimestamp =
                    g_GameWindow.currentTimestamp =
                        g_GameWindow.lastFrameTime =
                            g_GameWindow.timeOrigin =
                                g_GameWindow.GetTimestamp();
            }
            else if (g_FpsClockAnomalyCount == 4)
            {
                g_GameWindow.performanceFrequency.QuadPart = 0;
                g_GameWindow.lastTimestamp =
                    g_GameWindow.currentTimestamp =
                        g_GameWindow.lastFrameTime =
                            g_GameWindow.timeOrigin =
                                g_GameWindow.GetTimestamp();
                g_FpsClockAnomalyCount = 0;
            }
        }
        else
        {
            g_FpsClockAnomalyCount = 0;
        }

        if (g_SupervisorGameTask != NULL &&
            g_SupervisorGameTask->resultScreenActive == 0 &&
            g_SupervisorGameTask->captureActive == 0 &&
            g_SupervisorGameTask->gameplayLoadActive == 0 &&
            g_SupervisorGameTask->playerDeathTransitionComplete == 0 &&
            g_SupervisorGameTask->photoLimitTransitionComplete == 0 &&
            g_SupervisorGameTask->resetFpsSample == 0)
        {
            this->lagDenominator += 60.0;
            if (this->currentFps > 57.0f)
            {
                this->lagNumerator += 60.0;
            }
            else
            {
                this->lagNumerator += this->currentFps;
            }
        }

        if (g_SupervisorGameTask != NULL)
        {
            g_SupervisorGameTask->resetFpsSample = 0;
        }
        this->fpsFrameCount = 0;
    }
}
