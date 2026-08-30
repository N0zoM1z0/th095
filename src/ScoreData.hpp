#ifndef TH095_SCORE_DATA_HPP
#define TH095_SCORE_DATA_HPP

#include "Global.hpp"

namespace th095
{

struct ScoreFileHeader
{
    u32 magic;
    u32 fileSize;
    u32 unknown008;
    u32 unknown00c;
    i32 compressedSize;
    i32 uncompressedSize;
};

struct ResultBestShotImageView
{
    i32 score;
    u8 unknown004[4];
    u32 metadata[8];
    u8 unknown028[4];
    i32 replayValue;
    u8 unknown030[0x38 - 0x30];
    f32 slowRate;
    i32 stageValue;
    u8 unknown040[0x60 - 0x40];
};

struct ResultBestShotRecordView
{
    u32 magic;
    u8 type;
    u8 componentCount;
    u16 group;
    u16 scene;
    u16 version;
    u16 width;
    u16 height;
    i32 score;
    u8 unknown014[4];
    char comment[0x50];
    u8 valid;
    u8 componentsLoaded;
    u8 unknown06a[2];
    i32 photoIndex;
    void *componentData0;
    u8 *pixelData;
};

struct ResultScoreEntryView
{
    u16 magic;
    u16 version;
    u32 size;
    i32 checksum;
    i32 index;
    i32 score;
    u8 unknown014[0x18 - 0x14];
    i32 detailScore;
    u8 unknown01c[0x3c - 0x1c];
    i32 attemptCount;
    i32 bestShotChecksum;
    i32 unlockScore;
    f32 slowRate;
    f32 successRate;
    u32 flags;
    u8 unknown054[0x60 - 0x54];
};

struct ResultSaveDataView
{
    ScoreFileHeader *fileHeader;
    u8 unknown004[4];
    union
    {
        u8 profileData[0x458];
        struct
        {
            u8 unknown008[8];
            i32 profileChecksum;
            u8 unknown014[0x22 - 0x14];
            u8 nextSceneByGroup[11];
            u8 unknown02d[0x460 - 0x2d];
        } profile;
    };
    union
    {
        ResultScoreEntryView scoreEntries[120];
        ResultBestShotImageView bestShotImages[120];
    };
    ResultBestShotRecordView bestShotRecords[120];

    void UpdateBestShotRecord(i32 index);
    ZunResult WriteBestShotData();
};

typedef char ScoreFileHeaderSizeIs18[
    (sizeof(ScoreFileHeader) == 0x18) ? 1 : -1];
typedef char ResultBestShotImageSizeIs60[
    (sizeof(ResultBestShotImageView) == 0x60) ? 1 : -1];
typedef char ResultBestShotRecordSizeIs78[
    (sizeof(ResultBestShotRecordView) == 0x78) ? 1 : -1];
typedef char ResultScoreEntrySizeIs60[
    (sizeof(ResultScoreEntryView) == 0x60) ? 1 : -1];
typedef char ResultSaveProfileDataAt08[
    (offsetof(ResultSaveDataView, profileData) == 0x08) ? 1 : -1];
typedef char ResultSaveNextSceneAt22[
    (offsetof(ResultSaveDataView, profile.nextSceneByGroup) == 0x22) ? 1 : -1];
typedef char ResultSaveScoreEntriesAt460[
    (offsetof(ResultSaveDataView, scoreEntries) == 0x460) ? 1 : -1];
typedef char ResultSaveBestShotRecordsAt3160[
    (offsetof(ResultSaveDataView, bestShotRecords) == 0x3160) ? 1 : -1];
typedef char ResultBestShotImageMetadataAt08[
    (offsetof(ResultBestShotImageView, metadata) == 0x08) ? 1 : -1];
typedef char ResultBestShotRecordCommentAt18[
    (offsetof(ResultBestShotRecordView, comment) == 0x18) ? 1 : -1];
typedef char ResultBestShotRecordValidAt68[
    (offsetof(ResultBestShotRecordView, valid) == 0x68) ? 1 : -1];
typedef char ResultBestShotRecordPhotoIndexAt6C[
    (offsetof(ResultBestShotRecordView, photoIndex) == 0x6c) ? 1 : -1];
typedef char ResultBestShotRecordDataAt70[
    (offsetof(ResultBestShotRecordView, componentData0) == 0x70 &&
     offsetof(ResultBestShotRecordView, pixelData) == 0x74) ? 1 : -1];

} // namespace th095

#endif
