#include "AnmManager.hpp"
#include "AsciiManager.hpp"

#include <direct.h>
#include <math.h>
#include <mmsystem.h>
#include <process.h>
#include <shlguid.h>
#include <shobjidl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winnls32.h>

using namespace th095;

namespace th095
{
struct AnmVmId
{
    AnmVmId()
    {
        this->value = 0;
    }

    i32 value;
};

struct SupervisorGameTaskView
{
    u8 unknown000[0xfc];
    u32 active : 1;
    u32 unknownFlag1 : 1;
    u32 timingBlocked2 : 1;
    u32 unknownFlag3 : 1;
    u32 timingBlocked4 : 1;
    u32 timingBlocked5 : 1;
    u32 timingBlocked6 : 1;
    u32 resetFpsSample : 1;
    u32 unknownFlags8 : 24;
};

extern SupervisorGameTaskView *g_SupervisorGameTask;

struct SupervisorInputWorkerView
{
    void Start(void (__fastcall *callback)(void *), void *argument);
    void Stop();
};

struct SupervisorReplayScanWorkerView
{
    HANDLE handle;
    u32 threadId;
    i32 stopRequested;
    i32 active;
    void (__fastcall *threadProc)(void *);
};

struct FrontEndControllerView
{
    static FrontEndControllerView *__fastcall Create(i32 mode);
    void Destroy();
};

struct PhotoGameTaskView
{
    u8 unknown000[0x120];
    i32 replayMode;

    static PhotoGameTaskView *__fastcall Create(i32 replayMode);
    void Destroy();
};

extern PhotoGameTaskView *g_PhotoGameTask;
extern u32 g_ControllerRuntimeFlags;

struct SupervisorSoundPlayerView
{
    void UpdateFades();
};

struct SupervisorControllerView
{
    static u16 GetInput(i32 inputIndex);
};

struct SupervisorAnmManagerView
{
    u32 mixColor;                              // +0x0000
    i32 useMixColor;                           // +0x0004
    i32 captureSurfaceIdx;                     // +0x0008
    i32 captureAnmIdx;                         // +0x000c
    i32 scriptsStartedThisFrame;               // +0x0010
    i32 scriptsExecutedThisFrame;              // +0x0014
    i32 renderStateChangesThisFrame;           // +0x0018
    i32 flushesThisFrame;                      // +0x001c
    Float2 screenShakeOffset;                  // +0x0020
    u8 unknown028[0x1760 - 0x28];
    IDirect3DTexture8 *currentTexture;          // +0x1760
    u8 currentBlendMode;                       // +0x1764
    u8 currentColorOp;                         // +0x1765
    u8 currentVertexShader;                    // +0x1766
    u8 disableZWrite;                          // +0x1767
    u8 cameraMode;                             // +0x1768
    u8 unknown1769[3];
    void *currentSprite;                       // +0x176c

    ZunResult ServicePreloadedAnims();

    void ClearSprite() { this->currentSprite = NULL; }
    void ClearTexture() { this->currentTexture = NULL; }
    void ClearColorOp() { this->currentColorOp = 0xff; }
    void ClearBlendMode() { this->currentBlendMode = 3; }
    void ClearZWrite() { this->disableZWrite = 0xff; }
    void ResetFrameDebugInfo()
    {
        this->scriptsExecutedThisFrame = 0;
        this->renderStateChangesThisFrame = 0;
        this->scriptsStartedThisFrame = 0;
        this->flushesThisFrame = 0;
    }
    void ClearCameraSettings() { this->cameraMode = 0xff; }
    void SetMixColorDefault()
    {
        this->useMixColor = 0;
        this->mixColor = 0x80808080;
    }
    void ClearVertexShader() { this->currentVertexShader = 0xff; }
};

extern SupervisorSoundPlayerView g_SupervisorSoundPlayer;
extern SupervisorAnmManagerView *g_SupervisorAnmManager;

struct TextHelperView
{
    static void CreateTextBuffer();
    static void ReleaseTextBuffer();
};

struct MidiTimer
{
    MidiTimer();
    ~MidiTimer();
    virtual void OnTimerElapsed();
    void StartTimer();
    void StopTimer();

    UINT timerId;
    TIMECAPS timeCaps;
};

struct DummyMidiTimer : MidiTimer
{
    virtual void OnTimerElapsed();
    u32 unknown010;
};

typedef char MainMidiTimerSizeIs10[(sizeof(MidiTimer) == 0x10) ? 1 : -1];
typedef char MainDummyMidiTimerSizeIs14[(sizeof(DummyMidiTimer) == 0x14) ? 1 : -1];

struct PbgArchiveView
{
    bool Load(const char *path);
    void Release();
};

extern SupervisorInputWorkerView g_SupervisorInputWorker;
extern PbgArchiveView g_PbgArchive;
extern u32 g_PhotoScreenFadeColor;
extern AnmVmId g_SupervisorLoadingVms[3];
extern ScreenEffect *g_SupervisorScreenEffect;

void InitializeScoreData();
void ReleaseScoreData();
HANDLE StartSoundLoadThread();
i32 ReleasePhotoBulletAnm();
i32 ReleaseResultAnm();
i32 ReleaseReplayAnm();
i32 ReleasePhotoFrontAnm();
void ReleaseSceneSelectAnms();
i32 ReleasePhotoPlayerAnm();
}

