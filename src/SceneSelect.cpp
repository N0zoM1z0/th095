#include "SceneSelect.hpp"

namespace th095
{

i32 SceneSaveDataView::IsSceneGroupUnlocked(i32 group)
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
              (this->GetSceneGroupUnlockScore(group - 1) < 10000)) &&
             (this->CountCapturedScenes() <
              g_SceneUnlockCaptureRequirements[group])) &&
            (this->CountCapturedScenesInGroup(group - 1) <
             g_SceneUnlockGroupCaptureRequirements[group]))
               ? 0
               : 1;
}

i32 SceneSaveDataView::FindHighestUnlockedSceneGroup()
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

i32 SceneSaveDataView::CountCapturedScenes()
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

i32 SceneSaveDataView::CountCapturedScenesInGroup(i32 group)
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

i32 SceneSaveDataView::GetSceneGroupUnlockScore(i32 group)
{
    i32 score;

    score = 0;
    for (i32 scene = 0; scene < g_SceneGroupCounts[group]; scene++)
    {
        score += this->sceneScores[
            g_SceneGroups[group][scene].scoreEntryIndex].unlockScore;
    }
    return score;
}

} // namespace th095
