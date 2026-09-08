#include "SceneSelect.hpp"

#include "FileSystem.hpp"

#include <stdio.h>

namespace th095
{

extern i32 g_HelpLoadActive;
extern i32 g_HelpLoadComplete;

struct SceneSelectionAssetView
{
    u8 unknown0000[0x6120];
    union
    {
        u32 flags;
        struct
        {
            u32 unknownFlagBits0 : 5;
            u32 stopRequested : 1;
            u32 unknownFlagBits6 : 26;
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

typedef char SceneSelectionAssetFlagsAt6120[
    (offsetof(SceneSelectionAssetView, flags) == 0x6120) ? 1 : -1];
typedef char SceneSelectionAssetStateHistoryAt63B0[
    (offsetof(SceneSelectionAssetView, stateHistory) == 0x63b0) ? 1 : -1];
typedef char SceneSelectionAssetPendingCountAt63CC[
    (offsetof(SceneSelectionAssetView, pendingTextureCount) == 0x63cc) ? 1
                                                                      : -1];

struct SceneSelectionAssetLoadLocals
{
    i32 secondarySize;
    u8 *secondaryData;
    char missionPath[MAX_PATH];
    i32 primarySize;
    u8 *primaryData;
    i32 i;
    char facePath[256];
    SceneSelectionAssetView *view;
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
    if (queue->count > 0)
    {
        return queue->values[0];
    }
    return 0;
}

static __forceinline i32 AssetQueueSize(SceneValueQueue *queue)
{
    return queue->count;
}

static __forceinline void AssetQueueRead(SceneValueQueue *queue, i32 *value)
{
    if (queue->count > 0)
    {
        *value = queue->values[0];
    }
    else
    {
        *value = 0;
    }
}

static __forceinline void AssetQueuePush(SceneValueQueue *queue, i32 value)
{
    if (queue->count < queue->capacity)
    {
        queue->values[queue->count] = value;
        queue->count++;
    }
}

static __forceinline void AssetQueuePushPointer(SceneValueQueue *queue,
                                                u8 **value)
{
    if (queue->count < queue->capacity)
    {
        queue->values[queue->count] = reinterpret_cast<i32>(*value);
        queue->count++;
    }
}

static __forceinline void AssetQueuePushValue(SceneValueQueue *queue,
                                              i32 *value)
{
    if (queue->count < queue->capacity)
    {
        queue->values[queue->count] = *value;
        queue->count++;
    }
}

static __forceinline u32 SceneSelectionAssetsSupervisorStopRequested()
{
    return (
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(&g_SceneSupervisor) +
                                 0x444) >>
        7) & 1;
}

static __forceinline i32 SceneSelectionAssetsLoadComplete()
{
    i32 value = g_HelpLoadComplete;
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
    g_HelpLoadActive = 0;
    g_HelpLoadComplete = 1;
}

void __fastcall LoadSceneSelectionAssets(void *threadParameter)
{
    SceneSelectionAssetLoadLocals locals;
    locals.view =
        reinterpret_cast<SceneSelectionAssetView *>(g_SceneSelectController);

    while (true)
    {
        if (SceneSelectionAssetsSupervisorStopRequested() != 0)
        {
            break;
        }
        if (locals.view->flagBits.stopRequested != 0)
        {
            break;
        }
        if (SceneSelectionAssetsLoadComplete() != 0)
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
            locals.view->pendingPrimaryData[locals.view->pendingTextureCount] =
                reinterpret_cast<i32>(FileSystem::OpenFile(
                    locals.facePath,
                    &locals.view
                         ->pendingPrimarySize[locals.view->pendingTextureCount],
                    FALSE));
            if (locals.view
                    ->pendingPrimaryData[locals.view->pendingTextureCount] != 0)
            {
                sprintf(locals.facePath, "fc%.2db.anm", locals.queueValue);
                locals.view
                    ->pendingSecondaryData[locals.view->pendingTextureCount] =
                    reinterpret_cast<i32>(FileSystem::OpenFile(
                        locals.facePath,
                        &locals.view->pendingSecondarySize
                             [locals.view->pendingTextureCount],
                        FALSE));
                g_SceneSupervisor.EnterCriticalSectionWrapper(4);
                g_SceneSupervisor.lockCounts[4]++;
                locals.view->pendingTextureCount++;
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
                        if (SceneSelectionAssetsSupervisorStopRequested() != 0)
                        {
                            break;
                        }
                        if (locals.view->flagBits.stopRequested != 0)
                        {
                            break;
                        }
                        if (SceneSelectionAssetsLoadComplete() != 0)
                        {
                            break;
                        }
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
                        if (SceneSelectionAssetsSupervisorStopRequested() != 0)
                        {
                            break;
                        }
                        if (locals.view->flagBits.stopRequested != 0)
                        {
                            break;
                        }
                        if (SceneSelectionAssetsLoadComplete() != 0)
                        {
                            break;
                        }
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
                        while (AssetQueueSize(
                                   &locals.view->groupPreviewDataQueue) != 0)
                        {
                            Sleep(10);
                            if (SceneSelectionAssetsSupervisorStopRequested() !=
                                0)
                            {
                                break;
                            }
                            if (locals.view->flagBits.stopRequested != 0)
                            {
                                break;
                            }
                            if (SceneSelectionAssetsLoadComplete() != 0)
                            {
                                break;
                            }
                        }

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
                        while (AssetQueueSize(
                                   &locals.view->groupPreviewDataQueue) != 0)
                        {
                            Sleep(10);
                            if (SceneSelectionAssetsSupervisorStopRequested() !=
                                0)
                            {
                                break;
                            }
                            if (locals.view->flagBits.stopRequested != 0)
                            {
                                break;
                            }
                            if (SceneSelectionAssetsLoadComplete() != 0)
                            {
                                break;
                            }
                        }

                        g_SceneSupervisor.EnterCriticalSectionWrapper(4);
                        g_SceneSupervisor.lockCounts[4]++;
                        AssetQueuePushPointer(
                            &locals.view->groupPreviewDataQueue,
                            &locals.primaryData);
                        AssetQueuePush(&locals.view->groupPreviewSizeQueue,
                                       locals.primarySize);
                        AssetQueuePush(&locals.view->scenePreviewDataQueue, 0);
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
