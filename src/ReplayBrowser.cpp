#include "ReplayBrowser.hpp"
#include "SoundPlayer.hpp"

#include <direct.h>
#include <stdio.h>
#include <string.h>

namespace th095
{

struct ReplayBrowserLoadLocals
{
    i32 scanFinished2;
    i32 scanFinished1;
    i32 i;
    char path[MAX_PATH];
    ReplayBrowserView *browser;
    WIN32_FIND_DATAA findData;
    i32 slot;
    HANDLE findHandle;
};

typedef char ReplayBrowserLoadLocalsSizeIs25C[
    (sizeof(ReplayBrowserLoadLocals) == 0x25c) ? 1 : -1];
typedef char ReplayBrowserLoadPathAt0C[
    (offsetof(ReplayBrowserLoadLocals, path) == 0x0c) ? 1 : -1];
typedef char ReplayBrowserLoadFindDataAt114[
    (offsetof(ReplayBrowserLoadLocals, findData) == 0x114) ? 1 : -1];

extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;

inline u16 GetReplayBrowserPressedButtons(u16 buttons)
{
    return g_PressedButtons & buttons;
}

inline u16 IsReplayBrowserMenuInputPressed(u16 buttons)
{
    return (u16)((GetReplayBrowserPressedButtons(buttons) != 0) ||
                 ((g_ResultMenuInput & buttons) != 0));
}

void ReplayBrowserExitSignal::Request()
{
    this->requested = 1;
}

ChainCallbackResult ReplayBrowserView::Update()
{
    i32 i;
    i32 replayIndex;

    switch (this->state)
    {
    case 0:
        g_SceneSupervisor.StopReplayScan();

        g_SceneSupervisor.EnterCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]++;
        for (i = 0; i < 80; i++)
        {
            if (this->replays[i] != NULL)
            {
                delete this->replays[i];
                this->replays[i] = NULL;
            }
        }
        g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]--;

        g_SceneSupervisor.StartReplayScan(
            LoadReplayBrowserEntries, NULL);

        this->stateTimer.Reset();
        this->rowCursor.Push();
        this->rowCursor.count = 4;
        this->rowCursor.wraps = 1;
        this->rowCursor.Set(g_ReplayBrowserSelection / 20);
        this->state = 1;
        this->selectedReplayIndex = 0;

        this->vmIds[0x68] = this->sceneAnm->CreateVm(0x68, 7);
        this->vmIds[0x69] = this->sceneAnm->CreateVm(0x69, 7);
        this->vmIds.SetInterrupt(0x19, 3);
        this->vmIds.SetInterrupt(0x1a, 3);
        this->transitionVm.SetInterrupt(3);
        this->vmIds.SetInterrupt(0x1b, 3);
        this->vmIds[0x1f] = this->sceneAnm->CreateVm(0x1f, 7);
        this->vmIds[0x47] = this->sceneAnm->CreateVm(0x47, 7);

        this->columnCursor.count = 20;
        this->columnCursor.wraps = 1;
        this->columnCursor.Set(g_ReplayBrowserSelection % 20);
        g_ReplayBrowserSelection = 0;

    case 1:
        if (this->stateTimer < 30)
        {
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }

