#ifdef TH095_MATCH_EXACT
#include "PhotoStageExact.inl"
#else
#include "PhotoCamera.hpp"
#include "PhotoStage.hpp"
#include "GameplayGlobals.hpp"
#include "Main.hpp"
#include "ScoreData.hpp"
#include "SceneData.hpp"
#include "ScreenEffect.hpp"
#ifndef DIFFBUILD
#include "PhotoEffectRuntime.hpp"
#endif

#include <stdlib.h>
#include <string.h>
#include <time.h>

namespace th095
{

struct PhotoStageCameraView : PhotoCameraState
{
    i32 GetPhotoIndex()
    {
        return this->photoIndex;
    }

    i32 GetPhotoLimit()
    {
        return this->photoLimit;
    }
};

enum PhotoStageFlags
{
    PHOTO_STAGE_CAPTURING = 1 << 0,
    PHOTO_STAGE_WAITING_FOR_TEXTURE = 1 << 1,
    PHOTO_STAGE_FIRST_CAPTURE_FRAME = 1 << 2,
    PHOTO_STAGE_PLAYER_PASSED = 1 << 3,
};

struct PhotoStageTextureEntry
{
    IDirect3DTexture8 *texture;
    u8 unknown004[8];
    i32 bytesPerPixel;
};

typedef AnmLoaded PhotoStageAnmLoadedView;

struct PhotoStageBestShotRecord
{
    u32 magic;
    u8 type;
    u8 componentCount;
    u16 group;
    u16 scene;
    u16 version;
    u16 width;
    u16 height;
    i32 score;
    f32 slowRate;
    char comment[0x50];
    u8 valid;
    u8 componentsLoaded;
    u8 unknown06a[2];
    i32 photoIndex;
    void *componentData0;
    u8 *pixelData;
};

struct PhotoScoreBreakdownView
{
    i32 finalScore;
    i32 baseScore;
    i32 capturedBulletCount;
    i32 nearbyTargetCount;
    i32 nearbyTargetBonus;
    f32 enemyDistanceMultiplier;
    f32 bossRateMultiplier;
    u32 scoringFlags;
};

struct PhotoStageScorePayloadView
{
    PhotoScoreBreakdownView photoScore;
    u8 unknown020[4];
    i32 captureTime;
    u8 unknown028[4];
    u32 attemptCount;
    u8 unknown030[4];
    f32 slowRate;
    union
    {
        u32 flags;
        struct
        {
            u32 captured : 1;
            u32 bestShotLocked : 1;
            u32 unknownFlags : 30;
        };
    };
    u8 unknown03c[0x48 - 0x3c];
};

struct PhotoStageAnmManagerView
{
    u8 unknown000000[0x0c];
    i32 captureAnmIdx;
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

    AnmVm *GetVm(i32 id);
    void SetInterrupt(i32 id, i32 interrupt);
    void MarkVmForDeletion(i32 id);
    void SetPosition(i32 id, Float3 *position);
};

struct PhotoStageSupervisorView
{
    u8 unknown0000[0x10c8];
    PhotoStageAnmLoadedView *photoAnm;
};

struct PhotoStageGlobalStateView
{
    u8 unknown000[0xfc];
    union
    {
        u32 flags;
        struct
        {
            u32 captureActive : 1;
            u32 capturedPhotoActive : 1;
#if defined(TH095_MATCH_EXACT)
            u32 unknownFlag02 : 30;
#else
            u32 gameplayLoadActive : 1;
            u32 unknownFlags3_6 : 4;
            u32 resetFpsSample : 1;
            u32 unknownFlags8_31 : 24;
#endif
        };
    };
    i32 scoreIndex;
    u8 unknown104[0x114 - 0x104];
    i32 currentScore;
    u8 unknown118[8];
    i32 resultMode;
};

struct PhotoStageRuntimeView
{
    u8 unknown000[0x20];
    char comment[1];
};

struct PhotoCardInfoView;
extern PhotoCardInfoView *g_PhotoCardInfo;

#ifdef DIFFBUILD
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
    PhotoStageGlobalStateView *globalState;
    Float3 entryPosition;
    Float3 photoPosition;
};

struct PhotoStageCaptureLocals
{
    PhotoStageBestShotRecord *record;
    i32 allocationSize;
    u8 *destination;
    u8 *source;
    i32 y;
    i32 x;
    D3DLOCKED_RECT lockedRect;
    IDirect3DSurface8 *surface;
};

