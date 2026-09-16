#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
#define TH095_HELP_STATE_INITIALIZE 0
#define TH095_HELP_STATE_PAGE_SELECT 1
#else
enum HelpMenuOuterStateValue
{
    HELP_MENU_INITIALIZE = 0,
    HELP_MENU_PAGE_SELECT = 1,
};
#define TH095_HELP_STATE_INITIALIZE HELP_MENU_INITIALIZE
#define TH095_HELP_STATE_PAGE_SELECT HELP_MENU_PAGE_SELECT
#endif

#ifdef TH095_MATCH_EXACT
#include "HelpMenuExact.hpp"
#else
#ifndef TH095_HELP_MENU_HPP
#define TH095_HELP_MENU_HPP

#include "FrontEndGlobals.hpp"
#include "ReplayBrowser.hpp"

namespace th095
{

struct HelpMenuView
{
    SceneAnmLoadedView *sceneAnm;
    SceneAnmLoadedView *transitionAnm;
    ZunTimer stateTimer;
    ZunTimer animationTimer; // +0x14; advanced by the shared front-end update
    ResultScreenReplayCursor cursor;
    u8 unknown00f8[0xafc];
    SceneAnmVmIdArray vmIds;
    u8 unknown0e88[0x5278];
    AnmVmId transitionVm;
    u8 unknown6104[8];
    i32 state;
#ifdef DIFFBUILD
    i32 requestedState;
#else
    FrontEndRequestedState requestedState;
#endif
    u8 unknown6114[0x2f4];
    char helpAnmPath[MAX_PATH];
    i32 helpAnmSize;
    u8 *helpAnmData;

    i32 UpdateHelpMenu();
};

typedef char HelpMenuAnimationTimerAt14[
    (offsetof(HelpMenuView, animationTimer) == 0x14) ? 1 : -1];
typedef char HelpMenuSelectionVmsAtE38[
    (offsetof(HelpMenuView, vmIds) + 0x91 * sizeof(SceneAnmVmId) == 0xe38)
        ? 1
        : -1];
typedef char HelpMenuTransitionVmAt6100[
    (offsetof(HelpMenuView, transitionVm) == 0x6100) ? 1 : -1];
typedef char HelpMenuStateAt610C[
    (offsetof(HelpMenuView, state) == 0x610c) ? 1 : -1];
typedef char HelpMenuPathAt6408[
    (offsetof(HelpMenuView, helpAnmPath) == 0x6408) ? 1 : -1];
typedef char HelpMenuSizeAt650C[
    (offsetof(HelpMenuView, helpAnmSize) == 0x650c) ? 1 : -1];
typedef char HelpMenuDataAt6510[
    (offsetof(HelpMenuView, helpAnmData) == 0x6510) ? 1 : -1];

void __fastcall LoadHelpAnm(void *unused);

} // namespace th095

#endif

#endif // TH095_MATCH_EXACT
