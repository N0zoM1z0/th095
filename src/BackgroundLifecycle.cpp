#include "inttypes.hpp"
#include "utils.hpp"

#include <stddef.h>
#include <string.h>

namespace th095
{

// Bounded constructor-only view. Background.cpp intentionally keeps a storage
// view for the rest of the TH095-specific 0x201C runtime object; spelling the
// true non-trivial members here lets VC7 own their construction/EH machinery
// without perturbing the already-exact Background translation unit.
struct ZunTimer
{
    i32 previous;
    f32 subFrame;
    i32 current;

    ZunTimer()
    {
        current = 0;
        previous = -999999;
        subFrame = 0.0f;
    }

    void Initialize()
    {
        current = 0;
        subFrame = 0.0f;
        previous = -999999;
    }
};

struct AnmVm
{
    u8 bytes[0x2cc];
    AnmVm();
    ~AnmVm();
};

struct Background
{
    u8 unknown000[0x10];
    ZunTimer stageScriptTimer;
    u8 unknown01c[4];
    ZunTimer interpolationCurrentTimers[4];
    ZunTimer interpolationEndTimers[4];
    u8 unknown080[0x78];
    AnmVm stageVms[8];
    u8 unknown1758[0x28];
    AnmVm photoAreaVms[3];
    u8 unknown1fe4[0x38];

    Background();
    ~Background();
};

extern Background *g_Background;

typedef char BackgroundLifecycleTimerAt10[
    (offsetof(Background, stageScriptTimer) == 0x10) ? 1 : -1];
typedef char BackgroundLifecycleTimersAt20[
    (offsetof(Background, interpolationCurrentTimers) == 0x20) ? 1 : -1];
typedef char BackgroundLifecycleTimersAt50[
    (offsetof(Background, interpolationEndTimers) == 0x50) ? 1 : -1];
typedef char BackgroundLifecycleStageVmsAtF8[
    (offsetof(Background, stageVms) == 0xf8) ? 1 : -1];
typedef char BackgroundLifecyclePhotoVmsAt1780[
    (offsetof(Background, photoAreaVms) == 0x1780) ? 1 : -1];
typedef char BackgroundLifecycleSizeIs201C[
    (sizeof(Background) == 0x201c) ? 1 : -1];

Background::Background()
{
    utils::DebugPrint("initialize BackGroundInf\n");
    memset(this, 0, sizeof(*this));
    stageScriptTimer.Initialize();
    g_Background = this;
}

} // namespace th095
