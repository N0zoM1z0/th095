enum SupervisorLoadingScreenValue
{
    SUPERVISOR_LOADING_SCREEN_INACTIVE = 0,
    SUPERVISOR_LOADING_SCREEN_VMS_ACTIVE = 1,
    SUPERVISOR_LOADING_SCREEN_COMPLETION_START = 2,
    SUPERVISOR_LOADING_SCREEN_PROMPT_START = 5,
    SUPERVISOR_LOADING_SCREEN_PROMPT_PULSE_TURN = 35,
    SUPERVISOR_LOADING_SCREEN_PROMPT_CYCLE_END = 65,
};

enum SupervisorStartupSurfaceSlotValue
{
    SUPERVISOR_STARTUP_BACKGROUND_SURFACE = 8,
};

static __forceinline void GetSupervisorAnmSurface(
    IDirect3DSurface8 **output, AnmManager *manager, i32 surfaceIndex)
{
    *output = manager->surfaces[surfaceIndex];
}

// FUNCTION: TH095 0x004235D0.
i32 __fastcall Supervisor::OnDraw2(Supervisor *s)
{
    struct OnDraw2Locals
    {
        IDirect3DSurface8 *surface;
        i32 color2;
        i32 color1;
        Float3 position;
    } locals;

    s->ConfigureBackgroundViewport(SUPERVISOR_VIEWPORT_FULL_WINDOW);
    if (g_Supervisor.backbufferClearColor != 0)
    {
        g_Supervisor.d3dDevice->Clear(
            0, NULL, D3DCLEAR_TARGET, g_Supervisor.backbufferClearColor, 1.0f, 0);
    }

    if (s->loadingScreenState >= SUPERVISOR_LOADING_SCREEN_COMPLETION_START)
    {
        s->loadingScreenState++;
        if (s->loadingScreenState >= SUPERVISOR_LOADING_SCREEN_PROMPT_START)
        {
            locals.position.x = 288.0f;
            locals.position.y = 454.0f;
            locals.position.z = 0.0f;
            g_AsciiManager.scaleX = 0.5f;
            g_AsciiManager.scaleY = 0.5f;
            if (s->loadingScreenState < SUPERVISOR_LOADING_SCREEN_PROMPT_PULSE_TURN)
            {
                locals.color1 = 255 - (((s->loadingScreenState - SUPERVISOR_LOADING_SCREEN_PROMPT_START) << 7) / 30);
                g_AsciiManager.color.a = locals.color1;
            }
            else
            {
                locals.color2 = 255 - (((SUPERVISOR_LOADING_SCREEN_PROMPT_CYCLE_END - s->loadingScreenState) << 7) / 30);
                g_AsciiManager.color.a = locals.color2;
            }
            g_AsciiManager.AddFormatText(&locals.position, "Press Shot Button");
            g_AsciiManager.scaleX = 1.0f;
            g_AsciiManager.scaleY = 1.0f;
            g_AsciiManager.DrawStrings();
            g_AsciiManager.numStrings = 0;
            g_AsciiManager.numGuiStrings = 0;

            if (s->loadingScreenState >= SUPERVISOR_LOADING_SCREEN_PROMPT_CYCLE_END)
            {
                s->loadingScreenState = SUPERVISOR_LOADING_SCREEN_PROMPT_START;
            }
        }
    }

    if (s->loadingScreenState != SUPERVISOR_LOADING_SCREEN_INACTIVE)
    {
        g_AnmManager->CopySurfaceToBackbuffer(SUPERVISOR_STARTUP_BACKGROUND_SURFACE, 0, 0, 0, 0);
    }
    else
    {
        GetSupervisorAnmSurface(&locals.surface, g_AnmManager, SUPERVISOR_STARTUP_BACKGROUND_SURFACE);
        if (locals.surface != NULL)
        {
            g_AnmManager->ReleaseSurface(SUPERVISOR_STARTUP_BACKGROUND_SURFACE);
        }
    }
    return 1;
}

// FUNCTION: TH095 0x00423790.
i32 __fastcall Supervisor::DrawFpsCounter(Supervisor *s)
{
    struct DrawFpsCounterLocals
    {
        D3DCOLOR color;
        Float3 position;
        f32 currentFps;
    } locals;

    g_Supervisor.CalculateFps();
    locals.currentFps = g_Supervisor.currentFps;
    if (locals.currentFps < 30.0f)
    {
        locals.color = 0xff5050ff;
    }
    else
    {
        locals.color = locals.currentFps < 40.0f ? 0xffa0a0ff : 0xffffffff;
    }

    g_AsciiManager.color.color = locals.color;
    locals.position.x = 288.0f;
    locals.position.y = 0.0f;
    locals.position.z = 0.0f;
    g_AsciiManager.AddFormatText(
        &locals.position, "%2.1ffps", locals.currentFps);
    g_AsciiManager.color.color = 0xffffffff;
    return 1;
}
