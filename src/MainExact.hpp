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
#include "FileSystemApi.hpp"
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


namespace utils
{
void DebugPrint(char *format, ...);
}

extern GameWindow g_GameWindow;
extern Supervisor g_Supervisor;
extern AnmManager *g_AnmManager;
extern char g_WindowTitle[];
extern HANDLE g_ExclusiveMutex;

} // namespace th095

#endif
