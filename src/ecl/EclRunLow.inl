// TH08 1.00d RunEcl low-opcode body (opcodes 1..92).
//
// Evidence boundary:
//   - dispatcher: 0x004184B0, jump at 0x00418623
//   - comparison helper: 0x004215F0
//   - target executable: resources/th08.exe (hash-attested by verify-target.py)
//   - .analysis/ecl-decompile.c is used only as a navigation aid
//
// The dispatch table indexes (opcode - 1).  Consequently Ghidra's `case 0`
// is target opcode 1.  Opcodes 3, 84, and 85 all land on the ordinary
// instruction-advance path in the target.
//
// EclRun.cpp includes the declarations once, then includes this opcode body
// lexically inside EclManager::RunEcl.  That shared function scope is required
// for the target's handler order, labels, locals, and stack frame.
//
// Every flagged scalar operand in this switch is resolved through
// EclOperands.  Raw operands are used only where the target demonstrably does
// so: jump times/displacements, opcode 36's slot selector, opcode 52's sub-id,
// and opcode 88's remote sub-id.

#ifndef TH08_ECL_RUN_LOW_DECLARATIONS
#define TH08_ECL_RUN_LOW_DECLARATIONS

#include "AnmManager.hpp"
#include "EclManager.hpp"
#include "EclOperands.hpp"
#include "EnemyManager.hpp"
#include "Player.hpp"
#include "SoundPlayer.hpp"

#include <math.h>

namespace th095
{

namespace EclHelpers
{
void __fastcall ConfigurePolarMotion(
    Enemy *enemy, EclRawInstruction *instruction);
void __fastcall ConfigureRelativeMotion(
    Enemy *enemy, EclRawInstruction *instruction);
}

namespace EclRunLow
{

extern Player *g_Th095Player;

struct Th095EnemyFlagBits
{
    u32 unused0 : 2;
    u32 collision : 1;
    u32 damageable : 1;
    u32 noSprite : 1;
    u32 unused5 : 1;
    u32 acceptsDamage : 1;
    u32 unused7 : 19;
    u32 allowOffscreen : 1;
    u32 unused27 : 5;
};

struct Th095EnemyFlag2Bits
{
    u32 unused0 : 3;
    u32 noDeath : 1;
    u32 unused4 : 28;
};

// Observed helper ABIs for opcodes 90..92. Both spawners receive the parent in
// ECX and the current instruction in EDX; the chain-tail lookup uses only ECX.
Enemy *__fastcall FindAttachmentChainTail(
    Enemy *parent);
Enemy *__fastcall SpawnChildAtScriptPosition(
    Enemy *parent, EclRawInstruction *instruction);
Enemy *__fastcall SpawnChildAtParentOffset(
    Enemy *parent, EclRawInstruction *instruction);
void __fastcall ApplyRandomBiasedMove(
    Enemy *enemy, EclRawInstruction *instruction);

// The returned effect begins with an ANM VM.  Keep the call out-of-line: the
// target dispatch calls AnmVm::SetInterrupt at 0x00407120 rather than inlining
// the public header's convenience setter.
struct SpawnedEffectAnmVm
{
    void SetInterrupt(i16 interrupt);
};

void __fastcall ApplyInterpolationOperation(
    Enemy *enemy, EclRawInstruction *instruction);
void __fastcall InstallInterpolationSlot(
    Enemy *enemy, EclRawInstruction *instruction);
void __fastcall CallSubOnEnemy(Enemy *enemy,
                               EclRawInstruction *instruction, i32 rawSubId);
i32 __fastcall PopEclContext(Enemy *enemy,
                             EclRawInstruction *instruction);
void __fastcall SetPrimaryAnmScripts(
    Enemy *enemy, EclRawInstruction *instruction,
    i32 script0, i32 script1, i32 script2, i32 script3, i32 script4,
    i32 script5);
void __fastcall SetExtraAnmScript(Enemy *enemy,
                                  EclRawInstruction *instruction);

// Target behavior at 0x00422020 establishes Enemy in ECX and the current ECL
// instruction in EDX for this boundary-aware movement helper.
void __fastcall BeginBoundaryAwareMove(
    Enemy *enemy, EclRawInstruction *instruction);

inline i32 &RawInt(EclRawInstruction *instruction, i32 index)
{
    return instruction->operands[index].asInt;
}

inline f32 &RawFloat(EclRawInstruction *instruction, i32 index)
{
    return instruction->operands[index].asFloat;
}

inline i32 ReadInt(Enemy *enemy,
                   EclRawInstruction *instruction, i32 index)
{
    const i32 raw = RawInt(instruction, index);
    return (instruction->operandFlags & (1U << index))
               ? EclOperands::ResolveInt(enemy, raw)
               : raw;
}

inline f32 ReadFloat(Enemy *enemy,
                     EclRawInstruction *instruction, i32 index)
{
    const f32 raw = RawFloat(instruction, index);
    return (instruction->operandFlags & (1U << index))
               ? enemy->ResolveFloat(raw)
               : raw;
}

inline i32 *WriteInt(Enemy *enemy,
                     EclRawInstruction *instruction, i32 index)
{
    return EclOperands::ResolveIntLValue(enemy, &RawInt(instruction, index),
                                         instruction->operandFlags, index);
}

inline f32 *WriteFloat(Enemy *enemy,
                       EclRawInstruction *instruction, i32 index)
{
    return EclOperands::ResolveFloatLValue(enemy, &RawFloat(instruction, index),
                                           instruction->operandFlags, index);
}

// FUNCTION: th08 0x004215F0
// The twelve comparison opcodes share this target fastcall helper.  Keeping
// their resolver calls here is essential: RunEcl itself has one direct call
// per opcode and receives either the branch destination or NULL.
static EclRawInstruction *__fastcall CompareOperands(
    Enemy *enemy, EclRawInstruction *instruction)
{
    bool takeBranch = false;
    const i32 operation = instruction->opcode - 40;
    if (operation == 0)
        takeBranch = ReadInt(enemy, instruction, 0) == ReadInt(enemy, instruction, 1);
    else if (operation == 1)
        takeBranch = ReadFloat(enemy, instruction, 0) == ReadFloat(enemy, instruction, 1);
    else if (operation == 2)
        takeBranch = ReadInt(enemy, instruction, 0) != ReadInt(enemy, instruction, 1);
    else if (operation == 3)
        takeBranch = ReadFloat(enemy, instruction, 0) != ReadFloat(enemy, instruction, 1);
    else if (operation == 4)
        takeBranch = ReadInt(enemy, instruction, 0) < ReadInt(enemy, instruction, 1);
    else if (operation == 5)
        takeBranch = ReadFloat(enemy, instruction, 0) < ReadFloat(enemy, instruction, 1);
    else if (operation == 6)
        takeBranch = ReadInt(enemy, instruction, 0) <= ReadInt(enemy, instruction, 1);
    else if (operation == 7)
        takeBranch = ReadFloat(enemy, instruction, 0) <= ReadFloat(enemy, instruction, 1);
    else if (operation == 8)
        takeBranch = ReadInt(enemy, instruction, 0) > ReadInt(enemy, instruction, 1);
    else if (operation == 9)
        takeBranch = ReadFloat(enemy, instruction, 0) > ReadFloat(enemy, instruction, 1);
    else if (operation == 10)
        takeBranch = ReadInt(enemy, instruction, 0) >= ReadInt(enemy, instruction, 1);
    else if (operation == 11)
        takeBranch = ReadFloat(enemy, instruction, 0) >= ReadFloat(enemy, instruction, 1);

    if (!takeBranch)
        return NULL;

    EnemyEclContext *context = enemy->activeEclContext;
    context->time.current = RawInt(instruction, 2);
    return reinterpret_cast<EclRawInstruction *>(
        reinterpret_cast<u8 *>(instruction) + RawInt(instruction, 3));
}

} // namespace EclRunLow
} // namespace th095

