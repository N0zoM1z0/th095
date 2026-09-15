#ifdef TH095_MATCH_EXACT
#include "SceneSelectControllerExact.inl"
#else
#include "SceneSelect.hpp"
#include "AnmText.hpp"

namespace th095
{

// Target 0x004CA308 is the scene-text decoder's zero-initialized 0x40-byte
// scratch buffer.
DIFFABLE_STATIC_ARRAY(u8, 0x40, g_SceneTextBuffer);

#ifdef DIFFBUILD
#define TH095_SCENE_LOCKED_ENCODED_TEXT(view) ((view)->previewTextSources.lockedTextId)
#define TH095_SCENE_UNATTEMPTED_ENCODED_TEXT(view) ((view)->previewTextSources.unattemptedTextId)
#define TH095_SCENE_BELOW_REQUIREMENT_ENCODED_TEXT(view) ((view)->previewTextSources.belowRequirementTextId)
#define TH095_SCENE_ATTEMPTED_ENCODED_TEXT(view) ((view)->previewTextSources.attemptedTextId)
#define TH095_SCENE_TITLE_ENCODED_TEXT(scene) ((scene)->titleTextId)
#define TH095_SCENE_GROUP_PREVIEW_ASSET_SELECTOR(scene) ((scene)->groupDisplayValue)
#define TH095_SCENE_SCENE_PREVIEW_ASSET_SELECTOR(scene) ((scene)->sceneDisplayValue)
#else
#define TH095_SCENE_LOCKED_ENCODED_TEXT(view) ((view)->previewTextSources.lockedEncodedText)
#define TH095_SCENE_UNATTEMPTED_ENCODED_TEXT(view) ((view)->previewTextSources.unattemptedEncodedText)
#define TH095_SCENE_BELOW_REQUIREMENT_ENCODED_TEXT(view) ((view)->previewTextSources.belowRequirementEncodedText)
#define TH095_SCENE_ATTEMPTED_ENCODED_TEXT(view) ((view)->previewTextSources.attemptedEncodedText)
#define TH095_SCENE_TITLE_ENCODED_TEXT(scene) ((scene)->encodedTitleText)
#define TH095_SCENE_GROUP_PREVIEW_ASSET_SELECTOR(scene) ((scene)->groupPreviewAssetSelector)
#define TH095_SCENE_SCENE_PREVIEW_ASSET_SELECTOR(scene) ((scene)->scenePreviewAssetSelector)
#endif

void __cdecl SceneWriteText(SceneAnmManagerView *manager,
                            SceneAnmVmView *vm, u32 color, u32 shadowColor,
                            const char *text)
{
    reinterpret_cast<AnmTextManagerView *>(manager)->DrawTextLeft(
        reinterpret_cast<AnmTextVmView *>(vm), color, shadowColor, "%s", text);
}

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

struct SceneValueQueueCopyValue
{
    i32 value;
};

typedef char SceneValueQueueCopyValueSizeIs4[
    (sizeof(SceneValueQueueCopyValue) == sizeof(i32)) ? 1 : -1];

i32 SceneValueQueue::Pop()
{
    if (this->count > 0)
    {
        this->count--;
        for (i32 i = 0; i < 16; i++)
        {
            *reinterpret_cast<SceneValueQueueCopyValue *>(&this->values[i]) =
                *reinterpret_cast<const SceneValueQueueCopyValue *>(
                    &this->values[i + 1]);
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
void SceneSelectControllerView::RefreshSceneSelection(
    i32 ignoredSelectedScoreEntryIndex)
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
        this->groupPreviewQueue.Push(
            TH095_SCENE_GROUP_PREVIEW_ASSET_SELECTOR(g_SelectedScene));
        this->scenePreviewQueue.Push(
            TH095_SCENE_SCENE_PREVIEW_ASSET_SELECTOR(g_SelectedScene));
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
    else if (g_ResultSaveData->sceneScores[g_SelectedScene->scoreEntryIndex].score == 0)
    {
        if (g_ResultSaveData->sceneScores[g_SelectedScene->scoreEntryIndex].captureTime == 0)
        {
            refreshDisplayState = this->unattemptedDisplayState;
        }
        else
        {
            refreshDisplayState = this->attemptedDisplayState;
        }
    }
    else if (g_ResultSaveData->sceneScores[g_SelectedScene->scoreEntryIndex].score <
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

// Exact BuildScenePreviewText reads target 0x004C4AAC, the embedded
// Supervisor::textAnm owner.  The title controller's sceneAnm is title.anm;
// using it here makes dynamic scene descriptions overwrite the title menu.
#define BUILD_SCENE_PREVIEW_LINE(vmSlot, scriptIndex, columnIndex)             \
    this->previewTextVmIds[vmSlot] =                                          \
        g_Supervisor.textAnm->CreateVm(scriptIndex, 7);                       \
    g_AnmManager->GetVm(this->previewTextVmIds[vmSlot])->glyphHeight =   \
        0x13;                                                                 \
    g_AnmManager->GetVm(this->previewTextVmIds[vmSlot])->glyphWidth =    \
        0x13;                                                                 \
    {                                                                         \
        if (g_ResultSaveData->IsSceneGroupUnlocked(                            \
                this->GetSelectedGroup()) == 0)                               \
        {                                                                     \
            SceneWriteText(                                                   \
                g_AnmManager,                                            \
                g_AnmManager->GetVm(this->previewTextVmIds[vmSlot]),     \
                0x00df8f8f, 0,                                               \
                this->ResolveSceneText(                                       \
                    TH095_SCENE_LOCKED_ENCODED_TEXT(this), columnIndex,       \
                    0x62, 0));                                                \
        }                                                                     \
        else if (g_ResultSaveData                                              \
                     ->sceneScores[g_SelectedScene->scoreEntryIndex]          \
                     .score == 0)                                             \
        {                                                                     \
            if (g_ResultSaveData                                               \
                    ->sceneScores[g_SelectedScene->scoreEntryIndex]           \
                    .captureTime == 0)                                       \
            {                                                                 \
                SceneWriteText(                                               \
                    g_AnmManager,                                        \
                    g_AnmManager->GetVm(                                 \
                        this->previewTextVmIds[vmSlot]),                       \
                    0x00df8f8f, 0,                                           \
                    this->ResolveSceneText(                                   \
                        TH095_SCENE_UNATTEMPTED_ENCODED_TEXT(this),           \
                        columnIndex, 0x62, 1));                               \
            }                                                                 \
            else                                                              \
            {                                                                 \
                SceneWriteText(                                               \
                    g_AnmManager,                                        \
                    g_AnmManager->GetVm(                                 \
                        this->previewTextVmIds[vmSlot]),                       \
                    0x00df8f8f, 0,                                           \
                    this->ResolveSceneText(                                   \
                        TH095_SCENE_ATTEMPTED_ENCODED_TEXT(this), columnIndex,\
                        0x62, 3));                                            \
            }                                                                 \
        }                                                                     \
        else if (g_ResultSaveData                                              \
                     ->sceneScores[g_SelectedScene->scoreEntryIndex]          \
                     .score < g_SelectedScene->scoreRequirement)              \
        {                                                                     \
            SceneWriteText(                                                   \
                g_AnmManager,                                            \
                g_AnmManager->GetVm(this->previewTextVmIds[vmSlot]),     \
                0x00df8f8f, 0,                                               \
                this->ResolveSceneText(                                       \
                    TH095_SCENE_BELOW_REQUIREMENT_ENCODED_TEXT(this),          \
                    columnIndex, 0x62, 2));                                   \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            SceneWriteText(                                                   \
                g_AnmManager,                                            \
                g_AnmManager->GetVm(this->previewTextVmIds[vmSlot]),     \
                0x00cfcfff, 0,                                               \
                TH095_SCENE_TITLE_ENCODED_TEXT(g_SelectedScene) != 0                             \
                    ? this->ResolveSceneText(                                 \
                          TH095_SCENE_TITLE_ENCODED_TEXT(g_SelectedScene), columnIndex,           \
                          g_SelectedScene->titleArgument1,                     \
                          g_SelectedScene->titleArgument2)                     \
                    : " ");                                                   \
        }                                                                     \
    }

void SceneSelectControllerView::BuildScenePreviewText()
{
    if (this->previewTimer == 0)
    {
        g_AnmManager->SetInterrupt(this->previewTextVmIds[0], 1);
        g_AnmManager->SetInterrupt(this->previewTextVmIds[1], 1);
        g_AnmManager->SetInterrupt(this->previewTextVmIds[2], 1);
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

char *SceneSelectControllerView::ResolveSceneText(
#ifdef DIFFBUILD
    i32 textId,
#else
    SceneEncodedText encodedText,
#endif
    i32 column, i32 argument1, i32 argument2)
{
#ifdef DIFFBUILD
    u8 *source;
#else
    const u8 *source;
#endif
    {
        u8 key;
#ifdef DIFFBUILD
        source = (u8 *)(textId + column * 0x40);
#else
        source = encodedText + column * 0x40;
#endif
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

#endif // TH095_MATCH_EXACT
