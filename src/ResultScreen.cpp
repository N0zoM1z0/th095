#include "ResultScreen.hpp"
#include "ScoreData.hpp"
#include "SoundPlayer.hpp"
#include "ZunMath.hpp"

#include <stdio.h>
#include <string.h>
#include <time.h>

namespace th095
{

extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;
extern f32 g_AnmGameSpeed;

struct ResultScreenGlobalStateView
{
    u8 unknown000[0xfc];
    union
    {
        u32 flagsWord;
        struct
        {
            u32 unknownFlag0 : 1;
            u32 unknownFlag1 : 1;
            u32 suppressResultCallbacks : 1;
            u32 unknownFlags : 29;
        };
    };
    i32 bestShotIndex;
    u8 unknown104[0x114 - 0x104];
    i32 currentScore;
    u8 unknown118[8];
    i32 resultMode;
};

struct ResultAnmVmDrawView
{
    void Draw();
};

struct ResultAsciiManagerView
{
    u8 unknown0000[0x806c];
    u32 color;
    f32 scaleX;
    f32 scaleY;

    void AddString(Float3 *position, const char *text);
    void AddFormatText(Float3 *position, const char *format, ...);
};

struct ResultAnmManagerResultView
{
    u8 unknown000000[0x0c];
    i32 captureAnmIndex;
    u8 unknown000010[0x3817d0 - 0x10];
    i32 captureSourceX;
    i32 captureSourceY;
    i32 captureSourceWidth;
    i32 captureSourceHeight;
    i32 captureDestinationX;
    i32 captureDestinationY;
    i32 captureDestinationWidth;
    i32 captureDestinationHeight;
    i32 captureFlags;

    void DrawTextLeft(ResultScreenAnmVm *vm, u32 textColor,
                      u32 shadowColor, const char *format, ...);
};

typedef char ResultScreenGlobalBestShotIndexAt100[
    (offsetof(ResultScreenGlobalStateView, bestShotIndex) == 0x100) ? 1 : -1];
typedef char ResultScreenGlobalCurrentScoreAt114[
    (offsetof(ResultScreenGlobalStateView, currentScore) == 0x114) ? 1 : -1];
typedef char ResultAsciiColorAt806C[
    (offsetof(ResultAsciiManagerView, color) == 0x806c) ? 1 : -1];
typedef char ResultAsciiScaleXAt8070[
    (offsetof(ResultAsciiManagerView, scaleX) == 0x8070) ? 1 : -1];
typedef char ResultAsciiScaleYAt8074[
    (offsetof(ResultAsciiManagerView, scaleY) == 0x8074) ? 1 : -1];

struct ResultAnmVmHandleView
{
    u8 unknown000[0x228];
    u32 flagsWord;
};

extern ResultScreenGlobalStateView *g_ResultScreenGlobalState;

struct ResultRuntimeView
{
    u8 unknown000[8];
    i16 scene;
    u8 unknown00a[0x14 - 0x0a];
    char replayName[9];
};

struct ResultPlayerConfigView
{
    u8 unknown000[4];
    i32 group;
    i32 scene;
};

struct ResultScreenDrawLocals
{
    i32 totalScore;
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
};

typedef char ResultScreenDrawLocalsSizeIsF0[
    (sizeof(ResultScreenDrawLocals) == 0xf0) ? 1 : -1];
typedef char ResultScreenDrawReplayNameTitleAt04[
    (offsetof(ResultScreenDrawLocals, replayNameTitlePosition) == 0x04)
        ? 1
        : -1];
typedef char ResultScreenDrawBestShotPositionAt64[
    (offsetof(ResultScreenDrawLocals, bestShotPosition) == 0x64) ? 1 : -1];
typedef char ResultScreenDrawCharacterXAt70[
    (offsetof(ResultScreenDrawLocals, characterX) == 0x70) ? 1 : -1];
typedef char ResultScreenDrawReplayNamePositionAtA8[
    (offsetof(ResultScreenDrawLocals, replayNamePosition) == 0xa8) ? 1 : -1];
typedef char ResultScreenDrawReplayListPositionAtD0[
    (offsetof(ResultScreenDrawLocals, replayListPosition) == 0xd0) ? 1 : -1];
typedef char ResultScreenDrawLoopIndexAtEC[
    (offsetof(ResultScreenDrawLocals, i) == 0xec) ? 1 : -1];

typedef char ResultBestShotImageScoreAt00[
    (offsetof(ResultBestShotImageView, score) == 0x00) ? 1 : -1];
typedef char ResultBestShotImageMetadataAt08[
    (offsetof(ResultBestShotImageView, metadata) == 0x08) ? 1 : -1];
typedef char ResultBestShotImageReplayValueAt2C[
    (offsetof(ResultBestShotImageView, replayValue) == 0x2c) ? 1 : -1];
typedef char ResultBestShotImageSlowRateAt38[
    (offsetof(ResultBestShotImageView, slowRate) == 0x38) ? 1 : -1];
typedef char ResultBestShotImageStageValueAt3C[
    (offsetof(ResultBestShotImageView, stageValue) == 0x3c) ? 1 : -1];
typedef char ResultBestShotRecordCommentAt18[
    (offsetof(ResultBestShotRecordView, comment) == 0x18) ? 1 : -1];
typedef char ResultBestShotRecordValidAt68[
    (offsetof(ResultBestShotRecordView, valid) == 0x68) ? 1 : -1];
typedef char ResultBestShotRecordComponentsLoadedAt69[
    (offsetof(ResultBestShotRecordView, componentsLoaded) == 0x69) ? 1 : -1];
typedef char ResultBestShotRecordPhotoIndexAt6C[
    (offsetof(ResultBestShotRecordView, photoIndex) == 0x6c) ? 1 : -1];

extern i32 g_ResultSceneState;
extern ResultRuntimeView *g_ResultRuntime;
extern ResultPlayerConfigView *g_ResultPlayerConfig;
extern u8 *g_ResultPlayerConfigTable[];
extern i32 g_ResultSceneLimits[];
extern const char *g_ResultAlphabet;
extern ResultPhotoDataView *g_ResultPhotoData;
extern ResultPhotoControllerView *g_ResultPhotoController;
extern ResultSaveDataView *g_ResultSaveData;
extern ResultAsciiManagerView g_ResultAsciiManager;
extern ResultAnmManagerResultView *g_ResultAnmManager;
extern ResultScreenAnmLoadedView *g_ResultAuxAnm;
extern f64 g_ReplayLagNumerator;
extern f64 g_ReplayLagDenominator;

extern void __fastcall InitializeGameResultScreen(ResultScreen *resultScreen);
extern void __fastcall InitializePhotoResultScreen(ResultScreen *resultScreen);
extern void __fastcall InitializeReplayResultScreen(ResultScreen *resultScreen);
extern void __fastcall PreparePhotoResultScreen(ResultScreen *resultScreen);
extern void __fastcall FinalizeResultRuntime(ResultRuntimeView *runtime);
extern i32 __fastcall ExecuteResultVm(ResultScreenAnmVm *vm);

inline u16 GetPressedButtons(u16 buttons)
{
    return g_PressedButtons & buttons;
}

inline u16 IsResultMenuInputPressed(u16 buttons)
{
    return (u16)((GetPressedButtons(buttons) != 0) ||
                 ((g_ResultMenuInput & buttons) != 0));
}

inline ResultScreenAnmVm *GetResultVm(ResultScreen *resultScreen, i32 index)
{
    return &resultScreen->vms[index];
}

i32 ResultScreenTimer::Tick()
{
    this->previous = this->current;
    if (g_AnmGameSpeed <= 0.99f)
    {
        this->subFrame += g_AnmGameSpeed;
        this->current = (i32)this->subFrame;
    }
    else
    {
        this->current++;
        this->subFrame += 1.0f;
    }
    return this->current;
}

i32 ResultPhotoDataView::FindBestShot()
{
    i32 bestShot;

    bestShot = -1;
    {
        i32 bestScore;
        bestScore = -1;

        for (i32 i = 0; i < 11; i++)
        {
            if (this->slots[i].score > bestScore)
            {
                bestScore = this->slots[i].score;
                bestShot = i;
            }
        }
    }
    return bestShot;
}

void ResultSaveDataView::UpdateBestShotRecord(i32 index)
{
    if (this->bestShotRecords[index].componentData0 != NULL)
    {
        g_ZunMemory.Free(
            this->bestShotRecords[index].componentData0);
    }
    this->bestShotRecords[index].componentData0 = NULL;

    if (this->bestShotRecords[index].pixelData != NULL)
    {
        g_ZunMemory.Free(
            this->bestShotRecords[index].pixelData);
    }
    this->bestShotRecords[index].pixelData = NULL;
    this->bestShotRecords[index].componentsLoaded = 0;
    this->bestShotRecords[index].valid = 0;
}

void __fastcall InitializeGameResultScreen(ResultScreen *resultScreen)
{
    g_SoundPlayer.PlaySoundByIdx(SOUND_20, 0);
    resultScreen->state = 1;
    resultScreen->stateTimer.Reset();
    resultScreen->savedGameSpeed = g_AnmGameSpeed;
    g_AnmGameSpeed = 1.0f;
    g_ResultScreenGlobalState->flagsWord |= 0x10;
    g_ResultScreenGlobalState->flagsWord |= 0x80;

    ResultAnmManagerResultView *anmManager = g_ResultAnmManager;
    if (anmManager->captureAnmIndex < 0)
    {
        anmManager->captureAnmIndex = 10;
        anmManager->captureSourceX = 0x80;
        anmManager->captureSourceY = 0x10;
        anmManager->captureSourceWidth = 0x180;
        anmManager->captureSourceHeight = 0x1c0;
        anmManager->captureDestinationX = 0;
        anmManager->captureDestinationY = 0;
        anmManager->captureDestinationWidth = 0x80;
        anmManager->captureDestinationHeight = 0x80;
        anmManager->captureFlags = 0;
    }

    resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 0), 0);
    resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 3), 3);
    if (g_ResultScreenGlobalState->resultMode == 0)
    {
        resultScreen->state = 1;
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 4), 4);
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 6), 6);
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 5), 5);
    }
    else
    {
        resultScreen->state = 11;
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 16), 16);
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 17), 17);
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 18), 18);
    }

    resultScreen->replayCursor.Set(0);
    resultScreen->replayCursor.count = 3;
    resultScreen->replayCursor.wraps = 1;
}

