// Main emits this shared fastcall input-worker callback.
// Target 0x004238E0 ignores the supplied argument and always initializes the
// global Supervisor. Clear availability before setup, then derive it from
// pointers regardless of SetupDInput's return value; preserve unrelated bits.
void __fastcall Supervisor::InitializeInput(void *s)
{
    (void)s;
    g_Supervisor.flags.keyboardAvailable = 0;
    g_Supervisor.flags.controllerAvailable = 0;
    g_Supervisor.SetupDInput();
    g_Supervisor.flags.keyboardAvailable = g_Supervisor.keyboard != NULL;
    g_Supervisor.flags.controllerAvailable = g_Supervisor.controller != NULL;
}
