#pragma once

#include "diffbuild.hpp"
#include "inttypes.hpp"

namespace th095
{

enum FrontEndRequestedState
{
    FRONT_END_REQUESTED_STATE_INITIALIZE = 0,
    FRONT_END_REQUESTED_STATE_MAIN_MENU = 1,
    FRONT_END_REQUESTED_STATE_SCENE_SELECT = 2,
    FRONT_END_REQUESTED_STATE_REPLAY_BROWSER = 3,
    FRONT_END_REQUESTED_STATE_EXIT = 4,
    FRONT_END_REQUESTED_STATE_START_GAME = 5,
    FRONT_END_REQUESTED_STATE_START_REPLAY = 6,
    FRONT_END_REQUESTED_STATE_OPTIONS = 7,
    FRONT_END_REQUESTED_STATE_MUSIC_ROOM = 8,
    FRONT_END_REQUESTED_STATE_HELP = 9,
};

typedef char FrontEndRequestedStateSizeIs4[
    (sizeof(FrontEndRequestedState) == 4) ? 1 : -1];

// The active screen determines which state domain applies to the shared
// front-end substate. The requested-state enum above selects the screen.
enum FrontEndMainMenuStateValue
{
    FRONT_END_MAIN_MENU_STATE_INITIALIZE = 0,
    FRONT_END_MAIN_MENU_STATE_ACTIVE = 1,
};

enum ReplayBrowserStateValue
{
    REPLAY_BROWSER_STATE_INITIALIZE = 0,
    REPLAY_BROWSER_STATE_BROWSE = 1,
};

enum OptionsMenuStateValue
{
    OPTIONS_MENU_STATE_INITIALIZE = 0,
    OPTIONS_MENU_STATE_ACTIVE = 1,
};

enum MusicRoomStateValue
{
    MUSIC_ROOM_STATE_INITIALIZE = 0,
    MUSIC_ROOM_STATE_TRACK_LIST_REVEAL = 1,
    MUSIC_ROOM_STATE_INTERACTIVE = 2,
};

enum SceneSelectState
{
    SCENE_SELECT_STATE_INITIALIZE = 0,
    SCENE_SELECT_STATE_ACTIVE = 1,
};

enum HelpMenuStateValue
{
    HELP_MENU_INITIALIZE = 0,
    HELP_MENU_PAGE_SELECT = 1,
    HELP_MENU_PAGE_LOADING = 2,
    HELP_MENU_PAGE_DATA_READY = 3,
    HELP_MENU_PAGE_VIEW = 4,
};

struct FrontEndControllerFlagBits
{
    u32 titleLoadIncomplete : 1;
    u32 titleLoadFailed : 1;
    u32 previewPending : 1;
    u32 showRates : 1;
    u32 unknownFlagBit4 : 1;
    u32 assetLoadStopRequested : 1;
    u32 unknownFlagBits6 : 26;
};

typedef char FrontEndControllerFlagBitsSizeIs4[
    (sizeof(FrontEndControllerFlagBits) == 4) ? 1 : -1];

struct SceneStateHistory
{
    i32 values[3];
    i32 count;
};

// The target has one active front-end/pause controller pointer at 0x004CA2F4.
// Its concrete view changes with the current menu, so keep the storage type
// neutral and cast only at the owning call site.
DIFFABLE_EXTERN(void *, g_ActiveMenuController)

} // namespace th095