#define d3dDeviceStatus restartCommandProcessingLocal05
#define message averagedPanLocal12
#define renderResult iLocal11
#define i commandCursorLocal02
#pragma var_order(d3dDeviceStatus, message, renderResult, i)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR pCmdLine, int nCmdShow)
{
    HRESULT d3dDeviceStatus;
    i32 i;
    MSG message;
    i32 renderResult;

    renderResult = RENDER_RESULT_KEEP_RUNNING;
    g_Supervisor.instance = hInstance;

    SystemParametersInfoA(SPI_GETSCREENSAVEACTIVE, 0, &g_GameWindow.screenSaveActive, 0);
    SystemParametersInfoA(SPI_GETLOWPOWERACTIVE, 0, &g_GameWindow.lowPowerActive, 0);
    SystemParametersInfoA(SPI_GETPOWEROFFACTIVE, 0, &g_GameWindow.powerOffActive, 0);
    SystemParametersInfoA(SPI_SETSCREENSAVEACTIVE, 0, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(SPI_SETLOWPOWERACTIVE, 0, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(SPI_SETPOWEROFFACTIVE, 0, NULL, SPIF_SENDCHANGE);

    g_Supervisor.InitializeCriticalSections();
    g_GameErrorContext.Log(
        "\x93\x8c\x95\xfb\x93\xae\x8d\xec\x8b\x4c\x98\x5e\x20\x2d\x2d\x2d\x2d\x2d\x2d\x2d"
        "\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d"
        "\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d"
        "\x20\r\n");

    if (GameWindow::CheckForRunningGameInstance(hInstance) == -1)
        goto stop;

    if (g_Supervisor.LoadConfig("th095.cfg") != 0)
        goto stop;

    QueryPerformanceFrequency(&g_GameWindow.performanceFrequency);
    QueryPerformanceCounter(&g_GameWindow.performanceStart);

restart:
    if (GameWindow::InitD3DInterface())
        goto stop;

    if (GameWindow::CreateGameWindow(hInstance))
        goto stop;

    g_SoundPlayer.Initialize(g_GameWindow.window);

    if (GameWindow::InitD3DRendering())
        goto stop;

    Controller::GetJoystickCaps();
    Controller::ResetKeyboard();

    g_AnmManager = new AnmManager();

    if (g_Supervisor.config.windowed == 0)
    {
        WINNLSEnableIME(NULL, FALSE);
        ShowCursor(FALSE);
        SetCursor(NULL);
    }

    g_GameWindow.timeOrigin = 0.0;
    g_GameWindow.lastTimestamp = g_GameWindow.currentTimestamp = g_GameWindow.lastFrameTime =
        g_GameWindow.timeOrigin = g_GameWindow.GetTimestamp();

    renderResult = Supervisor::RegisterChain();
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
        g_GameErrorContext.Log(
            "\x8d\xc4\x8b\x4e\x93\xae\x82\xf0\x97\x76\x82\xb7\x82\xe9\x83\x49\x83\x76\x83\x56"
            "\x83\x87\x83\x93\x82\xaa\x95\xcf\x8d\x58\x82\xb3\x82\xea\x82\xbd\x82\xcc\x82\xc5"
            "\x8d\xc4\x8b\x4e\x93\xae\x82\xb5\x82\xdc\x82\xb7\r\n");

        if (g_Supervisor.config.windowed == 0)
            WINNLSEnableIME(NULL, TRUE);

        for (i = 0; i < 60;)
        {
            if (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }
            i++;
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
#undef d3dDeviceStatus
#undef message
#undef renderResult
#undef i

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

inline u16 WasPressed(u16 buttons)
{
    return g_PressedButtons & buttons;
}

#pragma var_order(i, screenshotPath)
void GameWindow::Present()
{
    // VC7.1 ignores the stock var-order pragma. These source-level backing
    // names reproduce the target's natural local allocation without padding.
#define screenshotPath snapshotPath
#define i count
    char screenshotPath[MAX_PATH - 4];
    i32 i;
    i32 presentScratch;

    if (g_Supervisor.d3dDevice->Present(NULL, NULL, NULL, NULL) < 0)
    {
        g_AnmManager->ReleaseSurfaces();
        g_Supervisor.d3dDevice->Reset(&g_Supervisor.presentParameters);
        ResetRenderState();
        g_Supervisor.screenTransitionCountdown = 2;
    }

    g_AnmManager->TakeScreenshots();
    if (WasPressed(0x800) != 0)
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
#undef screenshotPath
#undef i
}

#pragma var_order(performanceCounterValue, timestamp)
f64 GameWindow::GetTimestamp()
{
    LARGE_INTEGER performanceCounterValue;
#define timestampValue counterScratch
    f64 timestampValue;
    f64 timestamp;

    g_Supervisor.EnterCriticalSectionWrapper(5);
    g_Supervisor.criticalSectionLockCounts[5]++;

    if (g_GameWindow.performanceFrequency.QuadPart != 0)
    {
        QueryPerformanceCounter(&performanceCounterValue);
        timestampValue = (f64)(performanceCounterValue.QuadPart - g_GameWindow.performanceStart.QuadPart) /
                         (f64)g_GameWindow.performanceFrequency.QuadPart;
        if (g_GameWindow.timeOrigin > timestampValue)
            g_GameWindow.timeOrigin = timestampValue;

        g_Supervisor.LeaveCriticalSectionWrapper(5);
        g_Supervisor.criticalSectionLockCounts[5]--;
        return timestampValue - g_GameWindow.timeOrigin;
    }
    else
    {
        timeBeginPeriod(1);
        timestamp = (f64)timeGetTime();
        timeEndPeriod(1);
        if (g_GameWindow.timeOrigin > timestamp)
            g_GameWindow.timeOrigin = timestamp;
        timestampValue = g_GameWindow.timeOrigin * 1000.0;
        timestampValue = (timestamp - timestampValue) / 1000.0;

        g_Supervisor.LeaveCriticalSectionWrapper(5);
        g_Supervisor.criticalSectionLockCounts[5]--;
        return timestampValue;
    }
#undef timestampValue
}

i32 GameWindow::InitD3DInterface()
{
    g_Supervisor.d3dInterface = Direct3DCreate8(D3D_SDK_VERSION);
    if (g_Supervisor.d3dInterface == NULL)
    {
        g_GameErrorContext.Fatal(
            "Direct3D \x83\x49\x83\x75\x83\x57\x83\x46\x83\x4e\x83\x67\x82\xcd"
            "\x89\xbd\x8c\xcc\x82\xa9\x8d\xec\x90\xac\x8f\x6f\x97\x88\x82\xc8"
            "\x82\xa9\x82\xc1\x82\xbd\r\n");
        return TRUE;
    }
    return FALSE;
}

#pragma var_order(height, width, baseClass)
i32 GameWindow::CreateGameWindow(HINSTANCE instance)
{
    WNDCLASSA baseClass;
    i32 width;
    i32 height;

    ZeroMemory(&baseClass, sizeof(baseClass));
    baseClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    baseClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    baseClass.hInstance = instance;
    baseClass.lpfnWndProc = WindowProc;
    g_GameWindow.windowIsActive = TRUE;
    g_GameWindow.windowIsInactive = FALSE;
    baseClass.lpszClassName = "BASE";
    RegisterClassA(&baseClass);

    if (g_Supervisor.config.windowed == 0)
    {
        width = 640;
        height = 480;
        g_GameWindow.window = CreateWindowExA(0, "BASE", g_WindowTitle, WS_OVERLAPPEDWINDOW, 0, 0,
                                              width, height, NULL, NULL, instance, NULL);
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
    if (g_GameWindow.window == NULL)
    {
        return TRUE;
    }

    ActivateWindow(g_GameWindow.window);
    return FALSE;
}

LRESULT __stdcall GameWindow::WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_ERASEBKGND:
        return 1;
    case MM_MOM_DONE:
        if (g_Supervisor.midiOutput != NULL)
            g_Supervisor.midiOutput->UnprepareHeader((LPMIDIHDR)lparam);
        break;
    case WM_ACTIVATEAPP:
        g_GameWindow.windowIsActive = wparam;

        if (g_GameWindow.windowIsActive)
        {
            g_GameWindow.windowIsInactive = false;
        }
        else
        {
            g_GameWindow.windowIsInactive = true;
        }
        break;
    case WM_SETCURSOR:
        if (g_Supervisor.config.windowed == 0)
        {
            if (g_GameWindow.windowIsInactive)
            {
                SetCursor(LoadCursorA(NULL, IDC_ARROW));
                ShowCursor(TRUE);
            }
            else
            {
                ShowCursor(FALSE);
                SetCursor(NULL);
            }
        }
        else
        {
            SetCursor(LoadCursorA(NULL, IDC_ARROW));
            ShowCursor(TRUE);
        }
        return 1;
    case WM_CLOSE:
        g_Supervisor.flags.receivedCloseMsg = true;
        return 1;
    }
    return DefWindowProcA(window, message, wparam, lparam);
}

// Reuse the target-proven VC7.1 identifier hash buckets from the sound lane
// to reproduce the original var-order layout with the stock compiler.
#define failedToSetFramerate restartCommandProcessingLocal05
#define usingHardwareRenderer averagedPanLocal12
#define displayMode iLocal11
#define presentParameters commandCursorLocal02
#define cameraDistance soundIndexLocal01
#define halfHeight jLocal00
#define halfWidth preloadBufferLocal03
#define aspectRatio bgmPathLocal18
#define fov bgmFormatIndexLocal05
#pragma var_order(failedToSetFramerate, usingHardwareRenderer, displayMode, presentParameters, cameraDistance, \
                  halfHeight, halfWidth, aspectRatio, fov)
i32 GameWindow::InitD3DRendering()
{
    f32 aspectRatio;
    f32 cameraDistance;
    D3DDISPLAYMODE displayMode;
    i32 failedToSetFramerate;
    f32 fov;
    f32 halfHeight;
    f32 halfWidth;
    D3DPRESENT_PARAMETERS presentParameters;
    u8 usingHardwareRenderer;

    usingHardwareRenderer = TRUE;
    memset(&presentParameters, 0, sizeof(presentParameters));
    g_Supervisor.d3dInterface->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

    if (g_Supervisor.config.windowed == 0)
    {
        if (g_Supervisor.config.options.force16BitTextures)
        {
            presentParameters.BackBufferFormat = D3DFMT_R5G6B5;
            g_Supervisor.config.colorMode16bit = 1;
        }
        else
        {
            if (g_Supervisor.config.colorMode16bit == 0xff)
            {
                presentParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
                g_Supervisor.config.colorMode16bit = 0;
                g_GameErrorContext.Log(
                    "\x8f\x89\x89\xf1\x8b\x4e\x93\xae\x81\x41\x89\xe6\x96\xca\x82\xf0\x20"
                    "\x33\x32\x42\x69\x74\x73\x20\x82\xc5\x8f\x89\x8a\xfa\x89\xbb\x82\xb5"
                    "\x82\xdc\x82\xb5\x82\xbd\r\n");
            }
            else if (g_Supervisor.config.colorMode16bit == 0)
            {
                presentParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
            }
            else
            {
                presentParameters.BackBufferFormat = D3DFMT_R5G6B5;
            }
        }
        if (g_GameWindow.usesRelativePath)
            g_Supervisor.disableVsync = TRUE;

        if (!g_Supervisor.disableVsync)
        {
            presentParameters.FullScreen_RefreshRateInHz = 60;
            presentParameters.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
            g_GameErrorContext.Log(
                "\x83\x8a\x83\x74\x83\x8c\x83\x62\x83\x56\x83\x85\x83\x8c\x81\x5b\x83\x67"
                "\x82\xf0\x36\x30\x48\x7a\x82\xc9\x95\xcf\x8d\x58\x82\xf0\x8e\x8e\x82\xdd"
                "\x82\xdc\x82\xb7\r\n");
            if (g_Supervisor.config.frameskipConfig == 0)
                presentParameters.SwapEffect = D3DSWAPEFFECT_FLIP;
            else
                presentParameters.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
        }
        else
        {
            presentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
            presentParameters.SwapEffect = D3DSWAPEFFECT_COPY;
            presentParameters.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
            g_GameErrorContext.Log(
                "\x56\x53\x79\x6e\x63\x94\xf1\x93\xaf\x8a\xfa\x89\xc2\x94\x5c\x82\xa9\x82\xc7"
                "\x82\xa4\x82\xa9\x82\xf0\x8e\x8e\x82\xdd\x82\xdc\x82\xb7\r\n");
        }
    }
    else
    {
        presentParameters.BackBufferFormat = displayMode.Format;
        presentParameters.SwapEffect = D3DSWAPEFFECT_COPY;
        presentParameters.Windowed = TRUE;
    }

    presentParameters.BackBufferWidth = 640;
    presentParameters.BackBufferHeight = 480;
    presentParameters.EnableAutoDepthStencil = TRUE;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    presentParameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    g_Supervisor.flags.lockableBackbuffer = true;
    g_Supervisor.couldSetRefreshRate = TRUE;
    failedToSetFramerate = FALSE;

    for (;;)
    {
        if (g_Supervisor.config.options.useReferenceRasterizer)
            goto referenceRasterizer;

        if (g_Supervisor.d3dInterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_GameWindow.window,
                                                    D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters,
                                                    &g_Supervisor.d3dDevice) < 0)
        {
            if (failedToSetFramerate)
                g_GameErrorContext.Log(
                    "\x54\x26\x4c\x20\x48\x41\x4c\x20\x82\xcd\x8e\x67\x97\x70\x82\xc5\x82\xab"
                    "\x82\xc8\x82\xa2\x82\xe6\x82\xa4\x82\xc5\x82\xb7\r\n");

            if (g_Supervisor.d3dInterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_GameWindow.window,
                                                        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParameters,
                                                        &g_Supervisor.d3dDevice) < 0)
            {
                if (failedToSetFramerate)
                    g_GameErrorContext.Log(
                        "\x48\x41\x4c\x20\x82\xe0\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xc8\x82\xa2"
                        "\x82\xe6\x82\xa4\x82\xc5\x82\xb7\r\n");

            referenceRasterizer:
                if (g_Supervisor.d3dInterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, g_GameWindow.window,
                                                            D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParameters,
                                                            &g_Supervisor.d3dDevice) < 0)
                {
                    if (!g_Supervisor.disableVsync)
                    {
                        g_GameErrorContext.Log(
                            "\x83\x8a\x83\x74\x83\x8c\x83\x62\x83\x56\x83\x85\x83\x8c\x81\x5b\x83\x67"
                            "\x82\xaa\x95\xcf\x8d\x58\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\r\n");
                        presentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
                        g_Supervisor.couldSetRefreshRate = FALSE;
                        failedToSetFramerate = TRUE;
                        continue;
                    }

                    if (presentParameters.FullScreen_PresentationInterval == D3DPRESENT_INTERVAL_IMMEDIATE)
                    {
                        g_GameErrorContext.Log(
                            "\x94\xf1\x93\xaf\x8a\xfa\x8d\x58\x90\x56\x82\xe0\x8d\x73\x82\xa6\x82\xdc\x82\xb9"
                            "\x82\xf1\x81\x42\x88\xea\x94\xd4\x89\x98\x82\xa2\x83\x82\x81\x5b\x83\x68\x82\xc9"
                            "\x95\xcf\x8d\x58\x82\xb5\x82\xdc\x82\xb7\r\n");
                        presentParameters.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
                        presentParameters.SwapEffect = D3DSWAPEFFECT_COPY;
                        continue;
                    }
                    else
                    {
                        g_GameErrorContext.Fatal(
                            "\x44\x69\x72\x65\x63\x74\x33\x44\x20\x82\xcc\x8f\x89\x8a\xfa\x89\xbb\x82\xc9"
                            "\x8e\xb8\x94\x73\x81\x41\x82\xb1\x82\xea\x82\xc5\x82\xcd\x83\x51\x81\x5b\x83"
                            "\x80\x82\xcd\x8f\x6f\x97\x88\x82\xdc\x82\xb9\x82\xf1\r\n");
                        if (g_Supervisor.d3dInterface != NULL)
                        {
                            g_Supervisor.d3dInterface->Release();
                            g_Supervisor.d3dInterface = NULL;
                        }
                        return TRUE;
                    }
                }
                else
                {
                    g_GameErrorContext.Log(
                        "\x52\x45\x46\x20\x82\xc5\x93\xae\x8d\xec\x82\xb5\x82\xdc\x82\xb7\x82\xaa"
                        "\x81\x41\x8f\x64\x82\xb7\x82\xac\x82\xc4\x8b\xb0\x82\xe7\x82\xad\x83\x51"
                        "\x81\x5b\x83\x80\x82\xc9\x82\xc8\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x2e\x2e"
                        "\x2e\r\n");
                    g_Supervisor.flags.usingHardwareTL = false;
                    usingHardwareRenderer = FALSE;
                }
            }
            else
            {
                g_GameErrorContext.Log(
                    "\x48\x41\x4c\x20\x82\xc5\x93\xae\x8d\xec\x82\xb5\x82\xdc\x82\xb7\r\n");
                g_Supervisor.flags.usingHardwareTL = false;
            }
        }
        else
        {
            g_GameErrorContext.Log(
                "\x54\x26\x4c\x20\x48\x41\x4c\x20\x82\xc5\x93\xae\x8d\xec\x82\xb5\x82\xdc"
                "\x81\x60\x82\xb7\r\n");
            g_Supervisor.flags.usingHardwareTL = true;
        }
        break;
    }