        this->rowCursor.SaveCurrent();
        if (IsReplayBrowserMenuInputPressed(TH_BUTTON_LEFT))
        {
            this->rowCursor.Move(-1);
        }
        if (IsReplayBrowserMenuInputPressed(TH_BUTTON_RIGHT))
        {
            this->rowCursor.Move(1);
        }
        if (this->rowCursor.HasChanged())
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        }

        this->columnCursor.SaveCurrent();
        if (IsReplayBrowserMenuInputPressed(TH_BUTTON_UP))
        {
            this->columnCursor.Move(-1);
        }
        if (IsReplayBrowserMenuInputPressed(TH_BUTTON_DOWN))
        {
            this->columnCursor.Move(1);
        }
        if (this->columnCursor.HasChanged())
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        }

        if (GetReplayBrowserPressedButtons(0x1002) != 0)
        {
            i32 replayRow = this->rowCursor.current;
            i32 replayColumn = this->columnCursor.current;
            replayIndex = replayRow * 20 + replayColumn;
            if (this->replays[replayIndex] == NULL ||
                this->replays[replayIndex]->activeInputData == NULL)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_DAMAGE_LOW_HEALTH, 0);
            }
            else
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                this->requestedState = 6;
                this->rowCursor.Pop();
                this->stateTimer.Reset();
                this->state = 0;
                strcpy(g_SelectedReplayPath,
                       this->replays[replayIndex]->path);
                g_ReplayBrowserSelection = replayIndex;
            }
        }

        if (GetReplayBrowserPressedButtons(9) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            this->requestedState = 1;
            this->rowCursor.Pop();
            this->stateTimer.Reset();
            this->state = 0;
            g_ReplayBrowserExitSignal.Request();
            this->vmIds.SetInterrupt(0x1f, 1);
            this->vmIds.SetInterrupt(0x47, 1);
            this->vmIds.SetInterrupt(0x68, 1);
            this->vmIds.SetInterrupt(0x69, 1);
            this->vmIds[0x66] = this->sceneAnm->CreateVm(0x66, 7);
            this->vmIds[0x67] = this->sceneAnm->CreateVm(0x67, 7);
            this->vmIds.SetInterrupt(0x19, 2);
            this->vmIds.SetInterrupt(0x1a, 2);
            this->transitionVm.SetInterrupt(2);
            this->vmIds.SetInterrupt(0x1b, 2);
        }

    default:
        break;
    }

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

ZunResult ReplayBrowserView::LoadReplaySlot(i32 slot, char *path)
{
    g_SceneSupervisor.EnterCriticalSectionWrapper(4);
    g_SceneSupervisor.lockCounts[4]++;
    if (this->replays[slot] != NULL)
    {
        delete this->replays[slot];
        this->replays[slot] = NULL;
    }
    this->replays[slot] = NULL;
    g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
    g_SceneSupervisor.lockCounts[4]--;

    ReplayManager *replay = ReplayManager::Load(path);

    g_SceneSupervisor.EnterCriticalSectionWrapper(4);
    g_SceneSupervisor.lockCounts[4]++;
    this->replays[slot] = replay;
    g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
    g_SceneSupervisor.lockCounts[4]--;
    return ZUN_SUCCESS;
}

void __fastcall LoadReplayBrowserEntries(void *)
{
    ReplayBrowserLoadLocals locals;

    locals.browser = g_ReplayBrowser;
    for (locals.i = 0; locals.i < 20; locals.i++)
    {
        if (locals.browser->requestedState != 3)
        {
            goto finish;
        }
        locals.scanFinished1 = g_ReplayScanFinished;
        if (locals.scanFinished1 != 0)
        {
            goto finish;
        }
        sprintf(locals.path, "th95_%.2d.rpy", locals.i + 1);
        locals.browser->LoadReplaySlot(locals.i, locals.path);
    }

    locals.slot = 20;
    PrepareReplayDirectory("replay");
    _chdir("replay");
    locals.findHandle = FindFirstFileA(
        "th95_ud????.rpy", &locals.findData);
    if (locals.findHandle != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (locals.slot >= 80 ||
                locals.browser->requestedState != 3)
            {
                break;
            }
            locals.scanFinished2 = g_ReplayScanFinished;
            if (locals.scanFinished2 != 0)
            {
                break;
            }
            _chdir("../");
            locals.browser->LoadReplaySlot(
                locals.slot, locals.findData.cFileName);
            locals.slot++;
            _chdir("replay");
        } while (FindNextFileA(
                     locals.findHandle, &locals.findData) != 0);
    }
    FindClose(locals.findHandle);
    _chdir("../");

finish:
    g_ReplayScanActive = 0;
    g_ReplayScanFinished = 1;
}

} // namespace th095
