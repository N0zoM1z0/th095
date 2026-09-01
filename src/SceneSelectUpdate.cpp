#include "SceneSelect.hpp"

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

struct SceneSelectColorInterpolationView
{
    u8 unknown000[0x220];
    u32 color1;
    u8 unknown224[0x74];

    void SetColor1Interpolation(i32 duration, u8 mode, u32 initial,
                                u32 final);
};

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
 * Queue accessors preceding the first VM value-return call occupy the target's
 * EBP-4Ch..EBP-C0h band in evaluation order.  Keeping the pointer and value of
 * every access explicit preserves both the bounded empty-queue semantics and
 * VC7.1's target-observed stack ordering.
 */
struct SceneSelectQueueLocals
{
    i32 loadedSceneLoadValue;
    SceneValueQueue *loadedSceneLoadQueue;
    i32 loadedSceneConditionValue;
    SceneValueQueue *loadedSceneConditionQueue;
    i32 loadedSceneSize1;
    i32 loadedSceneSize0;
    i32 loadedGroupDrainedSize;
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
};

static __forceinline void FreeSceneOwned(void *Block)
{
    free(Block);
}

static __forceinline u16 SceneInputAnd(u16 input, u16 mask)
{
    return input & mask;
}

static __forceinline u16 SceneLeftInputMask()
{
    return TH_BUTTON_LEFT;
}

static __forceinline u16 SceneRightInputMask()
{
    return TH_BUTTON_RIGHT;
}

static __forceinline i32 SceneQueueSize(const SceneValueQueue *queue)
{
    return queue->count;
}

