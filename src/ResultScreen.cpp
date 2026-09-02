#include "ResultScreen.hpp"
#include "ScoreData.hpp"
#include "SoundPlayer.hpp"
#include "ZunMath.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

namespace th095
{

extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;
extern f32 g_AnmGameSpeed;
extern ResultScreen *g_ResultScreen;

struct ResultScreenAnmManagerLifecycleView
{
    ResultScreenAnmLoadedView *LoadAnm(i32 anmIndex, const char *path);
    void ReleaseAnm(i32 anmIndex);
    void MarkVmsForDeletion(ResultScreenAnmLoadedView *anm);
};

extern ResultScreenAnmManagerLifecycleView *g_AnmManager;

struct AnmVm;
struct AnmManager
{
    static i32 __fastcall ExecuteScript(AnmVm *vm);
};

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
            u32 unknownFlag3 : 1;
            u32 unknownFlag4 : 1;
            u32 replayResultActive : 1;
            u32 photoResultActive : 1;
            u32 unknownFlags : 25;
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
    u8 unknown000[0x14];
    char replayName[9];
    u8 unknown01d[3];
    i16 scene;
};

struct ResultPlayerConfigView
{
    u8 unknown000[4];
    i32 group;
    i32 scene;
};

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

typedef char ResultBestShotImageScoreAt10[
    (offsetof(ResultBestShotImageView, score) == 0x10) ? 1 : -1];
typedef char ResultBestShotImageMetadataAt18[
    (offsetof(ResultBestShotImageView, metadata) == 0x18) ? 1 : -1];
typedef char ResultBestShotImageReplayValueAt3C[
    (offsetof(ResultBestShotImageView, replayValue) == 0x3c) ? 1 : -1];
typedef char ResultBestShotImageSlowRateAt48[
    (offsetof(ResultBestShotImageView, slowRate) == 0x48) ? 1 : -1];