#endif // TH08_ECL_RUN_LOW_DECLARATIONS

#if !defined(TH08_ECL_RUN_DECLARATIONS_ONLY)

#if !defined(TH08_ECL_RUN_LOW_BODY) || !defined(TH08_ECL_RUN_SHARED_SWITCH)
#error EclRunLow.inl opcode body must be included inside EclManager::RunEcl
#endif

#define TH08_ECL_RUN_LOW_YIELD_SELECT_I(controlValue) \
    TH08_ECL_RUN_LOW_YIELD_##controlValue
#define TH08_ECL_RUN_LOW_YIELD_SELECT(controlValue) \
    TH08_ECL_RUN_LOW_YIELD_SELECT_I(controlValue)
#define TH08_ECL_RUN_LOW_YIELD_LOW_RESTART_RUN_LOOP(instructionValue) \
    do { goto restart_context; } while (0)
#define TH08_ECL_RUN_LOW_YIELD_LOW_SELECT_NEXT_CONTEXT(instructionValue) \
    do { goto low_select_next_context; } while (0)
#define TH08_ECL_RUN_LOW_YIELD(controlValue, instructionValue) \
    TH08_ECL_RUN_LOW_YIELD_SELECT(controlValue)(instructionValue)

#define RawInt(insn, index) \
    ((insn)->operands[(index)].asInt)
#define RawFloat(insn, index) \
    ((insn)->operands[(index)].asFloat)
#define ReadInt(owner, insn, index) \
    (((insn)->operandFlags & (1U << (index))) \
         ? EclOperands::ResolveInt((owner), RawInt((insn), (index))) \
         : RawInt((insn), (index)))
#define ReadFloat(owner, insn, index) \
    (((insn)->operandFlags & (1U << (index))) \
         ? (owner)->ResolveFloat(RawFloat((insn), (index))) \
         : RawFloat((insn), (index)))
#define ReadFloatRawArg(owner, insn, index) \
    (((insn)->operandFlags & (1U << (index))) \
         ? (owner)->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt((insn), (index)))) \
         : RawFloat((insn), (index)))

