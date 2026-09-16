#ifndef TH095_SCORE_DATA_HPP
#define TH095_SCORE_DATA_HPP

#include "Global.hpp"
#include <time.h>

namespace th095
{

#ifdef TH095_MATCH_EXACT
typedef ::ZunResult ScoreDataResult;
#else
typedef ZunResult ScoreDataResult;
#endif

struct ScoreFileHeader
{
    u32 magic;
    u32 fileSize;
#if defined(TH095_MATCH_EXACT)
    u32 unknown008;
#else
    u16 version;
    u16 unknown00a;
#endif
    u32 unknown00c;
    i32 compressedSize;
    i32 uncompressedSize;
};

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

#if !defined(TH095_MATCH_EXACT) && !defined(DIFFBUILD)
enum PhotoScoreFlags
{
    PHOTO_SCORE_ENEMY = 1 << 0,
    PHOTO_SCORE_SELF = 1 << 1,
    PHOTO_SCORE_TWO_SHOT = 1 << 2,
    PHOTO_SCORE_BOSS_RATE = 1 << 3,
    PHOTO_SCORE_NEARBY = 1 << 4,
    PHOTO_SCORE_UNKNOWN_5 = 1 << 5,
    PHOTO_SCORE_COLOR_1 = 1 << 6,
    PHOTO_SCORE_COLOR_2 = 1 << 7,
    PHOTO_SCORE_COLOR_3 = 1 << 8,
    PHOTO_SCORE_COLOR_4 = 1 << 9,
    PHOTO_SCORE_COLOR_5 = 1 << 10,
    PHOTO_SCORE_COLOR_6 = 1 << 11,
    PHOTO_SCORE_COLOR_7 = 1 << 12,
    PHOTO_SCORE_COLORFUL = 1 << 13,
    PHOTO_SCORE_RAINBOW = 1 << 14,
    PHOTO_SCORE_EMPTY = 1 << 15,
    PHOTO_SCORE_NO_BULLETS = 1 << 16,
    PHOTO_SCORE_UNKNOWN_17 = 1 << 17,
    PHOTO_SCORE_UNKNOWN_18 = 1 << 18,
    PHOTO_SCORE_UNKNOWN_19 = 1 << 19,
};
#endif

#if !defined(TH095_MATCH_EXACT)
struct PhotoScoreBreakdownView
{
    i32 finalScore;
    i32 baseScore;
    i32 capturedBulletCount;
    i32 nearbyTargetCount;
    i32 nearbyTargetBonus;
    f32 enemyDistanceMultiplier;
    f32 bossRateMultiplier;
    u32 scoringFlags;
};
#endif

struct ResultBestShotImageView
{
    u8 unknown000[0x10];
    i32 score;
    u8 unknown014[4];
#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
    u32 metadata[8];
#else
    PhotoScoreBreakdownView scoreBreakdown;
#endif
    u8 unknown038[4];
#ifdef TH095_MATCH_EXACT
    i32 replayValue;
#else
    i32 captureTime;
#endif
    u8 unknown040[0x48 - 0x40];
#ifdef TH095_MATCH_EXACT
    f32 slowRate;
    i32 stageValue;
#else
    f32 highScoreSlowRate;
    f32 bestShotSlowRate;
#endif
    u8 unknown050[0x60 - 0x50];
};

#if !defined(TH095_MATCH_EXACT) && !defined(DIFFBUILD)
typedef u8 ResultBestShotPayloadFormat;
enum ResultBestShotPayloadFormatValue
{
    RESULT_BEST_SHOT_PAYLOAD_COMPRESSED_PIXELS = 1,
    RESULT_BEST_SHOT_PAYLOAD_COMMENT_AND_COMPRESSED_PIXELS = 2,
};
#endif

struct ResultBestShotRecordView
{
    u32 magic;
#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
    u8 type;
#else
    ResultBestShotPayloadFormat payloadFormat;
#endif
    u8 componentCount;
    u16 group;
    u16 scene;
    u16 version;
    u16 width;
    u16 height;
    i32 score;
    union
    {
        u8 unknown014[4];
        f32 slowRate;
    };
    char comment[0x50];
    u8 valid;
    u8 componentsLoaded;
    u8 unknown06a[2];
    i32 photoIndex;
#ifdef TH095_MATCH_EXACT
    void *componentData0;
#else
    void *rawFileData;
#endif
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
#ifdef TH095_MATCH_EXACT
    i32 detailScore;
    u8 unknown01c[0x3c - 0x1c];
#else
    union
    {
        i32 detailScore;
        PhotoScoreBreakdownView scoreBreakdown;
    };
    u8 unknown038[4];
#endif
#ifdef TH095_MATCH_EXACT
    union
    {
        i32 attemptCount;
        time_t captureTime;
    };
#else
    i32 captureTime;
#endif
    i32 bestShotChecksum;
#ifdef TH095_MATCH_EXACT
    i32 unlockScore;
#else
    u32 attemptCount;
#endif
#ifdef TH095_MATCH_EXACT
    f32 slowRate;
    f32 successRate;
#else
    f32 highScoreSlowRate;
    f32 bestShotSlowRate;
#endif
    union
    {
        u32 flags;
        struct
        {
            u32 captured : 1;
#ifdef TH095_MATCH_EXACT
            u32 showSuccessRateMarker : 1;
#else
            u32 bestShotLocked : 1;
#endif
            u32 unknownFlags : 30;
        };
    };
    u8 unknown054[0x60 - 0x54];
};

struct ResultSaveDataView
{
    ScoreFileHeader *fileHeader;
    u8 *decompressedData;
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
        // The result and scene-select code address these fields directly in
        // the same 0x458-byte profile block.  They are aliases, not another
        // runtime object or another allocation.
        struct
        {
#if defined(TH095_MATCH_EXACT)
            u8 unknownRuntime008[0x0c];
#else
            ScoreRecordHeaderView profileRecordHeader;
#endif
            char replayName[9];
            u8 unknownRuntime01d;
            i16 lastSelectedGroup;
            union
            {
                i16 lastSelectedScene;
                i16 scene;
            };
            u8 unknownRuntime022[0x43e];
        };
    };
    union
    {
        ResultScoreEntryView scoreEntries[120];
        ResultScoreEntryView sceneScores[120];
        ResultBestShotImageView bestShotImages[120];
    };
    ResultBestShotRecordView bestShotRecords[120];

