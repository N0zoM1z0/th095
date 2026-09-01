#include "SceneSelect.hpp"
#include "ReplayManager.hpp"
#include "ZunMath.hpp"

#include <stdio.h>
#include <string.h>
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

// The target materializes each replay page/selection read through a distinct
// value home. Keeping the real member read inside this bounded inline helper
// preserves those four VC7.1 temporaries without introducing inert storage.
static __forceinline i32 FrontEndDrawSnapshot(i32 value)
{
    return value;
}

// All eight aliases below back live scene-summary locals. Stock VC7.1 orders
// them by identifier hash; these target-proven buckets recover the original
// six Float3 + two scalar physical chronology without padding.
#define totalScorePosition restartCommandProcessingLocal05
#define capturedPosition averagedPanLocal12
#define highScorePosition iLocal11
#define slowRatePosition commandCursorLocal02
#define successRatePosition soundIndexLocal01
#define markerPosition jLocal00
#define totalScore preloadBufferLocal03
#define i bufferLocal04
static __forceinline void FrontEndDrawSceneSummary(
    FrontEndControllerDrawView *view)
{
    Float3 totalScorePosition;
    Float3 capturedPosition;
    Float3 highScorePosition;
    Float3 slowRatePosition;
    Float3 successRatePosition;
    Float3 markerPosition;
    u32 i;
    i32 totalScore;
        totalScore = 0;
        for (i = 0; i < 120; i++)
        {
            totalScore += g_SceneSaveData->sceneScores[i].score;
        }

        totalScorePosition.x = 516.0f;
        totalScorePosition.y = 34.0f;
        totalScorePosition.z = 0.0f;
        g_FrontEndAsciiManager.AddFormatText(
            &totalScorePosition, "%.8d\n", totalScore);

        capturedPosition.x = 468.0f;
        capturedPosition.y = 48.0f;
        capturedPosition.z = 0.0f;
        g_FrontEndAsciiManager.AddFormatText(
            &capturedPosition, "%2d Scene Success\n",
            g_SceneSaveData->CountCapturedScenes());

        highScorePosition.x = 242.0f;
        highScorePosition.y = 332.0f;
        highScorePosition.z = 0.0f;
        g_FrontEndAsciiManager.AddFormatText(
            &highScorePosition, "High Score %.6d", g_SceneSaveData->sceneScores[view->selectedScoreEntry].score);

        g_FrontEndAsciiManager.color = 0xff80d0d0;
        slowRatePosition.x = 242.0f;
        slowRatePosition.y = 346.0f;
        slowRatePosition.z = 0.0f;
        g_FrontEndAsciiManager.AddFormatText(
            &slowRatePosition, "Slow Rate  %2.0f%%", g_SceneSaveData->sceneScores[view->selectedScoreEntry].slowRate);
        g_FrontEndAsciiManager.color = 0xffffffff;

        if (view->showSuccessRate)
        {
            g_FrontEndAsciiManager.color = 0xffc0e0e0;
            g_FrontEndAsciiManager.scaleX = 0.75f;
            g_FrontEndAsciiManager.scaleY = 0.75f;
            successRatePosition.x = 382.0f;
            successRatePosition.y = 285.0f;
            successRatePosition.z = 0.0f;
            g_FrontEndAsciiManager.AddFormatText(
                &successRatePosition, "%2.0f%%", g_SceneSaveData->sceneScores[view->selectedScoreEntry].successRate);
            g_FrontEndAsciiManager.scaleX = 1.0f;
            g_FrontEndAsciiManager.scaleY = 1.0f;
            g_FrontEndAsciiManager.color = 0xffffffff;
        }

        if (g_SceneSaveData->sceneScores[view->selectedScoreEntry].showSuccessRateMarker)
        {
            g_FrontEndAsciiManager.color = 0xffc0e0e0;
            g_FrontEndAsciiManager.scaleX = 0.75f;
            g_FrontEndAsciiManager.scaleY = 0.75f;
            markerPosition.x = 435.0f;
            markerPosition.y = 124.0f;
            markerPosition.z = 0.0f;
            g_FrontEndAsciiManager.AddFormatText(
                &markerPosition, "L", g_SceneSaveData->sceneScores[view->selectedScoreEntry].successRate);
            g_FrontEndAsciiManager.scaleX = 1.0f;
            g_FrontEndAsciiManager.scaleY = 1.0f;
            g_FrontEndAsciiManager.color = 0xffffffff;
        }

}

#undef totalScorePosition
#undef capturedPosition
#undef highScorePosition
#undef slowRatePosition
#undef successRatePosition
#undef markerPosition
#undef totalScore
#undef i

