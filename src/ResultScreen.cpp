#ifdef TH095_MATCH_EXACT
#include "ResultScreenExact.inl"
#else
#include "ResultScreen.hpp"
#include "ResultScreenState.hpp"
#include "FrontEndLocalTime.hpp"
#include "AsciiManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoGameTask.hpp"
#include "ControllerInputSlot.hpp"
#include "Main.hpp"
#include "PhotoStage.hpp"
#include "PhotoGamePlayer.hpp"
#include "ScoreData.hpp"
#include "SceneData.hpp"
#include "SoundPlayer.hpp"
#include "ZunMath.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef TH095_MATCH_EXACT
#define ZUN_SUCCESS TH095_LEGACY_ZUN_SUCCESS
#define ZUN_ERROR TH095_LEGACY_ZUN_ERROR
#endif

namespace th095
{

extern f32 g_AnmGameSpeed;
// Target 0x004C4E38 is the zero-initialized current result-screen owner; the
// constructor publishes it and the destructor clears it.
DIFFABLE_STATIC(ResultScreen *, g_ResultScreen);

struct ResultScreenInitializeLocals
{
    i32 lineIndex;
    u8 *next;
    char line[64];
    long currentLevel;
    size_t fileSize;
};

typedef char ResultScreenInitializeLocalsSizeIs50[
    (sizeof(ResultScreenInitializeLocals) == 0x50) ? 1 : -1];

typedef char ResultBestShotRecordCommentAt18[
    (offsetof(ResultBestShotRecordView, comment) == 0x18) ? 1 : -1];
typedef char ResultBestShotRecordValidAt68[
    (offsetof(ResultBestShotRecordView, valid) == 0x68) ? 1 : -1];
typedef char ResultBestShotRecordComponentsLoadedAt69[
    (offsetof(ResultBestShotRecordView, componentsLoaded) == 0x69) ? 1 : -1];
typedef char ResultBestShotRecordPhotoIndexAt6C[
    (offsetof(ResultBestShotRecordView, photoIndex) == 0x6c) ? 1 : -1];

extern i32 g_ResultSceneState;
#ifndef DIFFBUILD
#define g_ResultSceneState (g_Supervisor.requestedSceneState)
#endif
#ifndef DIFFBUILD
// Target 0x004C4E3C is a 12-entry zero-initialized BSS map.  Initialize reads
// it once as [selected scene group * 4 + base] and no target instruction writes
// any element: every TH095 scene group selects the same photo-result mode.
i32 g_ResultGroupMap[12];
#else
extern i32 g_ResultGroupMap[];
#endif
extern u8 *__fastcall ReadResultHelpLine(
    char *destination, u8 *source, i32 maxLength);
#ifndef DIFFBUILD
// Exact ResultScreen relocations named g_ResultSceneLimits and the canonical
// scene-selection relocations both solve to target 0x004A5830.  Keep one
// production owner for this twelve-entry table instead of duplicating its
// currently identical initializer under a partial-view name.
#define g_ResultSceneLimits g_SceneGroupCounts
// Target pointer 0x004A441C -> canonical 96-character replay-name keyboard at
// 0x00496398.  The final two hyphens occupy keyboard cells 94 and 95.
const char *g_ResultAlphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:;_@"
    "abcdefghijklmnopqrstuvwxyz+-/*=%"
    "0123456789#!?'\"$(){}[]<>&\\|~^ --";
#else
extern i32 g_ResultSceneLimits[];
extern const char *g_ResultAlphabet;
#endif
extern void __fastcall InitializeGameResultScreen(ResultScreen *resultScreen);
extern void __fastcall InitializePhotoResultScreen(ResultScreen *resultScreen);
extern void __fastcall InitializeReplayResultScreen(ResultScreen *resultScreen);

#ifdef DIFFBUILD
extern void __fastcall PreparePhotoResultScreen(ResultScreen *resultScreen);
#define TH095_RESULT_PREPARE_BEST_SHOT(resultScreen) PreparePhotoResultScreen(resultScreen)
#else
#define TH095_RESULT_PREPARE_BEST_SHOT(resultScreen) (resultScreen)->PrepareBestShot()
#endif
inline u16 GetPressedButtons(u16 buttons)
{
    return g_ControllerInputSlots[0].pressed & buttons;
}

inline u16 IsResultMenuInputPressed(u16 buttons)
{
    return (u16)((GetPressedButtons(buttons) != 0) ||
                 ((g_ControllerInputSlots[0].repeat & buttons) != 0));
}

inline ResultScreenAnmVm *GetResultVm(ResultScreen *resultScreen, i32 index)
{
    return &resultScreen->vms[index];
}

static __forceinline void FreeResultHelpText(ResultScreen *resultScreen)
{
    if (resultScreen->helpTextBuffer != NULL)
    {
        u8 *helpTextBuffer = resultScreen->helpTextBuffer;
        free(helpTextBuffer);
    }
}

// FUNCTION: TH095 0x004264B0.
ResultScreen::ResultScreen()
    : replayCursor()
{
    utils::DebugPrint("initialize PauseInf\n");
    memset(this, 0, sizeof(ResultScreen));
    g_ResultScreen = this;
}

// FUNCTION: TH095 0x00426880.
ResultScreen::~ResultScreen()
{
    utils::DebugPrint("shutdown PauseInf\n");
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);

