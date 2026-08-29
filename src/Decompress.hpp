#ifndef TH095_DECOMPRESS_HPP
#define TH095_DECOMPRESS_HPP

#include "Global.hpp"

namespace th095
{

extern u8 g_DecompressionRing[0x2000];

u8 *__fastcall DecompressData(u8 *input, i32 inputSize, u8 *output,
                              size_t outputSize);

} // namespace th095

#endif
