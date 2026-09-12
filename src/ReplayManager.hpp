#ifndef TH095_REPLAY_MANAGER_HPP
#define TH095_REPLAY_MANAGER_HPP

#include "Global.hpp"

namespace th095
{

#ifdef TH095_MATCH_EXACT
typedef ::ZunResult ReplayManagerResult;
#else
typedef ZunResult ReplayManagerResult;
#endif

enum ReplayManagerMode
{
    REPLAY_MANAGER_RECORD = 0,
    REPLAY_MANAGER_PLAYBACK = 1,
    REPLAY_MANAGER_LOAD_ONLY = 2,
};

// Persistent replay container header. The game writer emits the complete
// 0x24-byte block. LoadReplay trusts the payload sizes directly and does not
// validate the writer metadata before allocation, decryption, or decompression.
struct ReplayFileHeader
{
    u32 magic;                       // +0x00, writer emits 0x72353974
    u16 version;                     // +0x04, writer emits 1
    u8 unknown006[0x06];             // writer leaves zero after header memset
    u32 userDataOffset;              // +0x0c, writer emits 0x24 + compressed payload size
    u32 gameVersion;                 // +0x10, writer emits 0x102
    u8 unknown014[0x08];             // writer leaves zero after header memset
    u32 compressedPayloadSize;       // +0x1c
    u32 decompressedPayloadSize;     // +0x20
};

struct ReplayInputData
{
    u16 playerConfigId;              // +0x00
    i8 level;                        // +0x02
    i8 scene;                        // +0x03
    u16 rngSeed;                     // +0x04
    u8 unknown006;
#if defined(TH095_MATCH_EXACT)
    char replayName[8];              // +0x07
    u8 unknown00f;
#else
    char replayName[9];              // +0x07, eight visible bytes plus NUL
#endif
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
#if !defined(TH095_MATCH_EXACT)
typedef char ReplayInputDataReplayNameAt07[
    (offsetof(ReplayInputData, replayName) == 0x07 &&
     sizeof(((ReplayInputData *)0)->replayName) == 9) ? 1 : -1];
typedef char ReplayInputDataTimestampAt10[
    (offsetof(ReplayInputData, timestamp) == 0x10) ? 1 : -1];
#endif
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

    ReplayManagerResult Initialize(i32 mode, char *path);
    ReplayManagerResult WriteReplay(char *path, char *replayName);
    ReplayManagerResult LoadReplay(char *path);

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