    for (i32 i = 0; i < 20; i++)
    {
        if (this->replays[i] != NULL)
        {
            delete this->replays[i];
            this->replays[i] = NULL;
        }
    }
    FreeResultHelpText(this);
    g_AnmManager->MarkVmsForDeletion(this->anm);
    g_ResultScreen = NULL;
}

// FUNCTION: TH095 0x00426630.
ResultScreenResult ResultScreen::Initialize()
{
    u8 *cursor;
    ResultScreenInitializeLocals locals;

    this->anm = g_AnmManager->PreloadAnm(10, "pause.anm");
    if (this->anm == NULL)
    {
        g_GameErrorContext.Log(
            "\x89\xe6\x96\xca\x8d\x5c\x90\xac"
            "\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8c\xa9\x82\xc2\x82\xa9\x82\xe8"
            "\x82\xdc\x82\xb9\x82\xf1\x81\x42"
            "\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x89\xf3\x82\xea\x82\xc4\x82\xa2"
            "\x82\xdc\x82\xb7\r\n");
        return ZUN_ERROR;
    }

    this->helpTextBuffer =
        FileSystem::OpenFile(
            "sprt/help.txt", (i32 *)&locals.fileSize, FALSE);
    if (this->helpTextBuffer == NULL)
    {
        g_GameErrorContext.Log(
            "\x89\xe6\x96\xca\x8d\x5c\x90\xac"
            "\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8c\xa9\x82\xc2\x82\xa9\x82\xe8"
            "\x82\xdc\x82\xb9\x82\xf1\x81\x42"
            "\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x89\xf3\x82\xea\x82\xc4\x82\xa2"
            "\x82\xdc\x82\xb7\r\n");
        return ZUN_ERROR;
    }

    cursor = this->helpTextBuffer;
    locals.currentLevel = -1;
    locals.lineIndex = 0;
    while ((i32)locals.fileSize > 0)
    {
        locals.next = ReadResultHelpLine(
            locals.line, cursor, sizeof(locals.line));
        locals.fileSize -= locals.next - cursor;
        cursor = locals.next;

        if (locals.line[0] == '#' || locals.line[0] == '\0')
        {
            continue;
        }
        if (locals.line[0] == '\0')
        {
            continue;
        }
        if (strncmp(locals.line, "end", 3) == 0)
        {
            break;
        }
        if (strncmp(locals.line, "level:", 6) == 0)
        {
            locals.currentLevel = atol(locals.line + 6);
            this->sceneCounts[locals.currentLevel]++;
            locals.lineIndex = 0;
            continue;
        }
        if (locals.currentLevel >= 0)
        {
            strcpy(
                reinterpret_cast<char *>(
                    &this->sceneLabels[locals.currentLevel]
                         [this->sceneCounts[locals.currentLevel] - 1]) +
                    locals.lineIndex * 0x2c,
                locals.line);
            locals.lineIndex++;
        }
    }
    this->selectedGroup = g_ResultGroupMap[g_SelectedScene->group];
    return ZUN_SUCCESS;
}

#include "ResultScreenAnmResources.inl"

// FUNCTION: TH095 0x00426A50.
ResultScreen *ResultScreen::Create()
{
    ResultScreen *resultScreen;
    ChainElem *elem;

    resultScreen = new ResultScreen();
    if (resultScreen->Initialize() != ZUN_SUCCESS)
    {
        goto failure;
    }

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(ResultScreen::OnUpdate));
    elem->arg = resultScreen;
    g_Chain.AddToCalcChain(elem, 5);
    resultScreen->calcChain = elem;

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(ResultScreen::OnDraw));
    elem->arg = resultScreen;
    g_Chain.AddToDrawChain(elem, 0x1b);
    resultScreen->drawChain = elem;
    return resultScreen;

failure:
    if (resultScreen != NULL)
    {
        delete resultScreen;
        resultScreen = NULL;
    }
    return NULL;
}

// FUNCTION: TH095 0x00426B90.
void ResultScreen::Destroy()
{
    ResultScreen *resultScreen = this;
    if (resultScreen != NULL)
    {
        delete resultScreen;
        resultScreen = NULL;
    }
}

