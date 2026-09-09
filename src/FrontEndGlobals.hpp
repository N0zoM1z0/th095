#pragma once

#include "diffbuild.hpp"

namespace th095
{

// The target has one active front-end/pause controller pointer at 0x004CA2F4.
// Its concrete view changes with the current menu, so keep the storage type
// neutral and cast only at the owning call site.
DIFFABLE_EXTERN(void *, g_ActiveMenuController);

// These target globals are not independent storage. They are the +0x08 and
// +0x0c fields of Supervisor::replayScanWorker at target 0x004C4CB8. Exact
// probes retain their historical target-facing externs; the runnable build
// binds references to the embedded worker fields.
#if defined(DIFFBUILD) && !defined(TH095_MATCH_EXACT)
DIFFABLE_EXTERN(int, g_HelpLoadComplete);
DIFFABLE_EXTERN(int, g_HelpLoadActive);
#elif !defined(TH095_MATCH_EXACT)
extern int &g_HelpLoadComplete;
extern int &g_HelpLoadActive;
#endif

} // namespace th095
