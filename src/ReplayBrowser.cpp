#include "ReplayBrowser.hpp"
#include "AnmManager.hpp"
#include "ControllerInputSlot.hpp"
#include "FrontEndController.hpp"
#include "ReplayManager.hpp"
#include "SceneSelect.hpp"
#include "SoundPlayer.hpp"

#include <direct.h>
#include <stdio.h>
#include <string.h>

namespace th095
{

#ifndef DIFFBUILD
char g_SelectedReplayPath[0x100];
#endif

// Target 0x004BDECC remembers the browser cursor.
DIFFABLE_STATIC(i32, g_ReplayBrowserSelection);

static __forceinline void ReplayBrowserCreateVmAt(SceneSelectControllerView *view, i32 index)
{
    view->vmIds[index] = view->sceneAnm->CreateVm(index, 7);
}

struct ReplayBrowserLoadLocals
{
    i32 scanFinished1;
    i32 i;
    char path[MAX_PATH];
    FrontEndControllerView *browser;
    WIN32_FIND_DATAA findData;
    i32 slot;
    HANDLE findHandle;
};

typedef char ReplayBrowserLoadLocalsSizeIs258[
    (sizeof(ReplayBrowserLoadLocals) == 0x258) ? 1 : -1];
typedef char ReplayBrowserLoadPathAt08[
    (offsetof(ReplayBrowserLoadLocals, path) == 0x08) ? 1 : -1];
typedef char ReplayBrowserLoadFindDataAt110[
    (offsetof(ReplayBrowserLoadLocals, findData) == 0x110) ? 1 : -1];

inline u16 GetReplayBrowserPressedButtons(u16 buttons)
{
    return g_ControllerInputSlots[0].pressed & buttons;
}

inline u16 IsReplayBrowserMenuInputPressed(u16 buttons)
{
    return (u16)((GetReplayBrowserPressedButtons(buttons) != 0) ||
                 ((g_ControllerInputSlots[0].repeat & buttons) != 0));
}

ChainCallbackResult FrontEndControllerView::UpdateReplayBrowser()
{
// The existing Scene Select overlay supplies the shared animation handles.
#define sceneMenu (reinterpret_cast<SceneSelectControllerView *>(this))
    i32 i;
    i32 replayIndex;

    switch (this->state)
    {
    case REPLAY_BROWSER_STATE_INITIALIZE:
        g_Supervisor.StopReplayScan();

        g_Supervisor.EnterCriticalSectionWrapper(4);
        g_Supervisor.criticalSectionLockCounts[4]++;
        for (i = 0; i < 80; i++)
        {
            if (this->replays[i] != NULL)
            {
                delete this->replays[i];
                this->replays[i] = NULL;
            }
        }
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        g_Supervisor.criticalSectionLockCounts[4]--;

        g_Supervisor.StartReplayScan(
            LoadReplayBrowserEntries, NULL);

        FrontEndResetTimer(&this->stateTimer);
        this->groupCursor.Push();
        this->groupCursor.count = 4;
        this->groupCursor.wraps = 1;
        this->groupCursor.Set(g_ReplayBrowserSelection / 20);
        this->state = REPLAY_BROWSER_STATE_BROWSE;
        // The browser clears the shared scene-selection field on entry.
        this->selectedScoreEntryIndex = 0;

        ReplayBrowserCreateVmAt(sceneMenu, 0x68);
        ReplayBrowserCreateVmAt(sceneMenu, 0x69);
        sceneMenu->vmIds.SetInterrupt(0x19, 3);
        sceneMenu->vmIds.SetInterrupt(0x1a, 3);
        sceneMenu->transitionVm.SetInterrupt(3);
        sceneMenu->vmIds.SetInterrupt(0x1b, 3);
        ReplayBrowserCreateVmAt(sceneMenu, 0x1f);
        ReplayBrowserCreateVmAt(sceneMenu, 0x47);

        this->secondaryCursor.count = 20;
        this->secondaryCursor.wraps = 1;
        this->secondaryCursor.Set(g_ReplayBrowserSelection % 20);
        g_ReplayBrowserSelection = 0;

    case REPLAY_BROWSER_STATE_BROWSE:
        if (this->stateTimer < 30)
        {
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }

        this->groupCursor.SaveCurrent();
        if (IsReplayBrowserMenuInputPressed(TH_BUTTON_LEFT))
        {
            this->groupCursor.Move(-1);
        }
        if (IsReplayBrowserMenuInputPressed(TH_BUTTON_RIGHT))
        {
            this->groupCursor.Move(1);
        }
        if (this->groupCursor.HasChanged())
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        }

        this->secondaryCursor.SaveCurrent();
        if (IsReplayBrowserMenuInputPressed(TH_BUTTON_UP))
        {
            this->secondaryCursor.Move(-1);
        }
        if (IsReplayBrowserMenuInputPressed(TH_BUTTON_DOWN))
        {
            this->secondaryCursor.Move(1);
        }
        if (this->secondaryCursor.HasChanged())
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        }

