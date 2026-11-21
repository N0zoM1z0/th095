#ifdef TH095_MATCH_EXACT
#include "SceneSelectUpdateEmission.hpp"
#else
#define SCENE_ANM_MANAGER g_AnmManager
#define SCENE_VM_SHOW(vm) ((vm)->drawEnabled = 1)
#define SCENE_VM_HIDE(vm) ((vm)->drawEnabled = 0)
#define SCENE_VM_COLOR(vm) ((vm)->color1.color)
#endif

#include "SceneSelect.hpp"
#include "FrontEndController.hpp"
#include "AnmManager.hpp"
#include "AnmVmInterpolation.hpp"
#include "ControllerInputSlot.hpp"

#include "ResultScreen.hpp"
#include "SoundPlayer.hpp"

#include <stdlib.h>

namespace th095
{

#ifndef DIFFBUILD
// Canonical scene-group palette at 0x004A5860 and locked-state colors at
// 0x004A588C/0x004A5890.
u32 g_SceneGroupColors[11] = {
    0xffffc0c0, 0xffc0d0ff, 0xffbfffff, 0xffffffc0,
    0xffffc0c0, 0xffc0ffd0, 0xffffb0b0, 0xffe0c0ff,
    0xffffc0e0, 0xffffb0ff, 0xff808080};
u32 g_SceneLockedTransitionColor = 0xff8080ff;
u32 g_SceneLockedInitialColor = 0xffffffff;
#endif

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

// The old Scene Select view serves the remaining POD VM handle calls.
#define SCENE_MENU(owner) (reinterpret_cast<SceneSelectControllerView *>(owner))

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
    return ReadSceneQueueWord32(queue->words[SCENE_QUEUE_COUNT_WORD].value);
}

static __forceinline i32 SceneQueueFront(const SceneValueQueue *queue)
{
    if (ReadSceneQueueWord32(queue->words[SCENE_QUEUE_COUNT_WORD].value) > 0)
    {
        AcquireSceneQueuePayload();
        return ReadSceneQueueWord32(queue->words[0].value);
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
        SCENE_VM_HIDE(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(view)->vmIds[vmIndex]));   \
    }                                                                         \
    else                                                                      \
    {                                                                         \
        SCENE_VM_SHOW(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(view)->vmIds[vmIndex]));     \
    }

static __forceinline void SceneSelectInitialTimerViewPhase(FrontEndControllerView *view)
{
    u8 compilerStorage[4];
    FrontEndResetTimer(&view->stateTimer);
}

static __forceinline i32 SceneSelectInitialSelectionGroupPhase(
    FrontEndControllerView *view)
{
    u8 compilerStorage[0x10];
    return view->groupCursor.GetCurrent();
}

static __forceinline i32 SceneSelectPostTransitionGroupPhase(
    FrontEndControllerView *view)
{
    u8 compilerStorage[0x10];
    return view->groupCursor.GetCurrent();
}

static __forceinline void SceneSelectClearPreviewVmPhase(FrontEndControllerView *view)
{
    SceneAnmVmId clearedVmId = {0};
    SCENE_MENU(view)->vmIds.values[0x82] = clearedVmId;
}

static __forceinline void SceneSelectCreateIndexedVm(
    FrontEndControllerView *view, i32 vmIndex)
{
    SCENE_MENU(view)->vmIds[vmIndex] = SCENE_MENU(view)->sceneAnm->CreateVm(vmIndex, 7);
}

static __forceinline void SceneSelectInitialSceneVmPhase(FrontEndControllerView *view, i32 initialSceneIndex)
{
    SceneSelectCreateIndexedVm(view, initialSceneIndex * 3 + 0x25);
    SceneSelectCreateIndexedVm(view, initialSceneIndex * 3 + 0x26);
    SceneSelectCreateIndexedVm(view, initialSceneIndex * 3 + 0x27);

    SET_SCENE_VM_VISIBILITY(
        view, initialSceneIndex * 3 + 0x27,
        g_ResultSaveData
                ->scoreEntries[
                    g_SceneGroups[view->groupCursor.GetCurrent()]
                                 [initialSceneIndex]
                                     .scoreEntryIndex]
                .score != 0);
    if (g_ResultSaveData->IsSceneGroupUnlocked(
            view->groupCursor.GetCurrent()) == 0)
    {
        SCENE_VM_HIDE(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(view)->vmIds[initialSceneIndex * 3 + 0x25]));
        SCENE_VM_HIDE(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(view)->vmIds[initialSceneIndex * 3 + 0x26]));
        SCENE_VM_HIDE(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(view)->vmIds[initialSceneIndex * 3 + 0x27]));
    }
}

static __forceinline void SceneSelectInitialVmCreatePhase(FrontEndControllerView *view, i32 initialVmIndex)
{
    SceneSelectCreateIndexedVm(view, 0x4c + initialVmIndex);
}

static __forceinline void SceneSelectNewGroupSceneVmPhase(FrontEndControllerView *view, i32 newGroupIndex, i32 groupDirection)
{
    SceneSelectCreateIndexedVm(view, newGroupIndex * 3 + 0x25);
    SceneSelectCreateIndexedVm(view, newGroupIndex * 3 + 0x26);
    SceneSelectCreateIndexedVm(view, newGroupIndex * 3 + 0x27);
    SCENE_MENU(view)->vmIds.SetInterrupt(
        newGroupIndex * 3 + 0x25,
        groupDirection > 0 ? 10 : 9);
    SCENE_MENU(view)->vmIds.SetInterrupt(
        newGroupIndex * 3 + 0x26,
        groupDirection > 0 ? 10 : 9);
    SCENE_MENU(view)->vmIds.SetInterrupt(
        newGroupIndex * 3 + 0x27,
        groupDirection > 0 ? 10 : 9);

    SET_SCENE_VM_VISIBILITY(
        view, newGroupIndex * 3 + 0x27,
        g_ResultSaveData
                ->scoreEntries[
                    g_SceneGroups[view->groupCursor.GetCurrent()]
                                 [newGroupIndex]
                                     .scoreEntryIndex]
                .score != 0);
    if (g_ResultSaveData->IsSceneGroupUnlocked(
            view->groupCursor.GetCurrent()) == 0)
    {
        SCENE_VM_HIDE(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(view)->vmIds[newGroupIndex * 3 + 0x25]));
        SCENE_VM_HIDE(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(view)->vmIds[newGroupIndex * 3 + 0x26]));
        SCENE_VM_HIDE(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(view)->vmIds[newGroupIndex * 3 + 0x27]));
    }
}

