#pragma once

#include "ZunResult.hpp"
#include "diffbuild.hpp"

#include <stddef.h>

namespace th095
{

enum ChainCallbackResult
{
    CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB = (unsigned int)0,
    CHAIN_CALLBACK_RESULT_CONTINUE = (unsigned int)1,
    CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN = (unsigned int)2,
    CHAIN_CALLBACK_RESULT_BREAK = (unsigned int)3,
    CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS = (unsigned int)4,
    CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR = (unsigned int)5,
    CHAIN_CALLBACK_RESULT_RESTART_FROM_FIRST_JOB = (unsigned int)6,
};

// The target was built with /Gr, so these unannotated free-function types use
// __fastcall. Keep the enum/ZunResult returns: they are part of CreateElem's
// decorated ABI and distinguish the real Chain API from the former int-return
// probe declarations.
typedef ChainCallbackResult (*ChainCallback)(void *);
typedef ZunResult (*ChainLifetimeCallback)(void *);

enum ChainCalcPriority
{
    CHAIN_PRIO_CALC_SUPERVISOR = 0,
    CHAIN_PRIO_CALC_ASCIIMANAGER = 1,
    CHAIN_PRIO_CALC_GAMEMANAGER = 2,
    CHAIN_PRIO_CALC_SCREENEFFECT = 3,
    CHAIN_PRIO_CALC_TITLESCREEN = 4,
    CHAIN_PRIO_CALC_MUSICROOM = 4,
    CHAIN_PRIO_CALC_ENDING = 5,
    CHAIN_PRIO_CALC_REPLAYMANAGER_PLAYBACK_HIGH_PRIO = 6,
    CHAIN_PRIO_CALC_REPLAYMANAGER_LOW_PRIO = 7,
    CHAIN_PRIO_CALC_BACKGROUND = 8,
    CHAIN_PRIO_CALC_PLAYER = 9,
    CHAIN_PRIO_CALC_ENEMYMANAGER = 11,
    CHAIN_PRIO_CALC_SPELLCARD = 12,
    CHAIN_PRIO_CALC_EFFECTMANAGER = 13,
    CHAIN_PRIO_CALC_BULLETMANAGER = 14,
    CHAIN_PRIO_CALC_GUI = 15,
    CHAIN_PRIO_CALC_RESULTSCREEN = 16,
    CHAIN_PRIO_CALC_REPLAYMANAGER_RECORD_HIGH_PRIO = 17,
    CHAIN_PRIO_CALC_REPLAYMANAGER_SKIP_FRAMES = 18,
};

enum ChainDrawPriority
{
    CHAIN_PRIO_DRAW_SUPERVISOR = 0,
    CHAIN_PRIO_DRAW_SUPERVISOR_LOADING_VMS = 2,
    CHAIN_PRIO_DRAW_MUSICROOM = 3,
    CHAIN_PRIO_DRAW_TITLESCREEN = 3,
    CHAIN_PRIO_DRAW_ENDING = 4,
    CHAIN_PRIO_DRAW_GAMEMANAGER = 5,
    CHAIN_PRIO_DRAW_BACKGROUND_HIGH_PRIO = 6,
    CHAIN_PRIO_DRAW_BACKGROUND_LOW_PRIO = 7,
    CHAIN_PRIO_DRAW_ENEMYMANAGER_HIGH_PRIO = 8,
    CHAIN_PRIO_DRAW_PLAYER_HIGH_PRIO = 9,
    CHAIN_PRIO_DRAW_PLAYER_LOW_PRIO = 10,
    CHAIN_PRIO_DRAW_ENEMYMANAGER_LOW_PRIO = 11,
    CHAIN_PRIO_DRAW_EFFECTMANAGER = 12,
    CHAIN_PRIO_DRAW_BULLETMANAGER = 13,
    CHAIN_PRIO_DRAW_ASCIIMANAGER_HIGH_PRIO = 14,
    CHAIN_PRIO_DRAW_SPELLCARD = 15,
    CHAIN_PRIO_DRAW_SUPERVISOR_DRAW_FPS_COUNTER = 16,
    CHAIN_PRIO_DRAW_GUI = 17,
    CHAIN_PRIO_DRAW_RESULTSCREEN = 18,
    CHAIN_PRIO_DRAW_ASCIIMANAGER_LOW_PRIO = 20,
    CHAIN_PRIO_DRAW_SCREENEFFECT = 21,
};

// This must remain a class, not a struct spelling. MSVC records the tag kind
// in decorated names (PAV versus PAU); the target's exact Global.obj API uses
// PAVChainElem throughout.
class ChainElem
{
  public:
    ChainElem();
    ~ChainElem();

    void SetCallback(ChainCallback callback)
    {
        this->callback = callback;
        this->addedCallback = NULL;
        this->deletedCallback = NULL;
    }

    short priority;
    unsigned short isHeapAllocated : 1;
    ChainCallback callback;
    ChainLifetimeCallback addedCallback;
    ChainLifetimeCallback deletedCallback;
    ChainElem *prev;
    ChainElem *next;
    ChainElem *releaseTarget;
    void *arg;
};

typedef char ChainElemSizeIs20[(sizeof(ChainElem) == 0x20) ? 1 : -1];
typedef char ChainElemReleaseTargetAt18[
    (offsetof(ChainElem, releaseTarget) == 0x18) ? 1 : -1];

class Chain
{
  private:
    ChainElem calcChain;
    ChainElem drawChain;

    void ReleaseSingleChain(ChainElem *root);
    void CutImpl(ChainElem *toRemove);

  public:
    Chain();
    ~Chain();

    void Cut(ChainElem *toRemove);
    void Release();
    int AddToCalcChain(ChainElem *elem, int priority);
    int AddToDrawChain(ChainElem *elem, int priority);
    int RunDrawChain();
    int RunCalcChain();
    ChainElem *CreateElem(ChainCallback callback);
};

typedef char ChainSizeIs40[(sizeof(Chain) == 0x40) ? 1 : -1];

DIFFABLE_EXTERN(Chain, g_Chain);

} // namespace th095
