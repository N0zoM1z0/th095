#pragma once

#include "inttypes.hpp"

#include <stddef.h>

namespace th095
{

struct EclCallParameters
{
    i32 ints[4];
    f32 floats[4];
};

typedef char EclCallParametersSizeIs20[
    (sizeof(EclCallParameters) == 0x20) ? 1 : -1];
typedef char EclCallParametersFloatsAt10[
    (offsetof(EclCallParameters, floats) == 0x10) ? 1 : -1];

} // namespace th095
