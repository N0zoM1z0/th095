#include "AnmManager.hpp"
#include <math.h>

namespace th095
{
// Isolated target-local movement ABI. The shared EnemyManager.hpp models a
// later/larger Enemy layout used by other exact units and must not be mixed
// into this translation unit.

f32 AddNormalizeAngle(f32 a, f32 b);

static __forceinline f32 MovementFrameRateMultiplier()
{
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
    // The exact unit names the relocation base g_Supervisor, but its mapped
    // target address is 0x004BDD50; adding 0x188 resolves to 0x004BDED8.
    return *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(&g_Supervisor) + 0x188);
#else
    // In the runnable layout Supervisor has its real 0x004C4670-era shape.
    // Read the actual 0x004BDED8 owner instead of applying the exact probe's
    // synthetic relocation addend to the production Supervisor object.
    return g_AnmGameSpeed;
#endif
}

enum MovementModeProbe
{
    ENEMY_MOVEMENT_MODE_POLAR = 1,
    ENEMY_MOVEMENT_MODE_INTERPOLATED = 2,
    ENEMY_MOVEMENT_MODE_ORBIT = 3,
};
enum MovementEasingProbe
{
    ECL_EASING_IN_QUADRATIC = 1,
    ECL_EASING_IN_CUBIC = 2,
    ECL_EASING_IN_QUARTIC = 3,
    ECL_EASING_OUT_QUADRATIC = 4,
    ECL_EASING_OUT_CUBIC = 5,
    ECL_EASING_OUT_QUARTIC = 6,
};
enum MovementFlagProbe
{
    ENEMY_FLAG_MOVEMENT_MODE_SHIFT = 10,
    ENEMY_FLAG_MOVEMENT_EASING_SHIFT = 12,
    ENEMY_FLAG_MIRROR_MOVEMENT_X_SHIFT = 16,
    ENEMY_FLAG_MOVEMENT_MODE_MASK = 0xC00,
};

struct Enemy
{
    u8 prefix[0x28a0];
    Float3 position;
    Float3 positionOffset;
    Float3 velocity;
    Float3 previousPosition;
    Float3 lastFrameDisplacement;
    Float3 hitboxDimensions;
    Float3 secondaryHitboxDimensions;
    Float3 worldPosition;
    f32 movementAngle;
    f32 angularVelocity;
    f32 orbitAngle;
    f32 orbitAngularVelocity;
    Enemy *parentEnemy;
    f32 speed;
    f32 acceleration;
    f32 orbitRadius;
    f32 radialVelocity;
    Float3 shootOffset;
    Float3 movementInterpolationDelta;
    Float3 movementInterpolationOrigin;
    ZunTimer movementTimer;
    i32 movementDuration;
    u8 movementGap[0x2bf4 - 0x2958];
    u32 movementFlags;

    void UpdateMovement();
};
typedef char MovePosAt28A0[(offsetof(Enemy, position) == 0x28a0) ? 1 : -1];
typedef char MoveAngleAt2900[(offsetof(Enemy, movementAngle) == 0x2900) ? 1 : -1];
typedef char MoveDeltaAt2930[(offsetof(Enemy, movementInterpolationDelta) == 0x2930) ? 1 : -1];
typedef char MoveTimerAt2948[(offsetof(Enemy, movementTimer) == 0x2948) ? 1 : -1];
typedef char MoveFlagsAt2BF4[(offsetof(Enemy, movementFlags) == 0x2bf4) ? 1 : -1];

void Enemy::UpdateMovement()
{
    switch ((this->movementFlags >> ENEMY_FLAG_MOVEMENT_MODE_SHIFT) & 3)
    {
    case ENEMY_MOVEMENT_MODE_ORBIT:
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
                MovementFrameRateMultiplier() * this->orbitAngularVelocity);
        this->orbitRadius =
            MovementFrameRateMultiplier() * this->radialVelocity + this->orbitRadius;
        polarVelocity.FromAngleMagnitude(
            this->orbitAngle, this->orbitRadius);
        this->velocity.x = polarVelocity.x + this->movementInterpolationOrigin.x - this->position.x;
        this->velocity.y = polarVelocity.y + this->movementInterpolationOrigin.y - this->position.y;
        this->movementAngle = atan2f(this->velocity.y, this->velocity.x);
        if (this->movementDuration > 0)
        {
            this->movementTimer--;
            if (this->movementTimer <= 0)
                this->movementFlags &= ~ENEMY_FLAG_MOVEMENT_MODE_MASK;
        }
        break;
    }

    case ENEMY_MOVEMENT_MODE_POLAR:
        this->movementAngle =
            AddNormalizeAngle(
                this->movementAngle,
                MovementFrameRateMultiplier() * this->angularVelocity);
        this->speed = MovementFrameRateMultiplier() * this->acceleration + this->speed;
        this->velocity.FromAngleMagnitude(
            this->movementAngle, this->speed);
        this->velocity.operator float *()[2] = 0.0f;
        if (this->movementDuration > 0)
        {
            this->movementTimer--;
            if (this->movementTimer <= 0)
                this->movementFlags &= ~ENEMY_FLAG_MOVEMENT_MODE_MASK;
        }
        break;

    case ENEMY_MOVEMENT_MODE_INTERPOLATED:
    {
        f32 progress;

        this->movementTimer--;
        progress = 1.0f - (f32)this->movementTimer / this->movementDuration;
        if (progress < 0.0f)
            progress = 0.0f;
        switch ((this->movementFlags >> ENEMY_FLAG_MOVEMENT_EASING_SHIFT) & 7)
        {
        case ECL_EASING_IN_QUADRATIC: progress *= progress; break;
        case ECL_EASING_IN_CUBIC: progress = progress * progress * progress; break;
        case ECL_EASING_IN_QUARTIC: progress = progress * progress * progress * progress; break;
        case ECL_EASING_OUT_QUADRATIC:
            progress = 1.0f - progress;
            progress *= progress;
            progress = 1.0f - progress;
            break;
        case ECL_EASING_OUT_CUBIC:
            progress = 1.0f - progress;
            progress = progress * progress * progress;
            progress = 1.0f - progress;
            break;
        case ECL_EASING_OUT_QUARTIC:
            progress = 1.0f - progress;
            progress = progress * progress * progress * progress;
            progress = 1.0f - progress;
            break;
        }

        this->velocity = this->movementInterpolationOrigin + this->movementInterpolationDelta * progress - this->position;
        if (((this->movementFlags >> ENEMY_FLAG_MIRROR_MOVEMENT_X_SHIFT) & 1) != 0)
            this->velocity.x = -this->velocity.x;
        this->movementAngle = atan2f(this->velocity.y, this->velocity.x);
        if (this->movementTimer <= 0)
        {
            this->movementFlags &= ~ENEMY_FLAG_MOVEMENT_MODE_MASK;
            this->position = this->movementInterpolationOrigin + this->movementInterpolationDelta;
            this->velocity = Float3(0.0f, 0.0f, 0.0f);
        }
        break;
    }
    }
}
} // namespace th095
