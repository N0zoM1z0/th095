#ifdef TH095_MATCH_EXACT
#include "PhotoStageExact.inl"
#else
#include "PhotoGamePlayer.hpp"
#include "PhotoBulletManager.hpp"
#include "PhotoCardInfo.hpp"
#include "PhotoStage.hpp"
#include "PhotoFront.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoGameTask.hpp"
#include "Main.hpp"
#include "ScoreData.hpp"
#include "SceneData.hpp"
#include "ScreenEffect.hpp"
#ifndef DIFFBUILD
#include "PhotoEffectRuntime.hpp"
#include "ReplayManager.hpp"
#endif

#include <stdlib.h>
#include <string.h>
#include <time.h>

namespace th095
{

enum PhotoStageFlags
{
    PHOTO_STAGE_CAPTURING = 1 << 0,
    PHOTO_STAGE_WAITING_FOR_TEXTURE = 1 << 1,
    PHOTO_STAGE_FIRST_CAPTURE_FRAME = 1 << 2,
    PHOTO_STAGE_PLAYER_PASSED = 1 << 3,
};

typedef AnmLoaded PhotoStageAnmLoadedView;

#ifdef DIFFBUILD
extern PhotoCardInfoView *g_PhotoStageRuntime;
#define TH095_PHOTO_STAGE_CARD_INFO g_PhotoStageRuntime

struct PhotoStageEffectManagerView
{
    i32 CommitCapturedObjects();
};

struct PhotoStageBulletManagerView
{
    i32 ClearCapturedBullets();
};
#endif

struct PhotoStageSaveLocals
{
    f32 entryX;
    f32 photoX;
    PhotoGameTaskView *globalState;
    Float3 entryPosition;
    Float3 photoPosition;
};

struct PhotoStageCaptureLocals
{
    ResultBestShotRecordView *record;
    i32 allocationSize;
    u8 *destination;
    u8 *source;
    i32 y;
    i32 x;
    D3DLOCKED_RECT lockedRect;
    IDirect3DSurface8 *surface;
};

typedef char PhotoStageScoreEntryTailSizeIs48[
    (sizeof(ResultScoreEntryView) - offsetof(ResultScoreEntryView, detailScore) ==
     0x48)
        ? 1
        : -1];

#ifdef DIFFBUILD
extern PhotoStageEffectManagerView *g_PhotoStageEffectManager;
#endif

#ifndef DIFFBUILD
#define TH095_PHOTO_STAGE_CARD_INFO g_PhotoCardInfo
#endif

#ifdef DIFFBUILD
extern PhotoStageBulletManagerView *g_PhotoStageBulletManager;
#endif
// Target 0x004C45F8 is an independent zero-initialized byte published when a
// captured texture becomes ready.
DIFFABLE_STATIC(u8, g_PhotoCaptureCountdown);

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

static __forceinline AnmVmId PhotoStageAnmId(i32 value)
{
    AnmVmId id;
    id.value = value;
    return id;
}

#define TH095_PHOTO_STAGE_ANM_GET_VM(id)     g_AnmManager->GetVm(PhotoStageAnmId(id))
#define TH095_PHOTO_STAGE_ANM_SET_INTERRUPT(id, interrupt)     g_AnmManager->SetInterrupt(PhotoStageAnmId(id), (interrupt))
#define TH095_PHOTO_STAGE_ANM_MARK_DELETE(id)     g_AnmManager->MarkVmForDeletion(PhotoStageAnmId(id))
#define TH095_PHOTO_STAGE_ANM_SET_POSITION(id, position)     g_AnmManager->SetPosition(PhotoStageAnmId(id), (position))

static __forceinline void PhotoStageInterruptCurrentEntryPhase(
    PhotoStageStateView *state, i32 &entryIndex)
{
    u8 compilerStorage[8];
    entryIndex = g_RuntimePlayerOwner->camera.photoIndex - 1;
    TH095_PHOTO_STAGE_ANM_SET_INTERRUPT(
        state->capture.entryVms[entryIndex].value, 1);
}

static inline ResultScoreEntryView *GetPhotoStageScoreEntry(i32 index)
{
    return &g_ResultSaveData->scoreEntries[index];
}

static inline ResultBestShotRecordView *GetPhotoStageBestShotRecord(i32 index)
{
    return &g_ResultSaveData->bestShotRecords[index];
}

static __forceinline void ClearPhotoStageGlobalCaptureActive(
    PhotoGameTaskView *state)
{
    state->captureActive = 0;
}

static __forceinline void ClearPhotoStageGlobalCapturedPhotoActive(
    PhotoGameTaskView *state)
{
    state->capturedPhotoActive = 0;
}

static __forceinline void SetPhotoStageGlobalCapturedPhotoActive(
    PhotoGameTaskView *state)
{
    state->capturedPhotoActive = 1;
}

void __fastcall InitializePhotoStageDisplayVm(
    AnmVm *vm, const Float3 *position, i32 spriteIndex, i32 renderMode)
{
    u8 unknownStack[0x2c];

    reinterpret_cast<PhotoStageAnmLoadedView *>(g_PhotoStageState->anm)
        ->InitializeVm(vm, 0x23);
    reinterpret_cast<PhotoStageAnmLoadedView *>(g_PhotoStageState->anm)
        ->SetSprite(vm, spriteIndex);
    vm->positionOffset = *position;
    vm->intVar0 = renderMode;
    vm->counterVar1 = 1;
}

static inline u32 GetPhotoStagePixelCount(u32 width, u32 height)
{
    return width * height;
}

#ifdef DIFFBUILD
enum PhotoStageScoreFlags
{
    PHOTO_STAGE_SCORE_ENEMY = 1 << 0,
    PHOTO_STAGE_SCORE_SELF = 1 << 1,
    PHOTO_STAGE_SCORE_TWO_SHOT = 1 << 2,
    PHOTO_STAGE_SCORE_BOSS_RATE = 1 << 3,
    PHOTO_STAGE_SCORE_NEARBY = 1 << 4,
    PHOTO_STAGE_SCORE_UNKNOWN_5 = 1 << 5,
    PHOTO_STAGE_SCORE_COLOR_1 = 1 << 6,
    PHOTO_STAGE_SCORE_COLOR_2 = 1 << 7,
    PHOTO_STAGE_SCORE_COLOR_3 = 1 << 8,
    PHOTO_STAGE_SCORE_COLOR_4 = 1 << 9,
    PHOTO_STAGE_SCORE_COLOR_5 = 1 << 10,
    PHOTO_STAGE_SCORE_COLOR_6 = 1 << 11,
    PHOTO_STAGE_SCORE_COLOR_7 = 1 << 12,
    PHOTO_STAGE_SCORE_COLORFUL = 1 << 13,
    PHOTO_STAGE_SCORE_RAINBOW = 1 << 14,
    PHOTO_STAGE_SCORE_EMPTY = 1 << 15,
    PHOTO_STAGE_SCORE_NO_BULLETS = 1 << 16,
    PHOTO_STAGE_SCORE_UNKNOWN_17 = 1 << 17,
    PHOTO_STAGE_SCORE_UNKNOWN_18 = 1 << 18,
    PHOTO_STAGE_SCORE_UNKNOWN_19 = 1 << 19,
};
#define PHOTO_SCORE_ENEMY PHOTO_STAGE_SCORE_ENEMY
#define PHOTO_SCORE_SELF PHOTO_STAGE_SCORE_SELF
#define PHOTO_SCORE_TWO_SHOT PHOTO_STAGE_SCORE_TWO_SHOT
#define PHOTO_SCORE_BOSS_RATE PHOTO_STAGE_SCORE_BOSS_RATE
#define PHOTO_SCORE_NEARBY PHOTO_STAGE_SCORE_NEARBY
#define PHOTO_SCORE_UNKNOWN_5 PHOTO_STAGE_SCORE_UNKNOWN_5
#define PHOTO_SCORE_COLOR_1 PHOTO_STAGE_SCORE_COLOR_1
#define PHOTO_SCORE_COLOR_2 PHOTO_STAGE_SCORE_COLOR_2
#define PHOTO_SCORE_COLOR_3 PHOTO_STAGE_SCORE_COLOR_3
#define PHOTO_SCORE_COLOR_4 PHOTO_STAGE_SCORE_COLOR_4
#define PHOTO_SCORE_COLOR_5 PHOTO_STAGE_SCORE_COLOR_5
#define PHOTO_SCORE_COLOR_6 PHOTO_STAGE_SCORE_COLOR_6
#define PHOTO_SCORE_COLOR_7 PHOTO_STAGE_SCORE_COLOR_7
#define PHOTO_SCORE_COLORFUL PHOTO_STAGE_SCORE_COLORFUL
#define PHOTO_SCORE_RAINBOW PHOTO_STAGE_SCORE_RAINBOW
#define PHOTO_SCORE_EMPTY PHOTO_STAGE_SCORE_EMPTY
#define PHOTO_SCORE_NO_BULLETS PHOTO_STAGE_SCORE_NO_BULLETS
#define PHOTO_SCORE_UNKNOWN_17 PHOTO_STAGE_SCORE_UNKNOWN_17
#define PHOTO_SCORE_UNKNOWN_18 PHOTO_STAGE_SCORE_UNKNOWN_18
#define PHOTO_SCORE_UNKNOWN_19 PHOTO_STAGE_SCORE_UNKNOWN_19
#endif

#define ADD_PHOTO_STAGE_DISPLAY_VM(spriteIndex)                              \
    {                                                                        \
        InitializePhotoStageDisplayVm(                                      \
            &g_PhotoStageState->displayVms[displayVmCount++],                \
            &displayPosition, (spriteIndex), renderMode);                    \
    }

#define ADD_PHOTO_STAGE_SCORE_ROW(labelSprite, value)                       \
    ADD_PHOTO_STAGE_DISPLAY_VM(labelSprite);                                 \
    displayPosition.x += 90.0f;                                              \
    if ((value) >= 1000)                                                     \
    {                                                                        \
        ADD_PHOTO_STAGE_DISPLAY_VM(((value) / 1000) % 10 + 15);              \
    }                                                                        \
    displayPosition.x += 9.0f;                                               \
    if ((value) >= 100)                                                      \
    {                                                                        \
        ADD_PHOTO_STAGE_DISPLAY_VM(((value) / 100) % 10 + 15);               \
    }                                                                        \
    displayPosition.x += 9.0f;                                               \
    if ((value) >= 10)                                                       \
    {                                                                        \
        ADD_PHOTO_STAGE_DISPLAY_VM(((value) / 10) % 10 + 15);                \
    }                                                                        \
    displayPosition.x += 9.0f;                                               \
    ADD_PHOTO_STAGE_DISPLAY_VM((value) % 10 + 15);                           \
    displayPosition.x = photoPositionCopy.x;                               \
    renderMode += 4;                                                         \
    displayPosition.y += 12.0f

// Stock VC7.1 hash buckets recovered from the exact ResultScreen::Draw oracle.
// Both backing objects are fully live; the aliases only select their physical
// stack rank without changing the semantic names used by Build.
#define digitPosition resultDrawBacking000
#define displayVmCount resultDrawBacking022

void PhotoStageSlot::Build(
    i32 score, Float3 *photoPosition, Float3 *entryPosition,
    const PhotoScoreBreakdownView *scoreData)
{
    Float3 displayPosition = *photoPosition;
    struct PhotoStageDigitPosition
    {
        f32 x;
        f32 y;
        f32 z;
    } digitPosition;
    Float3 photoPositionCopy = displayPosition;
    this->score = score;
    if (scoreData != NULL)
    {
        this->scoreBreakdown = *scoreData;
    }

    if (entryPosition != NULL)
    {
        digitPosition =
            *reinterpret_cast<const PhotoStageDigitPosition *>(entryPosition);
        i32 digit = score / 100000;
        i32 leadingDigitVisible;

        leadingDigitVisible = 0;
        if (digit != 0)
        {
            g_PhotoStageState->anm->InitializeVm(&this->overlayVms[0], 0x1e);
            g_PhotoStageState->anm->SetSprite(
                &this->overlayVms[0], digit + 15);
            leadingDigitVisible = 1;
        }
        this->overlayVms[0].positionOffset =
            *reinterpret_cast<const Float3 *>(&digitPosition);
        digitPosition.x += 9.0f;

        digit = (score / 10000) % 10;
        if (digit != 0 || leadingDigitVisible != 0)
        {
            g_PhotoStageState->anm->InitializeVm(&this->overlayVms[1], 0x1e);
            g_PhotoStageState->anm->SetSprite(
                &this->overlayVms[1], digit + 15);
            leadingDigitVisible = 1;
        }
        this->overlayVms[1].positionOffset =
            *reinterpret_cast<const Float3 *>(&digitPosition);
        digitPosition.x += 9.0f;

        digit = (score / 1000) % 10;
        if (digit != 0 || leadingDigitVisible != 0)
        {
            g_PhotoStageState->anm->InitializeVm(&this->overlayVms[2], 0x1e);
            g_PhotoStageState->anm->SetSprite(
                &this->overlayVms[2], digit + 15);
            leadingDigitVisible = 1;
        }
        this->overlayVms[2].positionOffset =
            *reinterpret_cast<const Float3 *>(&digitPosition);
        digitPosition.x += 9.0f;

        digit = (score / 100) % 10;
        if (digit != 0 || leadingDigitVisible != 0)
        {
            g_PhotoStageState->anm->InitializeVm(&this->overlayVms[3], 0x1e);
            g_PhotoStageState->anm->SetSprite(
                &this->overlayVms[3], digit + 15);
            leadingDigitVisible = 1;
        }
        this->overlayVms[3].positionOffset =
            *reinterpret_cast<const Float3 *>(&digitPosition);
        digitPosition.x += 9.0f;

        digit = (score / 10) % 10;
        g_PhotoStageState->anm->InitializeVm(&this->overlayVms[4], 0x1e);
        g_PhotoStageState->anm->SetSprite(
            &this->overlayVms[4], digit + 15);
        this->overlayVms[4].positionOffset =
            *reinterpret_cast<const Float3 *>(&digitPosition);
        digitPosition.x += 9.0f;

        digit = score % 10;
        g_PhotoStageState->anm->InitializeVm(&this->overlayVms[5], 0x1e);
        g_PhotoStageState->anm->SetSprite(
            &this->overlayVms[5], digit + 15);
        this->overlayVms[5].positionOffset =
            *reinterpret_cast<const Float3 *>(&digitPosition);
    }

    i32 renderMode = 4;
    displayPosition = photoPositionCopy;
    displayPosition.y += 16.0f;
    i32 displayVmCount = 0;
    memset(
        g_PhotoStageState->displayVms, 0,
        sizeof(g_PhotoStageState->displayVms));
    g_PhotoStageState->flags &= ~PHOTO_STAGE_PLAYER_PASSED;

    {
        // PhotoFrontManagerView::Initialize independently proves the same
        // target 0x108 shallow-to-hidden-this allocation phase on the first
        // direct display-VM initialization.  In this larger body VC7.1 needs
        // the phase split into two source allocation classes: a single
        // 0x108/0x104 block rotates this behind the 92 compiler value temps,
        // while 0x100+4 gives the target this -> tv chronology exactly.
        u8 compilerStorage[0x100];
        u8 compilerStorage4[4];
        if ((scoreData->scoringFlags & PHOTO_SCORE_UNKNOWN_5) != 0)
        {
            ADD_PHOTO_STAGE_DISPLAY_VM(0x24);
            renderMode += 4;
            displayPosition.y += 12.0f;
        }
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_COLOR_1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x25, 300);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_COLOR_2) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x26, 300);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_COLOR_3) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x27, 300);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_COLOR_4) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x28, 300);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_COLOR_5) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x29, 300);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_COLOR_6) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2a, 300);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_COLOR_7) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2b, 300);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_COLORFUL) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2c, 900);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_RAINBOW) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2d, 2100);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_EMPTY) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2e, 0);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_NO_BULLETS) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2f, 100);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_UNKNOWN_17) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x30, 100);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_UNKNOWN_18) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x31, 0);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_UNKNOWN_19) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x32, 0);
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_NEARBY) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x23, scoreData->nearbyTargetBonus);
    }

    if ((scoreData->scoringFlags & PHOTO_SCORE_ENEMY) != 0)
    {
        ADD_PHOTO_STAGE_DISPLAY_VM(0x1f);
        displayPosition.x += 99.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(
            (i32)(scoreData->enemyDistanceMultiplier * 10.0f) /
                10 + 15);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x1a);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(
            (i32)(scoreData->enemyDistanceMultiplier * 10.0f) %
                10 + 15);
        renderMode += 4;
        displayPosition.x = photoPositionCopy.x;
        displayPosition.y += 12.0f;
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_SELF) != 0)
    {
        ADD_PHOTO_STAGE_DISPLAY_VM(0x20);
        displayPosition.x += 99.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x10);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x1a);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x11);
        renderMode += 4;
        displayPosition.x = photoPositionCopy.x;
        displayPosition.y += 12.0f;
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_TWO_SHOT) != 0)
    {
        ADD_PHOTO_STAGE_DISPLAY_VM(0x21);
        displayPosition.x += 99.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x10);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x1a);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x14);
        renderMode += 4;
        displayPosition.x = photoPositionCopy.x;
        displayPosition.y += 12.0f;
    }
    if ((scoreData->scoringFlags & PHOTO_SCORE_BOSS_RATE) != 0)
    {
        ADD_PHOTO_STAGE_DISPLAY_VM(0x22);
        displayPosition.x += 99.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(
            (i32)(scoreData->bossRateMultiplier * 10.0f) /
                10 + 15);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x1a);
        displayPosition.x += 9.0f;
        InitializePhotoStageDisplayVm(
            &g_PhotoStageState->displayVms[displayVmCount++],
            &displayPosition,
            (i32)(scoreData->bossRateMultiplier * 10.0f) %
                    10 +
                15,
            renderMode);
        renderMode += 4;
        displayPosition.x = photoPositionCopy.x;
        displayPosition.y += 12.0f;
    }

    g_PhotoStageState->displayBoundary = displayPosition;
}

