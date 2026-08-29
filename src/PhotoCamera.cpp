#include "PhotoCamera.hpp"

namespace th095
{

struct PhotoEnemyView
{
    u8 unknown0000[0x28a0];
    Float3 position;                   // +0x28a0
    u8 unknown28ac[0x2bf4 - 0x28ac];
    u32 flags;                         // +0x2bf4
    u32 flags2;                        // +0x2bf8
    u8 unknown2bfc[0x2c28 - 0x2bfc];
    f32 photoRateNumerator;            // +0x2c28
    i32 photoRateCount;                // +0x2c2c
    u8 unknown2c30[0x2c34 - 0x2c30];
    f32 photoRateDenominator;          // +0x2c34

    i32 HasPhotoRate() const
    {
        return this->photoRateCount > 0;
    }
};

typedef char PhotoEnemyPositionAt28A0[
    (offsetof(PhotoEnemyView, position) == 0x28a0) ? 1 : -1];
typedef char PhotoEnemyFlagsAt2BF4[
    (offsetof(PhotoEnemyView, flags) == 0x2bf4) ? 1 : -1];
typedef char PhotoEnemyPhotoRateAt2C28[
    (offsetof(PhotoEnemyView, photoRateNumerator) == 0x2c28) ? 1 : -1];

struct PhotoRuntimeView
{
    u8 unknown000000[0x26ae00];
    PhotoEnemyView *enemies[8];

    i32 CountPhotoTargets(const Float3 *position, const Float3 *size);
};

struct PhotoCapturedBulletView
{
    u8 unknown000[0x248];
    AnmVm *vm;                         // +0x248
    u8 unknown24c[0x2f4 - 0x24c];
    f32 photoScale;                    // +0x2f4
    u8 unknown2f8[0x35c - 0x2f8];
    PhotoCapturedBulletView *next;     // +0x35c
    u8 unknown360[0x656 - 0x360];
    i16 group;                         // +0x656
    i16 kind;                          // +0x658
};

typedef char PhotoCapturedBulletVmAt248[
    (offsetof(PhotoCapturedBulletView, vm) == 0x248) ? 1 : -1];
typedef char PhotoCapturedBulletScaleAt2F4[
    (offsetof(PhotoCapturedBulletView, photoScale) == 0x2f4) ? 1 : -1];
typedef char PhotoCapturedBulletNextAt35C[
    (offsetof(PhotoCapturedBulletView, next) == 0x35c) ? 1 : -1];
typedef char PhotoCapturedBulletKindAt658[
    (offsetof(PhotoCapturedBulletView, kind) == 0x658) ? 1 : -1];

struct PhotoGlobalStateView
{
    u8 unknown000[0xfc];
    u32 flags;
};

struct PhotoStageStateView
{
    u8 unknown00000[0x25718];
    f32 scoreMultiplier;
    PhotoAnmLoadedView *anm;
    u32 flags;

    void SavePhoto(i32 slot, const Float3 *position, i32 width, i32 height,
                   i32 score, const i32 *scoreData);
};

struct PhotoStageControllerView
{
    i32 CountNearbyTargets(const Float3 *position, f32 radius);
    i32 CountPhotoTargets(const Float3 *position, const Float3 *size);
};

struct PhotoAnmSpawnerView
{
    // ABI-facing form of the small-structure return used by the target call.
    void SpawnInto(PhotoAnmVmId *output, i32 script, Float3 *position);
};

struct PhotoBulletManagerView
{
    u8 unknown0000[0x1760];
    ZunColor photoColor;
    u8 unknown1764[0x27c5b0 - 0x1764];
    PhotoAnmSpawnerView *anmSpawner;

