#include "AnmManager.hpp"
#include "AnmVmId.hpp"
#include "PhotoGamePlayer.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoGameTask.hpp"
#include "PhotoEnemyManager.hpp"
#include "PhotoEffectRuntime.hpp"
#include "PhotoRotatingLaser.hpp"
#include "PhotoStraightLaser.hpp"
#include "PhotoItemManager.hpp"

namespace th095
{

#ifdef DIFFBUILD
#define TH095_EFFECT_DRAW_MODE_2D 1
#else
#define TH095_EFFECT_DRAW_MODE_2D ANM_VM_DRAW_MODE_2D
#endif

extern f32 g_AnmGameSpeed;


extern AnmManager *g_AnmManager;
// Target 0x004A43D8: color selected by each of the 16 photo-effect types.
u32 g_PhotoEffectColors[16] = {
    0xff808080, 0xffff0000, 0xffff0000, 0xff800080,
    0xff800080, 0xff0000ff, 0xff0000ff, 0xff008080,
    0xff008080, 0xff00ff00, 0xff00ff00, 0xff00ff00,
    0xff808000, 0xff808000, 0xff808000, 0xff808080};


static inline i32 PhotoEffectEitherFlag(i32 first, i32 second)
{
    return first | second;
}

extern i32 __fastcall GetPhotoBulletScriptBase(i32 type);
Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

static inline i32 PhotoEffectIsOutsidePlayfield(
    PhotoEffectVector *position, f32 width, f32 height)
{
    return width + position->x <= -192.0f ||
           position->x - width >= 192.0f ||
           height + position->y <= 0.0f ||
           position->y - height >= 448.0f;
}

static inline i32 PhotoEffectEndpointIsOutsidePlayfield(
    PhotoEffectVector *position, f32 width, f32 height)
{
    return position->x + width <= -192.0f ||
           position->x - width >= 192.0f ||
           position->y + height <= 0.0f ||
           position->y - height >= 448.0f;
}

i32 PhotoStraightLaserView::Update()
{
    if (this->length < this->spawn.maximumLength)
    {
        this->length += g_AnmGameSpeed * this->speed;
        if (this->length > this->spawn.maximumLength)
        {
            this->length = this->spawn.maximumLength;
        }
    }
    else
    {
        this->tailOffset += g_AnmGameSpeed * this->speed;
        this->position += this->velocity * g_AnmGameSpeed;
        if (this->spawn.terminalDistance > 0.0f &&
            this->spawn.terminalDistance < this->tailOffset + this->length)
        {
            this->length = this->spawn.terminalDistance - this->tailOffset;
            this->spawn.maximumLength = this->length;
            if (this->length <= 0.0f)
            {
                return 1;
            }
        }
    }

    Float3 endpoint;
    endpoint.FromAngleMagnitude(this->angle, this->length);
    endpoint += *reinterpret_cast<Float3 *>(&this->position);

    if (PhotoEffectIsOutsidePlayfield(
            &this->position, this->width, this->width))
    {
        if (PhotoEffectEndpointIsOutsidePlayfield(
                reinterpret_cast<PhotoEffectVector *>(&endpoint),
                this->width,
                this->width))
        {
            return 1;
        }
    }

    if (this->length > 16.0f && this->width > 3.0f)
    {
        Float3 collisionOrigin;
        collisionOrigin.FromAngleMagnitude(this->angle, 8.0f);
        collisionOrigin += *reinterpret_cast<Float3 *>(&this->position);

        if (g_RuntimePlayerOwner->CalcLaserHitbox(
                &collisionOrigin,
                this->angle,
                this->width < 32.0f
                    ? this->width * 0.5f
                    : this->width - 16.0f,
                this->length - 16.0f) != 0)
        {
            Float3 collisionSize(32.0f, 32.0f, 0.0f);
            this->CheckCollision(
                &g_RuntimePlayerOwner->playerPosition,
                &collisionSize,
                0);
        }
    }

    this->bodyVm.scale.x =
        this->width / this->bodyVm.loadedSprite->widthPx;
    this->bodyVm.scale.y =
        this->length / this->bodyVm.loadedSprite->heightPx;
    AnmManager::ExecuteScript(&this->bodyVm);
    if (this->tailOffset == 0.0f)
    {
        AnmManager::ExecuteScript(&this->tailVm);
    }
    return 0;
}

static __forceinline void PhotoEffectSetAdditivePhase(AnmVm *vm)
{
    u8 compilerStorage[0x2c];
    vm->SetBlendModeAdditive();
}

i32 PhotoStraightLaserView::Initialize(void *args)
{
    static_cast<PhotoStraightLaserSpawnArgs &>(this->spawn) =
        *static_cast<const PhotoStraightLaserSpawnArgs *>(args);
    this->state = PHOTO_EFFECT_STATE_ACTIVE;

    g_PhotoEffectManager->anm->InitializeVm(
        &this->bodyVm,
        GetPhotoBulletScriptBase(this->spawn.type) + this->spawn.color);
    this->bodyVm.pendingInterrupt = 2;
    AnmManager::ExecuteScript(&this->bodyVm);
    PhotoEffectSetAdditivePhase(&this->bodyVm);
    this->bodyVm.renderModeBits = TH095_EFFECT_DRAW_MODE_2D;
    this->bodyVm.alignmentX = ANM_ALIGNMENT_CENTER;
    this->bodyVm.alignmentY = ANM_ALIGNMENT_END;

    g_PhotoEffectManager->anm->InitializeVm(
        &this->tailVm, this->spawn.color + 0xc2);
    this->tailVm.pendingInterrupt = 2;
    AnmManager::ExecuteScript(&this->tailVm);
    PhotoEffectSetAdditivePhase(&this->tailVm);
    this->tailVm.renderModeBits = TH095_EFFECT_DRAW_MODE_2D;

    memcpy(&this->position, &this->spawn.position, sizeof(this->position));
    this->length = this->spawn.initialLength;
    this->width = this->spawn.width;
    this->speed = this->spawn.speed;
    this->angle = this->spawn.angle;
    if (this->length > 0.0f)
    {
        this->tailOffset = 0.01f;
    }
    else
    {
        this->tailOffset = 0.0f;
    }
    reinterpret_cast<Float3 *>(&this->velocity)
        ->FromAngleMagnitude(this->angle, this->speed);
    return 0;
}

i32 PhotoStraightLaserView::Draw()
{
    PhotoToScreen(
        &this->bodyVm.position,
        reinterpret_cast<const Float3 *>(&this->position));
    this->bodyVm.rotation.z =
        AddNormalizeAngle(this->angle, 1.5707964f);
    this->bodyVm.Draw();
    if (this->tailOffset == 0.0f)
    {
        PhotoToScreen(
            &this->tailVm.position,
            reinterpret_cast<const Float3 *>(&this->position));
        this->tailVm.Draw();
    }
    return 0;
}

i32 PhotoRotatingLaserView::Update()
{
    if (this->length < this->spawn.maximumLength)
    {
        this->length += g_AnmGameSpeed * this->speed;
        if (this->length > this->spawn.maximumLength)
        {
            this->length = this->spawn.maximumLength;
        }
    }

    this->angle = AddNormalizeAngle(
        this->angle, g_AnmGameSpeed * this->spawn.angularVelocity);

    if (this->spawn.followPhotoTarget != 0 &&
        g_PhotoEnemyManager->photoTargets[0] != NULL)
    {
        memcpy(&this->position,
               &g_PhotoEnemyManager->photoTargets[0]->position,
               sizeof(this->position));
    }

    *reinterpret_cast<Float3 *>(&this->position) +=
        this->spawn.velocity * g_AnmGameSpeed;

    switch (this->state)
    {
    case PHOTO_EFFECT_STATE_STARTUP:
        if (this->timer >= this->spawn.startupDuration)
        {
            this->timer = 0;
            this->state = PHOTO_EFFECT_STATE_GROWING;
        }
        break;

    case PHOTO_EFFECT_STATE_GROWING:
        if (this->timer >= this->spawn.growthDuration)
        {
            this->timer = 0;
            this->state = PHOTO_EFFECT_STATE_ACTIVE;
            this->width = this->spawn.maximumWidth;
        }
        else
        {
            this->width =
                static_cast<f32>(this->timer) *
                this->spawn.maximumWidth /
                this->spawn.growthDuration;
            break;
        }

    case PHOTO_EFFECT_STATE_ACTIVE:
        if (this->timer >= this->spawn.sustainDuration)
        {
            this->timer = 0;
            this->state = PHOTO_EFFECT_STATE_FADING;
        }
        else
        {
            break;
        }

    case PHOTO_EFFECT_STATE_FADING:
        if (this->timer >= this->spawn.fadeDuration)
        {
            return 1;
        }
        this->width =
            this->spawn.maximumWidth -
            static_cast<f32>(this->timer) *
                this->spawn.maximumWidth /
                this->spawn.fadeDuration;
        break;
    }

    if ((this->state == PHOTO_EFFECT_STATE_GROWING ||
         this->state == PHOTO_EFFECT_STATE_ACTIVE) &&
        this->length > 16.0f)
    {
        Float3 collisionOrigin;
        collisionOrigin.FromAngleMagnitude(this->angle, 8.0f);
        collisionOrigin += *reinterpret_cast<Float3 *>(&this->position);

        if (g_RuntimePlayerOwner->CalcLaserHitbox(
                &collisionOrigin,
                this->angle,
                this->width < 32.0f
                    ? this->width * 0.5f
                    : this->width - 16.0f,
                this->length - 16.0f) != 0)
        {
            Float3 collisionSize(32.0f, 32.0f, 0.0f);
            this->CheckCollision(
                &g_RuntimePlayerOwner->playerPosition,
                &collisionSize,
                0);
        }
    }

    this->bodyVm.scale.x =
        this->width / this->bodyVm.loadedSprite->widthPx;
    this->bodyVm.scale.y =
        this->length / this->bodyVm.loadedSprite->heightPx;
    AnmManager::ExecuteScript(&this->bodyVm);
    if (this->tailOffset == 0.0f)
    {
        AnmManager::ExecuteScript(&this->tailVm);
    }
    return 0;
}

i32 PhotoRotatingLaserView::Initialize(void *args)
{
    static_cast<PhotoRotatingLaserSpawnArgs &>(this->spawn) =
        *static_cast<const PhotoRotatingLaserSpawnArgs *>(args);
    this->state = PHOTO_EFFECT_STATE_STARTUP;

    g_PhotoEffectManager->anm->InitializeVm(
        &this->bodyVm,
        GetPhotoBulletScriptBase(this->spawn.type) + this->spawn.color);
    this->bodyVm.pendingInterrupt = 2;
    AnmManager::ExecuteScript(&this->bodyVm);
    PhotoEffectSetAdditivePhase(&this->bodyVm);
    this->bodyVm.renderModeBits = TH095_EFFECT_DRAW_MODE_2D;
    this->bodyVm.alignmentX = ANM_ALIGNMENT_CENTER;
    this->bodyVm.alignmentY = ANM_ALIGNMENT_END;

    g_PhotoEffectManager->anm->InitializeVm(
        &this->tailVm, this->spawn.color + 0xc2);
    this->tailVm.pendingInterrupt = 2;
    AnmManager::ExecuteScript(&this->tailVm);
    PhotoEffectSetAdditivePhase(&this->tailVm);
    this->tailVm.renderModeBits = TH095_EFFECT_DRAW_MODE_2D;

    memcpy(&this->position, &this->spawn.position, sizeof(this->position));
    this->length = this->spawn.initialLength;
    this->width = 2.0f;
    this->speed = this->spawn.speed;
    this->angle = this->spawn.angle;
    return 0;
}

i32 PhotoRotatingLaserView::Draw()
{
    PhotoToScreen(
        &this->bodyVm.position,
        reinterpret_cast<const Float3 *>(&this->position));
    this->bodyVm.rotation.z =
        AddNormalizeAngle(this->angle, 1.5707964f);
    this->bodyVm.Draw();
    if (this->tailOffset == 0.0f)
    {
        PhotoToScreen(
            &this->tailVm.position,
            reinterpret_cast<const Float3 *>(&this->position));
        this->tailVm.Draw();
    }
    return 0;
}

// The straight and rotating secondary-draw paths share the same target-proven
// VC7.1 local hash order. Hoisting the real VM pointer keeps all six live
// locals in one allocation lane; no scratch or padding locals are used.
#define secondaryStep restartCommandProcessingLocal05
#define secondaryCount averagedPanLocal12
#define secondaryPosition iLocal11
#define secondaryDistance commandCursorLocal02
#define secondaryVm soundIndexLocal01
#define secondaryStartPosition jLocal00
i32 PhotoStraightLaserView::DrawSecondary()
{
    i32 secondaryCount = 0;
    f32 secondaryDistance = 6.0f;
    AnmVm *secondaryVm;
    Float3 secondaryStep;
    secondaryStep.FromAngleMagnitude(this->angle, 6.0f);
    Float3 secondaryStartPosition =
        *reinterpret_cast<Float3 *>(&this->position) + secondaryStep;
    Float3 secondaryPosition = secondaryStartPosition;
    secondaryStep += secondaryStep;

    while (secondaryDistance + 6.0f < this->length)
    {
        secondaryCount++;
        secondaryVm = g_AnmManager->GetVm(
            g_PhotoEffectManager->anm->CreateVmAtWorld(
                0x126, &secondaryPosition));
        secondaryVm->color1.color = g_PhotoEffectColors[this->spawn.color];
        secondaryPosition += secondaryStep;
        secondaryDistance += 12.0f;
    }
    this->state = PHOTO_EFFECT_STATE_RETIRED;
    return secondaryCount;
}

i32 PhotoRotatingLaserView::DrawSecondary()
{
    i32 secondaryCount = 0;
    f32 secondaryDistance = 6.0f;
    AnmVm *secondaryVm;
    Float3 secondaryStep;
    secondaryStep.FromAngleMagnitude(this->angle, 6.0f);
    Float3 secondaryStartPosition =
        *reinterpret_cast<Float3 *>(&this->position) + secondaryStep;
    Float3 secondaryPosition = secondaryStartPosition;
    secondaryStep += secondaryStep;

    while (secondaryDistance + 6.0f < this->length)
    {
        secondaryCount++;
        secondaryVm = g_AnmManager->GetVm(
            g_PhotoEffectManager->anm->CreateVmAtWorld(
                0x126, &secondaryPosition));
        secondaryVm->color1.color = g_PhotoEffectColors[this->spawn.color];
        secondaryPosition += secondaryStep;
        secondaryDistance += 12.0f;
    }
    this->state = PHOTO_EFFECT_STATE_RETIRED;
    return secondaryCount;
}
#undef secondaryStep
#undef secondaryCount
#undef secondaryPosition
#undef secondaryDistance
#undef secondaryVm
#undef secondaryStartPosition

static __forceinline Float3 CollisionScaleStep(const Float3 &value, f32 scalar)
{
    return Float3(scalar * value.x, scalar * value.y, scalar * value.z);
}

#define step restartCommandProcessingLocal05
#define sampleCount averagedPanLocal12
#define hitCount iLocal11
#define minimum commandCursorLocal02
#define sample soundIndexLocal01
i32 PhotoStraightLaserView::CheckCollision(
    Float3 *position, Float3 *size, i32 capture)
{
    Float3 step;
    i32 sampleCount;
    i32 hitCount;
    Float3 minimum;
    Float3 sample;
    hitCount = 0;
    sampleCount = 0;
    {
    f32 bufferLocal04;
    Float3 preloadBufferLocal03 =
        *reinterpret_cast<Float3 *>(&this->position);
    bufferLocal04 = 6.0f;
    u8 hits[256];
    memset(hits, 0, sizeof(hits));

    Float3 jLocal00;
    jLocal00 = *size / 2.0f;
    minimum = *position - jLocal00;
    jLocal00 = *position + jLocal00;
    step.z = 0.0f;
    step.FromAngleMagnitude(this->angle, 6.0f);
    sample =
        *reinterpret_cast<Float3 *>(&this->position) + step;
    step += step;

    while (bufferLocal04 + 6.0f < this->length)
    {
        if (sample.x < minimum.x || sample.x > jLocal00.x ||
            sample.y < minimum.y || sample.y > jLocal00.y)
        {
        }
        else
        {
            hits[sampleCount] = 1;
            hitCount++;
            AnmVm *vm = g_AnmManager->GetVm(
                g_PhotoEffectManager->anm->CreateVmAtWorld(
                    0x126, &sample));
            vm->color1.color = g_PhotoEffectColors[this->spawn.color];
            if (capture != 0)
            {
                g_PhotoItemManager->Spawn(
                    0, &sample, vm->color1.color);
            }
        }
        sample += step;
        bufferLocal04 += 12.0f;
        sampleCount++;
    }

    if (hitCount != 0)
    {
        if (hitCount >= sampleCount)
        {
            this->deletionCounter = 1;
            return hitCount;
        }

        struct GapScanState
        {
            i32 gapLength;
            i32 gapStart;
            i32 sampleIndex;
        } gapScan;
#define gapLength gapScan.gapLength
#define gapStart gapScan.gapStart
#define sampleIndex gapScan.sampleIndex
        sampleIndex = 0;
        while (sampleIndex < sampleCount && hits[sampleIndex] != 0)
        {
            sampleIndex++;
        }

        if (sampleIndex != 0)
        {
            *reinterpret_cast<Float3 *>(&this->position) +=
                step * static_cast<f32>(sampleIndex);
            this->length -= static_cast<f32>(sampleIndex) * 12.0f;
            this->spawn.maximumLength = this->length;
            this->tailOffset = static_cast<f32>(sampleIndex) * 12.0f;
        }

        gapLength = 0;
        while (sampleIndex < sampleCount && hits[sampleIndex] == 0)
        {
            sampleIndex++;
            gapLength++;
        }

        if (sampleIndex < sampleCount)
        {
            this->spawn.maximumLength -=
                this->length - static_cast<f32>(gapLength) * 12.0f;
            this->length = static_cast<f32>(gapLength) * 12.0f;

            while (sampleIndex < sampleCount)
            {
                while (sampleIndex < sampleCount &&
                       hits[sampleIndex] != 0)
                {
                    sampleIndex++;
                }
                if (sampleIndex >= sampleCount)
                {
                    goto collisionDone;
                }

                gapLength = 0;
                gapStart = sampleIndex;
                while (sampleIndex < sampleCount &&
                       hits[sampleIndex] == 0)
                {
                    sampleIndex++;
                    gapLength++;
                }

                PhotoStraightLaserSpawnArgs args = this->spawn;
                args.initialLength = static_cast<f32>(gapLength) * 12.0f;
                args.maximumLength = args.initialLength;
                args.position =
                    preloadBufferLocal03 +
                    CollisionScaleStep(step, static_cast<f32>(gapStart));
                g_PhotoEffectManager->Spawn(PHOTO_EFFECT_SPAWN_STRAIGHT_LASER, &args);
            }
        }
    }
collisionDone:
    return hitCount;
#undef sampleIndex
#undef gapStart
#undef gapLength
    }
}
#undef sample
#undef minimum
#undef hitCount
#undef sampleCount
#undef step

#define step restartCommandProcessingLocal05
#define sampleCount averagedPanLocal12
#define hitCount iLocal11
#define minimum commandCursorLocal02
#define sample soundIndexLocal01
i32 PhotoRotatingLaserView::CheckCollision(
    Float3 *position, Float3 *size, i32 capture)
{
    Float3 step;
    i32 sampleCount;
    i32 hitCount;
    Float3 minimum;
    Float3 sample;
    hitCount = 0;
    sampleCount = 0;
    {
    f32 bufferLocal04;
    Float3 preloadBufferLocal03 =
        *reinterpret_cast<Float3 *>(&this->position);
    bufferLocal04 = 6.0f;
    u8 hits[256];
    memset(hits, 0, sizeof(hits));

    Float3 jLocal00;
    jLocal00 = *size / 2.0f;
    minimum = *position - jLocal00;
    jLocal00 = *position + jLocal00;
    step.z = 0.0f;
    step.FromAngleMagnitude(this->angle, 6.0f);
    sample =
        *reinterpret_cast<Float3 *>(&this->position) + step;
    step += step;

    while (bufferLocal04 + 6.0f < this->length)
    {
        if (sample.x < minimum.x || sample.x > jLocal00.x ||
            sample.y < minimum.y || sample.y > jLocal00.y)
        {
        }
        else
        {
            hits[sampleCount] = 1;
            hitCount++;
            AnmVm *vm = g_AnmManager->GetVm(
                g_PhotoEffectManager->anm->CreateVmAtWorld(
                    0x126, &sample));
            vm->color1.color = g_PhotoEffectColors[this->spawn.color];
            if (capture != 0)
            {
                g_PhotoItemManager->Spawn(
                    0, &sample, vm->color1.color);
            }
        }
        sample += step;
        bufferLocal04 += 12.0f;
        sampleCount++;
    }

    if (hitCount != 0)
    {
        struct GapScanState
        {
            i32 gapLength;
            i32 gapStart;
            i32 sampleIndex;
        } gapScan;
#define gapLength gapScan.gapLength
#define gapStart gapScan.gapStart
#define sampleIndex gapScan.sampleIndex
        sampleIndex = 0;
        while (sampleIndex < sampleCount && hits[sampleIndex] != 0)
        {
            sampleIndex++;
        }

        if (sampleIndex != 0)
        {
            this->length = 0.0f;
            gapLength = 0;
            goto scan_more;
        }
        else
        {
            gapLength = 0;
            while (sampleIndex < sampleCount && hits[sampleIndex] == 0)
            {
                sampleIndex++;
                gapLength++;
            }
            if (sampleIndex < sampleCount)
            {
                this->length = static_cast<f32>(gapLength) * 12.0f;
scan_more:
                while (sampleIndex < sampleCount)
                {
                    while (sampleIndex < sampleCount && hits[sampleIndex] != 0)
                    {
                        sampleIndex++;
                    }
                    if (sampleIndex >= sampleCount)
                    {
                        goto finish;
                    }

                    gapLength = 0;
                    gapStart = sampleIndex;
                    while (sampleIndex < sampleCount && hits[sampleIndex] == 0)
                    {
                        sampleIndex++;
                        gapLength++;
                    }

                    PhotoStraightLaserSpawnArgs args;
                    memset(&args, 0, sizeof(args));
                    args.initialLength = static_cast<f32>(gapLength) * 12.0f;
                    args.maximumLength = args.initialLength;
                    args.position =
                        preloadBufferLocal03 + CollisionScaleStep(
                            step, static_cast<f32>(gapStart));
                    args.speed = 8.0f;
                    args.angle = this->angle;
                    args.width = this->width;
                    args.type = this->spawn.type;
                    args.color = this->spawn.color;
                    args.terminalDistance =
                        this->spawn.maximumLength -
                        static_cast<f32>(gapStart) * 12.0f;
                    g_PhotoEffectManager->Spawn(PHOTO_EFFECT_SPAWN_STRAIGHT_LASER, &args);
                }
            }
        }
    }
finish:
    return hitCount;
#undef sampleIndex
#undef gapStart
#undef gapLength
    }
}
#undef sample
#undef minimum
#undef hitCount
#undef sampleCount
#undef step

i32 PhotoEffectBaseView::Initialize(void *args)
{
    return 0;
}

i32 PhotoEffectBaseView::Update()
{
    return 0;
}

i32 PhotoEffectBaseView::Draw()
{
    return 0;
}

i32 PhotoEffectBaseView::Cleanup()
{
    this->previous->next = this->next;
    if (this->next != NULL)
    {
        this->next->previous = this->previous;
    }
    return 0;
}

i32 PhotoEffectBaseView::DrawSecondary()
{
    return 0;
}

i32 PhotoEffectBaseView::CountPhotoTargets(
    Float3 *position, Float3 *size, i32 capture)
{
    return 0;
}

i32 PhotoEffectBaseView::CheckCollision(
    Float3 *position, Float3 *size, i32 capture)
{
    return 0;
}

i32 PhotoEffectBaseView::CountNearbyTargets(
    Float3 *position, f32 radius)
{
    return 0;
}

PhotoEffectManagerView::PhotoEffectManagerView()
{
    utils::DebugPrint("initialize LaserInf\n");
    memset(this, 0, sizeof(*this));
    g_PhotoEffectManager = this;
}

PhotoEffectManagerView::~PhotoEffectManagerView()
{
    utils::DebugPrint("shutdown LaserInf\n");
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);

