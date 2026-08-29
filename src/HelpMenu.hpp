#ifndef TH095_HELP_MENU_HPP
#define TH095_HELP_MENU_HPP

#include "ReplayBrowser.hpp"

namespace th095
{

struct HelpMenuView
{
    SceneAnmLoadedView *sceneAnm;
    SceneAnmLoadedView *transitionAnm;
    ResultScreenTimer stateTimer;
    u8 unknown0014[0x0c];
    ResultScreenReplayCursor cursor;
    u8 unknown00f8[0xafc];
    SceneAnmVmIdArray vmIds;
    u8 unknown0e88[0x5278];
    AnmVmId transitionVm;
    u8 unknown6104[8];
    i32 state;
    i32 requestedState;
    u8 unknown6114[0x2f4];
    char helpAnmPath[MAX_PATH];
    i32 helpAnmSize;
    u8 *helpAnmData;

    i32 UpdateHelpMenu();
};

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

extern HelpMenuView *g_HelpMenu;
extern i32 g_HelpLoadComplete;
extern i32 g_HelpLoadActive;

void __fastcall LoadHelpAnm(void *unused);

} // namespace th095

#endif
