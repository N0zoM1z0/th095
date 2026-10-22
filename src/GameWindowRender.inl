// Included inside namespace th095 at both existing Main emission sites.
// Target 0x00420770, 420 bytes. Both terminal calc-chain paths call 0x00425640,
// the primary worker Stop wrapper; 0x00425150 is the separate non-waiting close.
// Stop's actual wait/failure policy is retained: returning is not an
// unconditional successful-join guarantee. This source supplies no D3D/OS ABI.
RenderResult GameWindow::Render()
{
    i32 calcChainResult;

    this->currentTimestamp = this->GetTimestamp();
    if (this->lastTimestamp > this->currentTimestamp)
        this->lastFrameTime = this->currentTimestamp;
    this->lastTimestamp = this->currentTimestamp;

    if (this->lastFrameTime < this->currentTimestamp)
    {
        while (this->lastFrameTime < this->currentTimestamp)
            this->lastFrameTime += 1.0 / 60.0;

        g_AnmManager->FlushVertexBuffer();
        g_Supervisor.ConfigureGameplayViewport(SUPERVISOR_VIEWPORT_FULL_WINDOW);
        calcChainResult = g_Chain.RunCalcChain();
        g_SoundPlayer.ProcessQueues();

        if (calcChainResult == 0)
        {
            g_Supervisor.StopReplayScan();
            return RENDER_RESULT_EXIT_SUCCESS;
        }
        if (calcChainResult == -1)
        {
            g_Supervisor.StopReplayScan();
            return RENDER_RESULT_RESTART;
        }

        this->framesSinceRedraw++;
        if (g_Supervisor.config.frameskipConfig + 1 <= this->framesSinceRedraw)
        {
            g_Supervisor.d3dDevice->BeginScene();
            g_AnmManager->ClearVertexBuffer();
            g_Supervisor.fogState = SUPERVISOR_FOG_CACHE_INVALID;
            g_Supervisor.DisableFog();
            g_Chain.RunDrawChain();
            g_AnmManager->FlushVertexBuffer();
            g_Supervisor.d3dDevice->SetTexture(0, NULL);
            g_Supervisor.d3dDevice->EndScene();
            this->framesSinceRedraw = 0;
        }

        this->currentTimestamp = this->GetTimestamp();
        Present();
    }
    else
    {
        Sleep(0);
    }
    return RENDER_RESULT_KEEP_RUNNING;
}
