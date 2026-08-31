#ifndef TH095_CHECKSUM_HPP
#define TH095_CHECKSUM_HPP

#include "inttypes.hpp"

namespace th095
{

u8 __fastcall CalculateByteChecksum(u8 *data, i32 size);
i32 __fastcall CalculateAlignedChecksum(i32 *data, u32 size);

} // namespace th095

#endif