void __fastcall InitializeReplayResultScreen(ResultScreen *resultScreen)
{
    resultScreen->stateTimer.Reset();
    resultScreen->savedGameSpeed = g_AnmGameSpeed;
    g_AnmGameSpeed = 1.0f;
    g_ResultScreenGlobalState->flagsWord |= 0x10;
    g_ResultScreenGlobalState->flagsWord |= 0x80;

    ResultAnmManagerResultView *anmManager = g_ResultAnmManager;
    if (anmManager->captureAnmIndex < 0)
    {
        anmManager->captureAnmIndex = 10;
        anmManager->captureSourceX = 0x80;
        anmManager->captureSourceY = 0x10;
        anmManager->captureSourceWidth = 0x180;
        anmManager->captureSourceHeight = 0x1c0;
        anmManager->captureDestinationX = 0;
        anmManager->captureDestinationY = 0;
        anmManager->captureDestinationWidth = 0x80;
        anmManager->captureDestinationHeight = 0x80;
        anmManager->captureFlags = 0;
    }

    resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 1), 1);
    resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 7), 7);
    if (g_ResultScreenGlobalState->resultMode == 0)
    {
        resultScreen->state = 3;
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 9), 9);
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 10), 10);
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 8), 8);
        resultScreen->replayCursor.Set(0);
        resultScreen->replayCursor.count = 3;

        g_ResultAuxAnm->SetAndExecuteScript(GetResultVm(resultScreen, 21), 9);
        g_ResultAuxAnm->SetAndExecuteScript(GetResultVm(resultScreen, 22), 10);
        GetResultVm(resultScreen, 21)->glyphWidth = 0x12;
        GetResultVm(resultScreen, 21)->glyphHeight = 0x12;
        GetResultVm(resultScreen, 22)->glyphWidth = 0x12;
        GetResultVm(resultScreen, 22)->glyphHeight = 0x12;

        i32 group = resultScreen->selectedGroup;
        u8 scene = g_ResultSaveData->profile.nextSceneByGroup[group];
        g_ResultAnmManager->DrawTextLeft(
            GetResultVm(resultScreen, 21), 0xffe0c0, 0x300000,
            resultScreen->sceneLabels[group][scene].firstLine);
        g_ResultAnmManager->DrawTextLeft(
            GetResultVm(resultScreen, 22), 0xffe0c0, 0x300000,
            resultScreen->sceneLabels[group][scene].secondLine);

        g_ResultSaveData->profile.nextSceneByGroup[group]++;
        if (g_ResultSaveData->profile.nextSceneByGroup[group] >=
            resultScreen->sceneCounts[group])
        {
            g_ResultSaveData->profile.nextSceneByGroup[group] = 0;
        }

        time(reinterpret_cast<time_t *>(
            &g_ReplayManager->activeInputData->timestamp));
        g_ReplayManager->activeInputData->score =
            g_ResultScreenGlobalState->currentScore;
    }
    else
    {
        resultScreen->state = 7;
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 19), 19);
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 20), 20);
        resultScreen->replayCursor.Set(1);
        resultScreen->replayCursor.count = 2;

        g_ResultAuxAnm->SetAndExecuteScript(GetResultVm(resultScreen, 21), 9);
        g_ResultAuxAnm->SetAndExecuteScript(GetResultVm(resultScreen, 22), 10);
        GetResultVm(resultScreen, 21)->glyphWidth = 0x12;
        GetResultVm(resultScreen, 21)->glyphHeight = 0x12;
        GetResultVm(resultScreen, 22)->glyphWidth = 0x12;
        GetResultVm(resultScreen, 22)->glyphHeight = 0x12;
        g_ResultAnmManager->DrawTextLeft(
            GetResultVm(resultScreen, 21), 0xffe0c0, 0x300000, " ");
        g_ResultAnmManager->DrawTextLeft(
            GetResultVm(resultScreen, 22), 0xffe0c0, 0x300000, " ");
    }
    resultScreen->replayCursor.wraps = 1;
}

