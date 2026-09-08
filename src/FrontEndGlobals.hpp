#pragma once

#include "diffbuild.hpp"

namespace th095
{

// The target has one active front-end/pause controller pointer at 0x004CA2F4.
// Its concrete view changes with the current menu, so keep the storage type
// neutral and cast only at the owning call site.
DIFFABLE_EXTERN(void *, g_ActiveMenuController);

} // namespace th095
