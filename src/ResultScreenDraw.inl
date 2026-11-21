static __forceinline void DrawResultTotalScorePhase(Float3 *position)
{
    i32 totalScore = 0;
    for (u32 scoreIndex = 0; scoreIndex < 120; scoreIndex++)
    {
        totalScore += g_ResultSaveData->scoreEntries[scoreIndex].score;
    }
    position->x = 230.0f;
    position->y = 68.0f;
    position->z = 0.0f;
    g_AsciiManager.AddFormatText(position, "Total Score %.8d", totalScore);
}

static __forceinline i32 ResultFindBestShotPhase()
{
    // TH095 Draw assigns a 12-byte compiler allocation phase to this
    // best-shot frontend. 0/4/8/16-byte controls and moving the same 12
    // bytes to the total-score or ordinary-shot phases do not replay exact.
    u8 compilerStorage[12];
    return g_PhotoStageState->FindBestShot();
}

static __forceinline void DrawResultBestShotLinePhase(
    ResultScreen *resultScreen, Float3 *position)
{
    i32 photoIndex = resultScreen->photoCursor.GetCurrent();
    position->x = 230.0f;
    position->y = 290.0f;
    position->z = 0.0f;
    g_AsciiManager.AddFormatText(
        position, "  Best Shot   %.6d",
        g_PhotoStageState->slots[photoIndex].score);
}

static __forceinline void DrawResultShotLinePhase(
    ResultScreen *resultScreen, Float3 *position)
{
    i32 photoIndex = resultScreen->photoCursor.GetCurrent();
    position->x = 230.0f;
    position->y = 290.0f;
    position->z = 0.0f;
    g_AsciiManager.AddFormatText(
        position, "       Shot   %.6d",
        g_PhotoStageState->slots[photoIndex].score);
}

