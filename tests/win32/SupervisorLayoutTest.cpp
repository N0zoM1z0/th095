// Compile-only Win32 owner oracle using actual SDK and production declarations.
// Dependencies are loaded before checking owner pack restoration. No SDK or
// Supervisor fixture replaces a production type; this is not a Win64 ABI claim.
#include <d3d8.h>
#include <d3dx8.h>
#include <dinput.h>
#include <mmsystem.h>
#include <windows.h>
#include <stddef.h>
#include "inttypes.hpp"
#include "GameConfiguration.hpp"
#include "SupervisorMemberConstruction.hpp"
#include "MidiOutputApi.hpp"
#include "ReplayScanWorker.hpp"
#include "ScreenshotBitmapFileHeader.hpp"
#include "SupervisorFlags.hpp"
#include "SupervisorState.hpp"
#include "SupervisorStartupState.hpp"
#include "SupervisorFogState.hpp"
#include "SupervisorViewportConfiguration.hpp"
#include "ZunTimer.hpp"

#pragma pack(push, 1)
#include "SupervisorRuntime.hpp"
struct SupervisorCallerPackingProbe { unsigned char prefix; unsigned int value; };
#pragma pack(pop)

#define SUPERVISOR_LAYOUT_FIELDS(X) \
    X(instance, 0x000) \
    X(d3dInterface, 0x004) \
    X(d3dDevice, 0x008) \
    X(directInput, 0x00c) \
    X(keyboard, 0x010) \
    X(controller, 0x014) \
    X(controllerCaps, 0x018) \
    X(unknown044, 0x044) \
    X(gameWindow, 0x048) \
    X(viewMatrix, 0x04c) \
    X(projectionMatrix, 0x08c) \
    X(viewport, 0x0cc) \
    X(presentParameters, 0x0e4) \
    X(dummyMidiTimer, 0x118) \
    X(config, 0x11c) \
    X(viewportConfigurations, 0x1e4) \
    X(currentViewportConfiguration, 0x3c4) \
    X(currentViewportIndex, 0x3c8) \
    X(unknown3cc, 0x3cc) \
    X(timer, 0x3f4) \
    X(unknown400, 0x400) \
    X(calcCount, 0x404) \
    X(activeSceneState, 0x408) \
    X(requestedSceneState, 0x40c) \
    X(previousActiveSceneState, 0x410) \
    X(unknown414, 0x414) \
    X(screenTransitionCountdown, 0x424) \
    X(suppressFpsDisplay, 0x428) \
    X(disableVsync, 0x42c) \
    X(couldSetRefreshRate, 0x430) \
    X(lastFrameTime, 0x434) \
    X(midiOutput, 0x438) \
    X(textAnm, 0x43c) \
    X(loadingAnm, 0x440) \
    X(flags, 0x444) \
    X(totalPlayTime, 0x448) \
    X(systemTime, 0x44c) \
    X(d3dCaps, 0x450) \
    X(unknownAfterCaps, 0x524) \
    X(screenshotWorkerToken, 0x528) \
    X(screenshotFileHeader, 0x52c) \
    X(screenshotHeaderPadding, 0x53a) \
    X(screenshotInfoHeader, 0x53c) \
    X(screenshotPixels, 0x540) \
    X(screenshotPath, 0x544) \
    X(replayScanWorker, 0x648) \
    X(startupThreadState, 0x660) \
    X(criticalSections, 0x664) \
    X(criticalSectionLockCounts, 0x70c) \
    X(unknown713, 0x713) \
    X(loadingScreenState, 0x714) \
    X(unknown718, 0x718) \
    X(fogState, 0x768) \
    X(unknown76c, 0x76c) \
    X(versionDataSize, 0x774) \
    X(versionData, 0x778) \
    X(unknown77c, 0x77c) \
    X(frontEndController, 0x780) \
    X(photoGameTask, 0x784) \
    X(fpsFrameCount, 0x788) \
    X(lagNumerator, 0x78c) \
    X(lagDenominator, 0x794) \
    X(currentFps, 0x79c) \
    X(secondaryReplayScanWorker, 0x7a0) \
    X(backbufferClearColor, 0x7b8)

#define CHECK_SUPERVISOR_FIELD(member, expected) \
    typedef char SupervisorField_##member[(offsetof(th095::Supervisor, member) == expected) ? 1 : -1];
SUPERVISOR_LAYOUT_FIELDS(CHECK_SUPERVISOR_FIELD)
#undef CHECK_SUPERVISOR_FIELD

typedef char SupervisorCallerPackRestored[(offsetof(SupervisorCallerPackingProbe, value) == 1) ? 1 : -1];

// Uncast SDK callback bindings check the canonical API independently of native fixtures.
LPDIENUMDEVICESCALLBACKA supervisorDeviceCallbackApi = &th095::Supervisor::EnumGameControllersCb;
LPDIENUMDEVICEOBJECTSCALLBACKA supervisorObjectCallbackApi = &th095::Supervisor::ControllerCallback;
void (__fastcall *supervisorInputWorkerApi)(void *) = &th095::Supervisor::InitializeInput;

extern "C" unsigned int supervisorLayoutAbiReceipt[];
#define SUPERVISOR_FIELD_RECEIPT(member, expected) offsetof(th095::Supervisor, member),
unsigned int supervisorLayoutAbiReceipt[] = {
    sizeof(void *), sizeof(th095::Supervisor), sizeof(SupervisorCallerPackingProbe),
    offsetof(SupervisorCallerPackingProbe, value),
    SUPERVISOR_LAYOUT_FIELDS(SUPERVISOR_FIELD_RECEIPT)
    sizeof(th095::GameConfiguration), sizeof(th095::SupervisorViewportConfiguration),
    sizeof(th095::ZunTimer), sizeof(th095::ReplayScanWorker),
    sizeof(th095::SupervisorFlags), sizeof(D3DCAPS8), sizeof(DIDEVCAPS), sizeof(CRITICAL_SECTION)
};
#undef SUPERVISOR_FIELD_RECEIPT
#undef SUPERVISOR_LAYOUT_FIELDS
