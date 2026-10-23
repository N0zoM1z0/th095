// Compile the actual shared body against real Windows / DirectInput headers.
// The receiver and logger below are explicit test bindings, not the production
// Supervisor layout. COM methods and platform API types come from the SDK.
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <dinput.h>
#include "inttypes.hpp"

namespace th095
{
struct InputSdkConfigurationFixture
{
    struct Options { u32 disableDirectInput; } options;
};
struct InputSdkReceiverFixture
{
    HWND gameWindow;
    InputSdkConfigurationFixture config;
    IDirectInput8A *directInput;
    IDirectInputDevice8A *keyboard;
    IDirectInputDevice8A *controller;
    DIDEVCAPS controllerCaps;
    i32 SetupDInput();
    static BOOL CALLBACK EnumGameControllersCb(LPCDIDEVICEINSTANCEA, LPVOID);
    static BOOL CALLBACK ControllerCallback(LPCDIDEVICEOBJECTINSTANCEA, LPVOID);
};
struct InputSdkLogFixture { const char *Log(const char *, ...); };
extern InputSdkReceiverFixture g_Supervisor;
extern InputSdkLogFixture g_GameErrorContext;

#define Supervisor InputSdkReceiverFixture
#include "SupervisorSetupDInput.inl"
#undef Supervisor

// No casts on the function pointer: SDK return width and calling convention
// must agree with a native signed pointer-sized result in both architectures.
LONG_PTR (WINAPI *inputWindowModuleApi)(HWND, int) = &GetWindowLongPtrA;
typedef char InputSdkLongIsFourBytes[(sizeof(LONG) == 4) ? 1 : -1];
typedef char InputSdkModuleIsPointerWidth[(sizeof(HINSTANCE) == sizeof(void *)) ? 1 : -1];
typedef char InputSdkResultIsPointerWidth[(sizeof(LONG_PTR) == sizeof(void *)) ? 1 : -1];
typedef char InputSdkWindowInstanceIndex[(GWLP_HINSTANCE == -6) ? 1 : -1];
}

extern "C" unsigned int inputSdkAbiReceipt[];
unsigned int inputSdkAbiReceipt[] = {
    sizeof(void *), sizeof(HWND), sizeof(HINSTANCE), sizeof(LONG_PTR),
    sizeof(IDirectInput8A *), sizeof(DIDEVCAPS),
    static_cast<unsigned int>(GWLP_HINSTANCE), sizeof(LONG)
};