#undef digitPosition
#undef displayVmCount
#undef ADD_PHOTO_STAGE_SCORE_ROW
#undef ADD_PHOTO_STAGE_DISPLAY_VM

#include "PhotoStageCaptureRequest.inl"

static __forceinline void PhotoStageAccumulateCapturedScore(PhotoStageStateView *state)
{
    if ((state->slots[state->capture.slot].scoreBreakdown.scoringFlags &
         PHOTO_SCORE_ENEMY) != 0)
    {
        PhotoGameTaskView *globalState;
        i32 bgmFormatIndexLocal05 =
            state->slots[state->capture.slot].score;
        globalState = g_RuntimeGlobalStateOwner;
        globalState->score += bgmFormatIndexLocal05;
    }
}

static __forceinline void PhotoStagePublishSlowRate(PhotoStageStateView *state)
{
    state->slots[state->capture.slot].slowRate =
        GetPhotoStageBestShotRecord(g_RuntimeGlobalStateOwner->bestShotIndex)->slowRate =
            100.0f -
            (f32)(g_Supervisor.lagNumerator / g_Supervisor.lagDenominator) * 100.0f;
}

struct PhotoStageDisplayRowView
{
    AnmVm primaryVms[6];
    AnmVm overlayVms[6];
    u8 trailing[0x2214 - 0x10c8 - 6 * sizeof(AnmVm)];
};
typedef char PhotoStageDisplayRowViewSizeIs2214[
    (sizeof(PhotoStageDisplayRowView) == 0x2214) ? 1 : -1];
