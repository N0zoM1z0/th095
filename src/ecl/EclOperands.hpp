#pragma once

#include "inttypes.hpp"

namespace th095
{
struct Enemy;

namespace EclOperands
{

enum EclCallParameterSelector
{
    ECL_CALL_PARAMETER_INT0 = 0x2734,
    ECL_CALL_PARAMETER_INT1 = 0x2735,
    ECL_CALL_PARAMETER_INT2 = 0x2736,
    ECL_CALL_PARAMETER_INT3 = 0x2737,
    ECL_CALL_PARAMETER_FLOAT0 = 0x2738,
    ECL_CALL_PARAMETER_FLOAT1 = 0x2739,
    ECL_CALL_PARAMETER_FLOAT2 = 0x273a,
    ECL_CALL_PARAMETER_FLOAT3 = 0x273b,
    ECL_MANAGER_PARAMETER_INT0 = 0x273c,
    ECL_MANAGER_PARAMETER_INT1 = 0x273d,
    ECL_MANAGER_PARAMETER_INT2 = 0x273e,
    ECL_MANAGER_PARAMETER_INT3 = 0x273f,
    ECL_MANAGER_PARAMETER_FLOAT0 = 0x2740,
    ECL_MANAGER_PARAMETER_FLOAT1 = 0x2741,
    ECL_MANAGER_PARAMETER_FLOAT2 = 0x2742,
    ECL_MANAGER_PARAMETER_FLOAT3 = 0x2743,
};

#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
#define TH095_ECL_RANDOM_SELECTOR(rawValue, semanticName) rawValue
#else
enum EclRandomOperandSelector
{
    ECL_RANDOM_U31 = 0x2720,
    ECL_RANDOM_F32 = 0x2721,
    ECL_RANDOM_I32 = 0x2722,
    ECL_RANDOM_F32_SIGNED = 0x2723,
};
#define TH095_ECL_RANDOM_SELECTOR(rawValue, semanticName) EclOperands::semanticName
#endif

i32 __fastcall ResolveInt(Enemy *enemy, i32 operand);
i32 *__fastcall ResolveIntLValue(Enemy *enemy, i32 *operand, u16 flags, i32 flagIndex);
f32 *__fastcall ResolveFloatLValue(Enemy *enemy, f32 *operand, u16 flags, i32 flagIndex);

} // namespace EclOperands
} // namespace th095
