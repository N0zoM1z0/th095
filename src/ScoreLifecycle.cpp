#ifdef TH095_MATCH_EXACT
#include "ScoreLifecycleExact.inl"
#else
#ifdef TH095_MATCH_EXACT
#define TH095_MATCH_RNG_AS_STRUCT
#endif
#include "Rng.hpp"
#include "ScoreData.hpp"
#include <windows.h>
#include <stddef.h>
#include <string.h>

namespace th095
{

// The target constructor owns one four-byte compiler phase beside the real
// OpenFile size output. Keeping both in the producer frontend preserves the
// target -0x08/-0x04 physical pair without naming a dead function local.
static __forceinline LPBYTE ScoreOpenRawFilePhase()
{
    struct ScoreOpenLocals
    {
        u32 compilerStorage;
        i32 fileSize;
    } locals;
    return FileSystem::OpenFile("scoreth095.dat", &locals.fileSize, TRUE);
}

// Each owned buffer free independently contributes the target-observed
// four-byte cleanup phase. One-sided controls leave hidden this four bytes
// shallow; using both reproduces the destructor's full eight-byte interval.
static __forceinline void ScoreFreeRawFilePhase(void *data)
{
    u32 compilerStorage;
    free(data);
}

static __forceinline void ScoreFreeDecompressedPhase(void *data)
{
    u32 compilerStorage;
    free(data);
}

struct ScoreProfileRandomFillView
{
    u8 beforeRandomWords[8];
    u16 randomWords[512];
};

typedef char ScoreProfileRandomFillSizeIs408[
    (sizeof(ScoreProfileRandomFillView) == 0x408) ? 1 : -1];
typedef char ScoreProfileRandomFillWordsAt8[
    (offsetof(ScoreProfileRandomFillView, randomWords) == 8) ? 1 : -1];

struct ScoreProfileView
{
    u16 magic;
    u16 version;
    u32 size;
    i32 checksum;
    char replayName[9];
    u8 unknown015;
    i16 lastSelectedGroup;
    i16 lastSelectedScene;
    union
    {
        u8 nextSceneByGroup[11];
        ScoreProfileRandomFillView randomFill;
    };
    u8 tail[0x36];

    void Initialize();
};
typedef char ScoreProfileSizeIs458[(sizeof(ScoreProfileView) == 0x458) ? 1 : -1];
typedef char ScoreProfileReplayNameAt0C[
    (offsetof(ScoreProfileView, replayName) == 0x0c) ? 1 : -1];
typedef char ScoreProfileSelectionAt16[
    (offsetof(ScoreProfileView, lastSelectedGroup) == 0x16 &&
     offsetof(ScoreProfileView, lastSelectedScene) == 0x18) ? 1 : -1];
typedef char ScoreProfileNextSceneAt1A[
    (offsetof(ScoreProfileView, nextSceneByGroup) == 0x1a &&
     offsetof(ScoreProfileView, randomFill) == 0x1a) ? 1 : -1];

ResultSaveDataView *g_ResultSaveData;

// FUNCTION: TH095 0x004354B0.
ResultSaveDataView::ResultSaveDataView()
{
    memset(this, 0, sizeof(*this));
    this->fileHeader = reinterpret_cast<ScoreFileHeader *>(
        ScoreOpenRawFilePhase());
    reinterpret_cast<ScoreProfileView *>(
        reinterpret_cast<u8 *>(this) + 8)->Initialize();
    this->ParseScoreFile();
}

// FUNCTION: TH095 0x00435580.
ResultSaveDataView::~ResultSaveDataView()
{
    u32 index;
    void *rawFileData;
    void *decompressedData;

    if (*reinterpret_cast<void **>(this) != NULL)
    {
        rawFileData = *reinterpret_cast<void **>(this);
        ScoreFreeRawFilePhase(rawFileData);
    }
    if (*reinterpret_cast<void **>(reinterpret_cast<u8 *>(this) + 4) != NULL)
    {
        decompressedData =
            *reinterpret_cast<void **>(reinterpret_cast<u8 *>(this) + 4);
        ScoreFreeDecompressedPhase(decompressedData);
    }
    for (index = 0; index < 120; ++index)
        this->UpdateBestShotRecord(index);
}

// FUNCTION: TH095 0x00435500.
void ScoreProfileView::Initialize()
{
    this->magic = 0x5453;
    this->version = 0;
    this->size = 0x458;
    strcpy(this->replayName, "        ");
    for (u32 i = 0; i < 512; ++i)
        this->randomFill.randomWords[i] = g_Rng.GetRandomU16();
}

// FUNCTION: TH095 0x004355F0.
void InitializeScoreData()
{
    g_ResultSaveData = new ResultSaveDataView();
}

// FUNCTION: TH095 0x00435660.
void ReleaseScoreData()
{
    if (g_ResultSaveData != NULL)
    {
        delete g_ResultSaveData;
        g_ResultSaveData = NULL;
    }
    g_ResultSaveData = NULL;
}

} // namespace th095

#endif // TH095_MATCH_EXACT
