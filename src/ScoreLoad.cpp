#include "ScoreData.hpp"
#include "Checksum.hpp"
#include "Decompress.hpp"
#include "Global.hpp"
#include "utils.hpp"
#include <stdlib.h>
#include <string.h>

namespace th095
{

i32 ResultSaveDataView::ParseScoreFile()
{
    struct ScoreLoadState
    {
        size_t allocationSize;
        size_t headerSize;
        void *ownedFile;
        ScoreRecordHeaderView *cursor;
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
        this->fileHeader->magic = SCORE_FILE_MAGIC;
        this->fileHeader->version = SCORE_FILE_VERSION;
        this->fileHeader->unknown00c = 0x102;
        goto finished;
    }
    else
    {
        if (this->fileHeader->magic != SCORE_FILE_MAGIC ||
            this->fileHeader->version != SCORE_FILE_VERSION)
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
        cursor = reinterpret_cast<ScoreRecordHeaderView *>(
            this->decompressedData);
        while (remaining > 0)
        {
            if (cursor->magic == SCENE_SCORE_RECORD_MAGIC)
            {
                if (cursor->version == SCENE_SCORE_RECORD_VERSION)
                {
                    if (CalculateAlignedChecksum(
                            reinterpret_cast<i32 *>(cursor),
                            sizeof(ResultScoreEntryView)) -
                            cursor->checksum == cursor->checksum)
                    {
                        this->scoreEntries[
                            reinterpret_cast<ResultScoreEntryView *>(cursor)
                                ->index] =
                            *reinterpret_cast<ResultScoreEntryView *>(cursor);
                    }
                }
            }
            else if (cursor->magic == SCORE_PROFILE_RECORD_MAGIC)
            {
                if (cursor->version == SCORE_PROFILE_RECORD_VERSION)
                {
                    if (CalculateAlignedChecksum(
                            reinterpret_cast<i32 *>(cursor),
                            sizeof(ScoreProfileView)) -
                            cursor->checksum == cursor->checksum)
                    {
                        this->profile =
                            *reinterpret_cast<ScoreProfileView *>(cursor);
                    }
                }
            }
            else
            {
                utils::DebugPrint("error ScoreFile Data Error\n");
                goto initializeScoreFile;
            }

            remaining -= cursor->size;
            if (remaining < 0)
            {
                utils::DebugPrint("error ScoreFile Data Error\n");
                goto initializeScoreFile;
            }
            cursor = reinterpret_cast<ScoreRecordHeaderView *>(
                reinterpret_cast<u8 *>(cursor) + cursor->size);
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
