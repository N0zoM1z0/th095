// TH095 low opcode handlers. RunEcl includes the case labels in its
// shared switch so the handlers use one frame and one jump table.
#ifndef TH095_ECL_RUN_LOW_DECLARATIONS
#define TH095_ECL_RUN_LOW_DECLARATIONS

#include "AnmManagerEclView.hpp"
#include "EclManager.hpp"
#include "EclOperands.hpp"
#include "../PhotoEnemyLayout.hpp"
#include "Player.hpp"
#include "SoundPlayer.hpp"

#include <math.h>

namespace th095
{

struct PhotoEnemyView;

namespace EclHelpers
{
void __fastcall ConfigurePolarMotion(
    PhotoEnemyView *enemy, EclRawInstruction *instruction);
void __fastcall ConfigureRelativeMotion(
    PhotoEnemyView *enemy, EclRawInstruction *instruction);
}

namespace EclRunLow
{

extern Player *g_Th095Player;

void __fastcall ApplyInterpolationOperation(
    PhotoEnemyView *enemy, EclRawInstruction *instruction);
void __fastcall InstallInterpolationSlot(
    PhotoEnemyView *enemy, EclRawInstruction *instruction);
void __fastcall CallSubOnEnemy(PhotoEnemyView *enemy,
                               EclRawInstruction *instruction, i32 rawSubId);
i32 __fastcall PopEclContext(PhotoEnemyView *enemy,
                             EclRawInstruction *instruction);
void __fastcall SetPrimaryAnmScripts(
    PhotoEnemyView *enemy, EclRawInstruction *instruction,
    i32 script0, i32 script1, i32 script2, i32 script3, i32 script4,
    i32 script5);

// The target helper at 0x00412200 receives the enemy in ECX and
// the current instruction in EDX.
void __fastcall BeginBoundaryAwareMove(
    PhotoEnemyView *enemy, EclRawInstruction *instruction);

EclRawInstruction *__fastcall CompareOperands(
    PhotoEnemyView *enemy, EclRawInstruction *instruction);

} // namespace EclRunLow
} // namespace th095

#endif // TH095_ECL_RUN_LOW_DECLARATIONS

#if !defined(TH095_ECL_RUN_DECLARATIONS_ONLY)

#if !defined(TH095_ECL_RUN_LOW_BODY) || !defined(TH095_ECL_RUN_SHARED_SWITCH)
#error EclRunLow.inl opcode body must be included inside PhotoEnemyEclManagerView::RunEcl
#endif

#define RawInt(insn, index) \
    ((insn)->operands[(index)].asInt)
#define RawFloat(insn, index) \
    ((insn)->operands[(index)].asFloat)
#define ReadInt(owner, insn, index) \
    (((insn)->operandFlags & (1U << (index))) \
         ? EclOperands::ResolveInt(owner, RawInt((insn), (index))) \
         : RawInt((insn), (index)))
#define ReadFloat(owner, insn, index) \
    (((insn)->operandFlags & (1U << (index))) \
         ? owner->ResolveFloat(RawFloat((insn), (index))) \
         : RawFloat((insn), (index)))
#define ReadFloatRawArg(owner, insn, index) \
    (((insn)->operandFlags & (1U << (index))) \
         ? owner->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt((insn), (index)))) \
         : RawFloat((insn), (index)))

#define WriteInt(owner, insn, index) \
    EclOperands::ResolveIntLValue(owner, &RawInt((insn), (index)), \
                                  (insn)->operandFlags, (index))
