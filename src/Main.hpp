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
#include "GameWindow.hpp"
#include "GameErrorContext.hpp"
#include "GameConfiguration.hpp"
#include "MidiOutputApi.hpp"
#include "ReplayScanWorker.hpp"
#include "ScreenEffect.hpp"
#include "ScreenshotBitmapFileHeader.hpp"
#include "SoundPlayer.hpp"
#include "SupervisorFlags.hpp"
#include "SupervisorFogState.hpp"
#include "SupervisorFpsState.hpp"
#include "SupervisorState.hpp"
#include "SupervisorStartupState.hpp"
#include "SupervisorViewportConfiguration.hpp"
#include "ZunTimer.hpp"
#include "SupervisorRuntime.hpp"
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

// Supervisor storage/API is declared once in SupervisorRuntime.hpp.

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
