#include "EnemyManager.hpp"
#include "ecl/EclManager.hpp"
#include "ecl/EclOperands.hpp"
#if !defined(DIFFBUILD) && !defined(TH095_MATCH_EXACT)
#include "ecl/EnemyEclRuntimeView.hpp"
#endif
#include "ZunMath.hpp"
#include <math.h>

namespace th095
{
namespace EclHelpers
{
// The target-local ECL control word is intentionally separate from the later
// shared Enemy::flags1 layout in EnemyManager.hpp. Exact/diff builds retain the
// historical private view so compiler-local label numbering stays unchanged.
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
struct EclHelperEnemyFlagBits
{
    u32 unknown00 : 10;
    u32 movementMode : 2;
    u32 movementEasing : 3;
    u32 unknown15 : 1;
    u32 mirrorMovementX : 1;
    u32 unknown17 : 15;
};
typedef char EclHelperEnemyFlagBitsSizeCheck[(sizeof(EclHelperEnemyFlagBits) == 4) ? 1 : -1];

#define HelperFlags(enemy) (*reinterpret_cast<EclHelperEnemyFlagBits *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4))
#else
#define HelperFlags(enemy) TH095_ENEMY_ECL_CONTROL_BITS(enemy)
#endif
#define ReadInt(enemy, instruction, index) \
    ((instruction)->operandFlags & (1 << (index)) \
         ? EclOperands::ResolveInt((enemy), (instruction)->operands[index].asInt) \
         : (instruction)->operands[index].asInt)
#define ReadFloat(enemy, instruction, index) \
    ((instruction)->operandFlags & (1 << (index)) \
         ? (enemy)->ResolveFloat((instruction)->operands[index].asFloat) \
         : (instruction)->operands[index].asFloat)

// FUNCTION: TH095 0x00411150; TH08 0x00422020 is the source-shape oracle.
void __fastcall ConfigurePolarMotion(Enemy *enemy, EclRawInstruction *instruction)
{
    f32 angle = AddNormalizeAngle(ReadFloat(enemy, instruction, 2), 0.0f);
    enemy->movementInterpolationDelta.x = cosf(angle) * ReadFloat(enemy, instruction, 3) * ReadInt(enemy, instruction, 0);
    enemy->movementInterpolationDelta.y = sinf(angle) * ReadFloat(enemy, instruction, 3) * ReadInt(enemy, instruction, 0);
    enemy->movementInterpolationDelta.z = 0.0f;
    enemy->movementInterpolationOrigin = enemy->worldPosition;
    enemy->movementTimer = (enemy->movementDuration = ReadInt(enemy, instruction, 0));
    HelperFlags(enemy).movementEasing = ReadInt(enemy, instruction, 1);
    HelperFlags(enemy).movementMode = 2;
    if (HelperFlags(enemy).mirrorMovementX)
        enemy->movementInterpolationDelta.x = -enemy->movementInterpolationDelta.x;
}

// FUNCTION: TH095 0x00411390; TH08 0x00422260 is the source-shape oracle.
void __fastcall ConfigureRelativeMotion(Enemy *enemy, EclRawInstruction *instruction)
{
    Float3 target;
    target.x = ReadFloat(enemy, instruction, 2);
    target.y = ReadFloat(enemy, instruction, 3);
    target.z = 0.0f;
    enemy->movementInterpolationDelta = target - enemy->worldPosition;
    enemy->movementInterpolationOrigin = enemy->position;
    enemy->movementTimer = (enemy->movementDuration = ReadInt(enemy, instruction, 0));
    HelperFlags(enemy).movementEasing = ReadInt(enemy, instruction, 1);
    HelperFlags(enemy).movementMode = 2;
    enemy->velocity = Float3(0.0f, 0.0f, 0.0f);
    if (HelperFlags(enemy).mirrorMovementX)
        enemy->movementInterpolationDelta.x = -enemy->movementInterpolationDelta.x;
}
#undef HelperFlags
#undef ReadFloat
#undef ReadInt
}
}
