#ifndef TH095_ANM_WIRE_FORMAT_HPP
#define TH095_ANM_WIRE_FORMAT_HPP

#include "inttypes.hpp"
#include <stddef.h>

namespace th095
{

// ANM version 4 entry. Offsets are measured from the beginning of this record.
struct AnmRawEntryView
{
    i32 numSprites;
    i32 numScripts;
    u32 textureIdx;
    i32 width;
    i32 height;
    u32 format;
    u32 colorKey;
    u32 nameOffset;
    u32 spriteIdxOffset;
    u32 mipmapNameOffset;
    u32 version;
    u32 priority;
    u32 textureOffset;
    u8 hasData;
    u8 padding035[3];
    u32 nextOffset;
    u32 serializedReserved03c;
};

// The THTX pixel data starts immediately after this header.
struct AnmTextureHeaderView
{
    char magic[4];
    u16 serializedReserved004;
    i16 format;
    i16 width;
    i16 height;
    u16 serializedReserved00c;
    u16 serializedReserved00e;
};

typedef char AnmRawEntryViewTextureAt30[
    (offsetof(AnmRawEntryView, textureOffset) == 0x30) ? 1 : -1];
typedef char AnmRawEntryViewNextAt38[(offsetof(AnmRawEntryView, nextOffset) == 0x38) ? 1 : -1];
typedef char AnmRawEntryViewSizeIs40[(sizeof(AnmRawEntryView) == 0x40) ? 1 : -1];
typedef char AnmTextureHeaderViewSizeIs10[(sizeof(AnmTextureHeaderView) == 0x10) ? 1 : -1];

} // namespace th095

#endif
