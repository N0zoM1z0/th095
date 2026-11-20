#pragma once

namespace th095
{
// START/END identify the ordered local quad edge in the software/projected
// paths, including signed dimensions. Draw3D instead keeps its target-observed
// negative/positive absolute-half-size world translation. These names do not
// establish global left/right or top/bottom directions; see ANM_ALIGNMENT.md.
enum AnmAlignment
{
    ANM_ALIGNMENT_CENTER = 0,
    ANM_ALIGNMENT_START = 1,
    ANM_ALIGNMENT_END = 2,
    ANM_ALIGNMENT_UNHANDLED = 3
};
}
