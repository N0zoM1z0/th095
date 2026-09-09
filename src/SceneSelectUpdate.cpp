#ifdef TH095_MATCH_EXACT
#include "SceneSelectUpdateExact.inl"
#else
#include "SceneSelect.hpp"
#include "AnmVmInterpolation.hpp"
#include "InputRuntime.hpp"

#include "ReplayBrowser.hpp"
#include "ResultScreen.hpp"
#include "SoundPlayer.hpp"

#include <stdlib.h>

namespace th095
{

struct AnmTextVmView;

struct AnmTextManagerView
{
    void DrawTextCentered(AnmTextVmView *vm, COLORREF textColor,
                          COLORREF shadowColor, const char *format, ...);
};

#define g_FrontEndCurrentInput (RuntimeInputCurrent())
extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;
#define g_ResultMenuInput (RuntimeResultMenuInput())
#define g_PressedButtons (RuntimePressedButtons())

struct SceneSelectScoreFlagsView
{
    u32 captured : 1;
    u32 showSuccessRateMarker : 1;
    u32 unknownFlags : 30;
};

struct SceneSelectUpdateView
{
    SceneAnmLoadedView *sceneAnm;
    u8 unknown0004[4];
    ZunTimer stateTimer;
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
    union
    {
        u32 flags;
        struct
        {
            u32 unknownFlagBits0 : 2;
            u32 previewPending : 1;
            u32 showRates : 1;
            u32 unknownFlagBits4 : 28;
        } flagBits;
    };
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

/*
 * The target keeps these eighteen simultaneously live values in one
 * contiguous shallow stack band (EBP-48h through EBP-4).  Reverse field
 * order reflects the downward-growing x86 frame while retaining semantic
 * names at each use site.
 */
struct SceneSelectShallowLocals
{
    void *pendingSecondaryFree;
    void *pendingPrimaryFree;
    SceneAnmVmView *previewVm;
    i32 menuExitVmIndex;
    i32 menuExitSceneIndex;
    i32 gameExitVmIndex;
    i32 selected;
    i32 selectionIndex;
    SceneAnmVmView *transitionGroupVm;
    i32 newGroupIndex;
    i32 previousGroupIndex;
    i32 initialSelectionIndex;
    SceneAnmVmView *initialGroupVm;
    i32 initialVmIndex;
    i32 initialSceneIndex;
    i32 initialCursorIndex;
    i32 groupDirection;
    i32 i;
};

/*
 * Queue paths intentionally keep their pointer/value pairs as independent
 * semantic locals.  Stock VC7.1 identifier buckets reproduce the target
 * physical order; a single 92-byte aggregate creates a non-target alignment
 * hole before the shallow selector locals.
 */

static __forceinline void FreeSceneOwned(void *Block)
{
    free(Block);
}

static __forceinline u16 SceneInputAnd(u16 input, u16 mask)
{
    return input & mask;
}

static __forceinline u16 ScenePreviousInputMask()
{
    return TH_BUTTON_UP;
}

static __forceinline u16 SceneNextInputMask()
{
    return TH_BUTTON_DOWN;
}

static __forceinline i32 SceneQueueSize(const SceneValueQueue *queue)
{
    return queue->count;
}

static __forceinline i32 SceneQueueFront(const SceneValueQueue *queue)
{
    if (queue->count > 0)
    {
        return queue->values[0];
    }
    return 0;
}

static __forceinline void SceneSelectQueuePopPhase(SceneValueQueue *queue)
{
    u32 compilerStorage;
    queue->Pop();
}

struct SceneQueueFrontMemberView
{
    i32 values[16];
    i32 count;
    i32 capacity;
    __forceinline i32 Front() const
    {
        if (count > 0) return values[0];
        return 0;
    }
};

#define SET_SCENE_VM_VISIBILITY(view, vmIndex, condition)                    \
    if (!(condition))                                                         \
    {                                                                         \
        g_AnmManager->GetVm((view)->vmIds[vmIndex])->flagsWord &= ~2u;   \
    }                                                                         \
    else                                                                      \
    {                                                                         \
        g_AnmManager->GetVm((view)->vmIds[vmIndex])->flagsWord |= 2;     \
    }

static __forceinline void SceneSelectInitialTimerViewPhase(SceneSelectUpdateView *view)
{
    u8 compilerStorage[4];
    view->stateTimer.Reset();
}

static __forceinline i32 SceneSelectInitialSelectionGroupPhase(
    SceneSelectUpdateView *view)
{
    u8 compilerStorage[0x10];
    return view->groupCursor.GetCurrent();
}

static __forceinline i32 SceneSelectPostTransitionGroupPhase(
    SceneSelectUpdateView *view)
{
    u8 compilerStorage[0x10];
    return view->groupCursor.GetCurrent();
}

static __forceinline void SceneSelectClearPreviewVmPhase(SceneSelectUpdateView *view)
{
    SceneAnmVmId clearedVmId = {0};
    view->vmIds.values[0x82] = clearedVmId;
}

static __forceinline void SceneSelectCreateIndexedVm(
    SceneSelectUpdateView *view, i32 vmIndex)
{
    view->vmIds[vmIndex] = view->sceneAnm->CreateVm(vmIndex, 7);
}

static __forceinline void SceneSelectInitialSceneVmPhase(SceneSelectUpdateView *view, i32 initialSceneIndex)
{
            SceneSelectCreateIndexedVm(view, initialSceneIndex * 3 + 0x25);
            SceneSelectCreateIndexedVm(view, initialSceneIndex * 3 + 0x26);
            SceneSelectCreateIndexedVm(view, initialSceneIndex * 3 + 0x27);

            SET_SCENE_VM_VISIBILITY(
                view, initialSceneIndex * 3 + 0x27,
                g_ResultSaveData
                        ->sceneScores[
                            g_SceneGroups[view->groupCursor.GetCurrent()]
                                         [initialSceneIndex]
                                             .scoreEntryIndex]
                        .score != 0);
            if (g_ResultSaveData->IsSceneGroupUnlocked(
                    view->groupCursor.GetCurrent()) == 0)
            {
                g_AnmManager
                    ->GetVm(view->vmIds[initialSceneIndex * 3 + 0x25])
                    ->flagsWord &= ~2u;
                g_AnmManager
                    ->GetVm(view->vmIds[initialSceneIndex * 3 + 0x26])
                    ->flagsWord &= ~2u;
                g_AnmManager
                    ->GetVm(view->vmIds[initialSceneIndex * 3 + 0x27])
                    ->flagsWord &= ~2u;
            }
        }

static __forceinline void SceneSelectInitialVmCreatePhase(SceneSelectUpdateView *view, i32 initialVmIndex)
{
            SceneSelectCreateIndexedVm(view, 0x4c + initialVmIndex);
        }

static __forceinline void SceneSelectNewGroupSceneVmPhase(SceneSelectUpdateView *view, i32 newGroupIndex, i32 groupDirection)
{
            SceneSelectCreateIndexedVm(view, newGroupIndex * 3 + 0x25);
            SceneSelectCreateIndexedVm(view, newGroupIndex * 3 + 0x26);
            SceneSelectCreateIndexedVm(view, newGroupIndex * 3 + 0x27);
            view->vmIds.SetInterrupt(
                newGroupIndex * 3 + 0x25,
                groupDirection > 0 ? 10 : 9);
            view->vmIds.SetInterrupt(
                newGroupIndex * 3 + 0x26,
                groupDirection > 0 ? 10 : 9);
            view->vmIds.SetInterrupt(
                newGroupIndex * 3 + 0x27,
                groupDirection > 0 ? 10 : 9);

            SET_SCENE_VM_VISIBILITY(
                view, newGroupIndex * 3 + 0x27,
                g_ResultSaveData
                        ->sceneScores[
                            g_SceneGroups[view->groupCursor.GetCurrent()]
                                         [newGroupIndex]
                                             .scoreEntryIndex]
                        .score != 0);
            if (g_ResultSaveData->IsSceneGroupUnlocked(
                    view->groupCursor.GetCurrent()) == 0)
            {
                g_AnmManager
                    ->GetVm(view->vmIds[newGroupIndex * 3 + 0x25])
                    ->flagsWord &= ~2u;
                g_AnmManager
                    ->GetVm(view->vmIds[newGroupIndex * 3 + 0x26])
                    ->flagsWord &= ~2u;
                g_AnmManager
                    ->GetVm(view->vmIds[newGroupIndex * 3 + 0x27])
                    ->flagsWord &= ~2u;
            }
        }

static __forceinline void SceneSelectCreateVmAt(SceneSelectUpdateView *view, i32 vmIndex)
{
    view->vmIds[vmIndex] = view->sceneAnm->CreateVm(vmIndex, 7);
}

struct SceneSelectCursorCountSetterView
{
    i32 current; i32 previous; i32 count;
    __forceinline void SetCount(i32 value) { count = value; }
};

static __forceinline SceneSelectScoreFlagsView *SceneSelectScoreFlagsAt(
    SceneSaveDataView *saveData, i32 scoreIndex)
{
    return reinterpret_cast<SceneSelectScoreFlagsView *>(
        &saveData->sceneScores[scoreIndex].flags);
}

static __forceinline i32 SceneSelectDrainedQueueSizePhase(SceneValueQueue *queue)
{
    u8 compilerStorage[4];
    return queue->Size();
}

#define loadedSceneSize0 resultDrawBacking157
#define loadedSceneSize1 resultDrawBacking153
#define loadedSceneConditionQueue resultDrawBacking146
#define loadedSceneConditionValue resultDrawBacking142
#define loadedSceneLoadQueue resultDrawBacking096
#define loadedSceneLoadValue resultDrawBacking092
static __forceinline void SceneSelectProcessLoadedSceneQueue(
    SceneSelectUpdateView *view, SceneSelectControllerView *owner)
{
    i32 loadedSceneLoadValue;
    SceneValueQueue *loadedSceneLoadQueue;
    i32 loadedSceneConditionValue;
    SceneValueQueue *loadedSceneConditionQueue;
    i32 loadedSceneSize1;
    i32 loadedSceneSize0;
    loadedSceneSize0 = view->loadedSceneQueue.count;
    if (loadedSceneSize0 != 0)
    {
        loadedSceneSize1 = view->loadedSceneQueue.count;
        if (loadedSceneSize1 == 1)
        {
            loadedSceneConditionQueue = &view->loadedSceneQueue;
            if (loadedSceneConditionQueue->count > 0)
                loadedSceneConditionValue = loadedSceneConditionQueue->values[0];
            else
                loadedSceneConditionValue = 0;
            if (loadedSceneConditionValue >= 0)
            {
                loadedSceneLoadQueue = &view->loadedSceneQueue;
                if (loadedSceneLoadQueue->count > 0)
                    loadedSceneLoadValue = loadedSceneLoadQueue->values[0];
                else
                    loadedSceneLoadValue = 0;
                g_ResultSaveData->LoadScenePreviewTexture(
                    view->sceneAnm, 1, loadedSceneLoadValue);
                view->vmIds.SetInterrupt(0x82, 1);
                SceneSelectCreateVmAt(view, 0x82);
                reinterpret_cast<AnmTextManagerView *>(g_AnmManager)
                    ->DrawTextCentered(
                        reinterpret_cast<AnmTextVmView *>(
                            g_AnmManager->GetVm(view->vmIds.values[0x82])),
                        0x00efcfcf, 0,
                        reinterpret_cast<char *>(g_ResultSaveData) +
                            reinterpret_cast<SceneQueueFrontMemberView *>(
                                &view->loadedSceneQueue)->Front() * 0x78 + 0x3178);
            }
        }
        g_Supervisor.EnterCriticalSectionWrapper(4);
        g_Supervisor.criticalSectionLockCounts[4]++;
        view->loadedSceneQueue.Pop();
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        g_Supervisor.criticalSectionLockCounts[4]--;
        owner->UpdateSelectedSceneDetails();
    }
}
#undef loadedSceneSize0
#undef loadedSceneSize1
#undef loadedSceneConditionQueue
#undef loadedSceneConditionValue
#undef loadedSceneLoadQueue
#undef loadedSceneLoadValue

#define loadedGroupSize0 resultDrawBacking157
#define loadedGroupFrontQueue resultDrawBacking153
#define loadedGroupFrontValue resultDrawBacking146
#define loadedGroupSize1 resultDrawBacking142
#define groupPreviewSizePositiveQueue resultDrawBacking096
#define groupPreviewSizePositiveValue resultDrawBacking092
#define groupPreviewDataPositiveQueue resultDrawBacking026
#define groupPreviewDataPositiveValue resultDrawBacking022
#define scenePreviewSizePositiveQueue resultDrawBacking139
#define scenePreviewSizePositiveValue resultDrawBacking135
#define scenePreviewDataPositiveQueue resultDrawBacking131
#define scenePreviewDataPositiveValue resultDrawBacking119
#define groupPreviewDataPositiveFreeQueue resultDrawBacking115
#define groupPreviewDataPositiveFreeValue resultDrawBacking111
#define scenePreviewDataPositiveFreeQueue resultDrawBacking089
#define scenePreviewDataPositiveFreeValue resultDrawBacking085
#define loadedGroupNegativeSize resultDrawBacking081
#define groupPreviewSizeNegativeQueue resultDrawBacking017
#define groupPreviewSizeNegativeValue resultDrawBacking013
#define groupPreviewDataNegativeQueue resultDrawBacking127
#define groupPreviewDataNegativeValue resultDrawBacking123
#define groupPreviewDataNegativeFreeQueue resultDrawBacking107
#define groupPreviewDataNegativeFreeValue resultDrawBacking103
ChainCallbackResult SceneSelectControllerView::UpdateSceneSelect()
{
#define view (reinterpret_cast<SceneSelectUpdateView *>(this))
#define activeSceneCursor                                                   \
    (view->sceneCursors[view->groupCursor.GetCurrent()])
    SceneSelectShallowLocals shallow;
#define i shallow.i
#define groupDirection shallow.groupDirection
#define initialCursorIndex shallow.initialCursorIndex
#define initialSceneIndex shallow.initialSceneIndex
#define initialVmIndex shallow.initialVmIndex
#define initialGroupVm shallow.initialGroupVm
#define initialSelectionIndex shallow.initialSelectionIndex
#define previousGroupIndex shallow.previousGroupIndex
#define newGroupIndex shallow.newGroupIndex
#define transitionGroupVm shallow.transitionGroupVm
#define selectionIndex shallow.selectionIndex
#define selected shallow.selected
#define gameExitVmIndex shallow.gameExitVmIndex
#define menuExitSceneIndex shallow.menuExitSceneIndex
#define menuExitVmIndex shallow.menuExitVmIndex
#define previewVm shallow.previewVm
#define pendingPrimaryFree shallow.pendingPrimaryFree
#define pendingSecondaryFree shallow.pendingSecondaryFree

    /* Consume pending texture uploads before the asynchronous queues. */
    if (view->pendingTextureCount != 0)
    {
        if (view->pendingTextureCount == 1)
        {
            g_AnmManager->LoadTexture(
                &view->sceneAnm->textures[3],
                reinterpret_cast<u8 *>(view->pendingPrimaryData[0]),
                view->pendingPrimarySize[0],
                1, 0, 1);
            view->sceneAnm->textures[3].texture->PreLoad();
            g_AnmManager->LoadTexture(
                &view->sceneAnm->textures[4],
                reinterpret_cast<u8 *>(view->pendingSecondaryData[0]),
                view->pendingSecondarySize[0],
                1, 0, 1);
            view->sceneAnm->textures[4].texture->PreLoad();
        }
        pendingPrimaryFree =
            reinterpret_cast<void *>(view->pendingPrimaryData[0]);
        free(pendingPrimaryFree);
        view->pendingPrimaryData[0] = 0;
        pendingSecondaryFree =
            reinterpret_cast<void *>(view->pendingSecondaryData[0]);
        free(pendingSecondaryFree);
        view->pendingSecondaryData[0] = 0;

        g_Supervisor.EnterCriticalSectionWrapper(4);
        g_Supervisor.criticalSectionLockCounts[4]++;
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
        view->pendingSecondaryData[2] = 0;
        view->pendingPrimarySize[2] = 0;
        view->pendingSecondarySize[2] = 0;
        view->pendingTextureCount--;
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        g_Supervisor.criticalSectionLockCounts[4]--;

        if (view->pendingTextureCount == 0)
        {
            view->vmIds.SetInterrupt(0x15, 4);
            view->vmIds.SetInterrupt(0x16, 4);
        }
    }
    else
    {
        i32 groupPreviewDataNegativeFreeValue;
            SceneValueQueue *groupPreviewDataNegativeFreeQueue;
            i32 groupPreviewDataNegativeValue;
            SceneValueQueue *groupPreviewDataNegativeQueue;
            i32 groupPreviewSizeNegativeValue;
            SceneValueQueue *groupPreviewSizeNegativeQueue;
            i32 loadedGroupNegativeSize;
            i32 scenePreviewDataPositiveFreeValue;
            SceneValueQueue *scenePreviewDataPositiveFreeQueue;
            i32 groupPreviewDataPositiveFreeValue;
            SceneValueQueue *groupPreviewDataPositiveFreeQueue;
            i32 scenePreviewDataPositiveValue;
            SceneValueQueue *scenePreviewDataPositiveQueue;
            i32 scenePreviewSizePositiveValue;
            SceneValueQueue *scenePreviewSizePositiveQueue;
            i32 groupPreviewDataPositiveValue;
            SceneValueQueue *groupPreviewDataPositiveQueue;
            i32 groupPreviewSizePositiveValue;
            SceneValueQueue *groupPreviewSizePositiveQueue;
            i32 loadedGroupSize1;
            i32 loadedGroupFrontValue;
            SceneValueQueue *loadedGroupFrontQueue;
            i32 loadedGroupSize0;
        loadedGroupSize0 = view->loadedGroupQueue.count;
        if (loadedGroupSize0 != 0)
        {
            loadedGroupFrontQueue = &view->loadedGroupQueue;
            if (loadedGroupFrontQueue->count > 0)
            {
                loadedGroupFrontValue =
                    loadedGroupFrontQueue->values[0];
            }
            else
            {
                loadedGroupFrontValue = 0;
            }
            if (loadedGroupFrontValue >= 0)
            {
                loadedGroupSize1 =
                    view->loadedGroupQueue.count;
                if (loadedGroupSize1 == 1)
                {
                    groupPreviewSizePositiveQueue =
                        &view->groupPreviewSizeQueue;
                    if (groupPreviewSizePositiveQueue->count > 0)
                    {
                        groupPreviewSizePositiveValue =
                            groupPreviewSizePositiveQueue
                                ->values[0];
                    }
                    else
                    {
                        groupPreviewSizePositiveValue = 0;
                    }
                    groupPreviewDataPositiveQueue =
                        &view->groupPreviewDataQueue;
                    if (groupPreviewDataPositiveQueue->count > 0)
                    {
                        groupPreviewDataPositiveValue =
                            groupPreviewDataPositiveQueue
                                ->values[0];
                    }
                    else
                    {
                        groupPreviewDataPositiveValue = 0;
                    }
                    g_AnmManager->LoadTexture(
                        &view->sceneAnm->textures[2],
                        reinterpret_cast<u8 *>(
                            groupPreviewDataPositiveValue),
                        groupPreviewSizePositiveValue,
                        5, 0, 1);

                    scenePreviewSizePositiveQueue =
                        &view->scenePreviewSizeQueue;
                    if (scenePreviewSizePositiveQueue->count > 0)
                    {
                        scenePreviewSizePositiveValue =
                            scenePreviewSizePositiveQueue
                                ->values[0];
                    }
                    else
                    {
                        scenePreviewSizePositiveValue = 0;
                    }
                    scenePreviewDataPositiveQueue =
                        &view->scenePreviewDataQueue;
                    if (scenePreviewDataPositiveQueue->count > 0)
                    {
                        scenePreviewDataPositiveValue =
                            scenePreviewDataPositiveQueue
                                ->values[0];
                    }
                    else
                    {
                        scenePreviewDataPositiveValue = 0;
                    }
                    g_AnmManager->LoadTextureRegion(
                        &view->sceneAnm->textures[2],
                        reinterpret_cast<u8 *>(
                            scenePreviewDataPositiveValue),
                        scenePreviewSizePositiveValue,
                        5, 0, 1, 0x20);
                    view->sceneAnm->textures[2].texture->PreLoad();
                }

                groupPreviewDataPositiveFreeQueue =
                    &view->groupPreviewDataQueue;
                if (groupPreviewDataPositiveFreeQueue->count > 0)
                {
                    groupPreviewDataPositiveFreeValue =
                        groupPreviewDataPositiveFreeQueue
                            ->values[0];
                }
                else
                {
                    groupPreviewDataPositiveFreeValue = 0;
                }
                FreeSceneOwned(reinterpret_cast<void *>(
                    groupPreviewDataPositiveFreeValue));

                scenePreviewDataPositiveFreeQueue =
                    &view->scenePreviewDataQueue;
                if (scenePreviewDataPositiveFreeQueue->count > 0)
                {
                    scenePreviewDataPositiveFreeValue =
                        scenePreviewDataPositiveFreeQueue
                            ->values[0];
                }
                else
                {
                    scenePreviewDataPositiveFreeValue = 0;
                }
                FreeSceneOwned(reinterpret_cast<void *>(
                    scenePreviewDataPositiveFreeValue));
            }
            else
            {
                loadedGroupNegativeSize =
                    view->loadedGroupQueue.count;
                if (loadedGroupNegativeSize == 1)
                {
                    groupPreviewSizeNegativeQueue =
                        &view->groupPreviewSizeQueue;
                    if (groupPreviewSizeNegativeQueue->count > 0)
                    {
                        groupPreviewSizeNegativeValue =
                            groupPreviewSizeNegativeQueue
                                ->values[0];
                    }
                    else
                    {
                        groupPreviewSizeNegativeValue = 0;
                    }
                    groupPreviewDataNegativeQueue =
                        &view->groupPreviewDataQueue;
                    if (groupPreviewDataNegativeQueue->count > 0)
                    {
                        groupPreviewDataNegativeValue =
                            groupPreviewDataNegativeQueue
                                ->values[0];
                    }
                    else
                    {
                        groupPreviewDataNegativeValue = 0;
                    }
                    g_AnmManager->LoadTexture(
                        &view->sceneAnm->textures[2],
                        reinterpret_cast<u8 *>(
                            groupPreviewDataNegativeValue),
                        groupPreviewSizeNegativeValue,
                        5, 0, 1);
                    view->sceneAnm->textures[2].texture->PreLoad();
                }

                groupPreviewDataNegativeFreeQueue =
                    &view->groupPreviewDataQueue;
                if (groupPreviewDataNegativeFreeQueue->count > 0)
                {
                    groupPreviewDataNegativeFreeValue =
                        groupPreviewDataNegativeFreeQueue
                            ->values[0];
                }
                else
                {
                    groupPreviewDataNegativeFreeValue = 0;
                }
                FreeSceneOwned(reinterpret_cast<void *>(
                    groupPreviewDataNegativeFreeValue));
            }

            g_Supervisor.EnterCriticalSectionWrapper(4);
            g_Supervisor.criticalSectionLockCounts[4]++;
            view->loadedGroupQueue.Pop();
            SceneSelectQueuePopPhase(&view->groupPreviewDataQueue);
            SceneSelectQueuePopPhase(&view->scenePreviewDataQueue);
            SceneSelectQueuePopPhase(&view->groupPreviewSizeQueue);
            SceneSelectQueuePopPhase(&view->scenePreviewSizeQueue);
            g_Supervisor.LeaveCriticalSectionWrapper(4);
            g_Supervisor.criticalSectionLockCounts[4]--;
            if (SceneSelectDrainedQueueSizePhase(&view->loadedGroupQueue) == 0)
                view->vmIds.SetInterrupt(0x14, 4);
        }
        else
        {
            SceneSelectProcessLoadedSceneQueue(view, this);
        }
    }

    switch (view->state)
    {
    default:
        goto update_preview_text;
    case 0:
    {
        g_Supervisor.StopReplayScan();
        view->sceneAnm->textures[0].Clear();
        SceneSelectInitialTimerViewPhase(view);
        view->groupCursor.Push();

        if (g_ResultSaveData->FindHighestUnlockedSceneGroup() < 2)
        {
            view->groupCursor.count = 3;
        }
        else if (g_ResultSaveData->FindHighestUnlockedSceneGroup() < 5)
        {
            view->groupCursor.count = 6;
        }
        else
        {
            reinterpret_cast<SceneSelectCursorCountSetterView *>(&view->groupCursor)
                ->SetCount(
                    12 <= g_ResultSaveData->FindHighestUnlockedSceneGroup() + 2
                        ? 12 : g_ResultSaveData->FindHighestUnlockedSceneGroup() + 2);
        }
        view->groupCursor.wraps = 1;
        view->state = 1;
        view->flags |= 0x10;
        view->selectedScoreEntryIndex = 0;

        for (initialCursorIndex = 0; initialCursorIndex < 12;
             initialCursorIndex++)
        {
            reinterpret_cast<SceneSelectCursorCountSetterView *>(
                &view->sceneCursors[initialCursorIndex])
                ->SetCount(g_SceneGroupCounts[initialCursorIndex]);
            view->sceneCursors[initialCursorIndex].wraps = 1;
            view->sceneCursors[initialCursorIndex].Set(0);
        }
        view->groupCursor.Set(g_ResultSaveData->lastSelectedGroup);
        view->sceneCursors[view->groupCursor.GetCurrent()].Set(
            g_ResultSaveData->lastSelectedScene);

        g_SelectedScene =
            &g_SceneGroups[view->groupCursor.GetCurrent()]
                          [view->sceneCursors[view->groupCursor.GetCurrent()].GetCurrent()];
        view->selectedScoreEntryIndex = g_SelectedScene->scoreEntryIndex;

        SceneSelectCreateVmAt(view, 0x68);
        SceneSelectCreateVmAt(view, 0x69);
        view->vmIds.SetInterrupt(0x19, 3);
        view->vmIds.SetInterrupt(0x1a, 3);
        view->transitionVm.SetInterrupt(3);
        view->vmIds.SetInterrupt(0x1b, 3);
        SceneSelectCreateVmAt(view, 0x1e);
        SceneSelectCreateVmAt(view, 0x22);
        if (view->groupCursor.GetCurrent() <= 10)
        {
            SceneSelectCreateVmAt(view, 0x20);
            view->sceneAnm->SetSprite(
                g_AnmManager->GetVm(view->vmIds[0x22]),
                view->groupCursor.GetCurrent() + 0x37);
        }
        else
        {
            SceneSelectCreateVmAt(view, 0x21);
            view->sceneAnm->SetSprite(
                g_AnmManager->GetVm(view->vmIds[0x22]),
                view->groupCursor.GetCurrent() + 0x2c);
        }
        SceneSelectCreateVmAt(view, 0x23);
        SceneSelectCreateVmAt(view, 0x24);
        SET_SCENE_VM_VISIBILITY(
            view, 0x23, view->groupCursor.GetCurrent() != 0);
        SET_SCENE_VM_VISIBILITY(
            view, 0x24,
            view->groupCursor.GetCurrent() != view->groupCursor.GetCount() - 1);

        SceneSelectCreateVmAt(view, 0x44);
        SceneSelectCreateVmAt(view, 0x48);
        SceneSelectCreateVmAt(view, 0x15);
        SceneSelectCreateVmAt(view, 0x16);
        SceneSelectCreateVmAt(view, 0x45);
        SceneSelectCreateVmAt(view, 0x46);

        for (initialSceneIndex = 0;
             initialSceneIndex <
                 g_SceneGroupCounts[view->groupCursor.GetCurrent()];
             initialSceneIndex++)
        {
            SceneSelectInitialSceneVmPhase(view, initialSceneIndex);
        }

        view->groupPreviewQueue.capacity = 5;
        view->scenePreviewQueue.capacity = 5;
        view->loadedGroupQueue.capacity = 16;
        view->groupPreviewDataQueue.capacity = 16;
        view->groupPreviewSizeQueue.capacity = 16;
        view->scenePreviewDataQueue.capacity = 16;
        view->scenePreviewSizeQueue.capacity = 16;
        view->loadedGroupQueue.capacity = 16;
        view->selectionQueue.capacity = 5;
        view->loadedSceneQueue.capacity = 16;
        view->flags &= ~0x20u;

        g_Supervisor.StartReplayScan(LoadSceneSelectionAssets, NULL);
        SceneSelectCreateVmAt(view, 0x13);
        SceneSelectCreateVmAt(view, 0x12);
        view->flags |= 4;
        SceneSelectCreateVmAt(view, 0x14);
        SceneSelectCreateVmAt(view, 0x49);
        SceneSelectCreateVmAt(view, 0x4a);
        SceneSelectCreateVmAt(view, 0x4b);

        view->sceneAnm->SetSprite(
            g_AnmManager->GetVm(view->vmIds[0x49]),
            (view->groupCursor.GetCurrent() >= 11
                 ? view->groupCursor.GetCurrent() - 11
                 : view->groupCursor.GetCurrent()) +
                0x28);
        view->sceneAnm->SetSprite(
            g_AnmManager->GetVm(view->vmIds[0x4b]),
            view->sceneCursors[view->groupCursor.GetCurrent()].GetCurrent() + 0x28);

        for (initialVmIndex = 0; initialVmIndex < 16; initialVmIndex++)
        {
            SceneSelectInitialVmCreatePhase(view, initialVmIndex);
        }
        SceneSelectCreateVmAt(view, 0x5c);
        SceneSelectCreateVmAt(view, 0x5d);
        SceneSelectCreateVmAt(view, 0x5e);
        SceneSelectCreateVmAt(view, 0x5f);
        SceneSelectCreateVmAt(view, 0x60);
        SceneSelectCreateVmAt(view, 0x61);
        SceneSelectCreateVmAt(view, 0x62);
        SceneSelectCreateVmAt(view, 0x63);

        initialGroupVm = view->vmIds.values[0x68].GetVm();
        if (g_ResultSaveData->IsSceneGroupUnlocked(
                view->groupCursor.GetCurrent()) != 0)
        {
            reinterpret_cast<AnmVmColorInterpolationView *>(
                initialGroupVm)
                ->SetColor1Interpolation(
                    60, 0, initialGroupVm->color1.color,
                    g_SceneGroupColors[view->groupCursor.GetCurrent()]);
        }
        else
        {
            reinterpret_cast<AnmVmColorInterpolationView *>(
                initialGroupVm)
                ->SetColor1Interpolation(
                    60, 0, initialGroupVm->color1.color,
                    g_SceneLockedInitialColor);
        }
        this->UpdateSelectedSceneDetails();
    }
    case 1:
        break;
    }

    if ((view->stateTimer.current < 30) != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if ((view->stateTimer.current == 30) != 0)
    {
        for (initialSelectionIndex = 0;
             initialSelectionIndex <
                 g_SceneGroupCounts[SceneSelectInitialSelectionGroupPhase(view)];
             initialSelectionIndex++)
        {
            if (view->sceneCursors[view->groupCursor.GetCurrent()].GetCurrent() ==
                initialSelectionIndex)
            {
                view->vmIds.SetInterrupt(initialSelectionIndex * 3 + 0x25, 2);
                view->vmIds.SetInterrupt(initialSelectionIndex * 3 + 0x26, 2);
                view->vmIds.SetInterrupt(initialSelectionIndex * 3 + 0x27, 2);
            }
            else
            {
                view->vmIds.SetInterrupt(initialSelectionIndex * 3 + 0x25, 3);
                view->vmIds.SetInterrupt(initialSelectionIndex * 3 + 0x26, 3);
                view->vmIds.SetInterrupt(initialSelectionIndex * 3 + 0x27, 3);
            }
        }
        this->RefreshSceneSelection(view->selectedScoreEntryIndex);
    }

    if (SceneInputAnd(g_PressedButtons, TH_BUTTON_S) != 0)
    {
        view->flagBits.showRates = 1 - view->flagBits.showRates;
    }

    if (SceneInputAnd(g_FrontEndCurrentInput, TH_BUTTON_SKIP) != 0)
    {
        g_AnmManager->GetVm(view->vmIds[0x15])->flagsWord &= ~2u;
        g_AnmManager->GetVm(view->vmIds[0x16])->flagsWord &= ~2u;
        g_AnmManager->GetVm(view->vmIds[0x45])->flagsWord &= ~2u;
        if (view->previewTextVmIds[0].GetVm() != NULL)
        {
            view->previewTextVmIds[0].GetVm()->flagsWord &= ~2u;
        }
        if (view->previewTextVmIds[1].GetVm() != NULL)
        {
            view->previewTextVmIds[1].GetVm()->flagsWord &= ~2u;
        }
        if (view->previewTextVmIds[2].GetVm() != NULL)
        {
            view->previewTextVmIds[2].GetVm()->flagsWord &= ~2u;
        }
        view->flags &= ~0x10u;
    }
    else
    {
        g_AnmManager->GetVm(view->vmIds[0x15])->flagsWord |= 2;
        g_AnmManager->GetVm(view->vmIds[0x16])->flagsWord |= 2;
        g_AnmManager->GetVm(view->vmIds[0x45])->flagsWord |= 2;
        if (view->previewTextVmIds[0].GetVm() != NULL)
        {
            view->previewTextVmIds[0].GetVm()->flagsWord |= 2;
        }
        if (view->previewTextVmIds[1].GetVm() != NULL)
        {
            view->previewTextVmIds[1].GetVm()->flagsWord |= 2;
        }
        if (view->previewTextVmIds[2].GetVm() != NULL)
        {
            view->previewTextVmIds[2].GetVm()->flagsWord |= 2;
        }
        view->flags |= 0x10;
    }

    if (SceneInputAnd(g_PressedButtons, (u16)0x8000) != 0 &&
        *reinterpret_cast<u16 *>(
            &g_ResultSaveData
                 ->sceneScores[view->selectedScoreEntryIndex]) != 0)
    {
        SceneSelectScoreFlagsAt(
            g_ResultSaveData, view->selectedScoreEntryIndex)
            ->showSuccessRateMarker ^= 1;
    }

    view->groupCursor.SaveCurrent();
    groupDirection = 0;
    if (SceneInputAnd(g_PressedButtons, TH_BUTTON_LEFT) != 0)
    {
        view->groupCursor.Move(-1);
        groupDirection = -1;
    }
    if (SceneInputAnd(g_PressedButtons, TH_BUTTON_RIGHT) != 0)
    {
        view->groupCursor.Move(1);
        groupDirection = 1;
    }

    if (view->groupCursor.HasChanged())
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        g_AnmManager->MarkVmForDeletion(view->vmIds[0x20]);
        g_AnmManager->MarkVmForDeletion(view->vmIds[0x21]);
        g_AnmManager->MarkVmForDeletion(view->vmIds[0x22]);
        SceneSelectCreateVmAt(view, 0x22);
        if (view->groupCursor.GetCurrent() <= 10)
        {
            SceneSelectCreateVmAt(view, 0x20);
            view->sceneAnm->SetSprite(
                g_AnmManager->GetVm(view->vmIds[0x22]),
                view->groupCursor.GetCurrent() + 0x37);
        }
        else
        {
            SceneSelectCreateVmAt(view, 0x21);
            view->sceneAnm->SetSprite(
                g_AnmManager->GetVm(view->vmIds[0x22]),
                view->groupCursor.GetCurrent() + 0x2c);
        }

        g_SelectedScene =
            &g_SceneGroups[view->groupCursor.GetCurrent()]
                          [activeSceneCursor.GetCurrent()];
        view->selectedScoreEntryIndex = g_SelectedScene->scoreEntryIndex;

        for (previousGroupIndex = 0;
             previousGroupIndex <
                 g_SceneGroupCounts[view->groupCursor.GetPrevious()];
             previousGroupIndex++)
        {
            view->vmIds.SetInterrupt(
                previousGroupIndex * 3 + 0x25,
                groupDirection <= 0 ? 8 : 7);
            view->vmIds.SetInterrupt(
                previousGroupIndex * 3 + 0x26,
                groupDirection <= 0 ? 8 : 7);
            view->vmIds.SetInterrupt(
                previousGroupIndex * 3 + 0x27,
                groupDirection <= 0 ? 8 : 7);
        }
        for (newGroupIndex = 0;
             newGroupIndex < g_SceneGroupCounts[view->groupCursor.GetCurrent()];
             newGroupIndex++)
        {
            SceneSelectNewGroupSceneVmPhase(view, newGroupIndex, groupDirection);
        }

        view->vmIds.SetInterrupt(0x12, groupDirection <= 0 ? 8 : 7);
        SceneSelectCreateVmAt(view, 0x12);
        view->vmIds.SetInterrupt(0x12, 5);
        view->vmIds.SetInterrupt(0x12, groupDirection > 0 ? 10 : 9);
        view->vmIds.SetInterrupt(0x14, 5);
        view->stateTimer.Set(20);
        view->sceneAnm->SetSprite(
            g_AnmManager->GetVm(view->vmIds[0x49]),
            (view->groupCursor.GetCurrent() >= 11
                 ? view->groupCursor.GetCurrent() - 11
                 : view->groupCursor.GetCurrent()) +
                0x28);
        view->sceneAnm->SetSprite(
            g_AnmManager->GetVm(view->vmIds[0x4b]),
            activeSceneCursor.GetCurrent() + 0x28);
        view->vmIds.SetInterrupt(0x49, 2);
        view->vmIds.SetInterrupt(0x4a, 2);
        view->vmIds.SetInterrupt(0x4b, 2);
        this->UpdateSelectedSceneDetails();
        g_ResultSaveData->lastSelectedGroup =
            (i16)view->groupCursor.GetCurrent();
        g_ResultSaveData->lastSelectedScene =
            (i16)activeSceneCursor.GetCurrent();

        SET_SCENE_VM_VISIBILITY(
            view, 0x23, view->groupCursor.GetCurrent() != 0);
        SET_SCENE_VM_VISIBILITY(
            view, 0x24,
            view->groupCursor.GetCurrent() != view->groupCursor.GetCount() - 1);

        transitionGroupVm = view->vmIds.values[0x68].GetVm();
        if (g_ResultSaveData->IsSceneGroupUnlocked(
                view->groupCursor.GetCurrent()) != 0)
        {
            reinterpret_cast<AnmVmColorInterpolationView *>(
                transitionGroupVm)
                ->SetColor1Interpolation(
                    60, 0, transitionGroupVm->color1.color,
                    g_SceneGroupColors[view->groupCursor.GetCurrent()]);
        }
        else
        {
            reinterpret_cast<AnmVmColorInterpolationView *>(
                transitionGroupVm)
                ->SetColor1Interpolation(
                    60, 0, transitionGroupVm->color1.color,
                    g_SceneLockedTransitionColor);
        }    }
    else
    {
        if (g_ResultSaveData->IsSceneGroupUnlocked(
                SceneSelectPostTransitionGroupPhase(view)) != 0)
        {
            activeSceneCursor.SaveCurrent();
            if ((u16)(SceneInputAnd(g_PressedButtons, TH_BUTTON_UP) != 0 ||
                      (g_ResultMenuInput & ScenePreviousInputMask()) != 0) != 0)
            {
                activeSceneCursor.Move(-1);
            }
            if ((u16)(SceneInputAnd(g_PressedButtons, TH_BUTTON_DOWN) != 0 ||
                      (g_ResultMenuInput & SceneNextInputMask()) != 0) != 0)
            {
                activeSceneCursor.Move(1);
            }

            if (activeSceneCursor.HasChanged())
            {
                g_SelectedScene =
                    &g_SceneGroups[view->groupCursor.GetCurrent()]
                                  [activeSceneCursor.GetCurrent()];
                view->selectedScoreEntryIndex =
                    g_SelectedScene->scoreEntryIndex;
                for (selectionIndex = 0;
                     selectionIndex <
                         g_SceneGroupCounts[view->groupCursor.GetCurrent()];
                     selectionIndex++)
                {
                    if (activeSceneCursor.GetCurrent() == selectionIndex)
                    {
                        view->vmIds.SetInterrupt(
                            selectionIndex * 3 + 0x25, 2);
                        view->vmIds.SetInterrupt(
                            selectionIndex * 3 + 0x26, 2);
                        view->vmIds.SetInterrupt(
                            selectionIndex * 3 + 0x27, 2);
                    }
                    else
                    {
                        view->vmIds.SetInterrupt(
                            selectionIndex * 3 + 0x25, 3);
                        view->vmIds.SetInterrupt(
                            selectionIndex * 3 + 0x26, 3);
                        view->vmIds.SetInterrupt(
                            selectionIndex * 3 + 0x27, 3);
                    }
                }
                g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
                this->RefreshSceneSelection(view->selectedScoreEntryIndex);
                view->vmIds.SetInterrupt(0x12, 5);
                view->vmIds.SetInterrupt(0x14, 5);
                view->sceneAnm->SetSprite(
                    g_AnmManager->GetVm(view->vmIds[0x49]),
                    (view->groupCursor.GetCurrent() >= 11
                         ? view->groupCursor.GetCurrent() - 11
                         : view->groupCursor.GetCurrent()) +
                        0x28);
                view->sceneAnm->SetSprite(
                    g_AnmManager->GetVm(view->vmIds[0x4b]),
                    activeSceneCursor.GetCurrent() + 0x28);
                view->vmIds.SetInterrupt(0x49, 2);
                view->vmIds.SetInterrupt(0x4a, 2);
                view->vmIds.SetInterrupt(0x4b, 2);
                this->UpdateSelectedSceneDetails();
                g_ResultSaveData->lastSelectedScene =
                    (i16)activeSceneCursor.GetCurrent();
                goto update_preview_text;
            }

            if (SceneInputAnd(g_PressedButtons, (u16)0x1002) != 0)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                g_SelectedScene =
                    &g_SceneGroups[view->groupCursor.GetCurrent()]
                                  [activeSceneCursor.GetCurrent()];
                view->requestedState = 5;
                view->state = 0;
                view->stateTimer.Reset();
                view->flags |= 0x20;
                g_ReplayBrowserExitSignal.Request();
                g_AnmManager->MarkVmForDeletion(
                    view->previewTextVmIds[0]);
                g_AnmManager->MarkVmForDeletion(
                    view->previewTextVmIds[1]);
                g_AnmManager->MarkVmForDeletion(
                    view->previewTextVmIds[2]);
                selected = activeSceneCursor.GetCurrent();
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
                for (gameExitVmIndex = 0; gameExitVmIndex < 16;
                     gameExitVmIndex++)
                {
                    view->vmIds.SetInterrupt(0x4c + gameExitVmIndex, 1);
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
                SceneSelectClearPreviewVmPhase(view);
                return CHAIN_CALLBACK_RESULT_CONTINUE;
            }
        }

        if (SceneInputAnd(g_PressedButtons, 9) != 0)
        {
            view->flags |= 0x20;
            g_SelectedScene =
                &g_SceneGroups[view->groupCursor.GetCurrent()]
                              [view->sceneCursors[view->groupCursor.GetCurrent()]
                                   .GetCurrent()];
            view->groupCursor.Pop();
            view->requestedState = 1;
            view->state = 0;
            view->stateTimer.Reset();
            view->vmIds.SetInterrupt(0x68, 1);
            view->vmIds.SetInterrupt(0x69, 1);
            SceneSelectCreateVmAt(view, 0x66);
            SceneSelectCreateVmAt(view, 0x67);
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
            for (menuExitSceneIndex = 0; menuExitSceneIndex < 9;
                 menuExitSceneIndex++)
            {
                view->vmIds.SetInterrupt(menuExitSceneIndex * 3 + 0x25, 1);
                view->vmIds.SetInterrupt(menuExitSceneIndex * 3 + 0x26, 1);
                view->vmIds.SetInterrupt(menuExitSceneIndex * 3 + 0x27, 1);
            }
            view->vmIds.SetInterrupt(0x13, 1);
            view->vmIds.SetInterrupt(0x12, 1);
            view->vmIds.SetInterrupt(0x14, 1);
            view->vmIds.SetInterrupt(0x49, 1);
            view->vmIds.SetInterrupt(0x4a, 1);
            view->vmIds.SetInterrupt(0x4b, 1);
            for (menuExitVmIndex = 0; menuExitVmIndex < 16;
                 menuExitVmIndex++)
            {
                view->vmIds.SetInterrupt(0x4c + menuExitVmIndex, 1);
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
            g_AnmManager->SetInterrupt(view->previewTextVmIds[0], 1);
            g_AnmManager->SetInterrupt(view->previewTextVmIds[1], 1);
            g_AnmManager->SetInterrupt(view->previewTextVmIds[2], 1);
            view->vmIds.SetInterrupt(0x82, 1);
            SceneSelectClearPreviewVmPhase(view);
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
    }
update_preview_text:
    if (SceneQueueSize(&view->selectionQueue) != 0 ||
        SceneQueueSize(&view->loadedSceneQueue) != 0)
    {
        if (view->flagBits.previewPending == 0)
        {
            view->vmIds.SetInterrupt(0x12, 3);
            view->flags |= 4;
        }
    }
    else
    {
        if (view->flagBits.previewPending != 0)
        {
            previewVm =
                g_AnmManager->GetVm(view->vmIds.values[0x12]);
            if ((reinterpret_cast<u8 *>(g_ResultSaveData) +
                 view->selectedScoreEntryIndex * 0x78)[0x31c9] != 0)
            {
                previewVm->loadedSprite->uvEndX =
                    (f32)*reinterpret_cast<u16 *>(
                        reinterpret_cast<u8 *>(g_ResultSaveData) +
                        view->selectedScoreEntryIndex * 0x78 + 0x316c) /
                    256.0f;
                previewVm->loadedSprite->uvEndY =
                    (f32)*reinterpret_cast<u16 *>(
                        reinterpret_cast<u8 *>(g_ResultSaveData) +
                        view->selectedScoreEntryIndex * 0x78 + 0x316e) /
                    256.0f;
                previewVm->spriteWidth =
                    (f32)*reinterpret_cast<u16 *>(
                        reinterpret_cast<u8 *>(g_ResultSaveData) +
                        view->selectedScoreEntryIndex * 0x78 + 0x316c);
                previewVm->spriteHeight =
                    (f32)*reinterpret_cast<u16 *>(
                        reinterpret_cast<u8 *>(g_ResultSaveData) +
                        view->selectedScoreEntryIndex * 0x78 + 0x316e);
                previewVm->flagsWord |= 2;
            }
            else
            {
                previewVm->flagsWord &= ~2u;
            }
            g_AnmManager->SetInterrupt(view->vmIds.values[0x12], 2);
            view->flags &= ~4u;
        }
    }

    this->BuildScenePreviewText();
    return CHAIN_CALLBACK_RESULT_CONTINUE;
#undef pendingSecondaryFree
#undef activeSceneCursor
#undef pendingPrimaryFree
#undef previewVm
#undef menuExitVmIndex
#undef menuExitSceneIndex
#undef gameExitVmIndex
#undef selected
#undef selectionIndex
#undef transitionGroupVm
#undef newGroupIndex
#undef previousGroupIndex
#undef initialSelectionIndex
#undef initialGroupVm
#undef initialVmIndex
#undef initialSceneIndex
#undef initialCursorIndex
#undef groupDirection
#undef i
#undef loadedGroupSize0
#undef loadedGroupFrontQueue
#undef loadedGroupFrontValue
#undef loadedGroupSize1
#undef groupPreviewSizePositiveQueue
#undef groupPreviewSizePositiveValue
#undef groupPreviewDataPositiveQueue
#undef groupPreviewDataPositiveValue
#undef scenePreviewSizePositiveQueue
#undef scenePreviewSizePositiveValue
#undef scenePreviewDataPositiveQueue
#undef scenePreviewDataPositiveValue
#undef groupPreviewDataPositiveFreeQueue
#undef groupPreviewDataPositiveFreeValue
#undef scenePreviewDataPositiveFreeQueue
#undef scenePreviewDataPositiveFreeValue
#undef loadedGroupNegativeSize
#undef groupPreviewSizeNegativeQueue
#undef groupPreviewSizeNegativeValue
#undef groupPreviewDataNegativeQueue
#undef groupPreviewDataNegativeValue
#undef groupPreviewDataNegativeFreeQueue
#undef groupPreviewDataNegativeFreeValue
#undef view
}

#undef SET_SCENE_VM_VISIBILITY

} // namespace th095

#endif // TH095_MATCH_EXACT