        if (GetReplayBrowserPressedButtons(TH_BUTTON_ENTER | TH_BUTTON_BOMB) != 0)
        {
            replayIndex = this->groupCursor.GetCurrent() * 20 +
                          this->secondaryCursor.GetCurrent();
            if (this->replays[replayIndex] == NULL ||
                this->replays[replayIndex]->activeInputData == NULL)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_DAMAGE_LOW_HEALTH, 0);
            }
            else
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                this->requestedState = FRONT_END_REQUESTED_STATE_START_REPLAY;
                this->groupCursor.Pop();
                FrontEndResetTimer(&this->stateTimer);
                this->state = REPLAY_BROWSER_STATE_INITIALIZE;
                strcpy(g_SelectedReplayPath,
                       this->replays[replayIndex]->path);
                g_ReplayBrowserSelection = replayIndex;
            }
        }

        if (GetReplayBrowserPressedButtons(TH_BUTTON_MENU | TH_BUTTON_SHOOT) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            this->requestedState = FRONT_END_REQUESTED_STATE_MAIN_MENU;
            this->groupCursor.Pop();
            FrontEndResetTimer(&this->stateTimer);
            this->state = REPLAY_BROWSER_STATE_INITIALIZE;
            g_Supervisor.replayScanWorker.RequestExit();
            sceneMenu->vmIds.SetInterrupt(0x1f, 1);
            sceneMenu->vmIds.SetInterrupt(0x47, 1);
            sceneMenu->vmIds.SetInterrupt(0x68, 1);
            sceneMenu->vmIds.SetInterrupt(0x69, 1);
            ReplayBrowserCreateVmAt(sceneMenu, 0x66);
            ReplayBrowserCreateVmAt(sceneMenu, 0x67);
            sceneMenu->vmIds.SetInterrupt(0x19, 2);
            sceneMenu->vmIds.SetInterrupt(0x1a, 2);
            sceneMenu->transitionVm.SetInterrupt(2);
            sceneMenu->vmIds.SetInterrupt(0x1b, 2);
        }

    default:
        break;
    }

#undef sceneMenu
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

ZunResult FrontEndControllerView::LoadReplaySlot(i32 slot, char *path)
{
    g_Supervisor.EnterCriticalSectionWrapper(4);
    g_Supervisor.criticalSectionLockCounts[4]++;
    if (this->replays[slot] != NULL)
    {
        delete this->replays[slot];
        this->replays[slot] = NULL;
    }
    this->replays[slot] = NULL;
    g_Supervisor.LeaveCriticalSectionWrapper(4);
    g_Supervisor.criticalSectionLockCounts[4]--;

    ReplayManager *replay = ReplayManager::Load(path);

    g_Supervisor.EnterCriticalSectionWrapper(4);
    g_Supervisor.criticalSectionLockCounts[4]++;
    this->replays[slot] = replay;
    g_Supervisor.LeaveCriticalSectionWrapper(4);
    g_Supervisor.criticalSectionLockCounts[4]--;
    return ZUN_SUCCESS;
}

void __fastcall LoadReplayBrowserEntries(void *)
{
    i32 scanFinished2;
    ReplayBrowserLoadLocals locals;

    locals.browser =
        static_cast<FrontEndControllerView *>(g_ActiveMenuController);
    for (locals.i = 0; locals.i < 20; locals.i++)
    {
        if (locals.browser->requestedState != FRONT_END_REQUESTED_STATE_REPLAY_BROWSER)
        {
            goto finish;
        }
        ReadWorkerSignal(locals.scanFinished1, g_Supervisor.replayScanWorker.exitSignal);
        if (locals.scanFinished1 != 0)
        {
            goto finish;
        }
        sprintf(locals.path, "th95_%.2d.rpy", locals.i + 1);
        locals.browser->LoadReplaySlot(locals.i, locals.path);
    }

    locals.slot = 20;
    _mkdir("replay");
    _chdir("replay");
    locals.findHandle = FindFirstFileA(
        "th95_ud????.rpy", &locals.findData);
    if (locals.findHandle != INVALID_HANDLE_VALUE)
    {
        while (locals.slot < 80)
        {
            if (locals.browser->requestedState != FRONT_END_REQUESTED_STATE_REPLAY_BROWSER)
            {
                break;
            }
            ReadWorkerSignal(scanFinished2, g_Supervisor.replayScanWorker.exitSignal);
            if (scanFinished2 != 0)
            {
                break;
            }
            _chdir("../");
            locals.browser->LoadReplaySlot(
                locals.slot, locals.findData.cFileName);
            locals.slot++;
            _chdir("replay");
            if (FindNextFileA(
                    locals.findHandle, &locals.findData) == 0)
            {
                break;
            }
        }
    }
    FindClose(locals.findHandle);
    _chdir("../");

finish:
    WriteWorkerSignal(g_Supervisor.replayScanWorker.active, 0);
    WriteWorkerSignal(g_Supervisor.replayScanWorker.exitSignal, 1);
}

} // namespace th095
