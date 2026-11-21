#include "ControllerInputSlot.hpp"

namespace th095
{

// FUNCTION: TH095 0x004353B0.
void ControllerInputSlotView::UpdateReplayHistory()
{
    struct UpdateLocals
    {
        u16 currentBits;
        i32 bitIndex;
    } locals;

    locals.currentBits = this->replayHistory.current;
    // The replay update clears the live repeat mask before advancing its history.
    this->repeat = 0;
    for (locals.bitIndex = 0; locals.bitIndex < 16;
         ++locals.bitIndex, locals.currentBits >>= 1)
    {
        if ((locals.currentBits & 1) != 0)
        {
            this->replayHistory.heldFrames[locals.bitIndex]++;
            if (this->replayHistory.heldFrames[locals.bitIndex] >= 26)
            {
                // Target behavior: this internal repeat field receives bit 0,
                // even when a higher button is the one reaching the threshold.
                this->replayHistory.repeat |= 1;
                this->replayHistory.heldFrames[locals.bitIndex] -= 8;
            }
        }
        else
        {
            this->replayHistory.heldFrames[locals.bitIndex] = 0;
        }
    }

    this->replayHistory.pressed =
        (this->replayHistory.current ^ this->replayHistory.previous) & this->replayHistory.current;
    this->replayHistory.released =
        (this->replayHistory.current ^ this->replayHistory.previous) & ~this->replayHistory.current;
}

} // namespace th095