    ResultSaveDataView();
    ~ResultSaveDataView();
    i32 ParseScoreFile();
    void UpdateBestShotRecord(i32 index);
    ScoreDataResult WriteBestShotData();
    ScoreDataResult LoadScenePreviewTexture(
        struct AnmLoaded *anm, i32 textureIndex, i32 sceneIndex);
    i32 LoadBestShotForScene(i32 group, i32 scene);
    i32 IsSceneGroupUnlocked(i32 group);
    i32 FindHighestUnlockedSceneGroup();
    i32 CountCapturedScenes();
    i32 CountCapturedScenesInGroup(i32 group);
#ifdef TH095_MATCH_EXACT
    i32 GetSceneGroupUnlockScore(i32 group);
#else
    i32 GetSceneGroupAttemptCount(i32 group);
#endif
};

typedef char ScoreFileHeaderSizeIs18[
    (sizeof(ScoreFileHeader) == 0x18) ? 1 : -1];
#if !defined(TH095_MATCH_EXACT)
typedef char ScoreFileHeaderVersionAt08[
    (offsetof(ScoreFileHeader, version) == 0x08) ? 1 : -1];
#endif
typedef char ResultBestShotImageSizeIs60[
    (sizeof(ResultBestShotImageView) == 0x60) ? 1 : -1];
typedef char ResultBestShotRecordSizeIs78[
    (sizeof(ResultBestShotRecordView) == 0x78) ? 1 : -1];
#if !defined(TH095_MATCH_EXACT) && !defined(DIFFBUILD)
typedef char ResultBestShotPayloadFormatSizeIs1[
    (sizeof(ResultBestShotPayloadFormat) == 1) ? 1 : -1];
