#include "MusicRoom.hpp"
#include "FileSystem.hpp"
#include "SoundPlayer.hpp"
#include "Supervisor.hpp"

#include <string.h>

namespace th095
{

extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;

static __forceinline i32 MusicRoomTimerAtLeast(ResultScreenTimer *timer, i32 value)
{
    return timer->current >= value;
}

static __forceinline i32 MusicRoomTimerChangedAndEven(ResultScreenTimer *timer)
{
    return timer->current != timer->previous && timer->current % 2 == 0;
}

static __forceinline void MusicRoomCreateVmAt(MusicRoomView *view, i32 scriptIndex)
{
    view->vmIds[scriptIndex] = view->sceneAnm->CreateVm(scriptIndex, 7);
}

static __forceinline void MusicRoomFreeCommentFile(MusicRoomView *view)
{
    char *block = view->commentFile;
    free(block);
}

struct MusicRoomTextureEntryView
{
    IDirect3DTexture8 *texture;
    u8 unknown004[0x0c];

    void Clear();
};

struct MusicRoomAnmStorageView
{
    u8 unknown000[0x14];
    MusicRoomTextureEntryView *textures;
};

static __forceinline u16 GetMusicRoomPressedButtons(u16 buttons)
{
    return g_PressedButtons & buttons;
}

static __forceinline u16 IsMusicRoomMenuInputPressed(u16 buttons)
{
    return (u16)((GetMusicRoomPressedButtons(buttons) != 0) ||
                 ((g_ResultMenuInput & buttons) != 0));
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

i32 MusicRoomView::UpdateMusicRoom()
{
    switch (this->state)
    {
    case 0:
    {
        this->cursor.Push();
        this->vmIds.SetInterrupt(0x66, 1);
        this->vmIds.SetInterrupt(0x67, 1);
        MusicRoomCreateVmAt(this, 0x68);
        MusicRoomCreateVmAt(this, 0x69);
        MusicRoomCreateVmAt(this, 0x17);

        ((MusicRoomAnmStorageView *)g_SceneUiAnm)->textures[0].Clear();
        ((MusicRoomAnmStorageView *)this->sceneAnm)->textures[13].Clear();

        this->vmIds.SetInterrupt(0x19, 3);
        this->vmIds.SetInterrupt(0x1a, 3);
        this->transitionVm.SetInterrupt(3);
        this->vmIds.SetInterrupt(0x1b, 3);
        this->state = 1;
        this->stateTimer.Reset();

        struct MusicCommentLocals
        {
            i32 fileSize;
            i32 trackCount;
            char *fileCursor;
        } musicComment;
        musicComment.trackCount = 0;
        this->commentFile = (char *)FileSystem::OpenFile(
            "sprt/musiccmt.txt", &musicComment.fileSize, FALSE);
        if (this->commentFile == NULL)
            goto exit_music_room;

        musicComment.fileCursor = this->commentFile;
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
                    this->paths[musicComment.trackCount],
                    musicComment.fileCursor + 1, &musicComment.fileSize);
                musicComment.fileCursor = ReadMusicCommentLine(
                    this->titles[musicComment.trackCount],
                    musicComment.fileCursor, &musicComment.fileSize);
                for (i32 descriptionLine = 0; descriptionLine < 8; descriptionLine++)
                {
                    musicComment.fileCursor = ReadMusicCommentLine(
                        this->descriptions[musicComment.trackCount][descriptionLine],
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
            this->trackVms[trackVmIndex] =
                this->sceneAnm->CreateVm(trackVmIndex + 0x83, 7);
        for (i32 descriptionVmIndex = 0; descriptionVmIndex < 8; descriptionVmIndex++)
            this->descriptionVms[descriptionVmIndex] =
                g_SceneUiAnm->CreateVm(descriptionVmIndex + 1, 7);
        this->trackCount = musicComment.trackCount;
        this->cursor.count = musicComment.trackCount;
        this->cursor.Set(0);
    }

    case 1:
    {
        if (MusicRoomTimerAtLeast(&this->stateTimer, 2))
        {
            if (MusicRoomTimerChangedAndEven(&this->stateTimer))
            {
                i32 trackLine = (this->stateTimer.GetCurrent() - 2) / 2;
                if (trackLine < this->trackCount)
                {
                    SceneAnmVmView *trackVm =
                        this->trackVms[trackLine].GetVm();
                    SceneWriteText(g_SceneAnmManager, trackVm, 0x00dfdfff, 0,
                                   this->titles[trackLine]);
                    trackVm->pendingInterrupt =
                        (trackLine != this->cursor.GetCurrent()) + 2;
                }
            }
        }
        if (MusicRoomTimerAtLeast(&this->stateTimer, 30))
        {
            this->state = 2;
            this->stateTimer.Reset();
        }
        break;
    }

    case 2:
    {
        if (this->stateTimer < 26 && MusicRoomTimerAtLeast(&this->stateTimer, 10))
        {
            if (MusicRoomTimerChangedAndEven(&this->stateTimer))
            {
                i32 descriptionLine =
                    (this->stateTimer.GetCurrent() - 10) / 2;
                if (descriptionLine < 8)
                {
                    SceneAnmVmView *descriptionVm =
                        this->descriptionVms[descriptionLine].GetVm();
                    SceneWriteText(
                        g_SceneAnmManager, descriptionVm, 0x00dfdfff, 0,
                        this->descriptions[this->cursor.GetCurrent()][descriptionLine]);
                    descriptionVm->pendingInterrupt = 2;
                }
            }
        }

        this->cursor.SaveCurrent();
        if (IsMusicRoomMenuInputPressed(TH_BUTTON_UP))
            this->cursor.Move(-1);
        if (IsMusicRoomMenuInputPressed(TH_BUTTON_DOWN))
            this->cursor.Move(1);
        if (this->cursor.HasChanged())
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
            for (i32 trackIndex = 0; trackIndex < this->trackCount; trackIndex++)
            {
                SceneAnmVmView *trackVm = this->trackVms[trackIndex].GetVm();
                trackVm->pendingInterrupt =
                    (trackIndex != this->cursor.GetCurrent()) + 2;
            }
        }

        if (GetMusicRoomPressedButtons(0x1002) != 0)
        {
            for (i32 descriptionIndex = 0; descriptionIndex < 8; descriptionIndex++)
            {
                SceneAnmVmView *descriptionVm =
                    this->descriptionVms[descriptionIndex].GetVm();
                descriptionVm->pendingInterrupt = 3;
            }
            this->stateTimer.Reset();
            g_Supervisor.LoadMusic(0, this->paths[this->cursor.GetCurrent()]);
            g_Supervisor.PlayMusic(0, 0);
            break;
        }

        if (GetMusicRoomPressedButtons(9) != 0)
        {
        exit_music_room:
            if (this->commentFile != NULL)
                MusicRoomFreeCommentFile(this);
            this->commentFile = NULL;
            this->cursor.Pop();
            for (i32 trackIndex = 0; trackIndex < this->trackCount; trackIndex++)
                this->trackVms[trackIndex].SetInterrupt(1);
            for (i32 descriptionIndex = 0; descriptionIndex < 8; descriptionIndex++)
                this->descriptionVms[descriptionIndex].SetInterrupt(1);
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
            this->requestedState = 1;
            this->state = 0;
            this->stateTimer.Reset();
            g_Supervisor.LoadMusic(0, "bgm/th095_00.wav");
            g_Supervisor.PlayMusic(0, 0);
        }
        break;
    }
    }
    return 0;
}

} // namespace th095
