#include "SceneSelect.hpp"
#include "ReplayManager.hpp"
#include "ZunMath.hpp"

#include <stdio.h>
#include <time.h>

namespace th095
{

struct FrontEndAsciiManagerView
{
    u8 unknown0000[0x806c];
    u32 color;
    f32 scaleX;
    f32 scaleY;

    void AddFormatText(Float3 *position, const char *format, ...);
};

struct FrontEndControllerDrawView
{
    u8 unknown0000[0x20];
    i32 replayPage;
    u8 unknown0024[0xd4];
    i32 replaySelection;
    u8 unknown00fc[0xaf4];
    i32 selectedScoreEntry;
    u8 unknown0bf4[0x2b4];
    ReplayManager *replays[80];
    u8 unknown0fe8[0x5128];
    i32 requestedState;
    u8 unknown6114[0x0c];
    union
    {
        u32 flags;
        struct
        {
            u32 unknownFlags0 : 3;
            u32 showSuccessRate : 1;
            u32 unknownFlags4 : 28;
        };
    };
};

typedef char FrontEndAsciiColorAt806C[
    (offsetof(FrontEndAsciiManagerView, color) == 0x806c) ? 1 : -1];
typedef char FrontEndDrawReplayPageAt20[
    (offsetof(FrontEndControllerDrawView, replayPage) == 0x20) ? 1 : -1];
typedef char FrontEndDrawReplaySelectionAtF8[
    (offsetof(FrontEndControllerDrawView, replaySelection) == 0xf8) ? 1 : -1];
typedef char FrontEndDrawSelectedScoreAtBF0[
    (offsetof(FrontEndControllerDrawView, selectedScoreEntry) == 0xbf0)
        ? 1
        : -1];
typedef char FrontEndDrawReplaysAtEA8[
    (offsetof(FrontEndControllerDrawView, replays) == 0xea8) ? 1 : -1];
typedef char FrontEndDrawRequestedStateAt6110[
    (offsetof(FrontEndControllerDrawView, requestedState) == 0x6110)
        ? 1
        : -1];
typedef char FrontEndDrawFlagsAt6120[
    (offsetof(FrontEndControllerDrawView, flags) == 0x6120) ? 1 : -1];

extern FrontEndAsciiManagerView g_FrontEndAsciiManager;
extern u8 g_FrontEndCriticalSectionDepth;

ChainCallbackResult SceneSelectControllerView::Draw()
{
    FrontEndControllerDrawView *view =
        reinterpret_cast<FrontEndControllerDrawView *>(this);

    if (view->requestedState == 2)
    {
        i32 totalScore = 0;
        for (u32 i = 0; i < 120; i++)
        {
            totalScore += g_SceneSaveData->sceneScores[i].score;
        }

        Float3 totalScorePosition(516.0f, 34.0f, 0.0f);
        g_FrontEndAsciiManager.AddFormatText(
            &totalScorePosition, "%.8d\n", totalScore);

        Float3 capturedPosition(468.0f, 48.0f, 0.0f);
        g_FrontEndAsciiManager.AddFormatText(
            &capturedPosition, "%2d Scene Success\n",
            g_SceneSaveData->CountCapturedScenes());

        SceneScoreEntryView *score =
            &g_SceneSaveData->sceneScores[view->selectedScoreEntry];
        Float3 highScorePosition(242.0f, 332.0f, 0.0f);
        g_FrontEndAsciiManager.AddFormatText(
            &highScorePosition, "High Score %.6d", score->score);

        g_FrontEndAsciiManager.color = 0xff80d0d0;
        Float3 slowRatePosition(242.0f, 346.0f, 0.0f);
        g_FrontEndAsciiManager.AddFormatText(
            &slowRatePosition, "Slow Rate  %2.0f%%", score->slowRate);
        g_FrontEndAsciiManager.color = 0xffffffff;

        if (view->showSuccessRate)
        {
            g_FrontEndAsciiManager.color = 0xffc0e0e0;
            g_FrontEndAsciiManager.scaleX = 0.75f;
            g_FrontEndAsciiManager.scaleY = 0.75f;
            Float3 successRatePosition(382.0f, 285.0f, 0.0f);
            g_FrontEndAsciiManager.AddFormatText(
                &successRatePosition, "%2.0f%%", score->successRate);
            g_FrontEndAsciiManager.scaleX = 1.0f;
            g_FrontEndAsciiManager.scaleY = 1.0f;
            g_FrontEndAsciiManager.color = 0xffffffff;
        }

        if (score->showSuccessRateMarker)
        {
            g_FrontEndAsciiManager.color = 0xffc0e0e0;
            g_FrontEndAsciiManager.scaleX = 0.75f;
            g_FrontEndAsciiManager.scaleY = 0.75f;
            Float3 markerPosition(435.0f, 124.0f, 0.0f);
            g_FrontEndAsciiManager.AddFormatText(
                &markerPosition, "L", score->successRate);
            g_FrontEndAsciiManager.scaleX = 1.0f;
            g_FrontEndAsciiManager.scaleY = 1.0f;
            g_FrontEndAsciiManager.color = 0xffffffff;
        }
    }
    else if (view->requestedState == 3)
    {
        Float3 position(104.0f, 88.0f, 0.0f);

        g_SceneSupervisor.EnterCriticalSectionWrapper(4);
        g_FrontEndCriticalSectionDepth++;
        for (i32 replayIndex = view->replayPage * 20;
             replayIndex < view->replayPage * 20 + 20; replayIndex++)
        {
            if (view->replaySelection == replayIndex % 20)
            {
                g_FrontEndAsciiManager.color = 0xffffffff;
            }
            else
            {
                g_FrontEndAsciiManager.color = 0xff404040;
            }

            ReplayManager *replay = view->replays[replayIndex];
            if (view->replayPage == 0)
            {
                if (replay == NULL || replay->activeInputData == NULL)
                {
                    g_FrontEndAsciiManager.AddFormatText(
                        &position,
                        "No.%.2d --------  *-* --/--/-- --:-- ------ --%%",
                        replayIndex + 1);
                }
                else
                {
                    ReplayInputData *input = replay->activeInputData;
                    tm *timestamp = localtime(
                        reinterpret_cast<time_t *>(&input->timestamp));
                    char levelText[3];
                    char sceneText[8];
                    if (input->level == 10)
                    {
                        levelText[0] = 'E';
                        levelText[1] = 'X';
                        levelText[2] = '\0';
                    }
                    else
                    {
                        sprintf(levelText, "%2d", input->level + 1);
                    }
                    sprintf(sceneText, "%d", input->scene + 1);
                    g_FrontEndAsciiManager.AddFormatText(
                        &position,
                        "No.%.2d %s %s-%s %.2d/%.2d/%.2d %.2d:%.2d %6d %2.0f%%",
                        replayIndex + 1, input->replayName, levelText,
                        sceneText, timestamp->tm_year % 100,
                        timestamp->tm_mon + 1, timestamp->tm_mday,
                        timestamp->tm_hour, timestamp->tm_min, input->score,
                        input->slowRate);
                }
            }
            else if (replay == NULL || replay->activeInputData == NULL)
            {
                g_FrontEndAsciiManager.AddFormatText(
                    &position,
                    "User---- --------  *-* --/--/-- --:-- ------ --.-%%");
            }
            else
            {
                ReplayInputData *input = replay->activeInputData;
                tm *timestamp = localtime(
                    reinterpret_cast<time_t *>(&input->timestamp));
                char levelText[3];
                char sceneText[8];
                char userId[5];
                if (input->level == 10)
                {
                    levelText[0] = 'E';
                    levelText[1] = 'X';
                    levelText[2] = '\0';
                }
                else
                {
                    sprintf(levelText, "%2d", input->level + 1);
                }
                sprintf(sceneText, "%d", input->scene + 1);
                *reinterpret_cast<u32 *>(userId) =
                    *reinterpret_cast<u32 *>(&replay->path[7]);
                userId[4] = '\0';
                g_FrontEndAsciiManager.AddFormatText(
                    &position,
                    "User%.4s %s %s-%s %.2d/%.2d/%.2d %.2d:%.2d %6d %2.0f%%",
                    userId, input->replayName, levelText, sceneText,
                    timestamp->tm_year % 100, timestamp->tm_mon + 1,
                    timestamp->tm_mday, timestamp->tm_hour,
                    timestamp->tm_min, input->score, input->slowRate);
            }
            position.y += 18.0f;
        }
        g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
        g_FrontEndCriticalSectionDepth--;
        g_FrontEndAsciiManager.color = 0xffffffff;
    }

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

void __fastcall SceneSelectControllerView::OnDraw(
    SceneSelectControllerView *controller)
{
    controller->Draw();
}

void __fastcall SceneSelectControllerView::OnUpdate(
    SceneSelectControllerView *controller)
{
    controller->Update();
}

} // namespace th095