    PhotoEffectBaseView *effect = this->listRoot.next;
    while (effect != NULL)
    {
        PhotoEffectBaseView *next = effect->next;
        effect->Cleanup();
        effect->previous->next = effect->next;
        if (effect->next != NULL)
        {
            effect->next->previous = effect->previous;
        }
        delete effect;
        effect = NULL;
        effect = next;
    }
    g_PhotoEffectManager = NULL;
}

i32 PhotoEffectManagerView::Initialize()
{
    this->anm = g_AnmManager->PreloadAnm(6, "bullet.anm");
    if (this->anm == NULL)
    {
        g_GameErrorContext.Log(
            "\x93\x47\x92\x65\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x8c\xa9\x82\xc2"
            "\x82\xa9\x82\xe8\x82\xdc\x82\xb9"
            "\x82\xf1\x81\x42\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x89\xf3\x82\xea"
            "\x82\xc4\x82\xa2\x82\xdc\x82\xb7"
            "\r\n");
        return ZUN_ERROR;
    }
    this->last = &this->listRoot;
    return ZUN_SUCCESS;
}

PhotoEffectManagerView *PhotoEffectManagerView::Create()
{
    struct
    {
        PhotoEffectManagerView *manager;
        ChainElem *elem;
    } locals;

#define manager locals.manager
#define elem locals.elem

    manager = new PhotoEffectManagerView();
    if (manager->Initialize() != ZUN_SUCCESS)
    {
        goto failure;
    }

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoEffectManagerView::OnUpdate));
    elem->arg = manager;
    g_Chain.AddToCalcChain(elem, 0xd);
    manager->calcChain = elem;

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoEffectManagerView::OnDraw));
    elem->arg = manager;
    g_Chain.AddToDrawChain(elem, 0xd);
    manager->drawChain = elem;
    return manager;

