#include "EnemyManager.hpp"
#include "ecl/EclManager.hpp"
#include "ecl/EclOperands.hpp"
#include "utils.hpp"
#include <stdlib.h>

namespace th095
{
// TH095 keeps the runtime ECL manager at +0x4DF4 and its call-parameter
// block at +0x168. Keep these bounded views private to avoid disturbing the
// generic Enemy/TH08 ABI used by the exact 27KB RunEcl translation unit.
struct PhotoEnemyEclContextView;
struct PhotoEnemyEclManagerView
{
    u8 unknown000[0x168];
    EclRunLow::EclCallParameterCopy callParameters;
    i32 InitializeContext(PhotoEnemyEclContextView *context, i16 subroutineId);
};
typedef char EclDependencyManagerParametersAt168[(offsetof(PhotoEnemyEclManagerView, callParameters) == 0x168) ? 1 : -1];

struct EclDependencyRuntimeView
{
    u8 unknown0000[0x4df4];
    PhotoEnemyEclManagerView *eclManager;
};
typedef char EclDependencyRuntimeManagerAt4DF4[(offsetof(EclDependencyRuntimeView, eclManager) == 0x4df4) ? 1 : -1];
extern EclDependencyRuntimeView *g_PhotoEnemyManager;

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
    case 40: if (DEP_READ_INT(enemy, instruction, 0) == DEP_READ_INT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case 41: if (DEP_READ_FLOAT(enemy, instruction, 0) == DEP_READ_FLOAT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case 42: if (DEP_READ_INT(enemy, instruction, 0) != DEP_READ_INT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case 43: if (DEP_READ_FLOAT(enemy, instruction, 0) != DEP_READ_FLOAT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case 44: if (DEP_READ_INT(enemy, instruction, 0) < DEP_READ_INT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case 45: if (DEP_READ_FLOAT(enemy, instruction, 0) < DEP_READ_FLOAT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case 46: if (DEP_READ_INT(enemy, instruction, 0) <= DEP_READ_INT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case 47: if (DEP_READ_FLOAT(enemy, instruction, 0) <= DEP_READ_FLOAT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case 48: if (DEP_READ_INT(enemy, instruction, 0) > DEP_READ_INT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case 49: if (DEP_READ_FLOAT(enemy, instruction, 0) > DEP_READ_FLOAT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case 50: if (DEP_READ_INT(enemy, instruction, 0) >= DEP_READ_INT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
    case 51: if (DEP_READ_FLOAT(enemy, instruction, 0) >= DEP_READ_FLOAT(enemy, instruction, 1)) goto compare_success; goto compare_failure;
compare_success:
        enemy->activeEclContext->time = instruction->operands[2].asInt;
        return reinterpret_cast<EclRawInstruction *>(reinterpret_cast<u8 *>(instruction) + instruction->operands[3].asInt);
    default:
compare_failure:
        return NULL;
    }
}

// TH095 call-stack suppression is bit 24 of the target-local flags word at
// enemy +0x2BF4; the generic TH08 flag enum uses a different bit position.
static __forceinline u32 &TargetFlags1(Enemy *enemy)
{
    return *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4);
}
static __forceinline void **TargetAllocatedEclArgs(Enemy *enemy)
{
    return reinterpret_cast<void **>(reinterpret_cast<u8 *>(enemy) + 0x2cac);
}

// FUNCTION: TH095 0x00411F70; TH08 0x00421BD0 is the source-shape oracle.
void __fastcall CallSubOnEnemy(Enemy *enemy, EclRawInstruction *instruction, i32 rawSubId)
{
    enemy->activeEclContext->currentInstr =
        reinterpret_cast<EclRawInstruction *>(reinterpret_cast<u8 *>(instruction) + instruction->nextOffset);

    if (((TargetFlags1(enemy) >> 24) & 1) == 0)
    {
        enemy->activeEclCallStack[enemy->activeEclCallStackDepth] =
            *enemy->activeEclContext;
    }

    g_PhotoEnemyManager->eclManager->InitializeContext(
        reinterpret_cast<PhotoEnemyEclContextView *>(enemy->activeEclContext),
        static_cast<i16>(rawSubId));

    *reinterpret_cast<EclCallParameterCopy *>(
        &enemy->activeEclContext->callParameterInts[0]) =
        g_PhotoEnemyManager->eclManager->callParameters;

    if (((TargetFlags1(enemy) >> 24) & 1) == 0 &&
        enemy->activeEclCallStackDepth < 15)
    {
        ++enemy->activeEclCallStackDepth;
    }
}

// FUNCTION: TH095 0x00412060; TH08 0x00421CB0 is the source-shape oracle.
int __fastcall PopEclContext(Enemy *enemy, EclRawInstruction *instruction)
{
    i32 contextIndex;

    if (((TargetFlags1(enemy) >> 24) & 1) != 0)
        utils::DebugPrint("error : no Stack Ret\r\n");

    --enemy->activeEclCallStackDepth;
    if (enemy->activeEclCallStackDepth < 0)
    {
        contextIndex = enemy->activeEclContext->childContextSlot - 1;
        if (TargetAllocatedEclArgs(enemy)[contextIndex] != NULL)
        {
            void *argument = TargetAllocatedEclArgs(enemy)[contextIndex];
            free(argument);
        }
        TargetAllocatedEclArgs(enemy)[contextIndex] = NULL;
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
    *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x2c0e) = static_cast<i16>(script0);
    *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x2c14) = static_cast<i16>(script1);
    *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x2c16) = static_cast<i16>(script2);
    *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x2c10) = static_cast<i16>(script3);
    *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x2c12) = static_cast<i16>(script4);
    *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x2c18) = static_cast<i16>(script5);
    *reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(enemy) + 0x2c0a) = 0xff;
}

#undef DEP_READ_FLOAT
#undef DEP_READ_INT
}
}
