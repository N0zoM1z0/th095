#include "SceneSelect.hpp"

namespace th095
{

static __forceinline void RefreshSelectionQueuePush(
    SceneValueQueue *queue, i32 value)
{
    if (ReadSceneQueueWord32(queue->words[SCENE_QUEUE_COUNT_WORD].value) < ReadSceneQueueWord32(queue->words[SCENE_QUEUE_CAPACITY_WORD].value))
    {
        WriteSceneQueueWord32(queue->words[ReadSceneQueueWord32(queue->words[SCENE_QUEUE_COUNT_WORD].value)].value, value);
        IncrementSceneQueueCount32(queue->words[SCENE_QUEUE_COUNT_WORD].value);
    }
}

#include "SceneValueQueue.inl"
#include "ScenePreviewDataQueue.inl"

// Target-proven VC7.1 backing buckets keep the byte display state at EBP-1
// and the seven dword/pointer locals at EBP-8..EBP-20. The bounded inline
// queue helper above preserves the target's count-first evaluation order.
#define refreshDisplayState refreshDisplayStateLocal23
#define refreshSelectedGroup restartCommandProcessingLocal05
#define refreshGroupCursorIndex averagedPanLocal12
#define refreshSelectedScene iLocal11
#define refreshSelectionQueue commandCursorLocal02
#define refreshUnlockGroup soundIndexLocal01
#define refreshLockedGroup jLocal00
#define refreshStateGroup preloadBufferLocal03
void SceneSelectControllerView::RefreshSceneSelection(i32)
{
    i8 refreshDisplayState;
    i32 refreshSelectedGroup;
    i32 refreshGroupCursorIndex;
    i32 refreshSelectedScene;
    SceneValueQueue *refreshSelectionQueue;
    i32 refreshUnlockGroup;
    i32 refreshLockedGroup;
    i32 refreshStateGroup;

    g_SceneSupervisor.EnterCriticalSectionWrapper(4);
    g_SceneSupervisor.lockCounts[4]++;

    refreshSelectedGroup = this->selectedGroup;
    refreshGroupCursorIndex = this->selectedGroup;
    refreshSelectedScene = this->groupCursors[refreshGroupCursorIndex].current;
    refreshSelectionQueue = &this->selectionQueue;
    RefreshSelectionQueuePush(
        refreshSelectionQueue,
        (refreshSelectedGroup << 8) | refreshSelectedScene);

    this->vmIds.SetInterrupt(0x82, 1);

    refreshUnlockGroup = this->selectedGroup;
    if (g_ResultSaveData->IsSceneGroupUnlocked(refreshUnlockGroup) != 0)
    {
        this->groupPreviewQueue.Push(g_SelectedScene->groupPreviewAssetSelector);
        this->scenePreviewQueue.Push(g_SelectedScene->scenePreviewAssetSelector);
    }
    else
    {
        refreshLockedGroup = this->selectedGroup;
        this->groupPreviewQueue.Push(-(refreshLockedGroup + 1));
        this->scenePreviewQueue.Push(0);
    }

    refreshDisplayState = 0;
    refreshStateGroup = this->selectedGroup;
    if (g_ResultSaveData->IsSceneGroupUnlocked(refreshStateGroup) == 0)
    {
        refreshDisplayState = this->lockedDisplayState;
    }
    else if (g_ResultSaveData->scoreEntries[g_SelectedScene->scoreEntryIndex].score == 0)
    {
        if (g_ResultSaveData->scoreEntries[g_SelectedScene->scoreEntryIndex].captureTime == 0)
        {
            refreshDisplayState = this->unattemptedDisplayState;
        }
        else
        {
            refreshDisplayState = this->attemptedDisplayState;
        }
    }
    else if (g_ResultSaveData->scoreEntries[g_SelectedScene->scoreEntryIndex].score <
             g_SelectedScene->scoreRequirement)
    {
        refreshDisplayState = this->belowRequirementDisplayState;
    }
    else
    {
        refreshDisplayState = g_SelectedScene->displayState;
    }

    if (refreshDisplayState != this->currentDisplayState)
    {
        if (this->stateHistory.count > 2)
        {
            this->stateHistory.count = 2;
        }
        this->stateHistory.values[this->stateHistory.count] = refreshDisplayState;
        this->stateHistory.count++;
        this->currentDisplayState = refreshDisplayState;
        this->vmIds.SetInterrupt(0x15, 5);
        this->vmIds.SetInterrupt(0x16, 5);
    }

    g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
    g_SceneSupervisor.lockCounts[4]--;
    this->previewTimer = 0;
}
#undef refreshDisplayState
#undef refreshSelectedGroup
#undef refreshGroupCursorIndex
#undef refreshSelectedScene
#undef refreshSelectionQueue
#undef refreshUnlockGroup
#undef refreshLockedGroup
#undef refreshStateGroup

// Retail passes the decoded resource directly to the variadic text method.
#define DRAW_SCENE_PREVIEW_TEXT(vm, textColor, text) \
    reinterpret_cast<AnmTextManagerView *>(g_AnmManager)->DrawTextLeft( \
        reinterpret_cast<AnmTextVmView *>(vm), textColor, 0, text)

} // namespace th095
