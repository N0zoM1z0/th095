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
// Target 0x004BDEC8 is the standalone gameplay/global-state owner published by
// PhotoGameTask construction and cleared by its destruction. Do not merge it
// with the second publication slot below: target code writes the two slots at
// different points in the front-end/gameplay transition. This reconstruction
// owner name is production-only so it cannot perturb exact-unit COFF labels.
#if !defined(DIFFBUILD) && !defined(TH095_MATCH_EXACT)
extern void *g_RuntimeGlobalStateOwner;
#endif
// Target 0x004C4DF4 is Supervisor::photoGameTask at g_Supervisor + 0x784.
// Production binds a reference to that embedded slot; it is the front-end's
// publication of the task and is physically distinct from 0x004BDEC8.
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
DIFFABLE_EXTERN(void *, g_RuntimeGameTaskOwner);
#else
extern void *&g_RuntimeGameTaskOwner;
#endif
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