#ifdef TH095_MATCH_EXACT
i32 ResultScreenTimer::Tick()
#else
i32 ZunTimer::Tick()
#endif
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

i32 PhotoStageStateView::FindBestShot()
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
    if (this->bestShotRecords[index].rawFileData != NULL)
    {
        g_ZunMemory.Free(
            this->bestShotRecords[index].rawFileData);
    }
    this->bestShotRecords[index].rawFileData = NULL;

    if (this->bestShotRecords[index].pixelData != NULL)
    {
        g_ZunMemory.Free(
            this->bestShotRecords[index].pixelData);
    }
    this->bestShotRecords[index].pixelData = NULL;
    this->bestShotRecords[index].componentsLoaded = 0;
    this->bestShotRecords[index].valid = 0;
}

#include "ResultScreenCapture.inl"

static __forceinline void InitializeGameReplayCursorPhase(
    ResultScreenReplayCursor *cursor)
{
    u8 compilerStorage[0x90];
    cursor->Set(0);
}

void __fastcall InitializeGameResultScreen(ResultScreen *resultScreen)
{
    g_SoundPlayer.PlaySoundByIdx(SOUND_20, 0);
    resultScreen->state = RESULT_SCREEN_GAME_RESULT_MENU;
    resultScreen->stateTimer.Reset();
    resultScreen->savedGameSpeed = g_AnmGameSpeed;
    g_AnmGameSpeed = 1.0f;
    g_RuntimeGlobalStateOwner->resultScreenActive = 1;
    g_RuntimeGlobalStateOwner->resetFpsSample = 1;

    InitializeResultCapturePhase();

    resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 0), 0);
    resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 3), 3);
    if (g_RuntimeGlobalStateOwner->replayMode == REPLAY_MANAGER_RECORD)
    {
        resultScreen->state = RESULT_SCREEN_GAME_RESULT_MENU;
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 4), 4);
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 6), 6);
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 5), 5);
    }
    else
    {
        resultScreen->state = RESULT_SCREEN_GAME_RESULT_NON_RECORD_MENU;
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 16), 16);
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 17), 17);
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 18), 18);
    }

    InitializeGameReplayCursorPhase(&resultScreen->replayCursor);
    resultScreen->replayCursor.count = 3;
    resultScreen->replayCursor.wraps = 1;
}


static __forceinline void InitializeReplayNormalTailPhase()
{
    u8 compilerStorage[0xa0];
    g_ReplayManager->activeInputData->timestamp =
        (i32)time(NULL);
    g_ReplayManager->activeInputData->score =
        g_RuntimeGlobalStateOwner->score;
}

static __forceinline void InitializeReplayExtraTailPhase(
    ResultScreen *resultScreen)
{
    u8 compilerStorage[0x58];
    resultScreen->replayCursor.count = 2;
    // Target 0x00428B1C/0x00428B36 read the same Supervisor::textAnm owner
    // used by the normal replay-label path below.
    g_Supervisor.textAnm->InitializeVm(&resultScreen->auxiliaryVms[0], 9);
    g_Supervisor.textAnm->InitializeVm(&resultScreen->auxiliaryVms[1], 10);
    resultScreen->auxiliaryVms[0].glyphWidth = 0x12;
    resultScreen->auxiliaryVms[0].glyphHeight = 0x12;
    resultScreen->auxiliaryVms[1].glyphWidth = 0x12;
    resultScreen->auxiliaryVms[1].glyphHeight = 0x12;
    g_AnmManager->DrawTextLeft(
        &resultScreen->auxiliaryVms[0],
        0xffe0c0, 0x300000, " ");
    g_AnmManager->DrawTextLeft(
        &resultScreen->auxiliaryVms[1],
        0xffe0c0, 0x300000, " ");
}

