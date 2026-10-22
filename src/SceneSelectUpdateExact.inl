#include "SceneSelect.hpp"
#include "AnmVmInterpolation.hpp"

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

extern u16 g_FrontEndCurrentInput;
extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;

struct SceneSelectScoreFlagsView
{
    u32 captured : 1;
    u32 showSuccessRateMarker : 1;
    u32 unknownFlags : 30;
};

struct SceneSelectUpdateView
{
    SceneAnmLoadedView *sceneAnm;
    SceneAnmLoadedView *transitionAnm;
    ZunTimer stateTimer;
    ZunTimer animationTimer;
    ResultScreenReplayCursor groupCursor;
    ResultScreenReplayCursor secondaryCursor;
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
    FrontEndRequestedState requestedState;
    u8 unknown6114[0x0c];
    union
    {
        u32 flags;
        FrontEndControllerFlagBits flagBits;
    };
    u8 unknown6124[4];
    SceneValueQueue selectionQueue;
    SceneValueQueue loadedSceneQueue;
    ScenePreviewDataQueue groupPreviewDataQueue;
    SceneValueQueue groupPreviewSizeQueue;
    ScenePreviewDataQueue scenePreviewDataQueue;
    SceneValueQueue scenePreviewSizeQueue;
    SceneValueQueue groupPreviewQueue;
    SceneValueQueue scenePreviewQueue;
    SceneValueQueue loadedGroupQueue;
    SceneStateHistoryView stateHistory;
    u8 unknown63c0[0x0c];
    FrontEndPendingTextureData pendingTextures;
};


typedef char SceneSelectUpdatePendingTextureDataLayout[
    (sizeof(FrontEndPendingTextureData) == 0x34 &&
     offsetof(SceneSelectUpdateView, pendingTextures) == 0x63cc &&
     offsetof(FrontEndPendingTextureData, primaryData) == 0x04 &&
     offsetof(FrontEndPendingTextureData, primarySize) == 0x10 &&
     offsetof(FrontEndPendingTextureData, secondaryData) == 0x1c &&
     offsetof(FrontEndPendingTextureData, secondarySize) == 0x28) ? 1 : -1];
typedef char SceneSelectUpdateTimersAt8And14[
    (offsetof(SceneSelectUpdateView, stateTimer) == 0x08 &&
     offsetof(SceneSelectUpdateView, animationTimer) == 0x14) ? 1 : -1];
typedef char SceneSelectUpdateTransitionAnmAt4[
    (offsetof(SceneSelectUpdateView, transitionAnm) == 0x04) ? 1 : -1];
typedef char SceneSelectUpdateGroupCursorAt20[
    (offsetof(SceneSelectUpdateView, groupCursor) == 0x20) ? 1 : -1];
typedef char SceneSelectUpdateSecondaryCursorAtF8[
    (offsetof(SceneSelectUpdateView, secondaryCursor) == 0xf8) ? 1 : -1];
typedef char SceneSelectUpdateSceneCursorsAt1D0[
    (offsetof(SceneSelectUpdateView, sceneCursors) == 0x1d0) ? 1 : -1];
typedef char SceneSelectUpdateTransitionVmAt6100[
    (offsetof(SceneSelectUpdateView, transitionVm) == 0x6100) ? 1 : -1];
typedef char SceneSelectUpdateRequestedStateAt6110[
    (offsetof(SceneSelectUpdateView, requestedState) == 0x6110) ? 1 : -1];
typedef char SceneSelectUpdateFlagsAt6120[
    (offsetof(SceneSelectUpdateView, flags) == 0x6120) ? 1 : -1];
typedef char SceneSelectUpdateSelectionQueueAt6128[
    (offsetof(SceneSelectUpdateView, selectionQueue) == 0x6128) ? 1 : -1];
typedef char SceneSelectUpdateLoadedSceneQueueAt6170[
    (offsetof(SceneSelectUpdateView, loadedSceneQueue) == 0x6170) ? 1 : -1];
typedef char SceneSelectUpdateGroupPreviewQueueAt62D8[
    (offsetof(SceneSelectUpdateView, groupPreviewQueue) == 0x62d8) ? 1 : -1];
typedef char SceneSelectUpdateLoadedGroupQueueAt6368[
    (offsetof(SceneSelectUpdateView, loadedGroupQueue) == 0x6368) ? 1 : -1];
