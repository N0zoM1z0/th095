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
DIFFABLE_EXTERN(void *, g_RuntimeBulletManagerOwner);
DIFFABLE_EXTERN(void *, g_RuntimeEnemyManagerOwner);
DIFFABLE_EXTERN(void *, g_RuntimeBackgroundManagerOwner);
DIFFABLE_EXTERN(void *, g_RuntimeGameTaskOwner);
DIFFABLE_EXTERN(void *, g_RuntimeItemManagerOwner);
DIFFABLE_EXTERN(void *, g_RuntimeEffectManagerOwner);
DIFFABLE_EXTERN(void *, g_RuntimeStageStateOwner);
DIFFABLE_EXTERN(void *, g_RuntimePlayerOwner);

// Canonical exact probes must preserve the historical target-facing extern
// names without putting /DDIFFBUILD on the whole VC7.1 translation unit.
// Defining DIFFBUILD here is intentionally late: diffbuild.hpp and the shared
// ABI headers have already been parsed in their ordinary TH095 form, while
// source-local production owner aliases below this include are still suppressed.
#if defined(TH095_MATCH_EXACT) && !defined(DIFFBUILD)
#define DIFFBUILD
#endif

#ifndef DIFFBUILD
#define TH095_RUNTIME_GLOBAL_PTR(type, storage) \
    (*reinterpret_cast<type **>(&(storage)))
#endif

} // namespace th095

#endif
