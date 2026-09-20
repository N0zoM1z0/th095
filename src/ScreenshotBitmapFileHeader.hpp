#pragma once

#include <stddef.h>

#include "inttypes.hpp"

namespace th095
{

// Serialized 14-byte BMP file header embedded in Supervisor at +0x52C. This
// is distinct from the adjacent Win32 BITMAPINFOHEADER allocation.
#pragma pack(push, 1)
struct ScreenshotBitmapFileHeader
{
    u16 type;
    u32 size;
    u16 reserved1;
    u16 reserved2;
    u32 offBits;
};
#pragma pack(pop)

typedef char ScreenshotBitmapFileHeaderSizeIs0E[
    (sizeof(ScreenshotBitmapFileHeader) == 0x0e) ? 1 : -1];
typedef char ScreenshotBitmapFileHeaderSizeAt02[
    (offsetof(ScreenshotBitmapFileHeader, size) == 0x02) ? 1 : -1];
typedef char ScreenshotBitmapFileHeaderReserved1At06[
    (offsetof(ScreenshotBitmapFileHeader, reserved1) == 0x06) ? 1 : -1];
typedef char ScreenshotBitmapFileHeaderReserved2At08[
    (offsetof(ScreenshotBitmapFileHeader, reserved2) == 0x08) ? 1 : -1];
typedef char ScreenshotBitmapFileHeaderOffBitsAt0A[
    (offsetof(ScreenshotBitmapFileHeader, offBits) == 0x0a) ? 1 : -1];

} // namespace th095
