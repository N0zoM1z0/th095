#ifdef TH095_MATCH_EXACT
#include "SceneSelectExact.inl"
#else
#include "SceneSelect.hpp"

namespace th095
{

// The target stores all 93 scene definitions contiguously at
// 0x004A4690..0x004A57FF, followed by the twelve group pointers and counts at
// 0x004A5800/0x004A5830.  mission.msg fills textId, text, and displayState at
// runtime; the remaining fields below are the target-observed static catalog.
// Keep a plain storage view here because SceneDefinitionView exposes several
// offsets through unions whose first aggregate member is not always the
// pointer-shaped interpretation needed by a static initializer.
struct SceneDefinitionStorage
{
    i32 scoreEntryIndex;
    i32 group;
    i32 scene;
    const char *stageDataPath;
    const char *enemyAnmPath;
    const char *enemyEclPath;
    const char *musicPath;
    i32 frontScriptIndex;
    i8 groupDisplayValue;
    i8 sceneDisplayValue;
    u8 unknown022[2];
    i32 textId;
    char *text;
    i8 displayState;
    u8 unknown02d[3];
};

typedef char SceneDefinitionStorageSizeIs30[
    (sizeof(SceneDefinitionStorage) == sizeof(SceneDefinitionView)) ? 1 : -1];
typedef char SceneDefinitionStoragePathsAt0C[
    (offsetof(SceneDefinitionStorage, stageDataPath) == 0x0c &&
     offsetof(SceneDefinitionStorage, musicPath) == 0x18)
        ? 1
        : -1];
typedef char SceneDefinitionStorageRuntimeFieldsAt24[
    (offsetof(SceneDefinitionStorage, textId) == 0x24 &&
     offsetof(SceneDefinitionStorage, text) == 0x28 &&
     offsetof(SceneDefinitionStorage, displayState) == 0x2c)
        ? 1
        : -1];

#define TH095_SCENE(scoreIndex, groupIndex, sceneIndex, stagePath, anmPath,   \
                    eclPath, bgmPath, frontScript, groupDisplay,             \
                    sceneDisplay)                                            \
    {                                                                         \
        scoreIndex, groupIndex, sceneIndex, stagePath, anmPath, eclPath,      \
            bgmPath, frontScript, groupDisplay, sceneDisplay, {0, 0}, 0, 0,   \
            0, {0, 0, 0}                                                     \
    }

