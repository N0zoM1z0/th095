#include "PhotoCamera.hpp"

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

struct PhotoStageTextureEntry
{
    IDirect3DTexture8 *texture;
    u8 unknown004[8];
    i32 bytesPerPixel;
};

struct PhotoStageAnmLoadedView
{
    i32 anmIdx;
    void *rawData;
    i32 totalEntries;
    AnmLoadedSprite *sprites;
    AnmRawInstr **scripts;
    void *textures;
    i32 numberEntriesToBeLoaded;

    ZunResult SetSprite(AnmVm *vm, i32 spriteIndex);
    void InitializeVm(AnmVm *vm, i32 scriptIndex);
    PhotoAnmVmId CreateVm(i32 scriptIndex, i32 renderMode);
    PhotoAnmVmId CreateVm(i32 scriptIndex, Float3 *position);
};

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
    u8 unknown014[4];
    char comment[0x50];
    u8 valid;
    u8 componentsLoaded;
    u8 unknown06a[2];
    i32 photoIndex;
    void *componentData0;
    u8 *pixelData;
};

struct PhotoStageSaveDataView
{
    u8 unknown0000[0x3160];
    PhotoStageBestShotRecord bestShotRecords[120];

    void UpdateBestShotRecord(i32 index);
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

struct PhotoStageSlot
{
    u8 unknown0000[0x44];
    AnmVm primaryVms[6];
    AnmVm overlayVms[6];
    i32 scoreData[7];
    u32 flags;
    i32 score;
    i32 timestamp;
    f32 slowRate;
    i32 width;
    i32 height;
    char comment[12];
};

struct PhotoStageSupervisorView
{
    u8 unknown0000[0x10c8];
    PhotoStageAnmLoadedView *photoAnm;
};

struct PhotoStageGlobalStateView
{
    u8 unknown000[0xfc];
    u32 flags;
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

struct PhotoStagePlayerConfigView
{
    u8 unknown000[4];
    i32 group;
    i32 scene;
};

struct PhotoStageEffectManagerView
{
    i32 CommitCapturedObjects();
};

struct PhotoStageBulletManagerView
{
    i32 ClearCapturedBullets();
};

struct PhotoStageDisplayView
{
    AnmVm primaryVms[6];
    AnmVm overlayVms[6];
    i32 scoreData[8];
    i32 score;

    void Build(
        i32 score, Float3 *photoPosition, Float3 *entryPosition,
        const i32 *scoreData);
};

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

struct PhotoStageStateView
{
    PhotoStageSlot slots[11];
    u8 unknown176dc[0x17720 - 0x176dc];
    PhotoAnmVmId capturedPhotoVms[11];
    AnmVm displayVms[80];
    f32 boundaryX;
    f32 boundaryY;
    i32 unknown25714;
    f32 scoreMultiplier;
    PhotoAnmLoadedView *anm;
    u32 flags;
    i32 captureFrame;

    i32 Update();
    i32 SavePhoto(
        i32 slot, const Float3 *position, i32 width, i32 height,
        i32 score, const i32 *scoreData);
    i32 CapturePhotoPixels(i32 photoIndex);

    PhotoAnmVmId *GetEntryVms()
    {
        return reinterpret_cast<PhotoAnmVmId *>(this);
    }

    Float3 *GetCapturePosition()
    {
        return reinterpret_cast<Float3 *>(reinterpret_cast<u8 *>(this) + 0x2c);
    }

    i32 &GetCaptureWidth()
    {
        return *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x38);
    }

    i32 &GetCaptureHeight()
    {
        return *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x3c);
    }

    i32 &GetCaptureSlot()
    {
        return *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x40);
    }
};

typedef char PhotoStageTextureEntrySizeIs10[
    (sizeof(PhotoStageTextureEntry) == 0x10) ? 1 : -1];
typedef char PhotoStageBestShotRecordSizeIs78[
    (sizeof(PhotoStageBestShotRecord) == 0x78) ? 1 : -1];
typedef char PhotoStageSlotSizeIs2214[
    (sizeof(PhotoStageSlot) == 0x2214) ? 1 : -1];
typedef char PhotoStageSlotPrimaryVmsAt44[
    (offsetof(PhotoStageSlot, primaryVms) == 0x44) ? 1 : -1];
typedef char PhotoStageSlotOverlayVmsAt110C[
    (offsetof(PhotoStageSlot, overlayVms) == 0x110c) ? 1 : -1];