failure:
    if (manager != NULL)
    {
        delete manager;
        manager = NULL;
    }
#undef elem
#undef manager
    return NULL;
}

void PhotoEffectManagerView::Destroy()
{
    PhotoEffectManagerView *manager = this;
    if (manager != NULL)
    {
        delete manager;
        manager = NULL;
    }
}

void PhotoEffectManagerView::Remove(PhotoEffectBaseView *effect)
{
    this->effectCount--;
    effect->previous->next = effect->next;
    if (effect->next != NULL)
    {
        effect->next->previous = effect->previous;
    }
    if (this->last == effect)
    {
        this->last = effect->previous;
    }
}

i32 __fastcall PhotoEffectManagerView::Update(
    PhotoEffectManagerView *manager)
{
    PhotoEffectBaseView *effect = manager->listRoot.next;
    while (effect != NULL)
    {
        PhotoEffectBaseView *next = effect->next;

        if (effect->deletionCounter != 0)
        {
            effect->deletionCounter++;
            if (effect->deletionCounter >= 2)
            {
                effect->Cleanup();
                manager->Remove(effect);
                delete effect;
                effect = NULL;
                goto advanceEffect;
            }
        }

        if (effect->state == PHOTO_EFFECT_STATE_RETIRED)
        {
            effect->Cleanup();
            manager->Remove(effect);
            delete effect;
            effect = NULL;
        }
        else if (effect->Update() != 0)
        {
            effect->Cleanup();
            manager->Remove(effect);
            delete effect;
            effect = NULL;
        }
        else
        {
            effect->timer.Tick();
        }

    advanceEffect:
        effect = next;
    }
    return 1;
}

