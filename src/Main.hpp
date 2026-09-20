#ifndef TH095_MAIN_HPP
#define TH095_MAIN_HPP

#define _WIN32_WINNT 0x0500
#define DIRECTINPUT_VERSION 0x0800

#include <d3d8.h>
#include <d3dx8.h>
#include <dinput.h>
#include <windows.h>

#include <stddef.h>
#include "Chain.hpp"
#include "GameErrorContext.hpp"
#include "GameConfiguration.hpp"
#include "MidiOutputApi.hpp"
#include "ReplayScanWorker.hpp"
#include "ScreenEffect.hpp"
#include "ScreenshotBitmapFileHeader.hpp"
#include "SoundPlayer.hpp"
#include "SupervisorFlags.hpp"
#include "SupervisorFogState.hpp"
#include "SupervisorState.hpp"
#include "SupervisorStartupState.hpp"
#include "inttypes.hpp"

namespace th095
{
struct AnmVm;
struct AnmManager;
struct AnmLoaded;
struct Float3;
struct FrontEndControllerView;
struct PhotoGameTaskView;
struct DummyMidiTimer;

enum RenderResult
{
    RENDER_RESULT_KEEP_RUNNING = 0,
    RENDER_RESULT_EXIT_SUCCESS = 1,
    RENDER_RESULT_RESTART = 2,
    RENDER_RESULT_EXIT_ERROR = -1
};

#pragma pack(push, 4)
struct GameWindow
{
    HWND window;                         // +0x00
    i32 windowIsClosing;                 // +0x04
    i32 windowIsActive;                  // +0x08
    i32 windowIsInactive;                // +0x0c
    i8 framesSinceRedraw;                // +0x10
    u8 padding11[3];                     // +0x11
    LARGE_INTEGER performanceFrequency;  // +0x14
    LARGE_INTEGER performanceStart;      // +0x1c
#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
    u8 usesRelativePath;                 // +0x24
#else
    u8 startupPathDiffersFromExecutable; // +0x24
#endif
    u8 padding25[3];                     // +0x25
#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
    i32 screenSaveActive;                // +0x28
    i32 lowPowerActive;                  // +0x2c
    i32 powerOffActive;                  // +0x30
#else
    i32 savedScreenSaverActive;           // +0x28
    i32 savedLowPowerActive;              // +0x2c
    i32 savedPowerOffActive;              // +0x30
#endif
    f64 currentTimestamp;                // +0x34
    f64 lastTimestamp;                   // +0x3c
    f64 lastFrameTime;                   // +0x44
    f64 timeOrigin;                      // +0x4c

    RenderResult Render();
    static void Present();
    f64 GetTimestamp();
    static i32 InitD3DInterface();
    static i32 CreateGameWindow(HINSTANCE instance);
    static LRESULT __stdcall WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
    static i32 InitD3DRendering();
    static void ResetRenderState();
    static i32 CheckForRunningGameInstance(HINSTANCE instance);
    static void ActivateWindow(HWND hWnd);
    static i32 ResolveShortcut(char *shortcutPath, char *destination, i32 destinationSize);
};
#pragma pack(pop)

typedef char GameWindowSizeIs54[(sizeof(GameWindow) == 0x54) ? 1 : -1];
typedef char GameWindowFrequencyAt14[(offsetof(GameWindow, performanceFrequency) == 0x14) ? 1 : -1];
typedef char GameWindowCurrentTimeAt34[(offsetof(GameWindow, currentTimestamp) == 0x34) ? 1 : -1];

#pragma pack(push, 4)
struct Supervisor
{
    HINSTANCE instance;                         // +0x000
    IDirect3D8 *d3dInterface;                   // +0x004
    IDirect3DDevice8 *d3dDevice;                // +0x008
    IDirectInput8A *directInput;                // +0x00c
    IDirectInputDevice8A *keyboard;             // +0x010
    IDirectInputDevice8A *controller;           // +0x014
    DIDEVCAPS controllerCaps;                    // +0x018
    u8 unknown044[4];
    HWND gameWindow;                            // +0x048
    D3DXMATRIX viewMatrix;                      // +0x04c
    D3DXMATRIX projectionMatrix;                // +0x08c
    D3DVIEWPORT8 viewport;                      // +0x0cc
    D3DPRESENT_PARAMETERS presentParameters;    // +0x0e4
    DummyMidiTimer *dummyMidiTimer;              // +0x118
    GameConfiguration config;                   // +0x11c
#ifdef TH095_MATCH_EXACT
    u8 unknown1e4[0x220];
#else
    u8 backgroundViewportConfigurations[0x1e0]; // +0x1e4 (2 * 0xf0)
    void *currentBackgroundViewport;            // +0x3c4
    i32 currentBackgroundViewportIndex;         // +0x3c8
    u8 unknown3cc[0x38];
#endif
    i32 calcCount;                              // +0x404
    i32 activeSceneState;                       // +0x408
    i32 requestedSceneState;                    // +0x40c
    i32 previousActiveSceneState;               // +0x410
    u8 unknown414[0x10];
    i32 screenTransitionCountdown;              // +0x424
    i32 suppressFpsDisplay;                      // +0x428
    i32 disableVsync;                           // +0x42c
    i32 couldSetRefreshRate;                    // +0x430
    i32 lastFrameTime;                          // +0x434
    MidiOutput *midiOutput;                     // +0x438
    AnmLoaded *textAnm;                          // +0x43c
    AnmLoaded *loadingAnm;                       // +0x440
    SupervisorFlags flags;                      // +0x444
    DWORD totalPlayTime;                         // +0x448
    DWORD systemTime;                            // +0x44c
    D3DCAPS8 d3dCaps;                           // +0x450
    u8 unknownAfterCaps[0x528 - 0x450 - sizeof(D3DCAPS8)];
    u32 screenshotWorkerToken;                  // +0x528
    ScreenshotBitmapFileHeader screenshotFileHeader; // +0x52c
    u8 screenshotHeaderPadding[2];              // +0x53a
    BITMAPINFOHEADER *screenshotInfoHeader;      // +0x53c
    u8 *screenshotPixels;                       // +0x540
    char screenshotPath[MAX_PATH];              // +0x544
    ReplayScanWorker replayScanWorker;           // +0x648
    SupervisorStartupPhase startupThreadState;   // +0x660
    CRITICAL_SECTION criticalSections[7];       // +0x664
    u8 criticalSectionLockCounts[7];            // +0x70c
    u8 unknown713;
    // 0: inactive, 1: loading VMs active, >=2: completion/prompt frame counter.
    i32 loadingScreenState;                     // +0x714
    u8 unknown718[0x50];
    SupervisorFogCacheState fogState;           // +0x768
    u8 unknown76c[8];
    i32 versionDataSize;                        // +0x774
    u8 *versionData;                            // +0x778
    u8 unknown77c[4];
    FrontEndControllerView *frontEndController; // +0x780
    PhotoGameTaskView *photoGameTask;           // +0x784
    u32 fpsFrameCount;                          // +0x788
    f64 lagNumerator;                           // +0x78c
    f64 lagDenominator;                         // +0x794
    f32 currentFps;                             // +0x79c
    ReplayScanWorker secondaryReplayScanWorker;  // +0x7a0
    D3DCOLOR backbufferClearColor;              // +0x7b8