typedef char ResultBestShotImageStageValueAt4C[
    (offsetof(ResultBestShotImageView, stageValue) == 0x4c) ? 1 : -1];
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
extern i32 g_ResultGroupMap[];
extern u8 *__fastcall ReadResultHelpLine(
    char *destination, u8 *source, i32 maxLength);
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
    : replayCursor(0)
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
ZunResult ResultScreen::Initialize()
{
    u8 *cursor;
    ResultScreenInitializeLocals locals;

    this->anm = g_AnmManager->LoadAnm(10, "pause.anm");
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
    this->selectedGroup = g_ResultGroupMap[g_ResultPlayerConfig->group];
    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00426820.
ZunResult ResultScreen::LoadAnm()
{
    if (g_AnmManager->LoadAnm(10, "pause.anm") == NULL)
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
    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00426860.
ZunResult ResultScreen::ReleaseAnm()
{
    g_AnmManager->ReleaseAnm(10);
    return ZUN_SUCCESS;
}

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

static __forceinline void InitializeResultCapturePhase()
{
    ResultAnmManagerResultView *anmManager = g_ResultAnmManager;
    if (anmManager->captureAnmIndex >= 0)
    {
    }
    else
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
}

static __forceinline void InitializeGameReplayCursorPhase(
    ResultScreenReplayCursor *cursor)
{
    u8 compilerStorage[0x90];
    cursor->Set(0);
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

    InitializeResultCapturePhase();

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

    InitializeGameReplayCursorPhase(&resultScreen->replayCursor);
    resultScreen->replayCursor.count = 3;
    resultScreen->replayCursor.wraps = 1;
}


static __forceinline void InitializeReplayNormalTailPhase()
{
    u8 compilerStorage[0xa0];
    time(reinterpret_cast<time_t *>(
        &g_ReplayManager->activeInputData->timestamp));
    g_ReplayManager->activeInputData->score =
        g_ResultScreenGlobalState->currentScore;
}

static __forceinline void InitializeReplayExtraTailPhase(
    ResultScreen *resultScreen)
{
    u8 compilerStorage[0x58];
    resultScreen->replayCursor.count = 2;
    g_ResultAuxAnm->SetAndExecuteScript(&resultScreen->vms[21], 9);
    g_ResultAuxAnm->SetAndExecuteScript(&resultScreen->vms[22], 10);
    resultScreen->vms[21].glyphWidth = 0x12;
    resultScreen->vms[21].glyphHeight = 0x12;
    resultScreen->vms[22].glyphWidth = 0x12;
    resultScreen->vms[22].glyphHeight = 0x12;
    g_ResultAnmManager->DrawTextLeft(
        &resultScreen->vms[21], 0xffe0c0, 0x300000, " ");
    g_ResultAnmManager->DrawTextLeft(
        &resultScreen->vms[22], 0xffe0c0, 0x300000, " ");
}

void __fastcall InitializeReplayResultScreen(ResultScreen *resultScreen)
{
    resultScreen->stateTimer.Reset();
    resultScreen->savedGameSpeed = g_AnmGameSpeed;
    g_AnmGameSpeed = 1.0f;
    g_ResultScreenGlobalState->flagsWord |= 0x10;
    g_ResultScreenGlobalState->flagsWord |= 0x80;

    InitializeResultCapturePhase();

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

        g_ResultAuxAnm->SetAndExecuteScript(&resultScreen->vms[21], 9);
        g_ResultAuxAnm->SetAndExecuteScript(&resultScreen->vms[22], 10);
        resultScreen->vms[21].glyphWidth = 0x12;
        resultScreen->vms[21].glyphHeight = 0x12;
        resultScreen->vms[22].glyphWidth = 0x12;
        resultScreen->vms[22].glyphHeight = 0x12;

        g_ResultAnmManager->DrawTextLeft(
            &resultScreen->vms[21], 0xffe0c0, 0x300000,
            resultScreen->sceneLabels[resultScreen->selectedGroup]
                [g_ResultSaveData->profile.nextSceneByGroup[
                    resultScreen->selectedGroup]].firstLine);
        g_ResultAnmManager->DrawTextLeft(
            &resultScreen->vms[22], 0xffe0c0, 0x300000,
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
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 19), 19);
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 20), 20);
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
    g_ResultScreenGlobalState->flagsWord |= 0x10;
    g_ResultScreenGlobalState->flagsWord |= 0x80;

    InitializeResultCapturePhase();

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
            InitializePhotoDisableCursorPhase(&resultScreen->replayCursor);
            resultScreen->vms[13].color1 = 0x80000000;
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
                (f32)(g_ReplayLagNumerator / g_ReplayLagDenominator) * 100.0f;
        }
    }
    else
    {
        resultScreen->state = 9;
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 19), 19);
        resultScreen->anm->SetAndExecuteScript(GetResultVm(resultScreen, 20), 20);
        InitializePhotoExtraCursorPhase(&resultScreen->replayCursor);
        resultScreen->replayCursor.count = 2;
    }
    resultScreen->replayCursor.wraps = 1;
    resultScreen->PrepareBestShot();
    time(reinterpret_cast<time_t *>(
        &g_ReplayManager->activeInputData->timestamp));
    g_ReplayManager->activeInputData->score =
        g_ResultScreenGlobalState->currentScore;
}