#undef presentParameters
    memcpy(&g_Supervisor.presentParameters, &commandCursorLocal02, sizeof(commandCursorLocal02));
#define presentParameters commandCursorLocal02
    halfWidth = 320.0f;
    halfHeight = 240.0f;
    aspectRatio = 4.0f / 3.0f;
    fov = D3DX_PI / 6.0f;
    cameraDistance = halfHeight / (f32)tan(fov / 2.0f);

    D3DXMatrixLookAtLH(&g_Supervisor.viewMatrix, &D3DXVECTOR3(halfWidth, -halfHeight, -cameraDistance),
                       &D3DXVECTOR3(halfWidth, -halfHeight, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
    D3DXMatrixPerspectiveFovLH(&g_Supervisor.projectionMatrix, fov, aspectRatio, 100.0f, 10000.0f);
    g_Supervisor.d3dDevice->SetTransform(D3DTS_VIEW, &g_Supervisor.viewMatrix);
    g_Supervisor.d3dDevice->SetTransform(D3DTS_PROJECTION, &g_Supervisor.projectionMatrix);
    g_Supervisor.d3dDevice->GetViewport(&g_Supervisor.viewport);
    g_Supervisor.d3dDevice->GetDeviceCaps(&g_Supervisor.d3dCaps);

    if ((g_Supervisor.d3dCaps.TextureOpCaps & D3DTEXOPCAPS_ADD) == 0)
        g_GameErrorContext.Log(
            "\x44\x33\x44\x54\x45\x58\x4f\x50\x43\x41\x50\x53\x5f\x41\x44\x44\x20\x82\xf0"
            "\x83\x54\x83\x7c\x81\x5b\x83\x67\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb9\x82\xf1"
            "\x81\x41\x90\x46\x89\xc1\x8e\x5a\x83\x47\x83\x7e\x83\x85\x83\x8c\x81\x5b\x83\x67"
            "\x83\x82\x81\x5b\x83\x68\x82\xc5\x93\xae\x8d\xec\x82\xb5\x82\xdc\x82\xb7\r\n");

    if (g_Supervisor.d3dCaps.MaxTextureWidth <= 256)
        g_GameErrorContext.Log(
            "\x35\x31\x32\x20\x88\xc8\x8f\xe3\x82\xcc\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83"
            "\x82\xf0\x83\x54\x83\x7c\x81\x5b\x83\x67\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb9"
            "\x82\xf1\x81\x42\x96\x77\x82\xc7\x82\xcc\x8a\x47\x82\xaa\x83\x7b\x83\x50\x82\xc4"
            "\x95\x5c\x8e\xa6\x82\xb3\x82\xea\x82\xdc\x82\xb7\x81\x42\r\n");

    if (!g_Supervisor.config.options.force16BitTextures && usingHardwareRenderer)
    {
        if (g_Supervisor.d3dInterface->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                                        presentParameters.BackBufferFormat, 0, D3DRTYPE_TEXTURE,
                                                        D3DFMT_A8R8G8B8) == D3D_OK)
        {
            g_Supervisor.flags.using32BitGraphics = true;
        }
        else
        {
            g_Supervisor.flags.using32BitGraphics = false;
            g_Supervisor.config.options.force16BitTextures = true;
            g_GameErrorContext.Log(
                "\x44\x33\x44\x46\x4d\x54\x5f\x41\x38\x52\x38\x47\x38\x42\x38\x20\x82\xf0\x83"
                "\x54\x83\x7c\x81\x5b\x83\x67\x82\xb5\x82\xc4\x82\xa2\x82\xdc\x82\xb9\x82\xf1"
                "\x81\x41\x8c\xb8\x90\x46\x83\x82\x81\x5b\x83\x68\x82\xc5\x93\xae\x8d\xec\x82"
                "\xb5\x82\xdc\x82\xb7\r\n");
        }
    }

    ResetRenderState();
    ScreenEffect::SetViewport(0xff000000);
    g_GameWindow.windowIsClosing = FALSE;
    g_Supervisor.lastFrameTime = 0;
    return FALSE;
}
#undef failedToSetFramerate
#undef usingHardwareRenderer
#undef displayMode
#undef presentParameters
#undef cameraDistance
#undef halfHeight
#undef halfWidth
#undef aspectRatio
#undef fov

#pragma var_order(fogValue, fogDensity)
void GameWindow::ResetRenderState()
{
    f32 fogDensity;
    f32 fogValue;

    g_Supervisor.d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_ALPHAREF, 4);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    if (g_Supervisor.config.options.disableFog == 0)
        g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
    else
        g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

    fogDensity = 1.0f;
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGDENSITY, *(u32 *)&fogDensity);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGCOLOR, 0xffa0a0a0);

    fogValue = 1000.0f;
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGSTART, *(u32 *)&fogValue);
    fogValue = 5000.0f;
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGEND, *(u32 *)&fogValue);

    if (g_Supervisor.d3dCaps.RasterCaps | D3DPRASTERCAPS_ANTIALIASEDGES)
        g_Supervisor.d3dDevice->SetRenderState(D3DRS_EDGEANTIALIAS, FALSE);

    g_Supervisor.d3dDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTEXF_NONE);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);

    if (g_AnmManager != NULL)
    {
        g_AnmManager->ClearBlendMode();
        g_AnmManager->ClearColorOp();
        g_AnmManager->ClearVertexShader();
        g_AnmManager->ClearTexture();
        g_AnmManager->ClearCameraSettings();
    }
}

