#include "SupervisorRuntime.hpp"
#include "diffbuild.hpp"

#include <string.h>

namespace th095
{

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
    flags.unknownBit8 = 1;
}

Supervisor::~Supervisor()
{
}

} // namespace th095
