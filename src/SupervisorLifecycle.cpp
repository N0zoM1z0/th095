#include "inttypes.hpp"
#include "GameConfiguration.hpp"
#include "ReplayScanWorker.hpp"
#include "SupervisorFlags.hpp"
#include "diffbuild.hpp"

#include <stddef.h>
#include <string.h>

namespace th095
{

// Constructor-only views for the target-owned 0x7BC prefix. Main.hpp keeps the
// wider runtime layout used by other exact units; this TU isolates member/EH
// allocation phase just as the target constructor does.
// The target Supervisor constructor invokes GameConfiguration::Initialize as
// a member-construction phase before entering its body. Keep that compiler
// boundary without duplicating the canonical configuration layout.
struct GameConfigurationConstructionAdapter : GameConfiguration
{
    GameConfigurationConstructionAdapter()
    {
        Initialize();
    }
};

typedef char GameConfigurationConstructionAdapterSizeIsC8[
    (sizeof(GameConfigurationConstructionAdapter) == 0xc8) ? 1 : -1];

struct SupervisorViewportLifecycle
{
    u8 bytes[0xf0];
    SupervisorViewportLifecycle() {}
};

struct SupervisorTimerLifecycle
{
    i32 previous;
    f32 subFrame;
    i32 current;

    SupervisorTimerLifecycle()
    {
        current = 0;
        previous = -999999;
        subFrame = 0.0f;
    }
};

struct Supervisor
{
    u8 unknown000[0x11c];
    GameConfigurationConstructionAdapter config;
    SupervisorViewportLifecycle backgroundViewports[2];
    u8 unknown3c4[0x30];
    SupervisorTimerLifecycle timer;
    u8 unknown400[0x44];
    SupervisorFlags flags;
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
    u8 unknown448[0x200];
#else
    u8 unknown448[0x528 - 0x448];
    u32 screenshotWorkerToken;
    u8 unknown52c[0x648 - 0x52c];
#endif
    ReplayScanWorker replayWorker;
    u8 unknown660[0x140];
    ReplayScanWorker secondaryWorker;
    u32 backbufferClearColor;

    Supervisor();
    ~Supervisor();
};

typedef char SupervisorLifecycleConfigAt11C[
    (offsetof(Supervisor, config) == 0x11c) ? 1 : -1];
typedef char SupervisorLifecycleViewportsAt1E4[
    (offsetof(Supervisor, backgroundViewports) == 0x1e4) ? 1 : -1];
typedef char SupervisorLifecycleTimerAt3F4[
    (offsetof(Supervisor, timer) == 0x3f4) ? 1 : -1];
typedef char SupervisorLifecycleFlagsAt444[
    (offsetof(Supervisor, flags) == 0x444) ? 1 : -1];
#if !defined(DIFFBUILD) && !defined(TH095_MATCH_EXACT)
typedef char SupervisorLifecycleScreenshotWorkerTokenAt528[
    (offsetof(Supervisor, screenshotWorkerToken) == 0x528) ? 1 : -1];
#endif
typedef char SupervisorLifecycleWorkerAt648[
    (offsetof(Supervisor, replayWorker) == 0x648) ? 1 : -1];
typedef char SupervisorLifecycleWorker2At7A0[
    (offsetof(Supervisor, secondaryWorker) == 0x7a0) ? 1 : -1];
typedef char SupervisorLifecycleSizeIs7BC[
    (sizeof(Supervisor) == 0x7bc) ? 1 : -1];

// The verified target's static initializer at 0x00494040 constructs
// Supervisor::Supervisor on 0x004C4670 and registers the destructor wrapper at
// 0x00494270, which destroys the same storage.  Keep the production definition
// beside the exact constructor/destructor source so VC7.1 emits the real owner
// relationship.  DIFFBUILD deliberately externalizes it, preserving the
// address-bound canonical comparison units.
DIFFABLE_STATIC(Supervisor, g_Supervisor);

Supervisor::Supervisor()
{
    memset(this, 0, sizeof(*this));
    flags.dummyMidiTimerEnabled = 1;
    // No independent TH095-local consumer has established bit 8's role.
    flags.raw |= 0x100;
}

Supervisor::~Supervisor()
{
}

} // namespace th095
