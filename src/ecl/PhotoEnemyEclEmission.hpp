#pragma once

// EclRun exact/DIFF compiler boundary only.  The ancestral ECL translation
// unit still owns a conflicting private ANM declaration graph, so it cannot
// include PhotoEnemy.hpp until that graph is unified.  Normal EclRun consumes
// the complete canonical owner through PhotoEnemyManager.hpp.
namespace th095
{

struct PhotoEnemyView
{
    void ClampPosition();
};

} // namespace th095
