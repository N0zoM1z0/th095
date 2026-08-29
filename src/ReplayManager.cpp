#include "ReplayManager.hpp"
#include "ZunMath.hpp"

#include <stdlib.h>
#include <string.h>

namespace th095
{

struct ReplayInputSource
{
    u16 currentInput;
    u8 unknown002[4];
    u16 pressedInput;

    void Update();
};

extern ReplayInputSource g_ReplayInputSource;
extern u16 g_ReplayInputAux;
extern u16 g_ReplayInputFlags;
extern f32 g_ReplayFps;

struct ReplayAsciiManagerView
{
    u8 unknown0000[0x806c];
    u32 color;

    void AddFormatText(Float3 *position, const char *format, ...);

    void SetColor(u32 value)
    {
        this->color = value;
    }
};

typedef char ReplayAsciiManagerColorAt806c[
    (offsetof(ReplayAsciiManagerView, color) == 0x806c) ? 1 : -1];

extern ReplayAsciiManagerView g_ReplayAsciiManager;

struct ReplayGlobalStateView
{
    u8 unknown000[0xfc];
    u32 unknownFlag0 : 1;
    u32 unknownFlag1 : 1;
    u32 suppressReplayCallbacks : 1;
    u32 unknownFlags : 29;
};

extern ReplayGlobalStateView *g_ReplayGlobalState;

struct ReplayFrameScratch
{
    u32 unused[2];

    ReplayFrameScratch()
    {
    }
};

ReplayManager::ReplayManager()
{
    utils::DebugPrint("HDinitialize ReplayInf\n");
    memset(this, 0, sizeof(ReplayManager));
}

ReplayManager::~ReplayManager()
{
    struct FreeSlots
    {
        void *fileHeader;
        void *fpsData;
        void *inputData;
    } freeSlots;

    utils::DebugPrint("shitdown ReplayInf\n");
    if (this->inputData != NULL)
    {
        freeSlots.inputData = this->inputData;
        free(freeSlots.inputData);
    }
    if (this->mode == REPLAY_MANAGER_RECORD && this->fpsData != NULL)
    {
        freeSlots.fpsData = this->fpsData;
        free(freeSlots.fpsData);
    }
    if (this->fileHeader != NULL)
    {
        freeSlots.fileHeader = this->fileHeader;
        free(freeSlots.fileHeader);
    }
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);
    if (this == g_ReplayManager)
    {
        g_ReplayManager = NULL;
    }
}

ReplayManager *ReplayManager::Create(i32 mode, char *path)
{
    ChainElem *elem;
    ReplayManager *replayManager;

    replayManager = new ReplayManager();
    if (replayManager->Initialize(mode, path) != ZUN_SUCCESS)
    {
        goto failure;
    }

    elem = g_Chain.CreateElem((ChainCallback)ReplayManager::OnUpdate);
    elem->arg = replayManager;
    g_Chain.AddToCalcChain(elem, 7);
    replayManager->calcChain = elem;

    elem = g_Chain.CreateElem((ChainCallback)ReplayManager::OnDraw);
    elem->arg = replayManager;
    g_Chain.AddToDrawChain(elem, 3);
    replayManager->drawChain = elem;
    return replayManager;

failure:
    if (replayManager != NULL)
    {
        delete replayManager;
        replayManager = NULL;
    }
    return NULL;
}

ReplayManager *ReplayManager::Load(char *path)
{
    ReplayManager *replayManager;

    replayManager = new ReplayManager();
    if (replayManager->Initialize(REPLAY_MANAGER_LOAD_ONLY, path) != ZUN_SUCCESS)
    {
        delete replayManager;
        replayManager = NULL;
        return NULL;
    }
    return replayManager;
}

void ReplayManager::Destroy(ReplayManager *replayManager)
{
    ReplayManager *manager = replayManager;

    if (manager != NULL)
    {
        delete manager;
        manager = NULL;
    }
}

ChainCallbackResult ReplayManager::ProcessFrame()
{
    ReplayFrameScratch conversionScratch;

    if (this->mode == REPLAY_MANAGER_RECORD)
    {
        g_LastFrameInput = g_CurFrameInput;
        g_CurFrameInput = g_ReplayInputSource.currentInput;
        g_ReplayInputSource.Update();
        g_ReplayInputAux = g_ReplayInputSource.pressedInput;

        if ((u32)(this->inputCursor - (u8 *)this->inputData) >= 0x69780)
        {
            utils::DebugPrint("error : replay byffer over\n");
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        }

        *(u16 *)(this->inputCursor + 0) = g_CurFrameInput;
        *(u16 *)(this->inputCursor + 2) = g_ReplayInputAux;
        *(u16 *)(this->inputCursor + 4) = g_ReplayInputFlags;

        if (this->frameCounter % 30 == 0)
        {
            *this->fpsCursor = 255.0f <= g_ReplayFps + 0.5f
                                   ? 0xff
                                   : (u8)(g_ReplayFps + 0.5f);
            this->fpsCursor++;
        }
    }
    else
    {
        g_LastFrameInput = g_CurFrameInput;
        g_CurFrameInput = *(u16 *)(this->inputCursor + 0);
        g_ReplayInputAux = *(u16 *)(this->inputCursor + 2);
        g_ReplayInputFlags = *(u16 *)(this->inputCursor + 4);

        if (this->frameCounter % 30 == 0)
        {
            this->replayFps = *this->fpsCursor;
            this->fpsCursor++;
        }
    }

    this->inputCursor += 6;
    this->frameCounter++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

ChainCallbackResult ReplayManager::DrawFps()
{
    Float3 position;

    if (this->mode == REPLAY_MANAGER_PLAYBACK)
    {
        g_ReplayAsciiManager.SetColor(
            (f32)this->replayFps < 30.0f
                ? 0xff5050ff
                : ((f32)this->replayFps < 50.0f ? 0xffa0a0ff : 0xffffffff));
        position.x = 485.0f;
        position.y = 452.0f;
        position.z = 0.0f;
        g_ReplayAsciiManager.AddFormatText(&position, "%3d", this->replayFps);
        g_ReplayAsciiManager.SetColor(0xffffffff);
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

ChainCallbackResult ReplayManager::OnUpdate(ReplayManager *replayManager)
{
    if (g_ReplayGlobalState->suppressReplayCallbacks)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    return replayManager->ProcessFrame();
}

ChainCallbackResult ReplayManager::OnDraw(ReplayManager *replayManager)
{
    if (g_ReplayGlobalState->suppressReplayCallbacks)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    return replayManager->DrawFps();
}

} // namespace th095
