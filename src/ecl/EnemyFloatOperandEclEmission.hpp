#ifndef TH095_ENEMY_FLOAT_OPERAND_ECL_EMISSION_HPP
#define TH095_ENEMY_FLOAT_OPERAND_ECL_EMISSION_HPP

#include "EclManager.hpp"

namespace th095
{
namespace EclRunHigh
{

// Compiler-emission adapter for EclRun only. The runtime target is canonical
// Enemy::ResolveFloat @ 0x004105A0, but the accepted EclRun COFF relocations
// encode this historical receiver and by-value operand decoration. This type
// declares no storage and is not a compact-enemy owner.
struct EnemyFloatOperandView
{
    f32 ResolveFloat(EclRawOperand operand);
};

} // namespace EclRunHigh
} // namespace th095

#endif
