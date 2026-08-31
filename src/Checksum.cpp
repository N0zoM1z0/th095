#include "Checksum.hpp"

namespace th095
{

// FUNCTION: TH095 0x0041BA30. PBG uses this byte checksum to select one of
// eight per-entry decryption profiles.
u8 __fastcall CalculateByteChecksum(u8 *data, i32 size)
{
    u8 *cursor = data;
    u8 checksum = 0;
    while (size-- != 0)
        checksum += *cursor++;
    return checksum;
}

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
