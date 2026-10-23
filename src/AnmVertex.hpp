#pragma once
#include "ZunMath.hpp"
#include <stddef.h>
namespace th095
{
union ZunColor
{
    u32 color;
    struct
    {
        u8 b;
        u8 g;
        u8 r;
        u8 a;
    };
};

struct AnmVertex
{
    Float3 position;
    f32 rhw;
    ZunColor diffuse;
    Float2 uv;
};

typedef char AnmVertexSizeIs1C[(sizeof(AnmVertex) == 0x1c) ? 1 : -1];

typedef char AnmVertexPositionAt0[(offsetof(AnmVertex, position) == 0) ? 1 : -1];
typedef char AnmVertexRhwAtC[(offsetof(AnmVertex, rhw) == 0x0c) ? 1 : -1];
typedef char AnmVertexDiffuseAt10[(offsetof(AnmVertex, diffuse) == 0x10) ? 1 : -1];
typedef char AnmVertexUvAt14[(offsetof(AnmVertex, uv) == 0x14) ? 1 : -1];
typedef char AnmVertexColorIs4[(sizeof(ZunColor) == 4) ? 1 : -1];
typedef char AnmVertexColorIsBgra[(offsetof(ZunColor, b) == 0 && offsetof(ZunColor, g) == 1 && offsetof(ZunColor, r) == 2 && offsetof(ZunColor, a) == 3) ? 1 : -1];
} // namespace th095