i32 __fastcall PhotoEffectManagerView::Draw(
    PhotoEffectManagerView *manager)
{
    PhotoEffectBaseView *effect = manager->listRoot.next;
    while (effect != NULL)
    {
        PhotoEffectBaseView *next = effect->next;
        if (effect->state != PHOTO_EFFECT_STATE_RETIRED)
        {
            effect->Draw();
        }
        effect = next;
    }
    return 1;
}

i32 __fastcall PhotoEffectManagerView::OnUpdate(
    PhotoEffectManagerView *manager)
{
    if (PhotoEffectEitherFlag(
            g_RuntimeGlobalStateOwner->captureActive,
            g_RuntimeGlobalStateOwner->gameplayLoadActive) != 0)
    {
        return 1;
    }
    if (g_RuntimeGlobalStateOwner->photoTransitionActive != 0)
    {
        return 1;
    }
    if (g_RuntimeGlobalStateOwner->capturedPhotoActive != 0)
    {
        f32 gameSpeed = g_AnmGameSpeed;
        g_AnmGameSpeed = 0.0f;
        i32 result = Update(manager);
        g_AnmGameSpeed = gameSpeed;
        return result;
    }
    return Update(manager);
}

i32 __fastcall PhotoEffectManagerView::OnDraw(
    PhotoEffectManagerView *manager)
{
    if (g_RuntimeGlobalStateOwner->gameplayLoadActive != 0)
    {
        return 1;
    }
    return Draw(manager);
}

