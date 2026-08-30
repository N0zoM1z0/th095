#include "ScoreData.hpp"

#include "Checksum.hpp"
#include "FileSystem.hpp"
#include "SceneSelect.hpp"

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace th095
{

namespace ScoreFileWriter
{
ZunResult Open(char *path);
ZunResult Write(void *data, i32 size);
ZunResult Close();
} // namespace ScoreFileWriter

u8 *__fastcall CompressData(u8 *input, i32 inputSize, i32 *outputSize);

struct ScorePhotoStageView
{
    void CapturePhotoPixels(i32 photoIndex);
};

extern ScorePhotoStageView *g_ScorePhotoStage;

struct ScoreWriteLocals
{
    i32 bufferCapacity;
    u8 unknown004[8];
    ResultScoreEntryView *scoreEntry;
    u8 *rawCopyStart;
    u8 *compressedData;
    u32 scoreIndex;
    i32 pendingBestShotIndex;
    char path[0x100];
    i32 compressedPhotoSize;
    u8 *compressedPhotoData;
    i32 outputOffset;
    u8 *rawBuffer;
};

typedef char ScoreWriteLocalsSizeIs130[
    (sizeof(ScoreWriteLocals) == 0x130) ? 1 : -1];
typedef char ScoreWriteLocalsPathAt20[
    (offsetof(ScoreWriteLocals, path) == 0x20) ? 1 : -1];
typedef char ScoreWriteLocalsRawBufferAt12C[
    (offsetof(ScoreWriteLocals, rawBuffer) == 0x12c) ? 1 : -1];

ZunResult ResultSaveDataView::WriteBestShotData()
{
    ScoreWriteLocals locals;

    if (this->fileHeader == NULL)
    {
        return ZUN_ERROR;
    }

    _mkdir("bestshot");

    if (g_SelectedScene != NULL)
    {
        locals.pendingBestShotIndex = g_SelectedScene->scoreEntryIndex;
        if (this->bestShotRecords[locals.pendingBestShotIndex].valid != 0)
        {
            if (this->bestShotRecords[locals.pendingBestShotIndex].photoIndex >=
                0)
            {
                g_ScorePhotoStage->CapturePhotoPixels(
                    this->bestShotRecords[locals.pendingBestShotIndex]
                        .photoIndex);
            }

            locals.scoreEntry =
                &this->scoreEntries[locals.pendingBestShotIndex];
            locals.scoreEntry->magic = 0x4353;
            locals.scoreEntry->version = 1;
            locals.scoreEntry->size = sizeof(ResultScoreEntryView);
            this->scoreEntries[locals.pendingBestShotIndex]
                .bestShotChecksum =
                CalculateAlignedChecksum(
                    reinterpret_cast<i32 *>(
                        &this->bestShotRecords[locals.pendingBestShotIndex]),
                    0x18) +
                CalculateAlignedChecksum(
                    reinterpret_cast<i32 *>(
                        this->bestShotRecords[locals.pendingBestShotIndex]
                            .comment),
                    sizeof(this->bestShotRecords[0].comment)) +
                CalculateAlignedChecksum(
                    reinterpret_cast<i32 *>(
                        this->bestShotRecords[locals.pendingBestShotIndex]
                            .pixelData),
                    this->bestShotRecords[locals.pendingBestShotIndex].width *
                        this->bestShotRecords[locals.pendingBestShotIndex]
                            .height *
                        this->bestShotRecords[locals.pendingBestShotIndex]
                            .componentCount);

            if (g_SelectedScene->titleArgument1 != 10)
            {
                sprintf(locals.path, "bestshot/bs_%.2d_%d.dat",
                        g_SelectedScene->titleArgument1 + 1,
                        g_SelectedScene->titleArgument2 + 1);
            }
            else
            {
                sprintf(locals.path, "bestshot/bs_ex_%d.dat",
                        g_SelectedScene->titleArgument2 + 1);
            }

            ScoreFileWriter::Open(locals.path);
            ScoreFileWriter::Write(
                &this->bestShotRecords[locals.pendingBestShotIndex], 0x18);
            ScoreFileWriter::Write(
                this->bestShotRecords[locals.pendingBestShotIndex].comment,
                sizeof(this->bestShotRecords[0].comment));
            locals.compressedPhotoData = CompressData(
                this->bestShotRecords[locals.pendingBestShotIndex].pixelData,
                this->bestShotRecords[locals.pendingBestShotIndex].width *
                    this->bestShotRecords[locals.pendingBestShotIndex].height *
                    this->bestShotRecords[locals.pendingBestShotIndex]
                        .componentCount,
                &locals.compressedPhotoSize);
            ScoreFileWriter::Write(locals.compressedPhotoData,
                                   locals.compressedPhotoSize);
            ScoreFileWriter::Close();
            this->bestShotRecords[locals.pendingBestShotIndex].valid = 0;
            free(locals.compressedPhotoData);
            this->UpdateBestShotRecord(locals.pendingBestShotIndex);
        }
    }

    locals.bufferCapacity = 0x200000;
    locals.rawBuffer = reinterpret_cast<u8 *>(malloc(locals.bufferCapacity));
    locals.outputOffset = 0;
    *reinterpret_cast<ScoreFileHeader *>(locals.rawBuffer +
                                         locals.outputOffset) =
        *this->fileHeader;
    locals.outputOffset += sizeof(ScoreFileHeader);

    for (locals.scoreIndex = 0;
         locals.scoreIndex < ARRAY_SIZE(this->scoreEntries);
         locals.scoreIndex++)
    {
        if (this->scoreEntries[locals.scoreIndex].magic == 0x4353)
        {
            this->scoreEntries[locals.scoreIndex].index = locals.scoreIndex;
            this->scoreEntries[locals.scoreIndex].checksum = 0;
            this->scoreEntries[locals.scoreIndex].checksum =
                CalculateAlignedChecksum(
                    reinterpret_cast<i32 *>(
                        &this->scoreEntries[locals.scoreIndex]),
                    sizeof(ResultScoreEntryView));
            memcpy(locals.rawBuffer + locals.outputOffset,
                   &this->scoreEntries[locals.scoreIndex],
                   sizeof(ResultScoreEntryView));
            locals.outputOffset += sizeof(ResultScoreEntryView);
        }
    }

    this->profile.profileChecksum = 0;
    this->profile.profileChecksum = CalculateAlignedChecksum(
        reinterpret_cast<i32 *>(this->profileData), sizeof(this->profileData));
    memcpy(locals.rawBuffer + locals.outputOffset, this->profileData,
           sizeof(this->profileData));
    locals.outputOffset += sizeof(this->profileData);

    locals.rawCopyStart = locals.rawBuffer;
    this->fileHeader->uncompressedSize =
        locals.outputOffset - sizeof(ScoreFileHeader);
    locals.compressedData = CompressData(
        locals.rawCopyStart + sizeof(ScoreFileHeader),
        this->fileHeader->uncompressedSize,
        &this->fileHeader->compressedSize);
    this->fileHeader->fileSize =
        this->fileHeader->compressedSize + sizeof(ScoreFileHeader);
    FileSystem::Encrypt(
        locals.compressedData, this->fileHeader->compressedSize,
        0xac, 0x35, 0x10, this->fileHeader->compressedSize);

    if (ScoreFileWriter::Open("scoreth095.dat") != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }
    ScoreFileWriter::Write(this->fileHeader, sizeof(ScoreFileHeader));
    ScoreFileWriter::Write(locals.compressedData,
                           this->fileHeader->compressedSize);
    ScoreFileWriter::Close();
    free(locals.compressedData);
    free(locals.rawBuffer);
    return ZUN_SUCCESS;
}

} // namespace th095