#define WriteFloat(owner, insn, index) \
    EclOperands::ResolveFloatLValue(owner, &RawFloat((insn), (index)), \
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
    case ECL_COMPARE_INT_EQUAL:
    case ECL_COMPARE_FLOAT_EQUAL:
    case ECL_COMPARE_INT_NOT_EQUAL:
    case ECL_COMPARE_FLOAT_NOT_EQUAL:
    case ECL_COMPARE_INT_LESS:
    case ECL_COMPARE_FLOAT_LESS:
    case ECL_COMPARE_INT_LESS_EQUAL:
    case ECL_COMPARE_FLOAT_LESS_EQUAL:
    case ECL_COMPARE_INT_GREATER:
    case ECL_COMPARE_FLOAT_GREATER:
    case ECL_COMPARE_INT_GREATER_EQUAL:
    case ECL_COMPARE_FLOAT_GREATER_EQUAL:
    {
        EclRawInstruction *branch = CompareOperands(
            enemy, instruction);
        if (branch)
        {
            instruction = branch;
            goto low_redispatch_instruction;
        }
        break;
    }

    case 52:
        CallSubOnEnemy(enemy, instruction, RawInt(instruction, 0));
        goto restart_context;

    case 53:
        if (!PopEclContext(enemy, instruction))
            goto restart_context;
        goto low_select_next_context;

    case 54:
        g_PhotoEnemyManager->enemyAnm
            ->SetAndExecuteScriptIdx(
            &enemy->vm,
            ReadInt(enemy, instruction, 0));
        enemy->flags1 &=
            ~PHOTO_ENEMY_ALTERNATE_ANM_BANK_MASK;
        break;
    case 55:
        lhsInt = ReadInt(enemy, instruction, 0);
        SetPrimaryAnmScripts(enemy, instruction, lhsInt, lhsInt + 1,
                             lhsInt + 2, lhsInt + 3, lhsInt + 4,
                             lhsInt + 5);
        enemy->flags1 &=
            ~PHOTO_ENEMY_ALTERNATE_ANM_BANK_MASK;
        break;
    case 56:
        SetPrimaryAnmScripts(enemy, instruction,
                             ReadInt(enemy, instruction, 0),
                             ReadInt(enemy, instruction, 1),
                             ReadInt(enemy, instruction, 2),
                             ReadInt(enemy, instruction, 3),
                             ReadInt(enemy, instruction, 4),
                             ReadInt(enemy, instruction, 5));
        enemy->flags1 &=
            ~PHOTO_ENEMY_ALTERNATE_ANM_BANK_MASK;
        break;
    case 58:
        g_PhotoEnemyManager->unknown4dfc
            ->SetAndExecuteScriptIdx(
            &enemy->vm,
            ReadInt(enemy, instruction, 0));
        enemy->flags1 |=
            PHOTO_ENEMY_ALTERNATE_ANM_BANK_MASK;
        break;
    case 59:
        lhsInt = ReadInt(enemy, instruction, 0);
        SetPrimaryAnmScripts(enemy, instruction, lhsInt, lhsInt + 1, lhsInt + 2,
                             lhsInt + 3, lhsInt + 4, lhsInt + 5);
        enemy->flags1 |=
            PHOTO_ENEMY_ALTERNATE_ANM_BANK_MASK;
        break;
    case 60:
        SetPrimaryAnmScripts(enemy, instruction,
                             ReadInt(enemy, instruction, 0),
                             ReadInt(enemy, instruction, 1),
                             ReadInt(enemy, instruction, 2),
                             ReadInt(enemy, instruction, 3),
                             ReadInt(enemy, instruction, 4),
                             ReadInt(enemy, instruction, 5));
        enemy->flags1 |=
            PHOTO_ENEMY_ALTERNATE_ANM_BANK_MASK;
        break;
    case 62:
        if (((enemy->flags1 >>
              PHOTO_ENEMY_ALTERNATE_ANM_BANK_SHIFT) & 1U) == 0)
        {
            g_PhotoEnemyManager->enemyAnm
                ->SetAndExecuteScriptIdx(
                &enemy->vm,
                enemy->anmScripts.special);
        }
        else
        {
            g_PhotoEnemyManager->unknown4dfc
                ->SetAndExecuteScriptIdx(
                &enemy->vm,
                enemy->anmScripts.special);
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
        enemy->flags1 =
            (enemy->flags1 &
             0xfffff3ffU) | 0x400U;
        enemy->movementDuration = 0;
        enemy->movementTimer = 0;
        break;
    case 66:
        if (ReadInt(enemy, instruction, 0) <= 0)
        {
            enemy->movementAngle = AddNormalizeAngle(((instruction->operandFlags & (1U << 2)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 2))), 0.0f);
            enemy->speed = ((instruction->operandFlags & (1U << 3)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 3))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 3)));
            enemy->flags1 =
                (enemy->flags1 &
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
                TH095_ECL_PLAYER_ANGLE(
                    &enemy->position));
        enemy->speed = ((instruction->operandFlags & (1U << 1)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)));
        break;
    case 69:
        if (ReadInt(enemy, instruction, 0) <= 0)
        {
            enemy->movementAngle =
                AddNormalizeAngle(
                    ((instruction->operandFlags & (1U << 2)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 2))),
                    TH095_ECL_PLAYER_ANGLE(
                        &enemy->position));
            enemy->speed = ((instruction->operandFlags & (1U << 3)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 3))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 3)));
            enemy->flags1 =
                (enemy->flags1 &
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
        enemy->flags1 =
            (enemy->flags1 &
             0xfffff3ffU) | 0x400U;
        break;
    case 71:
        enemy->acceleration = ((instruction->operandFlags & (1U << 0)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 0))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 0)));
        enemy->flags1 =
            (enemy->flags1 &
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
        enemy->flags1 |=
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
        enemy->flags1 |=
            0xc00U;
        break;
    case 74:
        enemy->movementTimer =
            (enemy->movementDuration = ReadInt(enemy, instruction, 0));
        enemy->orbitAngularVelocity = ((instruction->operandFlags & (1U << 1)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 1))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 1)));
        enemy->radialVelocity = ((instruction->operandFlags & (1U << 2)) ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 2))) : *reinterpret_cast<f32 *>(&RawInt(instruction, 2)));
        enemy->flags1 |=
            0xc00U;
        break;
    case 75:
        enemy->movementBounds.lower.x =
            (instruction->operandFlags & (1U << 0))
                ? enemy->ResolveFloat(instruction->operands[0].asFloat)
                : instruction->operands[0].asFloat;
        enemy->movementBounds.lower.y =
            (instruction->operandFlags & (1U << 1))
                ? enemy->ResolveFloat(instruction->operands[1].asFloat)
                : instruction->operands[1].asFloat;
        enemy->movementBounds.upper.x =
            (instruction->operandFlags & (1U << 2))
                ? enemy->ResolveFloat(instruction->operands[2].asFloat)
                : instruction->operands[2].asFloat;
        enemy->movementBounds.upper.y =
            (instruction->operandFlags & (1U << 3))
                ? enemy->ResolveFloat(instruction->operands[3].asFloat)
                : instruction->operands[3].asFloat;
        enemy->flags1 |=
            PHOTO_ENEMY_CLAMP_TO_MOVEMENT_BOUNDS_MASK;
        break;
    case 76:
        enemy->flags1 &=
            ~PHOTO_ENEMY_CLAMP_TO_MOVEMENT_BOUNDS_MASK;
        break;
    case 77:
        enemy->collisionSize.x = ((instruction->operandFlags & (1U << 0))
            ? enemy->ResolveFloat(*reinterpret_cast<f32 *>(&RawInt(instruction, 0)))
            : *reinterpret_cast<f32 *>(&RawInt(instruction, 0)));
        enemy->collisionSize.y = ((instruction->operandFlags & (1U << 1))
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
        enemy->control.unknown006 = (lhsInt & 1) == 0;
        enemy->control.collidable = (lhsInt & 2) == 0;
        enemy->control.unknown003 = (lhsInt & 4) == 0;
        enemy->control.hiddenFromDrawGroups = (lhsInt & 8) != 0;
        enemy->control.skipOffscreenCheck = (lhsInt & 0x10) != 0;
        enemy->secondaryControl.unknown003 =
            (lhsInt & 0x20) != 0;
        break;

    case 80:
        lhsInt = ReadInt(enemy, instruction, 0);
        if (lhsInt & 1) enemy->flags1 &= ~0x40U;
        if (lhsInt & 2) enemy->flags1 &= ~4U;
        if (lhsInt & 4) enemy->flags1 &= ~8U;
        if (lhsInt & 8) enemy->flags1 |= 0x10U;
        if (lhsInt & 0x10) enemy->flags1 |= 0x4000000U;
        if (lhsInt & 0x20) enemy->flags2 |= 8U;
        break;

    case 81:
        lhsInt = ReadInt(enemy, instruction, 0);
        if (lhsInt & 1) enemy->flags1 |= 0x40U;
        if (lhsInt & 2) enemy->flags1 |= 4U;
        if (lhsInt & 4) enemy->flags1 |= 8U;
        if (lhsInt & 8) enemy->flags1 &= ~0x10U;
        if (lhsInt & 0x10) enemy->flags1 &= ~0x4000000U;
        if (lhsInt & 0x20) enemy->flags2 &= ~8U;
        break;



#undef WriteFloat
#undef WriteInt
#undef ReadFloat
#undef ReadInt
#undef RawFloat
#undef RawInt

#endif // !TH095_ECL_RUN_DECLARATIONS_ONLY
