#pragma once

namespace th095
{
struct PhotoGameTaskView;

// The factory publishes this slot, and task teardown clears it. Supervisor
// holds a separate pointer for its scene changes.
extern PhotoGameTaskView *g_RuntimeGlobalStateOwner;
}
