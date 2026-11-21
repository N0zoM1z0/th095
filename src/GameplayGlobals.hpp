#ifndef TH095_GAMEPLAY_GLOBALS_HPP
#define TH095_GAMEPLAY_GLOBALS_HPP

#include "PhotoGameTaskOwner.hpp"

namespace th095
{

struct PhotoStageStateView;
struct PhotoGameUpdateView;

// Each subsystem publishes its active instance in its constructor and clears
// the pointer during teardown.
extern PhotoStageStateView *g_PhotoStageState;
extern PhotoGameUpdateView *g_RuntimePlayerOwner;

} // namespace th095

#endif
