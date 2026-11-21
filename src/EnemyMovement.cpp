#include "AnmManager.hpp"
#include "PhotoEnemy.hpp"
#include <math.h>

namespace th095
{
f32 AddNormalizeAngle(f32 a, f32 b);

static __forceinline f32 MovementFrameRateMultiplier()
{
    // The historical exact expression spelled this as g_Supervisor + 0x188,
    // but the resolved target is the independently owned game-speed scalar at
    // 0x004BDED8.  Both exact and normal builds use that canonical owner.
    return g_AnmGameSpeed;
}

void PhotoEnemyView::UpdateMovement()
{
    switch ((this->flags1 >>
                PHOTO_ENEMY_MOVEMENT_MODE_SHIFT) & 3)
    {
    case PHOTO_ENEMY_MOVEMENT_ORBIT:
    {
        {
            // Direct TH08 ancestry keeps this otherwise-unused 12-byte local.
            // Its VC7.1 allocation rank is required by the TH095 target.
            Float3 legacyWork;
        }
        Float3 polarVelocity;

        this->orbitAngle =
            AddNormalizeAngle(
                this->orbitAngle,
                MovementFrameRateMultiplier() *
                    this->orbitAngularVelocity);
        this->orbitRadius =
            MovementFrameRateMultiplier() *
                this->radialVelocity +
            this->orbitRadius;
        polarVelocity.FromAngleMagnitude(
            this->orbitAngle,
            this->orbitRadius);
        this->velocity.x = polarVelocity.x +
            this->movementInterpolationOrigin.x -
            this->position.x;
        this->velocity.y = polarVelocity.y +
            this->movementInterpolationOrigin.y -
            this->position.y;
        this->movementAngle = atan2f(
            this->velocity.y,
            this->velocity.x);
        if (this->movementDuration > 0)
        {
            this->movementTimer--;
            if (this->movementTimer <= 0)
                this->flags1 &=
                    ~PHOTO_ENEMY_MOVEMENT_MODE_MASK;
        }
        break;
    }

    case PHOTO_ENEMY_MOVEMENT_POLAR:
        this->movementAngle =
            AddNormalizeAngle(
                this->movementAngle,
                MovementFrameRateMultiplier() *
                    this->angularVelocity);
        this->speed = MovementFrameRateMultiplier() *
            this->acceleration +
            this->speed;
        this->velocity.FromAngleMagnitude(
            this->movementAngle,
            this->speed);
        this->velocity.operator float *()[2] = 0.0f;
        if (this->movementDuration > 0)
        {
            this->movementTimer--;
            if (this->movementTimer <= 0)
                this->flags1 &=
                    ~PHOTO_ENEMY_MOVEMENT_MODE_MASK;
        }
        break;

    case PHOTO_ENEMY_MOVEMENT_INTERPOLATED:
    {
        f32 progress;

        this->movementTimer--;
        progress = 1.0f -
            (f32)this->movementTimer /
                this->movementDuration;
        if (progress < 0.0f)
            progress = 0.0f;
        switch ((this->flags1 >>
                    PHOTO_ENEMY_MOVEMENT_EASING_SHIFT) & 7)
        {
        case PHOTO_ENEMY_EASING_IN_QUADRATIC: progress *= progress; break;
        case PHOTO_ENEMY_EASING_IN_CUBIC: progress = progress * progress * progress; break;
        case PHOTO_ENEMY_EASING_IN_QUARTIC: progress = progress * progress * progress * progress; break;
        case PHOTO_ENEMY_EASING_OUT_QUADRATIC:
            progress = 1.0f - progress;
            progress *= progress;
            progress = 1.0f - progress;
            break;
        case PHOTO_ENEMY_EASING_OUT_CUBIC:
            progress = 1.0f - progress;
            progress = progress * progress * progress;
            progress = 1.0f - progress;
            break;
        case PHOTO_ENEMY_EASING_OUT_QUARTIC:
            progress = 1.0f - progress;
            progress = progress * progress * progress * progress;
            progress = 1.0f - progress;
            break;
        }

        this->velocity =
            this->movementInterpolationOrigin +
            this->movementInterpolationDelta * progress -
            this->position;
        if (((this->flags1 >>
                  PHOTO_ENEMY_MIRROR_MOVEMENT_X_SHIFT) & 1) != 0)
            this->velocity.x =
                -this->velocity.x;
        this->movementAngle = atan2f(
            this->velocity.y,
            this->velocity.x);
        if (this->movementTimer <= 0)
        {
            this->flags1 &=
                ~PHOTO_ENEMY_MOVEMENT_MODE_MASK;
            this->position =
                this->movementInterpolationOrigin +
                this->movementInterpolationDelta;
            this->velocity =
                Float3(0.0f, 0.0f, 0.0f);
        }
        break;
    }
    }
}

} // namespace th095