typedef char PhotoStageSlotScoreDataAt21D4[
    (offsetof(PhotoStageSlot, scoreData) == 0x21d4) ? 1 : -1];
typedef char PhotoStageSlotFlagsAt21F0[
    (offsetof(PhotoStageSlot, flags) == 0x21f0) ? 1 : -1];
typedef char PhotoStageSlotScoreAt21F4[
    (offsetof(PhotoStageSlot, score) == 0x21f4) ? 1 : -1];
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
extern PhotoStageSaveDataView *g_PhotoStageSaveData;
extern PhotoStageRuntimeView *g_PhotoStageRuntime;
extern PhotoStageSupervisorView *g_PhotoStageSupervisor;
extern PhotoStageEffectManagerView *g_PhotoStageEffectManager;
extern PhotoStageBulletManagerView *g_PhotoStageBulletManager;
extern PhotoStagePlayerConfigView *g_PhotoStagePlayerConfig;
extern PhotoStageStateView *g_PhotoStageState;
extern i32 g_PhotoCaptureCountdown;
extern f64 g_PhotoLagNumerator;
extern f64 g_PhotoLagDenominator;

void __fastcall SpawnPhotoStageEffect(
    i32 effectType, i32 script, i32 count, u32 color, i32 arg4, i32 arg5);
void __fastcall RotatePhotoStagePoint(
    Float3 *output, const Float3 *input, f32 angle);
Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

static inline PhotoStageAnmManagerView *GetPhotoStageAnmManager()
{
    return reinterpret_cast<PhotoStageAnmManagerView *>(g_AnmManager);
}

static inline PhotoStageTextureEntry *GetPhotoStageTextures(PhotoStageStateView *stage)
{
    return reinterpret_cast<PhotoStageTextureEntry *>(stage->anm->textures);
}

