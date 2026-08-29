#ifndef TH095_SCENE_SELECT_HPP
#define TH095_SCENE_SELECT_HPP

#include "Global.hpp"
#include "AnmVmId.hpp"

namespace th095
{

struct SceneScoreEntryView
{
    u8 unknown000[0x10];
    i32 score;
    u8 unknown014[0x28];
    i32 attemptCount;
    u8 unknown040[4];
    i32 unlockScore;
    u8 unknown048[8];
    u32 flags;
    u8 unknown054[0x0c];
};

struct SceneDefinitionView
{
    i32 scoreEntryIndex;
    u8 unknown004[0x1c];
    i8 groupDisplayValue;
    i8 sceneDisplayValue;
    u8 unknown022[2];
    i32 scoreRequirement;
    u8 unknown028[4];
    i8 displayState;
    u8 unknown02d[3];
};

struct SceneValueQueue
{
    i32 values[16];
    i32 count;
    i32 capacity;

    i32 Push(i32 value);
};

struct SceneGroupCursorView
{
    i32 current;
    u8 unknown004[0xd4];
};

struct SceneStateHistoryView
{
    i32 values[3];
    i32 count;
};

struct SceneSupervisorView
{
    u8 unknown000[0x664];
    CRITICAL_SECTION criticalSections[7];
    u8 lockCounts[7];

    void EnterCriticalSectionWrapper(i32 id)
    {
        EnterCriticalSection(&this->criticalSections[id]);
    }

    void LeaveCriticalSectionWrapper(i32 id)
    {
        LeaveCriticalSection(&this->criticalSections[id]);
    }
};

struct SceneAnmManagerView
{
    void SetInterrupt(AnmVmId id, i32 interrupt);
};

extern SceneAnmManagerView *g_SceneAnmManager;

struct SceneAnmVmIdArray
{
    AnmVmId values[165];

    void SetInterrupt(i32 index, i32 interrupt)
    {
        g_SceneAnmManager->SetInterrupt(this->values[index], interrupt);
    }
};

struct SceneSelectControllerView
{
    u8 unknown0000[0x20];
    i32 selectedGroup;
    u8 unknown0024[0x1ac];
    SceneGroupCursorView groupCursors[12];
    i32 refreshArgument;
    SceneAnmVmIdArray vmIds;
    i8 lockedDisplayState;
    i8 unattemptedDisplayState;
    i8 belowRequirementDisplayState;
    i8 attemptedDisplayState;
    u8 unknown0e8c[6];
    i8 currentDisplayState;
    u8 unknown0e93[0x0d];
    i32 previewTimer;
    u8 unknown0ea4[0x5284];
    SceneValueQueue selectionQueue;
    u8 unknown6170[0x168];
    SceneValueQueue groupPreviewQueue;
    SceneValueQueue scenePreviewQueue;
    u8 unknown6368[0x48];
    SceneStateHistoryView stateHistory;

    void RefreshSceneSelection(i32 unused);
};

struct SceneSaveDataView
{
    u8 unknown0000[0x460];
    SceneScoreEntryView sceneScores[120];

    i32 IsSceneGroupUnlocked(i32 group);
    i32 FindHighestUnlockedSceneGroup();
    i32 CountCapturedScenes();
    i32 CountCapturedScenesInGroup(i32 group);
    i32 GetSceneGroupUnlockScore(i32 group);
};

typedef char SceneScoreEntrySizeIs60[
    (sizeof(SceneScoreEntryView) == 0x60) ? 1 : -1];
typedef char SceneScoreEntryScoreAt10[
    (offsetof(SceneScoreEntryView, score) == 0x10) ? 1 : -1];
typedef char SceneScoreEntryUnlockScoreAt44[
    (offsetof(SceneScoreEntryView, unlockScore) == 0x44) ? 1 : -1];
typedef char SceneScoreEntryFlagsAt50[
    (offsetof(SceneScoreEntryView, flags) == 0x50) ? 1 : -1];
typedef char SceneDefinitionSizeIs30[
    (sizeof(SceneDefinitionView) == 0x30) ? 1 : -1];
typedef char SceneValueQueueSizeIs48[
    (sizeof(SceneValueQueue) == 0x48) ? 1 : -1];
typedef char SceneGroupCursorSizeIsD8[
    (sizeof(SceneGroupCursorView) == 0xd8) ? 1 : -1];
typedef char SceneSaveDataScoresAt460[
    (offsetof(SceneSaveDataView, sceneScores) == 0x460) ? 1 : -1];
typedef char SceneScoreEntryAttemptCountAt3C[
    (offsetof(SceneScoreEntryView, attemptCount) == 0x3c) ? 1 : -1];
typedef char SceneSelectGroupCursorsAt1D0[
    (offsetof(SceneSelectControllerView, groupCursors) == 0x1d0) ? 1 : -1];
typedef char SceneSelectVmIdsAtBF4[
    (offsetof(SceneSelectControllerView, vmIds) == 0xbf4) ? 1 : -1];
typedef char SceneSelectDisplayStatesAtE88[
    (offsetof(SceneSelectControllerView, lockedDisplayState) == 0xe88) ? 1 : -1];
typedef char SceneSelectPreviewTimerAtEA0[
    (offsetof(SceneSelectControllerView, previewTimer) == 0xea0) ? 1 : -1];
typedef char SceneSelectSelectionQueueAt6128[
    (offsetof(SceneSelectControllerView, selectionQueue) == 0x6128) ? 1 : -1];
typedef char SceneSelectGroupPreviewQueueAt62D8[
    (offsetof(SceneSelectControllerView, groupPreviewQueue) == 0x62d8) ? 1 : -1];
typedef char SceneSelectScenePreviewQueueAt6320[
    (offsetof(SceneSelectControllerView, scenePreviewQueue) == 0x6320) ? 1 : -1];
typedef char SceneSelectStateHistoryAt63B0[
    (offsetof(SceneSelectControllerView, stateHistory) == 0x63b0) ? 1 : -1];

extern SceneDefinitionView *g_SceneGroups[12];
extern i32 g_SceneGroupCounts[12];
extern i32 g_SceneUnlockScoreRequirements[12];
extern i32 g_SceneUnlockCaptureRequirements[12];
extern i32 g_SceneUnlockGroupCaptureRequirements[12];
extern SceneSaveDataView *g_SceneSaveData;
extern SceneDefinitionView *g_SelectedScene;
extern SceneSupervisorView g_SceneSupervisor;

} // namespace th095

#endif
