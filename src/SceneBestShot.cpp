#include "SceneSelect.hpp"

#include "Checksum.hpp"
#include "Decompress.hpp"
#include "FileSystem.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace th095
{

i32 SceneSaveDataView::LoadBestShotForScene(i32 group, i32 scene)
{
    i32 recordIndex = g_SceneGroups[group][scene].scoreEntryIndex;
    SceneScoreEntryView *scoreEntry = &this->sceneScores[recordIndex];

    if (scoreEntry->attemptCount != 0)
    {
        reinterpret_cast<ResultSaveDataView *>(this)
            ->UpdateBestShotRecord(recordIndex);

        char path[MAX_PATH];
        if (group == 10)
        {
            sprintf(path, "bestshot/bs_ex_%d.dat", scene + 1);
        }
        else
        {
            sprintf(path, "bestshot/bs_%.2d_%d.dat", group + 1,
                    scene + 1);
        }

        if (!FileSystem::CheckIfFileAlreadyExists(path))
        {
            scoreEntry->detailScore = 0;
            scoreEntry->attemptCount = 0;
        }
        else
        {
            i32 fileSize;
            ResultBestShotRecordView *record =
                &this->bestShotRecords[recordIndex];
            record->componentData0 = FileSystem::OpenFile(path, &fileSize,
                                                          TRUE);
            if (record->componentData0 != NULL)
            {
                memcpy(record, record->componentData0, 0x18);
                u8 *input =
                    reinterpret_cast<u8 *>(record->componentData0) + 0x18;
                size_t pixelSize = record->width * record->height *
                                   record->componentCount;
                record->pixelData = reinterpret_cast<u8 *>(malloc(pixelSize));

                if (record->type == 1)
                {
                    DecompressData(input, fileSize - 0x18,
                                   record->pixelData, pixelSize);
                    memset(record->comment, 0, sizeof(record->comment));
                }
                else
                {
                    memcpy(record->comment, input, sizeof(record->comment));
                    input += sizeof(record->comment);
                    DecompressData(input, fileSize - 0x68,
                                   record->pixelData, pixelSize);
                }

                i32 headerChecksum = CalculateAlignedChecksum(
                    reinterpret_cast<i32 *>(record), 0x18);
                i32 commentChecksum = CalculateAlignedChecksum(
                    reinterpret_cast<i32 *>(record->comment), 0x50);
                i32 imageChecksum = CalculateAlignedChecksum(
                    reinterpret_cast<i32 *>(record->pixelData), pixelSize);
                if (scoreEntry->bestShotChecksum ==
                    headerChecksum + commentChecksum + imageChecksum)
                {
                    record->valid = 1;
                    return 0;
                }
                utils::DebugPrint("Best Shot Sum Check Error\n");
            }
            record->valid = 0;
        }
    }
    return -1;
}

} // namespace th095