#define WriteInt(owner, insn, index) \
    EclOperands::ResolveIntLValue((owner), &RawInt((insn), (index)), \
                                  (insn)->operandFlags, (index))
#define WriteFloat(owner, insn, index) \
    EclOperands::ResolveFloatLValue((owner), &RawFloat((insn), (index)), \
                                    (insn)->operandFlags, (index))
    f32 angle;
    f32 magnitude;
    f32 lhsFloat;
    f32 rhsFloat;

    case 1:
        return ZUN_ERROR;

    case 2:
        context->secondaryTime = ReadInt(enemy, instruction, 0);
        break;

    case 3:
        break; // dispatch-table entry is the ordinary advance path

    case 5:
        --*WriteInt(enemy, instruction, 2);
        if (ReadInt(enemy, instruction, 2) <= 0)
            goto low_advance_instruction;

    case 4:
        context->time = RawInt(instruction, 0);
        instruction = reinterpret_cast<EclRawInstruction *>(
            reinterpret_cast<u8 *>(instruction) + RawInt(instruction, 1));
        goto low_redispatch_instruction;

    case 6:
        *WriteInt(enemy, instruction, 0) = ReadInt(enemy, instruction, 1);
        break;
    case 7:
        *WriteFloat(enemy, instruction, 0) = ((instruction->operandFlags & (1U << 1))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)));
        break;
    case 8:
        *WriteInt(enemy, instruction, 0) =
            (g_Rng.GetRandomU16() & 1U ? 1 : -1) * ReadInt(enemy, instruction, 1);
        break;
    case 9:
        *WriteFloat(enemy, instruction, 0) =
            (g_Rng.GetRandomU16() & 1U ? 1.0f : -1.0f) *
            ((instruction->operandFlags & (1U << 1))
                ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
                : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)));
        break;

    case 10: *WriteInt(enemy, instruction, 0) += ReadInt(enemy, instruction, 1); break;
    case 15: *WriteFloat(enemy, instruction, 0) += ((instruction->operandFlags & (1U << 1))
        ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
        : *reinterpret_cast<f32 *>(&RawInt(instruction, 1))); break;
    case 11: *WriteInt(enemy, instruction, 0) -= ReadInt(enemy, instruction, 1); break;
    case 16: *WriteFloat(enemy, instruction, 0) -= ((instruction->operandFlags & (1U << 1))
        ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
        : *reinterpret_cast<f32 *>(&RawInt(instruction, 1))); break;
    case 12: *WriteInt(enemy, instruction, 0) *= ReadInt(enemy, instruction, 1); break;
    case 17: *WriteFloat(enemy, instruction, 0) *= ((instruction->operandFlags & (1U << 1))
        ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
        : *reinterpret_cast<f32 *>(&RawInt(instruction, 1))); break;
    case 13: *WriteInt(enemy, instruction, 0) /= ReadInt(enemy, instruction, 1); break;
    case 18: *WriteFloat(enemy, instruction, 0) /= ((instruction->operandFlags & (1U << 1))
        ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
        : *reinterpret_cast<f32 *>(&RawInt(instruction, 1))); break;
    case 14: *WriteInt(enemy, instruction, 0) %= ReadInt(enemy, instruction, 1); break;
    case 19:
        *WriteFloat(enemy, instruction, 0) =
            fmodf(((instruction->operandFlags & (1U << 0))
                       ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 0)))
                       : *reinterpret_cast<f32 *>(&RawInt(instruction, 0))),
                  ((instruction->operandFlags & (1U << 1))
                       ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
                       : *reinterpret_cast<f32 *>(&RawInt(instruction, 1))));
        break;

    case 20: *WriteInt(enemy, instruction, 0) = ReadInt(enemy, instruction, 1) + ReadInt(enemy, instruction, 2); break;
    case 25: *WriteFloat(enemy, instruction, 0) =
        ((instruction->operandFlags & (1U << 1))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 1))) +
        ((instruction->operandFlags & (1U << 2))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 2))); break;
    case 21: *WriteInt(enemy, instruction, 0) = ReadInt(enemy, instruction, 1) - ReadInt(enemy, instruction, 2); break;
    case 26: *WriteFloat(enemy, instruction, 0) =
        ((instruction->operandFlags & (1U << 1))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 1))) -
        ((instruction->operandFlags & (1U << 2))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 2))); break;
    case 22: *WriteInt(enemy, instruction, 0) = ReadInt(enemy, instruction, 1) * ReadInt(enemy, instruction, 2); break;
    case 27: *WriteFloat(enemy, instruction, 0) =
        ((instruction->operandFlags & (1U << 1))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 1))) *
        ((instruction->operandFlags & (1U << 2))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 2))); break;
    case 23: *WriteInt(enemy, instruction, 0) = ReadInt(enemy, instruction, 1) / ReadInt(enemy, instruction, 2); break;
    case 28: *WriteFloat(enemy, instruction, 0) =
        ((instruction->operandFlags & (1U << 1))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 1))) /
        ((instruction->operandFlags & (1U << 2))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 2))); break;
    case 24: *WriteInt(enemy, instruction, 0) = ReadInt(enemy, instruction, 1) % ReadInt(enemy, instruction, 2); break;
    case 29:
        *WriteFloat(enemy, instruction, 0) =
            fmodf(((instruction->operandFlags & (1U << 1))
                       ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
                       : *reinterpret_cast<f32 *>(&RawInt(instruction, 1))),
                  ((instruction->operandFlags & (1U << 2))
                       ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2)))
                       : *reinterpret_cast<f32 *>(&RawInt(instruction, 2))));
        break;
    case 30: ++*WriteInt(enemy, instruction, 0); break;
    case 31: --*WriteInt(enemy, instruction, 0); break;
    case 32: *WriteFloat(enemy, instruction, 0) = sinf(((instruction->operandFlags & (1U << 1))
        ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
        : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)))); break;
    case 33: *WriteFloat(enemy, instruction, 0) = cosf(((instruction->operandFlags & (1U << 1))
        ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
        : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)))); break;
    case 34:
        *WriteFloat(enemy, instruction, 0) = atan2f((((instruction->operandFlags & (1U << 4))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 4)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 4))) - ((instruction->operandFlags & (1U << 2))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 2)))), (((instruction->operandFlags & (1U << 3))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 3)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 3))) - ((instruction->operandFlags & (1U << 1))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)))));
        break;

    case 37:
        *WriteFloat(enemy, instruction, 0) =
            AddNormalizeAngle(((instruction->operandFlags & (1U << 0))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 0)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 0))), 0.0f);
        break;

    case 35:
        ApplyInterpolationOperation(enemy, instruction);
        break;

    case 36:
        InstallInterpolationSlot(enemy, instruction);
        break;

    case 38:
        rhsFloat = AddNormalizeAngle(((instruction->operandFlags & (1U << 2)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 2))), 0.0f);
        magnitude = ((instruction->operandFlags & (1U << 3)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 3))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 3)));
        *WriteFloat(enemy, instruction, 0) = cosf(rhsFloat) * magnitude;
        *WriteFloat(enemy, instruction, 1) = sinf(rhsFloat) * magnitude;
        break;

    case 39:
        angle = ((instruction->operandFlags & (1U << 1))
                ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
                : *reinterpret_cast<f32 *>(&RawInt(instruction, 1))) - ((instruction->operandFlags & (1U << 3))
                ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 3)))
                : *reinterpret_cast<f32 *>(&RawInt(instruction, 3)));
        lhsFloat = ((instruction->operandFlags & (1U << 2))
                ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2)))
                : *reinterpret_cast<f32 *>(&RawInt(instruction, 2))) - ((instruction->operandFlags & (1U << 4))
                ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 4)))
                : *reinterpret_cast<f32 *>(&RawInt(instruction, 4)));
        *WriteFloat(enemy, instruction, 0) =
            sqrtf(angle * angle + lhsFloat * lhsFloat);
        break;

    // 0x004215F0 comparison order is ==, !=, <, <=, >, >=, with integer
    // and float variants interleaved.  Successful branches use raw operands
    // 2 and 3 for the replacement time and signed bytecode displacement.
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
    {
        EclRawInstruction *branch = CompareOperands(enemy, instruction);
        if (branch)
        {
            instruction = branch;
            goto low_redispatch_instruction;
        }
        break;
    }

    case 52:
        CallSubOnEnemy(enemy, instruction, RawInt(instruction, 0));
        TH08_ECL_RUN_LOW_YIELD(LOW_RESTART_RUN_LOOP, 0);

    case 53:
        if (!PopEclContext(enemy, instruction))
            TH08_ECL_RUN_LOW_YIELD(LOW_RESTART_RUN_LOOP, 0);
        TH08_ECL_RUN_LOW_YIELD(LOW_SELECT_NEXT_CONTEXT, 0);

    case 54:
        (*reinterpret_cast<AnmLoaded **>(
            EclRunHigh::g_Th095Runtime + 0x4df8))
            ->SetAndExecuteScriptIdx(
            &enemy->vm,
            ReadInt(enemy, instruction, 0));
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &=
            0x7fffffffU;
        break;
    case 55:
        lhsInt = ReadInt(enemy, instruction, 0);
        SetPrimaryAnmScripts(enemy, instruction, lhsInt, lhsInt + 1,
                             lhsInt + 2, lhsInt + 3, lhsInt + 4,
                             lhsInt + 5);
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &=
            0x7fffffffU;
        break;
    case 56:
        SetPrimaryAnmScripts(enemy, instruction,
                             ReadInt(enemy, instruction, 0),
                             ReadInt(enemy, instruction, 1),
                             ReadInt(enemy, instruction, 2),
                             ReadInt(enemy, instruction, 3),
                             ReadInt(enemy, instruction, 4),
                             ReadInt(enemy, instruction, 5));
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &=
            0x7fffffffU;
        break;