static inline PhotoStageAnmLoadedView *GetPhotoStageAnm(
    PhotoAnmLoadedView *anm)
{
    return reinterpret_cast<PhotoStageAnmLoadedView *>(anm);
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
        AnmVm *displayVm =                                                   \
            &g_PhotoStageState->displayVms[displayVmCount];                  \
        InitializePhotoStageDisplayVm(                                      \
            displayVm, &displayPosition, (spriteIndex), renderMode);         \
        displayVmCount++;                                                    \
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
    displayPosition.x = photoX;                                              \
    renderMode += 4;                                                         \
    displayPosition.y += 12.0f

void PhotoStageDisplayView::Build(
    i32 score, Float3 *photoPosition, Float3 *entryPosition,
    const i32 *scoreData)
{
    f32 photoX = photoPosition->x;
    f32 photoY = photoPosition->y;
    f32 photoZ = photoPosition->z;
    this->score = score;
    if (scoreData != NULL)
    {
        memcpy(this->scoreData, scoreData, sizeof(this->scoreData));
    }

    Float3 displayPosition;
    displayPosition.x = photoX;
    displayPosition.z = photoZ;
    if (entryPosition != NULL)
    {
        f32 digitX = entryPosition->x;
        f32 digitY = entryPosition->y;
        f32 digitZ = entryPosition->z;
        i32 digit = score / 100000;
        bool tenThousandsVisible;
        bool thousandsVisible;

        displayPosition.y = photoY;
        if (digit != 0)
        {
            g_PhotoStageState->anm->InitializeVm(&this->overlayVms[0], 0x1e);
            g_PhotoStageState->anm->SetSprite(
                &this->overlayVms[0], digit + 15);
        }
        this->overlayVms[0].positionOffset.x = digitX;
        this->overlayVms[0].positionOffset.y = digitY;
        this->overlayVms[0].positionOffset.z = digitZ;
        digitX += 9.0f;

        digit = (score / 10000) % 10;
        tenThousandsVisible = false;
        if (digit != 0 || score / 100000 != 0)
        {
            g_PhotoStageState->anm->InitializeVm(&this->overlayVms[1], 0x1e);
            g_PhotoStageState->anm->SetSprite(
                &this->overlayVms[1], digit + 15);
            tenThousandsVisible = true;
        }
        this->overlayVms[1].positionOffset.x = digitX;
        this->overlayVms[1].positionOffset.y = digitY;
        this->overlayVms[1].positionOffset.z = digitZ;
        digitX += 9.0f;

        digit = (score / 1000) % 10;
        thousandsVisible = false;
        if (digit != 0 || tenThousandsVisible)
        {
            g_PhotoStageState->anm->InitializeVm(&this->overlayVms[2], 0x1e);
            g_PhotoStageState->anm->SetSprite(
                &this->overlayVms[2], digit + 15);
            thousandsVisible = true;
        }
        this->overlayVms[2].positionOffset.x = digitX;
        this->overlayVms[2].positionOffset.y = digitY;
        this->overlayVms[2].positionOffset.z = digitZ;
        digitX += 9.0f;

        digit = (score / 100) % 10;
        if (digit != 0 || thousandsVisible)
        {
            g_PhotoStageState->anm->InitializeVm(&this->overlayVms[3], 0x1e);
            g_PhotoStageState->anm->SetSprite(
                &this->overlayVms[3], digit + 15);
        }
        this->overlayVms[3].positionOffset.x = digitX;
        this->overlayVms[3].positionOffset.y = digitY;
        this->overlayVms[3].positionOffset.z = digitZ;
        digitX += 9.0f;

        g_PhotoStageState->anm->InitializeVm(&this->overlayVms[4], 0x1e);
        g_PhotoStageState->anm->SetSprite(
            &this->overlayVms[4], (score / 10) % 10 + 15);
        this->overlayVms[4].positionOffset.x = digitX;
        this->overlayVms[4].positionOffset.y = digitY;
        this->overlayVms[4].positionOffset.z = digitZ;
        digitX += 9.0f;

        g_PhotoStageState->anm->InitializeVm(&this->overlayVms[5], 0x1e);
        g_PhotoStageState->anm->SetSprite(
            &this->overlayVms[5], score % 10 + 15);
        this->overlayVms[5].positionOffset.x = digitX;
        this->overlayVms[5].positionOffset.y = digitY;
        this->overlayVms[5].positionOffset.z = digitZ;
    }

    i32 renderMode = 4;
    displayPosition.y = photoY + 16.0f;
    i32 displayVmCount = 0;
    memset(
        g_PhotoStageState->displayVms, 0,
        sizeof(g_PhotoStageState->displayVms));
    g_PhotoStageState->flags &= ~PHOTO_STAGE_PLAYER_PASSED;
    displayPosition.x = photoX;
    displayPosition.z = photoZ;

    if (((u32)scoreData[7] >> 5 & 1) != 0)
    {
        ADD_PHOTO_STAGE_DISPLAY_VM(0x24);
        renderMode = 8;
        displayPosition.y += 12.0f;
    }
    if (((u32)scoreData[7] >> 6 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x25, 300);
    }
    if (((u32)scoreData[7] >> 7 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x26, 300);
    }
    if (((u32)scoreData[7] >> 8 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x27, 300);
    }
    if (((u32)scoreData[7] >> 9 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x28, 300);
    }
    if (((u32)scoreData[7] >> 10 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x29, 300);
    }
    if (((u32)scoreData[7] >> 11 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2a, 300);
    }
    if (((u32)scoreData[7] >> 12 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2b, 300);
    }
    if (((u32)scoreData[7] >> 13 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2c, 900);
    }
    if (((u32)scoreData[7] >> 14 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2d, 2100);
    }
    if (((u32)scoreData[7] >> 15 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2e, 0);
    }
    if (((u32)scoreData[7] >> 16 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x2f, 100);
    }
    if (((u32)scoreData[7] >> 17 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x30, 100);
    }
    if (((u32)scoreData[7] >> 18 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x31, 0);
    }
    if (((u32)scoreData[7] >> 19 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x32, 0);
    }
    if (((u32)scoreData[7] >> 4 & 1) != 0)
    {
        ADD_PHOTO_STAGE_SCORE_ROW(0x23, scoreData[4]);
    }

    if ((scoreData[7] & PHOTO_STAGE_SCORE_ENEMY) != 0)
    {
        ADD_PHOTO_STAGE_DISPLAY_VM(0x1f);
        displayPosition.x += 99.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(
            (i32)(*reinterpret_cast<const f32 *>(&scoreData[5]) * 10.0f) /
                10 + 15);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x1a);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(
            (i32)(*reinterpret_cast<const f32 *>(&scoreData[5]) * 10.0f) %
                10 + 15);
        renderMode += 4;
        displayPosition.y += 12.0f;
        displayPosition.x = photoX;
    }
    if (((u32)scoreData[7] >> 1 & 1) != 0)
    {
        ADD_PHOTO_STAGE_DISPLAY_VM(0x20);
        displayPosition.x += 99.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x10);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x1a);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x11);
        renderMode += 4;
        displayPosition.y += 12.0f;
        displayPosition.x = photoX;
    }
    if (((u32)scoreData[7] >> 2 & 1) != 0)
    {
        ADD_PHOTO_STAGE_DISPLAY_VM(0x21);
        displayPosition.x += 99.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x10);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x1a);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x14);
        renderMode += 4;
        displayPosition.y += 12.0f;
        displayPosition.x = photoX;
    }
    if (((u32)scoreData[7] >> 3 & 1) != 0)
    {
        ADD_PHOTO_STAGE_DISPLAY_VM(0x22);
        displayPosition.x += 99.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(
            (i32)(*reinterpret_cast<const f32 *>(&scoreData[6]) * 10.0f) /
                10 + 15);
        displayPosition.x += 9.0f;
        ADD_PHOTO_STAGE_DISPLAY_VM(0x1a);
        displayPosition.x += 9.0f;
        AnmVm *displayVm =
            &g_PhotoStageState->displayVms[displayVmCount];
        InitializePhotoStageDisplayVm(
            displayVm, &displayPosition,
            (i32)(*reinterpret_cast<const f32 *>(&scoreData[6]) * 10.0f) %
                    10 +
                15,
            renderMode);
        displayPosition.y += 12.0f;
        displayPosition.x = photoX;
    }

    g_PhotoStageState->boundaryX = displayPosition.x;
    g_PhotoStageState->boundaryY = displayPosition.y;
    g_PhotoStageState->unknown25714 =
        *reinterpret_cast<i32 *>(&displayPosition.z);
}

