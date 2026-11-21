#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoEnemyEclManager.hpp"
#include "PhotoEnemyEclAccess.hpp"
#include "PhotoEnemyManagerLayout.hpp"
#include "ecl/EclOperands.hpp"
#ifndef DIFFBUILD
#include "PhotoGamePlayer.hpp"
#endif

namespace th095
{

struct EclFloatPhotoCounterView
{
    i32 value;

    operator i32() const
    {
        return this->value;
    }
};

struct EclFloatOperandCameraView
{
    u8 unknown000[0xba8];
    EclFloatPhotoCounterView photoIndex;
    EclFloatPhotoCounterView photosTaken;
};

struct EclFloatOperandPlayerView
{
    u8 unknown0000[0x1e30];
    Float3 position;
    EclFloatOperandCameraView camera;

    f32 AngleFromPoint(Float3 *point);
};

extern EclFloatOperandPlayerView *g_EclFloatOperandPlayer;


#ifdef DIFFBUILD
#define TH095_ECL_FLOAT_PLAYER_ANGLE(point) g_EclFloatOperandPlayer->AngleFromPoint(point)
#else
#define TH095_ECL_FLOAT_PLAYER_ANGLE(point) \
    g_RuntimePlayerOwner->AngleFromPoint(point)
#endif

#ifdef DIFFBUILD
#define TH095_ECL_FLOAT_PLAYER_POSITION (g_EclFloatOperandPlayer->position)
#else
#define TH095_ECL_FLOAT_PLAYER_POSITION \
    (g_RuntimePlayerOwner->playerPosition)
#endif

#ifdef DIFFBUILD
#define TH095_ECL_FLOAT_PHOTO_INDEX (g_EclFloatOperandPlayer->camera.photoIndex)
#define TH095_ECL_FLOAT_PHOTOS_TAKEN (g_EclFloatOperandPlayer->camera.photosTaken)
#else
#define TH095_ECL_FLOAT_PHOTO_INDEX \
    (g_RuntimePlayerOwner->camera.photoIndex)
#define TH095_ECL_FLOAT_PHOTOS_TAKEN \
    (g_RuntimePlayerOwner->camera.photosTaken)
#endif

// FUNCTION: TH095 0x004105A0.  This is the float half of TH095's operand
// resolver pair, including photography counters and the active boss slots.
f32 Enemy::ResolveFloat(f32 operand)
{
    switch ((i32)operand)
    {
    case 0x2710: return (f32)this->activeEclContext->scriptState.intVariables[0];
    case 0x2711: return (f32)this->activeEclContext->scriptState.intVariables[1];
    case 0x2712: return (f32)this->activeEclContext->scriptState.intVariables[2];
    case 0x2713: return (f32)this->activeEclContext->scriptState.intVariables[3];
    case 0x2714: return (f32)this->activeEclContext->scriptState.intVariables[4];
    case 0x2715: return (f32)this->activeEclContext->scriptState.intVariables[5];
    case 0x2716: return (f32)this->activeEclContext->scriptState.intVariables[6];
    case 0x2717: return (f32)this->activeEclContext->scriptState.intVariables[7];

    case EclOperands::ECL_CALL_PARAMETER_INT0: return (f32)this->activeEclContext->scriptState.callParameters.ints[0];
    case EclOperands::ECL_CALL_PARAMETER_INT1: return (f32)this->activeEclContext->scriptState.callParameters.ints[1];
    case EclOperands::ECL_CALL_PARAMETER_INT2: return (f32)this->activeEclContext->scriptState.callParameters.ints[2];
    case EclOperands::ECL_CALL_PARAMETER_INT3: return (f32)this->activeEclContext->scriptState.callParameters.ints[3];
    case 0x2724: return (f32)this->activeEclContext->scriptState.extraIntVariables[0];
    case 0x2725: return (f32)this->activeEclContext->scriptState.extraIntVariables[1];
    case 0x2726: return (f32)this->activeEclContext->scriptState.extraIntVariables[2];
    case 0x2727: return (f32)this->activeEclContext->scriptState.extraIntVariables[3];

    case TH095_ECL_RANDOM_SELECTOR(0x2720, ECL_RANDOM_U31): return (f32)(g_Rng.GetRandomU32() & 0x7fffffff);
    case TH095_ECL_RANDOM_SELECTOR(0x2721, ECL_RANDOM_F32): return g_Rng.GetRandomF32();
    case TH095_ECL_RANDOM_SELECTOR(0x2722, ECL_RANDOM_I32): return (f32)(i32)g_Rng.GetRandomU32();
    case TH095_ECL_RANDOM_SELECTOR(0x2723, ECL_RANDOM_F32_SIGNED): return g_Rng.GetRandomF32Signed();
    case 0x2751: return g_Rng.GetRandomF32() * 6.2831855f - 3.1415927f;

    case EclOperands::ECL_ENEMY_TIMER: return (f32)reinterpret_cast<PhotoEnemyView *>(this)->eclTimer.current;
    case EclOperands::ECL_ENEMY_LIFE: return (f32)reinterpret_cast<PhotoEnemyView *>(this)->life;
    case EclOperands::ECL_ENEMY_ITEM_DROP_TYPE: return (f32)reinterpret_cast<PhotoEnemyView *>(this)->itemDropType;
    case EclOperands::ECL_ENEMY_SCORE: return (f32)reinterpret_cast<PhotoEnemyView *>(this)->score;

    case EclOperands::ECL_MANAGER_PARAMETER_INT0: return (f32)g_PhotoEnemyManager->eclManager->callParameters.ints[0];
    case EclOperands::ECL_MANAGER_PARAMETER_INT1: return (f32)g_PhotoEnemyManager->eclManager->callParameters.ints[1];
    case EclOperands::ECL_MANAGER_PARAMETER_INT2: return (f32)g_PhotoEnemyManager->eclManager->callParameters.ints[2];
    case EclOperands::ECL_MANAGER_PARAMETER_INT3: return (f32)g_PhotoEnemyManager->eclManager->callParameters.ints[3];
    case EclOperands::ECL_MANAGER_PARAMETER_FLOAT0: return g_PhotoEnemyManager->eclManager->callParameters.floats[0];
    case EclOperands::ECL_MANAGER_PARAMETER_FLOAT1: return g_PhotoEnemyManager->eclManager->callParameters.floats[1];
    case EclOperands::ECL_MANAGER_PARAMETER_FLOAT2: return g_PhotoEnemyManager->eclManager->callParameters.floats[2];
    case EclOperands::ECL_MANAGER_PARAMETER_FLOAT3: return g_PhotoEnemyManager->eclManager->callParameters.floats[3];

    case 0x2718: return this->activeEclContext->scriptState.floatVariables[0];
    case 0x2719: return this->activeEclContext->scriptState.floatVariables[1];
    case 0x271a: return this->activeEclContext->scriptState.floatVariables[2];
    case 0x271b: return this->activeEclContext->scriptState.floatVariables[3];
    case 0x271c: return this->activeEclContext->scriptState.floatVariables[4];
    case 0x271d: return this->activeEclContext->scriptState.floatVariables[5];
    case 0x271e: return this->activeEclContext->scriptState.floatVariables[6];
    case 0x271f: return this->activeEclContext->scriptState.floatVariables[7];
    case EclOperands::ECL_CALL_PARAMETER_FLOAT0: return this->activeEclContext->scriptState.callParameters.floats[0];
    case EclOperands::ECL_CALL_PARAMETER_FLOAT1: return this->activeEclContext->scriptState.callParameters.floats[1];
    case EclOperands::ECL_CALL_PARAMETER_FLOAT2: return this->activeEclContext->scriptState.callParameters.floats[2];
    case EclOperands::ECL_CALL_PARAMETER_FLOAT3: return this->activeEclContext->scriptState.callParameters.floats[3];

    case EclOperands::ECL_ENEMY_POSITION_X: return reinterpret_cast<PhotoEnemyView *>(this)->worldPosition.x;
    case EclOperands::ECL_ENEMY_POSITION_Y: return reinterpret_cast<PhotoEnemyView *>(this)->worldPosition.y;
    case EclOperands::ECL_ENEMY_POSITION_Z: return reinterpret_cast<PhotoEnemyView *>(this)->worldPosition.z;
    case 0x272d: return TH095_ECL_FLOAT_PLAYER_POSITION.x;
    case 0x272e: return TH095_ECL_FLOAT_PLAYER_POSITION.y;
    case 0x272f: return TH095_ECL_FLOAT_PLAYER_POSITION.z;
    case 0x275d: return this->activeEclContext->scriptState.extraFloatVariables[0];
    case 0x275e: return this->activeEclContext->scriptState.extraFloatVariables[1];
    case 0x275f: return this->activeEclContext->scriptState.extraFloatVariables[2];
    case 0x2760: return this->activeEclContext->scriptState.extraFloatVariables[3];
    case EclOperands::ECL_ENEMY_INTERPOLATION_ORIGIN_X: return reinterpret_cast<PhotoEnemyView *>(this)->movementInterpolationOrigin.x;
    case EclOperands::ECL_ENEMY_INTERPOLATION_ORIGIN_Y: return reinterpret_cast<PhotoEnemyView *>(this)->movementInterpolationOrigin.y;
    case EclOperands::ECL_ENEMY_INTERPOLATION_ORIGIN_Z: return reinterpret_cast<PhotoEnemyView *>(this)->movementInterpolationOrigin.z;
    case EclOperands::ECL_ENEMY_INTERPOLATION_DELTA_X: return reinterpret_cast<PhotoEnemyView *>(this)->movementInterpolationDelta.x;
    case EclOperands::ECL_ENEMY_INTERPOLATION_DELTA_Y: return reinterpret_cast<PhotoEnemyView *>(this)->movementInterpolationDelta.y;
    case EclOperands::ECL_ENEMY_INTERPOLATION_DELTA_Z: return reinterpret_cast<PhotoEnemyView *>(this)->movementInterpolationDelta.z;
    case 0x2754: return this->lastFrameDisplacement.x;
    case 0x2755: return this->lastFrameDisplacement.y;
    case 0x2756: return this->lastFrameDisplacement.z;

    case 0x2757: return (f32)TH095_ECL_SCHEDULE(this).photoIndices[0];
    case 0x2758: return (f32)TH095_ECL_SCHEDULE(this).photoIndices[1];
    case 0x2759: return (f32)TH095_ECL_SCHEDULE(this).photoIndices[2];
    case 0x275a: return (f32)TH095_ECL_SCHEDULE(this).photoIndices[3];
    case 0x2730: return TH095_ECL_FLOAT_PLAYER_ANGLE(&this->worldPosition);
    case 0x2765:
    {
        Float3 position = this->worldPosition + this->shootOffset;
        return TH095_ECL_FLOAT_PLAYER_ANGLE(&position);
    }

    case EclOperands::ECL_ENEMY_MOVEMENT_ANGLE: return reinterpret_cast<PhotoEnemyView *>(this)->movementAngle;
    case EclOperands::ECL_ENEMY_ANGULAR_VELOCITY: return reinterpret_cast<PhotoEnemyView *>(this)->angularVelocity;
    case EclOperands::ECL_ENEMY_SPEED: return reinterpret_cast<PhotoEnemyView *>(this)->speed;
    case EclOperands::ECL_ENEMY_ACCELERATION: return reinterpret_cast<PhotoEnemyView *>(this)->acceleration;
    case EclOperands::ECL_ENEMY_ORBIT_RADIUS: return reinterpret_cast<PhotoEnemyView *>(this)->orbitRadius;
    case EclOperands::ECL_ENEMY_ORBIT_ANGLE: return reinterpret_cast<PhotoEnemyView *>(this)->orbitAngle;
    case EclOperands::ECL_ENEMY_ORBIT_ANGULAR_VELOCITY: return reinterpret_cast<PhotoEnemyView *>(this)->orbitAngularVelocity;
    case 0x2753: return (f32)TH095_ECL_PHOTO_TARGET_SLOT(this);
    case 0x2752: return (f32)TH095_PHOTO_ENEMY_I32(
        this, PHOTO_ENEMY_ECL_UNKNOWN_2C50_OFFSET);
    case 0x2732:
    {
        Float3 delta = TH095_ECL_FLOAT_PLAYER_POSITION - this->worldPosition;
        return D3DXVec3Length(reinterpret_cast<D3DXVECTOR3 *>(&delta));
    }
    case 0x2761:
        return (f32)(i32)TH095_ECL_FLOAT_PHOTO_INDEX;
    case 0x2764:
        return (f32)(i32)TH095_ECL_FLOAT_PHOTOS_TAKEN;
    case 0x2762: return g_PhotoEnemyManager->photoTargets[0]->worldPosition.x;
    case 0x2763: return g_PhotoEnemyManager->photoTargets[0]->worldPosition.y;
    default: return operand;
    }
}

#undef TH095_ECL_FLOAT_PHOTOS_TAKEN
#undef TH095_ECL_FLOAT_PHOTO_INDEX
#undef TH095_ECL_FLOAT_PLAYER_POSITION
#undef TH095_ECL_PHOTO_TARGET_SLOT
#undef TH095_PHOTO_ENEMY_U8
#undef TH095_PHOTO_ENEMY_I32

} // namespace th095
