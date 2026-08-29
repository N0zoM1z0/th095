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
    u32 flags;
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

static __forceinline i32 AssetQueueFront(SceneValueQueue *queue)
{
    return queue->count < 1 ? 0 : queue->values[0];
}

static __forceinline void AssetQueuePush(SceneValueQueue *queue, i32 value)
{
    if (queue->count < queue->capacity)
    {
        queue->values[queue->count] = value;
        queue->count++;
    }
}

static __forceinline bool SceneSelectionAssetsShouldStop(
    SceneSelectionAssetView *view)
{
    u32 supervisorFlags =
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(&g_SceneSupervisor) +
                                 0x444);
    return ((supervisorFlags & 0x80) != 0) ||
           ((view->flags & 0x20) != 0) || g_HelpLoadComplete != 0;
}

void __fastcall LoadSceneSelectionAssets(void *)
{
    SceneSelectionAssetView *view =
        reinterpret_cast<SceneSelectionAssetView *>(g_SceneSelectController);
    char path[MAX_PATH];

    for (;;)
    {
        if (SceneSelectionAssetsShouldStop(view))
        {
            g_HelpLoadActive = 0;
            g_HelpLoadComplete = 1;
            return;
        }

        g_SceneSupervisor.EnterCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]++;
        bool hasWork = view->selectionQueue.count != 0 ||
                       view->groupPreviewQueue.count != 0 ||
                       view->stateHistory.count != 0;
        if (!hasWork)
        {
            g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]--;
            Sleep(1);
            continue;
        }
        g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]--;

        /* Face/status pages take priority over mission thumbnails. */
        if (view->stateHistory.count > 0)
        {
            u32 displayState = (u32)view->stateHistory.values[0];
            sprintf(path, "fc%.2d.anm", displayState);
            view->pendingPrimaryData[view->pendingTextureCount] =
                reinterpret_cast<i32>(FileSystem::OpenFile(
                    path,
                    &view->pendingPrimarySize[view->pendingTextureCount],
                    FALSE));
            if (view->pendingPrimaryData[view->pendingTextureCount] != 0)
            {
                sprintf(path, "fc%.2db.anm", displayState);
                view->pendingSecondaryData[view->pendingTextureCount] =
                    reinterpret_cast<i32>(FileSystem::OpenFile(
                        path,
                        &view->pendingSecondarySize[view->pendingTextureCount],
                        FALSE));
                g_SceneSupervisor.EnterCriticalSectionWrapper(4);
                g_SceneSupervisor.lockCounts[4]++;
                view->pendingTextureCount++;
                g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
                g_SceneSupervisor.lockCounts[4]--;
            }

            g_SceneSupervisor.EnterCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]++;
            for (i32 i = 0; i < 2; i++)
            {
                view->stateHistory.values[i] =
                    view->stateHistory.values[i + 1];
            }
            view->stateHistory.count--;
            view->stateHistory.values[2] = 0;
            g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]--;
            continue;
        }

        if (view->selectionQueue.count == 0 &&
            view->groupPreviewQueue.count != 0)
        {
            i32 groupPreview = AssetQueueFront(&view->groupPreviewQueue);
            if (groupPreview < 0)
            {
                i32 primarySize;
                sprintf(path, "mission_%.2d.anm", -groupPreview);
                u8 *primaryData =
                    FileSystem::OpenFile(path, &primarySize, FALSE);
                if (primaryData != NULL)
                {
                    while (view->groupPreviewDataQueue.count != 0)
                    {
                        Sleep(10);
                        if (SceneSelectionAssetsShouldStop(view))
                        {
                            break;
                        }
                    }

                    g_SceneSupervisor.EnterCriticalSectionWrapper(4);
                    g_SceneSupervisor.lockCounts[4]++;
                    AssetQueuePush(&view->groupPreviewDataQueue,
                                   reinterpret_cast<i32>(primaryData));
                    AssetQueuePush(&view->groupPreviewSizeQueue,
                                   primarySize);
                    AssetQueuePush(&view->scenePreviewDataQueue, 0);
                    AssetQueuePush(&view->scenePreviewSizeQueue, 0);
                    AssetQueuePush(&view->loadedGroupQueue, groupPreview);
                    g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
                    g_SceneSupervisor.lockCounts[4]--;
                }
            }
            else
            {
                i32 primarySize;
                sprintf(path, "mission%.3d.anm", groupPreview);
                u8 *primaryData =
                    FileSystem::OpenFile(path, &primarySize, FALSE);
                if (primaryData != NULL)
                {
                    i32 scenePreview =
                        AssetQueueFront(&view->scenePreviewQueue);
                    i32 secondarySize;
                    sprintf(path, "mission%.3d.anm", scenePreview + 100);
                    u8 *secondaryData =
                        FileSystem::OpenFile(path, &secondarySize, FALSE);
                    while (view->groupPreviewDataQueue.count != 0)
                    {
                        Sleep(10);
                        if (SceneSelectionAssetsShouldStop(view))
                        {
                            break;
                        }
                    }

                    g_SceneSupervisor.EnterCriticalSectionWrapper(4);
                    g_SceneSupervisor.lockCounts[4]++;
                    AssetQueuePush(&view->groupPreviewDataQueue,
                                   reinterpret_cast<i32>(primaryData));
                    AssetQueuePush(&view->groupPreviewSizeQueue,
                                   primarySize);
                    AssetQueuePush(&view->scenePreviewDataQueue,
                                   reinterpret_cast<i32>(secondaryData));
                    AssetQueuePush(&view->scenePreviewSizeQueue,
                                   secondarySize);
                    AssetQueuePush(&view->loadedGroupQueue, groupPreview);
                    g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
                    g_SceneSupervisor.lockCounts[4]--;
                }
            }

            g_SceneSupervisor.EnterCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]++;
            view->groupPreviewQueue.Pop();
            view->scenePreviewQueue.Pop();
            g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]--;
            continue;
        }

        i32 packedSelection = AssetQueueFront(&view->selectionQueue);
        i32 group = packedSelection >> 8;
        i32 scene = packedSelection & 0xff;
        i32 loadResult =
            g_SceneSaveData->LoadBestShotForScene(group, scene);

        if (loadResult == 0)
        {
            while (view->loadedSceneQueue.count != 0)
            {
                Sleep(10);
                if (SceneSelectionAssetsShouldStop(view))
                {
                    break;
                }
            }
            g_SceneSupervisor.EnterCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]++;
            AssetQueuePush(
                &view->loadedSceneQueue,
                g_SceneGroups[group][scene].scoreEntryIndex);
            g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]--;
        }
        else
        {
            while (view->loadedSceneQueue.count != 0)
            {
                Sleep(10);
                if (SceneSelectionAssetsShouldStop(view))
                {
                    break;
                }
            }
            g_SceneSupervisor.EnterCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]++;
            AssetQueuePush(&view->loadedSceneQueue, -1);
            g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
            g_SceneSupervisor.lockCounts[4]--;
        }

        g_SceneSupervisor.EnterCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]++;
        view->selectionQueue.Pop();
        g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
        g_SceneSupervisor.lockCounts[4]--;
    }
}

} // namespace th095