void __fastcall InitializeReplayResultScreen(ResultScreen *resultScreen)
{
    resultScreen->stateTimer.Reset();
    resultScreen->savedGameSpeed = g_AnmGameSpeed;
    g_AnmGameSpeed = 1.0f;
    g_RuntimeGlobalStateOwner->resultScreenActive = 1;
    g_RuntimeGlobalStateOwner->resetFpsSample = 1;

    InitializeResultCapturePhase();

    resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 1), 1);
    resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 7), 7);
    if (g_RuntimeGlobalStateOwner->replayMode == REPLAY_MANAGER_RECORD)
    {
        resultScreen->state = RESULT_SCREEN_REPLAY_RESULT_MENU;
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 9), 9);
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 10), 10);
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 8), 8);
        resultScreen->replayCursor.Set(0);
        resultScreen->replayCursor.count = 3;

        // Exact relocations at 0x00428DC0/0x00428DD9 read target
        // 0x004C4AAC, Supervisor::textAnm.  These two VMs render writable
        // replay labels and are not owned by the result-screen ANM.
        g_Supervisor.textAnm->InitializeVm(&resultScreen->auxiliaryVms[0], 9);
        g_Supervisor.textAnm->InitializeVm(&resultScreen->auxiliaryVms[1], 10);
        resultScreen->auxiliaryVms[0].glyphWidth = 0x12;
        resultScreen->auxiliaryVms[0].glyphHeight = 0x12;
        resultScreen->auxiliaryVms[1].glyphWidth = 0x12;
        resultScreen->auxiliaryVms[1].glyphHeight = 0x12;

        g_AnmManager->DrawTextLeft(
            &resultScreen->auxiliaryVms[0],
            0xffe0c0, 0x300000,
            resultScreen->sceneLabels[resultScreen->selectedGroup]
                [g_ResultSaveData->profile.nextSceneByGroup[
                    resultScreen->selectedGroup]].firstLine);
        g_AnmManager->DrawTextLeft(
            &resultScreen->auxiliaryVms[1],
            0xffe0c0, 0x300000,
            resultScreen->sceneLabels[resultScreen->selectedGroup]
                [g_ResultSaveData->profile.nextSceneByGroup[
                    resultScreen->selectedGroup]].secondLine);

        g_ResultSaveData
            ->profile.nextSceneByGroup[resultScreen->selectedGroup]++;
        if (g_ResultSaveData
                ->profile.nextSceneByGroup[resultScreen->selectedGroup] >=
            resultScreen->sceneCounts[resultScreen->selectedGroup])
        {
            g_ResultSaveData
                ->profile.nextSceneByGroup[resultScreen->selectedGroup] = 0;
        }

        InitializeReplayNormalTailPhase();
    }
    else
    {
        resultScreen->state = 7;
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 19), 19);
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 20), 20);
        resultScreen->replayCursor.Set(1);
        InitializeReplayExtraTailPhase(resultScreen);
    }
    resultScreen->replayCursor.wraps = 1;
}

static __forceinline void InitializePhotoDisableCursorPhase(
    ResultScreenReplayCursor *cursor)
{
    cursor->disabledEntries[cursor->disabledEntryCount++] = 1;
}

static __forceinline void InitializePhotoExtraCursorPhase(
    ResultScreenReplayCursor *cursor)
{
    u8 compilerStorage[0x48];
    cursor->Set(1);
}

void __fastcall InitializePhotoResultScreen(ResultScreen *resultScreen)
{
    resultScreen->stateTimer.Reset();
    resultScreen->savedGameSpeed = g_AnmGameSpeed;
    g_AnmGameSpeed = 1.0f;
    g_RuntimeGlobalStateOwner->resultScreenActive = 1;
    g_RuntimeGlobalStateOwner->resetFpsSample = 1;

    InitializeResultCapturePhase();

    resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 2), 2);
    resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 11), 11);
    if (g_RuntimeGlobalStateOwner->replayMode == REPLAY_MANAGER_RECORD)
    {
        resultScreen->state = RESULT_SCREEN_PHOTO_RESULT_MENU;
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 14), 14);
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 15), 15);
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 12), 12);
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 13), 13);
        resultScreen->replayCursor.Set(2);
        resultScreen->replayCursor.count = 4;

        if (g_SelectedScene->scene >=
            g_ResultSceneLimits[g_SelectedScene->group] - 1)
        {
            InitializePhotoDisableCursorPhase(&resultScreen->replayCursor);
            resultScreen->vms[13].color1.color = 0x80000000;
        }

        ResultScoreEntryView *scoreEntry =
            &g_ResultSaveData
                 ->scoreEntries[g_RuntimeGlobalStateOwner->bestShotIndex];
        scoreEntry->magic = SCENE_SCORE_RECORD_MAGIC;
        scoreEntry->version = SCENE_SCORE_RECORD_VERSION;
        scoreEntry->size = sizeof(ResultScoreEntryView);
        scoreEntry->index = g_RuntimeGlobalStateOwner->bestShotIndex;
        scoreEntry->flags |= 1;
        if (scoreEntry->score < g_RuntimeGlobalStateOwner->score)
        {
            scoreEntry->score = g_RuntimeGlobalStateOwner->score;
            scoreEntry->highScoreSlowRate =
                100.0f -
                (f32)(g_Supervisor.lagNumerator / g_Supervisor.lagDenominator) * 100.0f;
        }
    }
    else
    {
        resultScreen->state = 9;
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 19), 19);
        resultScreen->anm->InitializeVm(GetResultVm(resultScreen, 20), 20);
        InitializePhotoExtraCursorPhase(&resultScreen->replayCursor);
        resultScreen->replayCursor.count = 2;
    }
    resultScreen->replayCursor.wraps = 1;
    resultScreen->PrepareBestShot();
    g_ReplayManager->activeInputData->timestamp =
        (i32)time(NULL);
    g_ReplayManager->activeInputData->score =
        g_RuntimeGlobalStateOwner->score;
}

