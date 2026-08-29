#include "SceneSelect.hpp"

#include "ReplayBrowser.hpp"
#include "ResultScreen.hpp"
#include "SoundPlayer.hpp"

#include <stdlib.h>

namespace th095
{

extern u16 g_FrontEndCurrentInput;
extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;

struct SceneSelectColorInterpolationView
{
    u8 unknown000[0x220];
    u32 color1;
    u8 unknown224[0x74];

    void SetColor1Interpolation(i32 duration, u8 mode, u32 initial,
                                u32 final);
};

struct SceneSelectUpdateView
{
    SceneAnmLoadedView *sceneAnm;
    u8 unknown0004[4];
    ResultScreenTimer stateTimer;
    u8 unknown0014[0x0c];
    ResultScreenReplayCursor groupCursor;
    u8 unknown00f8[0xd8];
    ResultScreenReplayCursor sceneCursors[12];
    i32 selectedScoreEntryIndex;
    SceneAnmVmIdArray vmIds;
    i8 lockedDisplayState;
    i8 unattemptedDisplayState;
    i8 belowRequirementDisplayState;
    i8 attemptedDisplayState;
    u8 unknown0e8c[6];
    i8 currentDisplayState;
    u8 unknown0e93;
    SceneAnmVmId previewTextVmIds[3];
    i32 previewTimer;
    u8 unknown0ea4[0x525c];
    AnmVmId transitionVm;
    u8 unknown6104[8];
    i32 state;
    i32 requestedState;
    u8 unknown6114[0x0c];
    u32 flags;
    u8 unknown6124[4];
    SceneValueQueue selectionQueue;
    SceneValueQueue loadedSceneQueue;
    SceneValueQueue groupPreviewDataQueue;
    SceneValueQueue groupPreviewSizeQueue;
    SceneValueQueue scenePreviewDataQueue;
    SceneValueQueue scenePreviewSizeQueue;
    SceneValueQueue groupPreviewQueue;
    SceneValueQueue scenePreviewQueue;
    SceneValueQueue loadedGroupQueue;
    SceneStateHistoryView stateHistory;
    u8 unknown63c0[0x0c];
    i32 pendingTextureCount;
    i32 pendingPrimaryData[3];
    i32 pendingPrimarySize[3];
    i32 pendingSecondaryData[3];
    i32 pendingSecondarySize[3];
};

typedef char SceneSelectUpdateGroupCursorAt20[
    (offsetof(SceneSelectUpdateView, groupCursor) == 0x20) ? 1 : -1];
typedef char SceneSelectUpdateSceneCursorsAt1D0[
    (offsetof(SceneSelectUpdateView, sceneCursors) == 0x1d0) ? 1 : -1];
typedef char SceneSelectUpdateTransitionVmAt6100[
    (offsetof(SceneSelectUpdateView, transitionVm) == 0x6100) ? 1 : -1];
typedef char SceneSelectUpdateSelectionQueueAt6128[
    (offsetof(SceneSelectUpdateView, selectionQueue) == 0x6128) ? 1 : -1];
typedef char SceneSelectUpdateLoadedSceneQueueAt6170[
    (offsetof(SceneSelectUpdateView, loadedSceneQueue) == 0x6170) ? 1 : -1];
typedef char SceneSelectUpdateGroupPreviewQueueAt62D8[
    (offsetof(SceneSelectUpdateView, groupPreviewQueue) == 0x62d8) ? 1 : -1];
typedef char SceneSelectUpdateLoadedGroupQueueAt6368[
    (offsetof(SceneSelectUpdateView, loadedGroupQueue) == 0x6368) ? 1 : -1];
typedef char SceneSelectUpdatePendingTextureCountAt63CC[
    (offsetof(SceneSelectUpdateView, pendingTextureCount) == 0x63cc) ? 1 : -1];
typedef char SceneSelectUpdateSizeIs6400[
    (sizeof(SceneSelectUpdateView) == 0x6400) ? 1 : -1];

static __forceinline i32 SceneQueueFront(SceneValueQueue *queue)
{
    return queue->count < 1 ? 0 : queue->values[0];
}

static __forceinline void SetSceneVmVisible(SceneSelectUpdateView *view,
                                            i32 vmIndex, bool visible)
{
    SceneAnmVmView *vm =
        g_SceneAnmManager->GetVm(view->vmIds[vmIndex]);
    if (visible)
    {
        vm->flagsWord |= 2;
    }
    else
    {
        vm->flagsWord &= ~2u;
    }
}

#define SET_SCENE_VM_VISIBILITY(view, vmIndex, condition)                    \
    if (condition)                                                            \
    {                                                                         \
        g_SceneAnmManager->GetVm((view)->vmIds[vmIndex])->flagsWord |= 2;     \
    }                                                                         \
    else                                                                      \
    {                                                                         \
        g_SceneAnmManager->GetVm((view)->vmIds[vmIndex])->flagsWord &= ~2u;   \
    }

ChainCallbackResult SceneSelectControllerView::UpdateSceneSelect()
{
    SceneSelectUpdateView *view =
        reinterpret_cast<SceneSelectUpdateView *>(this);
    i32 i;

    /* Consume the scene thumbnail produced by the asynchronous worker. */
    if (view->pendingTextureCount == 0)
    {
        if (view->loadedGroupQueue.count == 0)
        {
            if (view->loadedSceneQueue.count != 0)
            {
                if (view->loadedSceneQueue.count == 1)
                {
                    i32 sceneIndex = SceneQueueFront(&view->loadedSceneQueue);
                    if (sceneIndex >= 0)
                    {
                        g_SceneSaveData->LoadScenePreviewTexture(
                            view->sceneAnm, 1, sceneIndex);
                        view->vmIds.SetInterrupt(0x82, 1);
                        view->vmIds[0x82] =
                            view->sceneAnm->CreateVm(0x82, 7);
                        SceneWriteText(
                            g_SceneAnmManager,
                            g_SceneAnmManager->GetVm(view->vmIds[0x82]),
                            0x00efcfcf, 0,
                            reinterpret_cast<char *>(g_SceneSaveData) +
                                sceneIndex * 0x78 + 0x3178);
                    }
                }

                g_SceneSupervisor.EnterCriticalSectionWrapper(4);
                g_SceneSupervisor.lockCounts[4]++;
                view->loadedSceneQueue.Pop();
                g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
                g_SceneSupervisor.lockCounts[4]--;
                this->UpdateSelectedSceneDetails();
            }
        }
        else
        {
            i32 previewGroup = SceneQueueFront(&view->loadedGroupQueue);
            if (previewGroup < 0)
            {
                if (view->loadedGroupQueue.count == 1)
                {
                    view->sceneAnm->textures[2].Load(
                        reinterpret_cast<u8 *>(SceneQueueFront(
                            &view->groupPreviewDataQueue)),
                        reinterpret_cast<i32 *>(SceneQueueFront(
                            &view->groupPreviewSizeQueue)),
                        5, 0, 1);
                    view->sceneAnm->textures[2].texture->PreLoad();
                }
                free(reinterpret_cast<void *>(SceneQueueFront(
                    &view->groupPreviewDataQueue)));
            }
            else
            {
                if (view->loadedGroupQueue.count == 1)
                {
                    view->sceneAnm->textures[2].Load(
                        reinterpret_cast<u8 *>(SceneQueueFront(
                            &view->groupPreviewDataQueue)),
                        reinterpret_cast<i32 *>(SceneQueueFront(
                            &view->groupPreviewSizeQueue)),
                        5, 0, 1);
                    view->sceneAnm->textures[3].LoadRegion(
                        reinterpret_cast<u8 *>(SceneQueueFront(
                            &view->scenePreviewDataQueue)),
                        reinterpret_cast<i32 *>(SceneQueueFront(
                            &view->scenePreviewSizeQueue)),
                        5, 0, 1, 0x20);
                    view->sceneAnm->textures[2].texture->PreLoad();
                }
                free(reinterpret_cast<void *>(SceneQueueFront(
                    &view->groupPreviewDataQueue)));
                free(reinterpret_cast<void *>(SceneQueueFront(
                    &view->scenePreviewDataQueue)));
            }

            g_SceneSupervisor.EnterCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]++;
            view->loadedGroupQueue.Pop();
            view->groupPreviewDataQueue.Pop();
            view->scenePreviewDataQueue.Pop();
            view->groupPreviewSizeQueue.Pop();
            view->scenePreviewSizeQueue.Pop();
            g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]--;
            if (view->loadedGroupQueue.count == 0)
            {
                view->vmIds.SetInterrupt(0x14, 4);
            }
        }
    }
    else
    {
        if (view->pendingTextureCount == 1)
        {
            view->sceneAnm->textures[3].Load(
                reinterpret_cast<u8 *>(view->pendingPrimaryData[0]),
                reinterpret_cast<i32 *>(view->pendingPrimarySize[0]),
                1, 0, 1);
            view->sceneAnm->textures[3].texture->PreLoad();
            view->sceneAnm->textures[4].Load(
                reinterpret_cast<u8 *>(view->pendingSecondaryData[0]),
                reinterpret_cast<i32 *>(view->pendingSecondarySize[0]),
                1, 0, 1);
            view->sceneAnm->textures[4].texture->PreLoad();
        }
        free(reinterpret_cast<void *>(view->pendingPrimaryData[0]));
        view->pendingPrimaryData[0] = 0;
        free(reinterpret_cast<void *>(view->pendingSecondaryData[0]));
        view->pendingSecondaryData[0] = 0;

        g_SceneSupervisor.EnterCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]++;
        for (i = 0; i < 2; i++)
        {
            view->pendingPrimaryData[i] = view->pendingPrimaryData[i + 1];
            view->pendingPrimarySize[i] = view->pendingPrimarySize[i + 1];
            view->pendingSecondaryData[i] =
                view->pendingSecondaryData[i + 1];
            view->pendingSecondarySize[i] =
                view->pendingSecondarySize[i + 1];
        }
        view->pendingPrimaryData[2] = 0;
        view->pendingPrimarySize[2] = 0;
        view->pendingSecondaryData[2] = 0;
        view->pendingSecondarySize[2] = 0;
        view->pendingTextureCount--;
        g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]--;

        if (view->pendingTextureCount == 0)
        {
            view->vmIds.SetInterrupt(0x15, 4);
            view->vmIds.SetInterrupt(0x16, 4);
        }
    }

    if (view->state == 0)
    {
        g_SceneSupervisor.StopReplayScan();
        g_SceneUiAnm->textures[0].Clear();
        view->stateTimer.Reset();
        view->groupCursor.Push();

        if (g_SceneSaveData->FindHighestUnlockedSceneGroup() < 2)
        {
            view->groupCursor.count = 3;
        }
        else if (g_SceneSaveData->FindHighestUnlockedSceneGroup() < 5)
        {
            view->groupCursor.count = 6;
        }
        else
        {
            if (g_SceneSaveData->FindHighestUnlockedSceneGroup() + 2 < 12)
            {
                view->groupCursor.count =
                    g_SceneSaveData->FindHighestUnlockedSceneGroup() + 2;
            }
            else
            {
                view->groupCursor.count = 12;
            }
        }
        view->groupCursor.wraps = 1;
        view->state = 1;
        view->flags |= 0x10;
        view->selectedScoreEntryIndex = 0;

        for (i = 0; i < 12; i++)
        {
            view->sceneCursors[i].count = g_SceneGroupCounts[i];
            view->sceneCursors[i].wraps = 1;
            view->sceneCursors[i].Set(0);
        }
        view->groupCursor.Set(g_SceneSaveData->lastSelectedGroup);
        view->sceneCursors[view->groupCursor.current].Set(
            g_SceneSaveData->lastSelectedScene);

        g_SelectedScene =
            &g_SceneGroups[view->groupCursor.current]
                          [view->sceneCursors[view->groupCursor.current].current];
        view->selectedScoreEntryIndex = g_SelectedScene->scoreEntryIndex;

        view->vmIds[0x68] = view->sceneAnm->CreateVm(0x68, 7);
        view->vmIds[0x69] = view->sceneAnm->CreateVm(0x69, 7);
        view->vmIds.SetInterrupt(0x19, 3);
        view->vmIds.SetInterrupt(0x1a, 3);
        view->transitionVm.SetInterrupt(3);
        view->vmIds.SetInterrupt(0x1b, 3);
        view->vmIds[0x1e] = view->sceneAnm->CreateVm(0x1e, 7);
        view->vmIds[0x22] = view->sceneAnm->CreateVm(0x22, 7);
        if (view->groupCursor.current < 11)
        {
            view->vmIds[0x20] = view->sceneAnm->CreateVm(0x20, 7);
            view->sceneAnm->SetSprite(
                g_SceneAnmManager->GetVm(view->vmIds[0x22]),
                view->groupCursor.current + 0x37);
        }
        else
        {
            view->vmIds[0x21] = view->sceneAnm->CreateVm(0x21, 7);
            view->sceneAnm->SetSprite(
                g_SceneAnmManager->GetVm(view->vmIds[0x22]),
                view->groupCursor.current + 0x2c);
        }
        view->vmIds[0x23] = view->sceneAnm->CreateVm(0x23, 7);
        view->vmIds[0x24] = view->sceneAnm->CreateVm(0x24, 7);
        SET_SCENE_VM_VISIBILITY(
            view, 0x23, view->groupCursor.current != 0);
        SET_SCENE_VM_VISIBILITY(
            view, 0x24,
            view->groupCursor.current != view->groupCursor.count - 1);

        view->vmIds[0x44] = view->sceneAnm->CreateVm(0x44, 7);
        view->vmIds[0x48] = view->sceneAnm->CreateVm(0x48, 7);
        view->vmIds[0x15] = view->sceneAnm->CreateVm(0x15, 7);
        view->vmIds[0x16] = view->sceneAnm->CreateVm(0x16, 7);
        view->vmIds[0x45] = view->sceneAnm->CreateVm(0x45, 7);
        view->vmIds[0x46] = view->sceneAnm->CreateVm(0x46, 7);

        for (i = 0; i < g_SceneGroupCounts[view->groupCursor.current]; i++)
        {
            i32 script = i * 3 + 0x25;
            view->vmIds[script] = view->sceneAnm->CreateVm(script, 7);
            view->vmIds[script + 1] =
                view->sceneAnm->CreateVm(script + 1, 7);
            view->vmIds[script + 2] =
                view->sceneAnm->CreateVm(script + 2, 7);

            bool attempted =
                g_SceneSaveData
                    ->sceneScores[g_SceneGroups[view->groupCursor.current][i]
                                      .scoreEntryIndex]
                    .score != 0;
            SET_SCENE_VM_VISIBILITY(view, script + 2, attempted);
            if (g_SceneSaveData->IsSceneGroupUnlocked(
                    view->groupCursor.current) == 0)
            {
                SetSceneVmVisible(view, script, false);
                SetSceneVmVisible(view, script + 1, false);
                SetSceneVmVisible(view, script + 2, false);
            }
        }

        view->groupPreviewQueue.capacity = 5;
        view->scenePreviewQueue.capacity = 5;
        view->loadedGroupQueue.capacity = 16;
        view->groupPreviewDataQueue.capacity = 16;
        view->groupPreviewSizeQueue.capacity = 16;
        view->scenePreviewDataQueue.capacity = 16;
        view->scenePreviewSizeQueue.capacity = 16;
        view->selectionQueue.capacity = 5;
        view->loadedSceneQueue.capacity = 16;
        view->flags &= ~0x20u;

        g_SceneSupervisor.StartReplayScan(LoadSceneSelectionAssets, NULL);
        view->vmIds[0x13] = view->sceneAnm->CreateVm(0x13, 7);
        view->vmIds[0x12] = view->sceneAnm->CreateVm(0x12, 7);
        view->flags |= 4;
        view->vmIds[0x14] = view->sceneAnm->CreateVm(0x14, 7);
        view->vmIds[0x49] = view->sceneAnm->CreateVm(0x49, 7);
        view->vmIds[0x4a] = view->sceneAnm->CreateVm(0x4a, 7);
        view->vmIds[0x4b] = view->sceneAnm->CreateVm(0x4b, 7);

        i32 groupSprite = view->groupCursor.current < 11
                              ? view->groupCursor.current
                              : view->groupCursor.current - 11;
        view->sceneAnm->SetSprite(
            g_SceneAnmManager->GetVm(view->vmIds[0x49]),
            groupSprite + 0x28);
        view->sceneAnm->SetSprite(
            g_SceneAnmManager->GetVm(view->vmIds[0x4b]),
            view->sceneCursors[view->groupCursor.current].current + 0x28);

        for (i = 0; i < 16; i++)
        {
            view->vmIds[0x4c + i] =
                view->sceneAnm->CreateVm(0x4c + i, 7);
        }
        view->vmIds[0x5c] = view->sceneAnm->CreateVm(0x5c, 7);
        view->vmIds[0x5d] = view->sceneAnm->CreateVm(0x5d, 7);
        view->vmIds[0x5e] = view->sceneAnm->CreateVm(0x5e, 7);
        view->vmIds[0x5f] = view->sceneAnm->CreateVm(0x5f, 7);
        view->vmIds[0x60] = view->sceneAnm->CreateVm(0x60, 7);
        view->vmIds[0x61] = view->sceneAnm->CreateVm(0x61, 7);
        view->vmIds[0x62] = view->sceneAnm->CreateVm(0x62, 7);
        view->vmIds[0x63] = view->sceneAnm->CreateVm(0x63, 7);

        SceneAnmVmView *groupVm = view->vmIds[0x68].GetVm();
        if (g_SceneSaveData->IsSceneGroupUnlocked(
                view->groupCursor.current) == 0)
        {
            reinterpret_cast<SceneSelectColorInterpolationView *>(groupVm)
                ->SetColor1Interpolation(
                    60, 0, groupVm->color1, g_SceneLockedInitialColor);
        }
        else
        {
            reinterpret_cast<SceneSelectColorInterpolationView *>(groupVm)
                ->SetColor1Interpolation(
                    60, 0, groupVm->color1,
                    g_SceneGroupColors[view->groupCursor.current]);
        }
        this->UpdateSelectedSceneDetails();
    }
    else if (view->state != 1)
    {
        goto update_preview_text;
    }

    if (view->stateTimer.current < 30)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (view->stateTimer.current == 30)
    {
        for (i = 0; i < g_SceneGroupCounts[view->groupCursor.current]; i++)
        {
            if (view->sceneCursors[view->groupCursor.current].current == i)
            {
                view->vmIds.SetInterrupt(i * 3 + 0x25, 2);
                view->vmIds.SetInterrupt(i * 3 + 0x26, 2);
                view->vmIds.SetInterrupt(i * 3 + 0x27, 2);
            }
            else
            {
                view->vmIds.SetInterrupt(i * 3 + 0x25, 3);
                view->vmIds.SetInterrupt(i * 3 + 0x26, 3);
                view->vmIds.SetInterrupt(i * 3 + 0x27, 3);
            }
        }
        this->RefreshSceneSelection(0);
    }

    if ((g_PressedButtons & 0x400) != 0)
    {
        view->flags ^= 8;
    }

    bool showRates = (g_FrontEndCurrentInput & 0x100) != 0;
    SET_SCENE_VM_VISIBILITY(view, 0x15, showRates);
    SET_SCENE_VM_VISIBILITY(view, 0x16, showRates);
    SET_SCENE_VM_VISIBILITY(view, 0x45, showRates);
    if (view->previewTextVmIds[0].GetVm() != NULL)
    {
        view->previewTextVmIds[0].GetVm()->flagsWord =
            showRates ? view->previewTextVmIds[0].GetVm()->flagsWord | 2
                      : view->previewTextVmIds[0].GetVm()->flagsWord & ~2u;
    }
    if (view->previewTextVmIds[1].GetVm() != NULL)
    {
        view->previewTextVmIds[1].GetVm()->flagsWord =
            showRates ? view->previewTextVmIds[1].GetVm()->flagsWord | 2
                      : view->previewTextVmIds[1].GetVm()->flagsWord & ~2u;
    }
    if (view->previewTextVmIds[2].GetVm() != NULL)
    {
        view->previewTextVmIds[2].GetVm()->flagsWord =
            showRates ? view->previewTextVmIds[2].GetVm()->flagsWord | 2
                      : view->previewTextVmIds[2].GetVm()->flagsWord & ~2u;
    }
    if (showRates)
    {
        view->flags &= ~0x10u;
    }
    else
    {
        view->flags |= 0x10;
    }

    if ((g_PressedButtons & 0x8000) != 0 &&
        g_SceneSaveData->sceneScores[view->selectedScoreEntryIndex]
                .attemptCount != 0)
    {
        g_SceneSaveData->sceneScores[view->selectedScoreEntryIndex].flags ^= 2;
    }

    view->groupCursor.SaveCurrent();
    i32 groupDirection = 0;
    if ((g_PressedButtons & TH_BUTTON_UP) != 0)
    {
        view->groupCursor.Move(-1);
        groupDirection = -1;
    }
    if ((g_PressedButtons & TH_BUTTON_DOWN) != 0)
    {
        view->groupCursor.Move(1);
        groupDirection = 1;
    }

    if (!view->groupCursor.HasChanged())
    {
        if (g_SceneSaveData->IsSceneGroupUnlocked(
                view->groupCursor.current) != 0)
        {
            ResultScreenReplayCursor &sceneCursor =
                view->sceneCursors[view->groupCursor.current];
            sceneCursor.SaveCurrent();
            if ((g_PressedButtons & TH_BUTTON_LEFT) != 0 ||
                (g_ResultMenuInput & TH_BUTTON_LEFT) != 0)
            {
                sceneCursor.Move(-1);
            }
            if ((g_PressedButtons & TH_BUTTON_RIGHT) != 0 ||
                (g_ResultMenuInput & TH_BUTTON_RIGHT) != 0)
            {
                sceneCursor.Move(1);
            }

            if (sceneCursor.HasChanged())
            {
                g_SelectedScene =
                    &g_SceneGroups[view->groupCursor.current]
                                  [sceneCursor.current];
                view->selectedScoreEntryIndex =
                    g_SelectedScene->scoreEntryIndex;
                for (i = 0;
                     i < g_SceneGroupCounts[view->groupCursor.current]; i++)
                {
                    if (sceneCursor.current == i)
                    {
                        view->vmIds.SetInterrupt(i * 3 + 0x25, 2);
                        view->vmIds.SetInterrupt(i * 3 + 0x26, 2);
                        view->vmIds.SetInterrupt(i * 3 + 0x27, 2);
                    }
                    else
                    {
                        view->vmIds.SetInterrupt(i * 3 + 0x25, 3);
                        view->vmIds.SetInterrupt(i * 3 + 0x26, 3);
                        view->vmIds.SetInterrupt(i * 3 + 0x27, 3);
                    }
                }
                g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
                this->RefreshSceneSelection(0);
                view->vmIds.SetInterrupt(0x12, 5);
                view->vmIds.SetInterrupt(0x14, 5);
                view->sceneAnm->SetSprite(
                    g_SceneAnmManager->GetVm(view->vmIds[0x49]),
                    (view->groupCursor.current < 11
                         ? view->groupCursor.current
                         : view->groupCursor.current - 11) +
                        0x28);
                view->sceneAnm->SetSprite(
                    g_SceneAnmManager->GetVm(view->vmIds[0x4b]),
                    sceneCursor.current + 0x28);
                view->vmIds.SetInterrupt(0x49, 2);
                view->vmIds.SetInterrupt(0x4a, 2);
                view->vmIds.SetInterrupt(0x4b, 2);
                this->UpdateSelectedSceneDetails();
                g_SceneSaveData->lastSelectedScene = (i16)sceneCursor.current;
                goto update_preview_text;
            }

            if ((g_PressedButtons & 0x1002) != 0)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                g_SelectedScene =
                    &g_SceneGroups[view->groupCursor.current]
                                  [sceneCursor.current];
                view->requestedState = 5;
                view->state = 0;
                view->stateTimer.Reset();
                view->flags |= 0x20;
                g_ReplayBrowserExitSignal.Request();
                g_SceneAnmManager->MarkVmForDeletion(
                    view->previewTextVmIds[0]);
                g_SceneAnmManager->MarkVmForDeletion(
                    view->previewTextVmIds[1]);
                g_SceneAnmManager->MarkVmForDeletion(
                    view->previewTextVmIds[2]);
                i32 selected = sceneCursor.current;
                view->vmIds.SetInterrupt(selected * 3 + 0x25, 6);
                view->vmIds.SetInterrupt(selected * 3 + 0x26, 6);
                view->vmIds.SetInterrupt(0x1e, 1);
                view->vmIds.SetInterrupt(0x23, 1);
                view->vmIds.SetInterrupt(0x24, 1);
                view->vmIds.SetInterrupt(0x20, 1);
                view->vmIds.SetInterrupt(0x21, 1);
                view->vmIds.SetInterrupt(0x22, 1);
                view->vmIds.SetInterrupt(0x44, 1);
                view->vmIds.SetInterrupt(0x15, 1);
                view->vmIds.SetInterrupt(0x16, 1);
                view->vmIds.SetInterrupt(0x45, 1);
                view->vmIds.SetInterrupt(0x46, 1);
                view->vmIds.SetInterrupt(0x13, 1);
                view->vmIds.SetInterrupt(0x12, 1);
                view->vmIds.SetInterrupt(0x14, 1);
                view->vmIds.SetInterrupt(0x49, 1);
                view->vmIds.SetInterrupt(0x4a, 1);
                view->vmIds.SetInterrupt(0x4b, 1);
                for (i = 0; i < 16; i++)
                {
                    view->vmIds.SetInterrupt(0x4c + i, 1);
                }
                view->vmIds.SetInterrupt(0x5c, 1);
                view->vmIds.SetInterrupt(0x5d, 1);
                view->vmIds.SetInterrupt(0x5e, 1);
                view->vmIds.SetInterrupt(0x5f, 1);
                view->vmIds.SetInterrupt(0x60, 1);
                view->vmIds.SetInterrupt(0x61, 1);
                view->vmIds.SetInterrupt(0x62, 1);
                view->vmIds.SetInterrupt(0x63, 1);
                view->vmIds.SetInterrupt(0x48, 1);
                view->vmIds.SetInterrupt(0x82, 1);
                view->vmIds[0x82].value = 0;
                return CHAIN_CALLBACK_RESULT_CONTINUE;
            }
        }

        if ((g_PressedButtons & 9) != 0)
        {
            view->flags |= 0x20;
            g_SelectedScene =
                &g_SceneGroups[view->groupCursor.current]
                              [view->sceneCursors[view->groupCursor.current]
                                   .current];
            view->groupCursor.Pop();
            view->requestedState = 1;
            view->state = 0;
            view->stateTimer.Reset();
            view->vmIds.SetInterrupt(0x68, 1);
            view->vmIds.SetInterrupt(0x69, 1);
            view->vmIds[0x66] = view->sceneAnm->CreateVm(0x66, 7);
            view->vmIds[0x67] = view->sceneAnm->CreateVm(0x67, 7);
            view->vmIds.SetInterrupt(0x19, 2);
            view->vmIds.SetInterrupt(0x1a, 2);
            view->transitionVm.SetInterrupt(2);
            view->vmIds.SetInterrupt(0x1b, 2);
            view->vmIds.SetInterrupt(0x1e, 1);
            view->vmIds.SetInterrupt(0x23, 1);
            view->vmIds.SetInterrupt(0x24, 1);
            view->vmIds.SetInterrupt(0x20, 1);
            view->vmIds.SetInterrupt(0x21, 1);
            view->vmIds.SetInterrupt(0x22, 1);
            view->vmIds.SetInterrupt(0x44, 1);
            view->vmIds.SetInterrupt(0x15, 1);
            view->vmIds.SetInterrupt(0x16, 1);
            view->vmIds.SetInterrupt(0x45, 1);
            view->vmIds.SetInterrupt(0x46, 1);
            for (i = 0; i < 9; i++)
            {
                view->vmIds.SetInterrupt(i * 3 + 0x25, 1);
                view->vmIds.SetInterrupt(i * 3 + 0x26, 1);
                view->vmIds.SetInterrupt(i * 3 + 0x27, 1);
            }
            view->vmIds.SetInterrupt(0x13, 1);
            view->vmIds.SetInterrupt(0x12, 1);
            view->vmIds.SetInterrupt(0x14, 1);
            view->vmIds.SetInterrupt(0x49, 1);
            view->vmIds.SetInterrupt(0x4a, 1);
            view->vmIds.SetInterrupt(0x4b, 1);
            for (i = 0; i < 16; i++)
            {
                view->vmIds.SetInterrupt(0x4c + i, 1);
            }
            view->vmIds.SetInterrupt(0x5c, 1);
            view->vmIds.SetInterrupt(0x5d, 1);
            view->vmIds.SetInterrupt(0x5e, 1);
            view->vmIds.SetInterrupt(0x5f, 1);
            view->vmIds.SetInterrupt(0x60, 1);
            view->vmIds.SetInterrupt(0x61, 1);
            view->vmIds.SetInterrupt(0x62, 1);
            view->vmIds.SetInterrupt(0x63, 1);
            view->vmIds.SetInterrupt(0x48, 1);
            g_SceneAnmManager->SetInterrupt(view->previewTextVmIds[0], 1);
            g_SceneAnmManager->SetInterrupt(view->previewTextVmIds[1], 1);
            g_SceneAnmManager->SetInterrupt(view->previewTextVmIds[2], 1);
            view->vmIds.SetInterrupt(0x82, 1);
            view->vmIds[0x82].value = 0;
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
    }
    else
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        g_SceneAnmManager->MarkVmForDeletion(view->vmIds[0x20]);
        g_SceneAnmManager->MarkVmForDeletion(view->vmIds[0x21]);
        g_SceneAnmManager->MarkVmForDeletion(view->vmIds[0x22]);
        view->vmIds[0x22] = view->sceneAnm->CreateVm(0x22, 7);
        if (view->groupCursor.current < 11)
        {
            view->vmIds[0x20] = view->sceneAnm->CreateVm(0x20, 7);
            view->sceneAnm->SetSprite(
                g_SceneAnmManager->GetVm(view->vmIds[0x22]),
                view->groupCursor.current + 0x37);
        }
        else
        {
            view->vmIds[0x21] = view->sceneAnm->CreateVm(0x21, 7);
            view->sceneAnm->SetSprite(
                g_SceneAnmManager->GetVm(view->vmIds[0x22]),
                view->groupCursor.current + 0x2c);
        }

        ResultScreenReplayCursor &sceneCursor =
            view->sceneCursors[view->groupCursor.current];
        g_SelectedScene =
            &g_SceneGroups[view->groupCursor.current][sceneCursor.current];
        view->selectedScoreEntryIndex = g_SelectedScene->scoreEntryIndex;

        for (i = 0; i < g_SceneGroupCounts[view->groupCursor.previous]; i++)
        {
            i32 interrupt = groupDirection < 1 ? 8 : 7;
            view->vmIds.SetInterrupt(i * 3 + 0x25, interrupt);
            view->vmIds.SetInterrupt(i * 3 + 0x26, interrupt);
            view->vmIds.SetInterrupt(i * 3 + 0x27, interrupt);
        }
        for (i = 0; i < g_SceneGroupCounts[view->groupCursor.current]; i++)
        {
            i32 script = i * 3 + 0x25;
            view->vmIds[script] = view->sceneAnm->CreateVm(script, 7);
            view->vmIds[script + 1] =
                view->sceneAnm->CreateVm(script + 1, 7);
            view->vmIds[script + 2] =
                view->sceneAnm->CreateVm(script + 2, 7);
            i32 interrupt = groupDirection > 0 ? 10 : 9;
            view->vmIds.SetInterrupt(script, interrupt);
            view->vmIds.SetInterrupt(script + 1, interrupt);
            view->vmIds.SetInterrupt(script + 2, interrupt);

            bool attempted =
                g_SceneSaveData
                    ->sceneScores[g_SceneGroups[view->groupCursor.current][i]
                                      .scoreEntryIndex]
                    .score != 0;
            SET_SCENE_VM_VISIBILITY(view, script + 2, attempted);
            if (g_SceneSaveData->IsSceneGroupUnlocked(
                    view->groupCursor.current) == 0)
            {
                SetSceneVmVisible(view, script, false);
                SetSceneVmVisible(view, script + 1, false);
                SetSceneVmVisible(view, script + 2, false);
            }
        }

        view->vmIds.SetInterrupt(0x12, groupDirection < 1 ? 8 : 7);
        view->vmIds[0x12] = view->sceneAnm->CreateVm(0x12, 7);
        view->vmIds.SetInterrupt(0x12, 5);
        view->vmIds.SetInterrupt(0x12, groupDirection > 0 ? 10 : 9);
        view->vmIds.SetInterrupt(0x14, 5);
        view->stateTimer.previous = -999999;
        view->stateTimer.subFrame = 20.0f;
        view->stateTimer.current = 20;
        view->sceneAnm->SetSprite(
            g_SceneAnmManager->GetVm(view->vmIds[0x49]),
            (view->groupCursor.current < 11
                 ? view->groupCursor.current
                 : view->groupCursor.current - 11) +
                0x28);
        view->sceneAnm->SetSprite(
            g_SceneAnmManager->GetVm(view->vmIds[0x4b]),
            sceneCursor.current + 0x28);
        view->vmIds.SetInterrupt(0x49, 2);
        view->vmIds.SetInterrupt(0x4a, 2);
        view->vmIds.SetInterrupt(0x4b, 2);
        this->UpdateSelectedSceneDetails();
        g_SceneSaveData->lastSelectedGroup =
            (i16)view->groupCursor.current;
        g_SceneSaveData->lastSelectedScene = (i16)sceneCursor.current;

        SET_SCENE_VM_VISIBILITY(
            view, 0x23, view->groupCursor.current != 0);
        SET_SCENE_VM_VISIBILITY(
            view, 0x24,
            view->groupCursor.current != view->groupCursor.count - 1);

        SceneAnmVmView *groupVm = view->vmIds[0x68].GetVm();
        if (g_SceneSaveData->IsSceneGroupUnlocked(
                view->groupCursor.current) == 0)
        {
            reinterpret_cast<SceneSelectColorInterpolationView *>(groupVm)
                ->SetColor1Interpolation(
                    60, 0, groupVm->color1,
                    g_SceneLockedTransitionColor);
        }
        else
        {
            reinterpret_cast<SceneSelectColorInterpolationView *>(groupVm)
                ->SetColor1Interpolation(
                    60, 0, groupVm->color1,
                    g_SceneGroupColors[view->groupCursor.current]);
        }
    }

