#include "ResultScreen.hpp"
#include "SoundPlayer.hpp"

#include <stdio.h>

namespace th095
{

extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;

inline u16 GetPressedButtons(u16 buttons)
{
    return g_PressedButtons & buttons;
}

inline u16 IsResultMenuInputPressed(u16 buttons)
{
    return (u16)((GetPressedButtons(buttons) != 0) ||
                 ((g_ResultMenuInput & buttons) != 0));
}

i32 ResultScreenReplayCursor::Move(i32 amount)
{
    i32 i;

tryAgain:
    this->current += amount;
    while (this->current >= this->count)
    {
        if (this->wraps != 0)
        {
            this->current -= this->count;
        }
        else
        {
            this->current = this->count - 1;
        }
    }
    while (this->current < 0)
    {
        if (this->wraps != 0)
        {
            this->current += this->count;
        }
        else
        {
            this->current = 0;
        }
    }
    for (i = 0; i < this->disabledEntryCount; i++)
    {
        if (this->disabledEntries[i] == this->current)
        {
            goto tryAgain;
        }
    }
    return this->current;
}

i32 ResultScreen::UpdateCursor(i32 firstVm)
{
    if (this->stateTimer < 10)
    {
        return 1;
    }
    if (this->stateTimer == 10)
    {
        for (i32 i = 0; i < this->replayCursor.GetCount(); i++)
        {
            this->vms[firstVm + i].SetInterrupt(
                (i != this->replayCursor.GetCurrent()) + 2);
        }
    }

    this->replayCursor.SaveCurrent();
    if (IsResultMenuInputPressed(0x10))
    {
        this->replayCursor.Move(-1);
    }
    if (IsResultMenuInputPressed(0x20))
    {
        this->replayCursor.Move(1);
    }

    if (this->replayCursor.HasChanged())
    {
        for (i32 i = 0; i < this->replayCursor.GetCount(); i++)
        {
            this->vms[firstVm + i].SetInterrupt(
                (i != this->replayCursor.GetCurrent()) + 2);
        }
        g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
    }
    return 0;
}

ZunResult ResultScreen::LoadReplays()
{
    char path[0x100];

    for (i32 i = 0; i < 20; i++)
    {
        if (this->replays[i] != NULL)
        {
            delete this->replays[i];
            this->replays[i] = NULL;
        }
        sprintf(path, "th95_%.2d.rpy", i + 1);
        this->replays[i] = ReplayManager::Load(path);
    }
    this->replayCursor.Set(0);
    return ZUN_SUCCESS;
}

} // namespace th095
