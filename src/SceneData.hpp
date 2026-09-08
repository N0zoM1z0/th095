#pragma once

#include "inttypes.hpp"

namespace th095
{

// One 0x30 scene record is shared by replay, result, background and photo
// subsystems.  The names in the unions document the role each subsystem gives
// the same target-observed offset; they are not separate address-bound views.
struct SceneDefinitionView
{
    union
    {
        i32 scoreEntryIndex;
        i32 bestShotIndex;
        struct
        {
            u16 id;
            u16 unknownIdHigh;
        };
    };
    union
    {
        i32 titleArgument1;
        i32 group;
        i32 level;
    };
    union
    {
        i32 titleArgument2;
        i32 scene;
        i32 variant;
    };
    union
    {
        u8 unknown00c[4];
        const char *stageDataPath;
    };
    char *enemyAnmPath;
    char *enemyEclPath;
    union
    {
        u8 unknown018[4];
        char *musicPath;
    };
    union
    {
        u8 unknown01c[4];
        i32 frontScriptIndex;
    };
    i8 groupDisplayValue;
    i8 sceneDisplayValue;
    u8 unknown022[2];
    union
    {
        i32 scoreRequirement;
        i32 textId;
    };
    union
    {
        i32 titleTextId;
        char *text;
    };
    i8 displayState;
    u8 unknown02d[3];
};

typedef char SceneDefinitionSizeIs30[
    (sizeof(SceneDefinitionView) == 0x30) ? 1 : -1];

extern SceneDefinitionView *g_SceneGroups[12];
extern i32 g_SceneGroupCounts[12];
extern i32 g_SceneUnlockScoreRequirements[12];
extern i32 g_SceneUnlockCaptureRequirements[12];
extern i32 g_SceneUnlockGroupCaptureRequirements[12];
extern SceneDefinitionView *g_SelectedScene;

} // namespace th095