static __forceinline void PhotoEffectAppendStraightPhase(
    PhotoEffectManagerView *manager, PhotoEffectBaseView *effect)
{
    u8 compilerStorage[8];
    manager->Append(effect);
}

static __forceinline void PhotoEffectAppendRotatingPhase(
    PhotoEffectManagerView *manager, PhotoEffectBaseView *effect)
{
    u8 compilerStorage[0x0c];
    manager->Append(effect);
}

i32 PhotoEffectManagerView::Spawn(i32 type, void *args)
{
    if (this->effectCount >= 0x100)
    {
        return 0;
    }

    this->nextId++;
    if (this->nextId == 0)
    {
        this->nextId++;
    }

    switch (type)
    {
    case PHOTO_EFFECT_SPAWN_STRAIGHT_LASER:
    {
        PhotoStraightLaserView *effect = new PhotoStraightLaserView;
        effect->id = this->nextId;
        PhotoEffectAppendStraightPhase(this, effect);
        this->effectCount++;
        effect->Initialize(args);
        break;
    }
    case PHOTO_EFFECT_SPAWN_ROTATING_LASER:
    {
        PhotoRotatingLaserView *effect = new PhotoRotatingLaserView;
        effect->id = this->nextId;
        PhotoEffectAppendRotatingPhase(this, effect);
        this->effectCount++;
        effect->Initialize(args);
        break;
    }
    }
    return this->nextId;
}