static __forceinline void SceneSelectCreateVmAt(FrontEndControllerView *view, i32 vmIndex)
{
    SCENE_MENU(view)->vmIds[vmIndex] = SCENE_MENU(view)->sceneAnm->CreateVm(vmIndex, 7);
}

static __forceinline ResultScoreEntryFlagBits *SceneSelectScoreFlagsAt(
    ResultSaveDataView *saveData, i32 scoreIndex)
{
    return &saveData->scoreEntries[scoreIndex].flagBits;
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
    FrontEndControllerView *view)
{
    i32 loadedSceneLoadValue;
    SceneValueQueue *loadedSceneLoadQueue;
    i32 loadedSceneConditionValue;
    SceneValueQueue *loadedSceneConditionQueue;
    i32 loadedSceneSize1;
    i32 loadedSceneSize0;
    loadedSceneSize0 = ReadSceneQueueWord32(view->loadedSceneQueue.words[SCENE_QUEUE_COUNT_WORD].value);
    if (loadedSceneSize0 != 0)
    {
        loadedSceneSize1 = ReadSceneQueueWord32(view->loadedSceneQueue.words[SCENE_QUEUE_COUNT_WORD].value);
        if (loadedSceneSize1 == 1)
        {
            loadedSceneConditionQueue = &view->loadedSceneQueue;
            if (ReadSceneQueueWord32(loadedSceneConditionQueue->words[SCENE_QUEUE_COUNT_WORD].value) > 0)
            {
                AcquireSceneQueuePayload();
                loadedSceneConditionValue = ReadSceneQueueWord32(loadedSceneConditionQueue->words[0].value);
            }
            else
                loadedSceneConditionValue = 0;
            if (loadedSceneConditionValue >= 0)
            {
                loadedSceneLoadQueue = &view->loadedSceneQueue;
                if (ReadSceneQueueWord32(loadedSceneLoadQueue->words[SCENE_QUEUE_COUNT_WORD].value) > 0)
                {
                    AcquireSceneQueuePayload();
                    loadedSceneLoadValue = ReadSceneQueueWord32(loadedSceneLoadQueue->words[0].value);
                }
                else
                    loadedSceneLoadValue = 0;
                // The exact scene update still uses the older ANM method
                // declarations. The preview consumes the actual loaded ANM.
                g_ResultSaveData->LoadScenePreviewTexture(
                    view->sceneAnm,
                    1, loadedSceneLoadValue);
                SCENE_MENU(view)->vmIds.SetInterrupt(0x82, 1);
                SceneSelectCreateVmAt(view, 0x82);
                g_AnmManager->DrawTextCentered(
                    reinterpret_cast<AnmVm *>(
                        SCENE_ANM_MANAGER->GetVm(SCENE_MENU(view)->vmIds.values[0x82])),
                    0x00efcfcf, 0,
                    g_ResultSaveData->bestShotRecords[
                        (&view->loadedSceneQueue)->Front()].comment);
            }
        }
        g_Supervisor.EnterCriticalSectionWrapper(4);
        g_Supervisor.criticalSectionLockCounts[4]++;
        view->loadedSceneQueue.Pop();
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        g_Supervisor.criticalSectionLockCounts[4]--;
        view->UpdateSelectedSceneDetails();
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
ChainCallbackResult FrontEndControllerView::UpdateSceneSelect()
{
#define activeSceneCursor                                                   \
    (this->sceneCursors[this->groupCursor.GetCurrent()])
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
    if (this->pendingTextures.count != 0)
    {
        if (this->pendingTextures.count == 1)
        {
            g_AnmManager->LoadTexture(
                &this->sceneAnm->textures[3],
                this->pendingTextures.primaryData[0],
                this->pendingTextures.primarySize[0],
                1, 0, 1);
            this->sceneAnm->textures[3].texture->PreLoad();
            g_AnmManager->LoadTexture(
                &this->sceneAnm->textures[4],
                this->pendingTextures.secondaryData[0],
                this->pendingTextures.secondarySize[0],
                1, 0, 1);
            this->sceneAnm->textures[4].texture->PreLoad();
        }
        pendingPrimaryFree = this->pendingTextures.primaryData[0];
        free(pendingPrimaryFree);
        this->pendingTextures.primaryData[0] = 0;
        pendingSecondaryFree = this->pendingTextures.secondaryData[0];
        free(pendingSecondaryFree);
        this->pendingTextures.secondaryData[0] = 0;

        g_Supervisor.EnterCriticalSectionWrapper(4);
        g_Supervisor.criticalSectionLockCounts[4]++;
#include "FrontEndPendingTextureAdvance.inl"
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        g_Supervisor.criticalSectionLockCounts[4]--;

        if (this->pendingTextures.count == 0)
        {
            SCENE_MENU(this)->vmIds.SetInterrupt(0x15, 4);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x16, 4);
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
        loadedGroupSize0 = ReadSceneQueueWord32(this->loadedGroupQueue.words[SCENE_QUEUE_COUNT_WORD].value);
        if (loadedGroupSize0 != 0)
        {
            loadedGroupFrontQueue = &this->loadedGroupQueue;
            if (ReadSceneQueueWord32(loadedGroupFrontQueue->words[SCENE_QUEUE_COUNT_WORD].value) > 0)
            {
                AcquireSceneQueuePayload();
                loadedGroupFrontValue =
                    ReadSceneQueueWord32(loadedGroupFrontQueue->words[0].value);
            }
            else
            {
                loadedGroupFrontValue = 0;
            }
            if (loadedGroupFrontValue >= 0)
            {
                loadedGroupSize1 =
                    ReadSceneQueueWord32(this->loadedGroupQueue.words[SCENE_QUEUE_COUNT_WORD].value);
                if (loadedGroupSize1 == 1)
                {
                    groupPreviewSizePositiveQueue =
                        &this->groupPreviewSizeQueue;
                    if (ReadSceneQueueWord32(groupPreviewSizePositiveQueue->words[SCENE_QUEUE_COUNT_WORD].value) > 0)
                    {
                        AcquireSceneQueuePayload();
                        groupPreviewSizePositiveValue =
                            ReadSceneQueueWord32(groupPreviewSizePositiveQueue
                                ->words[0].value);
                    }
                    else
                    {
                        groupPreviewSizePositiveValue = 0;
                    }
                    groupPreviewDataPositiveQueue =
                        &this->groupPreviewDataQueue;
                    if (static_cast<i32>(ReadSceneQueueAddressWord(groupPreviewDataPositiveQueue->words[SCENE_QUEUE_COUNT_WORD].bits)) > 0)
                    {
                        AcquireSceneQueuePayload();
                        groupPreviewDataPositiveValue =
                            reinterpret_cast<u8 *>(ReadSceneQueueAddressWord(groupPreviewDataPositiveQueue->words[0].bits));
                    }
                    else
                    {
                        groupPreviewDataPositiveValue = 0;
                    }
                    g_AnmManager->LoadTexture(
                        &this->sceneAnm->textures[2],
                        groupPreviewDataPositiveValue,
                        groupPreviewSizePositiveValue,
                        5, 0, 1);

                    scenePreviewSizePositiveQueue =
                        &this->scenePreviewSizeQueue;
                    if (ReadSceneQueueWord32(scenePreviewSizePositiveQueue->words[SCENE_QUEUE_COUNT_WORD].value) > 0)
                    {
                        AcquireSceneQueuePayload();
                        scenePreviewSizePositiveValue =
                            ReadSceneQueueWord32(scenePreviewSizePositiveQueue
                                ->words[0].value);
                    }
                    else
                    {
                        scenePreviewSizePositiveValue = 0;
                    }
                    scenePreviewDataPositiveQueue =
                        &this->scenePreviewDataQueue;
                    if (static_cast<i32>(ReadSceneQueueAddressWord(scenePreviewDataPositiveQueue->words[SCENE_QUEUE_COUNT_WORD].bits)) > 0)
                    {
                        AcquireSceneQueuePayload();
                        scenePreviewDataPositiveValue =
                            reinterpret_cast<u8 *>(ReadSceneQueueAddressWord(scenePreviewDataPositiveQueue->words[0].bits));
                    }
                    else
                    {
                        scenePreviewDataPositiveValue = 0;
                    }
                    g_AnmManager->LoadTextureRegion(
                        &this->sceneAnm->textures[2],
                        scenePreviewDataPositiveValue,
                        scenePreviewSizePositiveValue,
                        5, 0, 1, 0x20);
                    this->sceneAnm->textures[2].texture->PreLoad();
                }

                groupPreviewDataPositiveFreeQueue =
                    &this->groupPreviewDataQueue;
                if (static_cast<i32>(ReadSceneQueueAddressWord(groupPreviewDataPositiveFreeQueue->words[SCENE_QUEUE_COUNT_WORD].bits)) > 0)
                {
                    AcquireSceneQueuePayload();
                    groupPreviewDataPositiveFreeValue =
                        reinterpret_cast<u8 *>(ReadSceneQueueAddressWord(groupPreviewDataPositiveFreeQueue->words[0].bits));
                }
                else
                {
                    groupPreviewDataPositiveFreeValue = 0;
                }
                FreeSceneOwned(groupPreviewDataPositiveFreeValue);

                scenePreviewDataPositiveFreeQueue =
                    &this->scenePreviewDataQueue;
                if (static_cast<i32>(ReadSceneQueueAddressWord(scenePreviewDataPositiveFreeQueue->words[SCENE_QUEUE_COUNT_WORD].bits)) > 0)
                {
                    AcquireSceneQueuePayload();
                    scenePreviewDataPositiveFreeValue =
                        reinterpret_cast<u8 *>(ReadSceneQueueAddressWord(scenePreviewDataPositiveFreeQueue->words[0].bits));
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
                    ReadSceneQueueWord32(this->loadedGroupQueue.words[SCENE_QUEUE_COUNT_WORD].value);
                if (loadedGroupNegativeSize == 1)
                {
                    groupPreviewSizeNegativeQueue =
                        &this->groupPreviewSizeQueue;
                    if (ReadSceneQueueWord32(groupPreviewSizeNegativeQueue->words[SCENE_QUEUE_COUNT_WORD].value) > 0)
                    {
                        AcquireSceneQueuePayload();
                        groupPreviewSizeNegativeValue =
                            ReadSceneQueueWord32(groupPreviewSizeNegativeQueue
                                ->words[0].value);
                    }
                    else
                    {
                        groupPreviewSizeNegativeValue = 0;
                    }
                    groupPreviewDataNegativeQueue =
                        &this->groupPreviewDataQueue;
                    if (static_cast<i32>(ReadSceneQueueAddressWord(groupPreviewDataNegativeQueue->words[SCENE_QUEUE_COUNT_WORD].bits)) > 0)
                    {
                        AcquireSceneQueuePayload();
                        groupPreviewDataNegativeValue =
                            reinterpret_cast<u8 *>(ReadSceneQueueAddressWord(groupPreviewDataNegativeQueue->words[0].bits));
                    }
                    else
                    {
                        groupPreviewDataNegativeValue = 0;
                    }
                    g_AnmManager->LoadTexture(
                        &this->sceneAnm->textures[2],
                        groupPreviewDataNegativeValue,
                        groupPreviewSizeNegativeValue,
                        5, 0, 1);
                    this->sceneAnm->textures[2].texture->PreLoad();
                }

                groupPreviewDataNegativeFreeQueue =
                    &this->groupPreviewDataQueue;
                if (static_cast<i32>(ReadSceneQueueAddressWord(groupPreviewDataNegativeFreeQueue->words[SCENE_QUEUE_COUNT_WORD].bits)) > 0)
                {
                    AcquireSceneQueuePayload();
                    groupPreviewDataNegativeFreeValue =
                        reinterpret_cast<u8 *>(ReadSceneQueueAddressWord(groupPreviewDataNegativeFreeQueue->words[0].bits));
                }
                else
                {
                    groupPreviewDataNegativeFreeValue = 0;
                }
                FreeSceneOwned(groupPreviewDataNegativeFreeValue);
            }

            g_Supervisor.EnterCriticalSectionWrapper(4);
            g_Supervisor.criticalSectionLockCounts[4]++;
            this->loadedGroupQueue.Pop();
            SceneSelectQueuePopPhase(&this->groupPreviewDataQueue);
            SceneSelectQueuePopPhase(&this->scenePreviewDataQueue);
            SceneSelectQueuePopPhase(&this->groupPreviewSizeQueue);
            SceneSelectQueuePopPhase(&this->scenePreviewSizeQueue);
            g_Supervisor.LeaveCriticalSectionWrapper(4);
            g_Supervisor.criticalSectionLockCounts[4]--;
            if (SceneSelectDrainedQueueSizePhase(&this->loadedGroupQueue) == 0)
                SCENE_MENU(this)->vmIds.SetInterrupt(0x14, 4);
        }
        else
        {
            SceneSelectProcessLoadedSceneQueue(this);
        }
    }

    switch (this->state)
    {
    default:
        goto update_preview_text;
    case SCENE_SELECT_STATE_INITIALIZE:
    {
        g_Supervisor.StopReplayScan();
        g_Supervisor.textAnm->textures[0].Clear();
        SceneSelectInitialTimerViewPhase(this);
        this->groupCursor.Push();

        if (g_ResultSaveData->FindHighestUnlockedSceneGroup() < 2)
        {
            this->groupCursor.count = 3;
        }
        else if (g_ResultSaveData->FindHighestUnlockedSceneGroup() < 5)
        {
            this->groupCursor.count = 6;
        }
        else
        {
            this->groupCursor.SetCount(
                12 <= g_ResultSaveData->FindHighestUnlockedSceneGroup() + 2
                    ? 12 : g_ResultSaveData->FindHighestUnlockedSceneGroup() + 2);
        }
        this->groupCursor.wraps = 1;
        this->state = SCENE_SELECT_STATE_ACTIVE;
        this->flagBits.sceneDetailsVisible = 1;
        this->selectedScoreEntryIndex = 0;

        for (initialCursorIndex = 0; initialCursorIndex < 12;
             initialCursorIndex++)
        {
            this->sceneCursors[initialCursorIndex].SetCount(
                g_SceneGroupCounts[initialCursorIndex]);
            this->sceneCursors[initialCursorIndex].wraps = 1;
            this->sceneCursors[initialCursorIndex].Set(0);
        }
        this->groupCursor.Set(g_ResultSaveData->profile.lastSelectedGroup);
        this->sceneCursors[this->groupCursor.GetCurrent()].Set(
            g_ResultSaveData->profile.lastSelectedScene);

        g_SelectedScene =
            &g_SceneGroups[this->groupCursor.GetCurrent()]
                          [this->sceneCursors[this->groupCursor.GetCurrent()].GetCurrent()];
        this->selectedScoreEntryIndex = g_SelectedScene->scoreEntryIndex;

        SceneSelectCreateVmAt(this, 0x68);
        SceneSelectCreateVmAt(this, 0x69);
        SCENE_MENU(this)->vmIds.SetInterrupt(0x19, 3);
        SCENE_MENU(this)->vmIds.SetInterrupt(0x1a, 3);
        SCENE_MENU(this)->transitionVm.SetInterrupt(3);
        SCENE_MENU(this)->vmIds.SetInterrupt(0x1b, 3);
        SceneSelectCreateVmAt(this, 0x1e);
        SceneSelectCreateVmAt(this, 0x22);
        if (this->groupCursor.GetCurrent() <= 10)
        {
            SceneSelectCreateVmAt(this, 0x20);
            SCENE_MENU(this)->sceneAnm->SetSprite(
                SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x22]),
                this->groupCursor.GetCurrent() + 0x37);
        }
        else
        {
            SceneSelectCreateVmAt(this, 0x21);
            SCENE_MENU(this)->sceneAnm->SetSprite(
                SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x22]),
                this->groupCursor.GetCurrent() + 0x2c);
        }
        SceneSelectCreateVmAt(this, 0x23);
        SceneSelectCreateVmAt(this, 0x24);
        SET_SCENE_VM_VISIBILITY(
            this, 0x23, this->groupCursor.GetCurrent() != 0);
        SET_SCENE_VM_VISIBILITY(
            this, 0x24,
            this->groupCursor.GetCurrent() != this->groupCursor.GetCount() - 1);

        SceneSelectCreateVmAt(this, 0x44);
        SceneSelectCreateVmAt(this, 0x48);
        SceneSelectCreateVmAt(this, 0x15);
        SceneSelectCreateVmAt(this, 0x16);
        SceneSelectCreateVmAt(this, 0x45);
        SceneSelectCreateVmAt(this, 0x46);

        for (initialSceneIndex = 0;
             initialSceneIndex <
                 g_SceneGroupCounts[this->groupCursor.GetCurrent()];
             initialSceneIndex++)
        {
            SceneSelectInitialSceneVmPhase(this, initialSceneIndex);
        }

        WriteSceneQueueWord32(this->groupPreviewQueue.words[SCENE_QUEUE_CAPACITY_WORD].value, 5);
        WriteSceneQueueWord32(this->scenePreviewQueue.words[SCENE_QUEUE_CAPACITY_WORD].value, 5);
        WriteSceneQueueWord32(this->loadedGroupQueue.words[SCENE_QUEUE_CAPACITY_WORD].value, 16);
        WriteSceneQueueAddressWord(this->groupPreviewDataQueue.words[SCENE_QUEUE_CAPACITY_WORD].bits, 16);
        WriteSceneQueueWord32(this->groupPreviewSizeQueue.words[SCENE_QUEUE_CAPACITY_WORD].value, 16);
        WriteSceneQueueAddressWord(this->scenePreviewDataQueue.words[SCENE_QUEUE_CAPACITY_WORD].bits, 16);
        WriteSceneQueueWord32(this->scenePreviewSizeQueue.words[SCENE_QUEUE_CAPACITY_WORD].value, 16);
        WriteSceneQueueWord32(this->loadedGroupQueue.words[SCENE_QUEUE_CAPACITY_WORD].value, 16);
        WriteSceneQueueWord32(this->selectionQueue.words[SCENE_QUEUE_CAPACITY_WORD].value, 5);
        WriteSceneQueueWord32(this->loadedSceneQueue.words[SCENE_QUEUE_CAPACITY_WORD].value, 16);
        this->flagBits.assetLoadStopRequested = 0;

        g_Supervisor.StartReplayScan(LoadSceneSelectionAssets, NULL);
        SceneSelectCreateVmAt(this, 0x13);
        SceneSelectCreateVmAt(this, 0x12);
        this->flagBits.previewPending = 1;
        SceneSelectCreateVmAt(this, 0x14);
        SceneSelectCreateVmAt(this, 0x49);
        SceneSelectCreateVmAt(this, 0x4a);
        SceneSelectCreateVmAt(this, 0x4b);

        SCENE_MENU(this)->sceneAnm->SetSprite(
            SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x49]),
            (this->groupCursor.GetCurrent() >= 11
                 ? this->groupCursor.GetCurrent() - 11
                 : this->groupCursor.GetCurrent()) +
                0x28);
        SCENE_MENU(this)->sceneAnm->SetSprite(
            SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x4b]),
            this->sceneCursors[this->groupCursor.GetCurrent()].GetCurrent() + 0x28);

        for (initialVmIndex = 0; initialVmIndex < 16; initialVmIndex++)
        {
            SceneSelectInitialVmCreatePhase(this, initialVmIndex);
        }
        SceneSelectCreateVmAt(this, 0x5c);
        SceneSelectCreateVmAt(this, 0x5d);
        SceneSelectCreateVmAt(this, 0x5e);
        SceneSelectCreateVmAt(this, 0x5f);
        SceneSelectCreateVmAt(this, 0x60);
        SceneSelectCreateVmAt(this, 0x61);
        SceneSelectCreateVmAt(this, 0x62);
        SceneSelectCreateVmAt(this, 0x63);

        initialGroupVm = SCENE_MENU(this)->vmIds.values[0x68].GetVm();
        if (g_ResultSaveData->IsSceneGroupUnlocked(
                this->groupCursor.GetCurrent()) != 0)
        {
            reinterpret_cast<AnmVmColorInterpolationView *>(
                initialGroupVm)
                ->SetColor1Interpolation(
                    60, 0, SCENE_VM_COLOR(initialGroupVm),
                    g_SceneGroupColors[this->groupCursor.GetCurrent()]);
        }
        else
        {
            reinterpret_cast<AnmVmColorInterpolationView *>(
                initialGroupVm)
                ->SetColor1Interpolation(
                    60, 0, SCENE_VM_COLOR(initialGroupVm),
                    g_SceneLockedInitialColor);
        }
        this->UpdateSelectedSceneDetails();
    }
    case SCENE_SELECT_STATE_ACTIVE:
        break;
    }

    if ((this->stateTimer.current < 30) != 0)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if ((this->stateTimer.current == 30) != 0)
    {
        for (initialSelectionIndex = 0;
             initialSelectionIndex <
                 g_SceneGroupCounts[SceneSelectInitialSelectionGroupPhase(this)];
             initialSelectionIndex++)
        {
            if (this->sceneCursors[this->groupCursor.GetCurrent()].GetCurrent() ==
                initialSelectionIndex)
            {
                SCENE_MENU(this)->vmIds.SetInterrupt(initialSelectionIndex * 3 + 0x25, 2);
                SCENE_MENU(this)->vmIds.SetInterrupt(initialSelectionIndex * 3 + 0x26, 2);
                SCENE_MENU(this)->vmIds.SetInterrupt(initialSelectionIndex * 3 + 0x27, 2);
            }
            else
            {
                SCENE_MENU(this)->vmIds.SetInterrupt(initialSelectionIndex * 3 + 0x25, 3);
                SCENE_MENU(this)->vmIds.SetInterrupt(initialSelectionIndex * 3 + 0x26, 3);
                SCENE_MENU(this)->vmIds.SetInterrupt(initialSelectionIndex * 3 + 0x27, 3);
            }
        }
        this->RefreshSceneSelection(
            this->selectedScoreEntryIndex);
    }

    if (SceneInputAnd(g_ControllerInputSlots[0].pressed, TH_BUTTON_S) != 0)
    {
        this->flagBits.showRates = 1 - this->flagBits.showRates;
    }

    if (SceneInputAnd(g_ControllerInputSlots[0].current, TH_BUTTON_SKIP) != 0)
    {
        SCENE_VM_HIDE(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x15]));
        SCENE_VM_HIDE(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x16]));
        SCENE_VM_HIDE(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x45]));
        if (SCENE_MENU(this)->previewTextVmIds[0].GetVm() != NULL)
        {
            SCENE_VM_HIDE(SCENE_MENU(this)->previewTextVmIds[0].GetVm());
        }
        if (SCENE_MENU(this)->previewTextVmIds[1].GetVm() != NULL)
        {
            SCENE_VM_HIDE(SCENE_MENU(this)->previewTextVmIds[1].GetVm());
        }
        if (SCENE_MENU(this)->previewTextVmIds[2].GetVm() != NULL)
        {
            SCENE_VM_HIDE(SCENE_MENU(this)->previewTextVmIds[2].GetVm());
        }
        this->flagBits.sceneDetailsVisible = 0;
    }
    else
    {
        SCENE_VM_SHOW(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x15]));
        SCENE_VM_SHOW(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x16]));
        SCENE_VM_SHOW(SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x45]));
        if (SCENE_MENU(this)->previewTextVmIds[0].GetVm() != NULL)
        {
            SCENE_VM_SHOW(SCENE_MENU(this)->previewTextVmIds[0].GetVm());
        }
        if (SCENE_MENU(this)->previewTextVmIds[1].GetVm() != NULL)
        {
            SCENE_VM_SHOW(SCENE_MENU(this)->previewTextVmIds[1].GetVm());
        }
        if (SCENE_MENU(this)->previewTextVmIds[2].GetVm() != NULL)
        {
            SCENE_VM_SHOW(SCENE_MENU(this)->previewTextVmIds[2].GetVm());
        }
        this->flagBits.sceneDetailsVisible = 1;
    }

    if (SceneInputAnd(g_ControllerInputSlots[0].pressed, TH_BUTTON_L) != 0 &&
        g_ResultSaveData->scoreEntries[this->selectedScoreEntryIndex].magic != 0)
    {
        SceneSelectScoreFlagsAt(g_ResultSaveData, this->selectedScoreEntryIndex)
            ->bestShotLocked ^= 1;
    }

    this->groupCursor.SaveCurrent();
    groupDirection = 0;
    if (SceneInputAnd(g_ControllerInputSlots[0].pressed, TH_BUTTON_LEFT) != 0)
    {
        this->groupCursor.Move(-1);
        groupDirection = -1;
    }
    if (SceneInputAnd(g_ControllerInputSlots[0].pressed, TH_BUTTON_RIGHT) != 0)
    {
        this->groupCursor.Move(1);
        groupDirection = 1;
    }

    if (this->groupCursor.HasChanged())
    {
        g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        SCENE_ANM_MANAGER->MarkVmForDeletion(SCENE_MENU(this)->vmIds[0x20]);
        SCENE_ANM_MANAGER->MarkVmForDeletion(SCENE_MENU(this)->vmIds[0x21]);
        SCENE_ANM_MANAGER->MarkVmForDeletion(SCENE_MENU(this)->vmIds[0x22]);
        SceneSelectCreateVmAt(this, 0x22);
        if (this->groupCursor.GetCurrent() <= 10)
        {
            SceneSelectCreateVmAt(this, 0x20);
            SCENE_MENU(this)->sceneAnm->SetSprite(
                SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x22]),
                this->groupCursor.GetCurrent() + 0x37);
        }
        else
        {
            SceneSelectCreateVmAt(this, 0x21);
            SCENE_MENU(this)->sceneAnm->SetSprite(
                SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x22]),
                this->groupCursor.GetCurrent() + 0x2c);
        }

        g_SelectedScene =
            &g_SceneGroups[this->groupCursor.GetCurrent()]
                          [activeSceneCursor.GetCurrent()];
        this->selectedScoreEntryIndex = g_SelectedScene->scoreEntryIndex;

        for (previousGroupIndex = 0;
             previousGroupIndex <
                 g_SceneGroupCounts[this->groupCursor.GetPrevious()];
             previousGroupIndex++)
        {
            SCENE_MENU(this)->vmIds.SetInterrupt(
                previousGroupIndex * 3 + 0x25,
                groupDirection <= 0 ? 8 : 7);
            SCENE_MENU(this)->vmIds.SetInterrupt(
                previousGroupIndex * 3 + 0x26,
                groupDirection <= 0 ? 8 : 7);
            SCENE_MENU(this)->vmIds.SetInterrupt(
                previousGroupIndex * 3 + 0x27,
                groupDirection <= 0 ? 8 : 7);
        }
        for (newGroupIndex = 0;
             newGroupIndex < g_SceneGroupCounts[this->groupCursor.GetCurrent()];
             newGroupIndex++)
        {
            SceneSelectNewGroupSceneVmPhase(this, newGroupIndex, groupDirection);
        }

        SCENE_MENU(this)->vmIds.SetInterrupt(0x12, groupDirection <= 0 ? 8 : 7);
        SceneSelectCreateVmAt(this, 0x12);
        SCENE_MENU(this)->vmIds.SetInterrupt(0x12, 5);
        SCENE_MENU(this)->vmIds.SetInterrupt(0x12, groupDirection > 0 ? 10 : 9);
        SCENE_MENU(this)->vmIds.SetInterrupt(0x14, 5);
        this->stateTimer.Set(20);
        SCENE_MENU(this)->sceneAnm->SetSprite(
            SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x49]),
            (this->groupCursor.GetCurrent() >= 11
                 ? this->groupCursor.GetCurrent() - 11
                 : this->groupCursor.GetCurrent()) +
                0x28);
        SCENE_MENU(this)->sceneAnm->SetSprite(
            SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x4b]),
            activeSceneCursor.GetCurrent() + 0x28);
        SCENE_MENU(this)->vmIds.SetInterrupt(0x49, 2);
        SCENE_MENU(this)->vmIds.SetInterrupt(0x4a, 2);
        SCENE_MENU(this)->vmIds.SetInterrupt(0x4b, 2);
        this->UpdateSelectedSceneDetails();
        g_ResultSaveData->profile.lastSelectedGroup =
            (i16)this->groupCursor.GetCurrent();
        g_ResultSaveData->profile.lastSelectedScene =
            (i16)activeSceneCursor.GetCurrent();

        SET_SCENE_VM_VISIBILITY(
            this, 0x23, this->groupCursor.GetCurrent() != 0);
        SET_SCENE_VM_VISIBILITY(
            this, 0x24,
            this->groupCursor.GetCurrent() != this->groupCursor.GetCount() - 1);

        transitionGroupVm = SCENE_MENU(this)->vmIds.values[0x68].GetVm();
        if (g_ResultSaveData->IsSceneGroupUnlocked(
                this->groupCursor.GetCurrent()) != 0)
        {
            reinterpret_cast<AnmVmColorInterpolationView *>(
                transitionGroupVm)
                ->SetColor1Interpolation(
                    60, 0, SCENE_VM_COLOR(transitionGroupVm),
                    g_SceneGroupColors[this->groupCursor.GetCurrent()]);
        }
        else
        {
            reinterpret_cast<AnmVmColorInterpolationView *>(
                transitionGroupVm)
                ->SetColor1Interpolation(
                    60, 0, SCENE_VM_COLOR(transitionGroupVm),
                    g_SceneLockedTransitionColor);
        }    }
    else
    {
        if (g_ResultSaveData->IsSceneGroupUnlocked(
                SceneSelectPostTransitionGroupPhase(this)) != 0)
        {
            activeSceneCursor.SaveCurrent();
            if ((u16)(SceneInputAnd(g_ControllerInputSlots[0].pressed, TH_BUTTON_UP) != 0 ||
                      (g_ControllerInputSlots[0].repeat & ScenePreviousInputMask()) != 0) != 0)
            {
                activeSceneCursor.Move(-1);
            }
            if ((u16)(SceneInputAnd(g_ControllerInputSlots[0].pressed, TH_BUTTON_DOWN) != 0 ||
                      (g_ControllerInputSlots[0].repeat & SceneNextInputMask()) != 0) != 0)
            {
                activeSceneCursor.Move(1);
            }

            if (activeSceneCursor.HasChanged())
            {
                g_SelectedScene =
                    &g_SceneGroups[this->groupCursor.GetCurrent()]
                                  [activeSceneCursor.GetCurrent()];
                this->selectedScoreEntryIndex =
                    g_SelectedScene->scoreEntryIndex;
                for (selectionIndex = 0;
                     selectionIndex <
                         g_SceneGroupCounts[this->groupCursor.GetCurrent()];
                     selectionIndex++)
                {
                    if (activeSceneCursor.GetCurrent() == selectionIndex)
                    {
                        SCENE_MENU(this)->vmIds.SetInterrupt(
                            selectionIndex * 3 + 0x25, 2);
                        SCENE_MENU(this)->vmIds.SetInterrupt(
                            selectionIndex * 3 + 0x26, 2);
                        SCENE_MENU(this)->vmIds.SetInterrupt(
                            selectionIndex * 3 + 0x27, 2);
                    }
                    else
                    {
                        SCENE_MENU(this)->vmIds.SetInterrupt(
                            selectionIndex * 3 + 0x25, 3);
                        SCENE_MENU(this)->vmIds.SetInterrupt(
                            selectionIndex * 3 + 0x26, 3);
                        SCENE_MENU(this)->vmIds.SetInterrupt(
                            selectionIndex * 3 + 0x27, 3);
                    }
                }
                g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
                this->RefreshSceneSelection(
                    this->selectedScoreEntryIndex);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x12, 5);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x14, 5);
                SCENE_MENU(this)->sceneAnm->SetSprite(
                    SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x49]),
                    (this->groupCursor.GetCurrent() >= 11
                         ? this->groupCursor.GetCurrent() - 11
                         : this->groupCursor.GetCurrent()) +
                        0x28);
                SCENE_MENU(this)->sceneAnm->SetSprite(
                    SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds[0x4b]),
                    activeSceneCursor.GetCurrent() + 0x28);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x49, 2);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x4a, 2);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x4b, 2);
                this->UpdateSelectedSceneDetails();
                g_ResultSaveData->profile.lastSelectedScene =
                    (i16)activeSceneCursor.GetCurrent();
                goto update_preview_text;
            }

            if (SceneInputAnd(g_ControllerInputSlots[0].pressed, (u16)0x1002) != 0)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                g_SelectedScene =
                    &g_SceneGroups[this->groupCursor.GetCurrent()]
                                  [activeSceneCursor.GetCurrent()];
                this->requestedState = FRONT_END_REQUESTED_STATE_START_GAME;
                this->state = SCENE_SELECT_STATE_INITIALIZE;
                FrontEndResetTimer(&this->stateTimer);
                this->flagBits.assetLoadStopRequested = 1;
                g_Supervisor.replayScanWorker.RequestExit();
                g_AnmManager->MarkVmForDeletion(
                    SCENE_MENU(this)->previewTextVmIds[0]);
                g_AnmManager->MarkVmForDeletion(
                    SCENE_MENU(this)->previewTextVmIds[1]);
                g_AnmManager->MarkVmForDeletion(
                    SCENE_MENU(this)->previewTextVmIds[2]);
                selected = activeSceneCursor.GetCurrent();
                SCENE_MENU(this)->vmIds.SetInterrupt(selected * 3 + 0x25, 6);
                SCENE_MENU(this)->vmIds.SetInterrupt(selected * 3 + 0x26, 6);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x1e, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x23, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x24, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x20, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x21, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x22, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x44, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x15, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x16, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x45, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x46, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x13, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x12, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x14, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x49, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x4a, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x4b, 1);
                for (gameExitVmIndex = 0; gameExitVmIndex < 16;
                     gameExitVmIndex++)
                {
                    SCENE_MENU(this)->vmIds.SetInterrupt(0x4c + gameExitVmIndex, 1);
                }
                SCENE_MENU(this)->vmIds.SetInterrupt(0x5c, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x5d, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x5e, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x5f, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x60, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x61, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x62, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x63, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x48, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(0x82, 1);
                SceneSelectClearPreviewVmPhase(this);
                return CHAIN_CALLBACK_RESULT_CONTINUE;
            }
        }

        if (SceneInputAnd(g_ControllerInputSlots[0].pressed, 9) != 0)
        {
            this->flagBits.assetLoadStopRequested = 1;
            g_SelectedScene =
                &g_SceneGroups[this->groupCursor.GetCurrent()]
                              [this->sceneCursors[this->groupCursor.GetCurrent()]
                                   .GetCurrent()];
            this->groupCursor.Pop();
            this->requestedState = FRONT_END_REQUESTED_STATE_MAIN_MENU;
            this->state = SCENE_SELECT_STATE_INITIALIZE;
            FrontEndResetTimer(&this->stateTimer);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x68, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x69, 1);
            SceneSelectCreateVmAt(this, 0x66);
            SceneSelectCreateVmAt(this, 0x67);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x19, 2);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x1a, 2);
            SCENE_MENU(this)->transitionVm.SetInterrupt(2);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x1b, 2);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x1e, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x23, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x24, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x20, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x21, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x22, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x44, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x15, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x16, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x45, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x46, 1);
            for (menuExitSceneIndex = 0; menuExitSceneIndex < 9;
                 menuExitSceneIndex++)
            {
                SCENE_MENU(this)->vmIds.SetInterrupt(menuExitSceneIndex * 3 + 0x25, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(menuExitSceneIndex * 3 + 0x26, 1);
                SCENE_MENU(this)->vmIds.SetInterrupt(menuExitSceneIndex * 3 + 0x27, 1);
            }
            SCENE_MENU(this)->vmIds.SetInterrupt(0x13, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x12, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x14, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x49, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x4a, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x4b, 1);
            for (menuExitVmIndex = 0; menuExitVmIndex < 16;
                 menuExitVmIndex++)
            {
                SCENE_MENU(this)->vmIds.SetInterrupt(0x4c + menuExitVmIndex, 1);
            }
            SCENE_MENU(this)->vmIds.SetInterrupt(0x5c, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x5d, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x5e, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x5f, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x60, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x61, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x62, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x63, 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x48, 1);
            g_AnmManager->SetInterrupt(SCENE_MENU(this)->previewTextVmIds[0], 1);
            g_AnmManager->SetInterrupt(SCENE_MENU(this)->previewTextVmIds[1], 1);
            g_AnmManager->SetInterrupt(SCENE_MENU(this)->previewTextVmIds[2], 1);
            SCENE_MENU(this)->vmIds.SetInterrupt(0x82, 1);
            SceneSelectClearPreviewVmPhase(this);
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }
    }
