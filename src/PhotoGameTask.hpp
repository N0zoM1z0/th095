#ifndef TH095_PHOTO_GAME_TASK_HPP
#define TH095_PHOTO_GAME_TASK_HPP

#include "Main.hpp"
#include "ZunTimer.hpp"

namespace th095
{

struct Background;
struct PhotoBulletManagerView;
struct PhotoEffectManagerView;
struct PhotoEnemyManagerTaskView;
struct PhotoFrontManagerView;
struct PhotoGameUpdateView;
struct PhotoItemManagerView;
struct PhotoOverlayManagerView;
struct ReplayManager;
struct ResultScreen;

struct PhotoCompletionStateTaskView
{
    i32 unknown000;
    ZunTimer timer;
};

// Production-visible layout of the live photography task.  The exact build
// keeps its separately frozen target-facing view in PhotoGameTaskExact.inl.
struct PhotoGameTaskView
{
    Background *background;                 // +0x000
    PhotoFrontManagerView *front;            // +0x004
    PhotoBulletManagerView *bullets;         // +0x008
    PhotoGameUpdateView *player;             // +0x00c
    ReplayManager *replay;                   // +0x010
    PhotoEnemyManagerTaskView *enemies;      // +0x014
    PhotoOverlayManagerView *photoOverlay;   // +0x018
    PhotoItemManagerView *items;             // +0x01c
    ResultScreen *pause;                     // +0x020
    PhotoEffectManagerView *lasers;          // +0x024
    ZunTimer stageTimer;                     // +0x028
    GameConfiguration runtimeConfig;        // +0x034
    u32 flags;                               // +0x0fc
    i32 bestShotIndex;                       // +0x100
    PhotoCompletionStateTaskView completion; // +0x104
    i32 score;                               // +0x114
    ChainElem *calcChain;                    // +0x118
    ChainElem *drawChain;                    // +0x11c
    i32 replayMode;                          // +0x120

    PhotoGameTaskView();
    ~PhotoGameTaskView();

    static PhotoGameTaskView *__fastcall Create(i32 replayMode);
    void Destroy();
    static void __fastcall Load(void *argument);
    static i32 __fastcall OnUpdate(PhotoGameTaskView *task);
    static i32 __fastcall OnDraw(PhotoGameTaskView *task);
    i32 InitializeSubsystems();
    i32 Update();
    i32 DrawHud();
};

typedef char PhotoGameTaskSizeIs124[
    (sizeof(PhotoGameTaskView) == 0x124) ? 1 : -1];
typedef char PhotoGameTaskConfigAt34[
    (offsetof(PhotoGameTaskView, runtimeConfig) == 0x34) ? 1 : -1];
typedef char PhotoGameTaskCompletionAt104[
    (offsetof(PhotoGameTaskView, completion) == 0x104) ? 1 : -1];
typedef char PhotoGameTaskCompletionTimerAt108[
    (offsetof(PhotoGameTaskView, completion.timer) == 0x108) ? 1 : -1];
typedef char PhotoGameTaskChainsAt118[
    (offsetof(PhotoGameTaskView, calcChain) == 0x118) ? 1 : -1];

} // namespace th095

#endif
