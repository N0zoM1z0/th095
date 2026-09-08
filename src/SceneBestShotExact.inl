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

struct SceneBestShotIoLocals
{
    char path[MAX_PATH];
    u8 *input;
};

typedef char SceneBestShotIoLocalsSizeIs108[
    (sizeof(SceneBestShotIoLocals) == 0x108) ? 1 : -1];

// The target keeps the pixel-allocation size temporary and hidden receiver
// eight bytes deeper than the four real loader locals.  This phase belongs
// only to the inlined pixel allocator; 0-byte and aggregate-wide controls
// preserve the wrong compiler-home class.
static __forceinline u8 *SceneBestShotPixelAlloc(size_t size)
{
    u8 compilerStorage[8];
    return reinterpret_cast<u8 *>(malloc(size));
}

// Stock VC7.1 ranks ordinary locals by identifier hash rather than source
// declaration order.  Keep the real path/input pair contiguous so the input
// field occupies the third shallow slot, while recordIndex remains a scalar:
// that distinction makes VC7 preserve recordIndex * 0x78 in ESI across malloc.
#define io iLocal11
#define recordIndex averagedPanLocal12
#define fileSize restartCommandProcessingLocal05

i32 SceneSaveDataView::LoadBestShotForScene(i32 group, i32 scene)
{
    SceneBestShotIoLocals io;
    i32 recordIndex;
    i32 fileSize;
    recordIndex = g_SceneGroups[group][scene].scoreEntryIndex;

    if (this->sceneScores[recordIndex].attemptCount == 0)
    {
        return -1;
    }

    reinterpret_cast<ResultSaveDataView *>(this)
        ->UpdateBestShotRecord(recordIndex);

    if (group != 10)
    {
        sprintf(io.path, "bestshot/bs_%.2d_%d.dat", group + 1, scene + 1);
    }
    else
    {
        sprintf(io.path, "bestshot/bs_ex_%d.dat", scene + 1);
    }

    if (!FileSystem::CheckIfFileAlreadyExists(io.path))
    {
        this->sceneScores[recordIndex].detailScore = 0;
        this->sceneScores[recordIndex].attemptCount = 0;
        return -1;
    }

    this->bestShotRecords[recordIndex].componentData0 =
        FileSystem::OpenFile(io.path, &fileSize, TRUE);
    if (this->bestShotRecords[recordIndex].componentData0 == NULL)
        goto load_failed;
    {
        memcpy(&this->bestShotRecords[recordIndex],
               this->bestShotRecords[recordIndex].componentData0, 0x18);
        io.input = reinterpret_cast<u8 *>(
                        this->bestShotRecords[recordIndex].componentData0) +
                    0x18;
        this->bestShotRecords[recordIndex].pixelData =
            SceneBestShotPixelAlloc(
                this->bestShotRecords[recordIndex].width *
                this->bestShotRecords[recordIndex].height *
                this->bestShotRecords[recordIndex].componentCount);

        if (this->bestShotRecords[recordIndex].type == 1)
        {
            DecompressData(
                io.input, fileSize - 0x18,
                this->bestShotRecords[recordIndex].pixelData,
                this->bestShotRecords[recordIndex].width *
                    this->bestShotRecords[recordIndex].height *
                    this->bestShotRecords[recordIndex].componentCount);
            memset(this->bestShotRecords[recordIndex].comment, 0,
                   sizeof(this->bestShotRecords[recordIndex].comment));
        }
        else
        {
            *reinterpret_cast<SceneBestShotCommentBlock *>(this->bestShotRecords[recordIndex].comment) =
                *reinterpret_cast<const SceneBestShotCommentBlock *>(io.input);
            io.input += sizeof(this->bestShotRecords[recordIndex].comment);
            DecompressData(
                io.input, fileSize - 0x68,
                this->bestShotRecords[recordIndex].pixelData,
                this->bestShotRecords[recordIndex].width *
                    this->bestShotRecords[recordIndex].height *
                    this->bestShotRecords[recordIndex].componentCount);
        }

        if (this->sceneScores[recordIndex].bestShotChecksum !=
            CalculateAlignedChecksum(
                reinterpret_cast<i32 *>(&this->bestShotRecords[recordIndex]), 0x18) +
            CalculateAlignedChecksum(
                reinterpret_cast<i32 *>(this->bestShotRecords[recordIndex].comment), 0x50) +
            CalculateAlignedChecksum(
                reinterpret_cast<i32 *>(this->bestShotRecords[recordIndex].pixelData),
                this->bestShotRecords[recordIndex].width *
                    this->bestShotRecords[recordIndex].height *
                    this->bestShotRecords[recordIndex].componentCount))
        {
            utils::DebugPrint("Best Shot Sum Check Error\n");
        }
        else
        {
            this->bestShotRecords[recordIndex].componentsLoaded = 1;
            return 0;
        }
    }
load_failed:
    this->bestShotRecords[recordIndex].componentsLoaded = 0;
    return -1;
}
#undef fileSize
#undef recordIndex
#undef io

} // namespace th095
