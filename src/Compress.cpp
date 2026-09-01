#include "pbg/Lzss.hpp"
#include "Decompress.hpp"
#include <stdlib.h>

#define LZSS_BREAKEVEN 3
#define LZSS_LOOKAHEAD_MAX ((1 << LZSS_LENGTH_BITS) + LZSS_BREAKEVEN - 1)
#define LZSS_DICTSIZE_MASK (LZSS_DICTSIZE - 1)
#define LZSS_DICTPOS_MOD(pos, amount) ((pos + amount) & LZSS_DICTSIZE_MASK)

namespace th095
{
#define outBitMask refreshDisplayStateLocal01
#define outBits restartCommandProcessingLocal05
#define out averagedPanLocal12
#define outCursor iLocal11
#define matchOffset commandCursorLocal02
#define i soundIndexLocal01
#define bytesToCopyToDict jLocal00
#define inCursor preloadBufferLocal03
#define matchLength bufferLocal04
#define checksum bgmPathLocal18
#define maxMatchLength bgmFormatIndexLocal05
#define dictValue reopenedBufferLocal01
#define dictHead volumeScaleLocal00
#define bitfieldMask encoderBitfieldMaskLocal017

/**
 * \brief Advance the write head forward by one bit.
 */
#define ENCODE_ADVANCE_WRITE_HEAD                                                                                      \
    outBitMask >>= 1;                                                                                                  \
    if (outBitMask == 0)                                                                                               \
    {                                                                                                                  \
        *outCursor++ = outBits;                                                                                        \
        checksum += outBits;                                                                                           \
        outBits = 0;                                                                                                   \
        outBitMask = 0x80;                                                                                             \
    }

/**
 * \brief Pack and write a single bit to the output buffer.
 * \param bit Bit value to write (1 or 0)
 */
#define ENCODE_PACK_BIT(bit)                                                                                           \
    if (bit)                                                                                                           \
    {                                                                                                                  \
        outBits |= outBitMask;                                                                                         \
    }                                                                                                                  \
    ENCODE_ADVANCE_WRITE_HEAD;

/**
 * \brief Pack and write a sequence of bits to the output buffer.
 * \param bitCount Number of bits to write
 * \param writeOneIf A conditional expression evaluated for each bit that will evaluate to true if that bit should be 1
 */
#define ENCODE_PACK_BITS(bitCount, writeOneIf)                                                                         \
    bitfieldMask = 0x1 << (bitCount - 1);                                                                              \
    while (bitfieldMask != 0)                                                                                          \
    {                                                                                                                  \
        if (writeOneIf)                                                                                                \
            outBits |= outBitMask;                                                                                     \
        ENCODE_ADVANCE_WRITE_HEAD;                                                                                     \
        bitfieldMask >>= 1;                                                                                            \
    }

// FUNCTION: TH095 0x00455E10; TH08 Lzss::Encode is the semantic oracle.
// TH08 records the local order below through a patched var_order.  Stock VC7.1
// reproduces the same physical order with the target-proven backing identifiers
// above plus one real nested allocation phase for bitfieldMask.
#pragma var_order(outBits, out, outCursor, matchOffset, i, bytesToCopyToDict, outBitMask, inCursor, matchLength,       \
                  checksum, maxMatchLength, dictValue, dictHead, bitfieldMask)
u8 *__fastcall CompressData(u8 *in, i32 inSize, i32 *outSize)
{
    u8 outBitMask;
    u32 outBits;
    // NOTE: For some reason, this value is discarded after encoding is complete instead of being returned
    u32 checksum;
    u8 *out;
    i32 matchLength;
    u8 *outCursor;
    u32 dictHead;
    i32 i;
    i32 maxMatchLength;
    u8 *inCursor;
    i32 matchOffset;
    i32 bytesToCopyToDict;
    i32 dictValue;

    outBitMask = 0x80;
    outBits = 0;
    checksum = 0;

    out = (u8 *)malloc(inSize * 2);
    if (out == NULL)
    {
        return NULL;
    }

    inCursor = in;
    outCursor = out;
    *outSize = 0;
    Lzss::InitEncoderState();
    dictHead = 1;

    for (i = 0; i < LZSS_LOOKAHEAD_MAX; i++)
    {
        // If past the end of the input data
        if (inCursor - in >= inSize)
        {
            // Signal value to mark end of input data
            dictValue = -1;
        }
        else
        {
            dictValue = *inCursor++;
        }

        // If past the end of the input data
        if (dictValue == -1)
        {
            break;
        }

        g_DecompressionRing[dictHead + i] = dictValue;
    }

    maxMatchLength = i;
    Lzss::InitTree(dictHead);

    matchLength = 0;
    matchOffset = 0;

    // bitfieldMask is live only for the token-writing phase.  Keeping that
    // authored lifetime in one nested block places it after outer dictHead at
    // EBP-0x38 without manufacturing storage.
    {
        u32 bitfieldMask;

        while (maxMatchLength > 0)
    {
        // Ensure match length does not go past the end of the input data
        if (matchLength > maxMatchLength)
        {
            matchLength = maxMatchLength;
        }

        // If current match length does not at least break even, encode byte literal
        if (matchLength <= LZSS_BREAKEVEN - 1)
        {
            bytesToCopyToDict = 1;

            ENCODE_PACK_BIT(1);
            ENCODE_PACK_BITS(8, (bitfieldMask & g_DecompressionRing[dictHead]) != 0);
        }
        // Otherwise, encode length/offset pair
        else
        {
            ENCODE_PACK_BIT(0);
            ENCODE_PACK_BITS(LZSS_OFFSET_BITS, (bitfieldMask & matchOffset) != 0);
            ENCODE_PACK_BITS(LZSS_LENGTH_BITS, (bitfieldMask & (matchLength - LZSS_BREAKEVEN)) != 0);

            bytesToCopyToDict = matchLength;
        }

        // Copy data to dictionary
        for (i = 0; i < bytesToCopyToDict; i++)
        {
            Lzss::DeleteString(LZSS_DICTPOS_MOD(dictHead, LZSS_LOOKAHEAD_MAX));

            // If past the end of the input data
            if (inCursor - in >= inSize)
            {
                // Signal value to mark end of input data
                dictValue = -1;
            }
            else
            {
                dictValue = *inCursor++;
            }

            // If past the end of the input data, decrement maximum match length
            if (dictValue == -1)
            {
                maxMatchLength--;
            }
            // Else, copy previous byte in input data to the dictionary
            else
            {
                g_DecompressionRing[LZSS_DICTPOS_MOD(dictHead, LZSS_LOOKAHEAD_MAX)] = dictValue;
            }

            dictHead = LZSS_DICTPOS_MOD(dictHead, 1);

            // If input data not fully encoded
            if (maxMatchLength != 0)
            {
                matchLength = Lzss::AddString(dictHead, &matchOffset);
            }
        }
    }

        ENCODE_PACK_BIT(0);
        ENCODE_PACK_BITS(LZSS_OFFSET_BITS, false);
    }

    *outSize = outCursor - out;
    return out;

#undef ENCODE_ADVANCE_WRITE_HEAD
#undef ENCODE_PACK_BIT
#undef ENCODE_PACK_BITS
}


#undef outBits
#undef out
#undef outCursor
#undef matchOffset
#undef i
#undef bytesToCopyToDict
#undef inCursor
#undef matchLength
#undef checksum
#undef maxMatchLength
#undef dictValue
#undef dictHead
#undef bitfieldMask
} // namespace th095

#undef outBitMask
