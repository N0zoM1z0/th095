// Main emits this shared DirectInput device-enumeration callback.
// The target uses global input/controller slots and ignores context. A failed
// creation continues enumeration; any nonnegative result stops, without an
// added output-pointer test. An existing controller stops without another call.
BOOL CALLBACK Supervisor::EnumGameControllersCb(
    LPCDIDEVICEINSTANCEA instance, LPVOID context)
{
    HRESULT result;
    (void)context;

    if (g_Supervisor.controller == NULL)
    {
        result = g_Supervisor.directInput->CreateDevice(
            instance->guidInstance, &g_Supervisor.controller, NULL);
        if (result < 0)
        {
            return DIENUM_CONTINUE;
        }
    }

    return DIENUM_STOP;
}