i32 PhotoEffectManagerView::CountPhotoTargets(
    Float3 *position, Float3 *size)
{
    PhotoEffectBaseView *effect = this->listRoot.next;
    i32 count = 0;
    this->collisionPosition =
        *reinterpret_cast<PhotoEffectVector *>(position);
    this->collisionSize =
        *reinterpret_cast<PhotoEffectVector *>(size);

    while (effect != NULL)
    {
        PhotoEffectBaseView *next = effect->next;
        if (effect->state != PHOTO_EFFECT_STATE_RETIRED)
        {
            count += effect->CountPhotoTargets(position, size, 1);
        }
        effect = next;
    }
    return count;
}

i32 __fastcall PhotoEffectManagerView::CheckCollisionStored(
    PhotoEffectManagerView *manager)
{
    PhotoEffectBaseView *effect = manager->listRoot.next;
    i32 count = 0;
    while (effect != NULL)
    {
        PhotoEffectBaseView *next = effect->next;
        if (effect->state != PHOTO_EFFECT_STATE_RETIRED)
        {
            count += effect->CheckCollision(
                reinterpret_cast<Float3 *>(&manager->collisionPosition),
                reinterpret_cast<Float3 *>(&manager->collisionSize),
                1);
        }
        effect = next;
    }
    return count;
}

