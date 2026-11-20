#ifndef TH095_SCORE_DATA_HPP
#define TH095_SCORE_DATA_HPP

#include "Global.hpp"
#include "PhotoScoreBreakdown.hpp"
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

typedef u8 ResultBestShotPayloadFormat;
enum ResultBestShotPayloadFormatValue
{
    RESULT_BEST_SHOT_PAYLOAD_COMPRESSED_PIXELS = 1,
    RESULT_BEST_SHOT_PAYLOAD_COMMENT_AND_COMPRESSED_PIXELS = 2,
};

struct ResultBestShotRecordView
{
    u32 magic;
    ResultBestShotPayloadFormat payloadFormat;
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
    u8 *rawFileData;
    u8 *pixelData;
};

enum
{
    RESULT_BEST_SHOT_FILE_HEADER_SIZE = offsetof(ResultBestShotRecordView, comment),
    RESULT_BEST_SHOT_FILE_HEADER_WITH_COMMENT_SIZE = offsetof(ResultBestShotRecordView, valid),
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
    union
    {
        i32 detailScore;
        PhotoScoreBreakdownView scoreBreakdown;
    };
    u8 unknown038[4];
    i32 captureTime;
    i32 bestShotChecksum;
    u32 attemptCount;
    f32 highScoreSlowRate;
    f32 bestShotSlowRate;
    union
    {
        u32 flags;
        struct
        {
            u32 captured : 1;
            u32 bestShotLocked : 1;
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
typedef char ResultBestShotRecordSizeIs78[
    (sizeof(ResultBestShotRecordView) == 0x78) ? 1 : -1];
typedef char ResultBestShotPayloadFormatSizeIs1[
    (sizeof(ResultBestShotPayloadFormat) == 1) ? 1 : -1];
typedef char ResultBestShotPayloadFormatAt04[
    (offsetof(ResultBestShotRecordView, payloadFormat) == 0x04) ? 1 : -1];
typedef char ResultScoreEntrySizeIs60[
    (sizeof(ResultScoreEntryView) == 0x60) ? 1 : -1];
typedef char ResultScoreEntryScoreBreakdownAt18[
    (offsetof(ResultScoreEntryView, scoreBreakdown) == 0x18) ? 1 : -1];
typedef char ResultScoreEntryScoreAt10[
    (offsetof(ResultScoreEntryView, score) == 0x10) ? 1 : -1];
typedef char ResultScoreEntryCaptureTimeAt3C[
    (offsetof(ResultScoreEntryView, captureTime) == 0x3c) ? 1 : -1];
typedef char ResultScoreEntryHighScoreSlowRateAt48[
    (offsetof(ResultScoreEntryView, highScoreSlowRate) == 0x48) ? 1 : -1];
typedef char ResultScoreEntryBestShotSlowRateAt4C[
    (offsetof(ResultScoreEntryView, bestShotSlowRate) == 0x4c) ? 1 : -1];
typedef char ResultScoreEntryUnknown038At38[
    (offsetof(ResultScoreEntryView, unknown038) == 0x38) ? 1 : -1];
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
typedef char ResultBestShotRecordCommentAt18[
    (offsetof(ResultBestShotRecordView, comment) == 0x18) ? 1 : -1];
typedef char ResultBestShotRecordValidAt68[
    (offsetof(ResultBestShotRecordView, valid) == 0x68) ? 1 : -1];
typedef char ResultBestShotRecordPhotoIndexAt6C[
    (offsetof(ResultBestShotRecordView, photoIndex) == 0x6c) ? 1 : -1];
typedef char ResultBestShotRecordBuffersAt70[
    (offsetof(ResultBestShotRecordView, rawFileData) == 0x70 &&
     offsetof(ResultBestShotRecordView, pixelData) == 0x74) ? 1 : -1];

extern ResultSaveDataView *g_ResultSaveData;

} // namespace th095

#endif
