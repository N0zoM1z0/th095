#ifndef TH095_SUPERVISOR_STARTUP_STATE_HPP
#define TH095_SUPERVISOR_STARTUP_STATE_HPP

namespace th095
{

// Canonical TH095 startup-worker protocol at Supervisor +0x660.  The
// SupervisorStartupThreadState declaration in legacy Supervisor.hpp belongs
// to a distinct TH08-shaped 0x364 compatibility layout at +0x294.
enum SupervisorStartupPhase
{
    SUPERVISOR_STARTUP_PHASE_IDLE = 0,
    SUPERVISOR_STARTUP_PHASE_RUNNING = 1,
    SUPERVISOR_STARTUP_PHASE_FAILED = 2,
};

typedef char SupervisorStartupPhaseSizeIs4[
    (sizeof(SupervisorStartupPhase) == 4) ? 1 : -1];

} // namespace th095

#endif
