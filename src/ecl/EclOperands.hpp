#pragma once

#include "inttypes.hpp"

namespace th095
{
struct Enemy;

namespace EclOperands
{

#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
#define TH095_ECL_CALL_PARAMETER_INT0 0x2734
#define TH095_ECL_CALL_PARAMETER_INT1 0x2735
#define TH095_ECL_CALL_PARAMETER_INT2 0x2736
#define TH095_ECL_CALL_PARAMETER_INT3 0x2737
#define TH095_ECL_CALL_PARAMETER_FLOAT0 0x2738
#define TH095_ECL_CALL_PARAMETER_FLOAT1 0x2739
#define TH095_ECL_CALL_PARAMETER_FLOAT2 0x273a
#define TH095_ECL_CALL_PARAMETER_FLOAT3 0x273b
#else
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
};
#define TH095_ECL_CALL_PARAMETER_INT0 EclOperands::ECL_CALL_PARAMETER_INT0
#define TH095_ECL_CALL_PARAMETER_INT1 EclOperands::ECL_CALL_PARAMETER_INT1
#define TH095_ECL_CALL_PARAMETER_INT2 EclOperands::ECL_CALL_PARAMETER_INT2
#define TH095_ECL_CALL_PARAMETER_INT3 EclOperands::ECL_CALL_PARAMETER_INT3
#define TH095_ECL_CALL_PARAMETER_FLOAT0 EclOperands::ECL_CALL_PARAMETER_FLOAT0
#define TH095_ECL_CALL_PARAMETER_FLOAT1 EclOperands::ECL_CALL_PARAMETER_FLOAT1
#define TH095_ECL_CALL_PARAMETER_FLOAT2 EclOperands::ECL_CALL_PARAMETER_FLOAT2
#define TH095_ECL_CALL_PARAMETER_FLOAT3 EclOperands::ECL_CALL_PARAMETER_FLOAT3
#endif

i32 __fastcall ResolveInt(Enemy *enemy, i32 operand);
i32 *__fastcall ResolveIntLValue(Enemy *enemy, i32 *operand, u16 flags, i32 flagIndex);
f32 *__fastcall ResolveFloatLValue(Enemy *enemy, f32 *operand, u16 flags, i32 flagIndex);

} // namespace EclOperands

namespace EclRunLow
{
struct EclCallParameterCopy
{
    i32 ints[4];
    f32 floats[4];
};
extern EclCallParameterCopy g_EclCallParameters;
} // namespace EclRunLow
} // namespace th095