#if 0 // TH095: opcode 57 is the ordinary advance path.
    case 57:
        SetExtraAnmScript(enemy, instruction);
        enemy->flags2 &= ~4U;
        break;
#endif
    case 58:
        (*reinterpret_cast<AnmLoaded **>(
            EclRunHigh::g_Th095Runtime + 0x4dfc))
            ->SetAndExecuteScriptIdx(
            &enemy->vm,
            ReadInt(enemy, instruction, 0));
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) |=
            0x80000000U;
        break;
    case 59:
        lhsInt = ReadInt(enemy, instruction, 0);
        SetPrimaryAnmScripts(enemy, instruction, lhsInt, lhsInt + 1, lhsInt + 2,
                             lhsInt + 3, lhsInt + 4, lhsInt + 5);
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) |=
            0x80000000U;
        break;
    case 60:
        SetPrimaryAnmScripts(enemy, instruction,
                             ReadInt(enemy, instruction, 0),
                             ReadInt(enemy, instruction, 1),
                             ReadInt(enemy, instruction, 2),
                             ReadInt(enemy, instruction, 3),
                             ReadInt(enemy, instruction, 4),
                             ReadInt(enemy, instruction, 5));
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) |=
            0x80000000U;
        break;
#if 0 // TH095: opcode 61 is the ordinary advance path.
    case 61:
        enemy->flags2 |= 4U;
        SetExtraAnmScript(enemy, instruction);
        break;
