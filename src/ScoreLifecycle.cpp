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
