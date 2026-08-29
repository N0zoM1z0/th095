#ifndef TH095_SCENE_SELECT_HPP
#define TH095_SCENE_SELECT_HPP

#include "Global.hpp"

namespace th095
{

struct SceneScoreEntryView
{
    u8 unknown000[0x10];
    i32 score;
    u8 unknown014[0x30];
    i32 unlockScore;
    u8 unknown048[8];
    u32 flags;
    u8 unknown054[0x0c];
};

struct SceneDefinitionView
{
    i32 scoreEntryIndex;
    u8 unknown004[0x2c];
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
typedef char SceneSaveDataScoresAt460[
    (offsetof(SceneSaveDataView, sceneScores) == 0x460) ? 1 : -1];

extern SceneDefinitionView *g_SceneGroups[12];
extern i32 g_SceneGroupCounts[12];
extern i32 g_SceneUnlockScoreRequirements[12];
extern i32 g_SceneUnlockCaptureRequirements[12];
extern i32 g_SceneUnlockGroupCaptureRequirements[12];

} // namespace th095

#endif