static SceneDefinitionStorage g_SceneDefinitions[93] = {
    // Scene group 0: 6 scene(s).
    TH095_SCENE(0, 0, 0, "world01.std", "enm1.anm", "ecl1_a.ecl", "bgm/th095_01.wav", 0, 1, 3),
    TH095_SCENE(1, 0, 1, "world01.std", "enm2.anm", "ecl2_a.ecl", "bgm/th095_01.wav", 0, 2, 3),
    TH095_SCENE(2, 0, 2, "world01.std", "enm1.anm", "ecl1_b.ecl", "bgm/th095_01.wav", 0, 1, 5),
    TH095_SCENE(3, 0, 3, "world01.std", "enm2.anm", "ecl2_b.ecl", "bgm/th095_01.wav", 0, 2, 4),
    TH095_SCENE(4, 0, 4, "world01.std", "enm1.anm", "ecl1_c.ecl", "bgm/th095_01.wav", 0, 1, 5),
    TH095_SCENE(5, 0, 5, "world01.std", "enm2.anm", "ecl2_c.ecl", "bgm/th095_01.wav", 0, 2, 5),
    // Scene group 1: 6 scene(s).
    TH095_SCENE(10, 1, 0, "world02.std", "enm3.anm", "ecl3_a.ecl", "bgm/th095_01.wav", 0, 3, 5),
    TH095_SCENE(11, 1, 1, "world02.std", "enm4.anm", "ecl4_a.ecl", "bgm/th095_01.wav", 0, 4, 4),
    TH095_SCENE(12, 1, 2, "world02.std", "enm3.anm", "ecl3_b.ecl", "bgm/th095_01.wav", 0, 3, 5),
    TH095_SCENE(13, 1, 3, "world02.std", "enm4.anm", "ecl4_b.ecl", "bgm/th095_01.wav", 0, 4, 4),
    TH095_SCENE(14, 1, 4, "world02.std", "enm3.anm", "ecl3_c.ecl", "bgm/th095_01.wav", 0, 3, 5),
    TH095_SCENE(15, 1, 5, "world02.std", "enm4.anm", "ecl4_c.ecl", "bgm/th095_01.wav", 0, 4, 5),
    // Scene group 2: 8 scene(s).
    TH095_SCENE(20, 2, 0, "world03.std", "enm6.anm", "ecl6_a.ecl", "bgm/th095_02.wav", 1, 6, 5),
    TH095_SCENE(21, 2, 1, "world03.std", "enm5.anm", "ecl5_a.ecl", "bgm/th095_02.wav", 1, 5, 5),
    TH095_SCENE(22, 2, 2, "world03.std", "enm6.anm", "ecl6_b.ecl", "bgm/th095_02.wav", 1, 6, 5),
    TH095_SCENE(23, 2, 3, "world03.std", "enm5.anm", "ecl5_b.ecl", "bgm/th095_02.wav", 1, 5, 6),
    TH095_SCENE(24, 2, 4, "world03.std", "enm6.anm", "ecl6_c.ecl", "bgm/th095_02.wav", 1, 6, 5),
    TH095_SCENE(25, 2, 5, "world03.std", "enm5.anm", "ecl5_c.ecl", "bgm/th095_02.wav", 1, 5, 7),
    TH095_SCENE(26, 2, 6, "world03.std", "enm6.anm", "ecl6_d.ecl", "bgm/th095_02.wav", 1, 6, 5),
    TH095_SCENE(27, 2, 7, "world03.std", "enm5.anm", "ecl5_d.ecl", "bgm/th095_02.wav", 1, 5, 4),
    // Scene group 3: 9 scene(s).
    TH095_SCENE(30, 3, 0, "world04.std", "enm7.anm", "ecl7_a.ecl", "bgm/th095_02.wav", 1, 7, 4),
    TH095_SCENE(31, 3, 1, "world04.std", "enm9.anm", "ecl9_a.ecl", "bgm/th095_02.wav", 1, 9, 6),
    TH095_SCENE(32, 3, 2, "world04.std", "enm8.anm", "ecl8_a.ecl", "bgm/th095_02.wav", 1, 8, 5),
    TH095_SCENE(33, 3, 3, "world04.std", "enm7.anm", "ecl7_b.ecl", "bgm/th095_02.wav", 1, 7, 6),
    TH095_SCENE(34, 3, 4, "world04.std", "enm9.anm", "ecl9_b.ecl", "bgm/th095_02.wav", 1, 9, 5),
    TH095_SCENE(35, 3, 5, "world04.std", "enm7.anm", "ecl7_c.ecl", "bgm/th095_02.wav", 1, 7, 6),
    TH095_SCENE(36, 3, 6, "world04.std", "enm9.anm", "ecl9_c.ecl", "bgm/th095_02.wav", 1, 9, 6),
    TH095_SCENE(37, 3, 7, "world04.std", "enm8.anm", "ecl8_b.ecl", "bgm/th095_02.wav", 1, 8, 6),
    TH095_SCENE(38, 3, 8, "world04.std", "enm7.anm", "ecl7_d.ecl", "bgm/th095_02.wav", 1, 7, 5),
    // Scene group 4: 8 scene(s).
    TH095_SCENE(40, 4, 0, "world05.std", "enm10.anm", "ecl10_a.ecl", "bgm/th095_02.wav", 1, 10, 5),
    TH095_SCENE(41, 4, 1, "world05.std", "enm11.anm", "ecl11_a.ecl", "bgm/th095_02.wav", 1, 11, 5),
    TH095_SCENE(42, 4, 2, "world05.std", "enm10.anm", "ecl10_b.ecl", "bgm/th095_02.wav", 1, 10, 5),
    TH095_SCENE(43, 4, 3, "world05.std", "enm11.anm", "ecl11_b.ecl", "bgm/th095_02.wav", 1, 11, 5),
    TH095_SCENE(44, 4, 4, "world05.std", "enm10.anm", "ecl10_c.ecl", "bgm/th095_02.wav", 1, 10, 6),
    TH095_SCENE(45, 4, 5, "world05.std", "enm11.anm", "ecl11_c.ecl", "bgm/th095_02.wav", 1, 11, 5),
    TH095_SCENE(46, 4, 6, "world05.std", "enm10.anm", "ecl10_d.ecl", "bgm/th095_02.wav", 1, 10, 7),
    TH095_SCENE(47, 4, 7, "world05.std", "enm11.anm", "ecl11_d.ecl", "bgm/th095_02.wav", 1, 11, 6),
    // Scene group 5: 8 scene(s).
    TH095_SCENE(50, 5, 0, "world06.std", "enm12.anm", "ecl12_a.ecl", "bgm/th095_03.wav", 2, 12, 8),
    TH095_SCENE(51, 5, 1, "world06.std", "enm13.anm", "ecl13_a.ecl", "bgm/th095_03.wav", 2, 13, 4),
    TH095_SCENE(52, 5, 2, "world06.std", "enm12.anm", "ecl12_b.ecl", "bgm/th095_03.wav", 2, 12, 5),
    TH095_SCENE(53, 5, 3, "world06.std", "enm13.anm", "ecl13_b.ecl", "bgm/th095_03.wav", 2, 13, 3),
    TH095_SCENE(54, 5, 4, "world06.std", "enm12.anm", "ecl12_c.ecl", "bgm/th095_03.wav", 2, 12, 7),
    TH095_SCENE(55, 5, 5, "world06.std", "enm13.anm", "ecl13_c.ecl", "bgm/th095_03.wav", 2, 13, 5),
    TH095_SCENE(56, 5, 6, "world06.std", "enm12.anm", "ecl12_d.ecl", "bgm/th095_03.wav", 2, 12, 5),
    TH095_SCENE(57, 5, 7, "world06.std", "enm13.anm", "ecl13_d.ecl", "bgm/th095_03.wav", 2, 13, 5),
    // Scene group 6: 8 scene(s).
    TH095_SCENE(60, 6, 0, "world07.std", "enm14.anm", "ecl14_a.ecl", "bgm/th095_03.wav", 2, 14, 9),
    TH095_SCENE(61, 6, 1, "world07.std", "enm15.anm", "ecl15_a.ecl", "bgm/th095_03.wav", 2, 15, 3),
    TH095_SCENE(62, 6, 2, "world07.std", "enm14.anm", "ecl14_b.ecl", "bgm/th095_03.wav", 2, 14, 5),
    TH095_SCENE(63, 6, 3, "world07.std", "enm15.anm", "ecl15_b.ecl", "bgm/th095_03.wav", 2, 15, 7),
    TH095_SCENE(64, 6, 4, "world07.std", "enm14.anm", "ecl14_c.ecl", "bgm/th095_03.wav", 2, 14, 6),
    TH095_SCENE(65, 6, 5, "world07.std", "enm15.anm", "ecl15_c.ecl", "bgm/th095_03.wav", 2, 15, 7),
    TH095_SCENE(66, 6, 6, "world07.std", "enm14.anm", "ecl14_d.ecl", "bgm/th095_03.wav", 2, 14, 5),
    TH095_SCENE(67, 6, 7, "world07.std", "enm15.anm", "ecl15_d.ecl", "bgm/th095_03.wav", 2, 15, 7),
    // Scene group 7: 8 scene(s).
    TH095_SCENE(70, 7, 0, "world08.std", "enm16.anm", "ecl16_a.ecl", "bgm/th095_04.wav", 3, 16, 6),
    TH095_SCENE(71, 7, 1, "world08.std", "enm17.anm", "ecl17_a.ecl", "bgm/th095_04.wav", 3, 17, 7),
    TH095_SCENE(72, 7, 2, "world08.std", "enm16.anm", "ecl16_b.ecl", "bgm/th095_04.wav", 3, 16, 6),
    TH095_SCENE(73, 7, 3, "world08.std", "enm17.anm", "ecl17_b.ecl", "bgm/th095_04.wav", 3, 17, 7),
    TH095_SCENE(74, 7, 4, "world08.std", "enm16.anm", "ecl16_c.ecl", "bgm/th095_04.wav", 3, 16, 7),
    TH095_SCENE(75, 7, 5, "world08.std", "enm17.anm", "ecl17_c.ecl", "bgm/th095_04.wav", 3, 17, 10),
    TH095_SCENE(76, 7, 6, "world08.std", "enm16.anm", "ecl16_d.ecl", "bgm/th095_04.wav", 3, 16, 8),
    TH095_SCENE(77, 7, 7, "world08.std", "enm17.anm", "ecl17_d.ecl", "bgm/th095_04.wav", 3, 17, 4),
    // Scene group 8: 8 scene(s).
    TH095_SCENE(80, 8, 0, "world09.std", "enm18.anm", "ecl18_a.ecl", "bgm/th095_04.wav", 3, 18, 6),
    TH095_SCENE(81, 8, 1, "world09.std", "enm19.anm", "ecl19_a.ecl", "bgm/th095_04.wav", 3, 19, 6),
    TH095_SCENE(82, 8, 2, "world09.std", "enm18.anm", "ecl18_b.ecl", "bgm/th095_04.wav", 3, 18, 7),
    TH095_SCENE(83, 8, 3, "world09.std", "enm19.anm", "ecl19_b.ecl", "bgm/th095_04.wav", 3, 19, 6),
    TH095_SCENE(84, 8, 4, "world09.std", "enm18.anm", "ecl18_c.ecl", "bgm/th095_04.wav", 3, 18, 6),
    TH095_SCENE(85, 8, 5, "world09.std", "enm19.anm", "ecl19_c.ecl", "bgm/th095_04.wav", 3, 19, 7),
    TH095_SCENE(86, 8, 6, "world09.std", "enm18.anm", "ecl18_d.ecl", "bgm/th095_04.wav", 3, 18, 8),
    TH095_SCENE(87, 8, 7, "world09.std", "enm19.anm", "ecl19_d.ecl", "bgm/th095_04.wav", 3, 19, 7),
    // Scene group 9: 8 scene(s).
    TH095_SCENE(90, 9, 0, "world10.std", "enm20.anm", "ecl20_a.ecl", "bgm/th095_04.wav", 3, 20, 8),
    TH095_SCENE(91, 9, 1, "world10.std", "enm21.anm", "ecl21_a.ecl", "bgm/th095_04.wav", 3, 21, 6),
    TH095_SCENE(92, 9, 2, "world10.std", "enm20.anm", "ecl20_b.ecl", "bgm/th095_04.wav", 3, 20, 6),
    TH095_SCENE(93, 9, 3, "world10.std", "enm21.anm", "ecl21_b.ecl", "bgm/th095_04.wav", 3, 21, 10),
    TH095_SCENE(94, 9, 4, "world10.std", "enm20.anm", "ecl20_c.ecl", "bgm/th095_04.wav", 3, 20, 7),
    TH095_SCENE(95, 9, 5, "world10.std", "enm21.anm", "ecl21_c.ecl", "bgm/th095_04.wav", 3, 21, 6),
    TH095_SCENE(96, 9, 6, "world10.std", "enm20.anm", "ecl20_d.ecl", "bgm/th095_04.wav", 3, 20, 6),
    TH095_SCENE(97, 9, 7, "world10.std", "enm21.anm", "ecl21_d.ecl", "bgm/th09_08_2.wav", 4, 21, 9),
    // Scene group 10: 8 scene(s).
    TH095_SCENE(100, 10, 0, "world05.std", "enm22.anm", "ecl22_a.ecl", "bgm/th095_03.wav", 2, 22, 6),
    TH095_SCENE(101, 10, 1, "world05.std", "enm22.anm", "ecl22_b.ecl", "bgm/th095_03.wav", 2, 22, 6),
    TH095_SCENE(102, 10, 2, "world06.std", "enm23.anm", "ecl23_a.ecl", "bgm/th095_03.wav", 2, 23, 5),
    TH095_SCENE(103, 10, 3, "world06.std", "enm23.anm", "ecl23_b.ecl", "bgm/th095_03.wav", 2, 23, 6),
    TH095_SCENE(104, 10, 4, "world09.std", "enm24.anm", "ecl24_a.ecl", "bgm/th095_03.wav", 2, 24, 10),
    TH095_SCENE(105, 10, 5, "world09.std", "enm24.anm", "ecl24_b.ecl", "bgm/th095_03.wav", 2, 24, 5),
    TH095_SCENE(106, 10, 6, "world10.std", "enm25.anm", "ecl25_a.ecl", "bgm/th095_03.wav", 2, 25, 3),
    TH095_SCENE(107, 10, 7, "world10.std", "enm25.anm", "ecl25_b.ecl", "bgm/th095_03.wav", 2, 25, 10),
    // Scene group 11: 8 scene(s).
    TH095_SCENE(110, 11, 0, "world05.std", "enm22.anm", "ecl22_a.ecl", "bgm/th095_03.wav", 2, 22, 6),
    TH095_SCENE(111, 11, 1, "world05.std", "enm22.anm", "ecl22_b.ecl", "bgm/th095_03.wav", 2, 22, 6),
    TH095_SCENE(112, 11, 2, "world06.std", "enm23.anm", "ecl23_a.ecl", "bgm/th095_03.wav", 2, 23, 5),
    TH095_SCENE(113, 11, 3, "world06.std", "enm23.anm", "ecl23_b.ecl", "bgm/th095_03.wav", 2, 23, 6),
    TH095_SCENE(114, 11, 4, "world09.std", "enm24.anm", "ecl24_a.ecl", "bgm/th095_03.wav", 2, 24, 10),
    TH095_SCENE(115, 11, 5, "world09.std", "enm24.anm", "ecl24_b.ecl", "bgm/th095_03.wav", 2, 24, 5),
    TH095_SCENE(116, 11, 6, "world10.std", "enm25.anm", "ecl25_a.ecl", "bgm/th095_03.wav", 2, 25, 3),
    TH095_SCENE(117, 11, 7, "world10.std", "enm25.anm", "ecl25_b.ecl", "bgm/th095_03.wav", 2, 25, 10),
};