#undef ADD_PHOTO_STAGE_SCORE_ROW
#undef ADD_PHOTO_STAGE_DISPLAY_VM

i32 PhotoStageStateView::Update()
{
    i32 slotIndex;
    i32 vmIndex;

    for (slotIndex = 0; slotIndex < 11; slotIndex++)
    {
        for (vmIndex = 0; vmIndex < 6; vmIndex++)
        {
            AnmManager::ExecuteScript(&this->slots[slotIndex].primaryVms[vmIndex]);
            AnmManager::ExecuteScript(&this->slots[slotIndex].overlayVms[vmIndex]);
        }
    }

    PhotoAnmVmId *entryVms = this->GetEntryVms();
    if (entryVms[0] == 0 && g_PhotoGame->camera.photoLimit > 0)
    {
        for (vmIndex = 0; vmIndex < g_PhotoGame->camera.photoLimit; vmIndex++)
        {
            Float3 position;
            position.x = vmIndex < 5 ? 64.0f : 576.0f;
            position.y = 400.0f - (f32)(vmIndex % 5) * 80.0f;
            position.z = 0.0f;
            entryVms[vmIndex] =
                g_PhotoStageSupervisor->photoAnm->CreateVm(12, &position);
        }
    }

    if ((this->flags & PHOTO_STAGE_CAPTURING) != 0)
    {
        i32 captureSlot = this->GetCaptureSlot();

        if (this->captureFrame == 1)
        {
            Float3 capturePosition;
            PhotoToScreen(&capturePosition, this->GetCapturePosition());

            if (this->GetCaptureWidth() < 1)
            {
                this->GetCaptureWidth() = 64;
                this->GetCaptureHeight() = 48;
                this->flags |= PHOTO_STAGE_WAITING_FOR_TEXTURE;
            }
            else
            {
                i32 left = (i32)capturePosition.x -
                    (this->GetCaptureWidth() - 6) / 2;
                i32 right = left - 6 + this->GetCaptureWidth();
                i32 top = (i32)capturePosition.y -
                    (this->GetCaptureHeight() - 6) / 2;
                i32 bottom = top - 6 + this->GetCaptureHeight();

                if ((f32)left < 128.0f)
                {
                    left = 128;
                }
                if ((f32)right >= 512.0f)
                {
                    right = 511;
                }
                if ((f32)top < 16.0f)
                {
                    top = 16;
                }
                if ((f32)bottom >= 464.0f)
                {
                    bottom = 463;
                }

                this->GetCaptureWidth() = right - left + 6;
                this->GetCaptureHeight() = bottom - top + 6;

                PhotoStageAnmManagerView *anmManager =
                    GetPhotoStageAnmManager();
                if (anmManager->captureAnmIdx < 0)
                {
                    anmManager->captureAnmIdx = 9;
                    anmManager->captureSourceX = left;
                    anmManager->captureSourceY = top;
                    anmManager->captureSourceWidth = right - left;
                    anmManager->captureSourceHeight = bottom - top;
                    anmManager->captureDestinationX = 3;
                    anmManager->captureDestinationY = 3;
                    anmManager->captureDestinationWidth = right - left;
                    anmManager->captureDestinationHeight = bottom - top;
                    anmManager->captureFlags = captureSlot;
                }

                g_PhotoCaptureCountdown = 99;
                g_PhotoStageGlobalState->flags |= 0x80;
                this->flags &= ~PHOTO_STAGE_WAITING_FOR_TEXTURE;
            }

            PhotoStageTextureEntry *textures = GetPhotoStageTextures(this);
            IDirect3DSurface8 *surface = NULL;
            D3DLOCKED_RECT lockedRect;
            textures[captureSlot].texture->GetSurfaceLevel(0, &surface);
            surface->LockRect(&lockedRect, NULL, 0);

            i32 bytesPerPixel = textures[captureSlot].bytesPerPixel;
            for (i32 y = 0; y < this->GetCaptureHeight(); y++)
            {
                memset(
                    reinterpret_cast<u8 *>(lockedRect.pBits) +
                        y * lockedRect.Pitch,
                    0,
                    this->GetCaptureWidth() * bytesPerPixel);
            }
            for (i32 y = 0; y < 3; y++)
            {
                memset(
                    reinterpret_cast<u8 *>(lockedRect.pBits) +
                        y * lockedRect.Pitch,
                    0xff,
                    this->GetCaptureWidth() * bytesPerPixel);
            }
            for (i32 y = this->GetCaptureHeight() - 3;
                 y < this->GetCaptureHeight();
                 y++)
            {
                memset(
                    reinterpret_cast<u8 *>(lockedRect.pBits) +
                        y * lockedRect.Pitch,
                    0xff,
                    this->GetCaptureWidth() * bytesPerPixel);
            }
            for (i32 y = 3; y < this->GetCaptureHeight() - 3; y++)
            {
                u8 *row = reinterpret_cast<u8 *>(lockedRect.pBits) +
                    y * lockedRect.Pitch;
                memset(row, 0xff, bytesPerPixel * 3);
                memset(
                    row + (this->GetCaptureWidth() - 3) * bytesPerPixel,
                    0xff,
                    bytesPerPixel * 3);
            }

            surface->UnlockRect();
            surface->Release();

            if (this->capturedPhotoVms[captureSlot] != 0)
            {
                GetPhotoStageAnmManager()->MarkVmForDeletion(
                    this->capturedPhotoVms[captureSlot]);
            }
            this->capturedPhotoVms[captureSlot] =
                GetPhotoStageAnm(this->anm)->CreateVm(captureSlot * 2, 0);

            AnmVm *vm =
                GetPhotoStageAnmManager()->GetVm(
                    this->capturedPhotoVms[captureSlot]);
            vm->loadedSprite->uvEnd.x =
                (f32)this->GetCaptureWidth() / 256.0f;
            vm->loadedSprite->uvEnd.y =
                (f32)this->GetCaptureHeight() / 256.0f;
            vm->spriteSize.x = (f32)this->GetCaptureWidth();
            vm->spriteSize.y = (f32)this->GetCaptureHeight();
            GetPhotoStageAnmManager()->SetPosition(
                this->capturedPhotoVms[captureSlot], &capturePosition);

            g_PhotoStageGlobalState->flags &= ~1;
            g_PhotoStageGlobalState->flags |= 2;
        }
        else if (this->captureFrame == 2)
        {
            if ((this->flags & PHOTO_STAGE_WAITING_FOR_TEXTURE) == 0)
            {
                SpawnPhotoStageEffect(3, 15, 1, 0xc0ffafcf, 0, 0x1d);

                PhotoStageSlot *slot = &this->slots[captureSlot];
                if ((slot->flags & 1) != 0)
                {
                    g_PhotoStageGlobalState->currentScore += slot->score;
                }

                if (g_PhotoStageGlobalState->resultMode == 0)
                {
                    i32 scoreIndex = g_PhotoStageGlobalState->scoreIndex;
                    u8 *scoreData = reinterpret_cast<u8 *>(g_PhotoStageSaveData) +
                        scoreIndex * 0x60;
                    u32 *attemptCount = reinterpret_cast<u32 *>(scoreData + 0x4a4);
                    if (*attemptCount < 999999)
                    {
                        (*attemptCount)++;
                    }

                    time(reinterpret_cast<time_t *>(&slot->timestamp));

                    PhotoStageBestShotRecord *record =
                        &g_PhotoStageSaveData->bestShotRecords[scoreIndex];
                    f32 *recordSlowRate =
                        reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(record) + 0x14);
                    *recordSlowRate = 100.0f -
                        (f32)(g_PhotoLagNumerator / g_PhotoLagDenominator * 100.0);
                    slot->slowRate = *recordSlowRate;
                    slot->width = this->GetCaptureWidth();
                    slot->height = this->GetCaptureHeight();
                    if (g_PhotoStageRuntime == NULL)
                    {
                        memset(slot->comment, 0, 0x50);
                    }
                    else
                    {
                        strcpy(slot->comment, g_PhotoStageRuntime->comment);
                    }

                    u32 scoreFlags = *reinterpret_cast<u32 *>(scoreData + 0x4b0);
                    i32 bestScore = *reinterpret_cast<i32 *>(scoreData + 0x478);
                    if (((scoreFlags >> 1) & 1) == 0 && bestScore < slot->score)
                    {
                        memcpy(scoreData + 0x478, slot->scoreData, 8 * sizeof(i32));
                        g_PhotoStageSaveData->UpdateBestShotRecord(scoreIndex);

                        record->valid = 1;
                        record->magic = 0x53545342;
                        record->width = (u16)this->GetCaptureWidth();
                        record->height = (u16)this->GetCaptureHeight();
                        record->score = slot->score;
                        record->group = (u16)(g_PhotoStagePlayerConfig->group + 1);
                        record->scene = (u16)(g_PhotoStagePlayerConfig->scene + 1);
                        record->type = 2;
                        record->version = 0x102;
                        record->componentCount =
                            (u8)((GetPhotoStageTextures(this)[captureSlot]
                                      .bytesPerPixel == 4) + 2);
                        *reinterpret_cast<f32 *>(scoreData + 0x4ac) =
                            slot->slowRate;
                        *reinterpret_cast<i32 *>(scoreData + 0x49c) =
                            slot->timestamp;
                        if (g_PhotoStageRuntime == NULL)
                        {
                            memset(record->comment, 0, 0x50);
                        }
                        else
                        {
                            strcpy(record->comment, g_PhotoStageRuntime->comment);
                        }

                        if (captureSlot == 10)
                        {
                            record->photoIndex = -1;
                            this->CapturePhotoPixels(captureSlot);
                        }
                        else
                        {
                            record->photoIndex = captureSlot;
                        }
                    }
                }

                g_PhotoStageEffectManager->CommitCapturedObjects();
                g_PhotoStageBulletManager->ClearCapturedBullets();
            }
        }
        else if (this->captureFrame == 10)
        {
            if ((this->flags & PHOTO_STAGE_WAITING_FOR_TEXTURE) == 0 &&
                captureSlot != 10)
            {
                vmIndex = g_PhotoGame->camera.photoIndex - 1;
                GetPhotoStageAnmManager()->SetInterrupt(entryVms[vmIndex], 1);
            }
        }
        else if (this->captureFrame == 35)
        {
            if ((this->flags & PHOTO_STAGE_WAITING_FOR_TEXTURE) == 0 &&
                captureSlot != 10)
            {
                vmIndex = g_PhotoGame->camera.photoIndex - 1;
                GetPhotoStageAnmManager()->SetInterrupt(entryVms[vmIndex], 1);

                AnmVm *vm =
                    GetPhotoStageAnmManager()->GetVm(
                        this->capturedPhotoVms[captureSlot]);
                Float3 position;
                position.x = 0.0f;
                position.y = (-vm->spriteSize.y * 0.4f) / 2.0f;
                position.z = 0.0f;
                RotatePhotoStagePoint(&position, &position, vm->rotation.z);
                position += vm->position;
                entryVms[vmIndex] =
                    g_PhotoStageSupervisor->photoAnm->CreateVm(10, &position);
                position.y -= 6.0f;
                g_PhotoStageSupervisor->photoAnm->CreateVm(11, &position);
            }

            g_PhotoStageGlobalState->flags &= ~2;
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

    if ((this->flags & PHOTO_STAGE_PLAYER_PASSED) == 0)
    {
        if (g_PhotoGame->playerPosition.y < this->boundaryY + 32.0f &&
            ((this->boundaryX < 320.0f && g_PhotoGame->playerPosition.x < 0.0f) ||
             (this->boundaryX >= 320.0f && g_PhotoGame->playerPosition.x >= 0.0f)))
        {
            this->flags |= PHOTO_STAGE_PLAYER_PASSED;
            for (vmIndex = 0; vmIndex < 80; vmIndex++)
            {
                AnmVm *vm = &this->displayVms[vmIndex];
                if (vm->counterVar1 != 0)
                {
                    vm->interpCurrentTimers[ANM_INTERP_ALPHA1] = 0;
                    vm->interpEndTimers[ANM_INTERP_ALPHA1] = 16;
                    vm->interpModes[ANM_INTERP_ALPHA1] = ANM_INTERP_LINEAR;
                    vm->color1Initial.a = vm->color1.a;
                    vm->color1Final.a = 0x20;
                }
            }
        }
    }
    else if (g_PhotoGame->playerPosition.y >= this->boundaryY + 32.0f ||
             ((this->boundaryX >= 320.0f || g_PhotoGame->playerPosition.x >= 0.0f) &&
              (this->boundaryX < 320.0f || g_PhotoGame->playerPosition.x < 0.0f)))
    {
        this->flags &= ~PHOTO_STAGE_PLAYER_PASSED;
        for (vmIndex = 0; vmIndex < 80; vmIndex++)
        {
            AnmVm *vm = &this->displayVms[vmIndex];
            if (vm->counterVar1 != 0)
            {
                vm->interpCurrentTimers[ANM_INTERP_ALPHA1] = 0;
                vm->interpEndTimers[ANM_INTERP_ALPHA1] = 16;
                vm->interpModes[ANM_INTERP_ALPHA1] = ANM_INTERP_LINEAR;
                vm->color1Initial.a = vm->color1.a;
                vm->color1Final.a = 0xff;
            }
        }
    }

    for (vmIndex = 0; vmIndex < 80; vmIndex++)
    {
        AnmManager::ExecuteScript(&this->displayVms[vmIndex]);
    }

    return 1;
}

i32 __fastcall UpdatePhotoStage(PhotoStageStateView *stage)
{
    if (((g_PhotoStageGlobalState->flags >> 2) & 1) != 0)
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

    reinterpret_cast<PhotoStageDisplayView *>(
        &this->slots[slotIndex].primaryVms[0])
        ->Build(
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
            (u32)g_PhotoStageSaveData
                ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
                .width,
            (u32)g_PhotoStageSaveData
                ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
                .height) *
        (u32)g_PhotoStageSaveData
            ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
            .componentCount;
    locals.record = &g_PhotoStageSaveData
        ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex];
    locals.record->pixelData =
        reinterpret_cast<u8 *>(malloc(locals.allocationSize));

    reinterpret_cast<PhotoStageTextureEntry *>(this->anm->textures)
        [photoIndex]
            .texture->GetSurfaceLevel(0, &locals.surface);
    locals.surface->LockRect(&locals.lockedRect, NULL, 0);

    locals.destination = g_PhotoStageSaveData
        ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
        .pixelData;
    if (g_PhotoStageSaveData
            ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
            .componentCount == 3)
    {
        for (locals.y = 0;
             locals.y <
                 (i32)g_PhotoStageSaveData
                     ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
                     .height;
             locals.y++)
        {
            locals.source =
                reinterpret_cast<u8 *>(locals.lockedRect.pBits) +
                locals.y * locals.lockedRect.Pitch;
            for (locals.x = 0;
                 locals.x <
                     (i32)g_PhotoStageSaveData
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
                 (i32)g_PhotoStageSaveData
                     ->bestShotRecords[g_PhotoStageGlobalState->scoreIndex]
                     .height;
             locals.y++)
        {
            locals.source =
                reinterpret_cast<u8 *>(locals.lockedRect.pBits) +
                locals.y * locals.lockedRect.Pitch;
            for (locals.x = 0;
                 locals.x <
                     (i32)g_PhotoStageSaveData
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
