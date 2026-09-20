#pragma once

#include "../PhotoEnemyEclAccess.hpp"

namespace th095
{

// Compatibility names for the legacy Enemy* method ABI.  The dependency-light
// access layer is shared by exact and normal profiles and is pinned to the one
// canonical PhotoEnemyView declaration by PhotoEnemy.hpp assertions.
#define TH095_ENEMY_ECL_CONTROL_WORD(enemy) \
    TH095_ECL_CONTROL_WORD(enemy)
#define TH095_ENEMY_ECL_CONTROL_BITS(enemy) \
    TH095_ECL_CONTROL_BITS(enemy)
#define TH095_ENEMY_ECL_SECONDARY_WORD(enemy) \
    TH095_ECL_SECONDARY_CONTROL_WORD(enemy)
#define TH095_ENEMY_ECL_SECONDARY_BITS(enemy) \
    TH095_ECL_SECONDARY_CONTROL_BITS(enemy)

} // namespace th095