#undef TH095_SCENE

SceneDefinitionView *g_SceneGroups[12] = {
    reinterpret_cast<SceneDefinitionView *>(&g_SceneDefinitions[0]),
    reinterpret_cast<SceneDefinitionView *>(&g_SceneDefinitions[6]),
    reinterpret_cast<SceneDefinitionView *>(&g_SceneDefinitions[12]),
    reinterpret_cast<SceneDefinitionView *>(&g_SceneDefinitions[20]),
    reinterpret_cast<SceneDefinitionView *>(&g_SceneDefinitions[29]),
    reinterpret_cast<SceneDefinitionView *>(&g_SceneDefinitions[37]),
    reinterpret_cast<SceneDefinitionView *>(&g_SceneDefinitions[45]),
    reinterpret_cast<SceneDefinitionView *>(&g_SceneDefinitions[53]),
    reinterpret_cast<SceneDefinitionView *>(&g_SceneDefinitions[61]),
    reinterpret_cast<SceneDefinitionView *>(&g_SceneDefinitions[69]),
    reinterpret_cast<SceneDefinitionView *>(&g_SceneDefinitions[77]),
    reinterpret_cast<SceneDefinitionView *>(&g_SceneDefinitions[85]),
};
i32 g_SceneGroupCounts[12] = {6, 6, 8, 9, 8, 8, 8, 8, 8, 8, 8, 8};
i32 g_SceneUnlockScoreRequirements[12];
i32 g_SceneUnlockCaptureRequirements[12] = {
    0, 4, 8, 14, 20, 25, 30, 35, 40, 45, 66, 99};