struct PhotoStageStateDisplayAccessorView
{
    u8 unknown000[0x44];
    PhotoStageDisplayRowView rows[11];
};
struct PhotoStageAlphaPhaseLocals
{
    i32 interpolationMode;
    i32 initialAlpha;
    ZunTimer *endTimer;
    ZunTimer *currentTimer;
};
typedef char PhotoStageAlphaPhaseLocalsSizeIs10[
    (sizeof(PhotoStageAlphaPhaseLocals) == 0x10) ? 1 : -1];
static __forceinline void PhotoStageSetDisplayAlphaPhase(AnmVm *vm, u8 finalAlpha)
{
    PhotoStageAlphaPhaseLocals locals;
    locals.initialAlpha = vm->color1.a;
    locals.interpolationMode = ANM_INTERP_LINEAR;
    locals.currentTimer = &vm->interpCurrentTimers[ANM_INTERP_ALPHA1];
    locals.currentTimer->current = 0;
    locals.currentTimer->subFrame = 0.0f;
    locals.currentTimer->previous = -999999;
    locals.endTimer = &vm->interpEndTimers[ANM_INTERP_ALPHA1];
    locals.endTimer->current = 16;
    locals.endTimer->subFrame = 16.0f;
    locals.endTimer->previous = -999999;
    vm->interpModes[ANM_INTERP_ALPHA1] = (u8)locals.interpolationMode;
    vm->color1Initial.a = (u8)locals.initialAlpha;
    vm->color1Final.a = finalAlpha;
}

