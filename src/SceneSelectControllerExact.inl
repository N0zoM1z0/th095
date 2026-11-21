#include "SceneSelect.hpp"
#include "Main.hpp"

namespace th095
{

// Retail passes the decoded resource directly to the variadic text method.
#define DRAW_SCENE_PREVIEW_TEXT(vm, textColor, text) \
    g_AnmManager->DrawTextLeft(vm, textColor, 0, text)

} // namespace th095