static __forceinline i32 SceneQueueFront(const SceneValueQueue *queue)
{
    if (queue->count <= 0)
    {
        return 0;
    }
    return queue->values[0];
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
#define view (reinterpret_cast<SceneSelectUpdateView *>(this))
#define activeSceneCursor                                                   \
    (view->sceneCursors[view->groupCursor.GetCurrent()])
    SceneSelectShallowLocals shallow;
    SceneSelectQueueLocals queueLocals;
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
            g_SceneAnmManager->LoadTexture(
                &view->sceneAnm->textures[3],
                reinterpret_cast<u8 *>(view->pendingPrimaryData[0]),
                view->pendingPrimarySize[0],
                1, 0, 1);
            view->sceneAnm->textures[3].texture->PreLoad();
            g_SceneAnmManager->LoadTexture(
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
    else
    {
        queueLocals.loadedGroupSize0 = view->loadedGroupQueue.count;
        if (queueLocals.loadedGroupSize0 != 0)
        {
            queueLocals.loadedGroupFrontQueue = &view->loadedGroupQueue;
            if (queueLocals.loadedGroupFrontQueue->count > 0)
            {
                queueLocals.loadedGroupFrontValue =
                    queueLocals.loadedGroupFrontQueue->values[0];
            }
            else
            {
                queueLocals.loadedGroupFrontValue = 0;
            }
            if (queueLocals.loadedGroupFrontValue >= 0)
            {
                queueLocals.loadedGroupSize1 =
                    view->loadedGroupQueue.count;
                if (queueLocals.loadedGroupSize1 == 1)
                {
                    queueLocals.groupPreviewSizePositiveQueue =
                        &view->groupPreviewSizeQueue;
                    if (queueLocals.groupPreviewSizePositiveQueue->count > 0)
                    {
                        queueLocals.groupPreviewSizePositiveValue =
                            queueLocals.groupPreviewSizePositiveQueue
                                ->values[0];
                    }
                    else
                    {
                        queueLocals.groupPreviewSizePositiveValue = 0;
                    }
                    queueLocals.groupPreviewDataPositiveQueue =
                        &view->groupPreviewDataQueue;
                    if (queueLocals.groupPreviewDataPositiveQueue->count > 0)
                    {
                        queueLocals.groupPreviewDataPositiveValue =
                            queueLocals.groupPreviewDataPositiveQueue
                                ->values[0];
                    }
                    else
                    {
                        queueLocals.groupPreviewDataPositiveValue = 0;
                    }
                    g_SceneAnmManager->LoadTexture(
                        &view->sceneAnm->textures[2],
                        reinterpret_cast<u8 *>(
                            queueLocals.groupPreviewDataPositiveValue),
                        queueLocals.groupPreviewSizePositiveValue,
                        5, 0, 1);

                    queueLocals.scenePreviewSizePositiveQueue =
                        &view->scenePreviewSizeQueue;
                    if (queueLocals.scenePreviewSizePositiveQueue->count > 0)
                    {
                        queueLocals.scenePreviewSizePositiveValue =
                            queueLocals.scenePreviewSizePositiveQueue
                                ->values[0];
                    }
                    else
                    {
                        queueLocals.scenePreviewSizePositiveValue = 0;
                    }
                    queueLocals.scenePreviewDataPositiveQueue =
                        &view->scenePreviewDataQueue;
                    if (queueLocals.scenePreviewDataPositiveQueue->count > 0)
                    {
                        queueLocals.scenePreviewDataPositiveValue =
                            queueLocals.scenePreviewDataPositiveQueue
                                ->values[0];
                    }
                    else
                    {
                        queueLocals.scenePreviewDataPositiveValue = 0;
                    }
                    g_SceneAnmManager->LoadTextureRegion(
                        &view->sceneAnm->textures[3],
                        reinterpret_cast<u8 *>(
                            queueLocals.scenePreviewDataPositiveValue),
                        queueLocals.scenePreviewSizePositiveValue,
                        5, 0, 1, 0x20);
                    view->sceneAnm->textures[2].texture->PreLoad();
                }

                queueLocals.groupPreviewDataPositiveFreeQueue =
                    &view->groupPreviewDataQueue;
                if (queueLocals.groupPreviewDataPositiveFreeQueue->count > 0)
                {
                    queueLocals.groupPreviewDataPositiveFreeValue =
                        queueLocals.groupPreviewDataPositiveFreeQueue
                            ->values[0];
                }
                else
                {
                    queueLocals.groupPreviewDataPositiveFreeValue = 0;
                }
                FreeSceneOwned(reinterpret_cast<void *>(
                    queueLocals.groupPreviewDataPositiveFreeValue));

                queueLocals.scenePreviewDataPositiveFreeQueue =
                    &view->scenePreviewDataQueue;
                if (queueLocals.scenePreviewDataPositiveFreeQueue->count > 0)
                {
                    queueLocals.scenePreviewDataPositiveFreeValue =
                        queueLocals.scenePreviewDataPositiveFreeQueue
                            ->values[0];
                }
                else
                {
                    queueLocals.scenePreviewDataPositiveFreeValue = 0;
                }
                FreeSceneOwned(reinterpret_cast<void *>(
                    queueLocals.scenePreviewDataPositiveFreeValue));
            }
            else
            {
                queueLocals.loadedGroupNegativeSize =
                    view->loadedGroupQueue.count;
                if (queueLocals.loadedGroupNegativeSize == 1)
                {
                    queueLocals.groupPreviewSizeNegativeQueue =
                        &view->groupPreviewSizeQueue;
                    if (queueLocals.groupPreviewSizeNegativeQueue->count > 0)
                    {
                        queueLocals.groupPreviewSizeNegativeValue =
                            queueLocals.groupPreviewSizeNegativeQueue
                                ->values[0];
                    }
                    else
                    {
                        queueLocals.groupPreviewSizeNegativeValue = 0;
                    }
                    queueLocals.groupPreviewDataNegativeQueue =
                        &view->groupPreviewDataQueue;
                    if (queueLocals.groupPreviewDataNegativeQueue->count > 0)
                    {
                        queueLocals.groupPreviewDataNegativeValue =
                            queueLocals.groupPreviewDataNegativeQueue
                                ->values[0];
                    }
                    else
                    {
                        queueLocals.groupPreviewDataNegativeValue = 0;
                    }
                    g_SceneAnmManager->LoadTexture(
                        &view->sceneAnm->textures[2],
                        reinterpret_cast<u8 *>(
                            queueLocals.groupPreviewDataNegativeValue),
                        queueLocals.groupPreviewSizeNegativeValue,
                        5, 0, 1);
                    view->sceneAnm->textures[2].texture->PreLoad();
                }

                queueLocals.groupPreviewDataNegativeFreeQueue =
                    &view->groupPreviewDataQueue;
                if (queueLocals.groupPreviewDataNegativeFreeQueue->count > 0)
                {
                    queueLocals.groupPreviewDataNegativeFreeValue =
                        queueLocals.groupPreviewDataNegativeFreeQueue
                            ->values[0];
                }
                else
                {
                    queueLocals.groupPreviewDataNegativeFreeValue = 0;
                }
                FreeSceneOwned(reinterpret_cast<void *>(
                    queueLocals.groupPreviewDataNegativeFreeValue));
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
            queueLocals.loadedGroupDrainedSize =
                view->loadedGroupQueue.count;
            if (queueLocals.loadedGroupDrainedSize == 0)
            {
                view->vmIds.SetInterrupt(0x14, 4);
            }
        }
        else
        {
            queueLocals.loadedSceneSize0 = view->loadedSceneQueue.count;
            if (queueLocals.loadedSceneSize0 != 0)
            {
                queueLocals.loadedSceneSize1 =
                    view->loadedSceneQueue.count;
                if (queueLocals.loadedSceneSize1 == 1)
                {
                    queueLocals.loadedSceneConditionQueue =
                        &view->loadedSceneQueue;
                    if (queueLocals.loadedSceneConditionQueue->count > 0)
                    {
                        queueLocals.loadedSceneConditionValue =
                            queueLocals.loadedSceneConditionQueue->values[0];
                    }
                    else
                    {
                        queueLocals.loadedSceneConditionValue = 0;
                    }
                    if (queueLocals.loadedSceneConditionValue >= 0)
                    {
                        queueLocals.loadedSceneLoadQueue =
                            &view->loadedSceneQueue;
                        if (queueLocals.loadedSceneLoadQueue->count > 0)
                        {
                            queueLocals.loadedSceneLoadValue =
                                queueLocals.loadedSceneLoadQueue->values[0];
                        }
                        else
                        {
                            queueLocals.loadedSceneLoadValue = 0;
                        }
                        g_SceneSaveData->LoadScenePreviewTexture(
                            view->sceneAnm, 1,
                            queueLocals.loadedSceneLoadValue);
                        view->vmIds.SetInterrupt(0x82, 1);
                        view->vmIds[0x82] =
                            view->sceneAnm->CreateVm(0x82, 7);
                        reinterpret_cast<AnmTextManagerView *>(
                            g_SceneAnmManager)
                            ->DrawTextCentered(
                                reinterpret_cast<AnmTextVmView *>(
                                    g_SceneAnmManager->GetVm(
                                        view->vmIds.values[0x82])),
                                0x00efcfcf, 0,
                                reinterpret_cast<char *>(g_SceneSaveData) +
                                    SceneQueueFront(&view->loadedSceneQueue) *
                                        0x78 +
                                    0x3178);
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
    }

    switch (view->state)
    {
    default:
        goto update_preview_text;
    case 0:
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
            view->groupCursor.count =
                12 <= g_SceneSaveData->FindHighestUnlockedSceneGroup() + 2
                    ? 12
                    : g_SceneSaveData->FindHighestUnlockedSceneGroup() + 2;
        }
        view->groupCursor.wraps = 1;
        view->state = 1;
        view->flags |= 0x10;
        view->selectedScoreEntryIndex = 0;

        for (initialCursorIndex = 0; initialCursorIndex < 12;
             initialCursorIndex++)
        {
            int sceneCount = g_SceneGroupCounts[initialCursorIndex];
            view->sceneCursors[initialCursorIndex].count = sceneCount;
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

        view->vmIds[0x68] = view->sceneAnm->CreateVm(0x68, 7);
        view->vmIds[0x69] = view->sceneAnm->CreateVm(0x69, 7);
        view->vmIds.SetInterrupt(0x19, 3);
        view->vmIds.SetInterrupt(0x1a, 3);
        view->transitionVm.SetInterrupt(3);
        view->vmIds.SetInterrupt(0x1b, 3);
        view->vmIds[0x1e] = view->sceneAnm->CreateVm(0x1e, 7);
        view->vmIds[0x22] = view->sceneAnm->CreateVm(0x22, 7);
        if (view->groupCursor.GetCurrent() < 11)
        {
            view->vmIds[0x20] = view->sceneAnm->CreateVm(0x20, 7);
            view->sceneAnm->SetSprite(
                g_SceneAnmManager->GetVm(view->vmIds[0x22]),
                view->groupCursor.GetCurrent() + 0x37);
        }
        else
        {
            view->vmIds[0x21] = view->sceneAnm->CreateVm(0x21, 7);
            view->sceneAnm->SetSprite(
                g_SceneAnmManager->GetVm(view->vmIds[0x22]),
                view->groupCursor.GetCurrent() + 0x2c);
        }
        view->vmIds[0x23] = view->sceneAnm->CreateVm(0x23, 7);
        view->vmIds[0x24] = view->sceneAnm->CreateVm(0x24, 7);
        SET_SCENE_VM_VISIBILITY(
            view, 0x23, view->groupCursor.GetCurrent() != 0);
        SET_SCENE_VM_VISIBILITY(
            view, 0x24,
            view->groupCursor.GetCurrent() != view->groupCursor.GetCount() - 1);

        view->vmIds[0x44] = view->sceneAnm->CreateVm(0x44, 7);
        view->vmIds[0x48] = view->sceneAnm->CreateVm(0x48, 7);
        view->vmIds[0x15] = view->sceneAnm->CreateVm(0x15, 7);
        view->vmIds[0x16] = view->sceneAnm->CreateVm(0x16, 7);
        view->vmIds[0x45] = view->sceneAnm->CreateVm(0x45, 7);
        view->vmIds[0x46] = view->sceneAnm->CreateVm(0x46, 7);

        for (initialSceneIndex = 0;
             initialSceneIndex <
                 g_SceneGroupCounts[view->groupCursor.GetCurrent()];
             initialSceneIndex++)
        {
            int firstSceneVmIndex = initialSceneIndex * 3 + 0x25;
            view->vmIds[firstSceneVmIndex] =
                view->sceneAnm->CreateVm(firstSceneVmIndex, 7);
            int secondSceneVmIndex = initialSceneIndex * 3 + 0x26;
            view->vmIds[secondSceneVmIndex] =
                view->sceneAnm->CreateVm(secondSceneVmIndex, 7);
            int thirdSceneVmIndex = initialSceneIndex * 3 + 0x27;
            view->vmIds[thirdSceneVmIndex] =
                view->sceneAnm->CreateVm(thirdSceneVmIndex, 7);

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

        g_SceneSupervisor.StartReplayScan(LoadSceneSelectionAssets, NULL);
        view->vmIds[0x13] = view->sceneAnm->CreateVm(0x13, 7);
        view->vmIds[0x12] = view->sceneAnm->CreateVm(0x12, 7);
        view->flags |= 4;
        view->vmIds[0x14] = view->sceneAnm->CreateVm(0x14, 7);
        view->vmIds[0x49] = view->sceneAnm->CreateVm(0x49, 7);
        view->vmIds[0x4a] = view->sceneAnm->CreateVm(0x4a, 7);
        view->vmIds[0x4b] = view->sceneAnm->CreateVm(0x4b, 7);

        view->sceneAnm->SetSprite(
            g_SceneAnmManager->GetVm(view->vmIds[0x49]),
            (view->groupCursor.GetCurrent() < 11
                 ? view->groupCursor.GetCurrent()
                 : view->groupCursor.GetCurrent() - 11) +
                0x28);
        view->sceneAnm->SetSprite(
            g_SceneAnmManager->GetVm(view->vmIds[0x4b]),
            view->sceneCursors[view->groupCursor.GetCurrent()].GetCurrent() + 0x28);

        for (initialVmIndex = 0; initialVmIndex < 16; initialVmIndex++)
        {
            int vmIndex = 0x4c + initialVmIndex;
            view->vmIds[vmIndex] = view->sceneAnm->CreateVm(vmIndex, 7);
        }
        view->vmIds[0x5c] = view->sceneAnm->CreateVm(0x5c, 7);
        view->vmIds[0x5d] = view->sceneAnm->CreateVm(0x5d, 7);
        view->vmIds[0x5e] = view->sceneAnm->CreateVm(0x5e, 7);
        view->vmIds[0x5f] = view->sceneAnm->CreateVm(0x5f, 7);
        view->vmIds[0x60] = view->sceneAnm->CreateVm(0x60, 7);
        view->vmIds[0x61] = view->sceneAnm->CreateVm(0x61, 7);
        view->vmIds[0x62] = view->sceneAnm->CreateVm(0x62, 7);
        view->vmIds[0x63] = view->sceneAnm->CreateVm(0x63, 7);

        initialGroupVm = view->vmIds.values[0x68].GetVm();
        if (g_SceneSaveData->IsSceneGroupUnlocked(
                view->groupCursor.GetCurrent()) != 0)
        {
            reinterpret_cast<SceneSelectColorInterpolationView *>(
                initialGroupVm)
                ->SetColor1Interpolation(
                    60, 0, initialGroupVm->color1,
                    g_SceneGroupColors[view->groupCursor.GetCurrent()]);
        }
        else
        {
            reinterpret_cast<SceneSelectColorInterpolationView *>(
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
                 g_SceneGroupCounts[view->groupCursor.GetCurrent()];
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
        SceneSelectScoreFlagsView *scoreFlags =
            reinterpret_cast<SceneSelectScoreFlagsView *>(
                &g_SceneSaveData
                     ->sceneScores[view->selectedScoreEntryIndex]
                     .flags);
        scoreFlags->showSuccessRateMarker ^= 1;
    }

    view->groupCursor.SaveCurrent();
    groupDirection = 0;
    if (SceneInputAnd(g_PressedButtons, TH_BUTTON_UP) != 0)
    {
        view->groupCursor.Move(-1);
        groupDirection = -1;
    }
    if (SceneInputAnd(g_PressedButtons, TH_BUTTON_DOWN) != 0)
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
        view->vmIds[0x22] = view->sceneAnm->CreateVm(0x22, 7);
        if (view->groupCursor.GetCurrent() < 11)
        {
            view->vmIds[0x20] = view->sceneAnm->CreateVm(0x20, 7);
            view->sceneAnm->SetSprite(
                g_SceneAnmManager->GetVm(view->vmIds[0x22]),
                view->groupCursor.GetCurrent() + 0x37);
        }
        else
        {
            view->vmIds[0x21] = view->sceneAnm->CreateVm(0x21, 7);
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
                groupDirection < 1 ? 8 : 7);
            view->vmIds.SetInterrupt(
                previousGroupIndex * 3 + 0x26,
                groupDirection < 1 ? 8 : 7);
            view->vmIds.SetInterrupt(
                previousGroupIndex * 3 + 0x27,
                groupDirection < 1 ? 8 : 7);
        }
        for (newGroupIndex = 0;
             newGroupIndex < g_SceneGroupCounts[view->groupCursor.GetCurrent()];
             newGroupIndex++)
        {
            int firstSceneVmIndex = newGroupIndex * 3 + 0x25;
            view->vmIds[firstSceneVmIndex] =
                view->sceneAnm->CreateVm(firstSceneVmIndex, 7);
            int secondSceneVmIndex = newGroupIndex * 3 + 0x26;
            view->vmIds[secondSceneVmIndex] =
                view->sceneAnm->CreateVm(secondSceneVmIndex, 7);
            int thirdSceneVmIndex = newGroupIndex * 3 + 0x27;
            view->vmIds[thirdSceneVmIndex] =
                view->sceneAnm->CreateVm(thirdSceneVmIndex, 7);
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

        view->vmIds.SetInterrupt(0x12, groupDirection < 1 ? 8 : 7);
        view->vmIds[0x12] = view->sceneAnm->CreateVm(0x12, 7);
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
            reinterpret_cast<SceneSelectColorInterpolationView *>(
                transitionGroupVm)
                ->SetColor1Interpolation(
                    60, 0, transitionGroupVm->color1,
                    g_SceneGroupColors[view->groupCursor.GetCurrent()]);
        }
        else
        {
            reinterpret_cast<SceneSelectColorInterpolationView *>(
                transitionGroupVm)
                ->SetColor1Interpolation(
                    60, 0, transitionGroupVm->color1,
                    g_SceneLockedTransitionColor);
        }    }
    else
    {
        if (g_SceneSaveData->IsSceneGroupUnlocked(
                view->groupCursor.GetCurrent()) != 0)
        {
            activeSceneCursor.SaveCurrent();
            if ((u16)(SceneInputAnd(g_PressedButtons, TH_BUTTON_LEFT) != 0 ||
                      (g_ResultMenuInput & SceneLeftInputMask()) != 0) != 0)
            {
                activeSceneCursor.Move(-1);
            }
            if ((u16)(SceneInputAnd(g_PressedButtons, TH_BUTTON_RIGHT) != 0 ||
                      (g_ResultMenuInput & SceneRightInputMask()) != 0) != 0)
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
                    (view->groupCursor.GetCurrent() < 11
                         ? view->groupCursor.GetCurrent()
                         : view->groupCursor.GetCurrent() - 11) +
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
                SceneAnmVmId clearedVmId = {0};
                view->vmIds.values[0x82] = clearedVmId;
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
            SceneAnmVmId clearedVmId = {0};
            view->vmIds.values[0x82] = clearedVmId;
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
                g_SceneAnmManager->GetVm(view->vmIds.values[0x12]);
            if ((reinterpret_cast<u8 *>(g_SceneSaveData) +
                 view->selectedScoreEntryIndex * 0x78)[0x31c9] != 0)
            {
                previewVm->loadedSprite->uvEndX =
                    (f32)*reinterpret_cast<u16 *>(
                        reinterpret_cast<u8 *>(g_SceneSaveData) +
                        view->selectedScoreEntryIndex * 0x78 + 0x316c) /
                    256.0f;
                previewVm->loadedSprite->uvEndY =
                    (f32)*reinterpret_cast<u16 *>(
                        reinterpret_cast<u8 *>(g_SceneSaveData) +
                        view->selectedScoreEntryIndex * 0x78 + 0x316e) /
                    256.0f;
                previewVm->spriteWidth =
                    (f32)*reinterpret_cast<u16 *>(
                        reinterpret_cast<u8 *>(g_SceneSaveData) +
                        view->selectedScoreEntryIndex * 0x78 + 0x316c);
                previewVm->spriteHeight =
                    (f32)*reinterpret_cast<u16 *>(
                        reinterpret_cast<u8 *>(g_SceneSaveData) +
                        view->selectedScoreEntryIndex * 0x78 + 0x316e);
                previewVm->flagsWord |= 2;
            }
            else
            {
                previewVm->flagsWord &= ~2u;
            }
            g_SceneAnmManager->SetInterrupt(view->vmIds.values[0x12], 2);
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
#undef view
}

#undef SET_SCENE_VM_VISIBILITY

} // namespace th095
