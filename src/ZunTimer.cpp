#include "Supervisor.hpp"

namespace th095
{
extern f32 g_AnmGameSpeed;

// FUNCTION: TH095 0x0041B830.
void ZunTimer::Add(f32 value)
{
    this->previous = this->current;
    if (g_AnmGameSpeed > 0.99f)
        this->subFrame += value;
    else
        this->subFrame += g_AnmGameSpeed * value;
    this->current = (i32)this->subFrame;
}
} // namespace th095
