#include "SceneSelect.hpp"

namespace th095
{

static __forceinline void RefreshSelectionQueuePush(
    SceneValueQueue *queue, i32 value)
{
    if (queue->count < queue->capacity)
    {
        queue->values[queue->count] = value;
        queue->count++;
    }
}

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

i32 SceneValueQueue::Pop()
{
    if (this->count > 0)
    {
        this->count--;
        for (i32 i = 0; i < 16; i++)
        {
            (this->values + i)[0] = (this->values + i)[1];
        }
        return this->values[0];
    }
    return 0;
}

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
    if (g_SceneSaveData->IsSceneGroupUnlocked(refreshUnlockGroup) != 0)
    {
        this->groupPreviewQueue.Push(g_SelectedScene->groupDisplayValue);
        this->scenePreviewQueue.Push(g_SelectedScene->sceneDisplayValue);
    }
    else
    {
        refreshLockedGroup = this->selectedGroup;
        this->groupPreviewQueue.Push(-(refreshLockedGroup + 1));
        this->scenePreviewQueue.Push(0);
    }

    refreshDisplayState = 0;
    refreshStateGroup = this->selectedGroup;
    if (g_SceneSaveData->IsSceneGroupUnlocked(refreshStateGroup) == 0)
    {
        refreshDisplayState = this->lockedDisplayState;
    }
    else if (g_SceneSaveData->sceneScores[g_SelectedScene->scoreEntryIndex].score == 0)
    {
        if (g_SceneSaveData->sceneScores[g_SelectedScene->scoreEntryIndex].attemptCount == 0)
        {
            refreshDisplayState = this->unattemptedDisplayState;
        }
        else
        {
            refreshDisplayState = this->attemptedDisplayState;
        }
    }
    else if (g_SceneSaveData->sceneScores[g_SelectedScene->scoreEntryIndex].score <
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

#define BUILD_SCENE_PREVIEW_LINE(vmSlot, scriptIndex, columnIndex)             \
    this->previewTextVmIds[vmSlot] =                                          \
        g_SceneUiAnm->CreateVm(scriptIndex, 7);                               \
    g_SceneAnmManager->GetVm(this->previewTextVmIds[vmSlot])->glyphHeight =   \
        0x13;                                                                 \
    g_SceneAnmManager->GetVm(this->previewTextVmIds[vmSlot])->glyphWidth =    \
        0x13;                                                                 \
    {                                                                         \
        if (g_SceneSaveData->IsSceneGroupUnlocked(                            \
                this->GetSelectedGroup()) == 0)                               \
        {                                                                     \
            SceneWriteText(                                                   \
                g_SceneAnmManager,                                            \
                g_SceneAnmManager->GetVm(this->previewTextVmIds[vmSlot]),     \
                0x00df8f8f, 0,                                               \
                this->ResolveSceneText(                                       \
                    this->previewTextSources.lockedTextId, columnIndex,       \
                    0x62, 0));                                                \
        }                                                                     \
        else if (g_SceneSaveData                                              \
                     ->sceneScores[g_SelectedScene->scoreEntryIndex]          \
                     .score == 0)                                             \
        {                                                                     \
            if (g_SceneSaveData                                               \
                    ->sceneScores[g_SelectedScene->scoreEntryIndex]           \
                    .attemptCount == 0)                                       \
            {                                                                 \
                SceneWriteText(                                               \
                    g_SceneAnmManager,                                        \
                    g_SceneAnmManager->GetVm(                                 \
                        this->previewTextVmIds[vmSlot]),                       \
                    0x00df8f8f, 0,                                           \
                    this->ResolveSceneText(                                   \
                        this->previewTextSources.unattemptedTextId,           \
                        columnIndex, 0x62, 1));                               \
            }                                                                 \
            else                                                              \
            {                                                                 \
                SceneWriteText(                                               \
                    g_SceneAnmManager,                                        \
                    g_SceneAnmManager->GetVm(                                 \
                        this->previewTextVmIds[vmSlot]),                       \
                    0x00df8f8f, 0,                                           \
                    this->ResolveSceneText(                                   \
                        this->previewTextSources.attemptedTextId, columnIndex,\
                        0x62, 3));                                            \
            }                                                                 \
        }                                                                     \
        else if (g_SceneSaveData                                              \
                     ->sceneScores[g_SelectedScene->scoreEntryIndex]          \
                     .score < g_SelectedScene->scoreRequirement)              \
        {                                                                     \
            SceneWriteText(                                                   \
                g_SceneAnmManager,                                            \
                g_SceneAnmManager->GetVm(this->previewTextVmIds[vmSlot]),     \
                0x00df8f8f, 0,                                               \
                this->ResolveSceneText(                                       \
                    this->previewTextSources.belowRequirementTextId,          \
                    columnIndex, 0x62, 2));                                   \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            SceneWriteText(                                                   \
                g_SceneAnmManager,                                            \
                g_SceneAnmManager->GetVm(this->previewTextVmIds[vmSlot]),     \
                0x00cfcfff, 0,                                               \
                g_SelectedScene->titleTextId != 0                             \
                    ? this->ResolveSceneText(                                 \
                          g_SelectedScene->titleTextId, columnIndex,           \
                          g_SelectedScene->titleArgument1,                     \
                          g_SelectedScene->titleArgument2)                     \
                    : " ");                                                   \
        }                                                                     \
    }

void SceneSelectControllerView::BuildScenePreviewText()
{
    if (this->previewTimer == 0)
    {
        g_SceneAnmManager->SetInterrupt(this->previewTextVmIds[0], 1);
        g_SceneAnmManager->SetInterrupt(this->previewTextVmIds[1], 1);
        g_SceneAnmManager->SetInterrupt(this->previewTextVmIds[2], 1);
    }
    else if (this->previewTimer == 8)
    {
        BUILD_SCENE_PREVIEW_LINE(0, 0x0b, 0);
    }
    else if (this->previewTimer == 12)
    {
        BUILD_SCENE_PREVIEW_LINE(1, 0x0c, 1);
    }
    else if (this->previewTimer == 16)
    {
        BUILD_SCENE_PREVIEW_LINE(2, 0x0d, 2);
    }

    this->previewTimer++;
}

char *SceneSelectControllerView::ResolveSceneText(i32 textId, i32 column,
                                                  i32 argument1,
                                                  i32 argument2)
{
    u8 *source;
    {
        u8 key;
        source = (u8 *)(textId + column * 0x40);
        key = argument2 * 11 + argument1 * 7 + 58;
        for (i32 index = 0; index < 0x40; index++, source++)
        {
            g_SceneTextBuffer[index] = *source + key;
            key += (column + 1) * 23 + index;
        }
    }
    return (char *)g_SceneTextBuffer;
}

#undef BUILD_SCENE_PREVIEW_LINE

} // namespace th095
