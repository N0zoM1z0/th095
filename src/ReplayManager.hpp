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

struct ReplayManager
{
    i32 mode;                        // +0x000
    void *fileHeader;                // +0x004
    void *inputData;                 // +0x008
    void *fpsData;                   // +0x00c
    u8 *inputCursor;                 // +0x010
    u8 *fpsCursor;                   // +0x014
    u8 replayFps;                    // +0x018
    u8 unknown019[3];
    i32 frameCounter;                // +0x01c
    u8 unknown020[0x124 - 0x020];
    ChainElem *calcChain;            // +0x124
    ChainElem *drawChain;            // +0x128

    ReplayManager();
    ~ReplayManager();

    ZunResult Initialize(i32 mode, char *path);

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
