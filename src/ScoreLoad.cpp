#include "ScoreData.hpp"
#include "Checksum.hpp"
#include "Decompress.hpp"
#include "Global.hpp"
#include "utils.hpp"
#include <stdlib.h>
#include <string.h>

namespace th095
{

struct ScoreProfileRawView
{
    u8 bytes[0x458];
};

i32 ResultSaveDataView::ParseScoreFile()
{
    struct ScoreLoadState
    {
        size_t allocationSize;
        size_t headerSize;
        void *ownedFile;
        u8 *cursor;
        i32 remaining;
        u8 *compressedInput;
    } locals;
#define allocationSize locals.allocationSize
#define headerSize locals.headerSize
#define ownedFile locals.ownedFile
#define cursor locals.cursor
#define remaining locals.remaining
#define compressedInput locals.compressedInput

    if (this->fileHeader == NULL)
    {
initializeScoreFile:
        utils::DebugPrint("Init ScoreFile\n");
        if (this->fileHeader != NULL)
        {
            ownedFile = this->fileHeader;
            free(ownedFile);
        }
        headerSize = sizeof(ScoreFileHeader);
        this->fileHeader = reinterpret_cast<ScoreFileHeader *>(malloc(headerSize));
        memset(this->fileHeader, 0, sizeof(ScoreFileHeader));
        this->fileHeader->magic = 0x35394854;
        *reinterpret_cast<u16 *>(&this->fileHeader->unknown008) = 2;
        this->fileHeader->unknown00c = 0x102;
        goto finished;
    }
    else
    {
        if (this->fileHeader->magic != 0x35394854 ||
            *reinterpret_cast<u16 *>(&this->fileHeader->unknown008) != 2)
        {
            utils::DebugPrint("error ScoreFile Version Error\n");
            goto initializeScoreFile;
        }

        FileSystem::Decrypt(
            reinterpret_cast<u8 *>(this->fileHeader) + sizeof(ScoreFileHeader),
            this->fileHeader->compressedSize,
            0xac, 0x35, 0x10, this->fileHeader->compressedSize);
        compressedInput = reinterpret_cast<u8 *>(this->fileHeader) + sizeof(ScoreFileHeader);
        allocationSize = this->fileHeader->uncompressedSize * 4;
        this->decompressedData = reinterpret_cast<u8 *>(malloc(allocationSize));
        DecompressData(compressedInput, this->fileHeader->compressedSize,
                       this->decompressedData, this->fileHeader->uncompressedSize);

        remaining = this->fileHeader->uncompressedSize;
        cursor = this->decompressedData;
        while (remaining > 0)
        {
            if (*reinterpret_cast<u16 *>(cursor) == 0x4353)
            {
                if (*reinterpret_cast<u16 *>(cursor + 2) == 1)
                {
                    if (CalculateAlignedChecksum(reinterpret_cast<i32 *>(cursor), 0x60) -
                            *reinterpret_cast<i32 *>(cursor + 8) ==
                        *reinterpret_cast<i32 *>(cursor + 8))
                    {
                        this->scoreEntries[*reinterpret_cast<i32 *>(cursor + 0x0c)] =
                            *reinterpret_cast<ResultScoreEntryView *>(cursor);
                    }
                }
            }
            else if (*reinterpret_cast<u16 *>(cursor) == 0x5453)
            {
                if (*reinterpret_cast<u16 *>(cursor + 2) == 0)
                {
                    if (CalculateAlignedChecksum(reinterpret_cast<i32 *>(cursor), 0x458) -
                            *reinterpret_cast<i32 *>(cursor + 8) ==
                        *reinterpret_cast<i32 *>(cursor + 8))
                    {
                        *reinterpret_cast<ScoreProfileRawView *>(this->profileData) =
                            *reinterpret_cast<ScoreProfileRawView *>(cursor);
                    }
                }
            }
            else
            {
                utils::DebugPrint("error ScoreFile Data Error\n");
                goto initializeScoreFile;
            }

            remaining -= *reinterpret_cast<i32 *>(cursor + 4);
            if (remaining < 0)
            {
                utils::DebugPrint("error ScoreFile Data Error\n");
                goto initializeScoreFile;
            }
            cursor += *reinterpret_cast<i32 *>(cursor + 4);
        }
    }

finished:
    return 0;
#undef compressedInput
#undef remaining
#undef cursor
#undef ownedFile
#undef headerSize
#undef allocationSize
}

} // namespace th095
