#include "Decompress.hpp"

#include <stdlib.h>

namespace th095
{

#define READ_COMPRESSED_BIT(weight, value)                                   \
    if (bitMask == 0x80)                                                      \
    {                                                                         \
        currentByte = *inputCursor;                                           \
        if (inputCursor - input < inputSize)                                  \
        {                                                                     \
            inputCursor++;                                                    \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            currentByte = 0;                                                  \
        }                                                                     \
    }                                                                         \
    if ((bitMask & currentByte) != 0)                                         \
    {                                                                         \
        value |= weight;                                                      \
    }                                                                         \
    bitMask >>= 1;                                                            \
    if (bitMask == 0)                                                         \
    {                                                                         \
        bitMask = 0x80;                                                       \
    }

u8 *__fastcall DecompressData(u8 *input, i32 inputSize, u8 *output,
                              size_t outputSize)
{
    u8 bitMask = 0x80;
    u8 currentByte = 0;

    if (output == NULL)
    {
        output = reinterpret_cast<u8 *>(malloc(outputSize));
        if (output == NULL)
        {
            return NULL;
        }
    }

    u8 *outputCursor = output;
    u32 ringWrite = 1;
    u8 *inputCursor = input;
    for (;;)
    {
        u32 literalFlag = 0;
        READ_COMPRESSED_BIT(1, literalFlag);
        if (literalFlag != 0)
        {
            u32 value = 0;
            for (u32 weight = 0x80; weight != 0; weight >>= 1)
            {
                READ_COMPRESSED_BIT(weight, value);
            }
            *outputCursor++ = (u8)value;
            g_DecompressionRing[ringWrite] = (u8)value;
            ringWrite = (ringWrite + 1) & 0x1fff;
        }
        else
        {
            u32 offset = 0;
            for (u32 weight = 0x1000; weight != 0; weight >>= 1)
            {
                READ_COMPRESSED_BIT(weight, offset);
            }
            if (offset == 0)
            {
                break;
            }

            u32 length = 0;
            for (u32 weight = 8; weight != 0; weight >>= 1)
            {
                READ_COMPRESSED_BIT(weight, length);
            }
            for (i32 i = 0; i <= (i32)length + 2; i++)
            {
                u8 value = g_DecompressionRing[(offset + i) & 0x1fff];
                *outputCursor++ = value;
                g_DecompressionRing[ringWrite] = value;
                ringWrite = (ringWrite + 1) & 0x1fff;
            }
        }
    }

    while (bitMask != 0x80)
    {
        if (bitMask == 0x80 && inputCursor - input < inputSize)
        {
            inputCursor++;
        }
        bitMask >>= 1;
        if (bitMask == 0)
        {
            bitMask = 0x80;
        }
    }
    return output;
}

#undef READ_COMPRESSED_BIT

} // namespace th095