typedef char SceneSelectUpdatePendingTextureCountAt63CC[
    (offsetof(SceneSelectUpdateView, pendingTextures) == 0x63cc) ? 1 : -1];
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
    return queue->words[SCENE_QUEUE_COUNT_WORD].value;
}

static __forceinline i32 SceneQueueFront(const SceneValueQueue *queue)
{
    if (queue->words[SCENE_QUEUE_COUNT_WORD].value > 0)
    {
        return queue->words[0].value;
    }
    return 0;
}

template <typename Queue>
static __forceinline void SceneSelectQueuePopPhase(Queue *queue)
{
    u32 compilerStorage;
    queue->Pop();
}


#define SET_SCENE_VM_VISIBILITY(view, vmIndex, condition)                    \
    if (!(condition))                                                         \
    {                                                                         \
        g_SceneAnmManager->GetVm((view)->vmIds[vmIndex])->flagsWord &= ~2u;   \
    }                                                                         \
    else                                                                      \
    {                                                                         \
        g_SceneAnmManager->GetVm((view)->vmIds[vmIndex])->flagsWord |= 2;     \
    }

// The target resets the shared front-end timer in this store order.  The
// canonical ZunTimer::Reset initializes the same state in a different order.
static __forceinline void SceneSelectResetTimer(ZunTimer *timer)
{
    timer->current = 0;
    timer->subFrame = 0.0f;
    timer->previous = -999999;
}

static __forceinline void SceneSelectInitialTimerViewPhase(SceneSelectUpdateView *view)
{
    u8 compilerStorage[4];
    SceneSelectResetTimer(&view->stateTimer);
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
                g_SceneSaveData
                        ->sceneScores[
                            g_SceneGroups[view->groupCursor.GetCurrent()]
                                         [initialSceneIndex]
                                             .scoreEntryIndex]
                        .score != 0);
            if (g_SceneSaveData->IsSceneGroupUnlocked(
                    view->groupCursor.GetCurrent()) == 0)
            {
                g_SceneAnmManager
                    ->GetVm(view->vmIds[initialSceneIndex * 3 + 0x25])
                    ->flagsWord &= ~2u;
                g_SceneAnmManager
                    ->GetVm(view->vmIds[initialSceneIndex * 3 + 0x26])
                    ->flagsWord &= ~2u;
                g_SceneAnmManager
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
                g_SceneSaveData
                        ->sceneScores[
                            g_SceneGroups[view->groupCursor.GetCurrent()]
                                         [newGroupIndex]
                                             .scoreEntryIndex]
                        .score != 0);
            if (g_SceneSaveData->IsSceneGroupUnlocked(
                    view->groupCursor.GetCurrent()) == 0)
            {
                g_SceneAnmManager
                    ->GetVm(view->vmIds[newGroupIndex * 3 + 0x25])
                    ->flagsWord &= ~2u;
                g_SceneAnmManager
                    ->GetVm(view->vmIds[newGroupIndex * 3 + 0x26])
                    ->flagsWord &= ~2u;
                g_SceneAnmManager
                    ->GetVm(view->vmIds[newGroupIndex * 3 + 0x27])
                    ->flagsWord &= ~2u;
            }
        }