    void InitializeCriticalSections();
    void DeleteCriticalSections();
    i32 LoadConfig(char *path);
    static i32 RegisterChain();
    void ConfigureGameplayViewport(i32 index);
    void ConfigureBackgroundViewport(i32 index);
    void CalculateFps();
    i32 SetupDInput();
    static void __fastcall InitializeInput(Supervisor *s);
    static void StartInputWorker();
    void ReleaseGameManagers();
    i32 UpdateSceneState();
    void InitializeViewports();
    static i32 LoadDat();
    static i32 CheckFps();
    void SetupLoadingVms(Float3 *position);
    void HideLoadingVms();
    void BeginLoadingCompletion();
    i32 StartReplayScan(void (__fastcall *callback)(void *), void *argument);
    void StopReplayScan();
    static void __fastcall StartupThread(Supervisor *s);
    i32 LoadMusic(i32 preloadSlot, char *path);
    i32 PlayMusic(i32 musicIndex, i32 unused);
    i32 StopAudio();
    i32 FadeOutMusic(f32 durationSeconds);
    i32 EnableFog();
    i32 DisableFog();
    void SetRenderState(D3DRENDERSTATETYPE state, i32 value);
    void ThreadClose();
    i32 TakeScreenshot(char *path);
    static void __fastcall ScreenshotThread(void *unused);

    static i32 __fastcall OnUpdate(void *arg);
    static i32 __fastcall AddedCallback(Supervisor *s);
    static i32 __fastcall DeletedCallback(void *arg);
    static i32 __fastcall DrawFpsCounter(Supervisor *s);
    static i32 __fastcall OnDraw2(Supervisor *s);
    static i32 __fastcall FinalizeFrame(Supervisor *s);
    static BOOL CALLBACK EnumGameControllersCb(LPCDIDEVICEINSTANCEA instance, LPVOID context);
    static BOOL CALLBACK ControllerCallback(LPCDIDEVICEOBJECTINSTANCEA object, LPVOID context);

    void EnterCriticalSectionWrapper(i32 id)
    {
        EnterCriticalSection(&this->criticalSections[id]);
    }

    void LeaveCriticalSectionWrapper(i32 id)
    {
        LeaveCriticalSection(&this->criticalSections[id]);
    }

