#include "SceneSelect.hpp"

#include "Checksum.hpp"
#include "Decompress.hpp"
#include "FileSystem.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace th095
{

struct SceneBestShotCommentBlock { u32 words[20]; };

struct SceneBestShotLoadLocals
{
    char path[MAX_PATH];
    u8 *input;
    i32 recordIndex;
    i32 fileSize;
};

typedef char SceneBestShotLoadLocalsSizeIs110[
    (sizeof(SceneBestShotLoadLocals) == 0x110) ? 1 : -1];

i32 SceneSaveDataView::LoadBestShotForScene(i32 group, i32 scene)
{
    SceneBestShotLoadLocals locals;
    locals.recordIndex = g_SceneGroups[group][scene].scoreEntryIndex;

    if (this->sceneScores[locals.recordIndex].attemptCount == 0)
    {
        return -1;
    }

    reinterpret_cast<ResultSaveDataView *>(this)
        ->UpdateBestShotRecord(locals.recordIndex);

    if (group != 10)
    {
        sprintf(locals.path, "bestshot/bs_%.2d_%d.dat", group + 1, scene + 1);
    }
    else
    {
        sprintf(locals.path, "bestshot/bs_ex_%d.dat", scene + 1);
    }

    if (!FileSystem::CheckIfFileAlreadyExists(locals.path))
    {
        this->sceneScores[locals.recordIndex].detailScore = 0;
        this->sceneScores[locals.recordIndex].attemptCount = 0;
        return -1;
    }

    this->bestShotRecords[locals.recordIndex].componentData0 =
        FileSystem::OpenFile(locals.path, &locals.fileSize, TRUE);
    if (this->bestShotRecords[locals.recordIndex].componentData0 == NULL)
        goto load_failed;
    {
        memcpy(&this->bestShotRecords[locals.recordIndex],
               this->bestShotRecords[locals.recordIndex].componentData0, 0x18);
        locals.input = reinterpret_cast<u8 *>(
                        this->bestShotRecords[locals.recordIndex].componentData0) +
                    0x18;
        this->bestShotRecords[locals.recordIndex].pixelData =
            reinterpret_cast<u8 *>(g_ZunMemory.Alloc(
                this->bestShotRecords[locals.recordIndex].width *
                this->bestShotRecords[locals.recordIndex].height *
                this->bestShotRecords[locals.recordIndex].componentCount));

        if (this->bestShotRecords[locals.recordIndex].type == 1)
        {
            DecompressData(
                locals.input, locals.fileSize - 0x18,
                this->bestShotRecords[locals.recordIndex].pixelData,
                this->bestShotRecords[locals.recordIndex].width *
                    this->bestShotRecords[locals.recordIndex].height *
                    this->bestShotRecords[locals.recordIndex].componentCount);
            memset(this->bestShotRecords[locals.recordIndex].comment, 0,
                   sizeof(this->bestShotRecords[locals.recordIndex].comment));
        }
        else
        {
            *reinterpret_cast<SceneBestShotCommentBlock *>(this->bestShotRecords[locals.recordIndex].comment) =
                *reinterpret_cast<const SceneBestShotCommentBlock *>(locals.input);
            locals.input += sizeof(this->bestShotRecords[locals.recordIndex].comment);
            DecompressData(
                locals.input, locals.fileSize - 0x68,
                this->bestShotRecords[locals.recordIndex].pixelData,
                this->bestShotRecords[locals.recordIndex].width *
                    this->bestShotRecords[locals.recordIndex].height *
                    this->bestShotRecords[locals.recordIndex].componentCount);
        }

        if (this->sceneScores[locals.recordIndex].bestShotChecksum !=
            CalculateAlignedChecksum(
                reinterpret_cast<i32 *>(&this->bestShotRecords[locals.recordIndex]), 0x18) +
            CalculateAlignedChecksum(
                reinterpret_cast<i32 *>(this->bestShotRecords[locals.recordIndex].comment), 0x50) +
            CalculateAlignedChecksum(
                reinterpret_cast<i32 *>(this->bestShotRecords[locals.recordIndex].pixelData),
                this->bestShotRecords[locals.recordIndex].width *
                    this->bestShotRecords[locals.recordIndex].height *
                    this->bestShotRecords[locals.recordIndex].componentCount))
        {
            utils::DebugPrint("Best Shot Sum Check Error\n");
        }
        else
        {
            this->bestShotRecords[locals.recordIndex].componentsLoaded = 1;
            return 0;
        }
    }
load_failed:
    this->bestShotRecords[locals.recordIndex].componentsLoaded = 0;
    return -1;
}

} // namespace th095