typedef char PhotoStageTextureEntrySizeIs10[
    (sizeof(PhotoStageTextureEntry) == 0x10) ? 1 : -1];
typedef char PhotoStageBestShotRecordSizeIs78[
    (sizeof(PhotoStageBestShotRecord) == 0x78) ? 1 : -1];
typedef char PhotoScoreBreakdownSizeIs20[
    (sizeof(PhotoScoreBreakdownView) == 0x20) ? 1 : -1];
typedef char PhotoScoreBreakdownMultipliersAt14[
    (offsetof(PhotoScoreBreakdownView, enemyDistanceMultiplier) == 0x14 &&
     offsetof(PhotoScoreBreakdownView, bossRateMultiplier) == 0x18)
        ? 1
        : -1];
typedef char PhotoScoreBreakdownFlagsAt1C[
    (offsetof(PhotoScoreBreakdownView, scoringFlags) == 0x1c) ? 1 : -1];
typedef char PhotoStageScorePayloadSizeIs48[
    (sizeof(PhotoStageScorePayloadView) == 0x48) ? 1 : -1];
typedef char PhotoStageScorePayloadMatchesResultTail[
    (sizeof(PhotoStageScorePayloadView) ==
     sizeof(ResultScoreEntryView) - offsetof(ResultScoreEntryView, detailScore))
        ? 1
        : -1];
typedef char PhotoStageSlotSizeIs2214[
    (sizeof(PhotoStageSlot) == 0x2214) ? 1 : -1];
typedef char PhotoStageSlotPrimaryVmsAt44[
    (offsetof(PhotoStageSlot, display.primaryVms) == 0x44) ? 1 : -1];
typedef char PhotoStageSlotOverlayVmsAt110C[
    (offsetof(PhotoStageSlot, display.overlayVms) == 0x110c) ? 1 : -1];
typedef char PhotoStageSlotScoreDataAt21D4[
    (offsetof(PhotoStageSlot, display.scoreData) == 0x21d4) ? 1 : -1];
typedef char PhotoStageSlotFlagsAt21F0[
    (offsetof(PhotoStageSlot, display.scoreData) + 7 * sizeof(i32) == 0x21f0)
        ? 1 : -1];
typedef char PhotoStageSlotScoreAt21F4[
    (offsetof(PhotoStageSlot, display.score) == 0x21f4) ? 1 : -1];
typedef char PhotoStageSlotTimestampAt21F8[
    (offsetof(PhotoStageSlot, timestamp) == 0x21f8) ? 1 : -1];
typedef char PhotoStageSlotSlowRateAt21FC[
    (offsetof(PhotoStageSlot, slowRate) == 0x21fc) ? 1 : -1];
typedef char PhotoStageSlotWidthAt2200[
    (offsetof(PhotoStageSlot, width) == 0x2200) ? 1 : -1];
typedef char PhotoStageSlotHeightAt2204[
    (offsetof(PhotoStageSlot, height) == 0x2204) ? 1 : -1];
typedef char PhotoStageDisplayOverlayVmsAt10C8[
    (offsetof(PhotoStageDisplayView, overlayVms) == 0x10c8) ? 1 : -1];
typedef char PhotoStageDisplayScoreDataAt2190[
    (offsetof(PhotoStageDisplayView, scoreData) == 0x2190) ? 1 : -1];
typedef char PhotoStageDisplayScoreAt21B0[
    (offsetof(PhotoStageDisplayView, score) == 0x21b0) ? 1 : -1];
typedef char PhotoStageCapturedVmsAt17720[
    (offsetof(PhotoStageStateView, capturedPhotoVms) == 0x17720) ? 1 : -1];
typedef char PhotoStageDisplayVmsAt1774C[
    (offsetof(PhotoStageStateView, displayVms) == 0x1774c) ? 1 : -1];
typedef char PhotoStageBoundaryXAt2570C[
    (offsetof(PhotoStageStateView, boundaryX) == 0x2570c) ? 1 : -1];
typedef char PhotoStageAnmAt2571C[
    (offsetof(PhotoStageStateView, anm) == 0x2571c) ? 1 : -1];
typedef char PhotoStageCaptureFrameAt25724[
    (offsetof(PhotoStageStateView, captureFrame) == 0x25724) ? 1 : -1];