#endif
    case 62:
        if (((*reinterpret_cast<u32 *>(
                  reinterpret_cast<u8 *>(enemy) + 0x2bf4) >> 31) & 1U) == 0)
        {
            (*reinterpret_cast<AnmLoaded **>(
                EclRunHigh::g_Th095Runtime + 0x4df8))
                ->SetAndExecuteScriptIdx(
                &enemy->vm,
                *reinterpret_cast<i16 *>(
                    reinterpret_cast<u8 *>(enemy) + 0x2c18));
        }
        else
        {
            (*reinterpret_cast<AnmLoaded **>(
                EclRunHigh::g_Th095Runtime + 0x4dfc))
                ->SetAndExecuteScriptIdx(
                &enemy->vm,
                *reinterpret_cast<i16 *>(
                    reinterpret_cast<u8 *>(enemy) + 0x2c18));
        }
        break;

    case 63:
        enemy->position.x = ((instruction->operandFlags & (1U << 0)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 0))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 0)));
        enemy->position.y = ((instruction->operandFlags & (1U << 1)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)));
        enemy->position.z = 0.0f;
        enemy->ClampPosition();
        break;
    case 64:
        EclHelpers::ConfigureRelativeMotion(enemy, instruction);
        break;
    case 65:
        enemy->movementAngle = AddNormalizeAngle(((instruction->operandFlags & (1U << 0)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 0))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 0))), 0.0f);
        enemy->speed = ((instruction->operandFlags & (1U << 1)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)));
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) =
            (*reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &
             0xfffff3ffU) | 0x400U;
        enemy->movementDuration = 0;
        enemy->movementTimer = 0;
        break;
    case 66:
        if (ReadInt(enemy, instruction, 0) <= 0)
        {
            enemy->movementAngle = AddNormalizeAngle(((instruction->operandFlags & (1U << 2)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 2))), 0.0f);
            enemy->speed = ((instruction->operandFlags & (1U << 3)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 3))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 3)));
            *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) =
                (*reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &
                 0xfffff3ffU) | 0x400U;
            enemy->movementDuration = 0;
            enemy->movementTimer = 0;
        }
        else EclHelpers::ConfigurePolarMotion(enemy, instruction);
        break;
    case 67:
        BeginBoundaryAwareMove(enemy, instruction);
        break;
    case 68:
        enemy->movementAngle =
            AddNormalizeAngle(
                ((instruction->operandFlags & (1U << 0)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 0))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 0))),
                g_Th095Player->AngleToPoint(
                    &enemy->position));
        enemy->speed = ((instruction->operandFlags & (1U << 1)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)));
        break;
    case 69:
        if (ReadInt(enemy, instruction, 0) <= 0)
        {
            enemy->movementAngle =
                AddNormalizeAngle(
                    ((instruction->operandFlags & (1U << 2)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 2))),
                    g_Th095Player->AngleToPoint(
                        &enemy->position));
            enemy->speed = ((instruction->operandFlags & (1U << 3)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 3))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 3)));
            *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) =
                (*reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &
                 0xfffff3ffU) | 0x400U;
            // The target resolves operand 0 again before timer assignment.
            enemy->movementTimer =
                (enemy->movementDuration = ReadInt(enemy, instruction, 0));
        }
        else
        {
            EclHelpers::ConfigurePolarMotion(enemy, instruction);
        }
        break;

    case 70:
        enemy->angularVelocity = ((instruction->operandFlags & (1U << 0)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 0))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 0)));
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) =
            (*reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &
             0xfffff3ffU) | 0x400U;
        break;
    case 71:
        enemy->acceleration = ((instruction->operandFlags & (1U << 0)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 0))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 0)));
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) =
            (*reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &
             0xfffff3ffU) | 0x400U;
        break;
    case 72:
        enemy->movementTimer =
            (enemy->movementDuration = ReadInt(enemy, instruction, 0));
        enemy->movementInterpolationOrigin.x = ((instruction->operandFlags & (1U << 1)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)));
        enemy->movementInterpolationOrigin.y = ((instruction->operandFlags & (1U << 2))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 2)));
        enemy->orbitAngle = ((instruction->operandFlags & (1U << 3))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 3)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 3)));
        enemy->orbitAngularVelocity = ((instruction->operandFlags & (1U << 4))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 4)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 4)));
        enemy->orbitRadius = ((instruction->operandFlags & (1U << 5))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 5)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 5)));
        enemy->radialVelocity = ((instruction->operandFlags & (1U << 6))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 6)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 6)));
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) |=
            0xc00U;
        break;
    case 73:
        enemy->movementTimer =
            (enemy->movementDuration = ReadInt(enemy, instruction, 0));
        *reinterpret_cast<D3DXVECTOR3 *>(
            &enemy->movementInterpolationOrigin) =
            *reinterpret_cast<D3DXVECTOR3 *>(&enemy->position);
        enemy->orbitAngle = ((instruction->operandFlags & (1U << 1))
                ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
                : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)));
        enemy->orbitAngularVelocity = ((instruction->operandFlags & (1U << 2))
                ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2)))
                : *reinterpret_cast<f32 *>(&RawInt(instruction, 2)));
        enemy->orbitRadius = 0.0f;
        enemy->radialVelocity = ((instruction->operandFlags & (1U << 3))
                ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 3)))
                : *reinterpret_cast<f32 *>(&RawInt(instruction, 3)));
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) |=
            0xc00U;
        break;
    case 74:
        enemy->movementTimer =
            (enemy->movementDuration = ReadInt(enemy, instruction, 0));
        enemy->orbitAngularVelocity = ((instruction->operandFlags & (1U << 1)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)));
        enemy->radialVelocity = ((instruction->operandFlags & (1U << 2)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 2)));
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) |=
            0xc00U;
        break;
    case 75:
        *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(enemy) + 0x2c3c) = ((instruction->operandFlags & (1U << 0)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 0))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 0)));
        *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(enemy) + 0x2c40) = ((instruction->operandFlags & (1U << 1))
                ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
                : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)));
        *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(enemy) + 0x2c44) = ((instruction->operandFlags & (1U << 2))
                ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2)))
                : *reinterpret_cast<f32 *>(&RawInt(instruction, 2)));
        *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(enemy) + 0x2c48) = ((instruction->operandFlags & (1U << 3))
                ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 3)))
                : *reinterpret_cast<f32 *>(&RawInt(instruction, 3)));
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) |=
            0x20000U;
        break;
    case 76:
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &=
            0xfffdffffU;
        break;
    case 77:
        enemy->hitboxDimensions.x = ((instruction->operandFlags & (1U << 0))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 0)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 0)));
        enemy->hitboxDimensions.y = ((instruction->operandFlags & (1U << 1))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)));
        break;
    case 78:
        enemy->secondaryHitboxDimensions.x = ((instruction->operandFlags & (1U << 0))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 0)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 0)));
        enemy->secondaryHitboxDimensions.y = ((instruction->operandFlags & (1U << 1))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)));
        break;

    case 79:
        lhsInt = ReadInt(enemy, instruction, 0);
        reinterpret_cast<Th095EnemyFlagBits *>(
            reinterpret_cast<u8 *>(enemy) + 0x2bf4)->acceptsDamage = (lhsInt & 1) == 0;
        reinterpret_cast<Th095EnemyFlagBits *>(
            reinterpret_cast<u8 *>(enemy) + 0x2bf4)->collision = (lhsInt & 2) == 0;
        reinterpret_cast<Th095EnemyFlagBits *>(
            reinterpret_cast<u8 *>(enemy) + 0x2bf4)->damageable = (lhsInt & 4) == 0;
        reinterpret_cast<Th095EnemyFlagBits *>(
            reinterpret_cast<u8 *>(enemy) + 0x2bf4)->noSprite = (lhsInt & 8) != 0;
        reinterpret_cast<Th095EnemyFlagBits *>(
            reinterpret_cast<u8 *>(enemy) + 0x2bf4)->allowOffscreen = (lhsInt & 0x10) != 0;
        reinterpret_cast<Th095EnemyFlag2Bits *>(
            reinterpret_cast<u8 *>(enemy) + 0x2bf8)->noDeath = (lhsInt & 0x20) != 0;
        break;

    case 80:
        lhsInt = ReadInt(enemy, instruction, 0);
        if (lhsInt & 1) *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &= ~0x40U;
        if (lhsInt & 2) *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &= ~4U;
        if (lhsInt & 4) *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &= ~8U;
        if (lhsInt & 8) *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) |= 0x10U;
        if (lhsInt & 0x10) *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) |= 0x4000000U;
        if (lhsInt & 0x20) *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf8) |= 8U;
        break;

    case 81:
        lhsInt = ReadInt(enemy, instruction, 0);
        if (lhsInt & 1) *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) |= 0x40U;
        if (lhsInt & 2) *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) |= 4U;
        if (lhsInt & 4) *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) |= 8U;
        if (lhsInt & 8) *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &= ~0x10U;
        if (lhsInt & 0x10) *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4) &= ~0x4000000U;
        if (lhsInt & 0x20) *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf8) &= ~8U;
        break;

