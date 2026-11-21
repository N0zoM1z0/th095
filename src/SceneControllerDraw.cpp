#include "FrontEndController.hpp"

#ifdef TH095_MATCH_EXACT
#include "SceneControllerDrawExact.inl"
#else

namespace th095
{

ChainCallbackResult __fastcall FrontEndControllerView::OnUpdate(void *argument)
{
    FrontEndControllerView *controller = static_cast<FrontEndControllerView *>(argument);
    // The loading worker sets the failure bit without clearing the pending bit.
    // Update handles that failure before using the title resources.
    if (controller->flagBits.titleLoadIncomplete && !controller->flagBits.titleLoadFailed)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    return controller->Update();
}

} // namespace th095

#endif // TH095_MATCH_EXACT

#include "AsciiManager.hpp"
#include "FrontEndLocalTime.hpp"
#include "Main.hpp"
#include "ReplayManager.hpp"
#include "ScoreData.hpp"
#include "ZunMath.hpp"

#include <stdio.h>
#include <string.h>
#include <time.h>

namespace th095
{

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
#define bestShotSlowRatePosition soundIndexLocal01
#define markerPosition jLocal00
#define totalScore preloadBufferLocal03
#define i bufferLocal04
static __forceinline void FrontEndDrawSceneSummary(
    FrontEndControllerView *view)
{
    Float3 totalScorePosition;
    Float3 capturedPosition;
    Float3 highScorePosition;
    Float3 slowRatePosition;
    Float3 bestShotSlowRatePosition;
    Float3 markerPosition;
    u32 i;
    i32 totalScore;
        totalScore = 0;
        for (i = 0; i < 120; i++)
        {
            totalScore += g_ResultSaveData->scoreEntries[i].score;
        }

        totalScorePosition.x = 516.0f;
        totalScorePosition.y = 34.0f;
        totalScorePosition.z = 0.0f;
        g_AsciiManager.AddFormatText(
            &totalScorePosition, "%.8d\n", totalScore);

        capturedPosition.x = 468.0f;
        capturedPosition.y = 48.0f;
        capturedPosition.z = 0.0f;
        g_AsciiManager.AddFormatText(
            &capturedPosition, "%2d Scene Success\n",
            g_ResultSaveData->CountCapturedScenes());

        highScorePosition.x = 242.0f;
        highScorePosition.y = 332.0f;
        highScorePosition.z = 0.0f;
        g_AsciiManager.AddFormatText(
            &highScorePosition, "High Score %.6d", g_ResultSaveData->scoreEntries[view->selectedScoreEntryIndex].score);

        g_AsciiManager.color.color = 0xff80d0d0;
        slowRatePosition.x = 242.0f;
        slowRatePosition.y = 346.0f;
        slowRatePosition.z = 0.0f;
        g_AsciiManager.AddFormatText(
            &slowRatePosition, "Slow Rate  %2.0f%%", g_ResultSaveData->scoreEntries[view->selectedScoreEntryIndex].highScoreSlowRate);
        g_AsciiManager.color.color = 0xffffffff;

        if (view->flagBits.showRates)
        {
            g_AsciiManager.color.color = 0xffc0e0e0;
            g_AsciiManager.scaleX = 0.75f;
            g_AsciiManager.scaleY = 0.75f;
            bestShotSlowRatePosition.x = 382.0f;
            bestShotSlowRatePosition.y = 285.0f;
            bestShotSlowRatePosition.z = 0.0f;
            g_AsciiManager.AddFormatText(
                &bestShotSlowRatePosition, "%2.0f%%", g_ResultSaveData->scoreEntries[view->selectedScoreEntryIndex].bestShotSlowRate);
            g_AsciiManager.scaleX = 1.0f;
            g_AsciiManager.scaleY = 1.0f;
            g_AsciiManager.color.color = 0xffffffff;
        }

        if (g_ResultSaveData->scoreEntries[view->selectedScoreEntryIndex].flagBits.bestShotLocked)
        {
            g_AsciiManager.color.color = 0xffc0e0e0;
            g_AsciiManager.scaleX = 0.75f;
            g_AsciiManager.scaleY = 0.75f;
            markerPosition.x = 435.0f;
            markerPosition.y = 124.0f;
            markerPosition.z = 0.0f;
            g_AsciiManager.AddFormatText(
                &markerPosition, "L", g_ResultSaveData->scoreEntries[view->selectedScoreEntryIndex].bestShotSlowRate);
            g_AsciiManager.scaleX = 1.0f;
            g_AsciiManager.scaleY = 1.0f;
            g_AsciiManager.color.color = 0xffffffff;
        }

}

#undef totalScorePosition
#undef capturedPosition
#undef highScorePosition
#undef slowRatePosition
#undef bestShotSlowRatePosition
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
ChainCallbackResult FrontEndControllerView::Draw()
{
    switch (this->requestedState)
    {
    case FRONT_END_REQUESTED_STATE_SCENE_SELECT:
        FrontEndDrawSceneSummary(this);
        break;
    case FRONT_END_REQUESTED_STATE_REPLAY_BROWSER:
    {
        char sceneText[8];
        char userId[5];
        char levelText[8];
        Float3 position(104.0f, 88.0f, 0.0f);

        g_Supervisor.EnterCriticalSectionWrapper(4);
        g_Supervisor.criticalSectionLockCounts[4]++;
        for (i32 replayIndex = FrontEndDrawSnapshot(this->groupCursor.current) * 20;
             replayIndex < FrontEndDrawSnapshot(this->groupCursor.current) * 20 + 20; replayIndex++)
        {
            if (FrontEndDrawSnapshot(this->secondaryCursor.current) == replayIndex % 20)
            {
                g_AsciiManager.color.color = 0xffffffff;
            }
            else
            {
                g_AsciiManager.color.color = 0xff404040;
            }

            if (FrontEndDrawSnapshot(this->groupCursor.current) == 0)
            {
                if (this->replays[replayIndex] == NULL || this->replays[replayIndex]->activeInputData == NULL)
                {
                    g_AsciiManager.AddFormatText(
                        &position,
                        "No.%.2d --------  *-* --/--/-- --:-- ------ --%%",
                        replayIndex + 1);
                }
                else
                {
                    tm *timestamp = LocalTimeFromStoredTimestamp(
                        &this->replays[replayIndex]->activeInputData->timestamp);


                    if (this->replays[replayIndex]->activeInputData->level == 10)
                    {
                        strcpy(levelText, "EX");
                    }
                    else
                    {
                        sprintf(levelText, "%2d", this->replays[replayIndex]->activeInputData->level + 1);
                    }
                    sprintf(sceneText, "%d", this->replays[replayIndex]->activeInputData->scene + 1);
                    g_AsciiManager.AddFormatText(
                        &position,
                        "No.%.2d %s %s-%s %.2d/%.2d/%.2d %.2d:%.2d %6d %2.0f%%",
                        replayIndex + 1, this->replays[replayIndex]->activeInputData->replayName, levelText,
                        sceneText, timestamp->tm_year % 100,
                        timestamp->tm_mon + 1, timestamp->tm_mday,
                        timestamp->tm_hour, timestamp->tm_min, this->replays[replayIndex]->activeInputData->score,
                        this->replays[replayIndex]->activeInputData->slowRate);
                }
            }
            else if (this->replays[replayIndex] == NULL || this->replays[replayIndex]->activeInputData == NULL)
            {
                    g_AsciiManager.AddFormatText(
                    &position,
                    "User---- --------  *-* --/--/-- --:-- ------ --.-%%");
            }
            else
            {
                tm *timestamp = LocalTimeFromStoredTimestamp(
                    &this->replays[replayIndex]->activeInputData->timestamp);


                if (this->replays[replayIndex]->activeInputData->level == 10)
                {
                    strcpy(levelText, "EX");
                }
                else
                {
                    sprintf(levelText, "%2d", this->replays[replayIndex]->activeInputData->level + 1);
                }
                sprintf(sceneText, "%d", this->replays[replayIndex]->activeInputData->scene + 1);
                memcpy(userId, this->replays[replayIndex]->path + 7, sizeof(userId) - 1);
                userId[sizeof(userId) - 1] = '\0';
                g_AsciiManager.AddFormatText(
                    &position,
                    "User%.4s %s %s-%s %.2d/%.2d/%.2d %.2d:%.2d %6d %2.0f%%",
                    userId, this->replays[replayIndex]->activeInputData->replayName, levelText, sceneText,
                    timestamp->tm_year % 100, timestamp->tm_mon + 1,
                    timestamp->tm_mday, timestamp->tm_hour,
                    timestamp->tm_min, this->replays[replayIndex]->activeInputData->score, this->replays[replayIndex]->activeInputData->slowRate);
            }
            position.y += 18.0f;
        }
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        g_Supervisor.criticalSectionLockCounts[4]--;
        g_AsciiManager.color.color = 0xffffffff;
    }
        break;
    }

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

#undef sceneText
#undef userId
#undef levelText
#undef position

ChainCallbackResult __fastcall FrontEndControllerView::OnDraw(void *controller)
{
    return static_cast<FrontEndControllerView *>(controller)->Draw();
}

#undef LocalTimeFromStoredTimestamp

} // namespace th095
