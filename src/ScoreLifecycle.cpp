#include "inttypes.hpp"
#include <windows.h>
#include <stddef.h>
#include <string.h>

namespace th095
{

struct Rng
{
    u16 GetRandomU16();
};
extern Rng g_Rng;

namespace FileSystem
{
LPBYTE OpenFile(LPCSTR path, i32 *fileSize, BOOL isExternalResource);
}

struct ResultSaveDataView
{
    i32 ParseScoreFile();
    void UpdateBestShotRecord(i32 index);
};

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

struct ScoreProfileView
{
    u16 magic;
    u16 version;
    u32 size;
    i32 checksum;
    char name[9];
    u8 unknown015[0x0d];
    u16 randomWords[512];
    u8 tail[0x36];

    void Initialize();
};
typedef char ScoreProfileSizeIs458[(sizeof(ScoreProfileView) == 0x458) ? 1 : -1];
typedef char ScoreProfileNameAt0C[(offsetof(ScoreProfileView, name) == 0x0c) ? 1 : -1];
typedef char ScoreProfileRandomAt22[(offsetof(ScoreProfileView, randomWords) == 0x22) ? 1 : -1];

struct ResultSaveDataLifecycleView
{
    u8 bytes[0x69a0];
    ResultSaveDataLifecycleView();
    ~ResultSaveDataLifecycleView();
};
typedef char ResultSaveDataLifecycleSizeIs69A0[(sizeof(ResultSaveDataLifecycleView) == 0x69a0) ? 1 : -1];
extern ResultSaveDataLifecycleView *g_PhotoStageSaveData;

// FUNCTION: TH095 0x004354B0.
ResultSaveDataLifecycleView::ResultSaveDataLifecycleView()
{
    memset(this, 0, sizeof(*this));
    *reinterpret_cast<u8 **>(this) = ScoreOpenRawFilePhase();
    reinterpret_cast<ScoreProfileView *>(
        reinterpret_cast<u8 *>(this) + 8)->Initialize();
    reinterpret_cast<ResultSaveDataView *>(this)->ParseScoreFile();
}

// FUNCTION: TH095 0x00435580.
ResultSaveDataLifecycleView::~ResultSaveDataLifecycleView()
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
        reinterpret_cast<ResultSaveDataView *>(this)->UpdateBestShotRecord(index);
}

// FUNCTION: TH095 0x00435500.
void ScoreProfileView::Initialize()
{
    this->magic = 0x5453;
    this->version = 0;
    this->size = 0x458;
    strcpy(this->name, "        ");
    for (u32 i = 0; i < 512; ++i)
        this->randomWords[i] = g_Rng.GetRandomU16();
}

// FUNCTION: TH095 0x004355F0.
void InitializeScoreData()
{
    g_PhotoStageSaveData = new ResultSaveDataLifecycleView();
}

// FUNCTION: TH095 0x00435660.
void ReleaseScoreData()
{
    if (g_PhotoStageSaveData != NULL)
    {
        delete g_PhotoStageSaveData;
        g_PhotoStageSaveData = NULL;
    }
    g_PhotoStageSaveData = NULL;
}

} // namespace th095