#if 0 // TH095: opcodes 86..92 all share the ordinary advance path.
    case 86:
        *WriteInt(enemy, instruction, 0) =
            (instruction->operandFlags & 2U)
                ? EclOperands::ResolveInt(
                      g_EnemyManager.bosses[ReadInt(enemy, instruction, 2)],
                      RawInt(instruction, 1))
                : RawInt(instruction, 1);
        break;

    case 87:
        if (g_EnemyManager.bosses[ReadInt(enemy, instruction, 2)])
            *WriteFloat(enemy, instruction, 0) =
                (instruction->operandFlags & 2U)
                    ? g_EnemyManager.bosses[ReadInt(enemy, instruction, 2)]->ResolveFloat(
                          *reinterpret_cast<f32 *>(&RawInt(instruction, 1)))
                    : *reinterpret_cast<f32 *>(&RawInt(instruction, 1));
        break;

    case 88:
        lhsInt = ReadInt(enemy, instruction, 0);
        CallSubOnEnemy(
            g_EnemyManager.bosses[lhsInt],
            g_EnemyManager.bosses[lhsInt]->activeEclContext->currentInstr,
            RawInt(instruction, 1));
        break;

    case 89:
        if (g_EnemyManager.bosses[ReadInt(enemy, instruction, 0)])
        {
            // Target resolves operand 0 a second time before the store.
            g_EnemyManager.bosses[ReadInt(enemy, instruction, 0)]->pendingEclSubroutineIndex =
                static_cast<i16>(ReadInt(enemy, instruction, 1));
        }
        break;

    // Target fact map for the linked-child cluster:
    //   90: 0x0041AF5B..0x0041B10A, standard constructor 0x0041F110
    //   91: 0x0041B10B..0x0041B2BA, alternate constructor 0x0041F280
    //   92: 0x0041B2BB..0x0041B4DB, standard constructor 0x0041F110
    // Each case calls 0x0041EFC0, its constructor, IsYoukai three times when
    // initialization is enabled, 0x00425B70 and 0x00407120 when no effect is
    // attached, and unconditionally plays sound 0x24 through 0x0045D660.
    // Case 92 additionally calls D3DXVECTOR3::operator+ at 0x00409080.
    case 90:
    {
        Enemy *tail = FindAttachmentChainTail(enemy);
        Enemy *child =
            SpawnChildAtScriptPosition(enemy, instruction);

        if (!g_EnemyManager.lastSpawnFailed)
        {
            reinterpret_cast<EnemyFlag1Bits *>(&child->flags1)->
                linkedChild = 1;
            reinterpret_cast<EnemyFlag1Bits *>(&child->flags1)->
                youkaiAligned =
                g_Player.IsYoukai();
            child->drawGroup =
                static_cast<u8>(((g_Player.IsYoukai()
                                  ? -1
                                  : 0) &
                                 -2) +
                                2);
            child->flags1 &= ~ENEMY_FLAG_COLLISION;

            if (child->alignmentEffect == 0)
            {
                child->alignmentEffect =
                    g_EffectManager.SpawnEffectInSecondaryPool(
                    0x20,
                    reinterpret_cast<D3DXVECTOR3 *>(
                        &child->position),
                    1, -1);
                child->alignmentEffect->vm.SetInterrupt(
                        g_Player.IsYoukai()
                            ? 2
                            : 1);
                child->alignmentEffect->vm.flag17 =
                    reinterpret_cast<EnemyFlag1Bits *>(
                        &child->flags1)->
                            collision != 0;
                if (child->enemyIndex & 1)
                {
                    reinterpret_cast<AnmVm *>(child->alignmentEffect)
                        ->angleVel.z =
                        -reinterpret_cast<AnmVm *>(child->alignmentEffect)
                             ->angleVel.z;
                }
            }

            child->parentEnemy =
                enemy;
            tail->nextInAttachmentChain =
                child;
            child->previousInAttachmentChain =
                tail;
            ++enemy->linkedChildCount;
        }

        g_SoundPlayer.PlaySoundPositionedByIdx(
            SOUND_FAMILIAR_SPAWN,
            enemy->position.x);
        break;
    }
    case 91:
    {
        Enemy *tail = FindAttachmentChainTail(enemy);
        Enemy *child =
            SpawnChildAtParentOffset(enemy, instruction);

        if (!g_EnemyManager.lastSpawnFailed)
        {
            reinterpret_cast<EnemyFlag1Bits *>(&child->flags1)->
                linkedChild = 1;
            reinterpret_cast<EnemyFlag1Bits *>(&child->flags1)->
                youkaiAligned =
                g_Player.IsYoukai();
            child->drawGroup =
                static_cast<u8>(((g_Player.IsYoukai()
                                  ? -1
                                  : 0) &
                                 -2) +
                                2);
            child->flags1 &= ~ENEMY_FLAG_COLLISION;

            if (child->alignmentEffect == 0)
            {
                child->alignmentEffect =
                    g_EffectManager.SpawnEffectInSecondaryPool(
                    0x20,
                    reinterpret_cast<D3DXVECTOR3 *>(
                        &child->position),
                    1, -1);
                child->alignmentEffect->vm.SetInterrupt(
                        g_Player.IsYoukai()
                            ? 2
                            : 1);
                child->alignmentEffect->vm.flag17 =
                    reinterpret_cast<EnemyFlag1Bits *>(
                        &child->flags1)->
                            collision != 0;
                if (child->enemyIndex & 1)
                {
                    reinterpret_cast<AnmVm *>(child->alignmentEffect)
                        ->angleVel.z =
                        -reinterpret_cast<AnmVm *>(child->alignmentEffect)
                             ->angleVel.z;
                }
            }

            child->parentEnemy =
                enemy;
            tail->nextInAttachmentChain =
                child;
            child->previousInAttachmentChain =
                tail;
            ++enemy->linkedChildCount;
        }

        g_SoundPlayer.PlaySoundPositionedByIdx(
            SOUND_FAMILIAR_SPAWN,
            enemy->position.x);
        break;
    }
    case 92:
    {
        Enemy *tail = FindAttachmentChainTail(enemy);
        Enemy *child =
            SpawnChildAtScriptPosition(enemy, instruction);

        if (!g_EnemyManager.lastSpawnFailed)
        {
            reinterpret_cast<EnemyFlag1Bits *>(&child->flags1)->
                linkedChild = 1;
            reinterpret_cast<EnemyFlag1Bits *>(&child->flags1)->
                youkaiAligned =
                g_Player.IsYoukai();
            child->drawGroup =
                static_cast<u8>(((g_Player.IsYoukai()
                                  ? -1
                                  : 0) &
                                 -2) +
                                2);

            *reinterpret_cast<D3DXVECTOR3 *>(
                &child->positionOffset) =
                *reinterpret_cast<D3DXVECTOR3 *>(
                    &enemy->position);
            *reinterpret_cast<D3DXVECTOR3 *>(
                &child->worldPosition) =
                *reinterpret_cast<D3DXVECTOR3 *>(
                    &child->positionOffset) +
                *reinterpret_cast<D3DXVECTOR3 *>(
                    &child->position);
            child->flags1 &= ~ENEMY_FLAG_COLLISION;

            if (child->alignmentEffect == 0)
            {
                child->alignmentEffect =
                    g_EffectManager.SpawnEffectInSecondaryPool(
                    0x20,
                    reinterpret_cast<D3DXVECTOR3 *>(
                        &child->worldPosition),
                    1, -1);
                child->alignmentEffect->vm.SetInterrupt(
                        g_Player.IsYoukai()
                            ? 2
                            : 1);
                child->alignmentEffect->vm.flag17 =
                    reinterpret_cast<EnemyFlag1Bits *>(
                        &child->flags1)->
                            collision != 0;
                if (child->enemyIndex & 1)
                {
                    reinterpret_cast<AnmVm *>(child->alignmentEffect)
                        ->angleVel.z =
                        -reinterpret_cast<AnmVm *>(child->alignmentEffect)
                             ->angleVel.z;
                }
            }

            reinterpret_cast<EnemyFlag1Bits *>(&child->flags1)->
                inheritParentPosition = 1;
            child->parentEnemy =
                enemy;
            tail->nextInAttachmentChain =
                child;
            child->previousInAttachmentChain =
                tail;
            ++enemy->linkedChildCount;
        }

        g_SoundPlayer.PlaySoundPositionedByIdx(
            SOUND_FAMILIAR_SPAWN,
            enemy->position.x);
        break;
    }
#endif

#undef TH08_ECL_RUN_LOW_YIELD

#undef TH08_ECL_RUN_LOW_YIELD_LOW_SELECT_NEXT_CONTEXT
#undef TH08_ECL_RUN_LOW_YIELD_LOW_RESTART_RUN_LOOP
#undef TH08_ECL_RUN_LOW_YIELD_SELECT
#undef TH08_ECL_RUN_LOW_YIELD_SELECT_I
#undef WriteFloat
#undef WriteInt
#undef ReadFloat
#undef ReadInt
#undef RawFloat
#undef RawInt

#endif // !TH08_ECL_RUN_DECLARATIONS_ONLY
