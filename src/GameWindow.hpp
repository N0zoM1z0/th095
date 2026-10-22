#ifndef TH095_GAME_WINDOW_HPP
#define TH095_GAME_WINDOW_HPP

#include <stddef.h>
#include <windows.h>
#include "GameWindowRenderResult.hpp"
#include "inttypes.hpp"

namespace th095
{
// Win32 window/frame-loop owner; Main.cpp owns g_GameWindow and its methods.
// Offsets below describe the verified x86 target, not a serialized format.
// HWND keeps the platform pointer width. The remaining fields keep their
// target scalar widths, so x64 moves the tail by sizeof(HWND) - 4 bytes.
// Pack(4) preserves the target's QPC/double alignment; the two three-byte
// gaps after byte fields are ordinary alignment padding, not runtime state.
#pragma pack(push, 4)
struct GameWindow
{
    HWND window;                         // +0x00
    i32 windowIsClosing;                 // +0x04
    i32 windowIsActive;                  // +0x08
    i32 windowIsInactive;                // +0x0c
    i8 framesSinceRedraw;                // +0x10
    LARGE_INTEGER performanceFrequency;  // +0x14
    LARGE_INTEGER performanceStart;      // +0x1c
    u8 startupPathDiffersFromExecutable; // +0x24
    i32 savedScreenSaverActive;          // +0x28
    i32 savedLowPowerActive;             // +0x2c
    i32 savedPowerOffActive;             // +0x30
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

// One declaration serves x86 and x64 Windows. These relationships retain the
// target's 0x54 / +0x14 / +0x34 contract when HWND is four bytes, without
// truncating a native handle or making the whole owner a fixed-size packet.
typedef char GameWindowHandleIsNativeWidth[(sizeof(HWND) == sizeof(void *)) ? 1 : -1];
typedef char GameWindowCounterIsEightBytes[(sizeof(LARGE_INTEGER) == 8) ? 1 : -1];
typedef char GameWindowSizeFollowsHandle[(sizeof(GameWindow) == sizeof(HWND) + 0x50) ? 1 : -1];
typedef char GameWindowFrequencyFollowsHandle[
    (offsetof(GameWindow, performanceFrequency) == sizeof(HWND) + 0x10) ? 1 : -1];
typedef char GameWindowCurrentTimeFollowsHandle[
    (offsetof(GameWindow, currentTimestamp) == sizeof(HWND) + 0x30) ? 1 : -1];
}

#endif