static __forceinline void PrepareBestShotCursorSetPhase(
    ResultScreenReplayCursor *cursor, i32 bestShot)
{
    u8 compilerStorage[0x38];
    cursor->Set(bestShot);
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
        PrepareBestShotCursorSetPhase(&this->photoCursor, bestShot);

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

static __forceinline void ResultPhotoInterruptPreviousPhase(
    ResultScreen *resultScreen)
{
    u8 compilerStorage[0x24];
    g_ResultPhotoData
        ->photoVms[resultScreen->photoCursor.GetPrevious()]
        .SetInterrupt(3);
}

static __forceinline void ResultPhotoInterruptCurrentPhase(i32 photoIndex)
{
    u8 compilerStorage[8];
    g_ResultPhotoData->photoVms[photoIndex].SetInterrupt(2);
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

            ResultPhotoInterruptPreviousPhase(resultScreen);
            ResultPhotoInterruptCurrentPhase(photoIndex);
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
        else if (g_ResultScreenGlobalState->replayResultActive != 0)
        {
            InitializeReplayResultScreen(this);
            break;
        }
        else if (g_ResultScreenGlobalState->photoResultActive != 0)
        {
            InitializePhotoResultScreen(this);
            break;
        }
        break;

    case 1:
        if (this->UpdateCursor(4) != 0)
        {
            break;
        }
        if (GetPressedButtons(8) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            this->state = 2;
            this->replayCursor.Set(0);
            this->stateTimer.Reset();
            ResultUpdateInterruptFirstPhase(this);
            this->vms[23].SetInterrupt(1);
            break;
        }
        else if (GetPressedButtons(0x1002) != 0)
        {
            this->SetState(2);
            switch (this->replayCursor.GetCurrent())
            {
            case 0:
                ResultUpdateInterruptFirstPhase(this);
                this->vms[23].SetInterrupt(1);
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

    case 2:
        if (this->stateTimer >= 8)
        {
            g_AnmGameSpeed = this->savedGameSpeed;
            this->state = 0;
            switch (this->replayCursor.GetCurrent())
            {
            case 0:
                g_ResultScreenGlobalState->flagsWord &= ~0x10;
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
            g_ResultScreenGlobalState->flagsWord |= 0x80;
        }
        break;

    case 11:
        if (this->UpdateCursor(16) != 0)
        {
            break;
        }
        if (GetPressedButtons(8) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            this->state = 12;
            this->replayCursor.Set(0);
            this->stateTimer.Reset();
            ResultUpdateInterruptFirstPhase(this);
            this->vms[23].SetInterrupt(1);
            break;
        }
        else if (GetPressedButtons(0x1002) != 0)
        {
            this->SetState(12);
            switch (this->replayCursor.GetCurrent())
            {
            case 0:
                g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                ResultUpdateInterruptFirstPhase(this);
                this->vms[23].SetInterrupt(1);
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

    case 12:
        if (this->stateTimer >= 8)
        {
            g_AnmGameSpeed = this->savedGameSpeed;
            this->state = 0;
            switch (this->replayCursor.GetCurrent())
            {
            case 0:
                g_ResultScreenGlobalState->flagsWord &= ~0x10;
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

    case 3:
        if (this->UpdateCursor(8) != 0)
        {
            break;
        }
        if (GetPressedButtons(0x1002) != 0)
        {
            this->SetState(4);
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

    case 4:
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
                this->state = 13;
                this->LoadReplays();
                this->stateTimer.Reset();
                break;
            case 1:
                g_ResultSceneState = 2;
                break;
            }
            g_ResultSaveData->WriteBestShotData();
            g_ResultScreenGlobalState->flagsWord |= 0x80;
        }
        break;

    case 5:
        if (this->UpdateCursor(12) != 0)
        {
            break;
        }
        else
        {
            UpdatePhotoResultScreen(this);
            if (GetPressedButtons(0x1002) != 0)
            {
                this->SetState(6);
                for (i32 i = 3; i < 21; i++)
                {
                    this->vms[i].SetInterrupt(1);
                }
                this->vms[23].SetInterrupt(1);
                switch (this->replayCursor.GetCurrent())
                {
                default:
                    g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                    break;
                }
            }
        }
        break;

    case 6:
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
                this->state = 13;
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
                g_ResultRuntime->scene = (i16)(g_ResultPlayerConfig->scene + 1);
                g_ResultPlayerConfig = (ResultPlayerConfigView *)(
                    g_ResultPlayerConfigTable[g_ResultPlayerConfig->group] +
                    (g_ResultPlayerConfig->scene + 1) * 0x30);
                g_ResultSceneState = 8;
                break;
            }
            g_ResultScreenGlobalState->flagsWord |= 0x80;
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
            g_ResultScreenGlobalState->flagsWord |= 0x80;
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

        if (GetPressedButtons(0x1002) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
            this->SetState(14);
            strcpy(this->replayName, g_ResultRuntime->replayName);
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
            if (g_ResultScreenGlobalState->replayResultActive != 0)
            {
                this->stateTimer.Reset();
                this->anm->InitializeVm(GetResultVm(this, 7), 7);
                this->state = 3;
                this->anm->InitializeVm(GetResultVm(this, 9), 9);
                this->anm->InitializeVm(GetResultVm(this, 10), 10);
                this->anm->InitializeVm(GetResultVm(this, 8), 8);
                this->replayCursor.Pop();
                this->replayCursor.count = 3;
                this->replayCursor.wraps = 1;
                this->replayNameCursor = 0;
                goto updateResultVms;
            }
            else if (g_ResultScreenGlobalState->photoResultActive != 0)
            {
                ResultUpdatePhotoSetStatePhase(this, 5);
                this->anm->InitializeVm(GetResultVm(this, 11), 11);
                this->state = 5;
                this->anm->InitializeVm(GetResultVm(this, 14), 14);
                this->anm->InitializeVm(GetResultVm(this, 15), 15);
                this->anm->InitializeVm(GetResultVm(this, 12), 12);
                this->anm->InitializeVm(GetResultVm(this, 13), 13);
                this->replayCursor.Pop();
                if (g_ResultPlayerConfig->scene >=
                    g_ResultSceneLimits[g_ResultPlayerConfig->group] - 1)
                {
                    ResultUpdateReplayDisablePhase(&this->replayCursor, 1);
                    this->vms[13].color1 = 0x80000000;
                }
                PreparePhotoResultScreen(this);
                this->replayCursor.count = 4;
                this->replayCursor.wraps = 1;
                break;
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

        if (GetPressedButtons(0x1002) != 0)
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
                this->SetState(13);
                this->replayCursor.Pop();
            }
        }
        break;

    case 15:
        sprintf(path, "th95_%.2d.rpy", this->replayCursor.GetCurrent() + 1);
        g_ReplayManager->WriteReplay(path, this->replayName);
        strcpy(g_ResultRuntime->replayName, this->replayName);
        this->SetState(13);
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

static __forceinline void DrawResultTotalScorePhase(Float3 *position)
{
    i32 totalScore = 0;
    for (u32 scoreIndex = 0; scoreIndex < 120; scoreIndex++)
    {
        totalScore += g_ResultSaveData->bestShotImages[scoreIndex].score;
    }
    position->x = 230.0f;
    position->y = 68.0f;
    position->z = 0.0f;
    g_ResultAsciiManager.AddFormatText(position, "Total Score %.8d", totalScore);
}

static __forceinline i32 ResultFindBestShotPhase()
{
    // TH095 Draw assigns a 12-byte compiler allocation phase to this
    // best-shot frontend. 0/4/8/16-byte controls and moving the same 12
    // bytes to the total-score or ordinary-shot phases do not replay exact.
    u8 compilerStorage[12];
    return g_ResultPhotoData->FindBestShot();
}

static __forceinline void DrawResultBestShotLinePhase(
    ResultScreen *resultScreen, Float3 *position)
{
    i32 photoIndex = resultScreen->photoCursor.GetCurrent();
    position->x = 230.0f;
    position->y = 290.0f;
    position->z = 0.0f;
    g_ResultAsciiManager.AddFormatText(
        position, "  Best Shot   %.6d",
        g_ResultPhotoData->slots[photoIndex].score);
}

static __forceinline void DrawResultShotLinePhase(
    ResultScreen *resultScreen, Float3 *position)
{
    i32 photoIndex = resultScreen->photoCursor.GetCurrent();
    position->x = 230.0f;
    position->y = 290.0f;
    position->z = 0.0f;
    g_ResultAsciiManager.AddFormatText(
        position, "       Shot   %.6d",
        g_ResultPhotoData->slots[photoIndex].score);
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
        reinterpret_cast<ResultAnmVmDrawView *>(&this->vms[i])->Draw();
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
        g_ResultAsciiManager.AddFormatText(
            &scorePosition, "      Score  %.7d",
            g_ResultScreenGlobalState->currentScore);

        highScorePosition.x = 230.0f;
        highScorePosition.y = 54.0f;
        highScorePosition.z = 0.0f;
        g_ResultAsciiManager.AddFormatText(
            &highScorePosition, " High Score  %.7d",
            g_ResultSaveData
                ->bestShotImages[g_ResultScreenGlobalState->bestShotIndex]
                .score);

        DrawResultTotalScorePhase(&totalScorePosition);

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
        slowRatePosition.x = 230.0f;
        slowRatePosition.y = 82.0f;
        slowRatePosition.z = 0.0f;
        g_ResultAsciiManager.AddFormatText(
            &slowRatePosition, "  Slow Rate      %2.0f%%",
            g_ResultSaveData
                ->bestShotImages[g_ResultScreenGlobalState->bestShotIndex]
                .slowRate);
        g_ResultAsciiManager.color = 0xffffffff;

        if (this->notificationTimer > 0)
        {
            g_ResultAsciiManager.color = 0xffffff00;
            notificationPosition.x = 212.0f;
            notificationPosition.y = 224.0f;
            notificationPosition.z = 0.0f;
            g_ResultAsciiManager.AddFormatText(
                &notificationPosition, "BestShot was overwrited!");
            g_ResultAsciiManager.color = 0xffffffff;
            this->notificationTimer--;
        }
        break;
    }

    case 13:
    {
        replayListTitlePosition.x = 160.0f;
        replayListTitlePosition.y = 32.0f;
        replayListTitlePosition.z = 0.0f;
        g_ResultAsciiManager.AddFormatText(
            &replayListTitlePosition, "Select Replay Number");

        replayListPosition.x = 144.0f;
        replayListPosition.y = 64.0f;
        replayListPosition.z = 0.0f;
        for (replayListIndex = 0; replayListIndex < 20; replayListIndex++)
        {
            if (this->replayCursor.GetCurrent() == replayListIndex)
            {
                g_ResultAsciiManager.color = 0xffffffff;
            }
            else
            {
                g_ResultAsciiManager.color = 0xff404040;
            }

            if (this->replays[replayListIndex] == NULL ||
                this->replays[replayListIndex]->activeInputData == NULL)
            {
                g_ResultAsciiManager.AddFormatText(
                    &replayListPosition, "No.%.2d %s %s-%s %s ------",
                    replayListIndex + 1, "--------", "--", "-", "--/-- --:--");
            }
            else
            {
                replayListTimestamp = localtime(
                    reinterpret_cast<time_t *>(
                        &this->replays[replayListIndex]
                             ->activeInputData->timestamp));

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
                g_ResultAsciiManager.AddFormatText(
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
        g_ResultAsciiManager.color = 0xffffffff;
        break;
    }

    case 14:
    {
        replayNameTitlePosition.x = 160.0f;
        replayNameTitlePosition.y = 32.0f;
        replayNameTitlePosition.z = 0.0f;
        g_ResultAsciiManager.AddFormatText(
            &replayNameTitlePosition, "Replay Name Regist");

        replayNamePosition.x = 144.0f;
        replayNamePosition.y = 128.0f;
        replayNamePosition.z = 0.0f;
        g_ResultAsciiManager.color = 0xa0ffffc0;
        replayNamePosition.x =
            (f32)(this->replayNameCursor * 9) + 198.0f;
        g_ResultAsciiManager.AddFormatText(&replayNamePosition, "_");

        replayNamePosition.x = 144.0f;
        g_ResultAsciiManager.color = 0xffffffff;
        replayIndex = this->replayCursor.current;
        replayNameTimestamp = localtime(
            reinterpret_cast<time_t *>(
                &g_ReplayManager->activeInputData->timestamp));
        if (g_ResultPlayerConfig->group == 10)
        {
            strcpy(replayNameLevelText, "EX");
        }
        else
        {
            sprintf(replayNameLevelText, " %d",
                    g_ResultPlayerConfig->group + 1);
        }
        sprintf(replayNameSceneText, "%d", g_ResultPlayerConfig->scene + 1);
        g_ResultAsciiManager.AddFormatText(
            &replayNamePosition,
            "No.%.2d %s %s-%s %.2d/%.2d %.2d:%.2d %6d",
            replayIndex + 1, this->replayName, replayNameLevelText,
            replayNameSceneText, replayNameTimestamp->tm_mon + 1,
            replayNameTimestamp->tm_mday, replayNameTimestamp->tm_hour,
            replayNameTimestamp->tm_min,
            g_ResultScreenGlobalState->currentScore);

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
                    g_ResultAsciiManager.color = 0xffffffc0;
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
                    g_ResultAsciiManager.scaleX = offsetY;
                    g_ResultAsciiManager.scaleY = offsetY;
                    offsetY = -(offsetY - 1.0f) * 4.0f;
                    offsetX = offsetY;
                }
                else
                {
                    g_ResultAsciiManager.color = 0xc0c0c0c0;
                    g_ResultAsciiManager.scaleX = 1.0f;
                    g_ResultAsciiManager.scaleY = 1.0f;
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
                g_ResultAsciiManager.AddString(
                    &characterPosition, characterText);
            }
            rowY += 16.0f;
        }
        g_ResultAsciiManager.scaleX = 1.0f;
        g_ResultAsciiManager.scaleY = 1.0f;
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