#define moduleFilenameBuffer restartCommandProcessingLocal05
#define startupInfo averagedPanLocal12
#define consoleTitleBuffer iLocal11
#define fileExtension commandCursorLocal02
#pragma var_order(moduleFilenameBuffer, startupInfo, consoleTitleBuffer, fileExtension)
i32 GameWindow::CheckForRunningGameInstance(HINSTANCE hInstance)
{
    char consoleTitleBuffer[MAX_PATH + 1];
    char *fileExtension;
    char moduleFilenameBuffer[MAX_PATH + 1];
    STARTUPINFOA startupInfo;

    g_ExclusiveMutex = CreateMutexA(NULL, TRUE, "Touhou 08 App");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        g_GameErrorContext.Fatal(
            "\x93\xf1\x82\xc2\x82\xcd\x8b\x4e\x93\xae\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\r\n");
        return -1;
    }

    startupInfo.cb = sizeof(startupInfo);
    memset(&startupInfo.lpReserved, 0, sizeof(startupInfo) - sizeof(startupInfo.cb));
    GetModuleFileNameA(NULL, moduleFilenameBuffer, sizeof(moduleFilenameBuffer));
    GetConsoleTitleA(consoleTitleBuffer, sizeof(consoleTitleBuffer));
    GetStartupInfoA(&startupInfo);

    if (startupInfo.lpTitle != NULL)
    {
        fileExtension = strrchr(startupInfo.lpTitle, '.');
        if (FileSystem::FileExists(startupInfo.lpTitle) && fileExtension != NULL)
        {
            if (_stricmp(fileExtension, ".lnk") == 0)
            {
                do
                {
                    ResolveShortcut(startupInfo.lpTitle, consoleTitleBuffer, MAX_PATH);
                    fileExtension = strrchr(consoleTitleBuffer, '.');
                } while (_stricmp(fileExtension, ".lnk") == 0);
            }
            else
            {
                strcpy(consoleTitleBuffer, startupInfo.lpTitle);
            }

            if (strcmp(moduleFilenameBuffer, consoleTitleBuffer) != 0)
                g_GameWindow.usesRelativePath = true;
        }
        g_Supervisor.flags.dummyMidiTimerEnabled = false;
    }
    else
    {
        g_Supervisor.flags.dummyMidiTimerEnabled = true;
    }

    if (g_ExclusiveMutex == NULL)
        return -1;
    return 0;
}
#undef moduleFilenameBuffer
#undef startupInfo
#undef consoleTitleBuffer
#undef fileExtension

#define resolveResult restartCommandProcessingLocal05
#define returnValue averagedPanLocal12
#define shellLink iLocal11
#define persistFile commandCursorLocal02
#define widePath soundIndexLocal01
#define findData jLocal00
#pragma var_order(resolveResult, returnValue, shellLink, persistFile, widePath, findData)
i32 GameWindow::ResolveShortcut(char *shortcutPath, char *destination, i32 destinationSize)
{
    HRESULT resolveResult;
    IPersistFile *persistFile;
    IShellLinkA *shellLink;
    i32 returnValue;
    WIN32_FIND_DATAA findData;
    LPWSTR widePath;

    if (destination == NULL)
        return FALSE;

    returnValue = FALSE;
    CoInitialize(NULL);
    resolveResult = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkA,
                                     (LPVOID *)&shellLink);
    if (SUCCEEDED(resolveResult))
    {
        resolveResult = shellLink->QueryInterface(IID_IPersistFile, (void **)&persistFile);
        if (SUCCEEDED(resolveResult))
        {
            widePath = new WCHAR[destinationSize];
            if (SUCCEEDED(resolveResult))
            {
                MultiByteToWideChar(CP_ACP, 0, shortcutPath, -1, widePath, destinationSize);
                resolveResult = persistFile->Load(widePath, STGM_READ);
                if (SUCCEEDED(resolveResult))
                {
                    resolveResult = shellLink->GetPath(destination, destinationSize, &findData, 0);
                    if (SUCCEEDED(resolveResult))
                        returnValue = TRUE;
                }
            }

            delete widePath;
            persistFile->Release();
        }
        shellLink->Release();
    }
    CoUninitialize();
    return returnValue;
}
#undef resolveResult
#undef returnValue
#undef shellLink
#undef persistFile
#undef widePath
#undef findData

#define foregroundWinThread windowThread
#define touhouWinThread foregroundThread
#define lockoutTime foregroundLockTimeout
void GameWindow::ActivateWindow(HWND hWnd)
{
    DWORD foregroundWinThread;
    u32 lockoutTime;
    DWORD touhouWinThread;

    foregroundWinThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    touhouWinThread = GetWindowThreadProcessId(hWnd, NULL);
    AttachThreadInput(touhouWinThread, foregroundWinThread, TRUE);
    SystemParametersInfoA(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &lockoutTime, 0);
    SystemParametersInfoA(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, NULL, 0);
    SetActiveWindow(hWnd);
    SystemParametersInfoA(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, &lockoutTime, 0);
    AttachThreadInput(touhouWinThread, foregroundWinThread, FALSE);
}
#undef foregroundWinThread
#undef touhouWinThread
#undef lockoutTime

void AnmManager::ReleaseSurfaces()
{
    i32 i;

    for (i = 0; i < 32; i++)
    {
        if (this->surfaces[i] != NULL)
        {
            this->surfaces[i]->Release();
            this->surfaces[i] = NULL;
        }
    }
}

#define elem restartCommandProcessingLocal05
#define result averagedPanLocal12
#define supervisor iLocal11
#pragma var_order(elem, result, supervisor)
i32 Supervisor::RegisterChain()
{
    Supervisor *supervisor = &g_Supervisor;

    supervisor->wantedState = 0;
    supervisor->currentState = -1;
    supervisor->calcCount = 0;

    ChainElem *elem = g_Chain.CreateElem((ChainCallback)Supervisor::OnUpdate);
    elem->arg = supervisor;
    elem->addedCallback = (ChainLifetimeCallback)Supervisor::AddedCallback;
    elem->deletedCallback = (ChainLifetimeCallback)Supervisor::DeletedCallback;

    i32 result = g_Chain.AddToCalcChain(elem, 0);
    if (result != 0)
        return result;

    elem = g_Chain.CreateElem((ChainCallback)Supervisor::OnDraw2);
    elem->arg = supervisor;
    g_Chain.AddToDrawChain(elem, 0);

    elem = g_Chain.CreateElem((ChainCallback)Supervisor::DrawFpsCounter);
    elem->arg = supervisor;
    g_Chain.AddToDrawChain(elem, 0x17);

    elem = g_Chain.CreateElem((ChainCallback)Supervisor::FinalizeFrame);
    elem->arg = supervisor;
    g_Chain.AddToDrawChain(elem, 0x1e);
    return 0;
}
#undef elem
#undef result
#undef supervisor

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
        locals.replayScanActive = supervisor->replayScanActive;
        if (locals.replayScanActive == 0)
            return 4;
    }

    g_SupervisorSoundPlayer.UpdateFades();
    SupervisorControllerView::GetInput(0);

    g_SupervisorAnmManager->ClearSprite();
    g_SupervisorAnmManager->ClearTexture();
    g_SupervisorAnmManager->ClearColorOp();
    g_SupervisorAnmManager->ClearBlendMode();
    g_SupervisorAnmManager->ClearZWrite();
    g_SupervisorAnmManager->ResetFrameDebugInfo();
    g_SupervisorAnmManager->ClearCameraSettings();
    g_SupervisorAnmManager->SetMixColorDefault();
    g_SupervisorAnmManager->screenShakeOffset.x =
        g_SupervisorAnmManager->screenShakeOffset.y = 0.0f;

    if (g_SupervisorAnmManager->ServicePreloadedAnims() != ZUN_SUCCESS)
        return 4;

    g_SupervisorAnmManager->ClearVertexShader();
    if (supervisor->startupThreadState != 0)
    {
        if (supervisor->startupThreadState == 2)
            return 4;
        return 1;
    }

    locals.result = supervisor->UpdateSceneState();
    if (locals.result != 1)
        return locals.result;
    return 1;
#undef supervisor
}

static __forceinline void GetSupervisorAnmSurface(
    IDirect3DSurface8 **output, AnmManager *manager, i32 surfaceIndex)
{
    *output = manager->surfaces[surfaceIndex];
}

// FUNCTION: TH095 0x004235D0.
i32 __fastcall Supervisor::OnDraw2(Supervisor *s)
{
    struct OnDraw2Locals
    {
        IDirect3DSurface8 *surface;
        i32 color2;
        i32 color1;
        Float3 position;
    } locals;

    s->ConfigureGameplayViewport(1);
    if (g_Supervisor.backbufferClearColor != 0)
    {
        g_Supervisor.d3dDevice->Clear(
            0, NULL, D3DCLEAR_TARGET, g_Supervisor.backbufferClearColor, 1.0f, 0);
    }

    if (s->loadingVmsHaveBeenSetup >= 2)
    {
        s->loadingVmsHaveBeenSetup++;
        if (s->loadingVmsHaveBeenSetup >= 5)
        {
            locals.position.x = 288.0f;
            locals.position.y = 454.0f;
            locals.position.z = 0.0f;
            g_AsciiManager.scaleX = 0.5f;
            g_AsciiManager.scaleY = 0.5f;
            if (s->loadingVmsHaveBeenSetup < 35)
            {
                locals.color1 = 255 - (((s->loadingVmsHaveBeenSetup - 5) << 7) / 30);
                g_AsciiManager.color.a = locals.color1;
            }
            else
            {
                locals.color2 = 255 - (((65 - s->loadingVmsHaveBeenSetup) << 7) / 30);
                g_AsciiManager.color.a = locals.color2;
            }
            g_AsciiManager.AddFormatText(&locals.position, "Press Shot Button");
            g_AsciiManager.scaleX = 1.0f;
            g_AsciiManager.scaleY = 1.0f;
            g_AsciiManager.DrawStrings();
            g_AsciiManager.numStrings = 0;
            g_AsciiManager.numGuiStrings = 0;

            if (s->loadingVmsHaveBeenSetup >= 65)
            {
                s->loadingVmsHaveBeenSetup = 5;
            }
        }
    }

    if (s->loadingVmsHaveBeenSetup != 0)
    {
        g_AnmManager->CopySurfaceToBackbuffer(8, 0, 0, 0, 0);
    }
    else
    {
        GetSupervisorAnmSurface(&locals.surface, g_AnmManager, 8);
        if (locals.surface != NULL)
        {
            g_AnmManager->ReleaseSurface(8);
        }
    }
    return 1;
}

