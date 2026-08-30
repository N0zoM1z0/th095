#ifndef TH095_MAIN_HPP
#define TH095_MAIN_HPP

#define _WIN32_WINNT 0x0500

#include <d3d8.h>
#include <d3dx8.h>
#include <windows.h>

#include <stddef.h>

namespace th095
{
struct AnmVm;
struct AnmManager;

typedef signed char i8;
typedef unsigned char u8;
typedef signed short i16;
typedef unsigned short u16;
typedef signed int i32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;

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
    u8 usesRelativePath;                 // +0x24
    u8 padding25[3];                     // +0x25
    i32 screenSaveActive;                // +0x28
    i32 lowPowerActive;                  // +0x2c
    i32 powerOffActive;                  // +0x30
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
    u8 colorMode16bit;    // +0xac
    u8 musicMode;         // +0xad
    u8 playSounds;        // +0xae
    u8 windowed;          // +0xaf
    u8 frameskipConfig;   // +0xb0
    u8 effectQuality;     // +0xb1
    u8 unknown0b2;
    u8 unknown0b3;
    u8 unknown0b4;
    u8 musicVolume;       // +0xb5
    u8 sfxVolume;         // +0xb6
    u8 unknown0b7[0x0d];
    GameConfigOptions options;  // +0xc4

    void Initialize();
};

typedef char ControllerBindingSizeIs12[(sizeof(ControllerBinding) == 0x12) ? 1 : -1];
typedef char SerializedControllerMappingSizeIs6C[(sizeof(SerializedControllerMapping) == 0x6c) ? 1 : -1];
typedef char ControllerMappingSizeIsC4[(sizeof(ControllerMapping) == 0xc4) ? 1 : -1];
typedef char GameConfigurationSizeIsC8[(sizeof(GameConfiguration) == 0xc8) ? 1 : -1];

struct MidiOutput
{
    void StopPlayback();
    void UnprepareHeader(LPMIDIHDR header);
    ~MidiOutput();
};

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
    u32 unknown9 : 1;
    u32 keyboardAvailable : 1;
    u32 controllerAvailable : 1;
    u32 unknown12 : 20;
};

typedef i32 (__fastcall *ChainCallback)(void *arg);
typedef i32 (__fastcall *ChainLifetimeCallback)(void *arg);

struct ChainElem
{
    i16 priority;
    u16 isHeapAllocated : 1;
    ChainCallback callback;
    ChainLifetimeCallback addedCallback;
    ChainLifetimeCallback deletedCallback;
    ChainElem *prev;
    ChainElem *next;
    ChainElem *releaseTarget;
    void *arg;
};

#pragma pack(push, 4)
struct Supervisor
{
    HINSTANCE instance;                         // +0x000
    IDirect3D8 *d3dInterface;                   // +0x004
    IDirect3DDevice8 *d3dDevice;                // +0x008
    void *directInput;                          // +0x00c
    void *keyboard;                             // +0x010
    void *controller;                           // +0x014
    u8 unknown018[0x30];
    HWND gameWindow;                            // +0x048
    D3DXMATRIX viewMatrix;                      // +0x04c
    D3DXMATRIX projectionMatrix;                // +0x08c
    D3DVIEWPORT8 viewport;                      // +0x0cc
    D3DPRESENT_PARAMETERS presentParameters;    // +0x0e4
    u8 unknown118[4];
    GameConfiguration config;                   // +0x11c
    u8 unknown1e4[0x220];
    i32 calcCount;                              // +0x404
    i32 wantedState;                            // +0x408
    i32 currentState;                           // +0x40c
    u8 unknown410[0x14];
    i32 screenTransitionCountdown;              // +0x424
    u8 unknown428[4];
    i32 disableVsync;                           // +0x42c
    i32 couldSetRefreshRate;                    // +0x430
    i32 lastFrameTime;                          // +0x434
    MidiOutput *midiOutput;                     // +0x438
    u8 unknown43c[8];
    SupervisorFlags flags;                      // +0x444
    u8 unknown448[8];
    D3DCAPS8 d3dCaps;                           // +0x450
    u8 unknownAfterCaps[0x664 - 0x450 - sizeof(D3DCAPS8)];
    CRITICAL_SECTION criticalSections[7];       // +0x664
    u8 unknown70c[5];
    u8 timestampUsers;                          // +0x711
    u8 unknown712[2];
    i32 loadingVmsHaveBeenSetup;                // +0x714
    u8 unknown718[0x50];
    i32 fogState;                               // +0x768
    u8 unknown76c[0x1c];
    u32 fpsFrameCount;                          // +0x788
    f64 lagNumerator;                           // +0x78c
    f64 lagDenominator;                         // +0x794
    f32 currentFps;                             // +0x79c
    u8 unknown7a0[0x18];
    D3DCOLOR backbufferClearColor;              // +0x7b8
    i32 fpsClockAnomalyCount;                   // +0x7bc
    f64 lastFpsTimestamp;                       // +0x7c0