    __forceinline bool IsWindowed() const
    {
        return this->config.windowed != 0;
    }
};
#pragma pack(pop)

typedef char SupervisorPresentAtE4[(offsetof(Supervisor, presentParameters) == 0xe4) ? 1 : -1];
typedef char SupervisorDummyMidiTimerAt118[(offsetof(Supervisor, dummyMidiTimer) == 0x118) ? 1 : -1];
typedef char SupervisorControllerCapsAt18[(offsetof(Supervisor, controllerCaps) == 0x18) ? 1 : -1];
#ifndef TH095_MATCH_EXACT
typedef char SupervisorCurrentBackgroundViewportAt3C4[
    (offsetof(Supervisor, currentBackgroundViewport) == 0x3c4) ? 1 : -1];
typedef char SupervisorCurrentBackgroundViewportIndexAt3C8[
    (offsetof(Supervisor, currentBackgroundViewportIndex) == 0x3c8) ? 1 : -1];
#endif
typedef char SupervisorConfigAt11C[(offsetof(Supervisor, config) == 0x11c) ? 1 : -1];
typedef char SupervisorCapsAt450[(offsetof(Supervisor, d3dCaps) == 0x450) ? 1 : -1];
typedef char SupervisorLoadingAnmAt440[(offsetof(Supervisor, loadingAnm) == 0x440) ? 1 : -1];
typedef char SupervisorTextAnmAt43C[(offsetof(Supervisor, textAnm) == 0x43c) ? 1 : -1];
typedef char SupervisorReplayScanAt648[(offsetof(Supervisor, replayScanWorker) == 0x648) ? 1 : -1];
typedef char SupervisorStartupThreadStateAt660[(offsetof(Supervisor, startupThreadState) == 0x660) ? 1 : -1];
typedef char SupervisorCriticalSectionsAt664[(offsetof(Supervisor, criticalSections) == 0x664) ? 1 : -1];
typedef char SupervisorScreenshotWorkerTokenAt528[(offsetof(Supervisor, screenshotWorkerToken) == 0x528) ? 1 : -1];
typedef char SupervisorScreenshotFileHeaderAt52C[(offsetof(Supervisor, screenshotFileHeader) == 0x52c) ? 1 : -1];
typedef char SupervisorScreenshotInfoAt53C[(offsetof(Supervisor, screenshotInfoHeader) == 0x53c) ? 1 : -1];
typedef char SupervisorScreenshotPixelsAt540[(offsetof(Supervisor, screenshotPixels) == 0x540) ? 1 : -1];
typedef char SupervisorScreenshotPathAt544[(offsetof(Supervisor, screenshotPath) == 0x544) ? 1 : -1];
typedef char SupervisorCriticalLockCountsAt70C[(offsetof(Supervisor, criticalSectionLockCounts) == 0x70c) ? 1 : -1];
typedef char SupervisorLoadingScreenStateAt714[(offsetof(Supervisor, loadingScreenState) == 0x714) ? 1 : -1];
typedef char SupervisorLagNumeratorAt78C[(offsetof(Supervisor, lagNumerator) == 0x78c) ? 1 : -1];
typedef char SupervisorVersionDataAt778[(offsetof(Supervisor, versionData) == 0x778) ? 1 : -1];
typedef char SupervisorFrontEndAt780[(offsetof(Supervisor, frontEndController) == 0x780) ? 1 : -1];
typedef char SupervisorGameTaskAt784[(offsetof(Supervisor, photoGameTask) == 0x784) ? 1 : -1];
typedef char SupervisorCurrentFpsAt79C[(offsetof(Supervisor, currentFps) == 0x79c) ? 1 : -1];
typedef char SupervisorClearColorAt7B8[(offsetof(Supervisor, backbufferClearColor) == 0x7b8) ? 1 : -1];
typedef char SupervisorSizeIs7BC[(sizeof(Supervisor) == 0x7bc) ? 1 : -1];

// These values immediately follow g_Supervisor in the target image but are
// independent globals. Keeping them outside Supervisor preserves its
// constructor-proven 0x7bc boundary and the real worker at +0x7a0.
extern i32 g_FpsClockAnomalyCount;
extern f64 g_LastFpsTimestamp;

struct VertexTex1DiffuseXyzrhw
{
    VertexTex1DiffuseXyzrhw()
    {
    }

    f32 x;
    f32 y;
    f32 z;
    f32 w;
    u32 diffuse;
    f32 u;
    f32 v;
};

typedef char MainSoundPlayerBgmVolumeAt52C4[(offsetof(SoundPlayer, bgmVolume) == 0x52c4) ? 1 : -1];

namespace Controller
{
u16 GetJoystickCaps();
void ResetKeyboard();
} // namespace Controller

namespace FileSystem
{
u8 *OpenFile(const char *path, i32 *fileSize, BOOL loadFromDisk);
i32 WriteDataToFile(const char *path, void *data, size_t size);
BOOL CheckIfFileAlreadyExists(const char *path);
i32 OpenWriteFile(char *path);
i32 WriteToOpenFile(void *data, u32 size);
i32 CloseWriteFile();
} // namespace FileSystem

namespace utils
{
void DebugPrint(char *format, ...);
}

extern GameWindow g_GameWindow;
extern Supervisor g_Supervisor;
extern AnmManager *g_AnmManager;
extern u16 g_PressedButtons;
extern char g_WindowTitle[];
extern HANDLE g_ExclusiveMutex;
extern ControllerMapping g_ControllerMapping;

} // namespace th095

#endif
