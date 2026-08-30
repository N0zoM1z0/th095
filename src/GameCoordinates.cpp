#include "ZunMath.hpp"

namespace th095
{

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position)
{
    output->x = position->x + 128.0f + 192.0f;
    output->y = position->y + 16.0f;
    output->z = position->z;
    return output;
}

} // namespace th095