struct PhotoStageTextureClearLocalsOracle
{
    u8 *row;
    i32 y;
    D3DLOCKED_RECT lockedRect;
    IDirect3DSurface8 *surface;
};

static __forceinline f32 PhotoStageEntryXValue(i32 index)
{
    f32 value;
    if (index >= 5)
    {
        value = 576.0f;
    }
    else
    {
        value = 64.0f;
    }
    return value;
}

static __forceinline i32 PhotoStageEntryVmIsZero(const PhotoAnmVmId &id)
{
    return id == 0;
}

static __forceinline void PhotoStageInitFrame35Position(Float3 *position, f32 y)
{
    position->x = 0.0f;
    position->y = y;
    position->z = 0.0f;
}

#define i resultDrawBacking157
#define j resultDrawBacking153
#define k resultDrawBacking146
#define rawTop resultDrawBacking142
#define rawLeft resultDrawBacking096
#define top resultDrawBacking092
#define captureVm resultDrawBacking026
#define bottom resultDrawBacking022
#define resultDrawBacking012 resultDrawBacking139
#define right resultDrawBacking135
#define readByteCountLocal02 resultDrawBacking131
#define resultDrawBacking063 resultDrawBacking119
#define resultDrawFrameIndex000 resultDrawBacking115
#define frame35Vm resultDrawBacking111
#define entryIndex resultDrawBacking089
#define frame35Position resultDrawBacking085
#define fadeVm1 resultDrawBacking081
#define fadeIndex1 resultDrawBacking017
#define fadeVm2 resultDrawBacking013
#define fadeIndex2 resultDrawBacking127
#define executeVmIndex resultDrawBacking123
#define entryPosition resultDrawBacking107
i32 PhotoStageStateView::Update()
{
    Float3 entryPosition;
    i32 executeVmIndex;
    i32 fadeIndex2;
    AnmVm *fadeVm2;
    i32 fadeIndex1;
    AnmVm *fadeVm1;
    Float3 frame35Position;
    i32 entryIndex;
    AnmVm *frame35Vm;
    i32 resultDrawFrameIndex000;
    PhotoStageTextureClearLocalsOracle resultDrawBacking063;
    i32 readByteCountLocal02;
    i32 right;
    Float3 resultDrawBacking012;
    i32 bottom;
    AnmVm *captureVm;
    i32 top;
    i32 rawLeft;
    i32 rawTop;
    i32 k;
    i32 j;
    i32 i;

    for (i = 0; i < 11; i++)
    {
        for (j = 0; j < 6; j++)
        {
            AnmManager::ExecuteScript(
                &reinterpret_cast<PhotoStageStateDisplayAccessorView *>(this)
                     ->rows[i].primaryVms[j]);
            AnmManager::ExecuteScript(
                &reinterpret_cast<PhotoStageStateDisplayAccessorView *>(this)
                     ->rows[i].overlayVms[j]);
        }
    }

    if (PhotoStageEntryVmIsZero(this->capture.entryVms[0]) &&
        g_RuntimePlayerOwner->camera.photoLimit > 0)
    {
        for (k = 0; k < g_RuntimePlayerOwner->camera.photoLimit; k++)
        {
            entryPosition.x = PhotoStageEntryXValue(k);
            entryPosition.y = 400.0f - (f32)(k % 5) * 80.0f;
            entryPosition.z = 0.0f;
            this->capture.entryVms[k] =
                g_PhotoFrontManager->frontAnm->CreateVmAtScreen(
                    12, &entryPosition);
        }
    }

    if (this->capturing != 0)
    {
        if (this->captureFrame == 1)
        {
            PhotoToScreen(&resultDrawBacking012, &this->capture.position);

            if (this->capture.width > 0)
            {
                readByteCountLocal02 = (i32)resultDrawBacking012.x -
                    (this->capture.width - 6) / 2;
                rawLeft = readByteCountLocal02;
                right = readByteCountLocal02 - 6 + this->capture.width;

                if ((f32)readByteCountLocal02 < 128.0f)
                {
                    readByteCountLocal02 = 128;
                }
                if ((f32)right >= 512.0f)
                {
                    right = 511;
                }

                top = (i32)resultDrawBacking012.y -
                    (this->capture.height - 6) / 2;
                rawTop = top;
                bottom = top - 6 + this->capture.height;

                if ((f32)top < 16.0f)
                {
                    top = 16;
                }
                if ((f32)bottom >= 464.0f)
                {
                    bottom = 463;
                }

                this->capture.width = right - readByteCountLocal02 + 6;
                this->capture.height = bottom - top + 6;

                PhotoStagePublishCaptureRequestArgs(
                    g_AnmManager, this->capture.slot,
                    readByteCountLocal02, right, top, bottom);

                g_PhotoCaptureCountdown = 99;
                g_RuntimeGlobalStateOwner->resetFpsSample = 1;
                this->flags &= ~PHOTO_STAGE_WAITING_FOR_TEXTURE;
            }
            else
            {
                this->capture.width = 64;
                this->capture.height = 48;
                this->flags |= PHOTO_STAGE_WAITING_FOR_TEXTURE;
            }

            resultDrawBacking063.surface = NULL;
            this->anm->textures
                [this->capture.slot]
                .texture->GetSurfaceLevel(0, &resultDrawBacking063.surface);
            resultDrawBacking063.surface->LockRect(&resultDrawBacking063.lockedRect, NULL, 0);

            for (resultDrawBacking063.y = 0; resultDrawBacking063.y < this->capture.height; resultDrawBacking063.y++)
            {
                resultDrawBacking063.row = reinterpret_cast<u8 *>(resultDrawBacking063.lockedRect.pBits) +
                    resultDrawBacking063.y * resultDrawBacking063.lockedRect.Pitch;
                memset(
                    resultDrawBacking063.row,
                    0,
                    this->capture.width *
                        this->anm->textures[this->capture.slot]
                            .bytesPerPixel);
            }
            for (resultDrawBacking063.y = 0; resultDrawBacking063.y < 3; resultDrawBacking063.y++)
            {
                resultDrawBacking063.row = reinterpret_cast<u8 *>(resultDrawBacking063.lockedRect.pBits) +
                    resultDrawBacking063.y * resultDrawBacking063.lockedRect.Pitch;
                memset(
                    resultDrawBacking063.row,
                    0xff,
                    this->capture.width *
                        this->anm->textures[this->capture.slot]
                            .bytesPerPixel);
            }
            for (resultDrawBacking063.y = this->capture.height - 3;
                 resultDrawBacking063.y < this->capture.height;
                 resultDrawBacking063.y++)
            {
                resultDrawBacking063.row = reinterpret_cast<u8 *>(resultDrawBacking063.lockedRect.pBits) +
                    resultDrawBacking063.y * resultDrawBacking063.lockedRect.Pitch;
                memset(
                    resultDrawBacking063.row,
                    0xff,
                    this->capture.width *
                        this->anm->textures[this->capture.slot]
                            .bytesPerPixel);
            }
            for (resultDrawBacking063.y = 3; resultDrawBacking063.y < this->capture.height - 3; resultDrawBacking063.y++)
            {
                resultDrawBacking063.row = reinterpret_cast<u8 *>(resultDrawBacking063.lockedRect.pBits) +
                    resultDrawBacking063.y * resultDrawBacking063.lockedRect.Pitch;
                memset(
                    resultDrawBacking063.row, 0xff,
                    this->anm->textures[this->capture.slot]
                            .bytesPerPixel *
                        3);
                memset(
                    resultDrawBacking063.row + (this->capture.width - 3) *
                        this->anm->textures[this->capture.slot]
                            .bytesPerPixel,
                    0xff,
                    this->anm->textures[this->capture.slot]
                            .bytesPerPixel *
                        3);
            }

            resultDrawBacking063.surface->UnlockRect();
            resultDrawBacking063.surface->Release();

            if (this->capturedPhotoVms[this->capture.slot] != 0)
            {
                TH095_PHOTO_STAGE_ANM_MARK_DELETE(
                    this->capturedPhotoVms[this->capture.slot].value);
            }
            this->capturedPhotoVms[this->capture.slot] =
                reinterpret_cast<PhotoStageAnmLoadedView *>(this->anm)->CreateVm(
                    this->capture.slot * 2, 0);

            captureVm =
                TH095_PHOTO_STAGE_ANM_GET_VM(
                    this->capturedPhotoVms[this->capture.slot].value);
            captureVm->loadedSprite->uvEnd.x =
                (f32)this->capture.width / 256.0f;
            captureVm->loadedSprite->uvEnd.y =
                (f32)this->capture.height / 256.0f;
            captureVm->spriteSize.x = (f32)this->capture.width;
            captureVm->spriteSize.y = (f32)this->capture.height;
            TH095_PHOTO_STAGE_ANM_SET_POSITION(
                this->capturedPhotoVms[this->capture.slot].value,
                &resultDrawBacking012);

            ClearPhotoStageGlobalCaptureActive(g_RuntimeGlobalStateOwner);
            SetPhotoStageGlobalCapturedPhotoActive(
                g_RuntimeGlobalStateOwner);
        }
        else if (this->captureFrame == 2)
        {
            if (this->waitingForTexture == 0)
            {
                ScreenEffect::RegisterChain(
                    SCREEN_EFFECT_ARCADE_PULSE, 15, 1, 0xc0ffafcf, 0, 0x1d);

                PhotoStageAccumulateCapturedScore(this);

                if (g_RuntimeGlobalStateOwner->replayMode == REPLAY_MANAGER_RECORD)
                {
                    if (GetPhotoStageScoreEntry(
                            g_RuntimeGlobalStateOwner->bestShotIndex)
                            ->attemptCount < 999999)
                    {
                        GetPhotoStageScoreEntry(
                            g_RuntimeGlobalStateOwner->bestShotIndex)
                            ->attemptCount++;
                    }

                    this->slots[this->capture.slot].timestamp =
                        (i32)time(NULL);

                    PhotoStagePublishSlowRate(this);
                    this->slots[this->capture.slot].width =
                        this->capture.width;
                    this->slots[this->capture.slot].height =
                        this->capture.height;
                    if (TH095_PHOTO_STAGE_CARD_INFO != NULL)
                    {
                        strcpy(
                            this->slots[this->capture.slot].comment,
                            TH095_PHOTO_STAGE_CARD_INFO->text);
                    }
                    else
                    {
                        memset(
                            this->slots[this->capture.slot].comment, 0,
                            sizeof(this->slots[this->capture.slot].comment));
                    }

                    if (!GetPhotoStageScoreEntry(
                             g_RuntimeGlobalStateOwner->bestShotIndex)
                             ->flagBits.bestShotLocked &&
                        this->slots[this->capture.slot].score >
                            GetPhotoStageScoreEntry(
                                g_RuntimeGlobalStateOwner->bestShotIndex)
                                ->scoreBreakdown.finalScore)
                    {
                        memcpy(
                            &GetPhotoStageScoreEntry(
                                 g_RuntimeGlobalStateOwner->bestShotIndex)
                                 ->scoreBreakdown,
                            &this->slots[this->capture.slot].scoreBreakdown,
                            sizeof(PhotoScoreBreakdownView));
                        g_ResultSaveData->UpdateBestShotRecord(
                            g_RuntimeGlobalStateOwner->bestShotIndex);

                        GetPhotoStageBestShotRecord(
                            g_RuntimeGlobalStateOwner->bestShotIndex)->valid = 1;
                        GetPhotoStageBestShotRecord(
                            g_RuntimeGlobalStateOwner->bestShotIndex)->magic =
                            0x53545342;
                        GetPhotoStageBestShotRecord(
                            g_RuntimeGlobalStateOwner->bestShotIndex)->width =
                            (u16)this->capture.width;
                        GetPhotoStageBestShotRecord(
                            g_RuntimeGlobalStateOwner->bestShotIndex)->height =
                            (u16)this->capture.height;
                        GetPhotoStageBestShotRecord(
                            g_RuntimeGlobalStateOwner->bestShotIndex)->score =
                                this->slots[this->capture.slot]
                                    .score;
                        GetPhotoStageBestShotRecord(
                            g_RuntimeGlobalStateOwner->bestShotIndex)->group =
                                (u16)(g_SelectedScene->group + 1);
                        GetPhotoStageBestShotRecord(
                            g_RuntimeGlobalStateOwner->bestShotIndex)->scene =
                                (u16)(g_SelectedScene->scene + 1);
                        GetPhotoStageBestShotRecord(
                            g_RuntimeGlobalStateOwner->bestShotIndex)->payloadFormat =
                            RESULT_BEST_SHOT_PAYLOAD_COMMENT_AND_COMPRESSED_PIXELS;
                        GetPhotoStageBestShotRecord(
                            g_RuntimeGlobalStateOwner->bestShotIndex)->version =
                            0x102;
                        GetPhotoStageBestShotRecord(
                            g_RuntimeGlobalStateOwner->bestShotIndex)
                            ->componentCount =
                            (u8)((this->anm->textures
                                      [this->capture.slot]
                                          .bytesPerPixel == 4) +
                                2);
                        GetPhotoStageScoreEntry(
                            g_RuntimeGlobalStateOwner->bestShotIndex)->bestShotSlowRate =
                            this->slots[this->capture.slot].slowRate;
                        GetPhotoStageScoreEntry(
                            g_RuntimeGlobalStateOwner->bestShotIndex)->captureTime =
                            this->slots[this->capture.slot].timestamp;
                        if (TH095_PHOTO_STAGE_CARD_INFO != NULL)
                        {
                            strcpy(
                                GetPhotoStageBestShotRecord(
                                    g_RuntimeGlobalStateOwner->bestShotIndex)
                                    ->comment,
                                TH095_PHOTO_STAGE_CARD_INFO->text);
                        }
                        else
                        {
                            memset(
                                GetPhotoStageBestShotRecord(
                                    g_RuntimeGlobalStateOwner->bestShotIndex)
                                    ->comment,
                                0, 0x50);
                        }

                        if (this->capture.slot != 10)
                        {
                            GetPhotoStageBestShotRecord(
                                g_RuntimeGlobalStateOwner->bestShotIndex)
                                ->photoIndex = this->capture.slot;
                        }
                        else
                        {
                            GetPhotoStageBestShotRecord(
                                g_RuntimeGlobalStateOwner->bestShotIndex)
                                ->photoIndex = -1;
                            this->CapturePhotoPixels(
                                this->capture.slot);
                        }
                    }
                }

#ifdef DIFFBUILD
                g_PhotoStageEffectManager->CommitCapturedObjects();
                g_PhotoStageBulletManager->ClearCapturedBullets();
#else
                PhotoEffectManagerView::CheckCollisionStored(
                    TH095_RUNTIME_GLOBAL_PTR(
                        PhotoEffectManagerView, g_RuntimeEffectManagerOwner));
                TH095_RUNTIME_GLOBAL_PTR(
                    PhotoBulletManagerView, g_RuntimeBulletManagerOwner)
                    ->ClearCapturedBullets();
#endif
            }
        }
        else if (this->captureFrame == 10)
        {
            if (this->waitingForTexture == 0 &&
                this->capture.slot != 10)
            {
                PhotoStageInterruptCurrentEntryPhase(
                    this, resultDrawFrameIndex000);
            }
        }
        else if (this->captureFrame == 35)
        {
            if (this->waitingForTexture == 0 &&
                this->capture.slot != 10)
            {
                entryIndex = g_RuntimePlayerOwner->camera.photoIndex - 1;
                TH095_PHOTO_STAGE_ANM_SET_INTERRUPT(
                    this->capture.entryVms[entryIndex].value, 1);

                frame35Vm =
                    TH095_PHOTO_STAGE_ANM_GET_VM(
                        this->capturedPhotoVms[this->capture.slot]
                            .value);
                PhotoStageInitFrame35Position(
                    &frame35Position,
                    (-frame35Vm->spriteSize.y * 0.4f) / 2.0f);
                Rotate(&frame35Position, &frame35Position, frame35Vm->rotation.z);
                frame35Position += frame35Vm->position;
                this->capture.entryVms[entryIndex] =
                    g_PhotoFrontManager->frontAnm->CreateVmAtScreen(
                        10, &frame35Position);
                frame35Position.y -= 6.0f;
                g_PhotoFrontManager->frontAnm->CreateVmAtScreen(
                    11, &frame35Position);
            }

            ClearPhotoStageGlobalCapturedPhotoActive(
                g_RuntimeGlobalStateOwner);
            this->flags &= ~PHOTO_STAGE_CAPTURING;
        }

        this->captureFrame++;
        if (this->captureFrame == 1)
        {
            this->flags |= PHOTO_STAGE_FIRST_CAPTURE_FRAME;
        }
        else
        {
            this->flags &= ~PHOTO_STAGE_FIRST_CAPTURE_FRAME;
        }
    }

    if (this->playerPassed == 0)
    {
        if (this->displayBoundary.y + 32.0f >
                g_RuntimePlayerOwner->playerPosition.y &&
            ((this->displayBoundary.x < 320.0f &&
              g_RuntimePlayerOwner->playerPosition.x < 0.0f) ||
             (this->displayBoundary.x >= 320.0f &&
              g_RuntimePlayerOwner->playerPosition.x >= 0.0f)))
        {
            this->flags |= PHOTO_STAGE_PLAYER_PASSED;
            fadeVm1 = this->displayVms;
            for (fadeIndex1 = 0; fadeIndex1 < 80; fadeIndex1++, fadeVm1++)
            {
                if (fadeVm1->counterVar1 != 0)
                {
                    PhotoStageSetDisplayAlphaPhase(fadeVm1, 0x20);
                }
            }
        }
    }
    else if (!(this->displayBoundary.y + 32.0f >
                   g_RuntimePlayerOwner->playerPosition.y &&
               ((this->displayBoundary.x < 320.0f &&
                 g_RuntimePlayerOwner->playerPosition.x < 0.0f) ||
                (this->displayBoundary.x >= 320.0f &&
                 g_RuntimePlayerOwner->playerPosition.x >= 0.0f))))
    {
        this->flags &= ~PHOTO_STAGE_PLAYER_PASSED;
        fadeVm2 = this->displayVms;
        for (fadeIndex2 = 0; fadeIndex2 < 80; fadeIndex2++, fadeVm2++)
        {
            if (fadeVm2->counterVar1 != 0)
            {
                PhotoStageSetDisplayAlphaPhase(fadeVm2, 0xff);
            }
        }
    }

    for (executeVmIndex = 0; executeVmIndex < 80; executeVmIndex++)
    {
        AnmManager::ExecuteScript(&this->displayVms[executeVmIndex]);
    }

    return 1;
}