    void BeginPhotoCapture(const Float3 *position, const Float3 *size);
    i32 CountNearbyTargets(const Float3 *position, f32 radius);
    void *CapturePhotoTargets(const Float3 *position, const Float3 *size);
};

struct PhotoAnmManagerView
{
    AnmVm *FindVm(i32 id);
    void SetVmInterrupt(i32 id, i16 interrupt);
    void RemoveVm(i32 id);
    void SetVmPosition(i32 id, const Float3 *position);
};

struct PhotoSoundPlayerView
{
    void PlaySoundByIdx(i32 idx, i32 pan);
    void PlaySoundPositionedByIdx(i32 idx, f32 pan);
    void StopSoundByIdx(i32 idx);
};

static inline PhotoAnmManagerView *PhotoAnmManager()
{
    return reinterpret_cast<PhotoAnmManagerView *>(g_AnmManager);
}

static inline PhotoSoundPlayerView *PhotoSoundPlayer()
{
    return reinterpret_cast<PhotoSoundPlayerView *>(&g_SoundPlayer);
}

extern PhotoGameStateView *g_PhotoGame;
extern PhotoRuntimeView *g_PhotoRuntime;
extern PhotoGlobalStateView *g_PhotoGlobalState;
extern PhotoStageStateView *g_PhotoStageState;
extern PhotoStageControllerView *g_PhotoStageController;
extern PhotoBulletManagerView *g_PhotoBulletManager;
extern u16 g_PhotoInput;
extern u16 g_PhotoInputPressed;

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

enum PhotoCameraFlags
{
    PHOTO_FLAG_ALTERNATE_CAPTURE = 1 << 0,
    PHOTO_FLAG_FOCUSED = 1 << 1,
    PHOTO_FLAG_TARGET_FRAME_ACTIVE = 1 << 2,
    PHOTO_FLAG_CHARGE_UI_MASK = 3 << 3,
    PHOTO_FLAG_CHARGE_EFFECT_ACTIVE = 1 << 5,
    PHOTO_FLAG_TARGET_SOUND_PLAYED = 1 << 6,
};

enum PhotoScoreFlags
{
    PHOTO_SCORE_ENEMY = 1 << 0,
    PHOTO_SCORE_SELF = 1 << 1,
    PHOTO_SCORE_TWO_SHOT = 1 << 2,
    PHOTO_SCORE_BOSS_RATE = 1 << 3,
    PHOTO_SCORE_NEARBY = 1 << 4,
    PHOTO_SCORE_COLOR_1 = 1 << 6,
    PHOTO_SCORE_COLOR_2 = 1 << 7,
    PHOTO_SCORE_COLOR_3 = 1 << 8,
    PHOTO_SCORE_COLOR_4 = 1 << 9,
    PHOTO_SCORE_COLOR_5 = 1 << 10,
    PHOTO_SCORE_COLOR_6 = 1 << 11,
    PHOTO_SCORE_COLOR_7 = 1 << 12,
    PHOTO_SCORE_COLORFUL = 1 << 13,
    PHOTO_SCORE_RAINBOW = 1 << 14,
    PHOTO_SCORE_EMPTY = 1 << 15,
    PHOTO_SCORE_NO_BULLETS = 1 << 16,
};

enum PhotoViewfinderDirection
{
    PHOTO_DIRECTION_NONE = 0,
    PHOTO_DIRECTION_UP = 1,
    PHOTO_DIRECTION_DOWN = 2,
    PHOTO_DIRECTION_LEFT = 3,
    PHOTO_DIRECTION_RIGHT = 4,
    PHOTO_DIRECTION_UP_LEFT = 5,
    PHOTO_DIRECTION_UP_RIGHT = 6,
    PHOTO_DIRECTION_DOWN_LEFT = 7,
    PHOTO_DIRECTION_DOWN_RIGHT = 8,
};

static inline bool PhotoSoundsEnabled()
{
    return ((g_PhotoGlobalState->flags >> 9) & 1) == 0;
}

static inline u16 PhotoInputMask(u16 input, u16 mask)
{
    return input & mask;
}

static inline void SetPhotoVmColor(AnmVm *vm, u8 red, u8 green, u8 blue)
{
    vm->color1.a = 0xff;
    vm->color1.r = red;
    vm->color1.g = green;
    vm->color1.b = blue;
}

static inline i32 PreservePhotoId(i32 value)
{
    return *reinterpret_cast<volatile i32 *>(&value);
}

static inline i32 PhotoEnemyIsOffscreen(const Float3 *position)
{
    return position->x + 8.0f <= -192.0f ||
           position->x - 8.0f >= 192.0f ||
           position->y + 8.0f <= 0.0f ||
           position->y - 8.0f >= 448.0f;
}

static inline i32 PhotoRectangleContains(
    const Float3 *objectPosition, f32 objectWidth, f32 objectHeight,
    const Float3 *containerPosition, f32 containerWidth,
    f32 containerHeight)
{
    objectWidth *= 0.5f;
    objectHeight *= 0.5f;
    containerWidth *= 0.5f;
    containerHeight *= 0.5f;
    return objectPosition->x - objectWidth >=
               containerPosition->x - containerWidth &&
           objectPosition->x + objectWidth <=
               containerPosition->x + containerWidth &&
           objectPosition->y - objectHeight >=
               containerPosition->y - containerHeight &&
           objectPosition->y + objectHeight <=
               containerPosition->y + containerHeight;
}

static inline f32 PhotoRatio(volatile f32 denominator, volatile f32 numerator)
{
    f32 unused;
    return numerator / denominator;
}

static inline i32 PhotoTimerAdvancedOnEvenFrame(ZunTimer *timer)
{
    return timer->current != timer->previous && timer->current % 2 == 0;
}

f32 PhotoGameStateView::AngleToPoint(const Float3 *point)
{
    f32 deltaX = point->x - this->playerPosition.x;
    f32 deltaY = point->y - this->playerPosition.y;

    if (deltaY == 0.0f && deltaX == 0.0f)
    {
        return 1.5707964f;
    }
    return atan2f(deltaY, deltaX);
}

void PhotoCameraState::BeginCapture()
{
    if (this->mode != PHOTO_CAMERA_TRACKING)
    {
        return;
    }
    this->mode = PHOTO_CAMERA_CHARGING;
    this->modeTimer = 0;
    this->flags &= ~PHOTO_FLAG_TARGET_SOUND_PLAYED;
    this->vmIds[2] = g_PhotoStageState->anm->CreateVm(0x18, 0);
    this->vmIds[3] = g_PhotoStageState->anm->CreateVm(0x19, 0);
    this->vmIds[4] = g_PhotoStageState->anm->CreateVm(0x1a, 0);
    this->vmIds[5] = g_PhotoStageState->anm->CreateVm(0x1b, 0);
    this->vmIds[6] = g_PhotoStageState->anm->CreateVm(0x1c, 0);
    if (this->vmIds[9] != 0)
    {
        PhotoAnmManager()->RemoveVm(this->vmIds[9].value);
        this->vmIds[9].value = PreservePhotoId(0);
    }
    if (this->vmIds[10] != 0)
    {
        PhotoAnmManager()->RemoveVm(this->vmIds[10].value);
        this->vmIds[10].value = PreservePhotoId(0);
    }
    if (((g_PhotoGlobalState->flags >> 9) & 1) == 0)
    {
        PhotoSoundPlayer()->PlaySoundByIdx(0x2c, 0);
    }
}

void PhotoCameraState::UpdateViewfinder()
{
    struct ViewfinderLocals
    {
        Float3 cornerPosition;
        Float3 screenPosition;
        AnmVm *centerVm;
        PhotoViewfinderDirection direction;
        f32 offsetX;
        f32 offsetY;
    } locals;

    locals.offsetX = 0.0f;
    locals.offsetY = 0.0f;

    if (PhotoInputMask(g_PhotoInput, 0x50) == 0x50)
    {
        locals.direction = PHOTO_DIRECTION_UP_LEFT;
    }
    else if (PhotoInputMask(g_PhotoInput, 0x60) == 0x60)
    {
        locals.direction = PHOTO_DIRECTION_DOWN_LEFT;
    }
    else if (PhotoInputMask(g_PhotoInput, 0x90) == 0x90)
    {
        locals.direction = PHOTO_DIRECTION_UP_RIGHT;
    }
    else if (PhotoInputMask(g_PhotoInput, 0xa0) == 0xa0)
    {
        locals.direction = PHOTO_DIRECTION_DOWN_RIGHT;
    }
    else if (PhotoInputMask(g_PhotoInput, 0x20) != 0)
    {
        locals.direction = PHOTO_DIRECTION_DOWN;
    }
    else if (PhotoInputMask(g_PhotoInput, 0x10) != 0)
    {
        locals.direction = PHOTO_DIRECTION_UP;
    }
    else if (PhotoInputMask(g_PhotoInput, 0x40) != 0)
    {
        locals.direction = PHOTO_DIRECTION_LEFT;
    }
    else if (PhotoInputMask(g_PhotoInput, 0x80) != 0)
    {
        locals.direction = PHOTO_DIRECTION_RIGHT;
    }
    else
    {
        locals.direction = PHOTO_DIRECTION_NONE;
    }

    switch (locals.direction)
    {
    case PHOTO_DIRECTION_RIGHT:
        locals.offsetX = 3.3f;
        break;
    case PHOTO_DIRECTION_LEFT:
        locals.offsetX = -3.3f;
        break;
    case PHOTO_DIRECTION_UP:
        locals.offsetY = -3.3f;
        break;
    case PHOTO_DIRECTION_DOWN:
        locals.offsetY = 3.3f;
        break;
    case PHOTO_DIRECTION_UP_LEFT:
        locals.offsetX = -2.3334749f;
        locals.offsetY = locals.offsetX;
        break;
    case PHOTO_DIRECTION_DOWN_LEFT:
        locals.offsetY = 2.3334749f;
        locals.offsetX = -locals.offsetY;
        break;
    case PHOTO_DIRECTION_UP_RIGHT:
        locals.offsetX = 2.3334749f;
        locals.offsetY = -locals.offsetX;
        break;
    case PHOTO_DIRECTION_DOWN_RIGHT:
        locals.offsetX = 2.3334749f;
        locals.offsetY = locals.offsetX;
        break;
    }

    this->viewfinderPosition.x += locals.offsetX;
    this->viewfinderPosition.y += locals.offsetY;

    if (this->viewfinderPosition.x < -184.0)
    {
        this->viewfinderPosition.x = -184.0f;
    }
    else if (this->viewfinderPosition.x > 184.0)
    {
        this->viewfinderPosition.x = 184.0f;
    }
    if (this->viewfinderPosition.y < 32.0f)
    {
        this->viewfinderPosition.y = 32.0f;
    }
    else if (this->viewfinderPosition.y > 436.0f)
    {
        this->viewfinderPosition.y = 436.0f;
    }

    this->viewfinderSize.x = this->charge * 160.0f + 48.0f;
    this->viewfinderSize.y =
        (this->charge * 160.0f + 48.0f) * 0.75f;
    this->viewfinderSize.z = 0.0f;

    PhotoToScreen(&locals.screenPosition, &this->viewfinderPosition);
    locals.cornerPosition.z = 0.0f;
    locals.cornerPosition.x =
        locals.screenPosition.x - this->viewfinderSize.x / 2.0f;
    locals.cornerPosition.y =
        locals.screenPosition.y + this->viewfinderSize.y / 2.0f;
    PhotoAnmManager()->SetVmPosition(
        this->vmIds[2].value, &locals.cornerPosition);
    locals.cornerPosition.x =
        locals.screenPosition.x - this->viewfinderSize.x / 2.0f;
    locals.cornerPosition.y =
        locals.screenPosition.y - this->viewfinderSize.y / 2.0f;
    PhotoAnmManager()->SetVmPosition(
        this->vmIds[3].value, &locals.cornerPosition);
    locals.cornerPosition.x =
        locals.screenPosition.x + this->viewfinderSize.x / 2.0f;
    locals.cornerPosition.y =
        locals.screenPosition.y - this->viewfinderSize.y / 2.0f;
    PhotoAnmManager()->SetVmPosition(
        this->vmIds[4].value, &locals.cornerPosition);
    locals.cornerPosition.x =
        locals.screenPosition.x + this->viewfinderSize.x / 2.0f;
    locals.cornerPosition.y =
        locals.screenPosition.y + this->viewfinderSize.y / 2.0f;
    PhotoAnmManager()->SetVmPosition(
        this->vmIds[5].value, &locals.cornerPosition);

    locals.centerVm = PhotoAnmManager()->FindVm(this->vmIds[6].value);
    if (locals.centerVm != NULL)
    {
        locals.centerVm->scale.y = this->charge * 2.0f;
        locals.centerVm->scale.x = locals.centerVm->scale.y;
    }
    reinterpret_cast<PhotoAnmManagerView *>(g_AnmManager)->SetVmPosition(
        this->vmIds[6].value,
        PhotoToScreen(&locals.screenPosition, &this->viewfinderPosition));
}

u32 PhotoCameraState::TakePhoto()
{
    i32 scoreData[8];

    PhotoAnmManager()->RemoveVm(this->vmIds[2].value);
    PhotoAnmManager()->RemoveVm(this->vmIds[3].value);
    PhotoAnmManager()->RemoveVm(this->vmIds[4].value);
    PhotoAnmManager()->RemoveVm(this->vmIds[5].value);
    PhotoAnmManager()->RemoveVm(this->vmIds[6].value);

    scoreData[3] = g_PhotoBulletManager->CountNearbyTargets(
        &g_PhotoGame->playerPosition, 22.0f);
    scoreData[3] += g_PhotoStageController->CountNearbyTargets(
        &g_PhotoGame->playerPosition, 22.0f);

    this->CalculatePhotoScore(
        reinterpret_cast<PhotoCapturedBulletView *>(
            g_PhotoBulletManager->CapturePhotoTargets(
                &this->viewfinderPosition, &this->viewfinderSize)),
        scoreData,
        g_PhotoRuntime->CountPhotoTargets(
            &this->viewfinderPosition, &this->viewfinderSize),
        g_PhotoStageController->CountPhotoTargets(
            &this->viewfinderPosition, &this->viewfinderSize));

    if ((this->flags & PHOTO_FLAG_ALTERNATE_CAPTURE) != 0)
    {
        g_PhotoStageState->SavePhoto(
            this->photoIndex, &this->viewfinderPosition,
            (i32)this->viewfinderSize.x, (i32)this->viewfinderSize.y,
            scoreData[0], scoreData);
        this->photoIndex++;
    }
    else
    {
        g_PhotoStageState->SavePhoto(
            10, &this->viewfinderPosition,
            (i32)this->viewfinderSize.x, (i32)this->viewfinderSize.y,
            scoreData[0], scoreData);
    }

    this->photosTaken++;
    this->charge -= 1.0f;
    if (this->charge <= 0.0f)
    {
        this->charge = 0.0f;
    }
    if (this->photoIndex >= this->photoLimit)
    {
        this->charge = 0.0f;
        this->mode = PHOTO_CAMERA_DISABLED;
        g_PhotoGame->mode = 3;
        g_PhotoGame->completionTimer = 0;
    }
    else
    {
        this->mode = PHOTO_CAMERA_CAPTURED;
    }
    g_AnmGameSpeed = 1.0f;
    this->modeTimer = 0;
    PhotoSoundPlayer()->StopSoundByIdx(0x2c);
    if (((g_PhotoGlobalState->flags >> 9) & 1) == 0)
    {
        PhotoSoundPlayer()->PlaySoundByIdx(0x29, 0);
    }
    return this->flags & PHOTO_FLAG_ALTERNATE_CAPTURE;
}

void PhotoCameraState::CancelCapture()
{
    i32 scoreData[8];

    PhotoAnmManager()->RemoveVm(this->vmIds[2].value);
    PhotoAnmManager()->RemoveVm(this->vmIds[3].value);
    PhotoAnmManager()->RemoveVm(this->vmIds[4].value);
    PhotoAnmManager()->RemoveVm(this->vmIds[5].value);
    PhotoAnmManager()->RemoveVm(this->vmIds[6].value);
    memset(scoreData, 0, sizeof(scoreData));
    g_PhotoStageState->SavePhoto(
        10, &this->viewfinderPosition, 0, 0, 0, scoreData);
    this->charge = 0.5f;
    g_AnmGameSpeed = 1.0f;
    this->modeTimer = 0;
    this->flags &= ~PHOTO_FLAG_ALTERNATE_CAPTURE;
    this->mode = PHOTO_CAMERA_CAPTURED;
    g_AnmGameSpeed = 1.0f;
    this->modeTimer = 0;
    PhotoSoundPlayer()->StopSoundByIdx(0x2c);
}

i32 PhotoCameraState::CalculatePhotoScore(
    PhotoCapturedBulletView *bulletTargets, i32 *scoreData,
    i32 runtimeTargets, i32 stageTargets)
{
    i32 preservedNearbyTargets = scoreData[3];
    memset(scoreData, 0, sizeof(i32) * 8);
    scoreData[3] = preservedNearbyTargets;

    i32 colorCounts[9];
    colorCounts[8] = 0;
    PhotoCapturedBulletView *firstBullet = bulletTargets;
    scoreData[2] = 0;
    for (; bulletTargets != NULL; bulletTargets = bulletTargets->next)
    {
        scoreData[2]++;
        i32 bulletScore;
        if (bulletTargets->vm == NULL || bulletTargets->vm->scale.y <= 8.0f)
        {
            bulletScore = 10;
        }
        else if (bulletTargets->vm->scale.y <= 16.0f)
        {
            bulletScore = 20;
        }
        else if (bulletTargets->vm->scale.y <= 32.0f)
        {
            bulletScore = 40;
        }
        else if (bulletTargets->vm->scale.y <= 64.0f)
        {
            bulletScore = 150;
        }

        if (bulletTargets->photoScale >= 6.0f)
        {
            bulletScore *= 4;
        }
        else if (bulletTargets->photoScale >= 2.0f)
        {
            bulletScore += (i32)(
                (f32)bulletScore * (bulletTargets->photoScale - 2.0f) *
                4.0f / 4.0f);
        }
        bulletScore -= bulletScore % 10;
        colorCounts[8] += bulletScore;
    }

    colorCounts[8] += runtimeTargets * 170 + stageTargets * 10;
    scoreData[1] = colorCounts[8];

    f32 closestDistance;
    f32 bossRate;
    this->flags =
        (this->flags & ~PHOTO_FLAG_ALTERNATE_CAPTURE) |
        (this->CountPhotoTargets(&closestDistance, &bossRate) != 0);
    scoreData[7] =
        (scoreData[7] & ~PHOTO_SCORE_ENEMY) |
        (this->flags & PHOTO_FLAG_ALTERNATE_CAPTURE);

    f32 closestMultiplier;
    if (closestDistance >= 48.0f)
    {
        closestMultiplier = 1.2f;
    }
    else if (closestDistance <= 8.0f)
    {
        closestMultiplier = 2.0f;
    }
    else
    {
        closestMultiplier =
            2.0f - ((closestDistance - 8.0f) / 88.0f) * 0.8f;
    }
    *reinterpret_cast<f32 *>(&scoreData[5]) = closestMultiplier;

    scoreData[7] =
        (scoreData[7] & ~PHOTO_SCORE_BOSS_RATE) |
        ((bossRate > 0.0f) << 3);
    f32 bossMultiplier;
    if (bossRate >= 0.8f)
    {
        bossMultiplier = 1.5f;
    }
    else
    {
        bossMultiplier = bossRate * 0.3f / 0.8f + 1.2f;
    }
    *reinterpret_cast<f32 *>(&scoreData[6]) = bossMultiplier;

    f32 viewfinderHalfHeight = this->viewfinderSize.y;
    f32 viewfinderHalfWidth = this->viewfinderSize.x;
    Float3 *viewfinderPosition = &this->viewfinderPosition;
    f32 playerHalfHeight = 16.0f;
    f32 playerHalfWidth = 16.0f;
    Float3 *playerPosition = &g_PhotoGame->playerPosition;
    playerHalfWidth *= 0.5f;
    playerHalfHeight *= 0.5f;
    viewfinderHalfWidth *= 0.5f;
    viewfinderHalfHeight *= 0.5f;
    bool playerInViewfinder =
        playerPosition->x - playerHalfWidth <=
            viewfinderPosition->x + viewfinderHalfWidth &&
        viewfinderPosition->x - viewfinderHalfWidth <=
            playerPosition->x + playerHalfWidth &&
        playerPosition->y - playerHalfHeight <=
            viewfinderPosition->y + viewfinderHalfHeight &&
        viewfinderPosition->y - viewfinderHalfHeight <=
            playerPosition->y + playerHalfHeight;
    if (playerInViewfinder)
    {
        scoreData[7] |= PHOTO_SCORE_SELF;
    }

    if ((scoreData[7] & (PHOTO_SCORE_ENEMY | PHOTO_SCORE_SELF)) !=
        (PHOTO_SCORE_ENEMY | PHOTO_SCORE_SELF))
    {
        if ((scoreData[7] & PHOTO_SCORE_ENEMY) != 0)
        {
            if (scoreData[2] == 0)
            {
                scoreData[7] |= PHOTO_SCORE_NO_BULLETS;
                colorCounts[8] += 100;
            }
        }
        else if ((scoreData[7] & PHOTO_SCORE_SELF) != 0)
        {
            if (scoreData[2] == 0)
            {
                scoreData[7] |= PHOTO_SCORE_NO_BULLETS;
                colorCounts[8] += 100;
            }
        }
        else if (scoreData[2] == 0)
        {
            scoreData[7] |= PHOTO_SCORE_EMPTY;
        }
    }

    if (scoreData[3] > 2)
    {
        scoreData[4] =
            scoreData[3] >= 20 ? 2000 : scoreData[3] * 100;
        scoreData[7] |= PHOTO_SCORE_NEARBY;
    }

    bulletTargets = firstBullet;
    colorCounts[0] = 0;
    colorCounts[1] = 0;
    colorCounts[2] = 0;
    colorCounts[3] = 0;
    colorCounts[4] = 0;
    colorCounts[5] = 0;
    colorCounts[6] = 0;
    for (; bulletTargets != NULL; bulletTargets = bulletTargets->next)
    {
        if (bulletTargets->group < 12)
        {
            if (bulletTargets->kind == 1 || bulletTargets->kind == 2)
                colorCounts[0]++;
            else if (bulletTargets->kind == 3 || bulletTargets->kind == 4)
                colorCounts[1]++;
            else if (bulletTargets->kind == 5 || bulletTargets->kind == 6)
                colorCounts[2]++;
            else if (bulletTargets->kind == 7 || bulletTargets->kind == 8)
                colorCounts[3]++;
            else if (bulletTargets->kind == 9 ||
                     bulletTargets->kind == 10 ||
                     bulletTargets->kind == 11)
                colorCounts[4]++;
            else if (bulletTargets->kind == 12 || bulletTargets->kind == 13)
                colorCounts[5]++;
            else if (bulletTargets->kind == 14)
                colorCounts[6]++;
        }
    }

    for (i32 color = 0; color < 7; color++)
    {
        if (colorCounts[color] >= 100)
        {
            scoreData[7] |= PHOTO_SCORE_COLOR_1 << color;
            colorCounts[8] += 300;
        }
    }

    colorCounts[7] = 0;
    for (i32 color = 0; color < 7; color++)
    {
        if (colorCounts[color] >= 20)
            colorCounts[7]++;
    }
    if (colorCounts[7] >= 3)
    {
        scoreData[7] |= PHOTO_SCORE_COLORFUL;
        colorCounts[8] += 900;
    }

    colorCounts[7] = 0;
    for (i32 color = 0; color < 7; color++)
    {
        if (colorCounts[color] > 0)
            colorCounts[7]++;
    }
    if (colorCounts[7] >= 7)
    {
        scoreData[7] |= PHOTO_SCORE_RAINBOW;
        colorCounts[8] += 2100;
    }

    if ((scoreData[7] & PHOTO_SCORE_ENEMY) != 0 &&
        (scoreData[7] & PHOTO_SCORE_SELF) != 0)
    {
        scoreData[7] |= PHOTO_SCORE_TWO_SHOT;
        colorCounts[8] = (i32)(
            (f32)colorCounts[8] * closestMultiplier * 1.5f * 1.2f);
    }
    else if ((scoreData[7] & PHOTO_SCORE_ENEMY) != 0)
    {
        colorCounts[8] = (i32)((f32)colorCounts[8] * closestMultiplier);
    }
    else if ((scoreData[7] & PHOTO_SCORE_SELF) != 0)
    {
        colorCounts[8] = (i32)((f32)colorCounts[8] * 1.2f);
    }
    if ((scoreData[7] & PHOTO_SCORE_BOSS_RATE) != 0)
    {
        colorCounts[8] = (i32)((f32)colorCounts[8] * bossMultiplier);
    }
    colorCounts[8] =
        (i32)((f32)colorCounts[8] * g_PhotoStageState->scoreMultiplier);
    scoreData[0] = colorCounts[8] - colorCounts[8] % 10;
    return 0;
}

i32 PhotoCameraState::CountPhotoTargets(f32 *closestDistance, f32 *bossRate)
{
    struct PhotoTargetLocals
    {
        u32 enemyIndex;
        f32 nearestTarget;
        i32 targetCount;
        volatile f32 currentValue;
        f32 highestBossRate;
    } locals;

    locals.targetCount = 0;
    locals.nearestTarget = 998001.0f;
    locals.highestBossRate = 0.0f;
    for (locals.enemyIndex = 0; locals.enemyIndex < 8;
         locals.enemyIndex++)
    {
        if (g_PhotoRuntime->enemies[locals.enemyIndex] == NULL)
        {
            continue;
        }
        if (((g_PhotoRuntime->enemies[locals.enemyIndex]->flags >> 4) & 1) != 0 ||
            ((g_PhotoRuntime->enemies[locals.enemyIndex]->flags >> 5) & 1) != 0 ||
            ((g_PhotoRuntime->enemies[locals.enemyIndex]->flags2 >> 6) & 1) != 0)
        {
            continue;
        }

        if (!PhotoEnemyIsOffscreen(
                &g_PhotoRuntime->enemies[locals.enemyIndex]->position))
        {
            if (PhotoRectangleContains(
                    &g_PhotoRuntime->enemies[locals.enemyIndex]->position,
                    8.0f, 8.0f, &this->viewfinderPosition,
                    this->viewfinderSize.x, this->viewfinderSize.y))
            {
                locals.currentValue = PhotoDistance2D(
                    &g_PhotoRuntime->enemies[locals.enemyIndex]->position,
                    &this->viewfinderPosition);
                if (locals.currentValue < locals.nearestTarget)
                {
                    locals.nearestTarget = locals.currentValue;
                }
                if (g_PhotoRuntime->enemies[locals.enemyIndex]
                            ->HasPhotoRate() &&
                    (locals.currentValue = PhotoRatio(
                         g_PhotoRuntime->enemies[locals.enemyIndex]
                             ->photoRateDenominator,
                         g_PhotoRuntime->enemies[locals.enemyIndex]
                             ->photoRateNumerator),
                     locals.currentValue > locals.highestBossRate))
                {
                    locals.highestBossRate = locals.currentValue;
                }
                locals.targetCount++;
            }
        }
    }

    if (closestDistance != NULL)
    {
        *closestDistance = locals.nearestTarget;
    }
    if (bossRate != NULL)
    {
        *bossRate = locals.highestBossRate;
    }
    return locals.targetCount;
}

void PhotoCameraState::UpdateCharge()
{
    struct ChargeLocals
    {
        f32 timerValue;
        f32 timerComparison;
        ZunTimer *timer;
        PhotoAnmVmId effect;
    } locals;

    if (((this->flags >> 1) & 1) == 0)
    {
        if (this->charge < 1.0f)
        {
            if (PhotoInputMask(g_PhotoInput, 2) != 0 &&
                PhotoInputMask(g_PhotoInput, 1) != 0)
            {
                this->unknownbb8++;
                if (this->unknownbb8 >= 5)
                {
                    this->flags |= PHOTO_FLAG_FOCUSED;
                    if (((g_PhotoGlobalState->flags >> 9) & 1) == 0)
                    {
                        PhotoSoundPlayer()->PlaySoundByIdx(0x2a, 0);
                    }
                    locals.timer = &this->chargeTimer;
                    locals.timer->current = 0;
                    locals.timer->subFrame = 0.0f;
                    locals.timer->previous = -999999;
                    goto focusedCharge;
                }
            }
            else
            {
                this->unknownbb8 = 0;
            }
        }

        if (PhotoInputMask(g_PhotoInput, 1) == 0)
        {
            this->focusHeldFrames++;
            this->flags &= ~PHOTO_FLAG_CHARGE_EFFECT_ACTIVE;
        }
        else
        {
            this->focusHeldFrames = 0;
        }

normalCharge:
        {
            locals.timerComparison = this->chargeTimer.subFrame;
            this->charge +=
                locals.timerComparison < 60.0f
                    ? ((locals.timerValue = this->chargeTimer.subFrame),
                       ((locals.timerValue * 1.0f / 800.0f) / 60.0f +
                        0.000625f) * g_AnmGameSpeed)
                    : 0.001875f * g_AnmGameSpeed;
            if (this->charge > 1.0f)
            {
                this->charge = 1.0f;
            }
            this->chargeTimer.Tick();
        }
    }
    else
    {
        if (((g_PhotoGlobalState->flags >> 9) & 1) != 0)
        {
            PhotoSoundPlayer()->StopSoundByIdx(0x2a);
        }
        if (this->unknownbb8 > 60 ||
            PhotoTimerAdvancedOnEvenFrame(&this->auxiliaryTimer))
        {
            g_PhotoBulletManager->anmSpawner->SpawnInto(
                &locals.effect, 0x124, &g_PhotoGame->playerPosition);
        }
        this->unknownbb8++;
        this->flags |= PHOTO_FLAG_CHARGE_EFFECT_ACTIVE;
        this->focusHeldFrames = 0;
        if (PhotoInputMask(g_PhotoInput, 2) == 0 ||
            PhotoInputMask(g_PhotoInput, 1) == 0)
        {
            this->flags &= ~PHOTO_FLAG_FOCUSED;
            this->unknownbb8 = 0;
            PhotoSoundPlayer()->StopSoundByIdx(0x2a);
            goto normalCharge;
        }

focusedCharge:
        {
            this->charge +=
                this->unknownbb8 < 70
                    ? (((f32)this->unknownbb8 * 40.0f / 800.0f) / 30.0f +
                       0.00125f) * g_AnmGameSpeed
                    : 0.005f * g_AnmGameSpeed;
            if (this->charge > 1.0f)
            {
                this->charge = 1.0f;
                this->flags &= ~PHOTO_FLAG_FOCUSED;
                this->unknownbb8 = 0;
                PhotoSoundPlayer()->StopSoundByIdx(0x2a);
                goto normalCharge;
            }
            return;
        }
    }
}

f32 __fastcall PhotoDistance2D(const Float3 *left, const Float3 *right)
{
    return sqrtf(
        (left->x - right->x) * (left->x - right->x) +
        (left->y - right->y) * (left->y - right->y));
}

void __fastcall UpdatePhotoCamera(PhotoCameraState *camera)
{
    PhotoEnemyView *boss = g_PhotoRuntime->enemies[0];
    Float3 screenPosition;
    Float3 effectPosition;
    AnmVm *frameVm;
    i32 chargeDisplay;
    i32 angleSector;
    f32 targetAngle;

    switch (camera->mode)
    {
    case PHOTO_CAMERA_TRACKING:
        if ((camera->flags & PHOTO_FLAG_FOCUSED) == 0)
        {
            if (boss == NULL)
            {
                camera->cameraOffset = g_PhotoGame->playerPosition;
                camera->cameraOffset.y -= 64.0f;
            }
            else
            {
                if (g_PhotoGame->cameraTrackingMode == 2)
                {
                    camera->trackingRadius = 56.0f;
                }
                else if (g_PhotoGame->cameraTrackingMode == 1)
                {
                    f32 playerDistance = PhotoDistance2D(
                        &g_PhotoGame->playerPosition, &camera->viewfinderPosition);
                    f32 bossDistance = PhotoDistance2D(
                        &boss->position, &g_PhotoGame->playerPosition);
                    if (playerDistance < 56.0f)
                    {
                        camera->trackingRadius = 56.0f;
                    }
                    else if (playerDistance < bossDistance)
                    {
                        camera->trackingRadius = playerDistance + 2.0f;
                    }
                    else if (bossDistance < playerDistance)
                    {
                        camera->trackingRadius = playerDistance - 2.0f;
                    }
                    if (camera->trackingRadius >= 88.0f)
                    {
                        camera->trackingRadius = 88.0f;
                    }
                }
                else if (camera->trackingRadius >= 48.0f)
                {
                    camera->trackingRadius -= 1.0f;
                }
                else if (camera->trackingRadius >= 44.0f)
                {
                    camera->trackingRadius += 1.0f;
                }

                if (g_PhotoGame->cameraTrackingMode == 0)
                {
                    Float3 playerDelta;
                    playerDelta.x = g_PhotoGame->playerPosition.x - camera->previousTrackingOrigin.x;
                    playerDelta.y = g_PhotoGame->playerPosition.y - camera->previousTrackingOrigin.y;
                    playerDelta.z = g_PhotoGame->playerPosition.z - camera->previousTrackingOrigin.z;
                    if (playerDelta.x * playerDelta.x + playerDelta.y * playerDelta.y >= 0.1f)
                    {
                        targetAngle = atan2f(playerDelta.y, playerDelta.x);
                    }
                    else
                    {
                        targetAngle = g_PhotoGame->AngleToPoint(&boss->position);
                    }
                    targetAngle = AddNormalizeAngle(targetAngle - camera->trackingAngle, 0.0f);
                    camera->trackingAngle += targetAngle * 0.04f;
                    camera->cameraOffset.FromAngleMagnitude(
                        camera->trackingAngle, camera->trackingRadius);
                }
                else
                {
                    camera->cameraOffset.x = boss->position.x - g_PhotoGame->playerPosition.x;
                    camera->cameraOffset.y = boss->position.y - g_PhotoGame->playerPosition.y;
                    camera->cameraOffset.z = boss->position.z - g_PhotoGame->playerPosition.z;
                    D3DXVec3Normalize(
                        reinterpret_cast<D3DXVECTOR3 *>(&camera->cameraOffset),
                        reinterpret_cast<D3DXVECTOR3 *>(&camera->cameraOffset));
                    camera->cameraOffset.x *= camera->trackingRadius;
                    camera->cameraOffset.y *= camera->trackingRadius;
                    camera->cameraOffset.z *= camera->trackingRadius;
                }
                camera->cameraOffset += g_PhotoGame->playerPosition;
                camera->previousTrackingOrigin = g_PhotoGame->playerPosition;
            }

            if (g_PhotoGame->cameraTrackingMode == 0)
            {
                Float3 delta;
                delta.x = camera->cameraOffset.x - camera->viewfinderPosition.x;
                delta.y = camera->cameraOffset.y - camera->viewfinderPosition.y;
                delta.z = camera->cameraOffset.z - camera->viewfinderPosition.z;
                camera->viewfinderPosition.x += delta.x * 0.4f;
                camera->viewfinderPosition.y += delta.y * 0.4f;
                camera->viewfinderPosition.z += delta.z * 0.4f;
            }
            else
            {
                Float3 delta;
                delta.x = camera->cameraOffset.x - camera->viewfinderPosition.x;
                delta.y = camera->cameraOffset.y - camera->viewfinderPosition.y;
                delta.z = camera->cameraOffset.z - camera->viewfinderPosition.z;
                camera->viewfinderPosition.x += delta.x * 0.4f;
                camera->viewfinderPosition.y += delta.y * 0.4f;
                camera->viewfinderPosition.z += delta.z * 0.4f;
                camera->trackingAngle = atan2f(
                    camera->viewfinderPosition.y - g_PhotoGame->playerPosition.y,
                    camera->viewfinderPosition.x - g_PhotoGame->playerPosition.x);
            }
        }
        else
        {
            camera->cameraOffset = g_PhotoGame->playerPosition;
            Float3 delta;
            delta.x = camera->cameraOffset.x - camera->viewfinderPosition.x;
            delta.y = camera->cameraOffset.y - camera->viewfinderPosition.y;
            delta.z = camera->cameraOffset.z - camera->viewfinderPosition.z;
            camera->viewfinderPosition.x += delta.x * 0.4f;
            camera->viewfinderPosition.y += delta.y * 0.4f;
            camera->viewfinderPosition.z += delta.z * 0.4f;
        }

        if (camera->viewfinderPosition.x <= -176.0f)
            camera->viewfinderPosition.x = -176.0f;
        else if (camera->viewfinderPosition.x > 176.0f)
            camera->viewfinderPosition.x = 176.0f;
        if (camera->viewfinderPosition.y <= 16.0f)
            camera->viewfinderPosition.y = 16.0f;
        else if (camera->viewfinderPosition.y > 432.0f)
            camera->viewfinderPosition.y = 432.0f;
        goto updateCharge;

    case PHOTO_CAMERA_CHARGING:
        camera->UpdateViewfinder();
        if ((g_PhotoInput & 2) == 0)
        {
            camera->TakePhoto();
            break;
        }
        if (camera->modeTimer.current >= 4)
        {
            camera->charge -= 1.0f / 42.0f;
        }
        if (camera->charge <= 0.0f)
        {
            camera->CancelCapture();
            break;
        }
        if (!PhotoSoundsEnabled())
        {
            PhotoSoundPlayer()->StopSoundByIdx(0x2c);
        }
        goto cameraActive;

    case PHOTO_CAMERA_CAPTURED:
        if (camera->modeTimer.current != camera->modeTimer.previous &&
            camera->modeTimer.current == 20)
        {
            if (g_PhotoGame->movementState >= 0 && g_PhotoGame->movementState <= 2)
            {
                g_PhotoGame->effectAnm->InitializeVm(&g_PhotoGame->effectVm, 0);
            }
            if ((camera->flags & PHOTO_FLAG_ALTERNATE_CAPTURE) != 0)
            {
                if (PhotoSoundsEnabled())
                {
                    PhotoSoundPlayer()->PlaySoundPositionedByIdx(
                        0x21, camera->viewfinderPosition.x);
                }
                PhotoToScreen(&effectPosition, &g_PhotoGame->playerPosition);
                PhotoAnmVmId effect = g_PhotoStageState->anm->CreateVm(0x21, 0);
                PhotoAnmManager()->SetVmPosition(effect.value, &effectPosition);
            }
            else
            {
                if (PhotoSoundsEnabled())
                {
                    PhotoSoundPlayer()->PlaySoundPositionedByIdx(
                        0x25, camera->viewfinderPosition.x);
                }
                PhotoToScreen(&effectPosition, &g_PhotoGame->playerPosition);
                PhotoAnmVmId effect = g_PhotoStageState->anm->CreateVm(0x22, 0);
                PhotoAnmManager()->SetVmPosition(effect.value, &effectPosition);
            }
        }
        if (camera->modeTimer.current >= 60)
        {
            camera->mode = PHOTO_CAMERA_RECOVERING;
            camera->modeTimer.Initialize();
            camera->charge = 0.0f;
        }
        break;

    case PHOTO_CAMERA_RECOVERING:
        if (camera->modeTimer.current >= 20)
        {
            camera->mode = PHOTO_CAMERA_TRACKING;
        }
        goto updateCharge;

    case PHOTO_CAMERA_DISABLED:
        camera->charge = 0.0f;
        if (camera->vmIds[0].value != 0)
        {
            PhotoAnmManager()->SetVmInterrupt(camera->vmIds[0].value, 1);
            PhotoAnmManager()->SetVmInterrupt(camera->vmIds[1].value, 1);
            camera->vmIds[0].value = 0;
            camera->vmIds[1].value = 0;
        }
        break;
    }
    goto finish;

updateCharge:
    camera->UpdateCharge();
    if (camera->auxiliaryTimer.current >= 60)
    {
        if (camera->charge < 1.0f)
        {
            if ((camera->flags & PHOTO_FLAG_CHARGE_UI_MASK) != 0)
            {
                if (camera->vmIds[9].value != 0)
                {
                    PhotoAnmManager()->RemoveVm(camera->vmIds[9].value);
                    camera->vmIds[9].value = 0;
                }
                if (camera->vmIds[10].value == 0)
                {
                    camera->vmIds[10] = g_PhotoStageState->anm->CreateVm(0x20, 0);
                }
                PhotoAnmManager()->SetVmInterrupt(camera->vmIds[0].value, 3);
                PhotoAnmManager()->SetVmInterrupt(camera->vmIds[1].value, 3);
                camera->flags &= ~PHOTO_FLAG_CHARGE_UI_MASK;
                for (i32 i = 0; i < 4; ++i)
                {
                    camera->viewfinderVms[i].pendingInterrupt = 3;
                }
                frameVm = camera->vmIds[0].GetVm();
                SetPhotoVmColor(frameVm, 0xff, 0xff, 0xff);
            }
        }
        else
        {
            if ((camera->flags & PHOTO_FLAG_CHARGE_UI_MASK) != (1 << 3))
            {
                if (PhotoSoundsEnabled())
                {
                    PhotoSoundPlayer()->PlaySoundByIdx(0x2b, 0);
                }
                if (camera->vmIds[10].value != 0)
                {
                    PhotoAnmManager()->RemoveVm(camera->vmIds[10].value);
                    camera->vmIds[10].value = 0;
                }
                if (camera->vmIds[9].value == 0)
                {
                    camera->vmIds[9] = g_PhotoStageState->anm->CreateVm(0x1f, 0);
                }
                camera->vmIds[0].SetInterrupt(2);
                camera->vmIds[1].SetInterrupt(2);
                camera->flags = (camera->flags & ~PHOTO_FLAG_CHARGE_UI_MASK) | (1 << 3);
                for (i32 i = 0; i < 4; ++i)
                {
                    camera->viewfinderVms[i].pendingInterrupt = 2;
                }
            }
            camera->viewfinderSize.x = 256.0f;
            camera->viewfinderSize.y = 192.0f;
            camera->viewfinderSize.z = 0.0f;
            frameVm = camera->vmIds[0].GetVm();
            if (camera->CountPhotoTargets(NULL, NULL) == 0)
                SetPhotoVmColor(frameVm, 0xff, 0xff, 0xff);
            else
                SetPhotoVmColor(frameVm, 0x20, 0x20, 0xff);
        }
    }

    if ((g_PhotoInputPressed & 2) != 0)
        camera->captureRequested = 1;
    else if ((g_PhotoInput & 2) == 0)
        camera->captureRequested = 0;
    else if ((g_PhotoInput & 3) == 3)
        camera->captureRequested = 1;

    if ((camera->flags & PHOTO_FLAG_FOCUSED) == 0 &&
        camera->charge >= 1.0f &&
        (g_PhotoInput & 2) != 0 &&
        (g_PhotoInput & 1) == 0 &&
        camera->captureRequested != 0 &&
        (camera->focusHeldFrames >= 5 || (g_PhotoInputPressed & 2) != 0))
    {
        camera->BeginCapture();
        camera->flags |= PHOTO_FLAG_TARGET_FRAME_ACTIVE;

cameraActive:
        targetAngle = g_PhotoGame->AngleToPoint(&camera->viewfinderPosition) + 0.3926991f;
        if (targetAngle < 0.0f)
            targetAngle += 6.2831855f;
        angleSector = (i32)(targetAngle / 0.7853982f);
        g_PhotoGame->effectVm.anmFile =
            reinterpret_cast<AnmLoaded *>(g_PhotoGame->effectAnm);
        g_PhotoGame->effectVm.scriptIndex = 5;
        g_PhotoGame->effectAnm->SetAndExecuteScript(
            &g_PhotoGame->effectVm, g_PhotoGame->effectAnm->scripts[5]);
        g_PhotoGame->effectAnm->SetSprite(&g_PhotoGame->effectVm, angleSector + 0x18);

        if (camera->CountPhotoTargets(NULL, NULL) == 0)
        {
            camera->flags &= ~PHOTO_FLAG_TARGET_SOUND_PLAYED;
            if ((camera->flags & PHOTO_FLAG_TARGET_FRAME_ACTIVE) != 0)
            {
                frameVm = camera->vmIds[0].GetVm();
                SetPhotoVmColor(frameVm, 0xff, 0xff, 0xff);
                for (i32 i = 2; i <= 6; ++i)
                    PhotoAnmManager()->SetVmInterrupt(camera->vmIds[i].value, 3);
                camera->flags &= ~PHOTO_FLAG_TARGET_FRAME_ACTIVE;
            }
        }
        else
        {
            if ((camera->flags & PHOTO_FLAG_TARGET_SOUND_PLAYED) == 0)
            {
                if (PhotoSoundsEnabled())
                    PhotoSoundPlayer()->PlaySoundByIdx(0x2e, 0);
                camera->flags |= PHOTO_FLAG_TARGET_SOUND_PLAYED;
            }
            if ((camera->flags & PHOTO_FLAG_TARGET_FRAME_ACTIVE) == 0)
            {
                frameVm = camera->vmIds[0].GetVm();
                SetPhotoVmColor(frameVm, 0x20, 0x20, 0xff);
                for (i32 i = 2; i <= 6; ++i)
                    PhotoAnmManager()->SetVmInterrupt(camera->vmIds[i].value, 2);
                camera->flags |= PHOTO_FLAG_TARGET_FRAME_ACTIVE;
            }
        }

        g_PhotoBulletManager->BeginPhotoCapture(
            &camera->viewfinderPosition, &camera->viewfinderSize);
        if (camera->charge >= 0.35f)
        {
            g_AnmGameSpeed = 0.25f;
            g_PhotoBulletManager->photoColor.color = 0x60404040;
        }
        else
        {
            f32 slowRate = (0.35f - camera->charge) / 0.35f;
            g_AnmGameSpeed = slowRate * 0.75f + 0.25f;
            g_PhotoBulletManager->photoColor.a = (u8)(slowRate * 32.0f) + 0x60;
            g_PhotoBulletManager->photoColor.r = (u8)(slowRate * 64.0f) + 0x40;
            g_PhotoBulletManager->photoColor.g = (u8)(slowRate * 64.0f) + 0x40;
            g_PhotoBulletManager->photoColor.b = (u8)(slowRate * 64.0f) + 0x40;
        }
    }

finish:
    PhotoToScreen(&screenPosition, &camera->viewfinderPosition);
    PhotoAnmManager()->SetVmPosition(camera->vmIds[0].value, &screenPosition);
    PhotoAnmManager()->SetVmPosition(camera->vmIds[1].value, &screenPosition);
    PhotoAnmManager()->SetVmPosition(camera->vmIds[9].value, &screenPosition);
    PhotoAnmManager()->SetVmPosition(camera->vmIds[10].value, &screenPosition);
    if (((g_PhotoStageState->flags >> 2) & 1) != 0)
    {
        PhotoAnmManager()->SetVmInterrupt(camera->vmIds[0].value, 5);
        PhotoAnmManager()->SetVmInterrupt(camera->vmIds[1].value, 5);
        PhotoAnmManager()->SetVmInterrupt(camera->vmIds[9].value, 5);
        PhotoAnmManager()->SetVmInterrupt(camera->vmIds[10].value, 5);
    }
    for (i32 i = 0; i < 4; ++i)
    {
        camera->viewfinderVms[i].positionOffset = screenPosition;
    }

    chargeDisplay = (i32)(camera->charge * 100.0f);
    if (chargeDisplay / 100 == 0)
    {
        camera->viewfinderVms[0].flagsWord &= ~2U;
    }
    else
    {
        g_PhotoStageState->anm->SetSprite(
            &camera->viewfinderVms[0], chargeDisplay / 100 + 0xf);
        camera->viewfinderVms[0].flagsWord |= 2;
    }
    if (chargeDisplay / 10 == 0)
    {
        camera->viewfinderVms[1].flagsWord &= ~2U;
    }
    else
    {
        g_PhotoStageState->anm->SetSprite(
            &camera->viewfinderVms[1], chargeDisplay / 10 % 10 + 0xf);
        camera->viewfinderVms[1].flagsWord |= 2;
    }
    g_PhotoStageState->anm->SetSprite(
        &camera->viewfinderVms[2], chargeDisplay % 10 + 0xf);
    camera->viewfinderVms[2].flagsWord |= 2;
    camera->viewfinderVms[3].flagsWord |= 2;

    AnmManager::ExecuteScript(&camera->viewfinderVms[0]);
    AnmManager::ExecuteScript(&camera->viewfinderVms[1]);
    AnmManager::ExecuteScript(&camera->viewfinderVms[2]);
    AnmManager::ExecuteScript(&camera->viewfinderVms[3]);
    camera->modeTimer.Tick();
    camera->auxiliaryTimer.Tick();
}

} // namespace th095
