#ifndef TH095_RESULT_SCREEN_HPP
#define TH095_RESULT_SCREEN_HPP

#include "Global.hpp"
#include "AnmVmId.hpp"
#include "ReplayManager.hpp"

namespace th095
{

struct ResultScreenTimer
{
    i32 previous;
    f32 subFrame;
    i32 current;

    u32 operator==(i32 value) { return this->current == value; }
    u32 operator<(i32 value) { return this->current < value; }
    i32 Tick();

    void Reset()
    {
        this->current = 0;
        this->subFrame = 0.0f;
        this->previous = -999999;
    }
};

typedef char ResultScreenTimerSizeIsC[
    (sizeof(ResultScreenTimer) == 0x0c) ? 1 : -1];

struct ResultScreenReplayCursor
{
    i32 current;
    i32 previous;
    i32 count;
    i32 savedCurrent[16];
    i32 savedCount[16];
    i32 saveDepth;
    i32 disabledEntries[16];
    i32 wraps;
    i32 disabledEntryCount;

    i32 Move(i32 amount);
    void Push();
    void Pop();

    i32 GetCurrent() { return this->current; }
    i32 GetPrevious() { return this->previous; }
    i32 GetCount() { return this->count; }
    void SaveCurrent() { this->previous = this->current; }
    u32 HasChanged() { return this->previous != this->current; }

    void Set(i32 value)
    {
        if (this->count != 0)
        {
            this->current = value >= this->count
                                ? this->count - 1
                                : (value < 0 ? 0 : value);
        }
        else
        {
            this->current = value;
        }
    }
};

struct ResultScreenAnmVm
{
    u8 unknown000[0x40];
    struct
    {
        f32 x;
        f32 y;
    } spriteSize;
    u8 unknown048[0x220 - 0x48];
    u32 color1;
    u8 unknown224[0x22e - 0x224];
    i16 pendingInterrupt;
    u8 unknown230[0x244 - 0x230];
    struct ResultScreenLoadedSpriteView *loadedSprite;
    u8 unknown248[0x2cc - 0x248];

    void SetInterrupt(i32 interrupt)
    {
        this->pendingInterrupt = interrupt;
    }
};

typedef char ResultScreenAnmVmSizeIs2CC[
    (sizeof(ResultScreenAnmVm) == 0x2cc) ? 1 : -1];

struct ResultScreenAnmLoadedView
{
    u8 unknown000[0x14];
    struct ResultScreenTextureEntryView *textures;

    void SetAndExecuteScript(ResultScreenAnmVm *vm, i32 scriptIndex);
};

struct ResultScreenTextureEntryView
{
    u8 unknown000[0x0c];
    i32 format;
};

struct ResultScreenLoadedSpriteView
{
    u8 unknown000[0x28];
    f32 uvEndX;
    f32 uvEndY;
};

struct ResultPhotoSlotView
{
    u8 unknown0000[0x21d4];
    u32 metadata[8];
    i32 score;                        // +0x21f4
    i32 replayValue;                  // +0x21f8
    i32 stageValue;                   // +0x21fc
    u16 width;                        // +0x2200
    u16 unknown2202;
    u16 height;                       // +0x2204
    u16 unknown2206;
    char comment[12];                 // +0x2208
};

struct ResultPhotoDataView
{
    ResultPhotoSlotView slots[11];
    u8 unknown176dc[0x17720 - 0x176dc];
    AnmVmId photoVms[11];
    u8 unknown1774c[0x2571c - 0x1774c];
    ResultScreenAnmLoadedView *anm;

    i32 FindBestShot();
};

struct ResultPhotoControllerView
{
    u8 unknown000[0x29ec];
    i32 photoCount;

    i32 GetPhotoCount() { return this->photoCount; }
};

typedef char ResultPhotoSlotViewSizeIs2214[
    (sizeof(ResultPhotoSlotView) == 0x2214) ? 1 : -1];
typedef char ResultPhotoSlotMetadataAt21D4[
    (offsetof(ResultPhotoSlotView, metadata) == 0x21d4) ? 1 : -1];
typedef char ResultPhotoSlotScoreAt21F4[
    (offsetof(ResultPhotoSlotView, score) == 0x21f4) ? 1 : -1];
typedef char ResultPhotoSlotReplayValueAt21F8[
    (offsetof(ResultPhotoSlotView, replayValue) == 0x21f8) ? 1 : -1];
typedef char ResultPhotoSlotStageValueAt21FC[
    (offsetof(ResultPhotoSlotView, stageValue) == 0x21fc) ? 1 : -1];
typedef char ResultPhotoSlotWidthAt2200[
    (offsetof(ResultPhotoSlotView, width) == 0x2200) ? 1 : -1];
typedef char ResultPhotoSlotHeightAt2204[
    (offsetof(ResultPhotoSlotView, height) == 0x2204) ? 1 : -1];
typedef char ResultPhotoSlotCommentAt2208[
    (offsetof(ResultPhotoSlotView, comment) == 0x2208) ? 1 : -1];

struct ResultScreen
{
    ResultScreenAnmLoadedView *anm;       // +0x0000
    i32 state;                            // +0x0004
    ResultScreenTimer stateTimer;         // +0x0008
    f32 savedGameSpeed;                   // +0x0014
    ResultScreenAnmVm vms[25];            // +0x0018
    ResultScreenReplayCursor replayCursor; // +0x4604
    i32 keyboardSelection;                // +0x46dc
    i32 replayNameCursor;                 // +0x46e0
    u8 unknown46e4[0x6ce8 - 0x46e4];
    ReplayManager *replays[20];            // +0x6ce8
    u8 unknown6d38[4];
    char replayName[9];                   // +0x6d3c
    u8 unknown6d45[3];
    ResultScreenReplayCursor photoCursor; // +0x6d48
    i32 notificationTimer;                // +0x6e20

    i32 UpdateCursor(i32 firstVm);
    void PrepareBestShot();
    ZunResult LoadReplays();
    ChainCallbackResult Update();
    ChainCallbackResult Draw();
    static ChainCallbackResult OnUpdate(ResultScreen *resultScreen);
    static ChainCallbackResult OnDraw(ResultScreen *resultScreen);

    void SetState(i32 value)
    {
        this->state = value;
        this->stateTimer.Reset();
    }
};

typedef char ResultScreenVmsAt18[
    (offsetof(ResultScreen, vms) == 0x18) ? 1 : -1];
typedef char ResultScreenReplayCursorAt4604[
    (offsetof(ResultScreen, replayCursor) == 0x4604) ? 1 : -1];
typedef char ResultScreenReplayCursorSizeIsD8[
    (sizeof(ResultScreenReplayCursor) == 0xd8) ? 1 : -1];
typedef char ResultScreenReplaysAt6CE8[
    (offsetof(ResultScreen, replays) == 0x6ce8) ? 1 : -1];
typedef char ResultScreenReplayNameAt6D3C[
    (offsetof(ResultScreen, replayName) == 0x6d3c) ? 1 : -1];
typedef char ResultScreenPhotoCursorAt6D48[
    (offsetof(ResultScreen, photoCursor) == 0x6d48) ? 1 : -1];
typedef char ResultScreenNotificationTimerAt6E20[
    (offsetof(ResultScreen, notificationTimer) == 0x6e20) ? 1 : -1];

} // namespace th095

#endif
