#ifndef TH095_GAMEPLAY_GLOBALS_HPP
#define TH095_GAMEPLAY_GLOBALS_HPP

#include "diffbuild.hpp"

namespace th095
{

// The target owns one pointer at each address below.  Earlier reconstruction
// units gave the same storage a different view-specific C++ symbol in each
// translation unit.  Production builds must share the pointer value so that
// lifecycle stores are visible to every consumer.  DIFFBUILD units retain
// their target-specific extern names and relocation evidence.
DIFFABLE_EXTERN(void *, g_RuntimeGameManagerOwner);
DIFFABLE_EXTERN(void *, g_RuntimeBulletManagerOwner);
DIFFABLE_EXTERN(void *, g_RuntimeEnemyManagerOwner);
DIFFABLE_EXTERN(void *, g_RuntimeBackgroundManagerOwner);
DIFFABLE_EXTERN(void *, g_RuntimeGameTaskOwner);
DIFFABLE_EXTERN(void *, g_RuntimeItemManagerOwner);
DIFFABLE_EXTERN(void *, g_RuntimeEffectManagerOwner);
DIFFABLE_EXTERN(void *, g_RuntimeStageStateOwner);
DIFFABLE_EXTERN(void *, g_RuntimePlayerOwner);

#ifndef DIFFBUILD
#define TH095_RUNTIME_GLOBAL_PTR(type, storage) \
    (*reinterpret_cast<type **>(&(storage)))
#endif

} // namespace th095

#endif