void __fastcall InitializePhotoResultScreen(ResultScreen *resultScreen)
{
    resultScreen->stateTimer.Reset();
    resultScreen->savedGameSpeed = g_AnmGameSpeed;
    g_AnmGameSpeed = 1.0f;
    g_ResultScreenGlobalState->flagsWord |= 0x10;
    g_ResultScreenGlobalState->flagsWord |= 0x80;

    ResultAnmManagerResultView *anmManager = g_ResultAnmManager;
    if (anmManager->captureAnmIndex < 0)
    {
        anmManager->captureAnmIndex = 10;
        anmManager->captureSourceX = 0x80;
        anmManager->captureSourceY = 0x10;
        anmManager->captureSourceWidth = 0x180;
        anmManager->captureSourceHeight = 0x1c0;
        anmManager->captureDestinationX = 0;
        anmManager->captureDestinationY = 0;
        anmManager->captureDestinationWidth = 0x80;
        anmManager->captureDestinationHeight = 0x80;
        anmManager->captureFlags = 0;
    }

    resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 2), 2);
    resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 11), 11);
    if (g_ResultScreenGlobalState->resultMode == 0)
    {
        resultScreen->state = 5;
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 14), 14);
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 15), 15);
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 12), 12);
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 13), 13);
        resultScreen->replayCursor.Set(2);
        resultScreen->replayCursor.count = 4;

        if (g_ResultPlayerConfig->scene >=
            g_ResultSceneLimits[g_ResultPlayerConfig->group] - 1)
        {
            resultScreen->replayCursor.disabledEntries[
                resultScreen->replayCursor.disabledEntryCount++] = 1;
            GetResultVm(resultScreen, 13)->color1 = 0x80000000;
        }

        ResultScoreEntryView *scoreEntry =
            reinterpret_cast<ResultScoreEntryView *>(
                reinterpret_cast<u8 *>(g_ResultSaveData) + 0x460) +
            g_ResultScreenGlobalState->bestShotIndex;
        scoreEntry->magic = 0x4353;
        scoreEntry->version = 1;
        scoreEntry->size = sizeof(ResultScoreEntryView);
        scoreEntry->index = g_ResultScreenGlobalState->bestShotIndex;
        scoreEntry->flags |= 1;
        if (scoreEntry->score < g_ResultScreenGlobalState->currentScore)
        {
            scoreEntry->score = g_ResultScreenGlobalState->currentScore;
            scoreEntry->slowRate =
                100.0f -
                (f32)(g_ReplayLagNumerator / g_ReplayLagDenominator * 100.0);
        }
    }
    else
    {
        resultScreen->state = 9;
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 19), 19);
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 20), 20);
        resultScreen->replayCursor.Set(1);
        resultScreen->replayCursor.count = 2;
    }
    resultScreen->replayCursor.wraps = 1;
    resultScreen->PrepareBestShot();
    time(reinterpret_cast<time_t *>(
        &g_ReplayManager->activeInputData->timestamp));
    g_ReplayManager->activeInputData->score =
        g_ResultScreenGlobalState->currentScore;
}

void ResultScreen::PrepareBestShot()
{
    i32 bestShot = g_ResultPhotoData->FindBestShot();
    if (bestShot >= 0)
    {
        g_ResultPhotoData->anm->SetAndExecuteScript(
            &this->vms[23], bestShot * 2 + 1);

        ResultScreenAnmVm *vm = &this->vms[23];
        vm->spriteSize.x = vm->loadedSprite->uvEndX * 255.0f;
        vm->spriteSize.y = vm->loadedSprite->uvEndY * 255.0f;

        g_ResultPhotoData->photoVms[bestShot].SetInterrupt(2);
        this->photoCursor.Set(bestShot);

        if (bestShot < g_ResultPhotoController->GetPhotoCount())
        {
            this->photoCursor.count =
                g_ResultPhotoController->GetPhotoCount();
            this->photoCursor.wraps = 1;
            for (i32 i = 0;
                 i < g_ResultPhotoController->GetPhotoCount();
                 i++)
            {
                reinterpret_cast<ResultAnmVmHandleView *>(
                    g_ResultPhotoData->photoVms[i].GetVm())
                    ->flagsWord |= 0x10000000;
            }
        }
    }
}

