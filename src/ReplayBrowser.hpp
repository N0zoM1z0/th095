#ifdef TH095_MATCH_EXACT
#include "ReplayBrowserExact.hpp"
#else
#ifndef TH095_REPLAY_BROWSER_HPP
#define TH095_REPLAY_BROWSER_HPP

#include "FrontEndGlobals.hpp"
#include "ReplayManager.hpp"
#include "ResultScreen.hpp"
#include "SceneSelect.hpp"

namespace th095
{

#ifndef DIFFBUILD
typedef i32 ReplayBrowserState;
enum ReplayBrowserStateValue
{
    REPLAY_BROWSER_STATE_INITIALIZE = 0,
    REPLAY_BROWSER_STATE_BROWSE = 1,
};
typedef char ReplayBrowserStateSizeIs4[
    (sizeof(ReplayBrowserState) == sizeof(i32)) ? 1 : -1];
#endif

struct ReplayBrowserView
{
    SceneAnmLoadedView *sceneAnm;
    i32 unknown0004;
    ZunTimer stateTimer;
    ZunTimer animationTimer; // +0x14; advanced by the shared front-end update
    ResultScreenReplayCursor rowCursor;
    ResultScreenReplayCursor columnCursor;
    u8 unknown01d0[0xa20];
    i32 selectedReplayIndex;
    SceneAnmVmIdArray vmIds;
    u8 unknown0e88[0x20];
    ReplayManager *replays[80];
    u8 unknown0fe8[0x5118];
    AnmVmId transitionVm;
    u8 unknown6104[8];
#ifdef DIFFBUILD
    i32 state;
#else
    ReplayBrowserState state;
#endif
#ifdef DIFFBUILD
    i32 requestedState;
#else
    FrontEndRequestedState requestedState;
#endif

    ZunResult LoadReplaySlot(i32 slot, char *path);
    ChainCallbackResult Update();
};

typedef char ReplayBrowserAnimationTimerAt14[
    (offsetof(ReplayBrowserView, animationTimer) == 0x14) ? 1 : -1];
typedef char ReplayBrowserReplaysAtEA8[
    (offsetof(ReplayBrowserView, replays) == 0xea8) ? 1 : -1];
typedef char ReplayBrowserStateAt610C[
    (offsetof(ReplayBrowserView, state) == 0x610c) ? 1 : -1];
typedef char ReplayBrowserRequestedStateAt6110[
    (offsetof(ReplayBrowserView, requestedState) == 0x6110) ? 1 : -1];
typedef char ReplayBrowserRowCursorAt20[
    (offsetof(ReplayBrowserView, rowCursor) == 0x20) ? 1 : -1];
typedef char ReplayBrowserColumnCursorAtF8[
    (offsetof(ReplayBrowserView, columnCursor) == 0xf8) ? 1 : -1];
typedef char ReplayBrowserVmIdsAtBF4[
    (offsetof(ReplayBrowserView, vmIds) == 0xbf4) ? 1 : -1];
typedef char ReplayBrowserTransitionVmAt6100[
    (offsetof(ReplayBrowserView, transitionVm) == 0x6100) ? 1 : -1];

struct ReplayBrowserExitSignal
{
    u8 unknown000[8];
    i32 requested;

    void Request();
};
typedef char ReplayBrowserExitRequestedAt08[
    (offsetof(ReplayBrowserExitSignal, requested) == 0x08) ? 1 : -1];
typedef char ReplayBrowserExitSignalSizeIs0C[
    (sizeof(ReplayBrowserExitSignal) == 0x0c) ? 1 : -1];

extern i32 g_ReplayBrowserSelection;
extern char g_SelectedReplayPath[0x100];
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
extern ReplayBrowserExitSignal g_ReplayBrowserExitSignal;
#else
extern ReplayBrowserExitSignal &g_ReplayBrowserExitSignal;
#endif

void __fastcall LoadReplayBrowserEntries(void *unused);

} // namespace th095

#endif

#endif // TH095_MATCH_EXACT
