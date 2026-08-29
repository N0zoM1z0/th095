#ifndef TH095_REPLAY_MANAGER_HPP
#define TH095_REPLAY_MANAGER_HPP

#include "Global.hpp"

namespace th095
{

enum ReplayManagerMode
{
    REPLAY_MANAGER_RECORD = 0,
    REPLAY_MANAGER_PLAYBACK = 1,
    REPLAY_MANAGER_LOAD_ONLY = 2,
};

struct ReplayFileHeader
{
    u32 magic;                       // +0x00
    u16 version;                     // +0x04
    u8 unknown006[0x06];
    u32 fileSize;                    // +0x0c
    u32 gameVersion;                 // +0x10
    u8 unknown014[0x08];
    u32 compressedSize;              // +0x1c
    u32 decompressedSize;            // +0x20
};

struct ReplayInputData
{
    u16 playerConfigId;              // +0x00
    i8 level;                        // +0x02
    i8 scene;                        // +0x03
    u16 rngSeed;                     // +0x04
    u8 unknown006;
    char replayName[8];              // +0x07
    u8 unknown00f;
    i32 timestamp;                   // +0x10
    i32 score;                       // +0x14
    u8 globalStateSnapshot[0xc8];     // +0x18
    f32 slowRate;                    // +0xe0
    u8 unknown0e4[0x0c];
    u32 inputStreamSize;              // +0xf0
    u32 fpsStreamSize;                // +0xf4
};

typedef char ReplayFileHeaderSizeIs24[
    (sizeof(ReplayFileHeader) == 0x24) ? 1 : -1];
typedef char ReplayInputDataSizeIsF8[
    (sizeof(ReplayInputData) == 0xf8) ? 1 : -1];

struct ReplayManager
{
    i32 mode;                        // +0x000
    ReplayFileHeader *fileHeader;    // +0x004
    ReplayInputData *inputData;      // +0x008
    u8 *fpsData;                     // +0x00c
    u8 *inputCursor;                 // +0x010
    u8 *fpsCursor;                   // +0x014
    u8 replayFps;                    // +0x018
    u8 unknown019[3];
    i32 frameCounter;                // +0x01c
    ReplayInputData *activeInputData; // +0x020
    char path[0x100];                // +0x024
    ChainElem *calcChain;            // +0x124
    ChainElem *drawChain;            // +0x128

    ReplayManager();
    ~ReplayManager();

    ZunResult Initialize(i32 mode, char *path);
    ZunResult WriteReplay(char *path, char *replayName);
    ZunResult LoadReplay(char *path);

    static ReplayManager *Create(i32 mode, char *path);
    static ReplayManager *Load(char *path);
    static void Destroy(ReplayManager *replayManager);
    ChainCallbackResult ProcessFrame();
    ChainCallbackResult DrawFps();
    static ChainCallbackResult OnUpdate(ReplayManager *replayManager);
    static ChainCallbackResult OnDraw(ReplayManager *replayManager);
};

typedef char ReplayManagerCalcChainAt124[
    (offsetof(ReplayManager, calcChain) == 0x124) ? 1 : -1];
typedef char ReplayManagerSizeIs12C[
    (sizeof(ReplayManager) == 0x12c) ? 1 : -1];

extern ReplayManager *g_ReplayManager;

} // namespace th095

#endif
