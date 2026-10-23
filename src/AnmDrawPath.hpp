#pragma once

namespace th095
{
// Values stored in the manager's one-byte submission-path cache, not FVF
// values or shader handles. FlushVertexBuffer binds the buffered format
// without publishing a new cache value. See docs/ANM_DRAW_PATH.md.
enum AnmDrawPath
{
    ANM_DRAW_PATH_INITIAL = 0,
    ANM_DRAW_PATH_BUFFERED_QUADS = 1,
    ANM_DRAW_PATH_STREAMED_QUAD = 2,
    ANM_DRAW_PATH_IMMEDIATE_VERTICES = 3,
    ANM_DRAW_PATH_INVALIDATED = 0xff
};
}