static __forceinline void SceneSelectCreateVmAt(SceneSelectUpdateView *view, i32 vmIndex)
{
    view->vmIds[vmIndex] = view->sceneAnm->CreateVm(vmIndex, 7);
}

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
    loadedSceneSize0 = view->loadedSceneQueue.words[SCENE_QUEUE_COUNT_WORD].value;
    if (loadedSceneSize0 != 0)
    {
        loadedSceneSize1 = view->loadedSceneQueue.words[SCENE_QUEUE_COUNT_WORD].value;
        if (loadedSceneSize1 == 1)
        {
            loadedSceneConditionQueue = &view->loadedSceneQueue;
            if (loadedSceneConditionQueue->words[SCENE_QUEUE_COUNT_WORD].value > 0)
                loadedSceneConditionValue = loadedSceneConditionQueue->words[0].value;
            else
                loadedSceneConditionValue = 0;
            if (loadedSceneConditionValue >= 0)
            {
                loadedSceneLoadQueue = &view->loadedSceneQueue;
                if (loadedSceneLoadQueue->words[SCENE_QUEUE_COUNT_WORD].value > 0)
                    loadedSceneLoadValue = loadedSceneLoadQueue->words[0].value;
                else
                    loadedSceneLoadValue = 0;
                g_SceneSaveData->LoadScenePreviewTexture(
                    view->sceneAnm, 1, loadedSceneLoadValue);
                view->vmIds.SetInterrupt(0x82, 1);
                SceneSelectCreateVmAt(view, 0x82);
                reinterpret_cast<AnmTextManagerView *>(g_SceneAnmManager)
                    ->DrawTextCentered(
                        reinterpret_cast<AnmTextVmView *>(
                            g_SceneAnmManager->GetVm(view->vmIds.values[0x82])),
                        0x00efcfcf, 0,
                        g_SceneSaveData->bestShotRecords[
                            (&view->loadedSceneQueue)->Front()].comment);
            }
        }
        g_SceneSupervisor.EnterCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]++;
        view->loadedSceneQueue.Pop();
        g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]--;
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
    if (view->pendingTextures.count != 0)
    {
        if (view->pendingTextures.count == 1)
        {
            g_SceneAnmManager->LoadTexture(
                &view->sceneAnm->textures[3],
                view->pendingTextures.primaryData[0],
                view->pendingTextures.primarySize[0],
                1, 0, 1);
            view->sceneAnm->textures[3].texture->PreLoad();
            g_SceneAnmManager->LoadTexture(
                &view->sceneAnm->textures[4],
                view->pendingTextures.secondaryData[0],
                view->pendingTextures.secondarySize[0],
                1, 0, 1);
            view->sceneAnm->textures[4].texture->PreLoad();
        }
        pendingPrimaryFree = view->pendingTextures.primaryData[0];
        free(pendingPrimaryFree);
        view->pendingTextures.primaryData[0] = 0;
        pendingSecondaryFree = view->pendingTextures.secondaryData[0];
        free(pendingSecondaryFree);
        view->pendingTextures.secondaryData[0] = 0;

        g_SceneSupervisor.EnterCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]++;