// TH08's patched #pragma var_order is used only as a local-order oracle here.
// Stock VC7.1 ignores that pragma, so these target-proven identifier buckets
// reproduce the same physical order for the 28 real Draw locals. Keep the
// semantic aliases readable and change the backing rank only with a full replay.
#define i resultDrawBacking157
#define replayListSceneText resultDrawBacking153
#define replayListLevelText resultDrawBacking146
#define replayListPosition resultDrawBacking142
#define replayListIndex resultDrawBacking096
#define replayListTimestamp resultDrawBacking092
#define replayIndex resultDrawBacking026
#define replayNameSceneText resultDrawBacking022
#define replayNameLevelText resultDrawBacking139
#define replayNamePosition resultDrawBacking135
#define replayNameTimestamp resultDrawBacking131
#define characterPosition resultDrawBacking119
#define offsetY resultDrawBacking115
#define offsetX resultDrawBacking111
#define characterText resultDrawBacking089
#define keyboardColumn resultDrawBacking085
#define positionZ resultDrawBacking081
#define rowY resultDrawBacking017
#define characterX resultDrawBacking013
#define bestShotPosition resultDrawBacking127
#define shotPosition resultDrawBacking123
#define scorePosition resultDrawBacking107
#define highScorePosition resultDrawBacking103
#define totalScorePosition resultDrawBacking077
#define slowRatePosition resultDrawBacking073
#define notificationPosition resultDrawBacking066
#define replayListTitlePosition resultDrawBacking062
#define replayNameTitlePosition resultDrawBacking057
ChainCallbackResult ResultScreen::Draw()
{
    Float3 replayNameTitlePosition;
    Float3 replayListTitlePosition;
    Float3 notificationPosition;
    Float3 slowRatePosition;
    Float3 totalScorePosition;
    Float3 highScorePosition;
    Float3 scorePosition;
    Float3 shotPosition;
    Float3 bestShotPosition;
    f32 characterX;
    f32 rowY;
    f32 positionZ;
    i32 keyboardColumn;
    char characterText[16];
    f32 offsetX;
    f32 offsetY;
    Float3 characterPosition;
    tm *replayNameTimestamp;
    Float3 replayNamePosition;
    char replayNameLevelText[8];
    char replayNameSceneText[8];
    i32 replayIndex;
    tm *replayListTimestamp;
    i32 replayListIndex;
    Float3 replayListPosition;
    char replayListLevelText[8];
    char replayListSceneText[8];
    i32 i;

    for (i = 0; i < 21; i++)
    {
        this->vms[i].Draw();
    }
    this->photoVm.Draw();
    this->photoTransitionVm.Draw();

    switch (this->state)
    {
    case RESULT_SCREEN_REPLAY_RESULT_MENU:
        this->auxiliaryVms[0].Draw();
        this->auxiliaryVms[1].Draw();
        break;

    case RESULT_SCREEN_PHOTO_RESULT_MENU:
    {
        if (this->stateTimer.GetCurrent() < 30)
        {
            g_AsciiManager.color.color =
                (((this->stateTimer.GetCurrent() * 255) / 32) << 24) |
                0x00ffffff;
        }

        if (this->photoCursor.GetCurrent() == ResultFindBestShotPhase())
        {
            DrawResultBestShotLinePhase(this, &bestShotPosition);
        }
        else
        {
            DrawResultShotLinePhase(this, &shotPosition);
        }

        scorePosition.x = 230.0f;
        scorePosition.y = 40.0f;
        scorePosition.z = 0.0f;
        g_AsciiManager.AddFormatText(
            &scorePosition, "      Score  %.7d",
            g_RuntimeGlobalStateOwner->score);

        highScorePosition.x = 230.0f;
        highScorePosition.y = 54.0f;
        highScorePosition.z = 0.0f;
        g_AsciiManager.AddFormatText(
            &highScorePosition, " High Score  %.7d",
            g_ResultSaveData
                ->scoreEntries[g_RuntimeGlobalStateOwner->bestShotIndex]
                .score);

        DrawResultTotalScorePhase(&totalScorePosition);

        if (this->stateTimer.GetCurrent() < 30)
        {
            g_AsciiManager.color.color =
                (((this->stateTimer.GetCurrent() * 255) / 32) << 24) |
                0x00d0d0e0;
        }
        else
        {
            g_AsciiManager.color.color = 0xffd0d0e0;
        }
        slowRatePosition.x = 230.0f;
        slowRatePosition.y = 82.0f;
        slowRatePosition.z = 0.0f;
        g_AsciiManager.AddFormatText(
            &slowRatePosition, "  Slow Rate      %2.0f%%",
            g_ResultSaveData
                ->scoreEntries[g_RuntimeGlobalStateOwner->bestShotIndex]
                .highScoreSlowRate);
        g_AsciiManager.color.color = 0xffffffff;

        if (this->notificationTimer > 0)
        {
            g_AsciiManager.color.color = 0xffffff00;
            notificationPosition.x = 212.0f;
            notificationPosition.y = 224.0f;
            notificationPosition.z = 0.0f;
            g_AsciiManager.AddFormatText(
                &notificationPosition, "BestShot was overwrited!");
            g_AsciiManager.color.color = 0xffffffff;
            this->notificationTimer--;
        }
        break;
    }

    case RESULT_SCREEN_REPLAY_SLOT_SELECT:
    {
        replayListTitlePosition.x = 160.0f;
        replayListTitlePosition.y = 32.0f;
        replayListTitlePosition.z = 0.0f;
        g_AsciiManager.AddFormatText(
            &replayListTitlePosition, "Select Replay Number");

        replayListPosition.x = 144.0f;
        replayListPosition.y = 64.0f;
        replayListPosition.z = 0.0f;
        for (replayListIndex = 0; replayListIndex < 20; replayListIndex++)
        {
            if (this->replayCursor.GetCurrent() == replayListIndex)
            {
                g_AsciiManager.color.color = 0xffffffff;
            }
            else
            {
                g_AsciiManager.color.color = 0xff404040;
            }

            if (this->replays[replayListIndex] == NULL ||
                this->replays[replayListIndex]->activeInputData == NULL)
            {
                g_AsciiManager.AddFormatText(
                    &replayListPosition, "No.%.2d %s %s-%s %s ------",
                    replayListIndex + 1, "--------", "--", "-", "--/-- --:--");
            }
            else
            {
                replayListTimestamp = LocalTimeFromStoredTimestamp(
                    &this->replays[replayListIndex]
                        ->activeInputData->timestamp);

                if (this->replays[replayListIndex]->activeInputData->level == 10)
                {
                    strcpy(replayListLevelText, "EX");
                }
                else
                {
                    sprintf(replayListLevelText, "%2d",
                            this->replays[replayListIndex]
                                    ->activeInputData->level +
                                1);
                }
                sprintf(replayListSceneText, "%d",
                        this->replays[replayListIndex]->activeInputData->scene + 1);
                g_AsciiManager.AddFormatText(
                    &replayListPosition,
                    "No.%.2d %s %s-%s %.2d/%.2d %.2d:%.2d %6d",
                    replayListIndex + 1,
                    this->replays[replayListIndex]->activeInputData->replayName,
                    replayListLevelText, replayListSceneText,
                    replayListTimestamp->tm_mon + 1,
                    replayListTimestamp->tm_mday, replayListTimestamp->tm_hour,
                    replayListTimestamp->tm_min,
                    this->replays[replayListIndex]->activeInputData->score);
            }
            replayListPosition.y += 18.0f;
        }
        g_AsciiManager.color.color = 0xffffffff;
        break;
    }

    case RESULT_SCREEN_REPLAY_NAME_ENTRY:
    {
        replayNameTitlePosition.x = 160.0f;
        replayNameTitlePosition.y = 32.0f;
        replayNameTitlePosition.z = 0.0f;
        g_AsciiManager.AddFormatText(
            &replayNameTitlePosition, "Replay Name Regist");

        replayNamePosition.x = 144.0f;
        replayNamePosition.y = 128.0f;
        replayNamePosition.z = 0.0f;
        g_AsciiManager.color.color = 0xa0ffffc0;
        replayNamePosition.x =
            (f32)(this->replayNameCursor * 9) + 198.0f;
        g_AsciiManager.AddFormatText(&replayNamePosition, "_");

        replayNamePosition.x = 144.0f;
        g_AsciiManager.color.color = 0xffffffff;
        replayIndex = this->replayCursor.current;
        replayNameTimestamp = LocalTimeFromStoredTimestamp(
            &g_ReplayManager->activeInputData->timestamp);
        if (g_SelectedScene->group == 10)
        {
            strcpy(replayNameLevelText, "EX");
        }
        else
        {
            sprintf(replayNameLevelText, " %d",
                    g_SelectedScene->group + 1);
        }
        sprintf(replayNameSceneText, "%d", g_SelectedScene->scene + 1);
        g_AsciiManager.AddFormatText(
            &replayNamePosition,
            "No.%.2d %s %s-%s %.2d/%.2d %.2d:%.2d %6d",
            replayIndex + 1, this->replayName, replayNameLevelText,
            replayNameSceneText, replayNameTimestamp->tm_mon + 1,
            replayNameTimestamp->tm_mday, replayNameTimestamp->tm_hour,
            replayNameTimestamp->tm_min,
            g_RuntimeGlobalStateOwner->score);

        rowY = 320.0f;
        positionZ = 0.0f;
        for (i = 0; i < 6; i++)
        {
            characterX = 208.0f;
            for (keyboardColumn = 0; keyboardColumn < 16; keyboardColumn++)
            {
                characterX += 12.0f;
                offsetY = 0.0f;
                offsetX = 0.0f;
                if (this->keyboardSelection == i * 16 + keyboardColumn)
                {
                    g_AsciiManager.color.color = 0xffffffc0;
                    if (this->stateTimer.current % 32 < 16)
                    {
                        offsetY = (this->stateTimer.current % 16) *
                                      0.8f / 16.0f +
                                  1.2f;
                    }
                    else
                    {
                        offsetY = 2.0f -
                                  (this->stateTimer.current % 16) *
                                      0.8f / 16.0f;
                    }
                    g_AsciiManager.scaleX = offsetY;
                    g_AsciiManager.scaleY = offsetY;
                    offsetY = -(offsetY - 1.0f) * 8.0f;
                    offsetX = offsetY;
                }
                else
                {
                    g_AsciiManager.color.color = 0xc0c0c0c0;
                    g_AsciiManager.scaleX = 1.0f;
                    g_AsciiManager.scaleY = 1.0f;
                }

                characterPosition.x = characterX + offsetY;
                characterPosition.y = rowY + offsetX;
                characterPosition.z = positionZ;

                characterText[0] =
                    g_ResultAlphabet[i * 16 + keyboardColumn];
                characterText[1] = '\0';
                if (i == 5)
                {
                    if (keyboardColumn == 14)
                    {
                        characterText[0] = 0x7f;
                    }
                    else if (keyboardColumn == 15)
                    {
                        characterText[0] = (char)0x80;
                    }
                    else if (keyboardColumn == 13)
                    {
                        characterText[0] = (char)0x81;
                    }
                }
                g_AsciiManager.AddString(
                    &characterPosition, characterText);
            }
            rowY += 16.0f;
        }
        g_AsciiManager.scaleX = 1.0f;
        g_AsciiManager.scaleY = 1.0f;
        break;
    }
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}
#undef replayNameTitlePosition
#undef replayListTitlePosition
#undef notificationPosition
#undef slowRatePosition
#undef totalScorePosition
#undef highScorePosition
#undef scorePosition
#undef shotPosition
#undef bestShotPosition
#undef characterX
#undef rowY
#undef positionZ
#undef keyboardColumn
#undef characterText
#undef offsetX
#undef offsetY
#undef characterPosition
#undef replayNameTimestamp
#undef replayNamePosition
#undef replayNameLevelText
#undef replayNameSceneText
#undef replayIndex
#undef replayListTimestamp
#undef replayListIndex
#undef replayListPosition
#undef replayListLevelText
#undef replayListSceneText
#undef i
