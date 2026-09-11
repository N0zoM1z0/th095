#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <d3d8.h>
#include <d3dx8.h>
#include <dinput.h>
#include <mmsystem.h>
#include <windows.h>

#include <stddef.h>

#include "inttypes.hpp"
#include "MidiRuntime.hpp"

namespace th095
{

struct AnmLoaded;
struct DummyMidiTimer;
struct Float3;
struct FrontEndControllerView;
struct PhotoGameTaskView;

struct ReplayScanWorker
{
    uintptr_t handle;
    u32 threadId;
    i32 stopRequested;
    i32 active;
    u8 unknown010[4];
    void (__fastcall *threadProc)(void *);

    ReplayScanWorker();
    ~ReplayScanWorker();
    void Stop();
    void Start(void (__fastcall *callback)(void *), void *argument);
};

typedef char ReplayScanWorkerSizeIs18[
    (sizeof(ReplayScanWorker) == 0x18) ? 1 : -1];

struct GameConfigOptions
{
    u32 force16BitTextures : 1;
    u32 useReferenceRasterizer : 1;
    u32 disableFog : 1;
    u32 disableDirectInput : 1;
    u32 preloadMusic : 1;
    u32 disableVsync : 1;
    u32 disableTextBackgroundDetection : 1;
    u32 unknown7 : 25;
};

#pragma pack(push, 2)
struct ControllerBinding
{
    u32 inputs[4];
    u16 button;
};

struct SerializedControllerMapping
{
    ControllerBinding bindings[6];
};

struct ControllerMapping
{
    ControllerBinding primaryBindings[3];
    u8 unknown036[0x58];
    ControllerBinding secondaryBindings[3];
};
#pragma pack(pop)

struct GameConfiguration
{
    SerializedControllerMapping controllerMapping;  // +0x00
    u8 unknown06c[0x38];
    u32 version;         // +0xa4
    u16 padXAxis;        // +0xa8
    u16 padYAxis;        // +0xaa
    u8 colorMode16bit;   // +0xac
    u8 musicMode;        // +0xad
    u8 playSounds;       // +0xae
    u8 windowed;         // +0xaf
    u8 frameskipConfig;  // +0xb0
    u8 effectQuality;    // +0xb1
    u8 controllerAssignments[3]; // +0xb2
    i8 musicVolume;      // +0xb5
    i8 sfxVolume;        // +0xb6
    u8 unknown0b7[0x0d];
    GameConfigOptions options; // +0xc4

