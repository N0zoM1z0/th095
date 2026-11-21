#ifndef TH095_DECOMPRESS_HPP
#define TH095_DECOMPRESS_HPP

#include "Global.hpp"

namespace th095
{

u8 *__fastcall DecompressData(u8 *input, i32 inputSize, u8 *output,
                              size_t outputSize);

} // namespace th095

#endif
