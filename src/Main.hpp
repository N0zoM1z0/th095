#ifndef TH095_MAIN_HPP
#define TH095_MAIN_HPP

#define _WIN32_WINNT 0x0500

#include <d3d8.h>
#include <windows.h>

#include <stddef.h>

namespace th095
{
struct AnmVm;

typedef signed char i8;
typedef unsigned char u8;
typedef signed short i16;
typedef unsigned short u16;
typedef signed int i32;
typedef unsigned int u32;
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
    static void ActivateWindow(HWND window);
    static i32 ResolveShortcut(char *shortcutPath, char *destination, i32 destinationSize);
};
#pragma pack(pop)

typedef char GameWindowSizeIs54[(sizeof(GameWindow) == 0x54) ? 1 : -1];
typedef char GameWindowFrequencyAt14[(offsetof(GameWindow, performanceFrequency) == 0x14) ? 1 : -1];
typedef char GameWindowCurrentTimeAt34[(offsetof(GameWindow, currentTimestamp) == 0x34) ? 1 : -1];

struct GameConfiguration
{
    u8 unknown000[0xaf];
    u8 windowed;          // +0xaf
    u8 frameskipConfig;   // +0xb0
    u8 unknown0b1[0x17];
};

typedef char GameConfigurationSizeIsC8[(sizeof(GameConfiguration) == 0xc8) ? 1 : -1];

struct MidiOutput
{
    void StopPlayback();
    void UnprepareHeader(LPMIDIHDR header);
    ~MidiOutput();
};

struct SupervisorFlags
{
    u32 unknown0 : 4;
    u32 d3dDeviceNeedsReset : 1;
    u32 unknown5 : 27;
};

struct Supervisor
{
    HINSTANCE instance;                         // +0x000
    IDirect3D8 *d3dInterface;                   // +0x004
    IDirect3DDevice8 *d3dDevice;                // +0x008
    u8 unknown00c[0x3c];
    HWND gameWindow;                            // +0x048
    u8 unknown04c[0x98];
    D3DPRESENT_PARAMETERS presentParameters;    // +0x0e4
    u8 unknown118[4];
    GameConfiguration config;                   // +0x11c
    u8 unknown1e4[0x240];
    i32 screenTransitionCountdown;              // +0x424
    u8 unknown428[0x10];
    MidiOutput *midiOutput;                     // +0x438
    u8 unknown43c[8];
    SupervisorFlags flags;                      // +0x444
    u8 unknown448[0x21c];
    CRITICAL_SECTION criticalSections[7];       // +0x664
    u8 unknown70c[5];
    u8 timestampUsers;                          // +0x711
    u8 unknown712[0x56];
    i32 fogState;                               // +0x768

    void InitializeCriticalSections();
    void DeleteCriticalSections();
    i32 LoadConfig(char *path);
    static i32 RegisterChain();
    void ConfigureGameplayViewport(i32 index);
    void DisableFog();
    void ThreadClose();
    void TakeScreenshot(char *path);

    bool IsWindowed() const
    {
        return this->config.windowed != 0;
    }
};

typedef char SupervisorPresentAtE4[(offsetof(Supervisor, presentParameters) == 0xe4) ? 1 : -1];
typedef char SupervisorConfigAt11C[(offsetof(Supervisor, config) == 0x11c) ? 1 : -1];
typedef char SupervisorCriticalSectionsAt664[(offsetof(Supervisor, criticalSections) == 0x664) ? 1 : -1];

struct AnmManager
{
    u8 unknown000[0x10];
    i32 scriptsStartedThisFrame;  // +0x10
    u8 unknown014[0x38314c - 0x14];

    AnmManager();
    ~AnmManager();
    void ClearVertexBuffer();
    void FlushVertexBuffer();
    void ReleaseSurfaces();
    void TakeScreenshots();
    static i32 ExecuteScript(AnmVm *vm);
};

typedef char AnmManagerSizeIs38314C[(sizeof(AnmManager) == 0x38314c) ? 1 : -1];

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
};

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
    void ResetContext();
    void Flush();
};

struct Controller
{
    static void GetJoystickCaps();
    static void ResetKeyboard();
};

struct FileSystem
{
    static i32 WriteDataToFile(char *path, void *data, i32 size);
    static bool FileExists(char *path);
};

extern GameWindow g_GameWindow;
extern Supervisor g_Supervisor;
extern SoundPlayer g_SoundPlayer;
extern Chain g_Chain;
extern GameErrorContext g_GameErrorContext;
extern AnmManager *g_AnmManager;
extern u16 g_PressedButtons;
extern char g_WindowTitle[];

} // namespace th095

#endif
