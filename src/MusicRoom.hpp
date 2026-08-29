#ifndef TH095_MUSIC_ROOM_HPP
#define TH095_MUSIC_ROOM_HPP

#include "ReplayBrowser.hpp"

namespace th095
{

struct MusicRoomTrack
{
    char title[64];
    char path[64];
    char descriptions[8][64];
};

struct MusicRoomView
{
    SceneAnmLoadedView *sceneAnm;
    SceneAnmLoadedView *transitionAnm;
    ResultScreenTimer stateTimer;
    u8 unknown0014[0x0c];
    ResultScreenReplayCursor cursor;
    u8 unknown00f8[0xafc];
    SceneAnmVmIdArray vmIds;
    u8 unknown0e88[0x174];
    char *commentFile;
    char titles[32][64];
    char paths[32][64];
    char descriptions[32][8][64];
    SceneAnmVmId trackVms[32];
    SceneAnmVmId descriptionVms[8];
    u8 unknown60a0[0x60];
    AnmVmId transitionVm;
    i32 trackCount;
    u8 unknown6108[4];
    i32 state;
    i32 requestedState;

    i32 UpdateMusicRoom();
};

typedef char MusicRoomCommentFileAtFFC[
    (offsetof(MusicRoomView, commentFile) == 0xffc) ? 1 : -1];
typedef char MusicRoomTitlesAt1000[
    (offsetof(MusicRoomView, titles) == 0x1000) ? 1 : -1];
typedef char MusicRoomPathsAt1800[
    (offsetof(MusicRoomView, paths) == 0x1800) ? 1 : -1];
typedef char MusicRoomDescriptionsAt2000[
    (offsetof(MusicRoomView, descriptions) == 0x2000) ? 1 : -1];
typedef char MusicRoomTrackVmsAt6000[
    (offsetof(MusicRoomView, trackVms) == 0x6000) ? 1 : -1];
typedef char MusicRoomDescriptionVmsAt6080[
    (offsetof(MusicRoomView, descriptionVms) == 0x6080) ? 1 : -1];
typedef char MusicRoomTransitionVmAt6100[
    (offsetof(MusicRoomView, transitionVm) == 0x6100) ? 1 : -1];
typedef char MusicRoomTrackCountAt6104[
    (offsetof(MusicRoomView, trackCount) == 0x6104) ? 1 : -1];
typedef char MusicRoomStateAt610C[
    (offsetof(MusicRoomView, state) == 0x610c) ? 1 : -1];
typedef char MusicRoomRequestedStateAt6110[
    (offsetof(MusicRoomView, requestedState) == 0x6110) ? 1 : -1];

char *__fastcall SkipMusicCommentLine(char *cursor, i32 *remaining);
char *__fastcall ReadMusicCommentLine(char *destination, char *cursor,
                                      i32 *remaining);

} // namespace th095

#endif
