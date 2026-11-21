#include "ZunTimer.hpp"

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
// FUNCTION: TH095 0x0041B8A0.
i32 ZunTimer::Tick()
{
    this->previous = this->current;
    if (g_AnmGameSpeed <= 0.99f)
    {
        this->subFrame += g_AnmGameSpeed;
        this->current = (i32)this->subFrame;
    }
    else
    {
        this->current++;
        this->subFrame += 1.0f;
    }
    return this->current;
}

} // namespace th095