update_preview_text:
    if (view->selectionQueue.count == 0 &&
        view->loadedSceneQueue.count == 0)
    {
        if ((view->flags & 4) != 0)
        {
            SceneAnmVmView *previewVm =
                g_SceneAnmManager->GetVm(view->vmIds[0x12]);
            u8 *record = reinterpret_cast<u8 *>(g_SceneSaveData) +
                         view->selectedScoreEntryIndex * 0x78;
            if (record[0x31c9] == 0)
            {
                previewVm->flagsWord &= ~2u;
            }
            else
            {
                u16 width = *reinterpret_cast<u16 *>(record + 0x316c);
                u16 height = *reinterpret_cast<u16 *>(record + 0x316e);
                previewVm->loadedSprite->uvEndX = (f32)width / 256.0f;
                previewVm->loadedSprite->uvEndY = (f32)height / 256.0f;
                previewVm->spriteWidth = (f32)width;
                previewVm->spriteHeight = (f32)height;
                previewVm->flagsWord |= 2;
            }
            view->vmIds.SetInterrupt(0x12, 2);
            view->flags &= ~4u;
        }
    }
    else if ((view->flags & 4) == 0)
    {
        view->vmIds.SetInterrupt(0x12, 3);
        view->flags |= 4;
    }

    this->BuildScenePreviewText();
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

#undef SET_SCENE_VM_VISIBILITY

} // namespace th095
