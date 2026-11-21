#ifndef TH095_RESULT_SCREEN_HPP
#define TH095_RESULT_SCREEN_HPP

#include "AnmManager.hpp"
#include "Global.hpp"
#include "AnmVmId.hpp"
#include "ReplayManager.hpp"
#include "ResultScreenReplayCursor.hpp"
#include "ScoreData.hpp"
#include "FrontEndTimer.hpp"
#include <stdlib.h>

namespace th095
{

struct ResultScreenSceneLabel
{
    char firstLine[0x2c];
    char secondLine[0x2c];
};

typedef char ResultScreenSceneLabelSizeIs58[
    (sizeof(ResultScreenSceneLabel) == 0x58) ? 1 : -1];

struct ResultScreen
{
    AnmLoaded *anm;                            // +0x0000
    i32 state;                                 // +0x0004
    ZunTimer stateTimer;                       // +0x0008
    f32 savedGameSpeed;                        // +0x0014
    AnmVm vms[21];                             // +0x0018
    AnmVm auxiliaryVms[2];                     // +0x3ad4
    AnmVm photoVm;                             // +0x406c
    AnmVm photoTransitionVm;                   // +0x4338
    ResultScreenReplayCursor replayCursor;     // +0x4604
    i32 keyboardSelection;                     // +0x46dc
    i32 replayNameCursor;                      // +0x46e0
    u8 *helpTextBuffer;                        // +0x46e4
    ResultScreenSceneLabel sceneLabels[11][10];// +0x46e8
    i32 sceneCounts[11];                       // +0x6cb8
    i32 selectedGroup;                         // +0x6ce4
    ReplayManager *replays[20];                // +0x6ce8
    u8 unknown6d38[4];
    char replayName[9];                        // +0x6d3c
    u8 unknown6d45[3];
    ResultScreenReplayCursor photoCursor;      // +0x6d48
    i32 notificationTimer;                     // +0x6e20
    ChainElem *calcChain;                      // +0x6e24
    ChainElem *drawChain;                      // +0x6e28

    ResultScreen();
    ~ResultScreen();

    ZunResult Initialize();
    static ZunResult LoadAnm();
    static ZunResult ReleaseAnm();
    static ResultScreen *Create();
    void Destroy();
    i32 UpdateCursor(i32 firstVm);
    void PrepareBestShot();
    ZunResult LoadReplays();
    ChainCallbackResult Update();
    ChainCallbackResult Draw();
    static ChainCallbackResult OnUpdate(ResultScreen *resultScreen);
    static ChainCallbackResult OnDraw(ResultScreen *resultScreen);

    void SetState(i32 value)
    {
        this->state = value;
        FrontEndResetTimer(&this->stateTimer);
    }
};

typedef char ResultScreenVmsAt18[
    (offsetof(ResultScreen, vms) == 0x18) ? 1 : -1];
typedef char ResultScreenSceneLabelsAt46E8[
    (offsetof(ResultScreen, sceneLabels) == 0x46e8) ? 1 : -1];
typedef char ResultScreenSceneCountsAt6CB8[
    (offsetof(ResultScreen, sceneCounts) == 0x6cb8) ? 1 : -1];
typedef char ResultScreenSelectedGroupAt6CE4[
    (offsetof(ResultScreen, selectedGroup) == 0x6ce4) ? 1 : -1];
typedef char ResultScreenReplayCursorAt4604[
    (offsetof(ResultScreen, replayCursor) == 0x4604) ? 1 : -1];
typedef char ResultScreenReplayCursorSizeIsD8[
    (sizeof(ResultScreenReplayCursor) == 0xd8) ? 1 : -1];
typedef char ResultScreenReplaysAt6CE8[
    (offsetof(ResultScreen, replays) == 0x6ce8) ? 1 : -1];
typedef char ResultScreenReplayNameAt6D3C[
    (offsetof(ResultScreen, replayName) == 0x6d3c) ? 1 : -1];
typedef char ResultScreenPhotoCursorAt6D48[
    (offsetof(ResultScreen, photoCursor) == 0x6d48) ? 1 : -1];
typedef char ResultScreenNotificationTimerAt6E20[
    (offsetof(ResultScreen, notificationTimer) == 0x6e20) ? 1 : -1];
typedef char ResultScreenSizeIs6E2C[
    (sizeof(ResultScreen) == 0x6e2c) ? 1 : -1];

} // namespace th095

#endif