#include "FrontEndPendingTextureAdvance.inl"
        g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]--;

        if (view->pendingTextures.count == 0)
        {
            view->vmIds.SetInterrupt(0x15, 4);
            view->vmIds.SetInterrupt(0x16, 4);
        }
    }
    else
    {
        u8 *groupPreviewDataNegativeFreeValue;
            ScenePreviewDataQueue *groupPreviewDataNegativeFreeQueue;
            u8 *groupPreviewDataNegativeValue;
            ScenePreviewDataQueue *groupPreviewDataNegativeQueue;
            i32 groupPreviewSizeNegativeValue;
            SceneValueQueue *groupPreviewSizeNegativeQueue;
            i32 loadedGroupNegativeSize;
            u8 *scenePreviewDataPositiveFreeValue;
            ScenePreviewDataQueue *scenePreviewDataPositiveFreeQueue;
            u8 *groupPreviewDataPositiveFreeValue;
            ScenePreviewDataQueue *groupPreviewDataPositiveFreeQueue;
            u8 *scenePreviewDataPositiveValue;
            ScenePreviewDataQueue *scenePreviewDataPositiveQueue;
            i32 scenePreviewSizePositiveValue;
            SceneValueQueue *scenePreviewSizePositiveQueue;
            u8 *groupPreviewDataPositiveValue;
            ScenePreviewDataQueue *groupPreviewDataPositiveQueue;
            i32 groupPreviewSizePositiveValue;
            SceneValueQueue *groupPreviewSizePositiveQueue;
            i32 loadedGroupSize1;
            i32 loadedGroupFrontValue;
            SceneValueQueue *loadedGroupFrontQueue;
            i32 loadedGroupSize0;
        loadedGroupSize0 = view->loadedGroupQueue.words[SCENE_QUEUE_COUNT_WORD].value;
        if (loadedGroupSize0 != 0)
        {
            loadedGroupFrontQueue = &view->loadedGroupQueue;
            if (loadedGroupFrontQueue->words[SCENE_QUEUE_COUNT_WORD].value > 0)
            {
                loadedGroupFrontValue =
                    loadedGroupFrontQueue->words[0].value;
            }
            else
            {
                loadedGroupFrontValue = 0;
            }
            if (loadedGroupFrontValue >= 0)
            {
                loadedGroupSize1 =
                    view->loadedGroupQueue.words[SCENE_QUEUE_COUNT_WORD].value;
                if (loadedGroupSize1 == 1)
                {
                    groupPreviewSizePositiveQueue =
                        &view->groupPreviewSizeQueue;
                    if (groupPreviewSizePositiveQueue->words[SCENE_QUEUE_COUNT_WORD].value > 0)
                    {
                        groupPreviewSizePositiveValue =
                            groupPreviewSizePositiveQueue
                                ->words[0].value;
                    }
                    else
                    {
                        groupPreviewSizePositiveValue = 0;
                    }
                    groupPreviewDataPositiveQueue =
                        &view->groupPreviewDataQueue;
                    if (static_cast<i32>(groupPreviewDataPositiveQueue->words[SCENE_QUEUE_COUNT_WORD].bits) > 0)
                    {
                        groupPreviewDataPositiveValue =
                            reinterpret_cast<u8 *>(groupPreviewDataPositiveQueue->words[0].bits);
                    }
                    else
                    {
                        groupPreviewDataPositiveValue = 0;
                    }
                    g_SceneAnmManager->LoadTexture(
                        &view->sceneAnm->textures[2],
                        groupPreviewDataPositiveValue,
                        groupPreviewSizePositiveValue,
                        5, 0, 1);

                    scenePreviewSizePositiveQueue =
                        &view->scenePreviewSizeQueue;
                    if (scenePreviewSizePositiveQueue->words[SCENE_QUEUE_COUNT_WORD].value > 0)
                    {
                        scenePreviewSizePositiveValue =
                            scenePreviewSizePositiveQueue
                                ->words[0].value;
                    }
                    else
                    {
                        scenePreviewSizePositiveValue = 0;
                    }
                    scenePreviewDataPositiveQueue =
                        &view->scenePreviewDataQueue;
                    if (static_cast<i32>(scenePreviewDataPositiveQueue->words[SCENE_QUEUE_COUNT_WORD].bits) > 0)
                    {
                        scenePreviewDataPositiveValue =
                            reinterpret_cast<u8 *>(scenePreviewDataPositiveQueue->words[0].bits);
                    }
                    else
                    {
                        scenePreviewDataPositiveValue = 0;
                    }
                    g_SceneAnmManager->LoadTextureRegion(
                        &view->sceneAnm->textures[2],
                        scenePreviewDataPositiveValue,
                        scenePreviewSizePositiveValue,
                        5, 0, 1, 0x20);
                    view->sceneAnm->textures[2].texture->PreLoad();
                }

                groupPreviewDataPositiveFreeQueue =
                    &view->groupPreviewDataQueue;
                if (static_cast<i32>(groupPreviewDataPositiveFreeQueue->words[SCENE_QUEUE_COUNT_WORD].bits) > 0)
                {
                    groupPreviewDataPositiveFreeValue =
                        reinterpret_cast<u8 *>(groupPreviewDataPositiveFreeQueue->words[0].bits);
                }
                else
                {
                    groupPreviewDataPositiveFreeValue = 0;
                }
                FreeSceneOwned(groupPreviewDataPositiveFreeValue);

                scenePreviewDataPositiveFreeQueue =
                    &view->scenePreviewDataQueue;
                if (static_cast<i32>(scenePreviewDataPositiveFreeQueue->words[SCENE_QUEUE_COUNT_WORD].bits) > 0)
                {
                    scenePreviewDataPositiveFreeValue =
                        reinterpret_cast<u8 *>(scenePreviewDataPositiveFreeQueue->words[0].bits);
                }
                else
                {
                    scenePreviewDataPositiveFreeValue = 0;
                }
                FreeSceneOwned(scenePreviewDataPositiveFreeValue);
            }
            else
            {
                loadedGroupNegativeSize =
                    view->loadedGroupQueue.words[SCENE_QUEUE_COUNT_WORD].value;
                if (loadedGroupNegativeSize == 1)
                {
                    groupPreviewSizeNegativeQueue =
                        &view->groupPreviewSizeQueue;
                    if (groupPreviewSizeNegativeQueue->words[SCENE_QUEUE_COUNT_WORD].value > 0)
                    {
                        groupPreviewSizeNegativeValue =
                            groupPreviewSizeNegativeQueue
                                ->words[0].value;
                    }
                    else
                    {
                        groupPreviewSizeNegativeValue = 0;
                    }
                    groupPreviewDataNegativeQueue =
                        &view->groupPreviewDataQueue;
                    if (static_cast<i32>(groupPreviewDataNegativeQueue->words[SCENE_QUEUE_COUNT_WORD].bits) > 0)
                    {
                        groupPreviewDataNegativeValue =
                            reinterpret_cast<u8 *>(groupPreviewDataNegativeQueue->words[0].bits);
                    }
                    else
                    {
                        groupPreviewDataNegativeValue = 0;
                    }
                    g_SceneAnmManager->LoadTexture(
                        &view->sceneAnm->textures[2],
                        groupPreviewDataNegativeValue,
                        groupPreviewSizeNegativeValue,
                        5, 0, 1);
                    view->sceneAnm->textures[2].texture->PreLoad();
                }

                groupPreviewDataNegativeFreeQueue =
                    &view->groupPreviewDataQueue;
                if (static_cast<i32>(groupPreviewDataNegativeFreeQueue->words[SCENE_QUEUE_COUNT_WORD].bits) > 0)
                {
                    groupPreviewDataNegativeFreeValue =
                        reinterpret_cast<u8 *>(groupPreviewDataNegativeFreeQueue->words[0].bits);
                }
                else
                {
                    groupPreviewDataNegativeFreeValue = 0;
                }
                FreeSceneOwned(groupPreviewDataNegativeFreeValue);
            }

            g_SceneSupervisor.EnterCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]++;
            view->loadedGroupQueue.Pop();
            SceneSelectQueuePopPhase(&view->groupPreviewDataQueue);
            SceneSelectQueuePopPhase(&view->scenePreviewDataQueue);
            SceneSelectQueuePopPhase(&view->groupPreviewSizeQueue);
            SceneSelectQueuePopPhase(&view->scenePreviewSizeQueue);
            g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]--;
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
        g_SceneSupervisor.StopReplayScan();
        g_SceneUiAnm->textures[0].Clear();
        SceneSelectInitialTimerViewPhase(view);
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
            view->groupCursor.SetCount(
                12 <= g_SceneSaveData->FindHighestUnlockedSceneGroup() + 2
                    ? 12 : g_SceneSaveData->FindHighestUnlockedSceneGroup() + 2);
        }
        view->groupCursor.wraps = 1;
        view->state = 1;
        view->flags |= 0x10;
        view->selectedScoreEntryIndex = 0;

        for (initialCursorIndex = 0; initialCursorIndex < 12;
             initialCursorIndex++)
        {
            view->sceneCursors[initialCursorIndex].SetCount(
                g_SceneGroupCounts[initialCursorIndex]);
            view->sceneCursors[initialCursorIndex].wraps = 1;
            view->sceneCursors[initialCursorIndex].Set(0);
        }
        view->groupCursor.Set(g_SceneSaveData->lastSelectedGroup);
        view->sceneCursors[view->groupCursor.GetCurrent()].Set(
            g_SceneSaveData->lastSelectedScene);

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
                g_SceneAnmManager->GetVm(view->vmIds[0x22]),
                view->groupCursor.GetCurrent() + 0x37);
        }
        else
        {
            SceneSelectCreateVmAt(view, 0x21);
            view->sceneAnm->SetSprite(
                g_SceneAnmManager->GetVm(view->vmIds[0x22]),
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

        view->groupPreviewQueue.words[SCENE_QUEUE_CAPACITY_WORD].value = 5;
        view->scenePreviewQueue.words[SCENE_QUEUE_CAPACITY_WORD].value = 5;
        view->loadedGroupQueue.words[SCENE_QUEUE_CAPACITY_WORD].value = 16;
        view->groupPreviewDataQueue.words[SCENE_QUEUE_CAPACITY_WORD].bits = 16;
        view->groupPreviewSizeQueue.words[SCENE_QUEUE_CAPACITY_WORD].value = 16;
        view->scenePreviewDataQueue.words[SCENE_QUEUE_CAPACITY_WORD].bits = 16;
        view->scenePreviewSizeQueue.words[SCENE_QUEUE_CAPACITY_WORD].value = 16;
        view->loadedGroupQueue.words[SCENE_QUEUE_CAPACITY_WORD].value = 16;
        view->selectionQueue.words[SCENE_QUEUE_CAPACITY_WORD].value = 5;
        view->loadedSceneQueue.words[SCENE_QUEUE_CAPACITY_WORD].value = 16;
        view->flagBits.assetLoadStopRequested = 0;

        g_SceneSupervisor.StartReplayScan(LoadSceneSelectionAssets, NULL);
        SceneSelectCreateVmAt(view, 0x13);
        SceneSelectCreateVmAt(view, 0x12);
        view->flagBits.previewPending = 1;
        SceneSelectCreateVmAt(view, 0x14);
        SceneSelectCreateVmAt(view, 0x49);
        SceneSelectCreateVmAt(view, 0x4a);
        SceneSelectCreateVmAt(view, 0x4b);

        view->sceneAnm->SetSprite(
            g_SceneAnmManager->GetVm(view->vmIds[0x49]),
            (view->groupCursor.GetCurrent() >= 11
                 ? view->groupCursor.GetCurrent() - 11
                 : view->groupCursor.GetCurrent()) +
                0x28);
        view->sceneAnm->SetSprite(
            g_SceneAnmManager->GetVm(view->vmIds[0x4b]),
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
        if (g_SceneSaveData->IsSceneGroupUnlocked(
                view->groupCursor.GetCurrent()) != 0)
        {
            reinterpret_cast<AnmVmColorInterpolationView *>(
                initialGroupVm)
                ->SetColor1Interpolation(
                    60, 0, initialGroupVm->color1,
                    g_SceneGroupColors[view->groupCursor.GetCurrent()]);
        }
        else
        {
            reinterpret_cast<AnmVmColorInterpolationView *>(
                initialGroupVm)
                ->SetColor1Interpolation(
                    60, 0, initialGroupVm->color1,
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
        g_SceneAnmManager->GetVm(view->vmIds[0x15])->flagsWord &= ~2u;
        g_SceneAnmManager->GetVm(view->vmIds[0x16])->flagsWord &= ~2u;
        g_SceneAnmManager->GetVm(view->vmIds[0x45])->flagsWord &= ~2u;
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
        g_SceneAnmManager->GetVm(view->vmIds[0x15])->flagsWord |= 2;
        g_SceneAnmManager->GetVm(view->vmIds[0x16])->flagsWord |= 2;
        g_SceneAnmManager->GetVm(view->vmIds[0x45])->flagsWord |= 2;
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
            &g_SceneSaveData
                 ->sceneScores[view->selectedScoreEntryIndex]) != 0)
    {
        SceneSelectScoreFlagsAt(
            g_SceneSaveData, view->selectedScoreEntryIndex)
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
        g_SceneAnmManager->MarkVmForDeletion(view->vmIds[0x20]);
        g_SceneAnmManager->MarkVmForDeletion(view->vmIds[0x21]);
        g_SceneAnmManager->MarkVmForDeletion(view->vmIds[0x22]);
        SceneSelectCreateVmAt(view, 0x22);
        if (view->groupCursor.GetCurrent() <= 10)
        {
            SceneSelectCreateVmAt(view, 0x20);
            view->sceneAnm->SetSprite(
                g_SceneAnmManager->GetVm(view->vmIds[0x22]),
                view->groupCursor.GetCurrent() + 0x37);
        }
        else
        {
            SceneSelectCreateVmAt(view, 0x21);
            view->sceneAnm->SetSprite(
                g_SceneAnmManager->GetVm(view->vmIds[0x22]),
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
            g_SceneAnmManager->GetVm(view->vmIds[0x49]),
            (view->groupCursor.GetCurrent() >= 11
                 ? view->groupCursor.GetCurrent() - 11
                 : view->groupCursor.GetCurrent()) +
                0x28);
        view->sceneAnm->SetSprite(
            g_SceneAnmManager->GetVm(view->vmIds[0x4b]),
            activeSceneCursor.GetCurrent() + 0x28);
        view->vmIds.SetInterrupt(0x49, 2);
        view->vmIds.SetInterrupt(0x4a, 2);
        view->vmIds.SetInterrupt(0x4b, 2);
        this->UpdateSelectedSceneDetails();
        g_SceneSaveData->lastSelectedGroup =
            (i16)view->groupCursor.GetCurrent();
        g_SceneSaveData->lastSelectedScene =
            (i16)activeSceneCursor.GetCurrent();

        SET_SCENE_VM_VISIBILITY(
            view, 0x23, view->groupCursor.GetCurrent() != 0);
        SET_SCENE_VM_VISIBILITY(
            view, 0x24,
            view->groupCursor.GetCurrent() != view->groupCursor.GetCount() - 1);

        transitionGroupVm = view->vmIds.values[0x68].GetVm();
        if (g_SceneSaveData->IsSceneGroupUnlocked(
                view->groupCursor.GetCurrent()) != 0)
        {
            reinterpret_cast<AnmVmColorInterpolationView *>(
                transitionGroupVm)
                ->SetColor1Interpolation(
                    60, 0, transitionGroupVm->color1,
                    g_SceneGroupColors[view->groupCursor.GetCurrent()]);
        }
        else
        {
            reinterpret_cast<AnmVmColorInterpolationView *>(
                transitionGroupVm)
                ->SetColor1Interpolation(
                    60, 0, transitionGroupVm->color1,
                    g_SceneLockedTransitionColor);
        }    }
    else
    {
        if (g_SceneSaveData->IsSceneGroupUnlocked(
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
                    g_SceneAnmManager->GetVm(view->vmIds[0x49]),
                    (view->groupCursor.GetCurrent() >= 11
                         ? view->groupCursor.GetCurrent() - 11
                         : view->groupCursor.GetCurrent()) +
                        0x28);
                view->sceneAnm->SetSprite(
                    g_SceneAnmManager->GetVm(view->vmIds[0x4b]),
                    activeSceneCursor.GetCurrent() + 0x28);
                view->vmIds.SetInterrupt(0x49, 2);
                view->vmIds.SetInterrupt(0x4a, 2);
                view->vmIds.SetInterrupt(0x4b, 2);
                this->UpdateSelectedSceneDetails();
                g_SceneSaveData->lastSelectedScene =
                    (i16)activeSceneCursor.GetCurrent();
                goto update_preview_text;
            }

            if (SceneInputAnd(g_PressedButtons, (u16)0x1002) != 0)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                g_SelectedScene =
                    &g_SceneGroups[view->groupCursor.GetCurrent()]
                                  [activeSceneCursor.GetCurrent()];
                view->requestedState = FRONT_END_REQUESTED_STATE_START_GAME;
                view->state = 0;
                SceneSelectResetTimer(&view->stateTimer);
                view->flagBits.assetLoadStopRequested = 1;
                g_SceneSupervisor.replayScanWorker.RequestExit();
                g_SceneAnmManager->MarkVmForDeletion(
                    view->previewTextVmIds[0]);
                g_SceneAnmManager->MarkVmForDeletion(
                    view->previewTextVmIds[1]);
                g_SceneAnmManager->MarkVmForDeletion(
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
            view->flagBits.assetLoadStopRequested = 1;
            g_SelectedScene =
                &g_SceneGroups[view->groupCursor.GetCurrent()]
                              [view->sceneCursors[view->groupCursor.GetCurrent()]
                                   .GetCurrent()];
            view->groupCursor.Pop();
            view->requestedState = FRONT_END_REQUESTED_STATE_MAIN_MENU;
            view->state = 0;
            SceneSelectResetTimer(&view->stateTimer);
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
            g_SceneAnmManager->SetInterrupt(view->previewTextVmIds[0], 1);
            g_SceneAnmManager->SetInterrupt(view->previewTextVmIds[1], 1);
            g_SceneAnmManager->SetInterrupt(view->previewTextVmIds[2], 1);
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
            view->flagBits.previewPending = 1;
        }
    }
    else
    {
        if (view->flagBits.previewPending != 0)
        {
            previewVm =
                g_SceneAnmManager->GetVm(view->vmIds.values[0x12]);
            if (g_SceneSaveData->bestShotRecords[
                    view->selectedScoreEntryIndex].componentsLoaded != 0)
            {
                previewVm->loadedSprite->uvEndX =
                    (f32)g_SceneSaveData->bestShotRecords[
                        view->selectedScoreEntryIndex].width /
                    256.0f;
                previewVm->loadedSprite->uvEndY =
                    (f32)g_SceneSaveData->bestShotRecords[
                        view->selectedScoreEntryIndex].height /
                    256.0f;
                previewVm->spriteWidth =
                    (f32)g_SceneSaveData->bestShotRecords[
                        view->selectedScoreEntryIndex].width;
                previewVm->spriteHeight =
                    (f32)g_SceneSaveData->bestShotRecords[
                        view->selectedScoreEntryIndex].height;
                previewVm->flagsWord |= 2;
            }
            else
            {
                previewVm->flagsWord &= ~2u;
            }
            g_SceneAnmManager->SetInterrupt(view->vmIds.values[0x12], 2);
            view->flagBits.previewPending = 0;
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
