#include "Main.hpp"

#include <direct.h>
#include <mmsystem.h>
#include <stdio.h>
#include <winnls32.h>

using namespace th095;

#pragma var_order(d3dDeviceStatus, message, renderResult, i)
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPTSTR commandLine, int showCommand)
{
    HRESULT d3dDeviceStatus;
    i32 i;
    MSG message;
    RenderResult renderResult;

    (void)previousInstance;
    (void)commandLine;
    (void)showCommand;

    renderResult = RENDER_RESULT_KEEP_RUNNING;
    g_Supervisor.instance = instance;

    SystemParametersInfoA(SPI_GETSCREENSAVEACTIVE, 0, &g_GameWindow.screenSaveActive, 0);
    SystemParametersInfoA(SPI_GETLOWPOWERACTIVE, 0, &g_GameWindow.lowPowerActive, 0);
    SystemParametersInfoA(SPI_GETPOWEROFFACTIVE, 0, &g_GameWindow.powerOffActive, 0);
    SystemParametersInfoA(SPI_SETSCREENSAVEACTIVE, 0, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(SPI_SETLOWPOWERACTIVE, 0, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(SPI_SETPOWEROFFACTIVE, 0, NULL, SPIF_SENDCHANGE);

    g_Supervisor.InitializeCriticalSections();
    g_GameErrorContext.Log(GameErrorContext::LOGGER_START);

    if (GameWindow::CheckForRunningGameInstance(instance) == -1)
        goto stop;

    if (g_Supervisor.LoadConfig("th095.cfg") != 0)
        goto stop;

    QueryPerformanceFrequency(&g_GameWindow.performanceFrequency);
    QueryPerformanceCounter(&g_GameWindow.performanceStart);

restart:
    if (GameWindow::InitD3DInterface())
        goto stop;

    if (GameWindow::CreateGameWindow(instance))
        goto stop;

    g_SoundPlayer.Initialize(g_GameWindow.window);

    if (GameWindow::InitD3DRendering())
        goto stop;

    Controller::GetJoystickCaps();
    Controller::ResetKeyboard();

    g_AnmManager = new AnmManager;

    if (!g_Supervisor.IsWindowed())
    {
        WINNLSEnableIME(NULL, FALSE);
        ShowCursor(FALSE);
        SetCursor(NULL);
    }

    g_GameWindow.timeOrigin = 0.0;
    g_GameWindow.lastTimestamp = g_GameWindow.currentTimestamp = g_GameWindow.lastFrameTime =
        g_GameWindow.timeOrigin = g_GameWindow.GetTimestamp();

    renderResult = (RenderResult)Supervisor::RegisterChain();
    if (renderResult != RENDER_RESULT_KEEP_RUNNING)
    {
        if (renderResult == RENDER_RESULT_EXIT_ERROR)
            goto releaseGame;
        renderResult = RENDER_RESULT_RESTART;
        goto releaseGame;
    }

    renderResult = RENDER_RESULT_KEEP_RUNNING;
    g_GameWindow.framesSinceRedraw = -4;

    while (!g_GameWindow.windowIsClosing)
    {
        if (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        else
        {
            d3dDeviceStatus = g_Supervisor.d3dDevice->TestCooperativeLevel();
            if (d3dDeviceStatus == D3D_OK)
            {
                renderResult = g_GameWindow.Render();
                if (renderResult != RENDER_RESULT_KEEP_RUNNING)
                    break;
                g_Supervisor.flags.d3dDeviceNeedsReset = 0;
            }
            else if (d3dDeviceStatus == D3DERR_DEVICENOTRESET)
            {
                g_AnmManager->ReleaseSurfaces();
                if (g_Supervisor.d3dDevice->Reset(&g_Supervisor.presentParameters) != D3D_OK)
                    break;
                GameWindow::ResetRenderState();
                g_Supervisor.screenTransitionCountdown = 3;
                g_Supervisor.flags.d3dDeviceNeedsReset = 1;
            }
        }
    }

releaseGame:
    g_Chain.Release();
    while (g_SoundPlayer.ProcessQueues() != 0)
        ;

stop:
    g_SoundPlayer.RequestThreadStop();
    g_SoundPlayer.JoinThread();
    g_SoundPlayer.Release();

    delete g_AnmManager;
    g_AnmManager = NULL;

    if (g_Supervisor.d3dDevice != NULL)
        g_Supervisor.d3dDevice->Reset(&g_Supervisor.presentParameters);

    if (g_Supervisor.d3dDevice != NULL)
    {
        g_Supervisor.d3dDevice->Release();
        g_Supervisor.d3dDevice = NULL;
    }

    if (g_Supervisor.d3dInterface != NULL)
    {
        g_Supervisor.d3dInterface->Release();
        g_Supervisor.d3dInterface = NULL;
    }

    if (g_GameWindow.window != NULL)
    {
        ShowWindow(g_GameWindow.window, SW_HIDE);
        MoveWindow(g_GameWindow.window, 0, 0, 0, 0, FALSE);
        DestroyWindow(g_GameWindow.window);
        g_GameWindow.window = NULL;
    }

    ShowCursor(TRUE);

    if (renderResult == RENDER_RESULT_RESTART)
    {
        g_GameErrorContext.ResetContext();
        g_GameErrorContext.Log(GameErrorContext::OPTION_CHANGED_RESTART);

        if (!g_Supervisor.IsWindowed())
            WINNLSEnableIME(NULL, TRUE);

        for (i = 0; i < 60; i++)
        {
            if (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }
        }
        goto restart;
    }

    FileSystem::WriteDataToFile("th095.cfg", &g_Supervisor.config, sizeof(g_Supervisor.config));

    if (g_Supervisor.midiOutput != NULL)
    {
        g_Supervisor.midiOutput->StopPlayback();
        delete g_Supervisor.midiOutput;
        g_Supervisor.midiOutput = NULL;
    }

    g_GameErrorContext.Flush();
    g_Supervisor.DeleteCriticalSections();

    SystemParametersInfoA(SPI_SETSCREENSAVEACTIVE, g_GameWindow.screenSaveActive, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(SPI_SETLOWPOWERACTIVE, g_GameWindow.lowPowerActive, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(SPI_SETPOWEROFFACTIVE, g_GameWindow.powerOffActive, NULL, SPIF_SENDCHANGE);
    WINNLSEnableIME(NULL, TRUE);
    return 0;
}

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
        g_Supervisor.ConfigureGameplayViewport(1);
        calcChainResult = g_Chain.RunCalcChain();
        g_SoundPlayer.ProcessQueues();

        if (calcChainResult == 0)
        {
            g_Supervisor.ThreadClose();
            return RENDER_RESULT_EXIT_SUCCESS;
        }
        if (calcChainResult == -1)
        {
            g_Supervisor.ThreadClose();
            return RENDER_RESULT_RESTART;
        }

        this->framesSinceRedraw++;
        if (g_Supervisor.config.frameskipConfig + 1 <= this->framesSinceRedraw)
        {
            g_Supervisor.d3dDevice->BeginScene();
            g_AnmManager->ClearVertexBuffer();
            g_Supervisor.fogState = 0xff;
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

#pragma var_order(i, screenshotPath)
void GameWindow::Present()
{
    i32 i;
    char screenshotPath[MAX_PATH + 1];

    if (g_Supervisor.d3dDevice->Present(NULL, NULL, NULL, NULL) < 0)
    {
        g_AnmManager->ReleaseSurfaces();
        g_Supervisor.d3dDevice->Reset(&g_Supervisor.presentParameters);
        ResetRenderState();
        g_Supervisor.screenTransitionCountdown = 2;
    }

    g_AnmManager->TakeScreenshots();
    if ((g_PressedButtons & 0x800) != 0)
    {
        _mkdir("snapshot");
        for (i = 0; i < 1000; i++)
        {
            sprintf(screenshotPath, "snapshot/th%.3d.bmp", i);
            if (!FileSystem::FileExists(screenshotPath))
                break;
        }
        if (i < 1000)
            g_Supervisor.TakeScreenshot(screenshotPath);
    }
}

f64 GameWindow::GetTimestamp()
{
    LARGE_INTEGER counter;
    f64 timestamp;

    EnterCriticalSection(&g_Supervisor.criticalSections[5]);
    g_Supervisor.timestampUsers++;

    if (this->performanceFrequency.QuadPart == 0)
    {
        timeBeginPeriod(1);
        timestamp = (f64)timeGetTime();
        timeEndPeriod(1);
        if (timestamp < this->timeOrigin)
            this->timeOrigin = timestamp;
        timestamp = (timestamp - this->timeOrigin * 1000.0) / 1000.0;
    }
    else
    {
        QueryPerformanceCounter(&counter);
        timestamp = (f64)(counter.QuadPart - this->performanceStart.QuadPart) /
                    (f64)this->performanceFrequency.QuadPart;
        if (timestamp < this->timeOrigin)
            this->timeOrigin = timestamp;
        timestamp -= this->timeOrigin;
    }

    LeaveCriticalSection(&g_Supervisor.criticalSections[5]);
    g_Supervisor.timestampUsers--;
    return timestamp;
}

i32 GameWindow::InitD3DInterface()
{
    g_Supervisor.d3dInterface = Direct3DCreate8(D3D_SDK_VERSION);
    if (g_Supervisor.d3dInterface == NULL)
    {
        g_GameErrorContext.Fatal(GameErrorContext::D3D_CREATE_FAILED);
        return TRUE;
    }
    return FALSE;
}

#pragma var_order(height, width, windowClass)
i32 GameWindow::CreateGameWindow(HINSTANCE instance)
{
    WNDCLASSA windowClass;
    i32 height;
    i32 width;

    ZeroMemory(&windowClass, sizeof(windowClass));
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    windowClass.lpfnWndProc = WindowProc;
    g_GameWindow.windowIsActive = TRUE;
    g_GameWindow.windowIsInactive = FALSE;
    windowClass.lpszClassName = "BASE";
    windowClass.hInstance = instance;
    RegisterClassA(&windowClass);

    if (!g_Supervisor.IsWindowed())
    {
        width = 640;
        height = 480;
        g_GameWindow.window = CreateWindowExA(0, "BASE", g_WindowTitle, WS_OVERLAPPEDWINDOW, 0, 0, width,
                                              height, NULL, NULL, instance, NULL);
    }
    else
    {
        width = GetSystemMetrics(SM_CXDLGFRAME) * 2 + 640;
        height = GetSystemMetrics(SM_CYDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION) + 480;
        g_GameWindow.window = CreateWindowExA(0, "BASE", g_WindowTitle,
                                              WS_VISIBLE | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT,
                                              CW_USEDEFAULT, width, height, NULL, NULL, instance, NULL);
    }

    g_Supervisor.gameWindow = g_GameWindow.window;
    if (g_GameWindow.window != NULL)
        ActivateWindow(g_GameWindow.window);
    return g_GameWindow.window == NULL;
}

LRESULT __stdcall GameWindow::WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_CLOSE:
        g_Supervisor.flags.unknown5 |= 4;
        return 1;
    case WM_ERASEBKGND:
        return 1;
    case WM_ACTIVATEAPP:
        g_GameWindow.windowIsActive = wparam;
        g_GameWindow.windowIsInactive = wparam == 0;
        break;
    case WM_SETCURSOR:
        if (!g_Supervisor.IsWindowed() && !g_GameWindow.windowIsInactive)
        {
            ShowCursor(FALSE);
            SetCursor(NULL);
        }
        else
        {
            SetCursor(LoadCursorA(NULL, IDC_ARROW));
            ShowCursor(TRUE);
        }
        return 1;
    case MM_MOM_DONE:
        if (g_Supervisor.midiOutput != NULL)
            g_Supervisor.midiOutput->UnprepareHeader((LPMIDIHDR)lparam);
        break;
    }
    return DefWindowProcA(window, message, wparam, lparam);
}

void GameWindow::ActivateWindow(HWND window)
{
    HWND foregroundWindow;
    DWORD foregroundThread;
    DWORD windowThread;
    DWORD foregroundLockTimeout;

    foregroundWindow = GetForegroundWindow();
    foregroundThread = GetWindowThreadProcessId(foregroundWindow, NULL);
    windowThread = GetWindowThreadProcessId(window, NULL);
    AttachThreadInput(windowThread, foregroundThread, TRUE);
    SystemParametersInfoA(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &foregroundLockTimeout, 0);
    SystemParametersInfoA(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, NULL, 0);
    SetActiveWindow(window);
    SystemParametersInfoA(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, &foregroundLockTimeout, 0);
    AttachThreadInput(windowThread, foregroundThread, FALSE);
}