static __forceinline void PrepareBestShotCursorSetPhase(
    ResultScreenReplayCursor *cursor, i32 bestShot)
{
    u8 compilerStorage[0x38];
    cursor->Set(bestShot);
}

void ResultScreen::PrepareBestShot()
{
    i32 bestShot = g_PhotoStageState->FindBestShot();
    if (bestShot >= 0)
    {
        g_PhotoStageState->anm->InitializeVm(
            &this->photoVm, bestShot * 2 + 1);

        ResultScreenAnmVm *vm = &this->photoVm;
        vm->spriteSize.x = vm->loadedSprite->uvEndX * 256.0f;
        vm->spriteSize.y = vm->loadedSprite->uvEndY * 256.0f;

        g_PhotoStageState->capturedPhotoVms[bestShot].SetInterrupt(2);
        PrepareBestShotCursorSetPhase(&this->photoCursor, bestShot);

        if (bestShot < ReadResultPhotoLimit(&g_RuntimePlayerOwner->camera))
        {
            this->photoCursor.count =
                ReadResultPhotoLimit(&g_RuntimePlayerOwner->camera);
            this->photoCursor.wraps = 1;
            for (i32 i = 0;
                 i < ReadResultPhotoLimit(&g_RuntimePlayerOwner->camera);
                 i++)
            {
                g_PhotoStageState->capturedPhotoVms[i].GetVm()->flagsWord |=
                    ANM_VM_BYPASS_PHOTO_GAME_SUPPRESSION;
            }
        }
    }
}

static __forceinline void ResultPhotoInterruptPreviousPhase(
    ResultScreen *resultScreen)
{
    u8 compilerStorage[0x24];
    g_PhotoStageState
        ->capturedPhotoVms[resultScreen->photoCursor.GetPrevious()]
        .SetInterrupt(3);
}

static __forceinline void ResultPhotoInterruptCurrentPhase(i32 photoIndex)
{
    u8 compilerStorage[8];
    g_PhotoStageState->capturedPhotoVms[photoIndex].SetInterrupt(2);
}

void __fastcall UpdatePhotoResultScreen(ResultScreen *resultScreen)
{
    i32 direction;
    ResultScreenAnmVm *vm;

    direction = 0;
    if (resultScreen->photoCursor.GetCurrent() <
        ReadResultPhotoLimit(&g_RuntimePlayerOwner->camera))
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
            resultScreen->photoTransitionVm = resultScreen->photoVm;
            resultScreen->photoTransitionVm.SetInterrupt((direction <= 0) + 7);
            g_PhotoStageState->anm->InitializeVm(
                &resultScreen->photoVm, photoIndex * 2 + 1);
            resultScreen->photoVm.SetInterrupt((direction > 0) + 9);

            vm = &resultScreen->photoVm;
            vm->spriteSize.x = vm->loadedSprite->uvEndX * 256.0f;
            vm->spriteSize.y = vm->loadedSprite->uvEndY * 256.0f;

            ResultPhotoInterruptPreviousPhase(resultScreen);
            ResultPhotoInterruptCurrentPhase(photoIndex);
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        }
    }

    if (GetPressedButtons(0x400) != 0)
    {
        i32 photoIndex = resultScreen->photoCursor.GetCurrent();

        g_ResultSaveData
            ->scoreEntries[g_RuntimeGlobalStateOwner->bestShotIndex]
            .scoreBreakdown =
            g_PhotoStageState->slots[photoIndex].scoreBreakdown;
        g_ResultSaveData->UpdateBestShotRecord(
            g_RuntimeGlobalStateOwner->bestShotIndex);

        g_ResultSaveData
            ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
            .valid = 1;
        g_ResultSaveData
            ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
            .magic = 0x53545342;
        g_ResultSaveData
            ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
            .width = g_PhotoStageState->slots[photoIndex].width;
        g_ResultSaveData
            ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
            .height = g_PhotoStageState->slots[photoIndex].height;
        g_ResultSaveData
            ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
            .score = g_PhotoStageState->slots[photoIndex].score;
        g_ResultSaveData
            ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
            .group = (u16)(g_SelectedScene->group + 1);
        g_ResultSaveData
            ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
            .scene = (u16)(g_SelectedScene->scene + 1);
        g_ResultSaveData
            ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
            .payloadFormat =
            RESULT_BEST_SHOT_PAYLOAD_COMMENT_AND_COMPRESSED_PIXELS;
        g_ResultSaveData
            ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
            .version = 0x102;
        g_ResultSaveData
            ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
            .componentCount =
            (u8)((g_PhotoStageState->anm->textures[photoIndex].bytesPerPixel == 4) + 2);
        g_ResultSaveData
            ->scoreEntries[g_RuntimeGlobalStateOwner->bestShotIndex]
            .bestShotSlowRate = g_PhotoStageState->slots[photoIndex].slowRate;
        g_ResultSaveData
            ->scoreEntries[g_RuntimeGlobalStateOwner->bestShotIndex]
            .captureTime = g_PhotoStageState->slots[photoIndex].timestamp;
        strcpy(
            g_ResultSaveData
                ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
                .comment,
            g_PhotoStageState->slots[photoIndex].comment);
        g_ResultSaveData
            ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
            .photoIndex = photoIndex;

        g_ResultSaveData->WriteBestShotData();
        g_SoundPlayer.PlaySoundByIdx(SOUND_TAKE_PHOTO, 0);
        resultScreen->notificationTimer = 120;
    }
}

