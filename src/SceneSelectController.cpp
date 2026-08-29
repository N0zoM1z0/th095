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