extern PhotoGameStateView *g_PhotoGame;
extern PhotoStageGlobalStateView *g_PhotoStageGlobalState;
extern PhotoStageRuntimeView *g_PhotoStageRuntime;
extern PhotoStageSupervisorView *g_PhotoStageSupervisor;
#ifdef DIFFBUILD
extern PhotoStageEffectManagerView *g_PhotoStageEffectManager;
#endif

#ifndef DIFFBUILD
#define g_PhotoGame \
    TH095_RUNTIME_GLOBAL_PTR(PhotoGameStateView, g_RuntimePlayerOwner)
#define g_PhotoStageGlobalState \
    TH095_RUNTIME_GLOBAL_PTR(PhotoStageGlobalStateView, g_RuntimeGlobalStateOwner)
#define g_PhotoStageRuntime \
    (reinterpret_cast<PhotoStageRuntimeView *>(g_PhotoCardInfo))
#define g_PhotoStageSupervisor \
    TH095_RUNTIME_GLOBAL_PTR(PhotoStageSupervisorView, g_RuntimeBackgroundManagerOwner)
#endif
#ifdef DIFFBUILD
extern PhotoStageBulletManagerView *g_PhotoStageBulletManager;
#endif
extern PhotoStageStateView *g_PhotoStageState;
#define g_PhotoStageState \
    TH095_RUNTIME_GLOBAL_PTR(PhotoStageStateView, g_RuntimeStageStateOwner)
// Target 0x004C45F8 is an independent zero-initialized byte published when a
// captured texture becomes ready.
DIFFABLE_STATIC(u8, g_PhotoCaptureCountdown);

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

static inline PhotoStageAnmManagerView *GetPhotoStageAnmManager()
{
    return reinterpret_cast<PhotoStageAnmManagerView *>(g_AnmManager);
}

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

static inline PhotoStageTextureEntry *GetPhotoStageTextures(PhotoStageStateView *stage)
{
    return reinterpret_cast<PhotoStageTextureEntry *>(stage->anm->textures);
}

static inline PhotoStageAnmLoadedView *GetPhotoStageAnm(
    PhotoAnmLoadedView *anm)
{
    return reinterpret_cast<PhotoStageAnmLoadedView *>(anm);
}

static inline PhotoStageCameraView *GetPhotoStageCamera()
{
    return reinterpret_cast<PhotoStageCameraView *>(&g_PhotoGame->camera);
}

static __forceinline void PhotoStageInterruptCurrentEntryPhase(
    PhotoStageStateView *state, i32 &entryIndex)
{
    u8 compilerStorage[8];
    entryIndex = GetPhotoStageCamera()->GetPhotoIndex() - 1;
    TH095_PHOTO_STAGE_ANM_SET_INTERRUPT(
        state->slots[0].entryVms[entryIndex].value, 1);
}

static inline PhotoStageScorePayloadView *GetPhotoStageScorePayload(i32 index)
{
    return reinterpret_cast<PhotoStageScorePayloadView *>(
        &g_ResultSaveData->scoreEntries[index].detailScore);
}

static inline PhotoScoreBreakdownView *GetPhotoStageDisplayScoreBreakdown(
    PhotoStageDisplayView *display)
{
    return reinterpret_cast<PhotoScoreBreakdownView *>(display->scoreData);
}

static inline PhotoStageBestShotRecord *GetPhotoStageBestShotRecord(i32 index)
{
    return reinterpret_cast<PhotoStageBestShotRecord *>(
        &g_ResultSaveData->bestShotRecords[index]);
}

static __forceinline void ClearPhotoStageGlobalCaptureActive(
    PhotoStageGlobalStateView *state)
{
    state->captureActive = 0;
}

static __forceinline void ClearPhotoStageGlobalCapturedPhotoActive(
    PhotoStageGlobalStateView *state)
{
    state->capturedPhotoActive = 0;
}

static __forceinline void SetPhotoStageGlobalCapturedPhotoActive(
    PhotoStageGlobalStateView *state)
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

