#include "ResultScreen.hpp"

#include <stdio.h>

namespace th095
{

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
