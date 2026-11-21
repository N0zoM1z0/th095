#include "AnmManager.hpp"
#include "ControllerInputSlot.hpp"
#include "FileSystem.hpp"
#include "FrontEndController.hpp"
#include "SceneSelect.hpp"
#include "SoundPlayer.hpp"

#include <string.h>

namespace th095
{

struct MusicRoomVmIds
{
    AnmVmId tracks[32];
    AnmVmId descriptions[8];
};

typedef char MusicRoomVmIdsLayout[
    (offsetof(MusicRoomVmIds, descriptions) == 0x80 &&
     sizeof(MusicRoomVmIds) == sizeof(((FrontEndControllerView *)0)->musicVmStorage)) ? 1 : -1];

static __forceinline i32 MusicRoomTimerAtLeast(ZunTimer *timer, i32 value)
{
    return timer->current >= value;
}

static __forceinline i32 MusicRoomTimerChangedAndEven(ZunTimer *timer)
{
    return timer->current != timer->previous && timer->current % 2 == 0;
}

static __forceinline void MusicRoomCreateVmAt(FrontEndControllerView *view, i32 scriptIndex)
{
    view->vmIds[scriptIndex] = reinterpret_cast<SceneAnmLoadedView *>(view->sceneAnm)->CreateVm(scriptIndex, 7);
}

static __forceinline void MusicRoomFreeCommentFile(FrontEndControllerView *view)
{
    char *block = view->musicCommentFile;
    free(block);
}

static __forceinline u16 GetMusicRoomPressedButtons(u16 buttons)
{
    return g_ControllerInputSlots[0].pressed & buttons;
}

static __forceinline u16 IsMusicRoomMenuInputPressed(u16 buttons)
{
    return (u16)((GetMusicRoomPressedButtons(buttons) != 0) ||
                 ((g_ControllerInputSlots[0].repeat & buttons) != 0));
}

char *__fastcall SkipMusicCommentLine(char *cursor, i32 *remaining)
{
    char *current = cursor;

    while (*current != '\n' && *current != '\r' && *remaining != 0)
    {
        current++;
        (*remaining)--;
    }
    if (*remaining != 0)
    {
        while ((*current == '\n' || *current == '\r') && *remaining != 0)
        {
            current++;
            (*remaining)--;
        }
    }
    return current;
}

char *__fastcall ReadMusicCommentLine(char *destination, char *cursor,
                                      i32 *remaining)
{
    char *current = cursor;

    while (*current != '\n' && *current != '\r' && *remaining != 0)
    {
        current++;
        (*remaining)--;
    }
    if (*remaining != 0)
    {
        *current = '\0';
        strcpy(destination, cursor);
        current++;
        (*remaining)--;
        while ((*current == '\n' || *current == '\r') && *remaining != 0)
        {
            current++;
            (*remaining)--;
        }
    }
    return current;
}

i32 FrontEndControllerView::UpdateMusicRoom()
{
// The controller allocates these handle slots without running AnmVmId constructors.
#define musicVms (reinterpret_cast<MusicRoomVmIds *>(&this->musicVmStorage))
    switch (this->state)
    {
    case MUSIC_ROOM_STATE_INITIALIZE:
    {
        this->groupCursor.Push();
        this->vmIds.SetInterrupt(0x66, 1);
        this->vmIds.SetInterrupt(0x67, 1);
        MusicRoomCreateVmAt(this, 0x68);
        MusicRoomCreateVmAt(this, 0x69);
        MusicRoomCreateVmAt(this, 0x17);

        // Target 0x004510E1 reads 0x004C4AAC, Supervisor::textAnm, for the
        // writable text surface.  title.anm remains the owner of the music
        // room's static scripts and its separate dynamic entry 13.
        g_Supervisor.textAnm->textures[0].Clear();
        this->sceneAnm->textures[13].Clear();

        this->vmIds.SetInterrupt(0x19, 3);
        this->vmIds.SetInterrupt(0x1a, 3);
        this->transitionVm.SetInterrupt(3);
        this->vmIds.SetInterrupt(0x1b, 3);
        this->state = MUSIC_ROOM_STATE_TRACK_LIST_REVEAL;
        FrontEndResetTimer(&this->stateTimer);

        struct MusicCommentLocals
        {
            i32 fileSize;
            i32 trackCount;
            char *fileCursor;
        } musicComment;
        musicComment.trackCount = 0;
        this->musicCommentFile = (char *)FileSystem::OpenFile(
            "sprt/musiccmt.txt", &musicComment.fileSize, FALSE);
        if (this->musicCommentFile == NULL)
            goto exit_music_room;

        musicComment.fileCursor = this->musicCommentFile;
        while (musicComment.fileSize > 0)
        {
            if (*musicComment.fileCursor == '#')
            {
                musicComment.fileCursor = SkipMusicCommentLine(
                    musicComment.fileCursor, &musicComment.fileSize);
            }
            else if (*musicComment.fileCursor == '@')
            {
                musicComment.fileCursor = ReadMusicCommentLine(
                    this->musicTrackPaths[musicComment.trackCount],
                    musicComment.fileCursor + 1, &musicComment.fileSize);
                musicComment.fileCursor = ReadMusicCommentLine(
                    this->musicTrackTitles[musicComment.trackCount],
                    musicComment.fileCursor, &musicComment.fileSize);
                for (i32 descriptionLine = 0; descriptionLine < 8; descriptionLine++)
                {
                    musicComment.fileCursor = ReadMusicCommentLine(
                        this->musicTrackDescriptions[musicComment.trackCount][descriptionLine],
                        musicComment.fileCursor, &musicComment.fileSize);
                }
                musicComment.trackCount++;
            }
            else
            {
                musicComment.fileCursor = SkipMusicCommentLine(
                    musicComment.fileCursor, &musicComment.fileSize);
            }
        }

        for (i32 trackVmIndex = 0; trackVmIndex < musicComment.trackCount; trackVmIndex++)
            musicVms->tracks[trackVmIndex] =
                this->sceneAnm->CreateVm(trackVmIndex + 0x83, 7);
        for (i32 descriptionVmIndex = 0; descriptionVmIndex < 8; descriptionVmIndex++)
            musicVms->descriptions[descriptionVmIndex] =
                g_Supervisor.textAnm->CreateVm(descriptionVmIndex + 1, 7);
        this->musicTrackCount = musicComment.trackCount;
        this->groupCursor.count = musicComment.trackCount;
        this->groupCursor.Set(0);
    }

    case MUSIC_ROOM_STATE_TRACK_LIST_REVEAL:
    {
        if (MusicRoomTimerAtLeast(&this->stateTimer, 2))
        {
            if (MusicRoomTimerChangedAndEven(&this->stateTimer))
            {
                i32 trackLine = (this->stateTimer.GetCurrent() - 2) / 2;
                if (trackLine < this->musicTrackCount)
                {
                    AnmVm *trackVm =
                        musicVms->tracks[trackLine].GetVm();
                    SceneWriteText(g_AnmManager, trackVm, 0x00dfdfff, 0,
                                   this->musicTrackTitles[trackLine]);
                    trackVm->pendingInterrupt =
                        (trackLine != this->groupCursor.GetCurrent()) + 2;
                }
            }
        }
        if (MusicRoomTimerAtLeast(&this->stateTimer, 30))
        {
            this->state = MUSIC_ROOM_STATE_INTERACTIVE;
            FrontEndResetTimer(&this->stateTimer);
        }
        break;
    }

    case MUSIC_ROOM_STATE_INTERACTIVE:
    {
        if (this->stateTimer < 26 && MusicRoomTimerAtLeast(&this->stateTimer, 10))
        {
            if (MusicRoomTimerChangedAndEven(&this->stateTimer))
            {
                i32 descriptionLine =
                    (this->stateTimer.GetCurrent() - 10) / 2;
                if (descriptionLine < 8)
                {
                    AnmVm *descriptionVm =
                        musicVms->descriptions[descriptionLine].GetVm();
                    SceneWriteText(
                        g_AnmManager, descriptionVm, 0x00dfdfff, 0,
                        this->musicTrackDescriptions[this->groupCursor.GetCurrent()][descriptionLine]);
                    descriptionVm->pendingInterrupt = 2;
                }
            }
        }

        this->groupCursor.SaveCurrent();
        if (IsMusicRoomMenuInputPressed(TH_BUTTON_UP))
            this->groupCursor.Move(-1);
        if (IsMusicRoomMenuInputPressed(TH_BUTTON_DOWN))
            this->groupCursor.Move(1);
        if (this->groupCursor.HasChanged())
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
            for (i32 trackIndex = 0; trackIndex < this->musicTrackCount; trackIndex++)
            {
                AnmVm *trackVm = musicVms->tracks[trackIndex].GetVm();
                trackVm->pendingInterrupt =
                    (trackIndex != this->groupCursor.GetCurrent()) + 2;
            }
        }

        if (GetMusicRoomPressedButtons(TH_BUTTON_ENTER | TH_BUTTON_BOMB) != 0)
        {
            for (i32 descriptionIndex = 0; descriptionIndex < 8; descriptionIndex++)
            {
                AnmVm *descriptionVm =
                    musicVms->descriptions[descriptionIndex].GetVm();
                descriptionVm->pendingInterrupt = 3;
            }
            FrontEndResetTimer(&this->stateTimer);
            g_Supervisor.LoadMusic(0, this->musicTrackPaths[this->groupCursor.GetCurrent()]);
            g_Supervisor.PlayMusic(0, 0);
            break;
        }

        if (GetMusicRoomPressedButtons(TH_BUTTON_MENU | TH_BUTTON_SHOOT) != 0)
        {
        exit_music_room:
            if (this->musicCommentFile != NULL)
                MusicRoomFreeCommentFile(this);
            this->musicCommentFile = NULL;
            this->groupCursor.Pop();
            for (i32 trackIndex = 0; trackIndex < this->musicTrackCount; trackIndex++)
                musicVms->tracks[trackIndex].SetInterrupt(1);
            for (i32 descriptionIndex = 0; descriptionIndex < 8; descriptionIndex++)
                musicVms->descriptions[descriptionIndex].SetInterrupt(1);
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            this->vmIds.SetInterrupt(0x68, 1);
            this->vmIds.SetInterrupt(0x69, 1);
            MusicRoomCreateVmAt(this, 0x66);
            MusicRoomCreateVmAt(this, 0x67);
            this->vmIds.SetInterrupt(0x17, 1);
            this->vmIds.SetInterrupt(0x19, 2);
            this->vmIds.SetInterrupt(0x1a, 2);
            this->transitionVm.SetInterrupt(2);
            this->vmIds.SetInterrupt(0x1b, 2);
            this->requestedState = FRONT_END_REQUESTED_STATE_MAIN_MENU;
            this->state = MUSIC_ROOM_STATE_INITIALIZE;
            FrontEndResetTimer(&this->stateTimer);
            g_Supervisor.LoadMusic(0, "bgm/th095_00.wav");
            g_Supervisor.PlayMusic(0, 0);
        }
        break;
    }
    }
#undef musicVms
    return 0;
}

} // namespace th095
