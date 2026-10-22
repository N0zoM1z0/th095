#include "Main.hpp"
#include "SceneSelect.hpp"

#include "FileSystem.hpp"

#include <stdio.h>

namespace th095
{

typedef char SceneSelectionAssetPendingTextureDataLayout[
    (sizeof(FrontEndPendingTextureData) == 0x34 &&
     offsetof(SceneSelectControllerView, pendingTextures) == 0x63cc &&
     offsetof(FrontEndPendingTextureData, primaryData) == 0x04 &&
     offsetof(FrontEndPendingTextureData, primarySize) == 0x10 &&
     offsetof(FrontEndPendingTextureData, secondaryData) == 0x1c &&
     offsetof(FrontEndPendingTextureData, secondarySize) == 0x28) ? 1 : -1];
typedef char SceneSelectionAssetFlagsAt6120[
    (offsetof(SceneSelectControllerView, flags) == 0x6120) ? 1 : -1];
typedef char SceneSelectionAssetStateHistoryAt63B0[
    (offsetof(SceneSelectControllerView, stateHistory) == 0x63b0) ? 1 : -1];
typedef char SceneSelectionAssetPendingCountAt63CC[
    (offsetof(SceneSelectControllerView, pendingTextures) == 0x63cc) ? 1 : -1];

struct SceneSelectionAssetLoadLocals
{
    i32 secondarySize;
    u8 *secondaryData;
    char missionPath[MAX_PATH];
    i32 primarySize;
    u8 *primaryData;
    i32 i;
    char facePath[256];
    SceneSelectControllerView *view;
    i32 queueValue;
};

typedef char SceneSelectionAssetLoadLocalsSize[
    (sizeof(SceneSelectionAssetLoadLocals) == 0x220) ? 1 : -1];
typedef char SceneSelectionAssetLoadQueueValueAt21C[
    (offsetof(SceneSelectionAssetLoadLocals, queueValue) == 0x21c) ? 1 : -1];
typedef char SceneSelectionAssetLoadViewAt218[
    (offsetof(SceneSelectionAssetLoadLocals, view) == 0x218) ? 1 : -1];
typedef char SceneSelectionAssetLoadFacePathAt118[
    (offsetof(SceneSelectionAssetLoadLocals, facePath) == 0x118) ? 1 : -1];
typedef char SceneSelectionAssetLoadMissionPathAt8[
    (offsetof(SceneSelectionAssetLoadLocals, missionPath) == 8) ? 1 : -1];

static __forceinline i32 AssetQueueFront(SceneValueQueue *queue)
{
    if (ReadSceneQueueWord32(queue->words[SCENE_QUEUE_COUNT_WORD].value) > 0)
    {
        AcquireSceneQueuePayload();
        return ReadSceneQueueWord32(queue->words[0].value);
    }
    return 0;
}

static __forceinline i32 AssetQueueSize(SceneValueQueue *queue)
{
    return ReadSceneQueueWord32(queue->words[SCENE_QUEUE_COUNT_WORD].value);
}

static __forceinline i32 AssetQueueSize(ScenePreviewDataQueue *queue)
{
    return static_cast<i32>(ReadSceneQueueAddressWord(queue->words[SCENE_QUEUE_COUNT_WORD].bits));
}

static __forceinline void AssetQueueRead(SceneValueQueue *queue, i32 *value)
{
    if (ReadSceneQueueWord32(queue->words[SCENE_QUEUE_COUNT_WORD].value) > 0)
    {
        AcquireSceneQueuePayload();
        *value = ReadSceneQueueWord32(queue->words[0].value);
    }
    else
    {
        *value = 0;
    }
}

static __forceinline void AssetQueuePush(SceneValueQueue *queue, i32 value)
{
    if (ReadSceneQueueWord32(queue->words[SCENE_QUEUE_COUNT_WORD].value) < ReadSceneQueueWord32(queue->words[SCENE_QUEUE_CAPACITY_WORD].value))
    {
        WriteSceneQueueWord32(queue->words[ReadSceneQueueWord32(queue->words[SCENE_QUEUE_COUNT_WORD].value)].value, value);
        IncrementSceneQueueCount32(queue->words[SCENE_QUEUE_COUNT_WORD].value);
    }
}

static __forceinline void AssetQueuePushPointer(ScenePreviewDataQueue *queue,
                                                u8 **value)
{
#include "ScenePreviewDataPublish.inl"
}

static __forceinline void AssetQueuePushNull(ScenePreviewDataQueue *queue)
{
#include "ScenePreviewDataPublishNull.inl"
}

static __forceinline void AssetQueuePushValue(SceneValueQueue *queue,
                                              i32 *value)
{
    if (ReadSceneQueueWord32(queue->words[SCENE_QUEUE_COUNT_WORD].value) < ReadSceneQueueWord32(queue->words[SCENE_QUEUE_CAPACITY_WORD].value))
    {
        WriteSceneQueueWord32(queue->words[ReadSceneQueueWord32(queue->words[SCENE_QUEUE_COUNT_WORD].value)].value, *value);
        IncrementSceneQueueCount32(queue->words[SCENE_QUEUE_COUNT_WORD].value);
    }
}

static __forceinline u32 SceneSelectionAssetsSupervisorStopRequested()
{
    return g_SceneSupervisor.flags.receivedCloseMsg;
}

static __forceinline i32 SceneSelectionAssetsExitRequested()
{
    // Nonzero is also a stop request, not proof that asset loading completed.
    i32 value;
    ReadWorkerSignal(value, g_Supervisor.replayScanWorker.exitSignal);
    return value;
}

static __forceinline i32 SceneSelectionGroupPreviewSizePhase(
    SceneValueQueue *queue)
{
    u8 compilerStorage[4];
    return queue->Size();
}

static __forceinline void SceneSelectionFinishPhase()
{
    u8 compilerStorage[8];
    WriteWorkerSignal(g_Supervisor.replayScanWorker.active, 0);
    WriteWorkerSignal(g_Supervisor.replayScanWorker.exitSignal, 1);
}

void __fastcall LoadSceneSelectionAssets(void *threadParameter)
{
    SceneSelectionAssetLoadLocals locals;
    locals.view = g_SceneSelectController;

    while (true)
    {
        if (SceneSelectionAssetsSupervisorStopRequested() != 0)
        {
            break;
        }
        if (locals.view->flagBits.assetLoadStopRequested != 0)
        {
            break;
        }
        if (SceneSelectionAssetsExitRequested() != 0)
        {
            break;
        }

        g_SceneSupervisor.EnterCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]++;
        if (AssetQueueSize(&locals.view->selectionQueue) == 0 &&
            AssetQueueSize(&locals.view->groupPreviewQueue) == 0 &&
            locals.view->stateHistory.count == 0)
        {
            g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]--;
            Sleep(1);
            continue;
        }
        g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]--;

        /* Face/status pages take priority over mission thumbnails. */
        if (locals.view->stateHistory.count > 0)
        {
            locals.queueValue = locals.view->stateHistory.values[0];
            sprintf(locals.facePath, "fc%.2d.anm", locals.queueValue);
            locals.view->pendingTextures.primaryData
                [locals.view->pendingTextures.count] = FileSystem::OpenFile(
                    locals.facePath,
                    &locals.view
                         ->pendingTextures.primarySize
                              [locals.view->pendingTextures.count],
                    FALSE);
            if (locals.view
                    ->pendingTextures.primaryData
                         [locals.view->pendingTextures.count] != 0)
            {
                sprintf(locals.facePath, "fc%.2db.anm", locals.queueValue);
                locals.view
                    ->pendingTextures.secondaryData
                         [locals.view->pendingTextures.count] = FileSystem::OpenFile(
                        locals.facePath,
                        &locals.view->pendingTextures.secondarySize
                             [locals.view->pendingTextures.count],
                        FALSE);
                g_SceneSupervisor.EnterCriticalSectionWrapper(4);
                g_SceneSupervisor.lockCounts[4]++;
                locals.view->pendingTextures.count++;
                g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
                g_SceneSupervisor.lockCounts[4]--;
            }

            g_SceneSupervisor.EnterCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]++;
            for (locals.i = 0; locals.i < 2; locals.i++)
            {
                locals.view->stateHistory.values[locals.i] =
                    locals.view->stateHistory.values[locals.i + 1];
            }
            locals.view->stateHistory.count--;
            locals.view->stateHistory.values[2] = 0;
            g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]--;
        }
        else
        {
            if (AssetQueueSize(&locals.view->selectionQueue) != 0)
            {
                AssetQueueRead(&locals.view->selectionQueue,
                               &locals.queueValue);

                if (g_SceneSaveData->LoadBestShotForScene(
                        locals.queueValue >> 8,
                        locals.queueValue & 0xff) == 0)
                {
                    while (AssetQueueSize(&locals.view->loadedSceneQueue) != 0)
                    {
                        Sleep(10);
#include "SceneSelectionAssetWaitCancellation.inl"
                    }

                    g_SceneSupervisor.EnterCriticalSectionWrapper(4);
                    g_SceneSupervisor.lockCounts[4]++;
                    AssetQueuePush(
                        &locals.view->loadedSceneQueue,
                        g_SceneGroups[locals.queueValue >> 8]
                                     [locals.queueValue & 0xff]
                            .scoreEntryIndex);
                    g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
                    g_SceneSupervisor.lockCounts[4]--;
                }
                else
                {
                    while (AssetQueueSize(&locals.view->loadedSceneQueue) != 0)
                    {
                        Sleep(10);
#include "SceneSelectionAssetWaitCancellation.inl"
                    }

                    g_SceneSupervisor.EnterCriticalSectionWrapper(4);
                    g_SceneSupervisor.lockCounts[4]++;
                    AssetQueuePush(&locals.view->loadedSceneQueue, -1);
                    g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
                    g_SceneSupervisor.lockCounts[4]--;
                }
                g_SceneSupervisor.EnterCriticalSectionWrapper(4);
                g_SceneSupervisor.lockCounts[4]++;
                locals.view->selectionQueue.Pop();
                g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
                g_SceneSupervisor.lockCounts[4]--;
            }
            else if (SceneSelectionGroupPreviewSizePhase(&locals.view->groupPreviewQueue) != 0)
            {
                AssetQueueRead(&locals.view->groupPreviewQueue,
                               &locals.queueValue);
                if (locals.queueValue >= 0)
                {
                    sprintf(locals.missionPath, "mission%.3d.anm",
                            locals.queueValue);
                    locals.primaryData = FileSystem::OpenFile(
                        locals.missionPath, &locals.primarySize, FALSE);
                    if (locals.primaryData != NULL)
                    {
                        sprintf(locals.missionPath, "mission%.3d.anm",
                                AssetQueueFront(
                                    &locals.view->scenePreviewQueue) +
                                    100);
                        locals.secondaryData = FileSystem::OpenFile(
                            locals.missionPath, &locals.secondarySize, FALSE);
#include "ScenePreviewDataWait.inl"

                        g_SceneSupervisor.EnterCriticalSectionWrapper(4);
                        g_SceneSupervisor.lockCounts[4]++;
                        AssetQueuePushPointer(
                            &locals.view->groupPreviewDataQueue,
                            &locals.primaryData);
                        AssetQueuePush(&locals.view->groupPreviewSizeQueue,
                                       locals.primarySize);
                        AssetQueuePushPointer(
                            &locals.view->scenePreviewDataQueue,
                            &locals.secondaryData);
                        AssetQueuePush(&locals.view->scenePreviewSizeQueue,
                                       locals.secondarySize);
                        AssetQueuePushValue(&locals.view->loadedGroupQueue,
                                            &locals.queueValue);
                        g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
                        g_SceneSupervisor.lockCounts[4]--;
                    }
                }
                else
                {
                    sprintf(locals.missionPath, "mission_%.2d.anm",
                            -locals.queueValue);
                    locals.primaryData = FileSystem::OpenFile(
                        locals.missionPath, &locals.primarySize, FALSE);
                    if (locals.primaryData != NULL)
                    {
#include "ScenePreviewDataWait.inl"

                        g_SceneSupervisor.EnterCriticalSectionWrapper(4);
                        g_SceneSupervisor.lockCounts[4]++;
                        AssetQueuePushPointer(
                            &locals.view->groupPreviewDataQueue,
                            &locals.primaryData);
                        AssetQueuePush(&locals.view->groupPreviewSizeQueue,
                                       locals.primarySize);
                        AssetQueuePushNull(&locals.view->scenePreviewDataQueue);
                        AssetQueuePush(&locals.view->scenePreviewSizeQueue, 0);
                        AssetQueuePushValue(&locals.view->loadedGroupQueue,
                                            &locals.queueValue);
                        g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
                        g_SceneSupervisor.lockCounts[4]--;
                    }
                }

                g_SceneSupervisor.EnterCriticalSectionWrapper(4);
                g_SceneSupervisor.lockCounts[4]++;
                locals.view->groupPreviewQueue.Pop();
                locals.view->scenePreviewQueue.Pop();
                g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
                g_SceneSupervisor.lockCounts[4]--;
            }
        }
    }

    SceneSelectionFinishPhase();
}

} // namespace th095
