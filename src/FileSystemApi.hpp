#pragma once

#include "inttypes.hpp"

#include <stddef.h>
#include <windows.h>

namespace th095
{
namespace FileSystem
{

u8 *Decrypt(u8 *data, i32 size, u8 xorValue, u8 xorValueInc, i32 chunkSize, i32 maxBytes);
u8 *TryDecryptFromTable(u8 *data, i32 *unused, i32 size);
u8 *Encrypt(u8 *data, i32 size, u8 xorValue, u8 xorValueInc, i32 chunkSize, i32 maxBytes);
u8 *OpenFile(const char *path, i32 *fileSize, BOOL loadFromDisk);
BOOL CheckIfFileAlreadyExists(const char *path);

i32 WriteDataToFile(const char *path, void *data, size_t size);
i32 OpenWriteFile(char *path);
i32 WriteToOpenFile(void *data, u32 size);
i32 CloseWriteFile();

} // namespace FileSystem
} // namespace th095
