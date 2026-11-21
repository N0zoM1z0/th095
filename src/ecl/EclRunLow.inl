// TH095 low opcode handlers. RunEcl includes the case labels in its
// shared switch so the handlers use one frame and one jump table.
#ifndef TH095_ECL_RUN_LOW_DECLARATIONS
#define TH095_ECL_RUN_LOW_DECLARATIONS

#include "EclOperands.hpp"
#include "../PhotoEnemyLayout.hpp"
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

// Opcodes 79-81 use these bits in their first operand.
enum EclEnemyControlOperand
{
    ECL_ENEMY_CONTROL_BIT_0 = 1,
    ECL_ENEMY_CONTROL_DISABLE_COLLISION = 2,
    ECL_ENEMY_CONTROL_BIT_2 = 4,
    ECL_ENEMY_CONTROL_HIDE_DRAW_GROUP = 8,
    ECL_ENEMY_CONTROL_SKIP_OFFSCREEN_CHECK = 0x10,
    ECL_ENEMY_CONTROL_BIT_5 = 0x20,
};

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
#define ReadFloat(index) \
    ((instruction->operandFlags & (1U << (index))) \
         ? enemy->ResolveFloat(instruction->operands[(index)].asFloat) \
         : instruction->operands[(index)].asFloat)
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
        *WriteFloat(enemy, instruction, 0) = ReadFloat(1);
        break;
    case 8:
        *WriteInt(enemy, instruction, 0) =
            (g_Rng.GetRandomU16() & 1U ? 1 : -1) * ReadInt(enemy, instruction, 1);
        break;
    case 9:
        *WriteFloat(enemy, instruction, 0) =
            (g_Rng.GetRandomU16() & 1U ? 1.0f : -1.0f) *
            ReadFloat(1);
        break;

    case 10: *WriteInt(enemy, instruction, 0) += ReadInt(enemy, instruction, 1); break;
    case 15: *WriteFloat(enemy, instruction, 0) += ReadFloat(1); break;
    case 11: *WriteInt(enemy, instruction, 0) -= ReadInt(enemy, instruction, 1); break;
    case 16: *WriteFloat(enemy, instruction, 0) -= ReadFloat(1); break;
    case 12: *WriteInt(enemy, instruction, 0) *= ReadInt(enemy, instruction, 1); break;
    case 17: *WriteFloat(enemy, instruction, 0) *= ReadFloat(1); break;
    case 13: *WriteInt(enemy, instruction, 0) /= ReadInt(enemy, instruction, 1); break;
    case 18: *WriteFloat(enemy, instruction, 0) /= ReadFloat(1); break;
    case 14: *WriteInt(enemy, instruction, 0) %= ReadInt(enemy, instruction, 1); break;
    case 19:
        *WriteFloat(enemy, instruction, 0) =
            fmodf(ReadFloat(0),
                  ReadFloat(1));
        break;

    case 20: *WriteInt(enemy, instruction, 0) = ReadInt(enemy, instruction, 1) + ReadInt(enemy, instruction, 2); break;
    case 25: *WriteFloat(enemy, instruction, 0) = ReadFloat(1) + ReadFloat(2); break;
    case 21: *WriteInt(enemy, instruction, 0) = ReadInt(enemy, instruction, 1) - ReadInt(enemy, instruction, 2); break;
    case 26: *WriteFloat(enemy, instruction, 0) = ReadFloat(1) - ReadFloat(2); break;
    case 22: *WriteInt(enemy, instruction, 0) = ReadInt(enemy, instruction, 1) * ReadInt(enemy, instruction, 2); break;
    case 27: *WriteFloat(enemy, instruction, 0) = ReadFloat(1) * ReadFloat(2); break;
    case 23: *WriteInt(enemy, instruction, 0) = ReadInt(enemy, instruction, 1) / ReadInt(enemy, instruction, 2); break;
    case 28: *WriteFloat(enemy, instruction, 0) = ReadFloat(1) / ReadFloat(2); break;
    case 24: *WriteInt(enemy, instruction, 0) = ReadInt(enemy, instruction, 1) % ReadInt(enemy, instruction, 2); break;
    case 29:
        *WriteFloat(enemy, instruction, 0) =
            fmodf(ReadFloat(1),
                  ReadFloat(2));
        break;
    case 30: ++*WriteInt(enemy, instruction, 0); break;
    case 31: --*WriteInt(enemy, instruction, 0); break;
    case 32: *WriteFloat(enemy, instruction, 0) = sinf(ReadFloat(1)); break;
    case 33: *WriteFloat(enemy, instruction, 0) = cosf(ReadFloat(1)); break;
    case 34:
        *WriteFloat(enemy, instruction, 0) = atan2f(ReadFloat(4) - ReadFloat(2),
                  ReadFloat(3) - ReadFloat(1));
        break;

    case 37:
        *WriteFloat(enemy, instruction, 0) =
            AddNormalizeAngle(ReadFloat(0), 0.0f);
        break;

    case 35:
        ApplyInterpolationOperation(enemy, instruction);
        break;

    case 36:
        InstallInterpolationSlot(enemy, instruction);
        break;

    case 38:
        rhsFloat = AddNormalizeAngle(ReadFloat(2), 0.0f);
        magnitude = ReadFloat(3);
        *WriteFloat(enemy, instruction, 0) = cosf(rhsFloat) * magnitude;
        *WriteFloat(enemy, instruction, 1) = sinf(rhsFloat) * magnitude;
        break;

    case 39:
        angle = ReadFloat(1) - ReadFloat(3);
        lhsFloat = ReadFloat(2) - ReadFloat(4);
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
        enemy->control.alternateAnmBank = 0;
        break;
    case 55:
        lhsInt = ReadInt(enemy, instruction, 0);
        SetPrimaryAnmScripts(enemy, instruction, lhsInt, lhsInt + 1,
                             lhsInt + 2, lhsInt + 3, lhsInt + 4,
                             lhsInt + 5);
        enemy->control.alternateAnmBank = 0;
        break;
    case 56:
        SetPrimaryAnmScripts(enemy, instruction,
                             ReadInt(enemy, instruction, 0),
                             ReadInt(enemy, instruction, 1),
                             ReadInt(enemy, instruction, 2),
                             ReadInt(enemy, instruction, 3),
                             ReadInt(enemy, instruction, 4),
                             ReadInt(enemy, instruction, 5));
        enemy->control.alternateAnmBank = 0;
        break;
    case 58:
        g_PhotoEnemyManager->unknown4dfc
            ->SetAndExecuteScriptIdx(
            &enemy->vm,
            ReadInt(enemy, instruction, 0));
        enemy->control.alternateAnmBank = 1;
        break;
    case 59:
        lhsInt = ReadInt(enemy, instruction, 0);
        SetPrimaryAnmScripts(enemy, instruction, lhsInt, lhsInt + 1, lhsInt + 2,
                             lhsInt + 3, lhsInt + 4, lhsInt + 5);
        enemy->control.alternateAnmBank = 1;
        break;
    case 60:
        SetPrimaryAnmScripts(enemy, instruction,
                             ReadInt(enemy, instruction, 0),
                             ReadInt(enemy, instruction, 1),
                             ReadInt(enemy, instruction, 2),
                             ReadInt(enemy, instruction, 3),
                             ReadInt(enemy, instruction, 4),
                             ReadInt(enemy, instruction, 5));
        enemy->control.alternateAnmBank = 1;
        break;
    case 62:
        if (!enemy->control.alternateAnmBank)
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
        enemy->position.x = ReadFloat(0);
        enemy->position.y = ReadFloat(1);
        enemy->position.z = 0.0f;
        enemy->ClampPosition();
        break;
    case 64:
        EclHelpers::ConfigureRelativeMotion(enemy, instruction);
        break;
    case 65:
        enemy->movementAngle = AddNormalizeAngle(ReadFloat(0), 0.0f);
        enemy->speed = ReadFloat(1);
        enemy->control.movementMode = PHOTO_ENEMY_MOVEMENT_POLAR;
        enemy->movementDuration = 0;
        enemy->movementTimer = 0;
        break;
    case 66:
        if (ReadInt(enemy, instruction, 0) <= 0)
        {
            enemy->movementAngle = AddNormalizeAngle(ReadFloat(2), 0.0f);
            enemy->speed = ReadFloat(3);
            enemy->control.movementMode = PHOTO_ENEMY_MOVEMENT_POLAR;
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
                ReadFloat(0),
                g_RuntimePlayerOwner->AngleFromPoint(
                    &enemy->position));
        enemy->speed = ReadFloat(1);
        break;
    case 69:
        if (ReadInt(enemy, instruction, 0) <= 0)
        {
            enemy->movementAngle =
                AddNormalizeAngle(
                    ReadFloat(2),
                    g_RuntimePlayerOwner->AngleFromPoint(
                        &enemy->position));
            enemy->speed = ReadFloat(3);
            enemy->control.movementMode = PHOTO_ENEMY_MOVEMENT_POLAR;
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
        enemy->angularVelocity = ReadFloat(0);
        enemy->control.movementMode = PHOTO_ENEMY_MOVEMENT_POLAR;
        break;
    case 71:
        enemy->acceleration = ReadFloat(0);
        enemy->control.movementMode = PHOTO_ENEMY_MOVEMENT_POLAR;
        break;
    case 72:
        enemy->movementTimer =
            (enemy->movementDuration = ReadInt(enemy, instruction, 0));
        enemy->movementInterpolationOrigin.x = ReadFloat(1);
        enemy->movementInterpolationOrigin.y = ReadFloat(2);
        enemy->orbitAngle = ReadFloat(3);
        enemy->orbitAngularVelocity = ReadFloat(4);
        enemy->orbitRadius = ReadFloat(5);
        enemy->radialVelocity = ReadFloat(6);
        enemy->control.movementMode = PHOTO_ENEMY_MOVEMENT_ORBIT;
        break;
    case 73:
        enemy->movementTimer =
            (enemy->movementDuration = ReadInt(enemy, instruction, 0));
        enemy->movementInterpolationOrigin = enemy->position;
        enemy->orbitAngle = ReadFloat(1);
        enemy->orbitAngularVelocity = ReadFloat(2);
        enemy->orbitRadius = 0.0f;
        enemy->radialVelocity = ReadFloat(3);
        enemy->control.movementMode = PHOTO_ENEMY_MOVEMENT_ORBIT;
        break;
    case 74:
        enemy->movementTimer =
            (enemy->movementDuration = ReadInt(enemy, instruction, 0));
        enemy->orbitAngularVelocity = ReadFloat(1);
        enemy->radialVelocity = ReadFloat(2);
        enemy->control.movementMode = PHOTO_ENEMY_MOVEMENT_ORBIT;
        break;
    case 75:
        enemy->movementBounds.lower.x = ReadFloat(0);
        enemy->movementBounds.lower.y = ReadFloat(1);
        enemy->movementBounds.upper.x = ReadFloat(2);
        enemy->movementBounds.upper.y = ReadFloat(3);
        enemy->control.clampToMovementBounds = 1;
        break;
    case 76:
        enemy->control.clampToMovementBounds = 0;
        break;
    case 77:
        enemy->collisionSize.x = ReadFloat(0);
        enemy->collisionSize.y = ReadFloat(1);
        break;
    case 78:
        enemy->secondaryHitboxDimensions.x = ReadFloat(0);
        enemy->secondaryHitboxDimensions.y = ReadFloat(1);
        break;

    case 79:
        lhsInt = ReadInt(enemy, instruction, 0);
        enemy->control.unknown006 = (lhsInt & ECL_ENEMY_CONTROL_BIT_0) == 0;
        enemy->control.collidable =
            (lhsInt & ECL_ENEMY_CONTROL_DISABLE_COLLISION) == 0;
        enemy->control.unknown003 = (lhsInt & ECL_ENEMY_CONTROL_BIT_2) == 0;
        enemy->control.hiddenFromDrawGroups =
            (lhsInt & ECL_ENEMY_CONTROL_HIDE_DRAW_GROUP) != 0;
        enemy->control.skipOffscreenCheck =
            (lhsInt & ECL_ENEMY_CONTROL_SKIP_OFFSCREEN_CHECK) != 0;
        enemy->secondaryControl.unknown003 =
            (lhsInt & ECL_ENEMY_CONTROL_BIT_5) != 0;
        break;

    case 80:
        lhsInt = ReadInt(enemy, instruction, 0);
        if (lhsInt & ECL_ENEMY_CONTROL_BIT_0)
            enemy->control.unknown006 = 0;
        if (lhsInt & ECL_ENEMY_CONTROL_DISABLE_COLLISION)
            enemy->control.collidable = 0;
        if (lhsInt & ECL_ENEMY_CONTROL_BIT_2)
            enemy->control.unknown003 = 0;
        if (lhsInt & ECL_ENEMY_CONTROL_HIDE_DRAW_GROUP)
            enemy->control.hiddenFromDrawGroups = 1;
        if (lhsInt & ECL_ENEMY_CONTROL_SKIP_OFFSCREEN_CHECK)
            enemy->control.skipOffscreenCheck = 1;
        if (lhsInt & ECL_ENEMY_CONTROL_BIT_5)
            enemy->secondaryControl.unknown003 = 1;
        break;

    case 81:
        lhsInt = ReadInt(enemy, instruction, 0);
        if (lhsInt & ECL_ENEMY_CONTROL_BIT_0)
            enemy->control.unknown006 = 1;
        if (lhsInt & ECL_ENEMY_CONTROL_DISABLE_COLLISION)
            enemy->control.collidable = 1;
        if (lhsInt & ECL_ENEMY_CONTROL_BIT_2)
            enemy->control.unknown003 = 1;
        if (lhsInt & ECL_ENEMY_CONTROL_HIDE_DRAW_GROUP)
            enemy->control.hiddenFromDrawGroups = 0;
        if (lhsInt & ECL_ENEMY_CONTROL_SKIP_OFFSCREEN_CHECK)
            enemy->control.skipOffscreenCheck = 0;
        if (lhsInt & ECL_ENEMY_CONTROL_BIT_5)
            enemy->secondaryControl.unknown003 = 0;
        break;



#undef WriteFloat
#undef WriteInt
#undef ReadFloat
#undef ReadInt
#undef RawFloat
#undef RawInt

#endif // !TH095_ECL_RUN_DECLARATIONS_ONLY
