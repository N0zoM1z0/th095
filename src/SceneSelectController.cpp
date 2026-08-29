#include "SceneSelect.hpp"

namespace th095
{

i32 SceneValueQueue::Push(i32 value)
{
    if (this->count >= this->capacity)
    {
        this->count = this->capacity - 1;
    }
    this->values[this->count] = value;
    this->count++;
    return this->count;
}

void SceneSelectControllerView::RefreshSceneSelection(i32)
{
    i8 displayState;
    i32 selectedGroup;
    i32 groupCursorIndex;
    i32 selectedScene;
    SceneValueQueue *selectionQueue;
    i32 unlockGroup;
    i32 lockedGroup;
    i32 stateGroup;

    g_SceneSupervisor.EnterCriticalSectionWrapper(4);
    g_SceneSupervisor.lockCounts[4]++;

    selectedGroup = this->selectedGroup;
    groupCursorIndex = this->selectedGroup;
    selectedScene = this->groupCursors[groupCursorIndex].current;
    selectionQueue = &this->selectionQueue;
    if (selectionQueue->count < selectionQueue->capacity)
    {
        selectionQueue->values[selectionQueue->count] =
            (selectedGroup << 8) | selectedScene;
        selectionQueue->count++;
    }

    this->vmIds.SetInterrupt(0x82, 1);

    unlockGroup = this->selectedGroup;
    if (g_SceneSaveData->IsSceneGroupUnlocked(unlockGroup) != 0)
    {
        this->groupPreviewQueue.Push(g_SelectedScene->groupDisplayValue);
        this->scenePreviewQueue.Push(g_SelectedScene->sceneDisplayValue);
    }
    else
    {
        lockedGroup = this->selectedGroup;
        this->groupPreviewQueue.Push(-(lockedGroup + 1));
        this->scenePreviewQueue.Push(0);
    }

    displayState = 0;
    stateGroup = this->selectedGroup;
    if (g_SceneSaveData->IsSceneGroupUnlocked(stateGroup) == 0)
    {
        displayState = this->lockedDisplayState;
    }
    else if (g_SceneSaveData->sceneScores[g_SelectedScene->scoreEntryIndex].score == 0)
    {
        if (g_SceneSaveData->sceneScores[g_SelectedScene->scoreEntryIndex].attemptCount == 0)
        {
            displayState = this->unattemptedDisplayState;
        }
        else
        {
            displayState = this->attemptedDisplayState;
        }
    }
    else if (g_SceneSaveData->sceneScores[g_SelectedScene->scoreEntryIndex].score <
             g_SelectedScene->scoreRequirement)
    {
        displayState = this->belowRequirementDisplayState;
    }
    else
    {
        displayState = g_SelectedScene->displayState;
    }

    if (displayState != this->currentDisplayState)
    {
        if (this->stateHistory.count > 2)
        {
            this->stateHistory.count = 2;
        }
        this->stateHistory.values[this->stateHistory.count] = displayState;
        this->stateHistory.count++;
        this->currentDisplayState = displayState;
        this->vmIds.SetInterrupt(0x15, 5);
        this->vmIds.SetInterrupt(0x16, 5);
    }

    g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
    g_SceneSupervisor.lockCounts[4]--;
    this->previewTimer = 0;
}

} // namespace th095
