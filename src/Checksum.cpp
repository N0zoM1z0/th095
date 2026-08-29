#include "Checksum.hpp"

namespace th095
{

i32 __fastcall CalculateAlignedChecksum(i32 *data, u32 size)
{
    i32 *current = data;
    i32 checksum = 0;

    size -= size & 3;
    while ((i32)size > 0)
    {
        checksum += *current;
        size -= 4;
        current++;
    }
    return checksum;
}

} // namespace th095
