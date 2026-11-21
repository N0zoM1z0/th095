#ifdef TH095_MATCH_EXACT
#include "SceneSelectControllerExact.inl"
#else
#include "SceneSelect.hpp"
#include "AnmManager.hpp"

namespace th095
{

// Target 0x004CA308 is the scene-text decoder's zero-initialized 0x40-byte
// scratch buffer.
DIFFABLE_STATIC_ARRAY(u8, 0x40, g_SceneTextBuffer);

void __cdecl SceneWriteText(SceneAnmManagerView *manager,
                            SceneAnmVmView *vm, u32 color, u32 shadowColor,
                            const char *text)
{
    manager->DrawTextLeft(vm, color, shadowColor, "%s", text);
}

#define DRAW_SCENE_PREVIEW_TEXT(vm, textColor, text) \
    SceneWriteText(g_AnmManager, vm, textColor, 0, text)

} // namespace th095

#endif // TH095_MATCH_EXACT

#include "AnmManager.hpp"
#include "FrontEndController.hpp"

namespace th095
{

enum ScenePreviewMessageSlot
{
    SCENE_PREVIEW_LOCKED = 0,
    SCENE_PREVIEW_UNATTEMPTED = 1,
    SCENE_PREVIEW_BELOW_REQUIREMENT = 2,
    SCENE_PREVIEW_ATTEMPTED = 3,
};

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
void FrontEndControllerView::RefreshSceneSelection(i32)
{
    i8 refreshDisplayState;
    i32 refreshSelectedGroup;
    i32 refreshGroupCursorIndex;
    i32 refreshSelectedScene;
    SceneValueQueue *refreshSelectionQueue;
    i32 refreshUnlockGroup;
    i32 refreshLockedGroup;
    i32 refreshStateGroup;

    g_Supervisor.EnterCriticalSectionWrapper(4);
    g_Supervisor.criticalSectionLockCounts[4]++;

    refreshSelectedGroup = this->groupCursor.current;
    refreshGroupCursorIndex = this->groupCursor.current;
    refreshSelectedScene = this->sceneCursors[refreshGroupCursorIndex].current;
    refreshSelectionQueue = &this->selectionQueue;
    RefreshSelectionQueuePush(
        refreshSelectionQueue,
        (refreshSelectedGroup << 8) | refreshSelectedScene);

    reinterpret_cast<SceneSelectControllerView *>(this)->vmIds.SetInterrupt(0x82, 1);

    refreshUnlockGroup = this->groupCursor.current;
    if (g_ResultSaveData->IsSceneGroupUnlocked(refreshUnlockGroup) != 0)
    {
        this->groupPreviewQueue.Push(
            g_SelectedScene->groupPreviewAssetSelector);
        this->scenePreviewQueue.Push(
            g_SelectedScene->scenePreviewAssetSelector);
    }
    else
    {
        refreshLockedGroup = this->groupCursor.current;
        this->groupPreviewQueue.Push(-(refreshLockedGroup + 1));
        this->scenePreviewQueue.Push(0);
    }

    refreshDisplayState = 0;
    refreshStateGroup = this->groupCursor.current;
    if (g_ResultSaveData->IsSceneGroupUnlocked(refreshStateGroup) == 0)
    {
        refreshDisplayState = this->specialDisplayStates[SCENE_PREVIEW_LOCKED];
    }
    else if (g_ResultSaveData->scoreEntries[g_SelectedScene->scoreEntryIndex].score == 0)
    {
        if (g_ResultSaveData->scoreEntries[g_SelectedScene->scoreEntryIndex].captureTime == 0)
        {
            refreshDisplayState = this->specialDisplayStates[SCENE_PREVIEW_UNATTEMPTED];
        }
        else
        {
            refreshDisplayState = this->specialDisplayStates[SCENE_PREVIEW_ATTEMPTED];
        }
    }
    else if (g_ResultSaveData->scoreEntries[g_SelectedScene->scoreEntryIndex].score <
             g_SelectedScene->scoreRequirement)
    {
        refreshDisplayState = this->specialDisplayStates[SCENE_PREVIEW_BELOW_REQUIREMENT];
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
        reinterpret_cast<SceneSelectControllerView *>(this)->vmIds.SetInterrupt(0x15, 5);
        reinterpret_cast<SceneSelectControllerView *>(this)->vmIds.SetInterrupt(0x16, 5);
    }

    g_Supervisor.LeaveCriticalSectionWrapper(4);
    g_Supervisor.criticalSectionLockCounts[4]--;
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

// The preview VM handles still use the Scene Select overlay.
#define PREVIEW_TEXT_VM(slot) \
    (reinterpret_cast<SceneSelectControllerView *>(this)->previewTextVmIds[slot])

// The three dynamic columns belong to text.anm, published by Supervisor.
#define BUILD_SCENE_PREVIEW_LINE(vmSlot, scriptIndex, columnIndex)                 \
    PREVIEW_TEXT_VM(vmSlot) =                                                      \
        g_Supervisor.textAnm->CreateVm(scriptIndex, 7);                            \
    g_AnmManager->GetVm(PREVIEW_TEXT_VM(vmSlot))->glyphHeight = 0x13;              \
    g_AnmManager->GetVm(PREVIEW_TEXT_VM(vmSlot))->glyphWidth = 0x13;               \
    if (g_ResultSaveData->IsSceneGroupUnlocked(this->GetSelectedGroup()) == 0)    \
    {                                                                              \
        DRAW_SCENE_PREVIEW_TEXT(                                                   \
            g_AnmManager->GetVm(PREVIEW_TEXT_VM(vmSlot)), 0x00df8f8f,              \
            this->ResolveSceneText(                                                \
                this->specialEncodedText[SCENE_PREVIEW_LOCKED],                    \
                columnIndex, 0x62, SCENE_PREVIEW_LOCKED));                         \
    }                                                                              \
    else if (g_ResultSaveData->scoreEntries[g_SelectedScene->scoreEntryIndex]      \
                 .score == 0)                                                      \
    {                                                                              \
        if (g_ResultSaveData->scoreEntries[g_SelectedScene->scoreEntryIndex]       \
                .captureTime == 0)                                                 \
        {                                                                          \
            DRAW_SCENE_PREVIEW_TEXT(                                               \
                g_AnmManager->GetVm(PREVIEW_TEXT_VM(vmSlot)), 0x00df8f8f,          \
                this->ResolveSceneText(                                            \
                    this->specialEncodedText[SCENE_PREVIEW_UNATTEMPTED],           \
                    columnIndex, 0x62, SCENE_PREVIEW_UNATTEMPTED));                \
        }                                                                          \
        else                                                                       \
        {                                                                          \
            DRAW_SCENE_PREVIEW_TEXT(                                               \
                g_AnmManager->GetVm(PREVIEW_TEXT_VM(vmSlot)), 0x00df8f8f,          \
                this->ResolveSceneText(                                            \
                    this->specialEncodedText[SCENE_PREVIEW_ATTEMPTED],             \
                    columnIndex, 0x62, SCENE_PREVIEW_ATTEMPTED));                  \
        }                                                                          \
    }                                                                              \
    else if (g_ResultSaveData->scoreEntries[g_SelectedScene->scoreEntryIndex]      \
                 .score < g_SelectedScene->scoreRequirement)                       \
    {                                                                              \
        DRAW_SCENE_PREVIEW_TEXT(                                                   \
            g_AnmManager->GetVm(PREVIEW_TEXT_VM(vmSlot)), 0x00df8f8f,              \
            this->ResolveSceneText(                                                \
                this->specialEncodedText[SCENE_PREVIEW_BELOW_REQUIREMENT],         \
                columnIndex, 0x62, SCENE_PREVIEW_BELOW_REQUIREMENT));              \
    }                                                                              \
    else                                                                           \
    {                                                                              \
        DRAW_SCENE_PREVIEW_TEXT(                                                   \
            g_AnmManager->GetVm(PREVIEW_TEXT_VM(vmSlot)), 0x00cfcfff,              \
            g_SelectedScene->encodedTitleText != 0                                 \
                ? this->ResolveSceneText(                                          \
                      g_SelectedScene->encodedTitleText, columnIndex,              \
                      g_SelectedScene->group, g_SelectedScene->scene)              \
                : " ");                                                            \
    }

void FrontEndControllerView::BuildScenePreviewText()
{
    if (this->previewTimer == 0)
    {
        g_AnmManager->SetInterrupt(PREVIEW_TEXT_VM(0), 1);
        g_AnmManager->SetInterrupt(PREVIEW_TEXT_VM(1), 1);
        g_AnmManager->SetInterrupt(PREVIEW_TEXT_VM(2), 1);
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

#undef BUILD_SCENE_PREVIEW_LINE
#undef PREVIEW_TEXT_VM
#undef DRAW_SCENE_PREVIEW_TEXT

char *FrontEndControllerView::ResolveSceneText(
    SceneEncodedText encodedText,
    i32 column, i32 argument1, i32 argument2)
{
    const u8 *source;
    {
        u8 key;
        source = encodedText + column * 0x40;
        key = argument2 * 11 + argument1 * 7 + 58;
        for (i32 index = 0; index < 0x40; index++, source++)
        {
            g_SceneTextBuffer[index] = *source + key;
            key += (column + 1) * 23 + index;
        }
    }
    return (char *)g_SceneTextBuffer;
}

} // namespace th095
