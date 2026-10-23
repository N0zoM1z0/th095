#pragma once
#include "ZunMath.hpp"
#include <stddef.h>
namespace th095
{
// Target SetupVertexBuffer/Draw3D: D3DFVF_XYZ | D3DFVF_TEX1, stride 0x14.
// Unlike ScreenEffect XYZRHW/diffuse, the final two floats are U and V.
struct AnmQuadVertex
{
    Float3 position;
    Float2 textureUV;
};

// Target 0x004CA290 receives XYZ and UV at stride 0x18. No target
// rendering consumer is established; the intervening word is never written
// by SetupVertexBuffer. Keep its role unknown.
struct AnmQuadVertexCopy
{
    Float3 position;
    u32 unknown00c;
    Float2 textureUV;
};

typedef char AnmQuadVertexSizeIs14[(sizeof(AnmQuadVertex) == 0x14) ? 1 : -1];
typedef char AnmQuadVertexPositionAt0[(offsetof(AnmQuadVertex, position) == 0) ? 1 : -1];
typedef char AnmQuadVertexUvAtC[(offsetof(AnmQuadVertex, textureUV) == 0x0c) ? 1 : -1];
typedef char AnmQuadUvIs8[(sizeof(Float2) == 8 && offsetof(Float2, y) == 4) ? 1 : -1];
typedef char AnmQuadVertexCopySizeIs18[(sizeof(AnmQuadVertexCopy) == 0x18) ? 1 : -1];
typedef char AnmQuadVertexCopyPositionAt0[(offsetof(AnmQuadVertexCopy, position) == 0) ? 1 : -1];
typedef char AnmQuadVertexCopyUnknownAtC[(offsetof(AnmQuadVertexCopy, unknown00c) == 0x0c) ? 1 : -1];
typedef char AnmQuadVertexCopyUvAt10[(offsetof(AnmQuadVertexCopy, textureUV) == 0x10) ? 1 : -1];
} // namespace th095