typedef char ResultBestShotPayloadFormatAt04[
    (offsetof(ResultBestShotRecordView, payloadFormat) == 0x04) ? 1 : -1];
#endif
typedef char ResultScoreEntrySizeIs60[
    (sizeof(ResultScoreEntryView) == 0x60) ? 1 : -1];
#if !defined(TH095_MATCH_EXACT)
typedef char ResultScoreEntryScoreBreakdownAt18[
    (offsetof(ResultScoreEntryView, scoreBreakdown) == 0x18) ? 1 : -1];
typedef char ResultScoreEntryUnknown038At38[
    (offsetof(ResultScoreEntryView, unknown038) == 0x38) ? 1 : -1];
#endif
typedef char ResultSaveProfileDataAt08[
    (offsetof(ResultSaveDataView, profileData) == 0x08) ? 1 : -1];
#if !defined(TH095_MATCH_EXACT)
typedef char ResultSaveProfileRecordHeaderAt08[
    (offsetof(ResultSaveDataView, profileRecordHeader) == 0x08) ? 1 : -1];
#endif
typedef char ResultSaveNextSceneAt22[
    (offsetof(ResultSaveDataView, profile.nextSceneByGroup) == 0x22) ? 1 : -1];
typedef char ResultSaveScoreEntriesAt460[
    (offsetof(ResultSaveDataView, scoreEntries) == 0x460) ? 1 : -1];
typedef char ResultSaveBestShotRecordsAt3160[
    (offsetof(ResultSaveDataView, bestShotRecords) == 0x3160) ? 1 : -1];
typedef char ResultSaveDataSizeIs69A0[
    (sizeof(ResultSaveDataView) == 0x69a0) ? 1 : -1];
typedef char ResultSaveSelectionAt1E[
    (offsetof(ResultSaveDataView, lastSelectedGroup) == 0x1e &&
     offsetof(ResultSaveDataView, lastSelectedScene) == 0x20) ? 1 : -1];
#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
typedef char ResultBestShotImageMetadataAt18[
    (offsetof(ResultBestShotImageView, metadata) == 0x18) ? 1 : -1];
#else
typedef char PhotoScoreBreakdownSizeIs20[
    (sizeof(PhotoScoreBreakdownView) == 0x20) ? 1 : -1];
typedef char PhotoScoreBreakdownMultipliersAt14[
    (offsetof(PhotoScoreBreakdownView, enemyDistanceMultiplier) == 0x14 &&
     offsetof(PhotoScoreBreakdownView, bossRateMultiplier) == 0x18)
        ? 1
        : -1];
typedef char PhotoScoreBreakdownFlagsAt1C[
    (offsetof(PhotoScoreBreakdownView, scoringFlags) == 0x1c) ? 1 : -1];
typedef char ResultBestShotImageScoreBreakdownAt18[
    (offsetof(ResultBestShotImageView, scoreBreakdown) == 0x18) ? 1 : -1];
#endif
typedef char ResultBestShotRecordCommentAt18[
    (offsetof(ResultBestShotRecordView, comment) == 0x18) ? 1 : -1];
typedef char ResultBestShotRecordValidAt68[
    (offsetof(ResultBestShotRecordView, valid) == 0x68) ? 1 : -1];
typedef char ResultBestShotRecordPhotoIndexAt6C[
    (offsetof(ResultBestShotRecordView, photoIndex) == 0x6c) ? 1 : -1];
typedef char ResultBestShotRecordBuffersAt70[
    (
#ifdef TH095_MATCH_EXACT
     offsetof(ResultBestShotRecordView, componentData0) == 0x70 &&
#else
     offsetof(ResultBestShotRecordView, rawFileData) == 0x70 &&
#endif
     offsetof(ResultBestShotRecordView, pixelData) == 0x74) ? 1 : -1];

extern ResultSaveDataView *g_ResultSaveData;

} // namespace th095

#endif