void PhotoStageDisplayView::Build(
    i32 score, Float3 *photoPosition, Float3 *entryPosition,
    const i32 *scoreData)
{
    const PhotoScoreBreakdownView *scoreBreakdown =
        reinterpret_cast<const PhotoScoreBreakdownView *>(scoreData);
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
        *GetPhotoStageDisplayScoreBreakdown(this) = *scoreBreakdown;
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
        if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_UNKNOWN_5) != 0)
        {
            ADD_PHOTO_STAGE_DISPLAY_VM(0x24);
            renderMode += 4;
            displayPosition.y += 12.0f;
        }
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_COLOR_1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x25, 300);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_COLOR_2) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x26, 300);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_COLOR_3) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x27, 300);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_COLOR_4) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x28, 300);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_COLOR_5) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x29, 300);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_COLOR_6) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2a, 300);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_COLOR_7) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2b, 300);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_COLORFUL) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2c, 900);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_RAINBOW) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2d, 2100);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_EMPTY) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2e, 0);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_NO_BULLETS) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2f, 100);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_UNKNOWN_17) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x30, 100);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_UNKNOWN_18) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x31, 0);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_UNKNOWN_19) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x32, 0);
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_NEARBY) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x23, scoreBreakdown->nearbyTargetBonus);
    }

    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_ENEMY) != 0)
    {
        ADD_PHOTO_STAGE_DISPLAY_VM(0x1f);
        displayPosition.x += 99.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(
            (i32)(scoreBreakdown->enemyDistanceMultiplier * 10.0f) /
                10 + 15);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x1a);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(
            (i32)(scoreBreakdown->enemyDistanceMultiplier * 10.0f) %
                10 + 15);
        renderMode += 4;
        displayPosition.x = photoPositionCopy.x;
        displayPosition.y += 12.0f;
    }
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_SELF) != 0)
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
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_TWO_SHOT) != 0)
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
    if ((scoreBreakdown->scoringFlags & PHOTO_STAGE_SCORE_BOSS_RATE) != 0)
    {
        ADD_PHOTO_STAGE_DISPLAY_VM(0x22);
        displayPosition.x += 99.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(
            (i32)(scoreBreakdown->bossRateMultiplier * 10.0f) /
                10 + 15);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x1a);
        displayPosition.x += 9.0f;
        InitializePhotoStageDisplayVm(
            &g_PhotoStageState->displayVms[displayVmCount++],
            &displayPosition,
            (i32)(scoreBreakdown->bossRateMultiplier * 10.0f) %
                    10 +
                15,
            renderMode);
        renderMode += 4;
        displayPosition.x = photoPositionCopy.x;
        displayPosition.y += 12.0f;
    }

    *reinterpret_cast<Float3 *>(&g_PhotoStageState->boundaryX) =
        displayPosition;
}

#undef digitPosition
#undef displayVmCount
#undef ADD_PHOTO_STAGE_SCORE_ROW
#undef ADD_PHOTO_STAGE_DISPLAY_VM

static __forceinline void PhotoStagePublishCaptureRequestArgs(PhotoStageAnmManagerView *anmManager, i32 captureSlot, i32 left, i32 right, i32 top, i32 bottom)
{
    if (anmManager->captureAnmIdx >= 0) { } else {
        anmManager->captureAnmIdx=9; anmManager->captureSourceX=left; anmManager->captureSourceY=top;
        anmManager->captureSourceWidth=right-left; anmManager->captureSourceHeight=bottom-top;
        anmManager->captureDestinationX=3; anmManager->captureDestinationY=3;
        anmManager->captureDestinationWidth=right-left; anmManager->captureDestinationHeight=bottom-top;
        anmManager->captureFlags=captureSlot;
    }
}

static __forceinline void PhotoStageAccumulateCapturedScore(PhotoStageStateView *state)
{
    if ((GetPhotoStageDisplayScoreBreakdown(
             &state->slots[state->slots[0].captureSlot].display)
             ->scoringFlags &
         PHOTO_STAGE_SCORE_ENEMY) != 0)
    {
        PhotoStageGlobalStateView *globalState;
        i32 bgmFormatIndexLocal05 =
            state->slots[state->slots[0].captureSlot].display.score;
        globalState = g_PhotoStageGlobalState;
        globalState->currentScore += bgmFormatIndexLocal05;
    }
}

