#pragma once

#include "ZunMath.hpp"

#include <stddef.h>

namespace th095
{

struct EnemyMovementBounds
{
    Float2 lower;
    Float2 upper;
};
typedef char EnemyMovementBoundsSizeIs10[
    (sizeof(EnemyMovementBounds) == 0x10) ? 1 : -1];
typedef char EnemyMovementBoundsUpperAt8[
    (offsetof(EnemyMovementBounds, upper) == 8) ? 1 : -1];

} // namespace th095
