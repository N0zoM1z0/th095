#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoEnemyEclManager.hpp"
#include "ecl/EclManager.hpp"
#include "ecl/EclOperands.hpp"
#include "PhotoEnemyManagerLayout.hpp"
#include "utils.hpp"
#include "PhotoPlayerRuntime.hpp"
#include "ZunMath.hpp"
#include <stdlib.h>

namespace th095
{
namespace EclRunLow
{
#define DEP_READ_INT(enemy, instruction, index) \
    ((instruction)->operandFlags & (1U << (index)) \
         ? EclOperands::ResolveInt((enemy), (instruction)->operands[index].asInt) \
         : (instruction)->operands[index].asInt)
#define DEP_READ_FLOAT(enemy, instruction, index) \
    ((instruction)->operandFlags & (1U << (index)) \
         ? (enemy)->ResolveFloat((instruction)->operands[index].asFloat) \
         : (instruction)->operands[index].asFloat)

extern EnemyEclInterpolatorCallback g_EclInterpolatorCallbacks[];

#pragma var_order(end, start)

// FUNCTION: TH095 0x00412490; TH08 0x004222B0 is the source-shape oracle.
void __fastcall StartTimedPolarDisplacement(
    PhotoEnemyView *enemy, EclRawInstruction *instruction, f32 angle)
{
    enemy->movementInterpolationDelta.x =
        cosf(angle) * DEP_READ_FLOAT(reinterpret_cast<Enemy *>(enemy), instruction, 2) *
        DEP_READ_INT(reinterpret_cast<Enemy *>(enemy), instruction, 0);
    enemy->movementInterpolationDelta.y =
        sinf(angle) * DEP_READ_FLOAT(reinterpret_cast<Enemy *>(enemy), instruction, 2) *
        DEP_READ_INT(reinterpret_cast<Enemy *>(enemy), instruction, 0);
    enemy->movementInterpolationDelta.z = 0.0f;
    enemy->movementInterpolationOrigin = enemy->worldPosition;
    enemy->movementTimer =
        (enemy->movementDuration =
             DEP_READ_INT(reinterpret_cast<Enemy *>(enemy), instruction, 0));
    enemy->movementEasing = DEP_READ_INT(reinterpret_cast<Enemy *>(enemy), instruction, 1);
    enemy->movementMode = PHOTO_ENEMY_MOVEMENT_INTERPOLATED;
}

// FUNCTION: TH095 0x00412200; TH08 0x00422020 is the source-shape oracle.
void __fastcall BeginBoundaryAwareMove(
    PhotoEnemyView *enemy, EclRawInstruction *instruction)
{
    f32 angle;

    if (reinterpret_cast<const PhotoPlayerRuntimeView *>(g_RuntimePlayerOwner)
            ->playerPosition.x < enemy->position.x)
    {
        angle = AddNormalizeAngle(
            g_Rng.GetRandomF32InRange(1.5707964f) + 2.3561945f, 0.0f);
    }
    else
    {
        angle = g_Rng.GetRandomF32InRange(1.5707964f) - 0.78539819f;
    }

    if (enemy->position.x < enemy->movementBounds.lower.x + 96.0f)
    {
        if (angle > 1.5707964f)
            angle = 3.1415927f - angle;
        else if (angle < -1.5707964f)
            angle = -3.1415927f - angle;
    }

    if (enemy->position.x > enemy->movementBounds.upper.x - 96.0f)
    {
        if (angle < 1.5707964f && angle >= 0.0f)
            angle = 3.1415927f - enemy->movementAngle;
        else if (angle > -1.5707964f && angle <= 0.0f)
            angle = -3.1415927f - angle;
    }

    if (enemy->position.y < enemy->movementBounds.lower.y + 48.0f &&
        angle < 0.0f)
    {
        angle = -angle;
    }
    if (enemy->position.y > enemy->movementBounds.upper.y - 48.0f &&
        angle > 0.0f)
    {
        angle = -angle;
    }

    if (DEP_READ_INT(reinterpret_cast<Enemy *>(enemy), instruction, 0) <= 0)
    {
        enemy->movementAngle = angle;
        enemy->speed = DEP_READ_FLOAT(reinterpret_cast<Enemy *>(enemy), instruction, 2);
        enemy->movementMode = PHOTO_ENEMY_MOVEMENT_POLAR;
        enemy->movementDuration = 0;
        enemy->movementTimer = 0;
    }
    else
    {
        StartTimedPolarDisplacement(enemy, instruction, angle);
    }
}

// FUNCTION: TH095 0x004115A0; TH08 0x00421120 is the source-shape oracle.
void __fastcall InterpolateLinear(Enemy *enemy, EnemyEclInterpolationSlot *slot, f32 t)
{
    f32 start;
    f32 end;
    start = enemy->ResolveFloat(slot->parameters[0]);
    end = enemy->ResolveFloat(slot->parameters[1]);
    *EclOperands::ResolveFloatLValue(enemy, &slot->affectedVariable, 0, -1) =
        (end - start) * t + start;
}

// Stock VC7.1 lacks the patched TH08 var_order frontend. These scoped aliases
// map the eight real Hermite locals to target-proven identifier buckets; no
// padding or inactive storage is introduced.
#define hermiteWeight3 restartCommandProcessingLocal05
#define hermiteParameter3 averagedPanLocal12
#define hermiteWeight1 iLocal11
#define hermiteParameter2 commandCursorLocal02
#define hermiteParameter1 soundIndexLocal01
#define hermiteWeight2 jLocal00
#define hermiteWeight0 preloadBufferLocal03
#define hermiteParameter0 bufferLocal04

#pragma var_order(hermiteWeight3, hermiteParameter3, hermiteWeight1, hermiteParameter2, hermiteParameter1, hermiteWeight2, hermiteWeight0, hermiteParameter0)
// FUNCTION: TH095 0x00411600; TH08 0x00421180 is the source-shape oracle.
void __fastcall InterpolateHermite(Enemy *enemy, EnemyEclInterpolationSlot *slot, f32 t)
{
    f32 hermiteParameter0;
    f32 hermiteParameter1;
    f32 hermiteParameter2;
    f32 hermiteParameter3;
    f32 hermiteWeight0;
    f32 hermiteWeight1;
    f32 hermiteWeight2;
    f32 hermiteWeight3;
    hermiteParameter0 = enemy->ResolveFloat(slot->parameters[0]);
    hermiteParameter1 = enemy->ResolveFloat(slot->parameters[1]);
    hermiteParameter2 = enemy->ResolveFloat(slot->parameters[2]);
    hermiteParameter3 = enemy->ResolveFloat(slot->parameters[3]);
    hermiteWeight0 = (t - 1.0f) * (t - 1.0f) * (2.0f * t + 1.0f);
    hermiteWeight1 = t * t * (3.0f - 2.0f * t);
    hermiteWeight2 = (1.0f - t) * (1.0f - t) * t;
    hermiteWeight3 = (t - 1.0f) * t * t;
    *EclOperands::ResolveFloatLValue(enemy, &slot->affectedVariable, 0, -1) =
        hermiteWeight0 * hermiteParameter0 + hermiteWeight1 * hermiteParameter1 + hermiteWeight2 * hermiteParameter2 + hermiteWeight3 * hermiteParameter3;
}

#undef hermiteWeight3
#undef hermiteParameter3
#undef hermiteWeight1
#undef hermiteParameter2
#undef hermiteParameter1
#undef hermiteWeight2
#undef hermiteWeight0
#undef hermiteParameter0

#if !defined(DIFFBUILD) && !defined(TH095_MATCH_EXACT)
// Target 0x004A4250 contains seven linear interpolation entries followed by
// the single Hermite callback.  Bind named reconstructed functions so the
// production linker, rather than hard-coded image addresses, owns relocation.
EnemyEclInterpolatorCallback g_EclInterpolatorCallbacks[8] = {
    InterpolateLinear, InterpolateLinear, InterpolateLinear, InterpolateLinear,
    InterpolateLinear, InterpolateLinear, InterpolateLinear, InterpolateHermite};
#endif

// FUNCTION: TH095 0x00411700; TH08 0x00421300 is the source-shape oracle.
void __fastcall ApplyInterpolationOperation(Enemy *enemy, EclRawInstruction *instruction)
{
    f32 delta;
    delta = DEP_READ_FLOAT(enemy, instruction, 1) - DEP_READ_FLOAT(enemy, instruction, 2);
    *EclOperands::ResolveFloatLValue(enemy,
        reinterpret_cast<f32 *>(instruction->operands), instruction->operandFlags, 0) =
        delta * DEP_READ_FLOAT(enemy, instruction, 3) + DEP_READ_FLOAT(enemy, instruction, 2);
}

// FUNCTION: TH095 0x004117F0; TH08 0x004213F0 is the source-shape oracle.
void __fastcall InstallInterpolationSlot(Enemy *enemy, EclRawInstruction *instruction)
{
    EnemyEclInterpolationSlot *slot;
    i32 i;
    slot = enemy->activeEclContext->interpolationSlots;
    for (i = 0; i < 8; i++, slot++)
    {
        if (slot->callback != NULL &&
            slot->affectedVariable != *reinterpret_cast<f32 *>(instruction->operands))
            continue;
        slot->timer = 0;
        slot->affectedVariable = *reinterpret_cast<f32 *>(instruction->operands);
        slot->duration = DEP_READ_INT(enemy, instruction, 1);
        slot->callbackIndex = DEP_READ_INT(enemy, instruction, 2);
        slot->easing = DEP_READ_INT(enemy, instruction, 3);
        slot->callback = g_EclInterpolatorCallbacks[slot->callbackIndex];
        slot->parameters[0] = DEP_READ_FLOAT(enemy, instruction, 4);
        slot->parameters[1] = DEP_READ_FLOAT(enemy, instruction, 5);
        slot->parameters[2] = DEP_READ_FLOAT(enemy, instruction, 6);
        slot->parameters[3] = DEP_READ_FLOAT(enemy, instruction, 7);
        break;
    }
}

// FUNCTION: TH095 0x00411A00; TH08 0x004215F0 is the source-shape oracle.
EclRawInstruction *__fastcall CompareOperands(Enemy *enemy, EclRawInstruction *instruction)
{
    switch (instruction->opcode)
    {
    case TH095_ECL_COMPARE_OPCODE(40, ECL_COMPARE_INT_EQUAL): if (DEP_READ_INT(enemy, instruction, 0) == DEP_READ_INT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case TH095_ECL_COMPARE_OPCODE(41, ECL_COMPARE_FLOAT_EQUAL): if (DEP_READ_FLOAT(enemy, instruction, 0) == DEP_READ_FLOAT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case TH095_ECL_COMPARE_OPCODE(42, ECL_COMPARE_INT_NOT_EQUAL): if (DEP_READ_INT(enemy, instruction, 0) != DEP_READ_INT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case TH095_ECL_COMPARE_OPCODE(43, ECL_COMPARE_FLOAT_NOT_EQUAL): if (DEP_READ_FLOAT(enemy, instruction, 0) != DEP_READ_FLOAT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case TH095_ECL_COMPARE_OPCODE(44, ECL_COMPARE_INT_LESS): if (DEP_READ_INT(enemy, instruction, 0) < DEP_READ_INT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case TH095_ECL_COMPARE_OPCODE(45, ECL_COMPARE_FLOAT_LESS): if (DEP_READ_FLOAT(enemy, instruction, 0) < DEP_READ_FLOAT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case TH095_ECL_COMPARE_OPCODE(46, ECL_COMPARE_INT_LESS_EQUAL): if (DEP_READ_INT(enemy, instruction, 0) <= DEP_READ_INT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case TH095_ECL_COMPARE_OPCODE(47, ECL_COMPARE_FLOAT_LESS_EQUAL): if (DEP_READ_FLOAT(enemy, instruction, 0) <= DEP_READ_FLOAT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case TH095_ECL_COMPARE_OPCODE(48, ECL_COMPARE_INT_GREATER): if (DEP_READ_INT(enemy, instruction, 0) > DEP_READ_INT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case TH095_ECL_COMPARE_OPCODE(49, ECL_COMPARE_FLOAT_GREATER): if (DEP_READ_FLOAT(enemy, instruction, 0) > DEP_READ_FLOAT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case TH095_ECL_COMPARE_OPCODE(50, ECL_COMPARE_INT_GREATER_EQUAL): if (DEP_READ_INT(enemy, instruction, 0) >= DEP_READ_INT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case TH095_ECL_COMPARE_OPCODE(51, ECL_COMPARE_FLOAT_GREATER_EQUAL): if (DEP_READ_FLOAT(enemy, instruction, 0) >= DEP_READ_FLOAT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
compare_success:
        enemy->activeEclContext->time = instruction->operands[2].asInt;
        return reinterpret_cast<EclRawInstruction *>(reinterpret_cast<u8 *>(instruction) + instruction->operands[3].asInt);
    default:
compare_failure:
        return NULL;
    }
}

#define DEP_ANM_DIRECTION(enemy) \
    (reinterpret_cast<PhotoEnemyView *>(enemy)->anmDirection)
static __forceinline EnemyChildEclBlock **TargetChildEclBlocks(Enemy *enemy)
{
    return reinterpret_cast<PhotoEnemyView *>(enemy)->childEclBlocks;
}

// FUNCTION: TH095 0x00411F70; TH08 0x00421BD0 is the source-shape oracle.
void __fastcall CallSubOnEnemy(Enemy *enemy, EclRawInstruction *instruction, i32 rawSubId)
{
    enemy->activeEclContext->currentInstr =
        reinterpret_cast<EclRawInstruction *>(reinterpret_cast<u8 *>(instruction) + instruction->nextOffset);

    if (reinterpret_cast<PhotoEnemyView *>(enemy)->suppressEclCallStack == 0)
    {
        enemy->activeEclCallStack[enemy->activeEclCallStackDepth] =
            *enemy->activeEclContext;
    }

    g_PhotoEnemyManager->eclManager->InitializeContext(
        enemy->activeEclContext,
        static_cast<i16>(rawSubId));

    enemy->activeEclContext->scriptState.callParameters =
        g_PhotoEnemyManager->eclManager->callParameters;

    if (reinterpret_cast<PhotoEnemyView *>(enemy)->suppressEclCallStack == 0 &&
        enemy->activeEclCallStackDepth < 15)
    {
        ++enemy->activeEclCallStackDepth;
    }
}

// FUNCTION: TH095 0x00412060; TH08 0x00421CB0 is the source-shape oracle.
int __fastcall PopEclContext(Enemy *enemy, EclRawInstruction *instruction)
{
    i32 contextIndex;

    if (reinterpret_cast<PhotoEnemyView *>(enemy)->suppressEclCallStack != 0)
        utils::DebugPrint("error : no Stack Ret\r\n");

    --enemy->activeEclCallStackDepth;
    if (enemy->activeEclCallStackDepth < 0)
    {
        contextIndex = enemy->activeEclContext->childContextSlot - 1;
        if (TargetChildEclBlocks(enemy)[contextIndex] != NULL)
        {
            void *argument = TargetChildEclBlocks(enemy)[contextIndex];
            free(argument);
        }
        TargetChildEclBlocks(enemy)[contextIndex] = NULL;
        enemy->activeEclCallStack = &enemy->mainEclCallStackStorage[0];
        enemy->activeEclContext = &enemy->mainEclContextStorage;
        enemy->activeEclCallStackDepth = enemy->mainEclCallStackDepth;
        return 1;
    }

    *enemy->activeEclContext =
        enemy->activeEclCallStack[enemy->activeEclCallStackDepth];
    return 0;
}

// FUNCTION: TH095 0x00412190; TH08 0x00421DE0 is the source-shape oracle.
void __fastcall SetPrimaryAnmScripts(
    Enemy *enemy, EclRawInstruction *instruction,
    i32 script0, i32 script1, i32 script2, i32 script3, i32 script4, i32 script5)
{
    reinterpret_cast<PhotoEnemyView *>(enemy)->idleAnmScript =
        static_cast<i16>(script0);
    reinterpret_cast<PhotoEnemyView *>(enemy)->moveLeftAnmScript =
        static_cast<i16>(script1);
    reinterpret_cast<PhotoEnemyView *>(enemy)->moveRightAnmScript =
        static_cast<i16>(script2);
    reinterpret_cast<PhotoEnemyView *>(enemy)->idleFromLeftAnmScript =
        static_cast<i16>(script3);
    reinterpret_cast<PhotoEnemyView *>(enemy)->idleFromRightAnmScript =
        static_cast<i16>(script4);
    reinterpret_cast<PhotoEnemyView *>(enemy)->specialAnmScript =
        static_cast<i16>(script5);
    DEP_ANM_DIRECTION(enemy) = 0xff;
}

#undef DEP_ANM_DIRECTION
#undef DEP_READ_FLOAT
#undef DEP_READ_INT
}

}
