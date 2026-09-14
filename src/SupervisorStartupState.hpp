#ifndef TH095_SUPERVISOR_STARTUP_STATE_HPP
#define TH095_SUPERVISOR_STARTUP_STATE_HPP

namespace th095
{

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