#undef i
#undef j
#undef k
#undef rawTop
#undef rawLeft
#undef top
#undef captureVm
#undef bottom
#undef resultDrawBacking012
#undef right
#undef readByteCountLocal02
#undef resultDrawBacking063
#undef resultDrawFrameIndex000
#undef frame35Vm
#undef entryIndex
#undef frame35Position
#undef fadeVm1
#undef fadeIndex1
#undef fadeVm2
#undef fadeIndex2
#undef executeVmIndex
#undef entryPosition

i32 __fastcall UpdatePhotoStage(PhotoStageStateView *stage)
{
    if (g_RuntimeGlobalStateOwner->gameplayLoadActive != 0)
    {
        return 1;
    }
    return stage->Update();
}

i32 PhotoStageStateView::SavePhoto(
    i32 slotIndex, const Float3 *position, i32 width, i32 height,
    i32 score, const PhotoScoreBreakdownView *scoreData)
{
    PhotoStageSaveLocals locals;

    if ((this->flags & PHOTO_STAGE_CAPTURING) != 0)
    {
        return -1;
    }

    this->flags |= PHOTO_STAGE_CAPTURING;
    locals.globalState = g_RuntimeGlobalStateOwner;
    locals.globalState->captureActive = 1;
    this->captureFrame = 0;
    *this->GetCapturePosition() = *position;
    this->GetCaptureWidth() = width;
    this->GetCaptureHeight() = height;
    this->GetCaptureSlot() = slotIndex;

    if (g_RuntimePlayerOwner->playerPosition.y < 224.0f &&
        g_RuntimePlayerOwner->playerPosition.x < 0.0f)
    {
        locals.photoX = 352.0f;
    }
    else
    {
        locals.photoX = 160.0f;
    }
    locals.photoPosition.x = locals.photoX;
    locals.photoPosition.y = 80.0f;
    locals.photoPosition.z = 0.0f;

    if (slotIndex >= 5)
    {
        locals.entryX = 576.0f;
    }
    else
    {
        locals.entryX = 64.0f;
    }
    locals.entryPosition.x = locals.entryX;
    locals.entryPosition.y =
        440.0f - (f32)(slotIndex % 5) * 80.0f;
    locals.entryPosition.z = 0.0f;

    this->slots[slotIndex].Build(
        score, &locals.photoPosition,
        slotIndex == 10 ? NULL : &locals.entryPosition,
        scoreData);
    return 0;
}