i32 g_SceneUnlockGroupCaptureRequirements[12] = {
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 66, 99};
SceneDefinitionView *g_SelectedScene;

i32 ResultSaveDataView::IsSceneGroupUnlocked(i32 group)
{
    if (group == 0)
    {
        return 1;
    }

    i32 totalScore;
    totalScore = 0;
    for (u32 i = 0; i < 120; i++)
    {
        totalScore += this->sceneScores[i].score;
    }

    return ((((totalScore < g_SceneUnlockScoreRequirements[group]) ||
              (this->GetSceneGroupAttemptCount(group - 1) < 10000)) &&
             (this->CountCapturedScenes() <
              g_SceneUnlockCaptureRequirements[group])) &&
            (this->CountCapturedScenesInGroup(group - 1) <
             g_SceneUnlockGroupCaptureRequirements[group]))
               ? 0
               : 1;
}

i32 ResultSaveDataView::FindHighestUnlockedSceneGroup()
{
    i32 group;

    for (group = 1; group < 12; group++)
    {
        if (this->IsSceneGroupUnlocked(group) == 0)
        {
            break;
        }
    }
    return group - 1;
}

i32 ResultSaveDataView::CountCapturedScenes()
{
    i32 count;

    count = 0;
    for (i32 group = 0; group < 12; group++)
    {
        for (i32 scene = 0; scene < g_SceneGroupCounts[group]; scene++)
        {
            if ((this->sceneScores[
                     g_SceneGroups[group][scene].scoreEntryIndex]
                     .flags & 1) != 0)
            {
                count++;
            }
        }
    }
    return count;
}

i32 ResultSaveDataView::CountCapturedScenesInGroup(i32 group)
{
    i32 count;

    count = 0;
    for (i32 scene = 0; scene < g_SceneGroupCounts[group]; scene++)
    {
        if ((this->sceneScores[g_SceneGroups[group][scene].scoreEntryIndex]
                 .flags & 1) != 0)
        {
            count++;
        }
    }
    return count;
}

i32 ResultSaveDataView::GetSceneGroupAttemptCount(i32 group)
{
    i32 attemptCount;

    attemptCount = 0;
    for (i32 scene = 0; scene < g_SceneGroupCounts[group]; scene++)
    {
        attemptCount += this->sceneScores[
            g_SceneGroups[group][scene].scoreEntryIndex].attemptCount;
    }
    return attemptCount;
}

} // namespace th095

#endif // TH095_MATCH_EXACT
