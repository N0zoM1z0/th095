#pragma once

#include "inttypes.hpp"
#include <stddef.h>

namespace th095
{

typedef const u8 *SceneEncodedText;

// Static scene resources and the fields populated from mission.msg.
struct SceneDefinitionView
{
    i32 scoreEntryIndex;
    i32 group;
    i32 scene;
    const char *stageDataPath;
    char *enemyAnmPath;
    char *enemyEclPath;
    char *musicPath;
    i32 frontScriptIndex;
    i8 groupPreviewAssetSelector;
    i8 scenePreviewAssetSelector;
    u8 unknown022[2];
    i32 scoreRequirement;
    SceneEncodedText encodedTitleText;
    i8 displayState;
    u8 unknown02d[3];
};

typedef char SceneDefinitionSizeIs30[
    (sizeof(SceneDefinitionView) == 0x30) ? 1 : -1];
typedef char SceneDefinitionGroupAndSceneAt04[
    (offsetof(SceneDefinitionView, group) == 0x04 &&
     offsetof(SceneDefinitionView, scene) == 0x08) ? 1 : -1];
typedef char SceneDefinitionPathsAt0C[
    (offsetof(SceneDefinitionView, stageDataPath) == 0x0c &&
     offsetof(SceneDefinitionView, musicPath) == 0x18) ? 1 : -1];
typedef char SceneDefinitionPreviewSelectorsAt20[
    (offsetof(SceneDefinitionView, groupPreviewAssetSelector) == 0x20 &&
     offsetof(SceneDefinitionView, scenePreviewAssetSelector) == 0x21) ? 1 : -1];
typedef char SceneDefinitionMissionFieldsAt24[
    (offsetof(SceneDefinitionView, scoreRequirement) == 0x24 &&
     offsetof(SceneDefinitionView, encodedTitleText) == 0x28 &&
     offsetof(SceneDefinitionView, displayState) == 0x2c) ? 1 : -1];

extern SceneDefinitionView *g_SceneGroups[12];
extern i32 g_SceneGroupCounts[12];
extern i32 g_SceneUnlockScoreRequirements[12];
extern i32 g_SceneUnlockCaptureRequirements[12];
extern i32 g_SceneUnlockGroupCaptureRequirements[12];
extern SceneDefinitionView *g_SelectedScene;

} // namespace th095