    void InitializeCriticalSections();
    void DeleteCriticalSections();
    i32 LoadConfig(char *path);
    static i32 RegisterChain();
    void ConfigureGameplayViewport(i32 index);
    void CalculateFps();
    i32 SetupDInput();
    void InitializeInput();
    void DisableFog();
    void ThreadClose();
    void TakeScreenshot(char *path);

    static i32 __fastcall OnUpdate(void *arg);
    static i32 __fastcall AddedCallback(void *arg);
    static i32 __fastcall DeletedCallback(void *arg);
    static i32 __fastcall DrawFpsCounter(Supervisor *s);
    static i32 __fastcall OnDraw2(Supervisor *s);
    static i32 __fastcall FinalizeFrame(Supervisor *s);

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
typedef char SupervisorConfigAt11C[(offsetof(Supervisor, config) == 0x11c) ? 1 : -1];
typedef char SupervisorCapsAt450[(offsetof(Supervisor, d3dCaps) == 0x450) ? 1 : -1];
typedef char SupervisorCriticalSectionsAt664[(offsetof(Supervisor, criticalSections) == 0x664) ? 1 : -1];
typedef char SupervisorLoadingVmsAt714[(offsetof(Supervisor, loadingVmsHaveBeenSetup) == 0x714) ? 1 : -1];
typedef char SupervisorLagNumeratorAt78C[(offsetof(Supervisor, lagNumerator) == 0x78c) ? 1 : -1];
typedef char SupervisorCurrentFpsAt79C[(offsetof(Supervisor, currentFps) == 0x79c) ? 1 : -1];
typedef char SupervisorClearColorAt7B8[(offsetof(Supervisor, backbufferClearColor) == 0x7b8) ? 1 : -1];
typedef char SupervisorLastFpsTimestampAt7C0[(offsetof(Supervisor, lastFpsTimestamp) == 0x7c0) ? 1 : -1];

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

struct SoundPlayer
{
    i32 Initialize(HWND window);
    void RequestThreadStop();
    void JoinThread();
    i32 Release();
    i32 ProcessQueues();
};

struct Chain
{
    i32 RunCalcChain();
    void RunDrawChain();
    void Release();
    void Cut(ChainElem *elem);
    ChainElem *CreateElem(ChainCallback callback);
    i32 AddToCalcChain(ChainElem *elem, i32 priority);
    i32 AddToDrawChain(ChainElem *elem, i32 priority);
};

extern char *g_GameErrorContextCursor;

struct GameErrorContext
{
    enum Message
    {
        LOGGER_START,
        OPTION_CHANGED_RESTART,
        D3D_CREATE_FAILED
    };

    void Log(Message message);
    void Fatal(Message message);
    const char *Log(const char *format, ...);
    const char *Fatal(const char *format, ...);
    char buffer[0x2000];

    void ResetContext()
    {
        g_GameErrorContextCursor = this->buffer;
        *g_GameErrorContextCursor = '\0';
    }
    void Flush();
};

struct Controller
{
    static void GetJoystickCaps();
    static void ResetKeyboard();
};

struct ScreenEffect
{
    static void SetViewport(u32 color);
};

struct FileSystem
{
    static u8 *OpenFile(char *path, i32 *fileSize, i32 isExternalResource);
    static i32 WriteDataToFile(char *path, void *data, i32 size);
    static i32 FileExists(char *path);
};

namespace utils
{
void DebugPrint(char *format, ...);
}

extern GameWindow g_GameWindow;
extern Supervisor g_Supervisor;
extern SoundPlayer g_SoundPlayer;
extern Chain g_Chain;
extern GameErrorContext g_GameErrorContext;
extern AnmManager *g_AnmManager;
extern u16 g_PressedButtons;
extern char g_WindowTitle[];
extern HANDLE g_ExclusiveMutex;
extern ControllerMapping g_ControllerMapping;

} // namespace th095

#endif