// The replay renderer reuses one scene, user-id, and level buffer across both
// mutually exclusive formatting branches. These four live locals use the same
// proven hash buckets to reproduce the target's shallow replay-frame order.
#define sceneText restartCommandProcessingLocal05
#define userId averagedPanLocal12
#define levelText iLocal11
#define position commandCursorLocal02
ChainCallbackResult SceneSelectControllerView::Draw()
{
#define view (reinterpret_cast<FrontEndControllerDrawView *>(this))
    switch (view->requestedState)
    {
    case 2:
        FrontEndDrawSceneSummary(view);
        break;
    case 3:
    {
        char sceneText[8];
        char userId[5];
        char levelText[8];
        Float3 position(104.0f, 88.0f, 0.0f);

        g_SceneSupervisor.EnterCriticalSectionWrapper(4);
        g_FrontEndCriticalSectionDepth++;
        for (i32 replayIndex = FrontEndDrawSnapshot(view->replayPage) * 20;
             replayIndex < FrontEndDrawSnapshot(view->replayPage) * 20 + 20; replayIndex++)
        {
            if (FrontEndDrawSnapshot(view->replaySelection) == replayIndex % 20)
            {
                g_FrontEndAsciiManager.color = 0xffffffff;
            }
            else
            {
                g_FrontEndAsciiManager.color = 0xff404040;
            }

            if (FrontEndDrawSnapshot(view->replayPage) == 0)
            {
                if (view->replays[replayIndex] == NULL || view->replays[replayIndex]->activeInputData == NULL)
                {
                    g_FrontEndAsciiManager.AddFormatText(
                        &position,
                        "No.%.2d --------  *-* --/--/-- --:-- ------ --%%",
                        replayIndex + 1);
                }
                else
                {
                    tm *timestamp = localtime(
                        reinterpret_cast<time_t *>(&view->replays[replayIndex]->activeInputData->timestamp));


                    if (view->replays[replayIndex]->activeInputData->level == 10)
                    {
                        strcpy(levelText, "EX");
                    }
                    else
                    {
                        sprintf(levelText, "%2d", view->replays[replayIndex]->activeInputData->level + 1);
                    }
                    sprintf(sceneText, "%d", view->replays[replayIndex]->activeInputData->scene + 1);
                    g_FrontEndAsciiManager.AddFormatText(
                        &position,
                        "No.%.2d %s %s-%s %.2d/%.2d/%.2d %.2d:%.2d %6d %2.0f%%",
                        replayIndex + 1, view->replays[replayIndex]->activeInputData->replayName, levelText,
                        sceneText, timestamp->tm_year % 100,
                        timestamp->tm_mon + 1, timestamp->tm_mday,
                        timestamp->tm_hour, timestamp->tm_min, view->replays[replayIndex]->activeInputData->score,
                        view->replays[replayIndex]->activeInputData->slowRate);
                }
            }
            else if (view->replays[replayIndex] == NULL || view->replays[replayIndex]->activeInputData == NULL)
            {
                g_FrontEndAsciiManager.AddFormatText(
                    &position,
                    "User---- --------  *-* --/--/-- --:-- ------ --.-%%");
            }
            else
            {
                tm *timestamp = localtime(
                    reinterpret_cast<time_t *>(&view->replays[replayIndex]->activeInputData->timestamp));


                if (view->replays[replayIndex]->activeInputData->level == 10)
                {
                    strcpy(levelText, "EX");
                }
                else
                {
                    sprintf(levelText, "%2d", view->replays[replayIndex]->activeInputData->level + 1);
                }
                sprintf(sceneText, "%d", view->replays[replayIndex]->activeInputData->scene + 1);
                *reinterpret_cast<u32 *>(userId) =
                    *reinterpret_cast<u32 *>(&view->replays[replayIndex]->path[7]);
                userId[4] = '\0';
                g_FrontEndAsciiManager.AddFormatText(
                    &position,
                    "User%.4s %s %s-%s %.2d/%.2d/%.2d %.2d:%.2d %6d %2.0f%%",
                    userId, view->replays[replayIndex]->activeInputData->replayName, levelText, sceneText,
                    timestamp->tm_year % 100, timestamp->tm_mon + 1,
                    timestamp->tm_mday, timestamp->tm_hour,
                    timestamp->tm_min, view->replays[replayIndex]->activeInputData->score, view->replays[replayIndex]->activeInputData->slowRate);
            }
            position.y += 18.0f;
        }
        g_SceneSupervisor.LeaveCriticalSectionWrapper(4);
        g_FrontEndCriticalSectionDepth--;
        g_FrontEndAsciiManager.color = 0xffffffff;
    }
        break;
    }

#undef view
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

#undef sceneText
#undef userId
#undef levelText
#undef position

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