update_preview_text:
    if (SceneQueueSize(&this->selectionQueue) != 0 ||
        SceneQueueSize(&this->loadedSceneQueue) != 0)
    {
        if (this->flagBits.previewPending == 0)
        {
            SCENE_MENU(this)->vmIds.SetInterrupt(0x12, 3);
            this->flagBits.previewPending = 1;
        }
    }
    else
    {
        if (this->flagBits.previewPending != 0)
        {
            previewVm =
                SCENE_ANM_MANAGER->GetVm(SCENE_MENU(this)->vmIds.values[0x12]);
            if (g_ResultSaveData->bestShotRecords[
                    this->selectedScoreEntryIndex].componentsLoaded != 0)
            {
                previewVm->loadedSprite->uvEndX =
                    (f32)g_ResultSaveData->bestShotRecords[
                        this->selectedScoreEntryIndex].width /
                    256.0f;
                previewVm->loadedSprite->uvEndY =
                    (f32)g_ResultSaveData->bestShotRecords[
                        this->selectedScoreEntryIndex].height /
                    256.0f;
                previewVm->spriteWidth =
                    (f32)g_ResultSaveData->bestShotRecords[
                        this->selectedScoreEntryIndex].width;
                previewVm->spriteHeight =
                    (f32)g_ResultSaveData->bestShotRecords[
                        this->selectedScoreEntryIndex].height;
                SCENE_VM_SHOW(previewVm);
            }
            else
            {
                SCENE_VM_HIDE(previewVm);
            }
            SCENE_ANM_MANAGER->SetInterrupt(SCENE_MENU(this)->vmIds.values[0x12], 2);
            this->flagBits.previewPending = 0;
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
}

#undef SET_SCENE_VM_VISIBILITY
#undef SCENE_MENU
#undef SCENE_ANM_MANAGER
#undef SCENE_VM_SHOW
#undef SCENE_VM_HIDE
#undef SCENE_VM_COLOR

} // namespace th095
