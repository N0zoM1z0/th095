#pragma once

namespace th095
{

struct PhotoEnemyView;

// Normal ECL sources receive the complete declaration through
// PhotoEnemyManager.hpp before these accessors are expanded.  This header is
// now only a compatibility router for the legacy Enemy* method ABI; it no
// longer publishes a second partial layout for the compact enemy element.
#define TH095_ENEMY_ECL_CONTROL_WORD(enemy) \
    (reinterpret_cast<PhotoEnemyView *>(enemy)->flags1)
#define TH095_ENEMY_ECL_CONTROL_BITS(enemy) \
    (*reinterpret_cast<PhotoEnemyView *>(enemy))
#define TH095_ENEMY_ECL_SECONDARY_WORD(enemy) \
    (reinterpret_cast<PhotoEnemyView *>(enemy)->flags2)
#define TH095_ENEMY_ECL_SECONDARY_BITS(enemy) \
    (*reinterpret_cast<PhotoEnemyView *>(enemy))

} // namespace th095
