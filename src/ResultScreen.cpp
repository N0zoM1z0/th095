#include "ResultScreen.hpp"
#include "SoundPlayer.hpp"

#include <stdio.h>
#include <string.h>

namespace th095
{

extern u16 g_ResultMenuInput;
extern u16 g_PressedButtons;
extern f32 g_AnmGameSpeed;

struct ResultScreenGlobalStateView
{
    u8 unknown000[0xfc];
    u32 unknownFlag0 : 1;
    u32 unknownFlag1 : 1;
    u32 suppressResultCallbacks : 1;
    u32 unknownFlags : 29;
};

struct ResultAnmVmHandleView
{
    u8 unknown000[0x228];
    u32 flagsWord;
};

extern ResultScreenGlobalStateView *g_ResultScreenGlobalState;

struct ResultRuntimeView
{
    u8 unknown000[8];
    i16 scene;
    u8 unknown00a[0x14 - 0x0a];
    char replayName[9];
};

struct ResultPlayerConfigView
{
    u8 unknown000[4];
    i32 group;
    i32 scene;
};

extern i32 g_ResultSceneState;
extern ResultRuntimeView *g_ResultRuntime;
extern ResultPlayerConfigView *g_ResultPlayerConfig;
extern u8 *g_ResultPlayerConfigTable[];
extern i32 g_ResultSceneLimits[];
extern const char *g_ResultAlphabet;
extern ResultPhotoDataView *g_ResultPhotoData;
extern ResultPhotoControllerView *g_ResultPhotoController;

extern void __fastcall InitializeGameResultScreen(ResultScreen *resultScreen);
extern void __fastcall InitializePhotoResultScreen(ResultScreen *resultScreen);
extern void __fastcall InitializeReplayResultScreen(ResultScreen *resultScreen);
extern void __fastcall UpdatePhotoResultScreen(ResultScreen *resultScreen);
extern void __fastcall PreparePhotoResultScreen(ResultScreen *resultScreen);
extern void __fastcall FinalizeResultRuntime(ResultRuntimeView *runtime);
extern i32 __fastcall ExecuteResultVm(ResultScreenAnmVm *vm);

inline u16 GetPressedButtons(u16 buttons)
{
    return g_PressedButtons & buttons;
}

inline u16 IsResultMenuInputPressed(u16 buttons)
{
    return (u16)((GetPressedButtons(buttons) != 0) ||
                 ((g_ResultMenuInput & buttons) != 0));
}

i32 ResultScreenTimer::Tick()
{
    this->previous = this->current;
    if (g_AnmGameSpeed <= 0.99f)
    {
        this->subFrame += g_AnmGameSpeed;
        this->current = (i32)this->subFrame;
    }
    else
    {
        this->current++;
        this->subFrame += 1.0f;
    }
    return this->current;
}

i32 ResultPhotoDataView::FindBestShot()
{
    i32 bestShot;

    bestShot = -1;
    {
        i32 bestScore;
        bestScore = -1;

        for (i32 i = 0; i < 11; i++)
        {
            if (this->slots[i].score > bestScore)
            {
                bestScore = this->slots[i].score;
                bestShot = i;
            }
        }
    }
    return bestShot;
}

void ResultScreen::PrepareBestShot()
{
    i32 bestShot = g_ResultPhotoData->FindBestShot();
    if (bestShot >= 0)
    {
        g_ResultPhotoData->anm->SetAndExecuteScript(
            &this->vms[23], bestShot * 2 + 1);

        ResultScreenAnmVm *vm = &this->vms[23];
        vm->spriteSize.x = vm->loadedSprite->uvEndX * 255.0f;
        vm->spriteSize.y = vm->loadedSprite->uvEndY * 255.0f;

        g_ResultPhotoData->photoVms[bestShot].SetInterrupt(2);
        this->photoCursor.Set(bestShot);

        if (bestShot < g_ResultPhotoController->GetPhotoCount())
        {
            this->photoCursor.count =
                g_ResultPhotoController->GetPhotoCount();
            this->photoCursor.wraps = 1;
            for (i32 i = 0;
                 i < g_ResultPhotoController->GetPhotoCount();
                 i++)
            {
                reinterpret_cast<ResultAnmVmHandleView *>(
                    g_ResultPhotoData->photoVms[i].GetVm())
                    ->flagsWord |= 0x10000000;
            }
        }
    }
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

void ResultScreenReplayCursor::Push()
{
    this->savedCurrent[this->saveDepth] = this->current;
    this->savedCount[this->saveDepth] = this->count;
    this->saveDepth++;
    if (this->saveDepth >= 16)
    {
        this->saveDepth = 15;
    }
    this->disabledEntryCount = 0;
}

void ResultScreenReplayCursor::Pop()
{
    this->saveDepth--;
    if (this->saveDepth < 0)
    {
        this->saveDepth = 0;
    }
    this->current = this->savedCurrent[this->saveDepth];
    this->count = this->savedCount[this->saveDepth];
    this->disabledEntryCount = 0;
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

ChainCallbackResult ResultScreen::Update()
{
    i32 i;

    switch (this->state)
    {
    case 0:
        if ((g_PressedButtons & 8) != 0)
        {
            InitializeGameResultScreen(this);
        }
        else if ((g_ResultScreenGlobalState->unknownFlags & (1 << 2)) != 0)
        {
            InitializePhotoResultScreen(this);
        }
        else if ((g_ResultScreenGlobalState->unknownFlags & (1 << 3)) != 0)
        {
            InitializeReplayResultScreen(this);
        }
        break;

    case 1:
        if (this->UpdateCursor(4) == 0)
        {
            if ((g_PressedButtons & 8) != 0)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                this->SetState(2);
                this->replayCursor.Set(0);
                for (i = 0; i < 21; i++)
                {
                    this->vms[i].SetInterrupt(1);
                }
                this->vms[23].SetInterrupt(1);
            }
            else if ((g_PressedButtons & 0x1002) != 0)
            {
                this->SetState(2);
                if (this->replayCursor.current == 0)
                {
                    for (i = 0; i < 21; i++)
                    {
                        this->vms[i].SetInterrupt(1);
                    }
                    this->vms[23].SetInterrupt(1);
                    g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                }
                else
                {
                    g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                }
            }
        }
        break;

    case 2:
        if (this->stateTimer.current > 7)
        {
            g_AnmGameSpeed = this->savedGameSpeed;
            this->state = 0;
            if (this->replayCursor.current == 0)
            {
                g_ResultScreenGlobalState->unknownFlags &= ~(1 << 2);
            }
            else if (this->replayCursor.current == 1)
            {
                g_AnmGameSpeed = 1.0f;
                g_ResultSceneState = 4;
            }
            else if (this->replayCursor.current == 2)
            {
                g_AnmGameSpeed = 1.0f;
                g_ResultSceneState = 2;
            }
            FinalizeResultRuntime(g_ResultRuntime);
            g_ResultScreenGlobalState->unknownFlags |= (1 << 4);
        }
        break;

    case 3:
        if (this->UpdateCursor(8) == 0 &&
            (g_PressedButtons & 0x1002) != 0)
        {
            this->SetState(4);
            for (i = 3; i < 21; i++)
            {
                this->vms[i].SetInterrupt(1);
            }
            g_SoundPlayer.PlaySoundByIdx(
                this->replayCursor.current == 0 ? SOUND_BACK : SOUND_SELECT, 0);
        }
        break;

    case 4:
        if (this->stateTimer.current > 7)
        {
            g_AnmGameSpeed = 1.0f;
            this->state = 0;
            if (this->replayCursor.current == 0)
            {
                g_ResultSceneState = 4;
            }
            else if (this->replayCursor.current == 1)
            {
                g_ResultSceneState = 2;
            }
            else if (this->replayCursor.current == 2)
            {
                this->replayCursor.Push();
                this->replayCursor.count = 20;
                this->state = 13;
                this->LoadReplays();
                this->stateTimer.Reset();
            }
            FinalizeResultRuntime(g_ResultRuntime);
            g_ResultScreenGlobalState->unknownFlags |= (1 << 4);
        }
        break;

    case 5:
        if (this->UpdateCursor(12) == 0)
        {
            UpdatePhotoResultScreen(this);
            if ((g_PressedButtons & 0x1002) != 0)
            {
                this->SetState(6);
                for (i = 3; i < 21; i++)
                {
                    this->vms[i].SetInterrupt(1);
                }
                this->vms[23].SetInterrupt(1);
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
            }
        }
        break;

    case 6:
        if (this->stateTimer.current > 7)
        {
            g_AnmGameSpeed = 1.0f;
            this->state = 0;
            FinalizeResultRuntime(g_ResultRuntime);
            switch (this->replayCursor.current)
            {
            case 0:
                g_ResultSceneState = 4;
                break;
            case 1:
                g_ResultRuntime->scene = (i16)(g_ResultPlayerConfig->scene + 1);
                g_ResultPlayerConfig = (ResultPlayerConfigView *)(
                    g_ResultPlayerConfigTable[g_ResultPlayerConfig->group] +
                    (g_ResultPlayerConfig->scene + 1) * 0x30);
                g_ResultSceneState = 8;
                break;
            case 2:
                g_ResultSceneState = 2;
                break;
            case 3:
                this->replayCursor.Push();
                this->replayCursor.count = 20;
                this->state = 13;
                this->LoadReplays();
                this->stateTimer.Reset();
                break;
            }
            g_ResultScreenGlobalState->unknownFlags |= (1 << 4);
        }
        break;

    case 7:
    case 9:
        if (this->UpdateCursor(19) == 0 &&
            (g_PressedButtons & 0x1002) != 0)
        {
            this->SetState(8);
            g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
        }
        break;

    case 8:
    case 10:
        if (this->stateTimer.current > 7)
        {
            g_AnmGameSpeed = 1.0f;
            this->state = 0;
            g_ResultSceneState = this->replayCursor.current == 0 ? 4 : 2;
            g_ResultScreenGlobalState->unknownFlags |= (1 << 4);
        }
        break;

    case 11:
        if (this->UpdateCursor(16) == 0)
        {
            if ((g_PressedButtons & 8) != 0)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                this->SetState(12);
                this->replayCursor.Set(0);
                for (i = 0; i < 21; i++)
                {
                    this->vms[i].SetInterrupt(1);
                }
                this->vms[23].SetInterrupt(1);
            }
            else if ((g_PressedButtons & 0x1002) != 0)
            {
                this->SetState(12);
                if (this->replayCursor.current == 0)
                {
                    g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                    for (i = 0; i < 21; i++)
                    {
                        this->vms[i].SetInterrupt(1);
                    }
                    this->vms[23].SetInterrupt(1);
                }
                else
                {
                    g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                }
            }
        }
        break;

    case 12:
        if (this->stateTimer.current > 7)
        {
            g_AnmGameSpeed = this->savedGameSpeed;
            this->state = 0;
            if (this->replayCursor.current == 0)
            {
                g_ResultScreenGlobalState->unknownFlags &= ~(1 << 2);
            }
            else if (this->replayCursor.current == 1)
            {
                g_AnmGameSpeed = 1.0f;
                g_ResultSceneState = 4;
            }
            else if (this->replayCursor.current == 2)
            {
                g_AnmGameSpeed = 1.0f;
                g_ResultSceneState = 2;
            }
            FinalizeResultRuntime(g_ResultRuntime);
        }
        break;

    case 13:
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
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
        }

        if ((g_PressedButtons & 0x1002) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
            this->SetState(14);
            strcpy(this->replayName, g_ResultRuntime->replayName);
            if (strcmp(this->replayName, "--------") != 0)
            {
                this->keyboardSelection = 95;
            }
            this->replayCursor.Push();
        }
        else if ((g_PressedButtons & 9) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            if ((g_ResultScreenGlobalState->unknownFlags & (1 << 2)) != 0)
            {
                this->SetState(3);
                this->anm->SetAndExecuteScript(&this->vms[7], 7);
                this->anm->SetAndExecuteScript(&this->vms[9], 9);
                this->anm->SetAndExecuteScript(&this->vms[10], 10);
                this->anm->SetAndExecuteScript(&this->vms[8], 8);
                this->replayCursor.Pop();
                this->replayCursor.count = 3;
                this->replayCursor.wraps = 1;
                this->replayNameCursor = 0;
            }
            else if ((g_ResultScreenGlobalState->unknownFlags & (1 << 3)) != 0)
            {
                this->SetState(5);
                this->anm->SetAndExecuteScript(&this->vms[11], 11);
                this->anm->SetAndExecuteScript(&this->vms[14], 14);
                this->anm->SetAndExecuteScript(&this->vms[15], 15);
                this->anm->SetAndExecuteScript(&this->vms[12], 12);
                this->anm->SetAndExecuteScript(&this->vms[13], 13);
                this->replayCursor.Pop();
                if (g_ResultPlayerConfig->scene >=
                    g_ResultSceneLimits[g_ResultPlayerConfig->group] - 1)
                {
                    this->replayCursor.disabledEntries[
                        this->replayCursor.disabledEntryCount++] = 1;
                    this->vms[13].color1 = 0x80000000;
                }
                PreparePhotoResultScreen(this);
                this->replayCursor.count = 4;
                this->replayCursor.wraps = 1;
            }
        }
        break;

    case 14:
        if (IsResultMenuInputPressed(0x10))
        {
            this->keyboardSelection -= 16;
            if (this->keyboardSelection < 0)
            {
                this->keyboardSelection += 96;
            }
        }
        if (IsResultMenuInputPressed(0x20))
        {
            this->keyboardSelection += 16;
            if (this->keyboardSelection >= 96)
            {
                this->keyboardSelection -= 96;
            }
        }
        if (IsResultMenuInputPressed(0x40))
        {
            if (this->keyboardSelection % 16 == 0)
            {
                this->keyboardSelection += 15;
            }
            else
            {
                this->keyboardSelection--;
            }
        }
        if (IsResultMenuInputPressed(0x80))
        {
            this->keyboardSelection++;
            if (this->keyboardSelection % 16 == 0)
            {
                this->keyboardSelection -= 16;
            }
        }

        if ((g_PressedButtons & 0x1002) != 0)
        {
            if (this->keyboardSelection == 95)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                this->SetState(15);
            }
            else if (this->keyboardSelection == 94)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
                if (this->replayNameCursor == 7 && this->replayName[7] != ' ')
                {
                    this->replayName[7] = ' ';
                }
                else
                {
                    this->replayNameCursor--;
                    if (this->replayNameCursor < 0)
                    {
                        this->replayNameCursor = 0;
                    }
                    this->replayName[this->replayNameCursor] = ' ';
                }
            }
            else
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                this->replayName[this->replayNameCursor] =
                    g_ResultAlphabet[this->keyboardSelection];
                this->replayNameCursor++;
                if (this->replayNameCursor > 7)
                {
                    this->replayNameCursor = 7;
                    this->keyboardSelection = 95;
                }
            }
        }

        if ((g_PressedButtons & 9) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_BACK, 0);
            if (this->replayNameCursor < 1)
            {
                this->SetState(13);
                this->replayCursor.Pop();
            }
            else
            {
                this->replayNameCursor--;
                this->replayName[this->replayNameCursor] = ' ';
            }
        }
        break;

    case 15:
    {
        char path[0x100];
        sprintf(path, "th95_%.2d.rpy", this->replayCursor.current + 1);
        g_ReplayManager->WriteReplay(path, this->replayName);
        strcpy(g_ResultRuntime->replayName, this->replayName);
        this->SetState(13);
        this->LoadReplays();
        this->replayCursor.Pop();
        break;
    }
    }

    for (i = 0; i < 21; i++)
    {
        ExecuteResultVm(&this->vms[i]);
    }
    this->stateTimer.Tick();
    for (i = 21; i < 25; i++)
    {
        ExecuteResultVm(&this->vms[i]);
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

ChainCallbackResult ResultScreen::OnUpdate(ResultScreen *resultScreen)
{
    if (g_ResultScreenGlobalState->suppressResultCallbacks)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    return resultScreen->Update();
}

ChainCallbackResult ResultScreen::OnDraw(ResultScreen *resultScreen)
{
    if (g_ResultScreenGlobalState->suppressResultCallbacks)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    return resultScreen->Draw();
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