i32 __fastcall PhotoEffectManagerView::DrawSecondary(
    PhotoEffectManagerView *manager)
{
    PhotoEffectBaseView *effect = manager->listRoot.next;
    while (effect != NULL)
    {
        PhotoEffectBaseView *next = effect->next;
        if (effect->state != PHOTO_EFFECT_STATE_RETIRED)
        {
            effect->DrawSecondary();
        }
        effect = next;
    }
    return 1;
}

i32 PhotoEffectManagerView::CountNearbyTargets(
    Float3 *position, f32 radius)
{
    PhotoEffectBaseView *effect = this->listRoot.next;
    i32 count = 0;
    while (effect != NULL)
    {
        PhotoEffectBaseView *next = effect->next;
        if (effect->state != PHOTO_EFFECT_STATE_RETIRED)
        {
            count += effect->CountNearbyTargets(position, radius);
        }
        effect = next;
    }
    return count;
}

PhotoStraightLaserView::PhotoStraightLaserView()
{
}

PhotoRotatingLaserView::PhotoRotatingLaserView()
{
}

// Both target vtables use the same two query bodies.
#define TH095_PHOTO_LASER_TYPE PhotoStraightLaserView
#include "PhotoLaserQueries.inl"
#undef TH095_PHOTO_LASER_TYPE

#define TH095_PHOTO_LASER_TYPE PhotoRotatingLaserView
#include "PhotoLaserQueries.inl"
#undef TH095_PHOTO_LASER_TYPE

PhotoEffectManagerView *g_PhotoEffectManager; // Retail 0x004C45E0
}