    void Initialize();
};

typedef char ControllerBindingSizeIs12[(sizeof(ControllerBinding) == 0x12) ? 1 : -1];
typedef char SerializedControllerMappingSizeIs6C[(sizeof(SerializedControllerMapping) == 0x6c) ? 1 : -1];
typedef char ControllerMappingSizeIsC4[(sizeof(ControllerMapping) == 0xc4) ? 1 : -1];
typedef char GameConfigurationSizeIsC8[(sizeof(GameConfiguration) == 0xc8) ? 1 : -1];
typedef char GameConfigurationControllerAssignmentsAtB2[
    (offsetof(GameConfiguration, controllerAssignments) == 0xb2) ? 1 : -1];

struct SupervisorFlags
{
    u32 usingHardwareTL : 1;
    u32 lockableBackbuffer : 1;
    u32 using32BitGraphics : 1;
    u32 speedhackDetected : 1;
    u32 d3dDeviceNeedsReset : 1;
    u32 forceExtraTimerStep : 1;
    u32 dummyMidiTimerEnabled : 1;
    u32 receivedCloseMsg : 1;
    u32 scoreBackupPending : 1;
#if defined(TH095_MATCH_EXACT)
    u32 unknown9 : 1;
#else
    u32 resultRestartActive : 1;
#endif
    u32 keyboardAvailable : 1;
    u32 controllerAvailable : 1;
    u32 restartPhotoGame : 1;
    u32 unknown13 : 19;
};

enum SupervisorState
{
    SUPERVISOR_STATE_EXIT = 1,
    SUPERVISOR_STATE_FRONT_END = 2,
    SUPERVISOR_STATE_PHOTO_GAME = 3,
    SUPERVISOR_STATE_RESTART_PHOTO_GAME = 4,
    SUPERVISOR_STATE_ERROR = 6,
    SUPERVISOR_STATE_START_REPLAY = 7,
    SUPERVISOR_STATE_RETRY_PHOTO_GAME = 8
};

#pragma pack(push, 1)
struct ScreenshotBitmapFileHeader
{
    u16 type;
    u32 size;
    u16 reserved1;
    u16 reserved2;
    u32 offBits;
};
#pragma pack(pop)

typedef char ScreenshotBitmapFileHeaderSizeIs0E[
    (sizeof(ScreenshotBitmapFileHeader) == 0x0e) ? 1 : -1];

#pragma pack(push, 4)
struct Supervisor
{
    HINSTANCE instance;                              // +0x000
    IDirect3D8 *d3dInterface;                        // +0x004
    IDirect3DDevice8 *d3dDevice;                     // +0x008
    IDirectInput8A *directInput;                     // +0x00c
    IDirectInputDevice8A *keyboard;                  // +0x010
    IDirectInputDevice8A *controller;                // +0x014
    DIDEVCAPS controllerCaps;                         // +0x018
    u8 unknown044[4];
    HWND gameWindow;                                 // +0x048
    D3DXMATRIX viewMatrix;                           // +0x04c
    D3DXMATRIX projectionMatrix;                     // +0x08c
    D3DVIEWPORT8 viewport;                           // +0x0cc
    D3DPRESENT_PARAMETERS presentParameters;         // +0x0e4
    DummyMidiTimer *dummyMidiTimer;                  // +0x118
    GameConfiguration config;                        // +0x11c
#ifdef TH095_MATCH_EXACT
    u8 unknown1e4[0x220];
#else
    u8 backgroundViewportConfigurations[0x1e0]; // +0x1e4 (2 * 0xf0)
    void *currentBackgroundViewport;             // +0x3c4
    i32 currentBackgroundViewportIndex;          // +0x3c8
    u8 unknown3cc[0x38];
#endif
    i32 calcCount;                                   // +0x404
    i32 wantedState;                                 // +0x408
    i32 currentState;                                // +0x40c
    i32 previousState;                               // +0x410
    u8 unknown414[0x10];
    i32 screenTransitionCountdown;                   // +0x424
    i32 suppressFpsDisplay;                          // +0x428
    i32 disableVsync;                                // +0x42c
    i32 couldSetRefreshRate;                         // +0x430
    i32 lastFrameTime;                               // +0x434
    MidiOutput *midiOutput;                          // +0x438
    AnmLoaded *textAnm;                              // +0x43c
    AnmLoaded *loadingAnm;                           // +0x440
    SupervisorFlags flags;                           // +0x444
    DWORD totalPlayTime;                             // +0x448
    DWORD systemTime;                                // +0x44c
    D3DCAPS8 d3dCaps;                                // +0x450
    u8 unknownAfterCaps[0x528 - 0x450 - sizeof(D3DCAPS8)];
    u32 screenshotThread;                            // +0x528
    ScreenshotBitmapFileHeader screenshotFileHeader;// +0x52c
    u8 screenshotHeaderPadding[2];                   // +0x53a
    BITMAPINFOHEADER *screenshotInfoHeader;           // +0x53c
    u8 *screenshotPixels;                            // +0x540
    char screenshotPath[MAX_PATH];                   // +0x544
    ReplayScanWorker replayScanWorker;               // +0x648
    i32 startupThreadState;                          // +0x660
    CRITICAL_SECTION criticalSections[7];            // +0x664
    u8 criticalSectionLockCounts[7];                 // +0x70c
    u8 unknown713;
    i32 loadingVmsHaveBeenSetup;                     // +0x714
    u8 unknown718[0x50];
    i32 fogState;                                    // +0x768
    u8 unknown76c[8];
    i32 versionDataSize;                             // +0x774
    u8 *versionData;                                 // +0x778
    u8 unknown77c[4];
    FrontEndControllerView *frontEndController;      // +0x780
    PhotoGameTaskView *photoGameTask;                // +0x784
    u32 fpsFrameCount;                               // +0x788
    f64 lagNumerator;                                // +0x78c
    f64 lagDenominator;                              // +0x794
    f32 currentFps;                                  // +0x79c
    ReplayScanWorker secondaryReplayScanWorker;      // +0x7a0
    D3DCOLOR backbufferClearColor;                   // +0x7b8

    Supervisor();
    ~Supervisor();

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
    static BOOL CALLBACK EnumGameControllersCb(
        LPCDIDEVICEINSTANCEA instance, LPVOID context);
    static BOOL CALLBACK ControllerCallback(
        LPCDIDEVICEOBJECTINSTANCEA object, LPVOID context);

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

typedef char SupervisorSizeIs7BC[(sizeof(Supervisor) == 0x7bc) ? 1 : -1];
typedef char SupervisorPresentAtE4[(offsetof(Supervisor, presentParameters) == 0xe4) ? 1 : -1];
typedef char SupervisorConfigAt11C[(offsetof(Supervisor, config) == 0x11c) ? 1 : -1];
#ifndef TH095_MATCH_EXACT
typedef char SupervisorCurrentBackgroundViewportAt3C4[
    (offsetof(Supervisor, currentBackgroundViewport) == 0x3c4) ? 1 : -1];
typedef char SupervisorCurrentBackgroundViewportIndexAt3C8[
    (offsetof(Supervisor, currentBackgroundViewportIndex) == 0x3c8) ? 1 : -1];
#endif
typedef char SupervisorCapsAt450[(offsetof(Supervisor, d3dCaps) == 0x450) ? 1 : -1];
typedef char SupervisorReplayScanAt648[(offsetof(Supervisor, replayScanWorker) == 0x648) ? 1 : -1];
typedef char SupervisorCriticalSectionsAt664[(offsetof(Supervisor, criticalSections) == 0x664) ? 1 : -1];
typedef char SupervisorCriticalLockCountsAt70C[(offsetof(Supervisor, criticalSectionLockCounts) == 0x70c) ? 1 : -1];
typedef char SupervisorLoadingVmsAt714[(offsetof(Supervisor, loadingVmsHaveBeenSetup) == 0x714) ? 1 : -1];
typedef char SupervisorSecondaryReplayScanAt7A0[(offsetof(Supervisor, secondaryReplayScanWorker) == 0x7a0) ? 1 : -1];
typedef char SupervisorClearColorAt7B8[(offsetof(Supervisor, backbufferClearColor) == 0x7b8) ? 1 : -1];

extern Supervisor g_Supervisor;
extern ControllerMapping g_ControllerMapping;

} // namespace th095