#include "ResultScreenReplayCursor.inl"

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

static __forceinline void ResultUpdatePhotoSetStatePhase(
    ResultScreen *resultScreen, i32 value)
{
    u8 compilerStorage[0xb0];
    resultScreen->state = value;
    resultScreen->stateTimer.Reset();
}

static __forceinline void ResultUpdateReplayDisablePhase(
    ResultScreenReplayCursor *cursor, i32 value)
{
    u8 compilerStorage[0xdc];
    cursor->Disable(value);
}

static __forceinline void ResultUpdateInterruptFirstPhase(ResultScreen *resultScreen)
{
    for (i32 index = 0; index < 21; index++)
    {
        resultScreen->vms[index].SetInterrupt(1);
    }
}

ChainCallbackResult ResultScreen::Update()
{
    char path[0x100];

    switch (this->state)
    {
    case 0:
        if (GetPressedButtons(8) != 0)
        {
            InitializeGameResultScreen(this);
            break;
        }
        else if (g_RuntimeGlobalStateOwner->playerDeathTransitionComplete != 0)
        {
            InitializeReplayResultScreen(this);
            break;
        }
        else if (g_RuntimeGlobalStateOwner->photoLimitTransitionComplete != 0)
        {
            InitializePhotoResultScreen(this);
            break;
        }
        break;

    case RESULT_SCREEN_GAME_RESULT_MENU:
        if (this->UpdateCursor(4) != 0)
        {
            break;
        }
        if (GetPressedButtons(8) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            this->state = RESULT_SCREEN_GAME_RESULT_EXIT;
            this->replayCursor.Set(0);
            this->stateTimer.Reset();
            ResultUpdateInterruptFirstPhase(this);
            this->photoVm.SetInterrupt(1);
            break;
        }
        else if (GetPressedButtons(0x1002) != 0)
        {
            this->SetState(RESULT_SCREEN_GAME_RESULT_EXIT);
            switch (this->replayCursor.GetCurrent())
            {
            case 0:
                ResultUpdateInterruptFirstPhase(this);
                this->photoVm.SetInterrupt(1);
                g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                break;
            case 1:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                break;
            case 2:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                break;
            }
        }
        break;

    case RESULT_SCREEN_GAME_RESULT_EXIT:
        if (this->stateTimer >= 8)
        {
            g_AnmGameSpeed = this->savedGameSpeed;
            this->state = 0;
            switch (this->replayCursor.GetCurrent())
            {
            case 0:
                g_RuntimeGlobalStateOwner->resultScreenActive = 0;
                break;
            case 1:
                g_AnmGameSpeed = 1.0f;
                g_ResultSceneState = 4;
                break;
            case 2:
                g_AnmGameSpeed = 1.0f;
                g_ResultSceneState = 2;
                break;
            }
            g_ResultSaveData->WriteBestShotData();
            g_RuntimeGlobalStateOwner->resetFpsSample = 1;
        }
        break;

    case RESULT_SCREEN_GAME_RESULT_NON_RECORD_MENU:
        if (this->UpdateCursor(16) != 0)
        {
            break;
        }
        if (GetPressedButtons(8) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            this->state = RESULT_SCREEN_GAME_RESULT_NON_RECORD_EXIT;
            this->replayCursor.Set(0);
            this->stateTimer.Reset();
            ResultUpdateInterruptFirstPhase(this);
            this->photoVm.SetInterrupt(1);
            break;
        }
        else if (GetPressedButtons(0x1002) != 0)
        {
            this->SetState(RESULT_SCREEN_GAME_RESULT_NON_RECORD_EXIT);
            switch (this->replayCursor.GetCurrent())
            {
            case 0:
                g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                ResultUpdateInterruptFirstPhase(this);
                this->photoVm.SetInterrupt(1);
                break;
            case 1:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                break;
            case 2:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                break;
            }
        }
        break;

    case RESULT_SCREEN_GAME_RESULT_NON_RECORD_EXIT:
        if (this->stateTimer >= 8)
        {
            g_AnmGameSpeed = this->savedGameSpeed;
            this->state = 0;
            switch (this->replayCursor.GetCurrent())
            {
            case 0:
                g_RuntimeGlobalStateOwner->resultScreenActive = 0;
                break;
            case 1:
                g_AnmGameSpeed = 1.0f;
                g_ResultSceneState = 4;
                break;
            case 2:
                g_AnmGameSpeed = 1.0f;
                g_ResultSceneState = 2;
                break;
            }
            g_ResultSaveData->WriteBestShotData();
        }
        break;

    case RESULT_SCREEN_REPLAY_RESULT_MENU:
        if (this->UpdateCursor(8) != 0)
        {
            break;
        }
        if (GetPressedButtons(0x1002) != 0)
        {
            this->SetState(RESULT_SCREEN_REPLAY_RESULT_EXIT);
            for (i32 i = 3; i < 21; i++)
            {
                this->vms[i].SetInterrupt(1);
            }
            switch (this->replayCursor.GetCurrent())
            {
            case 0:
                g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                break;
            case 1:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                break;
            case 2:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                break;
            }
        }
        break;

    case RESULT_SCREEN_REPLAY_RESULT_EXIT:
        if (this->stateTimer >= 8)
        {
            g_AnmGameSpeed = 1.0f;
            this->state = 0;
            switch (this->replayCursor.GetCurrent())
            {
            case 0:
                g_ResultSceneState = 4;
                break;
            case 2:
                this->replayCursor.Push();
                this->replayCursor.count = 20;
                this->state = RESULT_SCREEN_REPLAY_SLOT_SELECT;
                this->LoadReplays();
                this->stateTimer.Reset();
                break;
            case 1:
                g_ResultSceneState = 2;
                break;
            }
            g_ResultSaveData->WriteBestShotData();
            g_RuntimeGlobalStateOwner->resetFpsSample = 1;
        }
        break;

    case RESULT_SCREEN_PHOTO_RESULT_MENU:
        if (this->UpdateCursor(12) != 0)
        {
            break;
        }
        else
        {
            UpdatePhotoResultScreen(this);
            if (GetPressedButtons(0x1002) != 0)
            {
                this->SetState(RESULT_SCREEN_PHOTO_RESULT_EXIT);
                for (i32 i = 3; i < 21; i++)
                {
                    this->vms[i].SetInterrupt(1);
                }
                this->photoVm.SetInterrupt(1);
                switch (this->replayCursor.GetCurrent())
                {
                default:
                    g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                    break;
                }
            }
        }
        break;

    case RESULT_SCREEN_PHOTO_RESULT_EXIT:
        if (this->stateTimer >= 8)
        {
            g_AnmGameSpeed = 1.0f;
            this->state = 0;
            g_ResultSaveData->WriteBestShotData();
            switch (this->replayCursor.GetCurrent())
            {
            case 3:
                this->replayCursor.Push();
                this->replayCursor.count = 20;
                this->state = RESULT_SCREEN_REPLAY_SLOT_SELECT;
                this->LoadReplays();
                this->stateTimer.Reset();
                break;
            case 2:
                g_ResultSceneState = 2;
                break;
            case 0:
                g_ResultSceneState = 4;
                break;
            case 1:
                g_ResultSaveData->profile.lastSelectedScene = (i16)(g_SelectedScene->scene + 1);
                g_SelectedScene =
                    g_SceneGroups[g_SelectedScene->group] +
                    (g_SelectedScene->scene + 1);
                g_ResultSceneState = 8;
                break;
            }
            g_RuntimeGlobalStateOwner->resetFpsSample = 1;
        }
        break;

    case 7:
    case 9:
        if (this->UpdateCursor(19) != 0)
        {
            break;
        }
        if (GetPressedButtons(0x1002) != 0)
        {
            this->SetState(8);
            switch (this->replayCursor.GetCurrent())
            {
            case 0:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                break;
            case 1:
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                break;
            }
        }
        break;

    case 8:
    case 10:
        if (this->stateTimer >= 8)
        {
            g_AnmGameSpeed = 1.0f;
            this->state = 0;
            switch (this->replayCursor.GetCurrent())
            {
            case 0:
                g_ResultSceneState = 4;
                break;
            case 1:
                g_ResultSceneState = 2;
                break;
            }
            g_RuntimeGlobalStateOwner->resetFpsSample = 1;
        }
        break;

    case RESULT_SCREEN_REPLAY_SLOT_SELECT:
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

        if (GetPressedButtons(0x1002) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
            this->SetState(RESULT_SCREEN_REPLAY_NAME_ENTRY);
            strcpy(this->replayName, g_ResultSaveData->profile.replayName);
            if (strcmp(this->replayName, "        ") != 0)
            {
                this->keyboardSelection = 95;
            }
            this->replayCursor.Push();
            goto updateResultVms;
        }
        else if (GetPressedButtons(9) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            if (g_RuntimeGlobalStateOwner->playerDeathTransitionComplete != 0)
            {
                this->stateTimer.Reset();
                this->anm->InitializeVm(GetResultVm(this, 7), 7);
                this->state = RESULT_SCREEN_REPLAY_RESULT_MENU;
                this->anm->InitializeVm(GetResultVm(this, 9), 9);
                this->anm->InitializeVm(GetResultVm(this, 10), 10);
                this->anm->InitializeVm(GetResultVm(this, 8), 8);
                this->replayCursor.Pop();
                this->replayCursor.count = 3;
                this->replayCursor.wraps = 1;
                this->replayNameCursor = 0;
                goto updateResultVms;
            }
            else if (g_RuntimeGlobalStateOwner->photoLimitTransitionComplete != 0)
            {
                ResultUpdatePhotoSetStatePhase(
                    this, RESULT_SCREEN_PHOTO_RESULT_MENU);
                this->anm->InitializeVm(GetResultVm(this, 11), 11);
                this->state = RESULT_SCREEN_PHOTO_RESULT_MENU;
                this->anm->InitializeVm(GetResultVm(this, 14), 14);
                this->anm->InitializeVm(GetResultVm(this, 15), 15);
                this->anm->InitializeVm(GetResultVm(this, 12), 12);
                this->anm->InitializeVm(GetResultVm(this, 13), 13);
                this->replayCursor.Pop();
                if (g_SelectedScene->scene >=
                    g_ResultSceneLimits[g_SelectedScene->group] - 1)
                {
                    ResultUpdateReplayDisablePhase(&this->replayCursor, 1);
                    this->vms[13].color1.color = 0x80000000;
                }
                TH095_RESULT_PREPARE_BEST_SHOT(this);
                this->replayCursor.count = 4;
                this->replayCursor.wraps = 1;
                break;
            }
        }
        break;

    case RESULT_SCREEN_REPLAY_NAME_ENTRY:
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

        if (GetPressedButtons(0x1002) != 0)
        {
            if (this->keyboardSelection == 95)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                this->SetState(RESULT_SCREEN_REPLAY_WRITE);
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
                if (this->replayNameCursor >= 8)
                {
                    this->replayNameCursor = 7;
                    this->keyboardSelection = 95;
                }
            }
        }

        if (GetPressedButtons(9) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            if (this->replayNameCursor > 0)
            {
                this->replayNameCursor--;
                this->replayName[this->replayNameCursor] = ' ';
            }
            else
            {
                this->SetState(RESULT_SCREEN_REPLAY_SLOT_SELECT);
                this->replayCursor.Pop();
            }
        }
        break;

    case RESULT_SCREEN_REPLAY_WRITE:
        sprintf(path, "th95_%.2d.rpy", this->replayCursor.GetCurrent() + 1);
        g_ReplayManager->WriteReplay(path, this->replayName);
        strcpy(g_ResultSaveData->profile.replayName, this->replayName);
        this->SetState(RESULT_SCREEN_REPLAY_SLOT_SELECT);
        this->LoadReplays();
        this->replayCursor.Pop();
        break;
    }

updateResultVms:
    for (i32 i = 0; i < 21; i++)
    {
        AnmManager::ExecuteScript(
            reinterpret_cast<AnmVm *>(&this->vms[i]));
    }
    this->stateTimer.Tick();
    AnmManager::ExecuteScript(
        reinterpret_cast<AnmVm *>(&this->auxiliaryVms[0]));
    AnmManager::ExecuteScript(
        reinterpret_cast<AnmVm *>(&this->auxiliaryVms[1]));
    AnmManager::ExecuteScript(reinterpret_cast<AnmVm *>(&this->photoVm));
    AnmManager::ExecuteScript(
        reinterpret_cast<AnmVm *>(&this->photoTransitionVm));
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

#include "ResultScreenDraw.inl"


ChainCallbackResult ResultScreen::OnUpdate(ResultScreen *resultScreen)
{
    if (g_RuntimeGlobalStateOwner->gameplayLoadActive)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    return resultScreen->Update();
}

ChainCallbackResult ResultScreen::OnDraw(ResultScreen *resultScreen)
{
    if (g_RuntimeGlobalStateOwner->gameplayLoadActive)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    return resultScreen->Draw();
}

ResultScreenResult ResultScreen::LoadReplays()
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

#endif // TH095_MATCH_EXACT
