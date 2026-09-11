#include "ScoreData.hpp"
#include "Checksum.hpp"
#include "Decompress.hpp"
#include "Global.hpp"
#include "utils.hpp"
#include <stdlib.h>
#include <string.h>

namespace th095
{

#if !defined(TH095_MATCH_EXACT)
struct ScoreRecordHeaderView
{
    u16 magic;
    u16 version;
    u32 size;
    i32 checksum;
};

typedef char ScoreRecordHeaderSizeIs0C[
    (sizeof(ScoreRecordHeaderView) == 0x0c) ? 1 : -1];
#endif

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
#if defined(TH095_MATCH_EXACT)
        *reinterpret_cast<u16 *>(&this->fileHeader->unknown008) = 2;
#else
        this->fileHeader->version = 2;
#endif
        this->fileHeader->unknown00c = 0x102;
        goto finished;
    }
    else
    {
        if (this->fileHeader->magic != 0x35394854 ||
#if defined(TH095_MATCH_EXACT)
            *reinterpret_cast<u16 *>(&this->fileHeader->unknown008) != 2)
#else
            this->fileHeader->version != 2)
#endif
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
#if !defined(TH095_MATCH_EXACT)
            ScoreRecordHeaderView *recordHeader =
                reinterpret_cast<ScoreRecordHeaderView *>(cursor);
#endif
            if (
#if defined(TH095_MATCH_EXACT)
                *reinterpret_cast<u16 *>(cursor)
#else
                recordHeader->magic
#endif
                == 0x4353)
            {
                if (
#if defined(TH095_MATCH_EXACT)
                    *reinterpret_cast<u16 *>(cursor + 2)
#else
                    recordHeader->version
#endif
                    == 1)
                {
                    if (CalculateAlignedChecksum(reinterpret_cast<i32 *>(cursor), 0x60) -
#if defined(TH095_MATCH_EXACT)
                            *reinterpret_cast<i32 *>(cursor + 8) ==
                        *reinterpret_cast<i32 *>(cursor + 8))
#else
                            recordHeader->checksum ==
                        recordHeader->checksum)
#endif
                    {
#if defined(TH095_MATCH_EXACT)
                        this->scoreEntries[*reinterpret_cast<i32 *>(cursor + 0x0c)] =
                            *reinterpret_cast<ResultScoreEntryView *>(cursor);
#else
                        ResultScoreEntryView *scoreRecord =
                            reinterpret_cast<ResultScoreEntryView *>(cursor);
                        this->scoreEntries[scoreRecord->index] = *scoreRecord;
#endif
                    }
                }
            }
            else if (
#if defined(TH095_MATCH_EXACT)
                *reinterpret_cast<u16 *>(cursor)
#else
                recordHeader->magic
#endif
                == 0x5453)
            {
                if (
#if defined(TH095_MATCH_EXACT)
                    *reinterpret_cast<u16 *>(cursor + 2)
#else
                    recordHeader->version
#endif
                    == 0)
                {
                    if (CalculateAlignedChecksum(reinterpret_cast<i32 *>(cursor), 0x458) -
#if defined(TH095_MATCH_EXACT)
                            *reinterpret_cast<i32 *>(cursor + 8) ==
                        *reinterpret_cast<i32 *>(cursor + 8))
#else
                            recordHeader->checksum ==
                        recordHeader->checksum)
#endif
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

#if defined(TH095_MATCH_EXACT)
            remaining -= *reinterpret_cast<i32 *>(cursor + 4);
#else
            remaining -= recordHeader->size;
#endif
            if (remaining < 0)
            {
                utils::DebugPrint("error ScoreFile Data Error\n");
                goto initializeScoreFile;
            }
#if defined(TH095_MATCH_EXACT)
            cursor += *reinterpret_cast<i32 *>(cursor + 4);
#else
            cursor += recordHeader->size;
#endif
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
