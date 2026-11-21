#pragma once

#include "AnmVmId.hpp"
#include "Chain.hpp"
#include "ControllerButtonMapping.hpp"
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

// Front-end transitions clear the fractional frame before the previous frame.
static __forceinline void FrontEndResetTimer(ZunTimer *timer)
{
    timer->current = 0;
    timer->subFrame = 0.0f;
    timer->previous = -999999;
}

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
    u8 unknown0e93[0x0d];
    i32 previewTimer;
    u8 optionsSavedWindowed;
    u8 unknown0ea5[3];
    ReplayManager *replays[80];
    ControllerButtonMapping optionsButtonMapping;
    u8 unknown0ffa[2];
    char *musicCommentFile;
    char musicTrackTitles[32][64];
    char musicTrackPaths[32][64];
    char musicTrackDescriptions[32][8][64];
    AnmVmIdStorage musicVmStorage[40];
    u8 unknown60a0[0x64];
    i32 musicTrackCount;
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
    char helpAnmPath[260]; // Win32 MAX_PATH
    i32 helpAnmSize;
    u8 *helpAnmData;

    FrontEndControllerView();
    ~FrontEndControllerView();
    i32 Initialize();
    static void __fastcall LoadResources(void *unused);
    static void __fastcall LoadThread(void *unused);
    static FrontEndControllerView *__fastcall Create(i32 entryMode);
    void Destroy();
    ChainCallbackResult Update();
    ChainCallbackResult UpdateMainMenu();
    i32 UpdateHelpMenu();
    i32 UpdateMusicRoom();
    ChainCallbackResult UpdateReplayBrowser();
    ChainCallbackResult UpdateOptionsMenu();
    ChainCallbackResult UpdateSceneSelect();
    ZunResult LoadReplaySlot(i32 slot, char *path);
    void RefreshSceneSelection(i32 selectedScoreEntryIndex);
    void BuildScenePreviewText();
    void UpdateSelectedSceneDetails();
    i32 GetSelectedGroup()
    {
        return this->groupCursor.current;
    }
    char *ResolveSceneText(SceneEncodedText encodedText, i32 column, i32 group, i32 scene);
    static ChainCallbackResult __fastcall OnUpdate(void *controller);
    ChainCallbackResult Draw();
    static ChainCallbackResult __fastcall OnDraw(void *controller);

private:
    __forceinline void SetOptionDigitSprite(i32 vmIndex, i32 digit);
    __forceinline void UpdateButton00Sprites();
    __forceinline void UpdateButton02Sprites();
    __forceinline void UpdateButton06Sprites();
    __forceinline void UpdateBgmVolumeSprites(i32 value);
    __forceinline void UpdateSfxVolumeSprites(i32 value);
    __forceinline void UpdateWindowModeSprites();
    __forceinline void UpdateOptionsSelectionSprites();
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
typedef char FrontEndControllerSceneDisplayAtE88[
    (offsetof(FrontEndControllerView, specialEncodedText) == 0xe60 &&
     offsetof(FrontEndControllerView, specialDisplayStates) == 0xe88 &&
     offsetof(FrontEndControllerView, previewTimer) == 0xea0) ? 1 : -1];
typedef char FrontEndLifecycleReplaysAtEA8[
    (offsetof(FrontEndControllerView, replays) == 0xea8) ? 1 : -1];
typedef char FrontEndControllerOptionsLayout[
    (offsetof(FrontEndControllerView, optionsSavedWindowed) == 0xea4 &&
     offsetof(FrontEndControllerView, optionsButtonMapping) == 0xfe8) ? 1 : -1];
typedef char FrontEndControllerMusicRoomLayout[
    (offsetof(FrontEndControllerView, musicCommentFile) == 0xffc &&
     offsetof(FrontEndControllerView, musicTrackTitles) == 0x1000 &&
     offsetof(FrontEndControllerView, musicTrackPaths) == 0x1800 &&
     offsetof(FrontEndControllerView, musicTrackDescriptions) == 0x2000 &&
     offsetof(FrontEndControllerView, musicVmStorage) == 0x6000 &&
     offsetof(FrontEndControllerView, musicTrackCount) == 0x6104) ? 1 : -1];
typedef char FrontEndLifecycleGroupDataQueueAt61B8[
    (offsetof(FrontEndControllerView, groupPreviewDataQueue) == 0x61b8)
        ? 1
        : -1];
typedef char FrontEndControllerSelectedScoreAtBF0[
    (offsetof(FrontEndControllerView, selectedScoreEntryIndex) == 0xbf0) ? 1 : -1];
typedef char FrontEndControllerUpdateFields[
    (offsetof(FrontEndControllerView, transitionAnm) == 0x04 &&
     offsetof(FrontEndControllerView, stateTimer) == 0x08 &&
     offsetof(FrontEndControllerView, animationTimer) == 0x14 &&
     offsetof(FrontEndControllerView, groupCursor) == 0x20 &&
     offsetof(FrontEndControllerView, secondaryCursor) == 0xf8 &&
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
typedef char FrontEndControllerHelpPageLayout[
    (offsetof(FrontEndControllerView, helpAnmPath) == 0x6408 &&
     offsetof(FrontEndControllerView, helpAnmSize) == 0x650c &&
     offsetof(FrontEndControllerView, helpAnmData) == 0x6510) ? 1 : -1];
typedef char FrontEndLifecycleSizeIs6514[
    (sizeof(FrontEndControllerView) == 0x6514) ? 1 : -1];

} // namespace th095
