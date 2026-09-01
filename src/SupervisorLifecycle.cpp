#include "inttypes.hpp"

#include <stddef.h>
#include <string.h>

namespace th095
{

// Constructor-only views for the target-owned 0x7BC prefix. Main.hpp keeps the
// wider runtime layout used by other exact units; this TU isolates member/EH
// allocation phase just as the target constructor does.
struct GameConfiguration
{
    u8 bytes[0xc8];
    void Initialize();

    GameConfiguration()
    {
        Initialize();
    }
};

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

struct ReplayScanWorker
{
    u32 handle;
    u32 threadId;
    i32 stopRequested;
    i32 active;
    u8 unknown010[4];
    void (__fastcall *threadProc)(void *);

    ReplayScanWorker();
    ~ReplayScanWorker();
};

struct Supervisor
{
    u8 unknown000[0x11c];
    GameConfiguration config;
    SupervisorViewportLifecycle backgroundViewports[2];
    u8 unknown3c4[0x30];
    SupervisorTimerLifecycle timer;
    u8 unknown400[0x44];
    u32 flags;
    u8 unknown448[0x200];
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
typedef char SupervisorLifecycleWorkerAt648[
    (offsetof(Supervisor, replayWorker) == 0x648) ? 1 : -1];
typedef char SupervisorLifecycleWorker2At7A0[
    (offsetof(Supervisor, secondaryWorker) == 0x7a0) ? 1 : -1];
typedef char SupervisorLifecycleSizeIs7BC[
    (sizeof(Supervisor) == 0x7bc) ? 1 : -1];

Supervisor::Supervisor()
{
    memset(this, 0, sizeof(*this));
    flags |= 0x40;
    flags |= 0x100;
}

Supervisor::~Supervisor()
{
}

} // namespace th095
