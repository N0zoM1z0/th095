#include "ScoreData.hpp"
#include "GameplayGlobals.hpp"

#include "Checksum.hpp"
#include "Main.hpp"
#include "SceneData.hpp"
#include "PhotoStage.hpp"

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace th095
{

u8 *__fastcall CompressData(u8 *input, i32 inputSize, i32 *outputSize);


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
                g_PhotoStageState->CapturePhotoPixels(
                    this->bestShotRecords[locals.pendingBestShotIndex]
                        .photoIndex);
            }

            locals.scoreEntry =
                &this->scoreEntries[locals.pendingBestShotIndex];
            locals.scoreEntry->magic = SCENE_SCORE_RECORD_MAGIC;
            locals.scoreEntry->version = SCENE_SCORE_RECORD_VERSION;
            locals.scoreEntry->size = sizeof(ResultScoreEntryView);
            this->scoreEntries[locals.pendingBestShotIndex]
                .bestShotChecksum =
                CalculateAlignedChecksum(
                    reinterpret_cast<i32 *>(
                        &this->bestShotRecords[locals.pendingBestShotIndex]),
                    RESULT_BEST_SHOT_FILE_HEADER_SIZE) +
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

            if (g_SelectedScene->group != 10)
            {
                sprintf(locals.path, "bestshot/bs_%.2d_%d.dat",
                        g_SelectedScene->group + 1,
                        g_SelectedScene->scene + 1);
            }
            else
            {
                sprintf(locals.path, "bestshot/bs_ex_%d.dat",
                        g_SelectedScene->scene + 1);
            }

            FileSystem::OpenWriteFile(locals.path);
            FileSystem::WriteToOpenFile(
                &this->bestShotRecords[locals.pendingBestShotIndex],
                RESULT_BEST_SHOT_FILE_HEADER_SIZE);
            FileSystem::WriteToOpenFile(
                this->bestShotRecords[locals.pendingBestShotIndex].comment,
                sizeof(this->bestShotRecords[0].comment));
            locals.compressedPhotoData = CompressData(
                this->bestShotRecords[locals.pendingBestShotIndex].pixelData,
                this->bestShotRecords[locals.pendingBestShotIndex].width *
                    this->bestShotRecords[locals.pendingBestShotIndex].height *
                    this->bestShotRecords[locals.pendingBestShotIndex]
                        .componentCount,
                &locals.compressedPhotoSize);
            FileSystem::WriteToOpenFile(locals.compressedPhotoData,
                                        locals.compressedPhotoSize);
            FileSystem::CloseWriteFile();
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
        if (this->scoreEntries[locals.scoreIndex].magic == SCENE_SCORE_RECORD_MAGIC)
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

    this->profile.recordHeader.checksum = 0;
    this->profile.recordHeader.checksum = CalculateAlignedChecksum(
        reinterpret_cast<i32 *>(&this->profile), sizeof(this->profile));
    memcpy(locals.rawBuffer + locals.outputOffset, &this->profile,
           sizeof(this->profile));
    locals.outputOffset += sizeof(this->profile);

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

    if (FileSystem::OpenWriteFile("scoreth095.dat") != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }
    FileSystem::WriteToOpenFile(this->fileHeader, sizeof(ScoreFileHeader));
    FileSystem::WriteToOpenFile(locals.compressedData,
                                this->fileHeader->compressedSize);
    FileSystem::CloseWriteFile();
    free(locals.compressedData);
    free(locals.rawBuffer);
    return ZUN_SUCCESS;
}

} // namespace th095
