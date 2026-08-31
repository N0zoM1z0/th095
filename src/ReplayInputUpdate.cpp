#include "ReplayInputSource.hpp"

namespace th095
{

// FUNCTION: TH095 0x004353B0.
void ReplayInputSource::Update()
{
    struct UpdateLocals
    {
        u16 currentBits;
        i32 bitIndex;
    } locals;

    locals.currentBits = this->historyCurrent;
    this->repeatOutput = 0;
    for (locals.bitIndex = 0; locals.bitIndex < 16;
         ++locals.bitIndex, locals.currentBits >>= 1)
    {
        if ((locals.currentBits & 1) != 0)
        {
            this->heldFrames[locals.bitIndex]++;
            if (this->heldFrames[locals.bitIndex] >= 26)
            {
                // Target behavior: this internal repeat field receives bit 0,
                // unlike Controller::GetInput's independently shifted mask.
                this->historyRepeat |= 1;
                this->heldFrames[locals.bitIndex] -= 8;
            }
        }
        else
        {
            this->heldFrames[locals.bitIndex] = 0;
        }
    }

    this->historyPressed =
        (this->historyCurrent ^ this->historyPrevious) & this->historyCurrent;
    this->historyReleased =
        (this->historyCurrent ^ this->historyPrevious) & ~this->historyCurrent;
}

} // namespace th095