void __fastcall UpdatePhotoResultScreen(ResultScreen *resultScreen)
{
    i32 direction;
    ResultScreenAnmVm *vm;

    direction = 0;
    if (resultScreen->photoCursor.GetCurrent() <
        g_ResultPhotoController->GetPhotoCount())
    {
        resultScreen->photoCursor.SaveCurrent();
        if (IsResultMenuInputPressed(0x40))
        {
            resultScreen->photoCursor.Move(-1);
            direction = -1;
        }
        if (IsResultMenuInputPressed(0x80))
        {
            resultScreen->photoCursor.Move(1);
            direction = 1;
        }

        if (resultScreen->photoCursor.HasChanged())
        {
            i32 photoIndex = resultScreen->photoCursor.GetCurrent();
            resultScreen->vms[24] = resultScreen->vms[23];
            resultScreen->vms[24].SetInterrupt((direction <= 0) + 7);
            g_ResultPhotoData->anm->SetAndExecuteScript(
                &resultScreen->vms[23], photoIndex * 2 + 1);
            resultScreen->vms[23].SetInterrupt((direction > 0) + 9);

            vm = &resultScreen->vms[23];
            vm->spriteSize.x = vm->loadedSprite->uvEndX * 255.0f;
            vm->spriteSize.y = vm->loadedSprite->uvEndY * 255.0f;

            g_ResultPhotoData
                ->photoVms[resultScreen->photoCursor.GetPrevious()]
                .SetInterrupt(3);
            g_ResultPhotoData->photoVms[photoIndex].SetInterrupt(2);
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        }
    }

    if (GetPressedButtons(0x400) != 0)
    {
        i32 photoIndex = resultScreen->photoCursor.GetCurrent();

        memcpy(
            g_ResultSaveData
                ->bestShotImages[g_ResultScreenGlobalState->bestShotIndex]
                .metadata,
            g_ResultPhotoData->slots[photoIndex].metadata,
            sizeof(g_ResultSaveData->bestShotImages[0].metadata));
        g_ResultSaveData->UpdateBestShotRecord(
            g_ResultScreenGlobalState->bestShotIndex);

        g_ResultSaveData
            ->bestShotRecords[g_ResultScreenGlobalState->bestShotIndex]
            .valid = 1;
        g_ResultSaveData
            ->bestShotRecords[g_ResultScreenGlobalState->bestShotIndex]
            .magic = 0x53545342;
        g_ResultSaveData
            ->bestShotRecords[g_ResultScreenGlobalState->bestShotIndex]
            .width = g_ResultPhotoData->slots[photoIndex].width;
        g_ResultSaveData
            ->bestShotRecords[g_ResultScreenGlobalState->bestShotIndex]
            .height = g_ResultPhotoData->slots[photoIndex].height;
        g_ResultSaveData
            ->bestShotRecords[g_ResultScreenGlobalState->bestShotIndex]
            .score = g_ResultPhotoData->slots[photoIndex].score;
        g_ResultSaveData
            ->bestShotRecords[g_ResultScreenGlobalState->bestShotIndex]
            .group = (u16)(g_ResultPlayerConfig->group + 1);
        g_ResultSaveData
            ->bestShotRecords[g_ResultScreenGlobalState->bestShotIndex]
            .scene = (u16)(g_ResultPlayerConfig->scene + 1);
        g_ResultSaveData
            ->bestShotRecords[g_ResultScreenGlobalState->bestShotIndex]
            .type = 2;
        g_ResultSaveData
            ->bestShotRecords[g_ResultScreenGlobalState->bestShotIndex]
            .version = 0x102;
        g_ResultSaveData
            ->bestShotRecords[g_ResultScreenGlobalState->bestShotIndex]
            .componentCount =
            (u8)((g_ResultPhotoData->anm->textures[photoIndex].format == 4) + 2);
        g_ResultSaveData
            ->bestShotImages[g_ResultScreenGlobalState->bestShotIndex]
            .stageValue = g_ResultPhotoData->slots[photoIndex].stageValue;
        g_ResultSaveData
            ->bestShotImages[g_ResultScreenGlobalState->bestShotIndex]
            .replayValue = g_ResultPhotoData->slots[photoIndex].replayValue;
        strcpy(
            g_ResultSaveData
                ->bestShotRecords[g_ResultScreenGlobalState->bestShotIndex]
                .comment,
            g_ResultPhotoData->slots[photoIndex].comment);
        g_ResultSaveData
            ->bestShotRecords[g_ResultScreenGlobalState->bestShotIndex]
            .photoIndex = photoIndex;

        g_ResultSaveData->WriteBestShotData();
        g_SoundPlayer.PlaySoundByIdx(SOUND_TAKE_PHOTO, 0);
        resultScreen->notificationTimer = 120;
    }
}

i32 ResultScreenReplayCursor::Move(i32 amount)
{
    i32 i;

tryAgain:
    this->current += amount;
    while (this->current >= this->count)
    {
        if (this->wraps != 0)
        {
            this->current -= this->count;
        }
        else
        {
            this->current = this->count - 1;
        }
    }
    while (this->current < 0)
    {
        if (this->wraps != 0)
        {
            this->current += this->count;
        }
        else
        {
            this->current = 0;
        }
    }
    for (i = 0; i < this->disabledEntryCount; i++)
    {
        if (this->disabledEntries[i] == this->current)
        {
            goto tryAgain;
        }
    }
    return this->current;
}

void ResultScreenReplayCursor::Push()
{
    this->savedCurrent[this->saveDepth] = this->current;
    this->savedCount[this->saveDepth] = this->count;
    this->saveDepth++;
    if (this->saveDepth >= 16)
    {
        this->saveDepth = 15;
    }
    this->disabledEntryCount = 0;
}

void ResultScreenReplayCursor::Pop()
{
    this->saveDepth--;
    if (this->saveDepth < 0)
    {
        this->saveDepth = 0;
    }
    this->current = this->savedCurrent[this->saveDepth];
    this->count = this->savedCount[this->saveDepth];
    this->disabledEntryCount = 0;
}

