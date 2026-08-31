#include "Decompress.hpp"

#include <stdlib.h>

namespace th095
{

// TH08's source records a patched var_order for this decoder. TH095 retains
// that live-local order, including the otherwise discarded input checksum.
// Stock VC7.1 reproduces it by using identifier buckets already proven by
// canonical units elsewhere in the target. All aliases below back real locals;
// there is no padding or inert storage.
#define currByte restartCommandProcessingLocal05
#define outCursor averagedPanLocal12
#define matchOffset iLocal11
#define i commandCursorLocal02
#define inCursor soundIndexLocal01
#define inBits jLocal00
#define size preloadBufferLocal03
#define matchLength bufferLocal04
#define checksum bgmPathLocal18
#define dictValue bgmFormatIndexLocal05
#define outBitMask reopenedBufferLocal01
#define dictHead volumeScaleLocal00

#define LZSS_OFFSET_BITS 13
#define LZSS_LENGTH_BITS 4
#define LZSS_BREAKEVEN 3
#define LZSS_DICTSIZE (1 << LZSS_OFFSET_BITS)
#define LZSS_DICTSIZE_MASK (LZSS_DICTSIZE - 1)
#define LZSS_DICTPOS_MOD(pos, amount) (((pos) + (amount)) & LZSS_DICTSIZE_MASK)

#define DECODE_ADVANCE_READ_HEAD                                                                                       \
    inBitMask >>= 1;                                                                                                   \
    if (inBitMask == 0)                                                                                                \
    {                                                                                                                  \
        inBitMask = 0x80;                                                                                              \
    }

#define DECODE_WRITE_BYTE(data)                                                                                        \
    *outCursor++ = (u8)(data);                                                                                         \
    g_DecompressionRing[dictHead] = (u8)(data);                                                                        \
    dictHead = LZSS_DICTPOS_MOD(dictHead, 1);

#define DECODE_HANDLE_FETCH                                                                                            \
    if (inBitMask == 0x80)                                                                                             \
    {                                                                                                                  \
        currByte = *inCursor;                                                                                          \
        if (inCursor - in >= size)                                                                                     \
        {                                                                                                              \
            currByte = 0;                                                                                              \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            inCursor++;                                                                                                \
        }                                                                                                              \
        checksum += currByte;                                                                                          \
    }

#define DECODE_UNPACK_BIT                                                                                              \
    DECODE_HANDLE_FETCH;                                                                                               \
    inBits = currByte & inBitMask;                                                                                     \
    DECODE_ADVANCE_READ_HEAD;

#define DECODE_UNPACK_BITS(bitCount)                                                                                   \
    outBitMask = 0x1 << ((bitCount) - 1);                                                                              \
    inBits = 0;                                                                                                        \
    while (outBitMask != 0)                                                                                            \
    {                                                                                                                  \
        DECODE_HANDLE_FETCH;                                                                                           \
        if ((currByte & inBitMask) != 0)                                                                               \
            inBits |= outBitMask;                                                                                      \
        outBitMask >>= 1;                                                                                              \
        DECODE_ADVANCE_READ_HEAD;                                                                                      \
    }

// FUNCTION: TH095 0x00456220; TH08 Lzss::Decode is the source-shape oracle.
u8 *__fastcall DecompressData(u8 *in, i32 inSize, u8 *out, size_t outSize)
{
    u8 inBitMask;
    u32 currByte;
    u32 checksum;
    // Declaration order is codegen-significant under stock VC7.1: the target
    // puts matchLength one dword deeper than the copied input size.
    i32 matchLength;
    u8 *inCursor, *outCursor;
    u32 dictHead;
    u32 inBits;
    i32 matchOffset;
    i32 size;
    i32 i;
    u32 dictValue;
    u32 outBitMask;

    inBitMask = 0x80;
    currByte = 0;
    checksum = 0;
    size = inSize;

    if (out == NULL)
    {
        out = (u8 *)malloc(outSize);
        if (out == NULL)
            return NULL;
    }

    inCursor = in;
    outCursor = out;
    dictHead = 1;

    for (;;)
    {
        DECODE_UNPACK_BIT;
        if (inBits != 0)
        {
            DECODE_UNPACK_BITS(8);
            DECODE_WRITE_BYTE(inBits);
        }
        else
        {
            DECODE_UNPACK_BITS(13);
            matchOffset = inBits;
            if (matchOffset == 0)
                break;

            DECODE_UNPACK_BITS(4);
            matchLength = inBits + 2;
            for (i = 0; i <= matchLength; i++)
            {
                dictValue = g_DecompressionRing[LZSS_DICTPOS_MOD(matchOffset, i)];
                DECODE_WRITE_BYTE(dictValue);
            }
        }
    }

    // The original consumes the remainder of the current input byte through
    // the same fetch/checksum path rather than merely shifting the bit mask.
    while (inBitMask != 0x80)
    {
        DECODE_UNPACK_BIT;
    }
    return out;
}

#undef DECODE_ADVANCE_READ_HEAD
#undef DECODE_WRITE_BYTE
#undef DECODE_HANDLE_FETCH
#undef DECODE_UNPACK_BIT
#undef DECODE_UNPACK_BITS
#undef LZSS_DICTPOS_MOD
#undef LZSS_DICTSIZE_MASK
#undef LZSS_DICTSIZE
#undef LZSS_BREAKEVEN
#undef LZSS_LENGTH_BITS
#undef LZSS_OFFSET_BITS
#undef currByte
#undef outCursor
#undef matchOffset
#undef i
#undef inCursor
#undef inBits
#undef size
#undef matchLength
#undef checksum
#undef dictValue
#undef outBitMask
#undef dictHead

} // namespace th095
