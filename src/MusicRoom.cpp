#include "MusicRoom.hpp"
#include "FileSystem.hpp"
#include "SoundPlayer.hpp"
#include "Supervisor.hpp"

#include <string.h>

namespace th095
{

extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;

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
    i32 i;
    i32 line;
    SceneAnmVmView *vm;
    size_t fileSize;
    char *cursor;

    switch (this->state)
    {
    case 0:
        this->cursor.Push();
        this->vmIds.SetInterrupt(0x66, 1);
        this->vmIds.SetInterrupt(0x67, 1);
        this->vmIds[0x68] = this->sceneAnm->CreateVm(0x68, 7);
        this->vmIds[0x69] = this->sceneAnm->CreateVm(0x69, 7);
        this->vmIds[0x17] = this->sceneAnm->CreateVm(0x17, 7);

        ((MusicRoomAnmStorageView *)g_SceneUiAnm)->textures[0].Clear();
        ((MusicRoomAnmStorageView *)this->sceneAnm)->textures[13].Clear();

        this->vmIds.SetInterrupt(0x19, 3);
        this->vmIds.SetInterrupt(0x1a, 3);
        this->transitionVm.SetInterrupt(3);
        this->vmIds.SetInterrupt(0x1b, 3);
        this->state = 1;
        this->stateTimer.Reset();

        i = 0;
        this->commentFile = (char *)FileSystem::OpenFile(
            "sprt/musiccmt.txt", (i32 *)&fileSize, FALSE);
        if (this->commentFile == NULL)
        {
            goto exit_music_room;
        }

        cursor = this->commentFile;
        while ((i32)fileSize > 0)
        {
            if (*cursor == '#')
            {
                cursor = SkipMusicCommentLine(cursor, (i32 *)&fileSize);
            }
            else if (*cursor == '@')
            {
                cursor = ReadMusicCommentLine(
                    this->paths[i], cursor + 1, (i32 *)&fileSize);
                cursor = ReadMusicCommentLine(
                    this->titles[i], cursor, (i32 *)&fileSize);
                for (line = 0; line < 8; line++)
                {
                    cursor = ReadMusicCommentLine(
                        this->descriptions[i][line], cursor,
                        (i32 *)&fileSize);
                }
                i++;
            }
            else
            {
                cursor = SkipMusicCommentLine(cursor, (i32 *)&fileSize);
            }
        }

        for (line = 0; line < i; line++)
        {
            this->trackVms[line] =
                this->sceneAnm->CreateVm(line + 0x83, 7);
        }
        for (line = 0; line < 8; line++)
        {
            this->descriptionVms[line] =
                g_SceneUiAnm->CreateVm(line + 1, 7);
        }
        this->trackCount = i;
        this->cursor.count = i;
        this->cursor.Set(0);

    case 1:
        if (this->stateTimer.current >= 2 &&
            this->stateTimer.current != this->stateTimer.previous &&
            (this->stateTimer.current & 1) == 0)
        {
            line = (this->stateTimer.current - 2) / 2;
            if (line < this->trackCount)
            {
                vm = g_SceneAnmManager->GetVm(this->trackVms[line]);
                SceneWriteText(g_SceneAnmManager, vm, 0x00dfdfff, 0,
                               this->titles[line]);
                vm->pendingInterrupt = (line != this->cursor.current) + 2;
            }
        }
        if (this->stateTimer.current < 30)
        {
            return 0;
        }
        this->state = 2;
        this->stateTimer.Reset();
        return 0;

    case 2:
        if (this->stateTimer.current < 26 &&
            this->stateTimer.current > 9 &&
            this->stateTimer.current != this->stateTimer.previous &&
            (this->stateTimer.current & 1) == 0)
        {
            line = (this->stateTimer.current - 10) / 2;
            if (line < 8)
            {
                vm = g_SceneAnmManager->GetVm(this->descriptionVms[line]);
                SceneWriteText(
                    g_SceneAnmManager, vm, 0x00dfdfff, 0,
                    this->descriptions[this->cursor.current][line]);
                vm->pendingInterrupt = 2;
            }
        }

        this->cursor.SaveCurrent();
        if (IsMusicRoomMenuInputPressed(TH_BUTTON_UP))
        {
            this->cursor.Move(-1);
        }
        if (IsMusicRoomMenuInputPressed(TH_BUTTON_DOWN))
        {
            this->cursor.Move(1);
        }
        if (this->cursor.HasChanged())
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
            for (line = 0; line < this->trackCount; line++)
            {
                vm = g_SceneAnmManager->GetVm(this->trackVms[line]);
                vm->pendingInterrupt =
                    (line != this->cursor.current) + 2;
            }
        }

        if (GetMusicRoomPressedButtons(0x1002) != 0)
        {
            for (line = 0; line < 8; line++)
            {
                vm = g_SceneAnmManager->GetVm(this->descriptionVms[line]);
                vm->pendingInterrupt = 3;
            }
            this->stateTimer.Reset();
            g_Supervisor.LoadMusic(0, this->paths[this->cursor.current]);
            g_Supervisor.PlayMusic(0, 0);
            return 0;
        }

        if (GetMusicRoomPressedButtons(9) == 0)
        {
            return 0;
        }

    exit_music_room:
        if (this->commentFile != NULL)
        {
            free(this->commentFile);
        }
        this->commentFile = NULL;
        this->cursor.Pop();
        for (line = 0; line < this->trackCount; line++)
        {
            this->trackVms[line].SetInterrupt(1);
        }
        for (line = 0; line < 8; line++)
        {
            this->descriptionVms[line].SetInterrupt(1);
        }
        g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
        this->vmIds.SetInterrupt(0x68, 1);
        this->vmIds.SetInterrupt(0x69, 1);
        this->vmIds[0x66] = this->sceneAnm->CreateVm(0x66, 7);
        this->vmIds[0x67] = this->sceneAnm->CreateVm(0x67, 7);
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
        return 0;
    }

    return 0;
}

} // namespace th095
