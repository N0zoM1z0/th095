// Compile-only ABI oracle using real Windows headers, with no Main/D3D owner
// fixtures. See SEM-340 for VC7.1 x86 and MinGW-w64 x86/x64 commands.
#include <windows.h>
#include <stddef.h>

// The owner must restore its includer's packing even when included alone.
#pragma pack(push, 1)
#include "GameWindow.hpp"
#include "GameWindow.hpp"
struct GameWindowIncluderPacking { char byte; void *pointer; };
typedef char GameWindowRestoresIncluderPacking[
    (sizeof(GameWindowIncluderPacking) == 1 + sizeof(void *)) ? 1 : -1];
#pragma pack(pop)

using th095::GameWindow;

// Independently pin every target field, then check the native-width Win32
// extension. These are runtime object offsets, not a file-format contract.
#define CHECK_WINDOW_FIELD(field, targetOffset) \
    typedef char GameWindowOffset_##field[ \
        (offsetof(GameWindow, field) == (targetOffset) + sizeof(HWND) - 4) ? 1 : -1]
typedef char GameWindowHandleAtZero[(offsetof(GameWindow, window) == 0) ? 1 : -1];
CHECK_WINDOW_FIELD(windowIsClosing, 0x04);
CHECK_WINDOW_FIELD(windowIsActive, 0x08);
CHECK_WINDOW_FIELD(windowIsInactive, 0x0c);
CHECK_WINDOW_FIELD(framesSinceRedraw, 0x10);
CHECK_WINDOW_FIELD(performanceFrequency, 0x14);
CHECK_WINDOW_FIELD(performanceStart, 0x1c);
CHECK_WINDOW_FIELD(startupPathDiffersFromExecutable, 0x24);
CHECK_WINDOW_FIELD(savedScreenSaverActive, 0x28);
CHECK_WINDOW_FIELD(savedLowPowerActive, 0x2c);
CHECK_WINDOW_FIELD(savedPowerOffActive, 0x30);
CHECK_WINDOW_FIELD(currentTimestamp, 0x34);
CHECK_WINDOW_FIELD(lastTimestamp, 0x3c);
CHECK_WINDOW_FIELD(lastFrameTime, 0x44);
CHECK_WINDOW_FIELD(timeOrigin, 0x4c);
#undef CHECK_WINDOW_FIELD

typedef char GameWindowSizeForWin32OrWin64[
    (sizeof(GameWindow) == (sizeof(void *) == 4 ? 0x54 : 0x58)) ? 1 : -1];
typedef char GameWindowPointerWidthForWindows[
    (sizeof(void *) == 4 || sizeof(void *) == 8) ? 1 : -1];
typedef char GameWindowNativeMessageWidths[
    (sizeof(WPARAM) == sizeof(void *) && sizeof(LPARAM) == sizeof(void *) &&
     sizeof(LRESULT) == sizeof(void *)) ? 1 : -1];

// No casts: the SDK must accept the real callback and QPC/handle fields.
WNDPROC gameWindowCallbackAbi = &GameWindow::WindowProc;
void CheckGameWindowApiTypes(GameWindow &window)
{
    QueryPerformanceFrequency(&window.performanceFrequency);
    QueryPerformanceCounter(&window.performanceStart);
    DestroyWindow(window.window);
}

// Readable from the object without pretending to execute a Windows program.
extern "C" unsigned int gameWindowAbiReceipt[];
unsigned int gameWindowAbiReceipt[] = {
    sizeof(void *), sizeof(HWND), sizeof(WPARAM), sizeof(LPARAM), sizeof(LRESULT),
    sizeof(GameWindow), offsetof(GameWindow, performanceFrequency),
    offsetof(GameWindow, performanceStart), offsetof(GameWindow, currentTimestamp),
    offsetof(GameWindow, timeOrigin)
};
