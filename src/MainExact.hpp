#ifndef TH095_MAIN_HPP
#define TH095_MAIN_HPP

#define _WIN32_WINNT 0x0500
#define DIRECTINPUT_VERSION 0x0800

#include <d3d8.h>
#include <d3dx8.h>
#include <dinput.h>
#include <windows.h>

#include <stddef.h>
#include "AnmVertex.hpp"
#include "inttypes.hpp"
#include "Chain.hpp"
#include "GameWindow.hpp"
#include "GameConfiguration.hpp"
#include "MidiOutputApi.hpp"
#include "ReplayScanWorker.hpp"
#include "ScreenEffect.hpp"
#include "ScreenshotBitmapFileHeader.hpp"
#include "SupervisorFlags.hpp"
#include "SupervisorFogState.hpp"
#include "SupervisorFpsState.hpp"
#include "SupervisorState.hpp"
#include "SupervisorStartupState.hpp"
#include "SupervisorViewportConfiguration.hpp"
#include "ZunTimer.hpp"
#include "SupervisorRuntime.hpp"

namespace th095
{
struct AnmVm;
struct AnmManager;
struct AnmLoaded;
struct Float3;
struct FrontEndControllerView;
struct PhotoGameTaskView;
struct DummyMidiTimer;

// Supervisor storage/API is declared once in SupervisorRuntime.hpp.


struct SoundPlayer
{
    i32 Initialize(HWND window);
    void RequestThreadStop();
    void JoinThread();
    i32 Release();
    i32 ProcessQueues();
    i32 LoadFmt(char *path);
    void QueueCommand(i32 opcode, i32 argument, char *path);

    u8 unknown000[0x52c4];
    i32 bgmVolume;                           // +0x52c4
    i32 sfxVolume;                           // +0x52c8
    i32 unconsumedBgmAttenuation;            // +0x52cc
};

typedef char MainSoundPlayerBgmVolumeAt52C4[(offsetof(SoundPlayer, bgmVolume) == 0x52c4) ? 1 : -1];

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

struct FileSystem
{
    static u8 *OpenFile(char *path, i32 *fileSize, i32 isExternalResource);
    static i32 WriteDataToFile(char *path, void *data, i32 size);
    static i32 FileExists(char *path);
    static i32 OpenWriteFile(char *path);
    static i32 WriteToOpenFile(void *data, u32 size);
    static i32 CloseWriteFile();
};

namespace utils
{
void DebugPrint(char *format, ...);
}

extern GameWindow g_GameWindow;
extern Supervisor g_Supervisor;
extern SoundPlayer g_SoundPlayer;
extern GameErrorContext g_GameErrorContext;
extern AnmManager *g_AnmManager;
extern u16 g_PressedButtons;
extern char g_WindowTitle[];
extern HANDLE g_ExclusiveMutex;
extern ControllerMapping g_ControllerMapping;

} // namespace th095

#endif
