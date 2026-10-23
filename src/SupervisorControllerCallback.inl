// Canonical normal callback: context is unused and owns no storage/device.
// VC7.1 emits 104 bytes without the target's unused context copy. The existing
// MainExact.inl wrapper retains that 109-byte target shape; both wrappers use
// the shared semantic statements below. See WINDOW_INPUT_INITIALIZATION.md.
BOOL CALLBACK Supervisor::ControllerCallback(
    LPCDIDEVICEOBJECTINSTANCEA object, LPVOID context)
{
    DIPROPRANGE range;
    (void)context;
#include "SupervisorControllerAxisRange.inl"
}
