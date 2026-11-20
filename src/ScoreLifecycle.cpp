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

ResultSaveDataView *g_ResultSaveData;

// FUNCTION: TH095 0x004354B0.
ResultSaveDataView::ResultSaveDataView()
{
    memset(this, 0, sizeof(*this));
    this->fileHeader = reinterpret_cast<ScoreFileHeader *>(
        ScoreOpenRawFilePhase());
    this->profile.Initialize();
    this->ParseScoreFile();
}

// FUNCTION: TH095 0x00435580.
ResultSaveDataView::~ResultSaveDataView()
{
    u32 index;
    void *rawFileData;
    void *decompressedData;

    if (this->fileHeader != NULL)
    {
        rawFileData = this->fileHeader;
        ScoreFreeRawFilePhase(rawFileData);
    }
    if (this->decompressedData != NULL)
    {
        decompressedData = this->decompressedData;
        ScoreFreeDecompressedPhase(decompressedData);
    }
    for (index = 0; index < 120; ++index)
        this->UpdateBestShotRecord(index);
}

// FUNCTION: TH095 0x00435500.
void ScoreProfileView::Initialize()
{
    this->recordHeader.magic = SCORE_PROFILE_RECORD_MAGIC;
    this->recordHeader.version = SCORE_PROFILE_RECORD_VERSION;
    this->recordHeader.size = sizeof(*this);
    strcpy(this->replayName, "        ");
    for (u32 i = 0; i < ARRAY_SIZE(this->randomFill.randomWords); ++i)
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
