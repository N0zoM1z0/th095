#pragma once

namespace th095
{

// Shared render/main-loop outcomes. Render emits 0/1/2; WinMain also uses -1
// to classify chain-registration failure. These values are not calc-chain
// callback codes: Render translates calc results 0/-1 to exit/restart 1/2.
enum RenderResult
{
    RENDER_RESULT_KEEP_RUNNING = 0,
    RENDER_RESULT_EXIT_SUCCESS = 1,
    RENDER_RESULT_RESTART = 2,
    RENDER_RESULT_EXIT_ERROR = -1
};

typedef char RenderResultWidthIs4[(sizeof(RenderResult) == 4) ? 1 : -1];

} // namespace th095
