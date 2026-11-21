// FUNCTION: TH095 0x00423D00.
void Supervisor::ReleaseGameManagers()
{
    if (this->frontEndController != NULL)
    {
        this->frontEndController->Destroy();
    }
    this->frontEndController = NULL;

    if (this->photoGameTask != NULL)
    {
        this->photoGameTask->Destroy();
    }
    this->photoGameTask = NULL;
}

// FUNCTION: TH095 0x00425EF0.
i32 Supervisor::UpdateSceneState()
{
    struct
    {
        ReplayManagerMode resultMode;
        ReplayManagerMode replayMode;
    } locals;

    if (this->activeSceneState != this->requestedSceneState)
    {
        this->EnterCriticalSectionWrapper(5);
        this->criticalSectionLockCounts[5]++;
        this->previousActiveSceneState = this->activeSceneState;
        utils::DebugPrint(
            "scene %d -> %d\r\n", this->activeSceneState, this->requestedSceneState);
        this->backbufferClearColor = 0xff000000;

        switch (this->activeSceneState)
        {
        case 0:
            this->requestedSceneState = SUPERVISOR_STATE_FRONT_END;
            this->frontEndController =
                FrontEndControllerView::Create(FRONT_END_ENTRY_MAIN_MENU);
            if (this->frontEndController == NULL)
            {
                goto failure;
            }
            break;

        case SUPERVISOR_STATE_FRONT_END:
            switch (this->requestedSceneState)
            {
            case SUPERVISOR_STATE_ERROR:
                goto failure;

            case SUPERVISOR_STATE_EXIT:
                this->frontEndController->Destroy();
                this->frontEndController = NULL;
                this->LeaveCriticalSectionWrapper(5);
                this->criticalSectionLockCounts[5]--;
                return 4;

            case SUPERVISOR_STATE_PHOTO_GAME:
                this->frontEndController->Destroy();
                this->frontEndController = NULL;
                break;

            case SUPERVISOR_STATE_START_REPLAY:
                this->requestedSceneState = SUPERVISOR_STATE_PHOTO_GAME;
                this->frontEndController->Destroy();
                this->frontEndController = NULL;
                break;
            }
            break;

        case SUPERVISOR_STATE_PHOTO_GAME:
            switch (this->requestedSceneState)
            {
            case SUPERVISOR_STATE_EXIT:
                this->photoGameTask->Destroy();
                this->photoGameTask = NULL;
                return 4;

            case SUPERVISOR_STATE_FRONT_END:
                locals.replayMode = this->photoGameTask->replayMode;
                this->photoGameTask->Destroy();
                this->photoGameTask = NULL;
                this->frontEndController =
                    FrontEndControllerView::Create(
                        locals.replayMode != REPLAY_MANAGER_RECORD
                            ? FRONT_END_ENTRY_REPLAY_BROWSER : FRONT_END_ENTRY_SCENE_SELECT);
                if (this->frontEndController == NULL)
                {
                    goto failure;
                }
                break;

            case SUPERVISOR_STATE_ERROR:
                goto failure;

            case SUPERVISOR_STATE_RETRY_PHOTO_GAME:
                this->photoGameTask->Destroy();
                this->photoGameTask = NULL;
                this->flags.restartPhotoGame = 1;
                this->photoGameTask = PhotoGameTaskView::Create(REPLAY_MANAGER_RECORD);
                if (this->photoGameTask == NULL)
                {
                    goto failure;
                }
                this->requestedSceneState = SUPERVISOR_STATE_PHOTO_GAME;
                break;

            case SUPERVISOR_STATE_RESTART_PHOTO_GAME:
                locals.resultMode = g_RuntimeGlobalStateOwner->replayMode;
                g_Supervisor.flags.resultRestartActive = 1;
                this->photoGameTask->Destroy();
                this->photoGameTask = NULL;
                this->photoGameTask =
                    PhotoGameTaskView::Create(locals.resultMode);
                if (this->photoGameTask == NULL)
                {
                    goto failure;
                }
                this->requestedSceneState = SUPERVISOR_STATE_PHOTO_GAME;
                break;
            }
            break;

        case SUPERVISOR_STATE_ERROR:
        failure:
            this->ReleaseGameManagers();
            this->LeaveCriticalSectionWrapper(5);
            this->criticalSectionLockCounts[5]--;
            return 4;
        }

        this->activeSceneState = this->requestedSceneState;
        this->LeaveCriticalSectionWrapper(5);
        this->criticalSectionLockCounts[5]--;
    }
    return 1;
}