// FUNCTION: TH095 0x00423790.
i32 __fastcall Supervisor::DrawFpsCounter(Supervisor *s)
{
    struct DrawFpsCounterLocals
    {
        D3DCOLOR color;
        Float3 position;
        f32 currentFps;
    } locals;

    g_Supervisor.CalculateFps();
    locals.currentFps = g_Supervisor.currentFps;
    if (locals.currentFps < 30.0f)
    {
        locals.color = 0xff5050ff;
    }
    else
    {
        locals.color = locals.currentFps < 40.0f ? 0xffa0a0ff : 0xffffffff;
    }

    g_AsciiManager.color.color = locals.color;
    locals.position.x = 288.0f;
    locals.position.y = 0.0f;
    locals.position.z = 0.0f;
    g_AsciiManager.AddFormatText(
        &locals.position, "%2.1ffps", locals.currentFps);
    g_AsciiManager.color.color = 0xffffffff;
    return 1;
}

// FUNCTION: TH095 0x00424720.
void Supervisor::CalculateFps()
{
    struct CalculateFpsLocals
    {
        f64 elapsed;
        f64 currentTime;
    } locals;

    locals.currentTime = g_GameWindow.GetTimestamp();
    if (g_Supervisor.lastFpsTimestamp > locals.currentTime)
    {
        g_Supervisor.lastFpsTimestamp = locals.currentTime;
    }

    if (locals.currentTime - g_Supervisor.lastFpsTimestamp >= 0.5)
    {
        locals.elapsed = locals.currentTime - g_Supervisor.lastFpsTimestamp;
        g_Supervisor.lastFpsTimestamp += locals.elapsed;
        this->currentFps =
            static_cast<f64>(this->fpsFrameCount) / locals.elapsed;

        if (this->currentFps > 65.0f)
        {
            g_Supervisor.fpsClockAnomalyCount++;
            if (g_Supervisor.fpsClockAnomalyCount == 2)
            {
                g_GameWindow.lastTimestamp =
                    g_GameWindow.currentTimestamp =
                        g_GameWindow.lastFrameTime =
                            g_GameWindow.timeOrigin =
                                g_GameWindow.GetTimestamp();
            }
            else if (g_Supervisor.fpsClockAnomalyCount == 4)
            {
                g_GameWindow.performanceFrequency.QuadPart = 0;
                g_GameWindow.lastTimestamp =
                    g_GameWindow.currentTimestamp =
                        g_GameWindow.lastFrameTime =
                            g_GameWindow.timeOrigin =
                                g_GameWindow.GetTimestamp();
                g_Supervisor.fpsClockAnomalyCount = 0;
            }
        }
        else
        {
            g_Supervisor.fpsClockAnomalyCount = 0;
        }

        if (g_SupervisorGameTask != NULL &&
            g_SupervisorGameTask->timingBlocked4 == 0 &&
            g_SupervisorGameTask->active == 0 &&
            g_SupervisorGameTask->timingBlocked2 == 0 &&
            g_SupervisorGameTask->timingBlocked5 == 0 &&
            g_SupervisorGameTask->timingBlocked6 == 0 &&
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

// FUNCTION: TH095 0x00423840.
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

// FUNCTION: TH095 0x004238E0.
void __fastcall Supervisor::InitializeInput(Supervisor *s)
{
    g_Supervisor.flags.keyboardAvailable = 0;
    g_Supervisor.flags.controllerAvailable = 0;
    g_Supervisor.SetupDInput();
    g_Supervisor.flags.keyboardAvailable = g_Supervisor.keyboard != NULL;
    g_Supervisor.flags.controllerAvailable = g_Supervisor.controller != NULL;
}

// FUNCTION: TH095 0x00423960.
i32 Supervisor::SetupDInput()
{
    HINSTANCE instance = (HINSTANCE)GetWindowLongA(this->gameWindow, GWL_HINSTANCE);

    if (this->config.options.disableDirectInput != 0)
    {
        return -1;
    }

    if (DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8A,
                           (void **)&this->directInput, NULL) < 0)
    {
        this->directInput = NULL;
        g_GameErrorContext.Log("DirectInput \x82\xaa\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\r\n");
        return -1;
    }

    if (this->directInput->CreateDevice(GUID_SysKeyboard, &this->keyboard, NULL) < 0)
    {
        if (this->directInput != NULL)
        {
            this->directInput->Release();
            this->directInput = NULL;
        }
        g_GameErrorContext.Log("DirectInput \x82\xaa\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\r\n");
        return -1;
    }

    if (this->keyboard->SetDataFormat(&c_dfDIKeyboard) < 0)
    {
        if (this->keyboard != NULL)
        {
            this->keyboard->Release();
            this->keyboard = NULL;
        }
        if (this->directInput != NULL)
        {
            this->directInput->Release();
            this->directInput = NULL;
        }
        g_GameErrorContext.Log(
            "DirectInput SetDataFormat \x82\xaa\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\r\n");
        return -1;
    }

    if (this->keyboard->SetCooperativeLevel(
            this->gameWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY) < 0)
    {
        if (this->keyboard != NULL)
        {
            this->keyboard->Release();
            this->keyboard = NULL;
        }
        if (this->directInput != NULL)
        {
            this->directInput->Release();
            this->directInput = NULL;
        }
        g_GameErrorContext.Log(
            "DirectInput SetCooperativeLevel \x82\xaa\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\r\n");
        return -1;
    }

    this->keyboard->Acquire();
    g_GameErrorContext.Log(
        "DirectInput \x82\xcd\x90\xb3\x8f\xed\x82\xc9\x8f\x89\x8a\xfa\x89\xbb\x82\xb3\x82\xea\x82\xdc\x82\xb5\x82\xbd\r\n");
    this->directInput->EnumDevices(
        DI8DEVCLASS_GAMECTRL, Supervisor::EnumGameControllersCb, NULL, DIEDFL_ATTACHEDONLY);

    if (this->controller != NULL)
    {
        this->controller->SetDataFormat(&c_dfDIJoystick);
        this->controller->SetCooperativeLevel(
            this->gameWindow, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
        g_Supervisor.controllerCaps.dwSize = sizeof(DIDEVCAPS);
        this->controller->GetCapabilities(&g_Supervisor.controllerCaps);
        this->controller->EnumObjects(Supervisor::ControllerCallback, NULL, 0);
        g_GameErrorContext.Log(
            "\x97\x4c\x8c\xf8\x82\xc8\x83\x70\x83\x62\x83\x68\x82\xf0\x94\xad\x8c\xa9\x82\xb5\x82\xdc\x82\xb5\x82\xbd\r\n");
    }

    return 0;
}

// FUNCTION: TH095 0x00423C20.
BOOL CALLBACK Supervisor::EnumGameControllersCb(
    LPCDIDEVICEINSTANCEA instance, LPVOID context)
{
    HRESULT result;

    if (g_Supervisor.controller == NULL)
    {
        result = g_Supervisor.directInput->CreateDevice(
            instance->guidInstance, &g_Supervisor.controller, NULL);
        if (result < 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}

// FUNCTION: TH095 0x00423C70.
BOOL CALLBACK Supervisor::ControllerCallback(
    LPCDIDEVICEOBJECTINSTANCEA object, LPVOID context)
{
    DIPROPRANGE range;
    LPVOID callbackContext;

    callbackContext = context;

    if ((object->dwType & 3) != 0)
    {
        range.diph.dwSize = sizeof(DIPROPRANGE);
        range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        range.diph.dwHow = DIPH_BYID;
        range.diph.dwObj = object->dwType;
        range.lMin = -1000;
        range.lMax = 1000;

        if (g_Supervisor.controller->SetProperty(DIPROP_RANGE, &range.diph) < 0)
        {
            return FALSE;
        }
    }

    return TRUE;
}

// FUNCTION: TH095 0x00423CE0.
void Supervisor::StartInputWorker()
{
    g_SupervisorInputWorker.Start(
        (void (__fastcall *)(void *))Supervisor::InitializeInput,
        &g_Supervisor);
}

// FUNCTION: TH095 0x00423D00.
void Supervisor::ReleaseGameManagers()
{
    if (this->frontEndController != NULL)
    {
        this->frontEndController->Destroy();
    }
    this->frontEndController = NULL;

    if (this->photoGameTask != NULL)
    {
        this->photoGameTask->Destroy();
    }
    this->photoGameTask = NULL;
}

// FUNCTION: TH095 0x00425EF0.
i32 Supervisor::UpdateSceneState()
{
    struct
    {
        i32 resultMode;
        i32 replayMode;
    } locals;

    if (this->wantedState != this->currentState)
    {
        this->EnterCriticalSectionWrapper(5);
        this->criticalSectionLockCounts[5]++;
        this->previousState = this->wantedState;
        utils::DebugPrint(
            "scene %d -> %d\r\n", this->wantedState, this->currentState);
        this->backbufferClearColor = 0xff000000;

        switch (this->wantedState)
        {
        case 0:
            this->currentState = SUPERVISOR_STATE_FRONT_END;
            this->frontEndController = FrontEndControllerView::Create(0);
            if (this->frontEndController == NULL)
            {
                goto failure;
            }
            break;

        case SUPERVISOR_STATE_FRONT_END:
            switch (this->currentState)
            {
            case SUPERVISOR_STATE_ERROR:
                goto failure;

            case SUPERVISOR_STATE_EXIT:
                this->frontEndController->Destroy();
                this->frontEndController = NULL;
                this->LeaveCriticalSectionWrapper(5);
                this->criticalSectionLockCounts[5]--;
                return 4;

            case SUPERVISOR_STATE_PHOTO_GAME:
                this->frontEndController->Destroy();
                this->frontEndController = NULL;
                break;

            case SUPERVISOR_STATE_START_REPLAY:
                this->currentState = SUPERVISOR_STATE_PHOTO_GAME;
                this->frontEndController->Destroy();
                this->frontEndController = NULL;
                break;
            }
            break;

        case SUPERVISOR_STATE_PHOTO_GAME:
            switch (this->currentState)
            {
            case SUPERVISOR_STATE_EXIT:
                this->photoGameTask->Destroy();
                this->photoGameTask = NULL;
                return 4;

            case SUPERVISOR_STATE_FRONT_END:
                locals.replayMode = this->photoGameTask->replayMode;
                this->photoGameTask->Destroy();
                this->photoGameTask = NULL;
                this->frontEndController =
                    FrontEndControllerView::Create(
                        locals.replayMode != 0 ? 2 : 1);
                if (this->frontEndController == NULL)
                {
                    goto failure;
                }
                break;

            case SUPERVISOR_STATE_ERROR:
                goto failure;

            case SUPERVISOR_STATE_RETRY_PHOTO_GAME:
                this->photoGameTask->Destroy();
                this->photoGameTask = NULL;
                this->flags.restartPhotoGame = 1;
                this->photoGameTask = PhotoGameTaskView::Create(0);
                if (this->photoGameTask == NULL)
                {
                    goto failure;
                }
                this->currentState = SUPERVISOR_STATE_PHOTO_GAME;
                break;

            case SUPERVISOR_STATE_RESTART_PHOTO_GAME:
                locals.resultMode = g_PhotoGameTask->replayMode;
                g_ControllerRuntimeFlags |= 0x200;
                this->photoGameTask->Destroy();
                this->photoGameTask = NULL;
                this->photoGameTask =
                    PhotoGameTaskView::Create(locals.resultMode);
                if (this->photoGameTask == NULL)
                {
                    goto failure;
                }
                this->currentState = SUPERVISOR_STATE_PHOTO_GAME;
                break;
            }
            break;

        case SUPERVISOR_STATE_ERROR:
        failure:
            this->ReleaseGameManagers();
            this->LeaveCriticalSectionWrapper(5);
            this->criticalSectionLockCounts[5]--;
            return 4;
        }

        this->wantedState = this->currentState;
        this->LeaveCriticalSectionWrapper(5);
        this->criticalSectionLockCounts[5]--;
    }
    return 1;
}

// FUNCTION: TH095 0x00423E70.
i32 __fastcall Supervisor::AddedCallback(Supervisor *s)
{
    g_AnmGameSpeed = 1.0f;
    g_Supervisor.InitializeViewports();
    g_Supervisor.totalPlayTime = timeGetTime();
    g_Rng.seed = (u16)g_Supervisor.totalPlayTime;
    g_Rng2.seed = (u16)g_Supervisor.totalPlayTime;

    if (Supervisor::LoadDat() != 0)
    {
        return -1;
    }

    g_PhotoScreenFadeColor = 0xff000000;
    InitializeScoreData();
    g_AnmManager->LoadSurface(8, "title/th08logo.jpg");
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
    g_Supervisor.startupThreadState = 1;
    g_Supervisor.StartReplayScan(
        (void (__fastcall *)(void *))Supervisor::StartupThread, s);
    return 0;
}

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
        g_AnmManager->CopySurfaceToBackbuffer(8, 0, 0, 0, 0);
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
void __fastcall Supervisor::StartupThread(Supervisor *s)
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
        g_Supervisor.dummyMidiTimer = new DummyMidiTimer();
        if (g_Supervisor.dummyMidiTimer != NULL)
        {
            g_Supervisor.dummyMidiTimer->StartTimer();
        }
    }

    g_Supervisor.ThreadClose();
    g_Supervisor.startupThreadState = 0;
    g_Supervisor.flags.scoreBackupPending = 0;
    g_Supervisor.replayScanActive = 0;
    g_Supervisor.replayScanStopRequested = 1;
    return;

error:
    g_Supervisor.ThreadClose();
    g_Supervisor.startupThreadState = 2;
    g_Supervisor.flags.receivedCloseMsg = 1;
    g_Supervisor.replayScanActive = 0;
    g_Supervisor.replayScanStopRequested = 1;
}

// FUNCTION: TH095 0x004244D0.
i32 __fastcall Supervisor::DeletedCallback(void *arg)
{
    g_SupervisorInputWorker.Stop();
    g_SoundPlayer.RequestThreadStop();
    g_Supervisor.StopReplayScan();

    if (g_Supervisor.versionData != NULL)
    {
        void *versionData = g_Supervisor.versionData;
        free(versionData);
        g_Supervisor.versionData = NULL;
    }

    ((Supervisor *)arg)->ReleaseGameManagers();
    ReleasePhotoBulletAnm();
    ReleaseResultAnm();
    ReleaseReplayAnm();
    ReleasePhotoFrontAnm();
    ReleaseSceneSelectAnms();
    ReleasePhotoPlayerAnm();
    ReleaseScoreData();

    AnmManager *anmManager = g_AnmManager;
    if (anmManager->quadVertexBuffer != NULL)
    {
        anmManager->quadVertexBuffer->Release();
        anmManager->quadVertexBuffer = NULL;
    }
    g_AnmManager->ReleaseAnm(0);
    g_AnmManager->ReleaseAnm(2);
    g_AnmManager->ReleaseSurface(8);
    AsciiManager::CutChain();
    g_SoundPlayer.QueueCommand(4, 0, "dummy");
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

    if (((Supervisor *)arg)->controller != NULL)
    {
        utils::DebugPrint("DirectInput(Pad) Release\n");
        ((Supervisor *)arg)->controller->Unacquire();
        if (((Supervisor *)arg)->controller != NULL)
        {
            ((Supervisor *)arg)->controller->Release();
            ((Supervisor *)arg)->controller = NULL;
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

// FUNCTION: TH095 0x00424980.
void __fastcall Supervisor::ScreenshotThread(void *unused)
{
    void *infoHeader;
    void *pixels;

    FileSystem::OpenWriteFile(g_Supervisor.screenshotPath);
    FileSystem::WriteToOpenFile(&g_Supervisor.screenshotFileHeader,
                                sizeof(g_Supervisor.screenshotFileHeader));
    FileSystem::WriteToOpenFile(g_Supervisor.screenshotInfoHeader,
                                sizeof(BITMAPINFOHEADER));
    FileSystem::WriteToOpenFile(g_Supervisor.screenshotPixels, 0xe1000);
    FileSystem::CloseWriteFile();
    infoHeader = g_Supervisor.screenshotInfoHeader;
    free(infoHeader);
    pixels = g_Supervisor.screenshotPixels;
    free(pixels);
    g_Supervisor.screenshotThread = 0;
}

// FUNCTION: TH095 0x00424A00.
i32 Supervisor::TakeScreenshot(char *path)
{
    struct ScreenshotCaptureLocals
    {
        i32 allocationSize;
        D3DLOCKED_RECT lockedRect;
        i32 destinationRow;
        i32 x;
        i32 y;
        u8 *destination;
        u8 *source;
        i32 widthBytes;
        IDirect3DSurface8 *backbuffer;
    } locals;

#define allocationSize locals.allocationSize
#define lockedRect locals.lockedRect
#define destinationRow locals.destinationRow
#define x locals.x
#define y locals.y
#define destination locals.destination
#define source locals.source
#define widthBytes locals.widthBytes
#define backbuffer locals.backbuffer

    while (this->screenshotThread != 0)
        Sleep(10);

    backbuffer = NULL;
    utils::DebugPrint("SnapShot! %s\n", path);
    this->d3dDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

    memset(&this->screenshotFileHeader, 0,
           sizeof(this->screenshotFileHeader));
    this->screenshotFileHeader.type = *(u16 *)"BM";
    this->screenshotFileHeader.offBits = 0x36;
    this->screenshotFileHeader.size = this->screenshotFileHeader.offBits;
    strcpy(this->screenshotPath, path);

    switch (this->presentParameters.BackBufferFormat)
    {
    case D3DFMT_R5G6B5:
        utils::DebugPrint(
            "16bit \x82\xcd\x8e\xe6\x82\xe8\x8d\x9e\x82\xdf\x82\xc8\x82\xa2\r\n");
        g_GameErrorContext.Log(
            "16bit \x82\xcd\x8e\xe6\x82\xe8\x8d\x9e\x82\xdf\x82\xc8\x82\xa2\r\n");
        goto cleanup;

    case D3DFMT_X8R8G8B8:
        allocationSize = 0x2c;
        this->screenshotInfoHeader =
            (BITMAPINFOHEADER *)malloc(allocationSize);
        if (this->screenshotInfoHeader == NULL)
        {
            g_GameErrorContext.Log(
                "snapShotScreen : \x8a\x6d\x95\xdb\x82\xb5\x82\xad\x82\xe8\r\n");
            goto cleanup;
        }
        memset(this->screenshotInfoHeader, 0, 0x2c);

        widthBytes = 0x780;
        this->screenshotPixels = (u8 *)malloc(widthBytes * 0x1e0);
        if (this->screenshotPixels == NULL)
        {
            g_GameErrorContext.Log(
                "snapShotScreen : \x8a\x6d\x95\xdb\x82\xb5\x82\xad\x82\xe8\r\n");
            goto cleanup;
        }

        this->screenshotFileHeader.size += widthBytes * 0x1e0;
        this->screenshotInfoHeader->biBitCount = 0x18;
        this->screenshotInfoHeader->biSize = 0x28;
        this->screenshotInfoHeader->biWidth = 0x280;
        this->screenshotInfoHeader->biHeight = 0x1e0;
        this->screenshotInfoHeader->biPlanes = 1;
        this->screenshotInfoHeader->biCompression = 0;

        backbuffer->LockRect(&lockedRect, NULL, 0);
        destinationRow = 0;
        for (y = 0x1df; y > -1; y--, destinationRow++)
        {
            destination = this->screenshotPixels + widthBytes * destinationRow;
            source = (u8 *)lockedRect.pBits + lockedRect.Pitch * y;
            for (x = 0; x < 0x280; x++)
            {
                *(u16 *)destination = *(u16 *)source;
                destination[2] = source[2];
                source += 4;
                destination += 3;
            }
        }
        backbuffer->UnlockRect();
        g_Supervisor.screenshotThread =
            _beginthread((void (__cdecl *)(void *))Supervisor::ScreenshotThread,
                         0, NULL);
        goto cleanup;

    default:
        g_GameErrorContext.Log("error ? .\\mother.cpp\r\n");
        return 1;
    }

cleanup:
    if (backbuffer != NULL)
    {
        backbuffer->Release();
        backbuffer = NULL;
    }
#undef allocationSize
#undef lockedRect
#undef destinationRow
#undef x
#undef y
#undef destination
#undef source
#undef widthBytes
#undef backbuffer
    return 0;
}

// FUNCTION: TH095 0x00425150.
void Supervisor::ThreadClose()
{
    SupervisorReplayScanWorkerView *worker;

    this->EnterCriticalSectionWrapper(6);
    this->criticalSectionLockCounts[6]++;
    worker = (SupervisorReplayScanWorkerView *)&this->replayScanThreadHandle;
    if (worker->handle != NULL)
    {
        CloseHandle(worker->handle);
        worker->handle = NULL;
        worker->active = 0;
    }
    this->LeaveCriticalSectionWrapper(6);
    this->criticalSectionLockCounts[6]--;
}

void Supervisor::InitializeCriticalSections()
{
    for (u32 i = 0; i < 7; i++)
        InitializeCriticalSection(&this->criticalSections[i]);
}

void Supervisor::DeleteCriticalSections()
{
    for (u32 i = 0; i < 7; i++)
        DeleteCriticalSection(&this->criticalSections[i]);
}

void GameConfiguration::Initialize()
{
    memset(this, 0, sizeof(GameConfiguration));
    this->colorMode16bit = 0;
    this->version = 0x95001;
    this->padXAxis = 600;
    this->padYAxis = 600;
    this->musicMode = 1;
    this->playSounds = 1;
    this->windowed = 0;
    this->frameskipConfig = 0;
    this->controllerMapping.bindings[0] = g_ControllerMapping.primaryBindings[0];
    this->controllerMapping.bindings[1] = g_ControllerMapping.primaryBindings[1];
    this->controllerMapping.bindings[2] = g_ControllerMapping.primaryBindings[2];
    this->controllerMapping.bindings[3] = g_ControllerMapping.secondaryBindings[0];
    this->controllerMapping.bindings[4] = g_ControllerMapping.secondaryBindings[1];
    this->controllerMapping.bindings[5] = g_ControllerMapping.secondaryBindings[2];
    this->effectQuality = 2;
    this->musicVolume = 100;
    this->sfxVolume = 80;
    this->unknown0b2 = 0;
    this->unknown0b3 = 1;
    this->unknown0b4 = 2;
}

#define fileSize restartCommandProcessingLocal05
#define configFileBuffer averagedPanLocal12
#define bgmHandle iLocal11
#define bytesRead commandCursorLocal02
#define bgmBuffer soundIndexLocal01
#pragma var_order(fileSize, configFileBuffer, bgmHandle, bytesRead, bgmBuffer)
i32 Supervisor::LoadConfig(char *configFile)
{
    i32 bgmBuffer[4];
    HANDLE bgmHandle;
    DWORD bytesRead;
    u8 *configFileBuffer;
    i32 fileSize;

    g_Supervisor.config.Initialize();
    configFileBuffer = FileSystem::OpenFile(configFile, &fileSize, true);
    if (configFileBuffer == NULL)
    {
        g_GameErrorContext.Log(
            "\x83\x52\x83\x93\x83\x74\x83\x42\x83\x4f\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8c\xa9\x82\xc2\x82\xa9\x82\xe7\x82\xc8\x82\xa2\x82\xcc\x82\xc5\x8f\x89"
            "\x8a\xfa\x89\xbb\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x0d\x0a");
    SET_DEFAULT:
        g_Supervisor.config.Initialize();
        bgmHandle = CreateFileA("./thbgm.dat", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
        if (bgmHandle != INVALID_HANDLE_VALUE)
        {
            ReadFile(bgmHandle, bgmBuffer, 16, &bytesRead, NULL);
            CloseHandle(bgmHandle);
            g_Supervisor.config.musicMode = 1;
        }
        else
        {
            g_Supervisor.config.musicMode = 2;
            utils::DebugPrint(
                "\x77\x61\x76\x65\x20\x83\x66\x81\x5b\x83\x5e\x82\xaa\x96\xb3\x82\xa2"
                "\x82\xcc\x82\xc5\x81\x41\x6d\x69\x64\x69\x20\x82\xc9\x82\xb5\x82\xdc"
                "\x82\xb7\x0d\x0a");
        }
    }
    else
    {
        g_Supervisor.config = *(GameConfiguration *)configFileBuffer;
        free(configFileBuffer);
        if (g_Supervisor.config.colorMode16bit >= 2 || g_Supervisor.config.musicMode >= 3 ||
            g_Supervisor.config.playSounds >= 2 || g_Supervisor.config.windowed >= 2 ||
            g_Supervisor.config.frameskipConfig >= 3 || g_Supervisor.config.effectQuality >= 3 ||
            g_Supervisor.config.version != 0x95001 || fileSize != sizeof(GameConfiguration))
        {
            g_GameErrorContext.Log(
                "\x83\x52\x83\x93\x83\x74\x83\x42\x83\x4f\x83\x66\x81\x5b\x83\x5e\x82\xaa"
                "\x88\xd9\x8f\xed\x82\xc5\x82\xb5\x82\xbd\x82\xcc\x82\xc5\x8d\xc4\x8f\x89"
                "\x8a\xfa\x89\xbb\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x0d\x0a");
            goto SET_DEFAULT;
        }
        g_ControllerMapping.primaryBindings[0] = g_Supervisor.config.controllerMapping.bindings[0];
        g_ControllerMapping.primaryBindings[1] = g_Supervisor.config.controllerMapping.bindings[1];
        g_ControllerMapping.primaryBindings[2] = g_Supervisor.config.controllerMapping.bindings[2];
        g_ControllerMapping.secondaryBindings[0] = g_Supervisor.config.controllerMapping.bindings[3];
        g_ControllerMapping.secondaryBindings[1] = g_Supervisor.config.controllerMapping.bindings[4];
        g_ControllerMapping.secondaryBindings[2] = g_Supervisor.config.controllerMapping.bindings[5];
    }

    this->disableVsync = 0;
    if (this->config.options.disableFog)
        g_GameErrorContext.Log(
            "\x83\x74\x83\x48\x83\x4f\x82\xcc\x8e\x67\x97\x70\x82\xf0\x97\x7d\x90\xa7"
            "\x82\xb5\x82\xdc\x82\xb7\x0d\x0a");
    if (this->config.options.force16BitTextures)
        g_GameErrorContext.Log(
            "\x31\x36\x42\x69\x74\x20\x82\xcc\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83"
            "\x82\xcc\x8e\x67\x97\x70\x82\xf0\x8b\xad\x90\xa7\x82\xb5\x82\xdc\x82\xb7"
            "\x0d\x0a");
    if (this->config.windowed)
        g_GameErrorContext.Log(
            "\x83\x45\x83\x42\x83\x93\x83\x68\x83\x45\x83\x82\x81\x5b\x83\x68\x82\xc5"
            "\x8b\x4e\x93\xae\x82\xb5\x82\xdc\x82\xb7\x0d\x0a");
    if (this->config.options.useReferenceRasterizer)
        g_GameErrorContext.Log(
            "\x83\x8a\x83\x74\x83\x40\x83\x8c\x83\x93\x83\x58\x83\x89\x83\x58\x83\x5e"
            "\x83\x89\x83\x43\x83\x55\x82\xf0\x8b\xad\x90\xa7\x82\xb5\x82\xdc\x82\xb7"
            "\x0d\x0a");
    if (this->config.options.disableDirectInput)
        g_GameErrorContext.Log(
            "\x83\x70\x83\x62\x83\x68\x81\x41\x83\x4c\x81\x5b\x83\x7b\x81\x5b\x83\x68"
            "\x82\xcc\x93\xfc\x97\xcd\x82\xc9\x20\x44\x69\x72\x65\x63\x74\x49\x6e\x70"
            "\x75\x74\x20\x82\xf0\x8e\x67\x97\x70\x82\xb5\x82\xdc\x82\xb9\x82\xf1\x0d"
            "\x0a");
    if (this->config.options.preloadMusic)
        g_GameErrorContext.Log(
            "\x82\x61\x82\x66\x82\x6c\x82\xf0\x83\x81\x83\x82\x83\x8a\x82\xc9\x93\xc7"
            "\xdd\x8d\x9e\x82\xdd\x82\xdc\x82\xb7\x0d\x0a");
    if (this->config.options.disableVsync)
    {
        g_GameErrorContext.Log(
            "\x90\x82\x92\xbc\x93\xaf\x8a\xfa\x82\xf0\x8e\xe6\x82\xe8\x82\xdc\x82\xb9"
            "\x82\xf1\x0d\x0a");
        g_Supervisor.disableVsync = 1;
    }
    if (this->config.options.disableTextBackgroundDetection)
        g_GameErrorContext.Log(
            "\x95\xb6\x8e\x9a\x95\x60\x89\xe6\x82\xcc\x8a\xc2\x8b\xab\x82\xf0\x8e\xa9"
            "\x93\xae\x8c\x9f\x8f\x6f\x82\xb5\x82\xdc\x82\xb9\x82\xf1\x0d\x0a");

    if (FileSystem::WriteDataToFile(configFile, &g_Supervisor.config, sizeof(GameConfiguration)) != 0)
    {
        g_GameErrorContext.Fatal(
            "\x83\x74\x83\x40\x83\x43\x83\x8b\x82\xaa\x8f\x91\x82\xab\x8f\x6f\x82\xb9"
            "\x82\xdc\x82\xb9\x82\xf1\x20\x25\x73\x0d\x0a",
            configFile);
        g_GameErrorContext.Fatal(
            "\x83\x74\x83\x48\x83\x8b\x83\x5f\x82\xaa\x8f\x91\x8d\x9e\x82\xdd\x8b\xd6"
            "\x8e\x7e\x91\xae\x90\xab\x82\xc9\x82\xc8\x82\xc1\x82\xc4\x82\xa2\x82\xe9"
            "\x82\xa9\x81\x41\x83\x66\x83\x42\x83\x58\x83\x4e\x82\xaa\x82\xa2\x82\xc1"
            "\x82\xcf\x82\xa2\x82\xa2\x82\xc1\x82\xcf\x82\xa2\x82\xc9\x82\xc8\x82\xc1"
            "\x82\xc4\x82\xdc\x82\xb9\x82\xf1\x82\xa9\x81\x48\x0d\x0a");
        return -1;
    }
    return 0;
}
#undef fileSize
#undef configFileBuffer
#undef bgmHandle
#undef bytesRead
#undef bgmBuffer

// FUNCTION: TH095 0x004251F0.
i32 Supervisor::LoadMusic(i32 preloadSlot, char *path)
{
    struct LoadMusicLocals
    {
        char wavPath[MAX_PATH];
        char *extension;
    } locals;

#define wavPath locals.wavPath
#define extension locals.extension

    if (g_Supervisor.config.musicMode == 2)
    {
        if (g_Supervisor.midiOutput != NULL)
            g_Supervisor.midiOutput->ReadFileData(preloadSlot, path);
        return 0;
    }
    else if (g_Supervisor.config.musicMode == 1)
    {
        strcpy(wavPath, path);
        extension = strrchr(wavPath, '.');
        extension[1] = 'w';
        extension[2] = 'a';
        extension[3] = 'v';
        g_SoundPlayer.QueueCommand(1, preloadSlot, wavPath);
    }
#undef wavPath
#undef extension
    return 1;
}

// FUNCTION: TH095 0x004252F0.
i32 Supervisor::PlayMusic(i32 musicIndex, i32 unused)
{
    MidiOutput *midiOutput;

    if (g_Supervisor.config.musicMode == 2)
    {
        if (g_Supervisor.midiOutput != NULL)
        {
            midiOutput = g_Supervisor.midiOutput;
            midiOutput->StopPlayback();
            midiOutput->ParseFile(musicIndex);
            midiOutput->Play();
        }
        return 0;
    }
    else if (g_Supervisor.config.musicMode == 1)
    {
        if (g_Supervisor.config.options.preloadMusic)
            g_SoundPlayer.QueueCommand(4, 0, "dummy");
        g_SoundPlayer.QueueCommand(2, musicIndex, "dummy");
    }
    return 0;
}

// FUNCTION: TH095 0x00425390.
i32 Supervisor::StopAudio()
{
    if (g_Supervisor.config.musicMode == 2)
    {
        if (g_Supervisor.midiOutput != NULL)
            g_Supervisor.midiOutput->StopPlayback();
    }
    else if (g_Supervisor.config.musicMode == 1)
    {
        if (g_Supervisor.config.options.preloadMusic)
            g_SoundPlayer.QueueCommand(4, 0, "dummy");
        else
            g_SoundPlayer.QueueCommand(3, 0, "dummy");
    }
    else
    {
        return -1;
    }
    return 0;
}

// FUNCTION: TH095 0x00425410.
i32 Supervisor::FadeOutMusic(f32 durationSeconds)
{
    f32 fadeTime;

    if (g_Supervisor.config.musicMode == 2)
    {
        if (g_Supervisor.midiOutput != NULL)
            g_Supervisor.midiOutput->SetFadeOut(
                (u32)(1000.0f * durationSeconds));
    }
    else if (g_Supervisor.config.musicMode == 1)
    {
        if (g_AnmGameSpeed == 0.0f)
            fadeTime = durationSeconds;
        else if (g_AnmGameSpeed > 1.0f)
            fadeTime = durationSeconds;
        else
            fadeTime = durationSeconds / g_AnmGameSpeed;
        g_SoundPlayer.QueueCommand(5, (i32)fadeTime, "");
    }
    else
    {
        return -1;
    }
    return 0;
}

// FUNCTION: TH095 0x004254D0.
i32 Supervisor::EnableFog()
{
    if (this->fogState != 1)
    {
        g_AnmManager->FlushVertexBuffer();
        this->fogState = 1;
        return this->d3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
    }
    return 0;
}

// FUNCTION: TH095 0x00425520.
i32 Supervisor::DisableFog()
{
    if (this->fogState != 0)
    {
        g_AnmManager->FlushVertexBuffer();
        this->fogState = 0;
        return this->d3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }
    return 0;
}

// FUNCTION: TH095 0x00425570.
void Supervisor::SetRenderState(D3DRENDERSTATETYPE state, i32 value)
{
    g_AnmManager->FlushVertexBuffer();
    this->d3dDevice->SetRenderState(state, value);
}

// FUNCTION: TH095 0x00425660.
void Supervisor::SetupLoadingVms(Float3 *position)
{
    if (this->loadingVmsHaveBeenSetup == 0)
    {
        g_SupervisorLoadingVms[0] = this->loadingAnm->CreateVm(0, 7);
        g_SupervisorLoadingVms[1] = this->loadingAnm->CreateVm(1, 7);
        g_SupervisorLoadingVms[2] = this->loadingAnm->CreateVm(2, 7);
        this->loadingVmsHaveBeenSetup = 1;
        g_AnmManager->SetPosition(g_SupervisorLoadingVms[0], position);
        g_AnmManager->SetPosition(g_SupervisorLoadingVms[1], position);
        g_AnmManager->SetPosition(g_SupervisorLoadingVms[2], position);
    }
}

// FUNCTION: TH095 0x00425730.
void Supervisor::HideLoadingVms()
{
    if (this->loadingVmsHaveBeenSetup == 1)
    {
        g_AnmManager->SetInterrupt(g_SupervisorLoadingVms[0], 1);
        g_AnmManager->SetInterrupt(g_SupervisorLoadingVms[1], 1);
        g_AnmManager->SetInterrupt(g_SupervisorLoadingVms[2], 1);
        g_SupervisorLoadingVms[0] = AnmVmId();
        g_SupervisorLoadingVms[1] = AnmVmId();
        g_SupervisorLoadingVms[2] = AnmVmId();
        this->loadingVmsHaveBeenSetup = 0;
    }
    if (g_SupervisorScreenEffect != NULL)
    {
        g_SupervisorScreenEffect = NULL;
    }
}

// FUNCTION: TH095 0x004257E0.
void Supervisor::BeginLoadingCompletion()
{
    if (this->loadingVmsHaveBeenSetup == 1)
    {
        g_AnmManager->SetInterrupt(g_SupervisorLoadingVms[0], 2);
        g_AnmManager->SetInterrupt(g_SupervisorLoadingVms[1], 2);
        g_AnmManager->SetInterrupt(g_SupervisorLoadingVms[2], 2);
        g_SupervisorLoadingVms[0] = AnmVmId();
        g_SupervisorLoadingVms[1] = AnmVmId();
        g_SupervisorLoadingVms[2] = AnmVmId();
        this->loadingVmsHaveBeenSetup = 2;
    }
    if (g_SupervisorScreenEffect != NULL)
    {
        g_SupervisorScreenEffect = NULL;
    }
}
