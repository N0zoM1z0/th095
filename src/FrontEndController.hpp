#pragma once

#include "Chain.hpp"
#include "FrontEndGlobals.hpp"
#include "FrontEndPendingTextureData.hpp"
#include "ResultScreenReplayCursor.hpp"
#include "SceneData.hpp"
#include "ScenePreviewDataQueue.hpp"
#include "SceneValueQueue.hpp"
#include "ZunTimer.hpp"

namespace th095
{

struct AnmLoaded;
struct ReplayManager;

enum FrontEndEntryMode
{
    FRONT_END_ENTRY_MAIN_MENU = 0,
    FRONT_END_ENTRY_SCENE_SELECT = 1,
    FRONT_END_ENTRY_REPLAY_BROWSER = 2,
};

struct FrontEndControllerView
{
    AnmLoaded *sceneAnm;
    AnmLoaded *transitionAnm;
    ZunTimer stateTimer;
    ZunTimer animationTimer;
    ResultScreenReplayCursor groupCursor;
    ResultScreenReplayCursor secondaryCursor;
    ResultScreenReplayCursor sceneCursors[12];
    i32 selectedScoreEntryIndex;
    u8 unknown0bf4[0x268];
    u8 *ownedMissionMessageData;
    SceneEncodedText specialEncodedText[10];
    i8 specialDisplayStates[10];
    i8 currentDisplayState;
    u8 unknown0e93[0x15];
    ReplayManager *replays[80];
    u8 unknown0fe8[0x14];
    void *replayListData;
    u8 unknown1000[0x5108];
    i32 transitionReady;
    i32 state;
    FrontEndRequestedState requestedState;
    u8 unknown6114[0x0c];
    FrontEndControllerFlagBits flagBits;
    i32 entryMode;
    SceneValueQueue selectionQueue;
    SceneValueQueue loadedSceneQueue;
    ScenePreviewDataQueue groupPreviewDataQueue;
    SceneValueQueue groupPreviewSizeQueue;
    ScenePreviewDataQueue scenePreviewDataQueue;
    SceneValueQueue scenePreviewSizeQueue;
    SceneValueQueue groupPreviewQueue;
    SceneValueQueue scenePreviewQueue;
    SceneValueQueue loadedGroupQueue;
    SceneStateHistory stateHistory;
    u8 unknown63c0[0x0c];
    FrontEndPendingTextureData pendingTextures;
    ChainElem *calcChain;
    ChainElem *drawChain;
    u8 unknown6408[0x10c];

    FrontEndControllerView();
    ~FrontEndControllerView();
    i32 Initialize();
    static void __fastcall LoadResources(void *unused);
    static void __fastcall LoadThread(void *unused);
    static FrontEndControllerView *__fastcall Create(i32 entryMode);
    void Destroy();
    ChainCallbackResult Update();
    ChainCallbackResult UpdateMainMenu();
    static ChainCallbackResult __fastcall OnUpdate(void *controller);
    ChainCallbackResult Draw();
    static ChainCallbackResult __fastcall OnDraw(void *controller);
};


typedef char FrontEndLifecyclePendingTextureDataLayout[
    (sizeof(FrontEndPendingTextureData) == 0x34 &&
     offsetof(FrontEndControllerView, pendingTextures) == 0x63cc &&
     offsetof(FrontEndPendingTextureData, primaryData) == 0x04 &&
     offsetof(FrontEndPendingTextureData, primarySize) == 0x10 &&
     offsetof(FrontEndPendingTextureData, secondaryData) == 0x1c &&
     offsetof(FrontEndPendingTextureData, secondarySize) == 0x28) ? 1 : -1];
typedef char FrontEndLifecycleOwnedMissionDataAtE5C[
    (offsetof(FrontEndControllerView, ownedMissionMessageData) == 0xe5c)
        ? 1
        : -1];
typedef char FrontEndLifecycleSpecialEncodedTextAtE60[
    (offsetof(FrontEndControllerView, specialEncodedText) == 0xe60) ? 1 : -1];
typedef char FrontEndLifecycleDisplayStateAtE92[
    (offsetof(FrontEndControllerView, currentDisplayState) == 0xe92) ? 1
                                                                   : -1];
typedef char FrontEndLifecycleReplaysAtEA8[
    (offsetof(FrontEndControllerView, replays) == 0xea8) ? 1 : -1];
typedef char FrontEndLifecycleReplayListDataAtFFC[
    (offsetof(FrontEndControllerView, replayListData) == 0xffc) ? 1 : -1];
typedef char FrontEndLifecycleGroupDataQueueAt61B8[
    (offsetof(FrontEndControllerView, groupPreviewDataQueue) == 0x61b8)
        ? 1
        : -1];
typedef char FrontEndControllerSelectedScoreAtBF0[
    (offsetof(FrontEndControllerView, selectedScoreEntryIndex) == 0xbf0) ? 1 : -1];
typedef char FrontEndControllerUpdateFields[
    (offsetof(FrontEndControllerView, stateTimer) == 0x08 &&
     offsetof(FrontEndControllerView, animationTimer) == 0x14 &&
     offsetof(FrontEndControllerView, groupCursor) == 0x20 &&
     offsetof(FrontEndControllerView, transitionReady) == 0x6108 &&
     offsetof(FrontEndControllerView, state) == 0x610c) ? 1 : -1];
typedef char FrontEndControllerRequestedStateAt6110[
    (offsetof(FrontEndControllerView, requestedState) == 0x6110) ? 1 : -1];
typedef char FrontEndLifecycleFlagsAt6120[
    (offsetof(FrontEndControllerView, flagBits) == 0x6120) ? 1 : -1];
typedef char FrontEndLifecycleEntryModeAt6124[
    (offsetof(FrontEndControllerView, entryMode) == 0x6124) ? 1 : -1];
typedef char FrontEndLifecycleSceneDataQueueAt6248[
    (offsetof(FrontEndControllerView, scenePreviewDataQueue) == 0x6248)
        ? 1
        : -1];
typedef char FrontEndLifecyclePendingPrimaryAt63D0[
    (offsetof(FrontEndControllerView, pendingTextures) +
         offsetof(FrontEndPendingTextureData, primaryData) == 0x63d0)
        ? 1
        : -1];
typedef char FrontEndLifecyclePendingSecondaryAt63E8[
    (offsetof(FrontEndControllerView, pendingTextures) +
         offsetof(FrontEndPendingTextureData, secondaryData) == 0x63e8)
        ? 1
        : -1];
typedef char FrontEndControllerStateHistoryAt63B0[
    (offsetof(FrontEndControllerView, stateHistory) == 0x63b0 &&
     offsetof(SceneStateHistory, count) == 0x0c) ? 1 : -1];
typedef char FrontEndLifecycleChainsAt6400[
    (offsetof(FrontEndControllerView, calcChain) == 0x6400 &&
     offsetof(FrontEndControllerView, drawChain) == 0x6404)
        ? 1
        : -1];
typedef char FrontEndLifecycleSizeIs6514[
    (sizeof(FrontEndControllerView) == 0x6514) ? 1 : -1];

} // namespace th095