i32 ResultScreen::UpdateCursor(i32 firstVm)
{
    if (this->stateTimer < 10)
    {
        return 1;
    }
    if (this->stateTimer == 10)
    {
        for (i32 i = 0; i < this->replayCursor.GetCount(); i++)
        {
            this->vms[firstVm + i].SetInterrupt(
                (i != this->replayCursor.GetCurrent()) + 2);
        }
    }

    this->replayCursor.SaveCurrent();
    if (IsResultMenuInputPressed(0x10))
    {
        this->replayCursor.Move(-1);
    }
    if (IsResultMenuInputPressed(0x20))
    {
        this->replayCursor.Move(1);
    }

    if (this->replayCursor.HasChanged())
    {
        for (i32 i = 0; i < this->replayCursor.GetCount(); i++)
        {
            this->vms[firstVm + i].SetInterrupt(
                (i != this->replayCursor.GetCurrent()) + 2);
        }
        g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
    }
    return 0;
}

ChainCallbackResult ResultScreen::Update()
{
    i32 i;

    switch (this->state)
    {
    case 0:
        if ((g_PressedButtons & 8) != 0)
        {
            InitializeGameResultScreen(this);
        }
        else if ((g_ResultScreenGlobalState->unknownFlags & (1 << 2)) != 0)
        {
            InitializePhotoResultScreen(this);
        }
        else if ((g_ResultScreenGlobalState->unknownFlags & (1 << 3)) != 0)
        {
            InitializeReplayResultScreen(this);
        }
        break;

    case 1:
        if (this->UpdateCursor(4) == 0)
        {
            if ((g_PressedButtons & 8) != 0)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                this->SetState(2);
                this->replayCursor.Set(0);
                for (i = 0; i < 21; i++)
                {
                    this->vms[i].SetInterrupt(1);
                }
                this->vms[23].SetInterrupt(1);
            }
            else if ((g_PressedButtons & 0x1002) != 0)
            {
                this->SetState(2);
                if (this->replayCursor.current == 0)
                {
                    for (i = 0; i < 21; i++)
                    {
                        this->vms[i].SetInterrupt(1);
                    }
                    this->vms[23].SetInterrupt(1);
                    g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                }
                else
                {
                    g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                }
            }
        }
        break;

    case 2:
        if (this->stateTimer.current > 7)
        {
            g_AnmGameSpeed = this->savedGameSpeed;
            this->state = 0;
            if (this->replayCursor.current == 0)
            {
                g_ResultScreenGlobalState->unknownFlags &= ~(1 << 2);
            }
            else if (this->replayCursor.current == 1)
            {
                g_AnmGameSpeed = 1.0f;
                g_ResultSceneState = 4;
            }
            else if (this->replayCursor.current == 2)
            {
                g_AnmGameSpeed = 1.0f;
                g_ResultSceneState = 2;
            }
            FinalizeResultRuntime(g_ResultRuntime);
            g_ResultScreenGlobalState->unknownFlags |= (1 << 4);
        }
        break;

    case 3:
        if (this->UpdateCursor(8) == 0 &&
            (g_PressedButtons & 0x1002) != 0)
        {
            this->SetState(4);
            for (i = 3; i < 21; i++)
            {
                this->vms[i].SetInterrupt(1);
            }
            g_SoundPlayer.PlaySoundByIdx(
                this->replayCursor.current == 0 ? SOUND_BACK : SOUND_SELECT, 0);
        }
        break;

    case 4:
        if (this->stateTimer.current > 7)
        {
            g_AnmGameSpeed = 1.0f;
            this->state = 0;
            if (this->replayCursor.current == 0)
            {
                g_ResultSceneState = 4;
            }
            else if (this->replayCursor.current == 1)
            {
                g_ResultSceneState = 2;
            }
            else if (this->replayCursor.current == 2)
            {
                this->replayCursor.Push();
                this->replayCursor.count = 20;
                this->state = 13;
                this->LoadReplays();
                this->stateTimer.Reset();
            }
            FinalizeResultRuntime(g_ResultRuntime);
            g_ResultScreenGlobalState->unknownFlags |= (1 << 4);
        }
        break;

    case 5:
        if (this->UpdateCursor(12) == 0)
        {
            UpdatePhotoResultScreen(this);
            if ((g_PressedButtons & 0x1002) != 0)
            {
                this->SetState(6);
                for (i = 3; i < 21; i++)
                {
                    this->vms[i].SetInterrupt(1);
                }
                this->vms[23].SetInterrupt(1);
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
            }
        }
        break;

    case 6:
        if (this->stateTimer.current > 7)
        {
            g_AnmGameSpeed = 1.0f;
            this->state = 0;
            FinalizeResultRuntime(g_ResultRuntime);
            switch (this->replayCursor.current)
            {
            case 0:
                g_ResultSceneState = 4;
                break;
            case 1:
                g_ResultRuntime->scene = (i16)(g_ResultPlayerConfig->scene + 1);
                g_ResultPlayerConfig = (ResultPlayerConfigView *)(
                    g_ResultPlayerConfigTable[g_ResultPlayerConfig->group] +
                    (g_ResultPlayerConfig->scene + 1) * 0x30);
                g_ResultSceneState = 8;
                break;
            case 2:
                g_ResultSceneState = 2;
                break;
            case 3:
                this->replayCursor.Push();
                this->replayCursor.count = 20;
                this->state = 13;
                this->LoadReplays();
                this->stateTimer.Reset();
                break;
            }
            g_ResultScreenGlobalState->unknownFlags |= (1 << 4);
        }
        break;

    case 7:
    case 9:
        if (this->UpdateCursor(19) == 0 &&
            (g_PressedButtons & 0x1002) != 0)
        {
            this->SetState(8);
            g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
        }
        break;

    case 8:
    case 10:
        if (this->stateTimer.current > 7)
        {
            g_AnmGameSpeed = 1.0f;
            this->state = 0;
            g_ResultSceneState = this->replayCursor.current == 0 ? 4 : 2;
            g_ResultScreenGlobalState->unknownFlags |= (1 << 4);
        }
        break;

    case 11:
        if (this->UpdateCursor(16) == 0)
        {
            if ((g_PressedButtons & 8) != 0)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                this->SetState(12);
                this->replayCursor.Set(0);
                for (i = 0; i < 21; i++)
                {
                    this->vms[i].SetInterrupt(1);
                }
                this->vms[23].SetInterrupt(1);
            }
            else if ((g_PressedButtons & 0x1002) != 0)
            {
                this->SetState(12);
                if (this->replayCursor.current == 0)
                {
                    g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                    for (i = 0; i < 21; i++)
                    {
                        this->vms[i].SetInterrupt(1);
                    }
                    this->vms[23].SetInterrupt(1);
                }
                else
                {
                    g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                }
            }
        }
        break;

    case 12:
        if (this->stateTimer.current > 7)
        {
            g_AnmGameSpeed = this->savedGameSpeed;
            this->state = 0;
            if (this->replayCursor.current == 0)
            {
                g_ResultScreenGlobalState->unknownFlags &= ~(1 << 2);
            }
            else if (this->replayCursor.current == 1)
            {
                g_AnmGameSpeed = 1.0f;
                g_ResultSceneState = 4;
            }
            else if (this->replayCursor.current == 2)
            {
                g_AnmGameSpeed = 1.0f;
                g_ResultSceneState = 2;
            }
            FinalizeResultRuntime(g_ResultRuntime);
        }
        break;

    case 13:
        this->replayCursor.SaveCurrent();
        if (IsResultMenuInputPressed(0x10))
        {
            this->replayCursor.Move(-1);
        }
        if (IsResultMenuInputPressed(0x20))
        {
            this->replayCursor.Move(1);
        }
        if (this->replayCursor.HasChanged())
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        }

        if ((g_PressedButtons & 0x1002) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
            this->SetState(14);
            strcpy(this->replayName, g_ResultRuntime->replayName);
            if (strcmp(this->replayName, "--------") != 0)
            {
                this->keyboardSelection = 95;
            }
            this->replayCursor.Push();
        }
        else if ((g_PressedButtons & 9) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            if ((g_ResultScreenGlobalState->unknownFlags & (1 << 2)) != 0)
            {
                this->SetState(3);
                this->anm->SetAndExecuteScript(&this->vms[7], 7);
                this->anm->SetAndExecuteScript(&this->vms[9], 9);
                this->anm->SetAndExecuteScript(&this->vms[10], 10);
                this->anm->SetAndExecuteScript(&this->vms[8], 8);
                this->replayCursor.Pop();
                this->replayCursor.count = 3;
                this->replayCursor.wraps = 1;
                this->replayNameCursor = 0;
            }
            else if ((g_ResultScreenGlobalState->unknownFlags & (1 << 3)) != 0)
            {
                this->SetState(5);
                this->anm->SetAndExecuteScript(&this->vms[11], 11);
                this->anm->SetAndExecuteScript(&this->vms[14], 14);
                this->anm->SetAndExecuteScript(&this->vms[15], 15);
                this->anm->SetAndExecuteScript(&this->vms[12], 12);
                this->anm->SetAndExecuteScript(&this->vms[13], 13);
                this->replayCursor.Pop();
                if (g_ResultPlayerConfig->scene >=
                    g_ResultSceneLimits[g_ResultPlayerConfig->group] - 1)
                {
                    this->replayCursor.disabledEntries[
                        this->replayCursor.disabledEntryCount++] = 1;
                    this->vms[13].color1 = 0x80000000;
                }
                PreparePhotoResultScreen(this);
                this->replayCursor.count = 4;
                this->replayCursor.wraps = 1;
            }
        }
        break;

    case 14:
        if (IsResultMenuInputPressed(0x10))
        {
            this->keyboardSelection -= 16;
            if (this->keyboardSelection < 0)
            {
                this->keyboardSelection += 96;
            }
        }
        if (IsResultMenuInputPressed(0x20))
        {
            this->keyboardSelection += 16;
            if (this->keyboardSelection >= 96)
            {
                this->keyboardSelection -= 96;
            }
        }
        if (IsResultMenuInputPressed(0x40))
        {
            if (this->keyboardSelection % 16 == 0)
            {
                this->keyboardSelection += 15;
            }
            else
            {
                this->keyboardSelection--;
            }
        }
        if (IsResultMenuInputPressed(0x80))
        {
            this->keyboardSelection++;
            if (this->keyboardSelection % 16 == 0)
            {
                this->keyboardSelection -= 16;
            }
        }

        if ((g_PressedButtons & 0x1002) != 0)
        {
            if (this->keyboardSelection == 95)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                this->SetState(15);
            }
            else if (this->keyboardSelection == 94)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                if (this->replayNameCursor == 7 && this->replayName[7] != ' ')
                {
                    this->replayName[7] = ' ';
                }
                else
                {
                    this->replayNameCursor--;
                    if (this->replayNameCursor < 0)
                    {
                        this->replayNameCursor = 0;
                    }
                    this->replayName[this->replayNameCursor] = ' ';
                }
            }
            else
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                this->replayName[this->replayNameCursor] =
                    g_ResultAlphabet[this->keyboardSelection];
                this->replayNameCursor++;
                if (this->replayNameCursor > 7)
                {
                    this->replayNameCursor = 7;
                    this->keyboardSelection = 95;
                }
            }
        }

        if ((g_PressedButtons & 9) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            if (this->replayNameCursor < 1)
            {
                this->SetState(13);
                this->replayCursor.Pop();
            }
            else
            {
                this->replayNameCursor--;
                this->replayName[this->replayNameCursor] = ' ';
            }
        }
        break;

    case 15:
    {
        char path[0x100];
        sprintf(path, "th95_%.2d.rpy", this->replayCursor.current + 1);
        g_ReplayManager->WriteReplay(path, this->replayName);
        strcpy(g_ResultRuntime->replayName, this->replayName);
        this->SetState(13);
        this->LoadReplays();
        this->replayCursor.Pop();
        break;
    }
    }

    for (i = 0; i < 21; i++)
    {
        ExecuteResultVm(&this->vms[i]);
    }
    this->stateTimer.Tick();
    for (i = 21; i < 25; i++)
    {
        ExecuteResultVm(&this->vms[i]);
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

ChainCallbackResult ResultScreen::Draw()
{
    ResultScreenDrawLocals locals;

    for (locals.i = 0; locals.i < 21; locals.i++)
    {
        reinterpret_cast<ResultAnmVmDrawView *>(&this->vms[locals.i])->Draw();
    }
    reinterpret_cast<ResultAnmVmDrawView *>(&this->vms[23])->Draw();
    reinterpret_cast<ResultAnmVmDrawView *>(&this->vms[24])->Draw();

    switch (this->state)
    {
    case 3:
        reinterpret_cast<ResultAnmVmDrawView *>(&this->vms[21])->Draw();
        reinterpret_cast<ResultAnmVmDrawView *>(&this->vms[22])->Draw();
        break;

    case 5:
    {
        if (this->stateTimer.GetCurrent() < 30)
        {
            g_ResultAsciiManager.color =
                (((this->stateTimer.GetCurrent() * 255) / 32) << 24) |
                0x00ffffff;
        }

        if (this->photoCursor.GetCurrent() == g_ResultPhotoData->FindBestShot())
        {
            locals.bestShotPosition.x = 230.0f;
            locals.bestShotPosition.y = 290.0f;
            locals.bestShotPosition.z = 0.0f;
            g_ResultAsciiManager.AddFormatText(
                &locals.bestShotPosition, "  Best Shot   %.6d",
                g_ResultPhotoData->slots[this->photoCursor.GetCurrent()].score);
        }
        else
        {
            locals.shotPosition.x = 230.0f;
            locals.shotPosition.y = 290.0f;
            locals.shotPosition.z = 0.0f;
            g_ResultAsciiManager.AddFormatText(
                &locals.shotPosition, "       Shot   %.6d",
                g_ResultPhotoData->slots[this->photoCursor.GetCurrent()].score);
        }

        locals.scorePosition.x = 230.0f;
        locals.scorePosition.y = 40.0f;
        locals.scorePosition.z = 0.0f;
        g_ResultAsciiManager.AddFormatText(
            &locals.scorePosition, "      Score  %.7d",
            g_ResultScreenGlobalState->currentScore);

        locals.highScorePosition.x = 230.0f;
        locals.highScorePosition.y = 54.0f;
        locals.highScorePosition.z = 0.0f;
        g_ResultAsciiManager.AddFormatText(
            &locals.highScorePosition, " High Score  %.7d",
            g_ResultSaveData
                ->bestShotImages[g_ResultScreenGlobalState->bestShotIndex]
                .score);

        locals.totalScore = 0;
        for (u32 scoreIndex = 0; scoreIndex < 120; scoreIndex++)
        {
            locals.totalScore +=
                g_ResultSaveData->bestShotImages[scoreIndex].score;
        }
        locals.totalScorePosition.x = 230.0f;
        locals.totalScorePosition.y = 68.0f;
        locals.totalScorePosition.z = 0.0f;
        g_ResultAsciiManager.AddFormatText(
            &locals.totalScorePosition, "Total Score %.8d",
            locals.totalScore);

        if (this->stateTimer.GetCurrent() < 30)
        {
            g_ResultAsciiManager.color =
                (((this->stateTimer.GetCurrent() * 255) / 32) << 24) |
                0x00d0d0e0;
        }
        else
        {
            g_ResultAsciiManager.color = 0xffd0d0e0;
        }
        locals.slowRatePosition.x = 230.0f;
        locals.slowRatePosition.y = 82.0f;
        locals.slowRatePosition.z = 0.0f;
        g_ResultAsciiManager.AddFormatText(
            &locals.slowRatePosition, "  Slow Rate      %2.0f%%",
            g_ResultSaveData
                ->bestShotImages[g_ResultScreenGlobalState->bestShotIndex]
                .slowRate);
        g_ResultAsciiManager.color = 0xffffffff;

        if (this->notificationTimer > 0)
        {
            g_ResultAsciiManager.color = 0xffffff00;
            locals.notificationPosition.x = 212.0f;
            locals.notificationPosition.y = 224.0f;
            locals.notificationPosition.z = 0.0f;
            g_ResultAsciiManager.AddFormatText(
                &locals.notificationPosition, "BestShot was overwrited!");
            g_ResultAsciiManager.color = 0xffffffff;
            this->notificationTimer--;
        }
        break;
    }

    case 13:
    {
        locals.replayListTitlePosition.x = 160.0f;
        locals.replayListTitlePosition.y = 32.0f;
        locals.replayListTitlePosition.z = 0.0f;
        g_ResultAsciiManager.AddFormatText(
            &locals.replayListTitlePosition, "Select Replay Number");

        locals.replayListPosition.x = 144.0f;
        locals.replayListPosition.y = 64.0f;
        locals.replayListPosition.z = 0.0f;
        for (locals.replayListIndex = 0; locals.replayListIndex < 20; locals.replayListIndex++)
        {
            if (this->replayCursor.GetCurrent() == locals.replayListIndex)
            {
                g_ResultAsciiManager.color = 0xffffffff;
            }
            else
            {
                g_ResultAsciiManager.color = 0xff404040;
            }

            if (this->replays[locals.replayListIndex] == NULL ||
                this->replays[locals.replayListIndex]->activeInputData == NULL)
            {
                g_ResultAsciiManager.AddFormatText(
                    &locals.replayListPosition, "No.%.2d %s %s-%s %s ------",
                    locals.replayListIndex + 1, "--------", "--", "-", "--/-- --:--");
            }
            else
            {
                locals.replayListTimestamp = localtime(
                    reinterpret_cast<time_t *>(
                        &this->replays[locals.replayListIndex]
                             ->activeInputData->timestamp));

                if (this->replays[locals.replayListIndex]->activeInputData->level == 10)
                {
                    strcpy(locals.replayListLevelText, "EX");
                }
                else
                {
                    sprintf(locals.replayListLevelText, "%2d",
                            this->replays[locals.replayListIndex]
                                    ->activeInputData->level +
                                1);
                }
                sprintf(locals.replayListSceneText, "%d",
                        this->replays[locals.replayListIndex]->activeInputData->scene + 1);
                g_ResultAsciiManager.AddFormatText(
                    &locals.replayListPosition,
                    "No.%.2d %s %s-%s %.2d/%.2d %.2d:%.2d %6d",
                    locals.replayListIndex + 1,
                    this->replays[locals.replayListIndex]->activeInputData->replayName,
                    locals.replayListLevelText, locals.replayListSceneText,
                    locals.replayListTimestamp->tm_mon + 1,
                    locals.replayListTimestamp->tm_mday, locals.replayListTimestamp->tm_hour,
                    locals.replayListTimestamp->tm_min,
                    this->replays[locals.replayListIndex]->activeInputData->score);
            }
            locals.replayListPosition.y += 18.0f;
        }
        g_ResultAsciiManager.color = 0xffffffff;
        break;
    }

    case 14:
    {
        locals.replayNameTitlePosition.x = 160.0f;
        locals.replayNameTitlePosition.y = 32.0f;
        locals.replayNameTitlePosition.z = 0.0f;
        g_ResultAsciiManager.AddFormatText(
            &locals.replayNameTitlePosition, "Replay Name Regist");

        locals.replayNamePosition.x = 144.0f;
        locals.replayNamePosition.y = 128.0f;
        locals.replayNamePosition.z = 0.0f;
        g_ResultAsciiManager.color = 0xa0ffffc0;
        locals.replayNamePosition.x =
            (f32)(this->replayNameCursor * 9) + 198.0f;
        g_ResultAsciiManager.AddFormatText(&locals.replayNamePosition, "_");

        locals.replayNamePosition.x = 144.0f;
        g_ResultAsciiManager.color = 0xffffffff;
        locals.replayIndex = this->replayCursor.current;
        locals.replayNameTimestamp = localtime(
            reinterpret_cast<time_t *>(
                &g_ReplayManager->activeInputData->timestamp));
        if (g_ResultPlayerConfig->group == 10)
        {
            strcpy(locals.replayNameLevelText, "EX");
        }
        else
        {
            sprintf(locals.replayNameLevelText, " %d",
                    g_ResultPlayerConfig->group + 1);
        }
        sprintf(locals.replayNameSceneText, "%d", g_ResultPlayerConfig->scene + 1);
        g_ResultAsciiManager.AddFormatText(
            &locals.replayNamePosition,
            "No.%.2d %s %s-%s %.2d/%.2d %.2d:%.2d %6d",
            locals.replayIndex + 1, this->replayName, locals.replayNameLevelText,
            locals.replayNameSceneText, locals.replayNameTimestamp->tm_mon + 1,
            locals.replayNameTimestamp->tm_mday, locals.replayNameTimestamp->tm_hour,
            locals.replayNameTimestamp->tm_min,
            g_ResultScreenGlobalState->currentScore);

        locals.rowY = 320.0f;
        locals.positionZ = 0.0f;
        for (locals.i = 0; locals.i < 6; locals.i++)
        {
            locals.characterX = 208.0f;
            for (locals.keyboardColumn = 0; locals.keyboardColumn < 16; locals.keyboardColumn++)
            {
                locals.characterX += 12.0f;
                locals.offsetY = 0.0f;
                locals.offsetX = 0.0f;
                if (this->keyboardSelection == locals.i * 16 + locals.keyboardColumn)
                {
                    g_ResultAsciiManager.color = 0xffffffc0;
                    if (this->stateTimer.current % 32 < 16)
                    {
                        locals.offsetY = (this->stateTimer.current % 16) *
                                      0.8f / 16.0f +
                                  1.2f;
                    }
                    else
                    {
                        locals.offsetY = 2.0f -
                                  (this->stateTimer.current % 16) *
                                      0.8f / 16.0f;
                    }
                    g_ResultAsciiManager.scaleX = locals.offsetY;
                    g_ResultAsciiManager.scaleY = locals.offsetY;
                    locals.offsetY = -(locals.offsetY - 1.0f) * 4.0f;
                    locals.offsetX = locals.offsetY;
                }
                else
                {
                    g_ResultAsciiManager.color = 0xc0c0c0c0;
                    g_ResultAsciiManager.scaleX = 1.0f;
                    g_ResultAsciiManager.scaleY = 1.0f;
                }

                locals.characterPosition.x = locals.characterX + locals.offsetY;
                locals.characterPosition.y = locals.rowY + locals.offsetX;
                locals.characterPosition.z = locals.positionZ;

                locals.characterText[0] =
                    g_ResultAlphabet[locals.i * 16 + locals.keyboardColumn];
                locals.characterText[1] = '\0';
                if (locals.i == 5)
                {
                    if (locals.keyboardColumn == 14)
                    {
                        locals.characterText[0] = 0x7f;
                    }
                    else if (locals.keyboardColumn == 15)
                    {
                        locals.characterText[0] = (char)0x80;
                    }
                    else if (locals.keyboardColumn == 13)
                    {
                        locals.characterText[0] = (char)0x81;
                    }
                }
                g_ResultAsciiManager.AddString(
                    &locals.characterPosition, locals.characterText);
            }
            locals.rowY += 16.0f;
        }
        g_ResultAsciiManager.scaleX = 1.0f;
        g_ResultAsciiManager.scaleY = 1.0f;
        break;
    }
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

ChainCallbackResult ResultScreen::OnUpdate(ResultScreen *resultScreen)
{
    if (g_ResultScreenGlobalState->suppressResultCallbacks)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    return resultScreen->Update();
}

ChainCallbackResult ResultScreen::OnDraw(ResultScreen *resultScreen)
{
    if (g_ResultScreenGlobalState->suppressResultCallbacks)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    return resultScreen->Draw();
}

ZunResult ResultScreen::LoadReplays()
{
    char path[0x100];

    for (i32 i = 0; i < 20; i++)
    {
        if (this->replays[i] != NULL)
        {
            delete this->replays[i];
            this->replays[i] = NULL;
        }
        sprintf(path, "th95_%.2d.rpy", i + 1);
        this->replays[i] = ReplayManager::Load(path);
    }
    this->replayCursor.Set(0);
    return ZUN_SUCCESS;
}

} // namespace th095
