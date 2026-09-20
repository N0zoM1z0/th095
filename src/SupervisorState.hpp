#pragma once

namespace th095
{

// Scene-routing domain for the canonical TH095 0x7BC Supervisor owner. The
// enum with the same type name in legacy Supervisor.hpp belongs to a distinct
// TH08-shaped compatibility state machine and has incompatible values.
enum SupervisorState
{
    SUPERVISOR_STATE_EXIT = 1,
    SUPERVISOR_STATE_FRONT_END = 2,
    SUPERVISOR_STATE_PHOTO_GAME = 3,
    SUPERVISOR_STATE_RESTART_PHOTO_GAME = 4,
    SUPERVISOR_STATE_ERROR = 6,
    SUPERVISOR_STATE_START_REPLAY = 7,
    SUPERVISOR_STATE_RETRY_PHOTO_GAME = 8
};

} // namespace th095
