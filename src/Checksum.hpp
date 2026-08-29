#ifndef TH095_CHECKSUM_HPP
#define TH095_CHECKSUM_HPP

#include "inttypes.hpp"

namespace th095
{

i32 __fastcall CalculateAlignedChecksum(i32 *data, u32 size);

} // namespace th095

#endif
