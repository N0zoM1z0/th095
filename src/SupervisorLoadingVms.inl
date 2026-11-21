// FUNCTION: TH095 0x00425660.
void Supervisor::SetupLoadingVms(Float3 *position)
{
    if (this->loadingScreenState == SUPERVISOR_LOADING_SCREEN_INACTIVE)
    {
        g_SupervisorLoadingVms[0] = this->loadingAnm->CreateVm(0, 7);
        g_SupervisorLoadingVms[1] = this->loadingAnm->CreateVm(1, 7);
        g_SupervisorLoadingVms[2] = this->loadingAnm->CreateVm(2, 7);
        this->loadingScreenState = SUPERVISOR_LOADING_SCREEN_VMS_ACTIVE;
        g_AnmManager->SetPosition(g_SupervisorLoadingVms[0], position);
        g_AnmManager->SetPosition(g_SupervisorLoadingVms[1], position);
        g_AnmManager->SetPosition(g_SupervisorLoadingVms[2], position);
    }
}

// FUNCTION: TH095 0x00425730.
void Supervisor::HideLoadingVms()
{
    if (this->loadingScreenState == SUPERVISOR_LOADING_SCREEN_VMS_ACTIVE)
    {
        g_AnmManager->SetInterrupt(g_SupervisorLoadingVms[0], 1);
        g_AnmManager->SetInterrupt(g_SupervisorLoadingVms[1], 1);
        g_AnmManager->SetInterrupt(g_SupervisorLoadingVms[2], 1);
        g_SupervisorLoadingVms[0] = AnmVmId();
        g_SupervisorLoadingVms[1] = AnmVmId();
        g_SupervisorLoadingVms[2] = AnmVmId();
        this->loadingScreenState = SUPERVISOR_LOADING_SCREEN_INACTIVE;
    }
    if (g_SupervisorScreenEffect != NULL)
    {
        g_SupervisorScreenEffect = NULL;
    }
}

// FUNCTION: TH095 0x004257E0.
void Supervisor::BeginLoadingCompletion()
{
    if (this->loadingScreenState == SUPERVISOR_LOADING_SCREEN_VMS_ACTIVE)
    {
        g_AnmManager->SetInterrupt(g_SupervisorLoadingVms[0], 2);
        g_AnmManager->SetInterrupt(g_SupervisorLoadingVms[1], 2);
        g_AnmManager->SetInterrupt(g_SupervisorLoadingVms[2], 2);
        g_SupervisorLoadingVms[0] = AnmVmId();
        g_SupervisorLoadingVms[1] = AnmVmId();
        g_SupervisorLoadingVms[2] = AnmVmId();
        this->loadingScreenState = SUPERVISOR_LOADING_SCREEN_COMPLETION_START;
    }
    if (g_SupervisorScreenEffect != NULL)
    {
        g_SupervisorScreenEffect = NULL;
    }
}
