#include "AnmManager.hpp"
#include "PhotoEnemy.hpp"
#include <math.h>

namespace th095
{
// Target-facing method ABI only.  Storage and semantic fields belong to the
// canonical compact PhotoEnemyView; the inherited Enemy name is retained only
// because EclRun calls the decorated Enemy::UpdateMovement symbol.

f32 AddNormalizeAngle(f32 a, f32 b);

static __forceinline f32 MovementFrameRateMultiplier()
{
    // The historical exact expression spelled this as g_Supervisor + 0x188,
    // but the resolved target is the independently owned game-speed scalar at
    // 0x004BDED8.  Both exact and normal builds use that canonical owner.
    return g_AnmGameSpeed;
}

struct Enemy : PhotoEnemyView
{
    void UpdateMovement();
};

#define PHOTO_ENEMY_MOVEMENT_FIELD(field) this->field

void Enemy::UpdateMovement()
{
    switch ((PHOTO_ENEMY_MOVEMENT_FIELD(flags1) >>
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

        PHOTO_ENEMY_MOVEMENT_FIELD(orbitAngle) =
            AddNormalizeAngle(
                PHOTO_ENEMY_MOVEMENT_FIELD(orbitAngle),
                MovementFrameRateMultiplier() *
                    PHOTO_ENEMY_MOVEMENT_FIELD(orbitAngularVelocity));
        PHOTO_ENEMY_MOVEMENT_FIELD(orbitRadius) =
            MovementFrameRateMultiplier() *
                PHOTO_ENEMY_MOVEMENT_FIELD(radialVelocity) +
            PHOTO_ENEMY_MOVEMENT_FIELD(orbitRadius);
        polarVelocity.FromAngleMagnitude(
            PHOTO_ENEMY_MOVEMENT_FIELD(orbitAngle),
            PHOTO_ENEMY_MOVEMENT_FIELD(orbitRadius));
        PHOTO_ENEMY_MOVEMENT_FIELD(velocity).x = polarVelocity.x +
            PHOTO_ENEMY_MOVEMENT_FIELD(movementInterpolationOrigin).x -
            PHOTO_ENEMY_MOVEMENT_FIELD(position).x;
        PHOTO_ENEMY_MOVEMENT_FIELD(velocity).y = polarVelocity.y +
            PHOTO_ENEMY_MOVEMENT_FIELD(movementInterpolationOrigin).y -
            PHOTO_ENEMY_MOVEMENT_FIELD(position).y;
        PHOTO_ENEMY_MOVEMENT_FIELD(movementAngle) = atan2f(
            PHOTO_ENEMY_MOVEMENT_FIELD(velocity).y,
            PHOTO_ENEMY_MOVEMENT_FIELD(velocity).x);
        if (PHOTO_ENEMY_MOVEMENT_FIELD(movementDuration) > 0)
        {
            PHOTO_ENEMY_MOVEMENT_FIELD(movementTimer)--;
            if (PHOTO_ENEMY_MOVEMENT_FIELD(movementTimer) <= 0)
                PHOTO_ENEMY_MOVEMENT_FIELD(flags1) &=
                    ~PHOTO_ENEMY_MOVEMENT_MODE_MASK;
        }
        break;
    }

    case PHOTO_ENEMY_MOVEMENT_POLAR:
        PHOTO_ENEMY_MOVEMENT_FIELD(movementAngle) =
            AddNormalizeAngle(
                PHOTO_ENEMY_MOVEMENT_FIELD(movementAngle),
                MovementFrameRateMultiplier() *
                    PHOTO_ENEMY_MOVEMENT_FIELD(angularVelocity));
        PHOTO_ENEMY_MOVEMENT_FIELD(speed) = MovementFrameRateMultiplier() *
            PHOTO_ENEMY_MOVEMENT_FIELD(acceleration) +
            PHOTO_ENEMY_MOVEMENT_FIELD(speed);
        PHOTO_ENEMY_MOVEMENT_FIELD(velocity).FromAngleMagnitude(
            PHOTO_ENEMY_MOVEMENT_FIELD(movementAngle),
            PHOTO_ENEMY_MOVEMENT_FIELD(speed));
        PHOTO_ENEMY_MOVEMENT_FIELD(velocity).operator float *()[2] = 0.0f;
        if (PHOTO_ENEMY_MOVEMENT_FIELD(movementDuration) > 0)
        {
            PHOTO_ENEMY_MOVEMENT_FIELD(movementTimer)--;
            if (PHOTO_ENEMY_MOVEMENT_FIELD(movementTimer) <= 0)
                PHOTO_ENEMY_MOVEMENT_FIELD(flags1) &=
                    ~PHOTO_ENEMY_MOVEMENT_MODE_MASK;
        }
        break;

    case PHOTO_ENEMY_MOVEMENT_INTERPOLATED:
    {
        f32 progress;

        PHOTO_ENEMY_MOVEMENT_FIELD(movementTimer)--;
        progress = 1.0f -
            (f32)PHOTO_ENEMY_MOVEMENT_FIELD(movementTimer) /
                PHOTO_ENEMY_MOVEMENT_FIELD(movementDuration);
        if (progress < 0.0f)
            progress = 0.0f;
        switch ((PHOTO_ENEMY_MOVEMENT_FIELD(flags1) >>
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

        PHOTO_ENEMY_MOVEMENT_FIELD(velocity) =
            PHOTO_ENEMY_MOVEMENT_FIELD(movementInterpolationOrigin) +
            PHOTO_ENEMY_MOVEMENT_FIELD(movementInterpolationDelta) * progress -
            PHOTO_ENEMY_MOVEMENT_FIELD(position);
        if (((PHOTO_ENEMY_MOVEMENT_FIELD(flags1) >>
                  PHOTO_ENEMY_MIRROR_MOVEMENT_X_SHIFT) & 1) != 0)
            PHOTO_ENEMY_MOVEMENT_FIELD(velocity).x =
                -PHOTO_ENEMY_MOVEMENT_FIELD(velocity).x;
        PHOTO_ENEMY_MOVEMENT_FIELD(movementAngle) = atan2f(
            PHOTO_ENEMY_MOVEMENT_FIELD(velocity).y,
            PHOTO_ENEMY_MOVEMENT_FIELD(velocity).x);
        if (PHOTO_ENEMY_MOVEMENT_FIELD(movementTimer) <= 0)
        {
            PHOTO_ENEMY_MOVEMENT_FIELD(flags1) &=
                ~PHOTO_ENEMY_MOVEMENT_MODE_MASK;
            PHOTO_ENEMY_MOVEMENT_FIELD(position) =
                PHOTO_ENEMY_MOVEMENT_FIELD(movementInterpolationOrigin) +
                PHOTO_ENEMY_MOVEMENT_FIELD(movementInterpolationDelta);
            PHOTO_ENEMY_MOVEMENT_FIELD(velocity) =
                Float3(0.0f, 0.0f, 0.0f);
        }
        break;
    }
    }
}

#undef PHOTO_ENEMY_MOVEMENT_FIELD
} // namespace th095