static __forceinline void PhotoStagePublishSlowRate(PhotoStageStateView *state)
{
    state->slots[state->slots[0].captureSlot].slowRate =
        GetPhotoStageBestShotRecord(g_PhotoStageGlobalState->scoreIndex)->slowRate =
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

    if (PhotoStageEntryVmIsZero(this->slots[0].entryVms[0]) &&
        GetPhotoStageCamera()->GetPhotoLimit() > 0)
    {
        for (k = 0; k < GetPhotoStageCamera()->GetPhotoLimit(); k++)
        {
            entryPosition.x = PhotoStageEntryXValue(k);
            entryPosition.y = 400.0f - (f32)(k % 5) * 80.0f;
            entryPosition.z = 0.0f;
            this->slots[0].entryVms[k] =
                g_PhotoStageSupervisor->photoAnm->CreateVmAtScreen(
                    12, &entryPosition);
        }
    }

    if (this->capturing != 0)
    {
        if (this->captureFrame == 1)
        {
            PhotoToScreen(&resultDrawBacking012, &this->slots[0].capturePosition);

            if (this->slots[0].captureWidth > 0)
            {
                readByteCountLocal02 = (i32)resultDrawBacking012.x -
                    (this->slots[0].captureWidth - 6) / 2;
                rawLeft = readByteCountLocal02;
                right = readByteCountLocal02 - 6 + this->slots[0].captureWidth;

                if ((f32)readByteCountLocal02 < 128.0f)
                {
                    readByteCountLocal02 = 128;
                }
                if ((f32)right >= 512.0f)
                {
                    right = 511;
                }

                top = (i32)resultDrawBacking012.y -
                    (this->slots[0].captureHeight - 6) / 2;
                rawTop = top;
                bottom = top - 6 + this->slots[0].captureHeight;

                if ((f32)top < 16.0f)
                {
                    top = 16;
                }
                if ((f32)bottom >= 464.0f)
                {
                    bottom = 463;
                }

                this->slots[0].captureWidth = right - readByteCountLocal02 + 6;
                this->slots[0].captureHeight = bottom - top + 6;

                PhotoStagePublishCaptureRequestArgs(
                    GetPhotoStageAnmManager(), this->slots[0].captureSlot,
                    readByteCountLocal02, right, top, bottom);

                g_PhotoCaptureCountdown = 99;
#ifdef DIFFBUILD
                g_PhotoStageGlobalState->flags |= 0x80;
#else
                g_PhotoStageGlobalState->resetFpsSample = 1;
#endif
                this->flags &= ~PHOTO_STAGE_WAITING_FOR_TEXTURE;
            }
            else
            {
                this->slots[0].captureWidth = 64;
                this->slots[0].captureHeight = 48;
                this->flags |= PHOTO_STAGE_WAITING_FOR_TEXTURE;
            }

            resultDrawBacking063.surface = NULL;
            reinterpret_cast<PhotoStageTextureEntry *>(this->anm->textures)
                [this->slots[0].captureSlot]
                .texture->GetSurfaceLevel(0, &resultDrawBacking063.surface);
            resultDrawBacking063.surface->LockRect(&resultDrawBacking063.lockedRect, NULL, 0);

            for (resultDrawBacking063.y = 0; resultDrawBacking063.y < this->slots[0].captureHeight; resultDrawBacking063.y++)
            {
                resultDrawBacking063.row = reinterpret_cast<u8 *>(resultDrawBacking063.lockedRect.pBits) +
                    resultDrawBacking063.y * resultDrawBacking063.lockedRect.Pitch;
                memset(
                    resultDrawBacking063.row,
                    0,
                    this->slots[0].captureWidth *
                        reinterpret_cast<PhotoStageTextureEntry *>(
                            this->anm->textures)[this->slots[0].captureSlot]
                            .bytesPerPixel);
            }
            for (resultDrawBacking063.y = 0; resultDrawBacking063.y < 3; resultDrawBacking063.y++)
            {
                resultDrawBacking063.row = reinterpret_cast<u8 *>(resultDrawBacking063.lockedRect.pBits) +
                    resultDrawBacking063.y * resultDrawBacking063.lockedRect.Pitch;
                memset(
                    resultDrawBacking063.row,
                    0xff,
                    this->slots[0].captureWidth *
                        reinterpret_cast<PhotoStageTextureEntry *>(
                            this->anm->textures)[this->slots[0].captureSlot]
                            .bytesPerPixel);
            }
            for (resultDrawBacking063.y = this->slots[0].captureHeight - 3;
                 resultDrawBacking063.y < this->slots[0].captureHeight;
                 resultDrawBacking063.y++)
            {
                resultDrawBacking063.row = reinterpret_cast<u8 *>(resultDrawBacking063.lockedRect.pBits) +
                    resultDrawBacking063.y * resultDrawBacking063.lockedRect.Pitch;
                memset(
                    resultDrawBacking063.row,
                    0xff,
                    this->slots[0].captureWidth *
                        reinterpret_cast<PhotoStageTextureEntry *>(
                            this->anm->textures)[this->slots[0].captureSlot]
                            .bytesPerPixel);
            }
            for (resultDrawBacking063.y = 3; resultDrawBacking063.y < this->slots[0].captureHeight - 3; resultDrawBacking063.y++)
            {
                resultDrawBacking063.row = reinterpret_cast<u8 *>(resultDrawBacking063.lockedRect.pBits) +
                    resultDrawBacking063.y * resultDrawBacking063.lockedRect.Pitch;
                memset(
                    resultDrawBacking063.row, 0xff,
                    reinterpret_cast<PhotoStageTextureEntry *>(
                        this->anm->textures)[this->slots[0].captureSlot]
                            .bytesPerPixel *
                        3);
                memset(
                    resultDrawBacking063.row + (this->slots[0].captureWidth - 3) *
                        reinterpret_cast<PhotoStageTextureEntry *>(
                            this->anm->textures)[this->slots[0].captureSlot]
                            .bytesPerPixel,
                    0xff,
                    reinterpret_cast<PhotoStageTextureEntry *>(
                        this->anm->textures)[this->slots[0].captureSlot]
                            .bytesPerPixel *
                        3);
            }

            resultDrawBacking063.surface->UnlockRect();
            resultDrawBacking063.surface->Release();

            if (this->capturedPhotoVms[this->slots[0].captureSlot] != 0)
            {
                TH095_PHOTO_STAGE_ANM_MARK_DELETE(
                    this->capturedPhotoVms[this->slots[0].captureSlot].value);
            }
            this->capturedPhotoVms[this->slots[0].captureSlot] =
                reinterpret_cast<PhotoStageAnmLoadedView *>(this->anm)->CreateVm(
                    this->slots[0].captureSlot * 2, 0);

            captureVm =
                TH095_PHOTO_STAGE_ANM_GET_VM(
                    this->capturedPhotoVms[this->slots[0].captureSlot].value);
            captureVm->loadedSprite->uvEnd.x =
                (f32)this->slots[0].captureWidth / 256.0f;
            captureVm->loadedSprite->uvEnd.y =
                (f32)this->slots[0].captureHeight / 256.0f;
            captureVm->spriteSize.x = (f32)this->slots[0].captureWidth;
            captureVm->spriteSize.y = (f32)this->slots[0].captureHeight;
            TH095_PHOTO_STAGE_ANM_SET_POSITION(
                this->capturedPhotoVms[this->slots[0].captureSlot].value,
                &resultDrawBacking012);

            ClearPhotoStageGlobalCaptureActive(g_PhotoStageGlobalState);
            SetPhotoStageGlobalCapturedPhotoActive(
                g_PhotoStageGlobalState);
        }
        else if (this->captureFrame == 2)
        {
            if (this->waitingForTexture == 0)
            {
                ScreenEffect::RegisterChain(
                    SCREEN_EFFECT_FULL_FADE_OUT, 15, 1, 0xc0ffafcf, 0, 0x1d);

                PhotoStageAccumulateCapturedScore(this);

                if (g_PhotoStageGlobalState->resultMode == 0)
                {
                    if (GetPhotoStageScorePayload(
                            g_PhotoStageGlobalState->scoreIndex)
                            ->attemptCount < 999999)
                    {
                        GetPhotoStageScorePayload(
                            g_PhotoStageGlobalState->scoreIndex)
                            ->attemptCount++;
                    }

                    this->slots[this->slots[0].captureSlot].timestamp =
                        (i32)time(NULL);

                    PhotoStagePublishSlowRate(this);
                    this->slots[this->slots[0].captureSlot].width =
                        this->slots[0].captureWidth;
                    this->slots[this->slots[0].captureSlot].height =
                        this->slots[0].captureHeight;
                    if (g_PhotoStageRuntime != NULL)
                    {
                        strcpy(
                            this->slots[this->slots[0].captureSlot].comment,
                            g_PhotoStageRuntime->comment);
                    }
                    else
                    {
                        memset(
                            this->slots[this->slots[0].captureSlot].comment,
                            0, 0x50);
                    }

                    if (!GetPhotoStageScorePayload(
                             g_PhotoStageGlobalState->scoreIndex)
                             ->bestShotLocked &&
                        this->slots[this->slots[0].captureSlot].display.score >
                            GetPhotoStageScorePayload(
                                g_PhotoStageGlobalState->scoreIndex)
                                ->photoScore.finalScore)
                    {
                        memcpy(
                            &GetPhotoStageScorePayload(
                                 g_PhotoStageGlobalState->scoreIndex)
                                 ->photoScore,
                            GetPhotoStageDisplayScoreBreakdown(
                                &this->slots[this->slots[0].captureSlot].display),
                            sizeof(PhotoScoreBreakdownView));
                        g_ResultSaveData->UpdateBestShotRecord(
                            g_PhotoStageGlobalState->scoreIndex);

                        GetPhotoStageBestShotRecord(
                            g_PhotoStageGlobalState->scoreIndex)->valid = 1;
                        GetPhotoStageBestShotRecord(
                            g_PhotoStageGlobalState->scoreIndex)->magic =
                            0x53545342;
                        GetPhotoStageBestShotRecord(
                            g_PhotoStageGlobalState->scoreIndex)->width =
                            (u16)this->slots[0].captureWidth;
                        GetPhotoStageBestShotRecord(
                            g_PhotoStageGlobalState->scoreIndex)->height =
                            (u16)this->slots[0].captureHeight;
                        GetPhotoStageBestShotRecord(
                            g_PhotoStageGlobalState->scoreIndex)->score =
                                this->slots[this->slots[0].captureSlot]
                                    .display.score;
                        GetPhotoStageBestShotRecord(
                            g_PhotoStageGlobalState->scoreIndex)->group =
                                (u16)(g_SelectedScene->group + 1);
                        GetPhotoStageBestShotRecord(
                            g_PhotoStageGlobalState->scoreIndex)->scene =
                                (u16)(g_SelectedScene->scene + 1);
                        GetPhotoStageBestShotRecord(
                            g_PhotoStageGlobalState->scoreIndex)->type = 2;
                        GetPhotoStageBestShotRecord(
                            g_PhotoStageGlobalState->scoreIndex)->version =
                            0x102;
                        GetPhotoStageBestShotRecord(
                            g_PhotoStageGlobalState->scoreIndex)
                            ->componentCount =
                            (u8)((reinterpret_cast<PhotoStageTextureEntry *>(
                                      this->anm->textures)
                                      [this->slots[0].captureSlot]
                                          .bytesPerPixel == 4) +
                                2);
                        GetPhotoStageScorePayload(
                            g_PhotoStageGlobalState->scoreIndex)->slowRate =
                            this->slots[this->slots[0].captureSlot].slowRate;
                        GetPhotoStageScorePayload(
                            g_PhotoStageGlobalState->scoreIndex)->captureTime =
                            this->slots[this->slots[0].captureSlot].timestamp;
                        if (g_PhotoStageRuntime != NULL)
                        {
                            strcpy(
                                GetPhotoStageBestShotRecord(
                                    g_PhotoStageGlobalState->scoreIndex)
                                    ->comment,
                                g_PhotoStageRuntime->comment);
                        }
                        else
                        {
                            memset(
                                GetPhotoStageBestShotRecord(
                                    g_PhotoStageGlobalState->scoreIndex)
                                    ->comment,
                                0, 0x50);
                        }

                        if (this->slots[0].captureSlot != 10)
                        {
                            GetPhotoStageBestShotRecord(
                                g_PhotoStageGlobalState->scoreIndex)
                                ->photoIndex = this->slots[0].captureSlot;
                        }
                        else
                        {
                            GetPhotoStageBestShotRecord(
                                g_PhotoStageGlobalState->scoreIndex)
                                ->photoIndex = -1;
                            this->CapturePhotoPixels(
                                this->slots[0].captureSlot);
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
                this->slots[0].captureSlot != 10)
            {
                PhotoStageInterruptCurrentEntryPhase(
                    this, resultDrawFrameIndex000);
            }
        }
        else if (this->captureFrame == 35)
        {
            if (this->waitingForTexture == 0 &&
                this->slots[0].captureSlot != 10)
            {
                entryIndex = GetPhotoStageCamera()->GetPhotoIndex() - 1;
                TH095_PHOTO_STAGE_ANM_SET_INTERRUPT(
                    this->slots[0].entryVms[entryIndex].value, 1);

                frame35Vm =
                    TH095_PHOTO_STAGE_ANM_GET_VM(
                        this->capturedPhotoVms[this->slots[0].captureSlot]
                            .value);
                PhotoStageInitFrame35Position(
                    &frame35Position,
                    (-frame35Vm->spriteSize.y * 0.4f) / 2.0f);
                Rotate(&frame35Position, &frame35Position, frame35Vm->rotation.z);
                frame35Position += frame35Vm->position;
                this->slots[0].entryVms[entryIndex] =
                    g_PhotoStageSupervisor->photoAnm->CreateVmAtScreen(
                        10, &frame35Position);
                frame35Position.y -= 6.0f;
                g_PhotoStageSupervisor->photoAnm->CreateVmAtScreen(
                    11, &frame35Position);
            }

            ClearPhotoStageGlobalCapturedPhotoActive(
                g_PhotoStageGlobalState);
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
        if (this->boundaryY + 32.0f > g_PhotoGame->playerPosition.y &&
            ((this->boundaryX < 320.0f &&
              g_PhotoGame->playerPosition.x < 0.0f) ||
             (this->boundaryX >= 320.0f &&
              g_PhotoGame->playerPosition.x >= 0.0f)))
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
    else if (!(this->boundaryY + 32.0f >
                   g_PhotoGame->playerPosition.y &&
               ((this->boundaryX < 320.0f &&
                 g_PhotoGame->playerPosition.x < 0.0f) ||
                (this->boundaryX >= 320.0f &&
                 g_PhotoGame->playerPosition.x >= 0.0f))))
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
#if defined(TH095_MATCH_EXACT)
    if (((g_PhotoStageGlobalState->flags >> 2) & 1) != 0)
#else
    if (g_PhotoStageGlobalState->gameplayLoadActive != 0)
#endif
    {
        return 1;
    }
    return stage->Update();
}

i32 PhotoStageStateView::SavePhoto(
    i32 slotIndex, const Float3 *position, i32 width, i32 height,
    i32 score, const i32 *scoreData)
{
    PhotoStageSaveLocals locals;

    if ((this->flags & PHOTO_STAGE_CAPTURING) != 0)
    {
        return -1;
    }

    this->flags |= PHOTO_STAGE_CAPTURING;
    locals.globalState = g_PhotoStageGlobalState;
    locals.globalState->flags |= 1;
    this->captureFrame = 0;
    *this->GetCapturePosition() = *position;
    this->GetCaptureWidth() = width;
    this->GetCaptureHeight() = height;
    this->GetCaptureSlot() = slotIndex;

    if (g_PhotoGame->playerPosition.y < 224.0f &&
        g_PhotoGame->playerPosition.x < 0.0f)
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

    this->slots[slotIndex].display.Build(
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
                ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
                .width,
            (u32)g_ResultSaveData
                ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
                .height) *
        (u32)g_ResultSaveData
            ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
            .componentCount;
    locals.record = reinterpret_cast<PhotoStageBestShotRecord *>(
        &g_ResultSaveData
             ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]);
    locals.record->pixelData =
        reinterpret_cast<u8 *>(malloc(locals.allocationSize));

    reinterpret_cast<PhotoStageTextureEntry *>(this->anm->textures)
        [photoIndex]
            .texture->GetSurfaceLevel(0, &locals.surface);
    locals.surface->LockRect(&locals.lockedRect, NULL, 0);

    locals.destination = g_ResultSaveData
        ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
        .pixelData;
    if (g_ResultSaveData
            ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
            .componentCount == 3)
    {
        for (locals.y = 0;
             locals.y <
                 (i32)g_ResultSaveData
                     ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
                     .height;
             locals.y++)
        {
            locals.source =
                reinterpret_cast<u8 *>(locals.lockedRect.pBits) +
                locals.y * locals.lockedRect.Pitch;
            for (locals.x = 0;
                 locals.x <
                     (i32)g_ResultSaveData
                         ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
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
                     ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
                     .height;
             locals.y++)
        {
            locals.source =
                reinterpret_cast<u8 *>(locals.lockedRect.pBits) +
                locals.y * locals.lockedRect.Pitch;
            for (locals.x = 0;
                 locals.x <
                     (i32)g_ResultSaveData
                         ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
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