i32 PhotoStageStateView::CapturePhotoPixels(i32 photoIndex)
{
    PhotoStageCaptureLocals locals;

    locals.surface = NULL;
    locals.allocationSize =
        GetPhotoStagePixelCount(
            (u32)g_ResultSaveData
                ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
                .width,
            (u32)g_ResultSaveData
                ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
                .height) *
        (u32)g_ResultSaveData
            ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
            .componentCount;
    locals.record = &g_ResultSaveData
        ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex];
    locals.record->pixelData =
        reinterpret_cast<u8 *>(malloc(locals.allocationSize));

    this->anm->textures
        [photoIndex]
            .texture->GetSurfaceLevel(0, &locals.surface);
    locals.surface->LockRect(&locals.lockedRect, NULL, 0);

    locals.destination = g_ResultSaveData
        ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
        .pixelData;
    if (g_ResultSaveData
            ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
            .componentCount == 3)
    {
        for (locals.y = 0;
             locals.y <
                 (i32)g_ResultSaveData
                     ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
                     .height;
             locals.y++)
        {
            locals.source =
                reinterpret_cast<u8 *>(locals.lockedRect.pBits) +
                locals.y * locals.lockedRect.Pitch;
            for (locals.x = 0;
                 locals.x <
                     (i32)g_ResultSaveData
                         ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
                         .width;
                 locals.x++, locals.source += 4, locals.destination += 3)
            {
                locals.destination[0] = locals.source[0];
                locals.destination[1] = locals.source[1];
                locals.destination[2] = locals.source[2];
            }
        }
    }
    else
    {
        for (locals.y = 0;
             locals.y <
                 (i32)g_ResultSaveData
                     ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
                     .height;
             locals.y++)
        {
            locals.source =
                reinterpret_cast<u8 *>(locals.lockedRect.pBits) +
                locals.y * locals.lockedRect.Pitch;
            for (locals.x = 0;
                 locals.x <
                     (i32)g_ResultSaveData
                         ->bestShotRecords[g_RuntimeGlobalStateOwner->bestShotIndex]
                         .width;
                 locals.x++, locals.source += 2, locals.destination += 2)
            {
                locals.destination[0] = locals.source[0];
                locals.destination[1] =
                    (locals.source[1] & 0x0f) | 0xf0;
            }
        }
    }

    locals.surface->UnlockRect();
    locals.surface->Release();
    return 0;
}

} // namespace th095

#endif // TH095_MATCH_EXACT
