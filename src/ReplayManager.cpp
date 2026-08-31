#include "ReplayManager.hpp"
#include "ReplayInputSource.hpp"
#include "ZunMath.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <direct.h>

namespace th095
{

extern u16 g_ReplayInputAux;
extern u16 g_ReplayInputFlags;
extern f32 g_ReplayFps;
extern u16 g_ReplayRngSeed;

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
    u8 unknown000[0x34];
    u8 replayStateSnapshot[0xc8];
    u32 unknownFlag0 : 1;
    u32 unknownFlag1 : 1;
    u32 suppressReplayCallbacks : 1;
    u32 unknownFlags : 29;
};

extern ReplayGlobalStateView *g_ReplayGlobalState;

struct ReplayPlayerConfigView
{
    u16 id;
    u8 unknown002[2];
    i8 group;
    u8 unknown005[3];
    i8 variant;
    u8 unknown009[0x27];
};

typedef char ReplayPlayerConfigSizeIs30[
    (sizeof(ReplayPlayerConfigView) == 0x30) ? 1 : -1];

extern ReplayPlayerConfigView *g_ReplayPlayerConfig;
extern ReplayPlayerConfigView *g_ReplayPlayerConfigTable[];
extern i32 g_ReplayUsesArchive;
extern f64 g_ReplayLagNumerator;
extern f64 g_ReplayLagDenominator;

namespace ReplayFile
{
int Create(char *path);
int Open(char *path);
int Write(void *data, u32 size);
void *Read(u32 size);
void Close();
};

namespace ReplayLzss
{
u8 *Encode(u8 *input, i32 inputSize, i32 *outputSize);
u8 *Decode(u8 *input, i32 inputSize, u8 *output, i32 outputSize);
};

struct ReplayUserDataHeader
{
    u32 magic;
    i32 size;
    u8 type;
    u8 padding[3];
};

typedef char ReplayUserDataHeaderSizeIs0C[
    (sizeof(ReplayUserDataHeader) == 0x0c) ? 1 : -1];

struct ReplayFrameScratch
{
    u32 unused[2];

    ReplayFrameScratch()
    {
    }
};

struct ReplayInitializeScratch
{
    u16 unused000;
    u16 rngSeed;
    u32 fpsSize;
    u32 inputSize;
    u32 headerSize;

    ReplayInitializeScratch()
    {
    }
};

struct ReplayLoadLocals
{
    u32 allocationSize;
    i32 fileSize;
    char fullPath[0x100];
    ReplayInputData *inputData;
    u8 *compressedData;
};

struct ReplayWriteLocals
{
    i32 totalStreamSize;
    tm *localTime;
    ReplayUserDataHeader *userDataHeader;
    u8 *userData;
    char *userDataCursor;
    i32 i;
    char fullPath[0x100];
    ReplayInputData *inputData;
    i32 compressedSize;
    u8 *uncompressedData;
    u8 *compressedData;
};

ReplayManager::ReplayManager()
{
    utils::DebugPrint("HDinitialize ReplayInf\n");
    memset(this, 0, sizeof(ReplayManager));
}

ZunResult ReplayManager::Initialize(i32 mode, char *path)
{
    ReplayInitializeScratch scratch;

    this->mode = mode;
    if (this->mode == REPLAY_MANAGER_RECORD)
    {
        g_ReplayManager = this;
        scratch.headerSize = sizeof(ReplayFileHeader);
        this->fileHeader = (ReplayFileHeader *)malloc(scratch.headerSize);
        scratch.inputSize = 0x69780;
        this->inputData = (ReplayInputData *)malloc(scratch.inputSize);
        scratch.fpsSize = 0x11940;
        this->fpsData = (u8 *)malloc(scratch.fpsSize);

        memset(this->fileHeader, 0, sizeof(ReplayFileHeader));
        memset(this->inputData, 0, 0x69780);
        memset(this->fpsData, 0, 0x11940);

        this->fileHeader->magic = 0x72353974;
        this->fileHeader->version = 1;
        this->fileHeader->gameVersion = 0x102;

        this->activeInputData = this->inputData;
        this->inputCursor =
            (u8 *)this->activeInputData + sizeof(ReplayInputData);
        this->fpsCursor = this->fpsData;

        this->activeInputData->playerConfigId = g_ReplayPlayerConfig->id;
        this->activeInputData->level = g_ReplayPlayerConfig->group;
        this->activeInputData->scene =
            g_ReplayPlayerConfig->variant;
        memcpy(this->activeInputData->globalStateSnapshot,
               g_ReplayGlobalState->replayStateSnapshot,
               sizeof(this->activeInputData->globalStateSnapshot));
        this->activeInputData->rngSeed = g_ReplayRngSeed;
    }
    else if (this->mode == REPLAY_MANAGER_PLAYBACK)
    {
        g_ReplayManager = this;
        if (this->LoadReplay(path) != ZUN_SUCCESS)
        {
            return ZUN_ERROR;
        }

        this->activeInputData = this->inputData;
        this->inputCursor =
            (u8 *)this->activeInputData + sizeof(ReplayInputData);
        this->fpsCursor = this->fpsData;
        scratch.rngSeed = this->activeInputData->rngSeed;
        g_ReplayRngSeed = scratch.rngSeed;
        g_ReplayPlayerConfig =
            g_ReplayPlayerConfigTable[this->activeInputData->level] +
            this->activeInputData->scene;
        memcpy(g_ReplayGlobalState->replayStateSnapshot,
               this->activeInputData->globalStateSnapshot,
               sizeof(this->activeInputData->globalStateSnapshot));
    }
    else if (this->mode == REPLAY_MANAGER_LOAD_ONLY)
    {
        if (this->LoadReplay(path) != ZUN_SUCCESS)
        {
            return ZUN_ERROR;
        }
        this->activeInputData = this->inputData;
    }
    return ZUN_SUCCESS;
}

ZunResult ReplayManager::LoadReplay(char *path)
{
    ReplayLoadLocals locals;

    locals.compressedData = NULL;
    strcpy(this->path, path);

    if (g_ReplayUsesArchive == 0)
    {
        sprintf(locals.fullPath, "replay/%s", path);
        if (!FileSystem::CheckIfFileAlreadyExists(locals.fullPath))
        {
            return ZUN_ERROR;
        }
        if (ReplayFile::Open(locals.fullPath) != ZUN_SUCCESS)
        {
            return ZUN_ERROR;
        }
        this->fileHeader =
            (ReplayFileHeader *)ReplayFile::Read(sizeof(ReplayFileHeader));
        locals.compressedData =
            (u8 *)ReplayFile::Read(this->fileHeader->compressedSize);
        ReplayFile::Close();
    }
    else
    {
        this->fileHeader = (ReplayFileHeader *)FileSystem::OpenFile(
            path, &locals.fileSize, FALSE);
        locals.compressedData = (u8 *)this->fileHeader + sizeof(ReplayFileHeader);
    }

    locals.allocationSize = this->fileHeader->decompressedSize;
    this->inputData = (ReplayInputData *)malloc(locals.allocationSize);
    FileSystem::Decrypt(locals.compressedData, this->fileHeader->compressedSize,
                        0xaa, 0xe1, 0x400,
                        this->fileHeader->compressedSize);
    FileSystem::Decrypt(locals.compressedData, this->fileHeader->compressedSize,
                        0x3d, 0x7a, 0x80,
                        this->fileHeader->compressedSize);
    ReplayLzss::Decode(locals.compressedData, this->fileHeader->compressedSize,
                       (u8 *)this->inputData,
                       this->fileHeader->decompressedSize);

    locals.inputData = this->inputData;
    this->fpsData = (u8 *)(locals.inputData->inputStreamSize +
                           sizeof(ReplayInputData) + (u32)this->inputData);
    if (g_ReplayUsesArchive == 0)
    {
        free(locals.compressedData);
    }
    return ZUN_SUCCESS;
}

ZunResult ReplayManager::WriteReplay(char *path, char *replayName)
{
    i32 userDataAllocationSize;
    ReplayWriteLocals locals;

    locals.inputData = this->inputData;
    strcpy(locals.inputData->replayName, replayName);
    for (locals.i = strlen(replayName);
         locals.i < 8; locals.i++)
    {
        locals.inputData->replayName[locals.i] = ' ';
    }

    locals.inputData->inputStreamSize =
        this->inputCursor - ((u8 *)locals.inputData + sizeof(ReplayInputData));
    locals.inputData->fpsStreamSize = this->fpsCursor - this->fpsData;
    locals.inputData->slowRate =
        100.0f - (f32)(g_ReplayLagNumerator / g_ReplayLagDenominator) * 100.0f;

    _mkdir("replay");
    sprintf(locals.fullPath, "replay/%s", path);

    locals.totalStreamSize =
        sizeof(ReplayInputData) + locals.inputData->inputStreamSize +
        locals.inputData->fpsStreamSize;
    locals.uncompressedData = (u8 *)malloc(locals.totalStreamSize);
    memcpy(locals.uncompressedData, this->inputData,
           sizeof(ReplayInputData) + locals.inputData->inputStreamSize);
    memcpy(locals.uncompressedData + sizeof(ReplayInputData) +
               locals.inputData->inputStreamSize,
           this->fpsData, locals.inputData->fpsStreamSize);

    locals.compressedData = ReplayLzss::Encode(
        locals.uncompressedData,
        sizeof(ReplayInputData) + locals.inputData->inputStreamSize +
            locals.inputData->fpsStreamSize,
        &locals.compressedSize);
    free(locals.uncompressedData);

    FileSystem::Encrypt(locals.compressedData, locals.compressedSize, 0x3d,
                        0x7a, 0x80, locals.compressedSize);
    FileSystem::Encrypt(locals.compressedData, locals.compressedSize, 0xaa,
                        0xe1, 0x400, locals.compressedSize);

    this->fileHeader->decompressedSize =
        sizeof(ReplayInputData) + locals.inputData->inputStreamSize +
        locals.inputData->fpsStreamSize;
    this->fileHeader->compressedSize = locals.compressedSize;
    this->fileHeader->fileSize =
        this->fileHeader->compressedSize + sizeof(ReplayFileHeader);

    ReplayFile::Create(locals.fullPath);
    ReplayFile::Write(this->fileHeader, sizeof(ReplayFileHeader));
    ReplayFile::Write(locals.compressedData, locals.compressedSize);
    free(locals.compressedData);

    userDataAllocationSize = 0xffff;
    locals.userData = (u8 *)malloc(userDataAllocationSize);
    memset(locals.userData, 0, 0xffff);
    locals.userDataHeader = (ReplayUserDataHeader *)locals.userData;
    locals.userDataHeader->magic = 0x52455355;
    locals.userDataHeader->type = 0;
    locals.userDataCursor = (char *)(locals.userDataHeader + 1);
    locals.userDataCursor += sprintf(
        locals.userDataCursor,
        "\x93\x8c\x95\xfb\x95\xb6\x89\xd4\x92\x9f\x20\x83\x8a\x83\x76"
        "\x83\x8c\x83\x43\x83\x74\x83\x40\x83\x43\x83\x8b\x8f\xee"
        "\x95\xf1\r\n");
    locals.userDataCursor +=
        sprintf(locals.userDataCursor, "Version %s\r\n", "1.02a");
    locals.userDataCursor += sprintf(locals.userDataCursor, "Name %s\r\n",
                                     locals.inputData->replayName);
    if (locals.inputData->level == 10)
    {
        locals.userDataCursor +=
            sprintf(locals.userDataCursor, "Level EX\r\n");
    }
    else
    {
        locals.userDataCursor += sprintf(locals.userDataCursor,
                                         "Level %d\r\n",
                                         locals.inputData->level + 1);
    }
    locals.userDataCursor += sprintf(locals.userDataCursor, "Scene %d\r\n",
                                     locals.inputData->scene + 1);
    locals.localTime = localtime((time_t *)&locals.inputData->timestamp);
    locals.userDataCursor += sprintf(
        locals.userDataCursor, "Date %.2d/%.2d/%.2d %.2d:%.2d\r\n",
        locals.localTime->tm_year % 100, locals.localTime->tm_mon + 1,
        locals.localTime->tm_mday, locals.localTime->tm_hour,
        locals.localTime->tm_min);
    locals.userDataCursor += sprintf(locals.userDataCursor, "Score %d\r\n",
                                     locals.inputData->score);
    locals.userDataCursor +=
        sprintf(locals.userDataCursor, "Slow Rate %2.2f\r\n",
                locals.inputData->slowRate);
    locals.userDataCursor++;
    if ((locals.userDataCursor - (char *)locals.userData) % 4 != 0)
    {
        locals.userDataCursor +=
            4 - (locals.userDataCursor - (char *)locals.userData) % 4;
    }
    locals.userDataHeader->size =
        locals.userDataCursor - (char *)locals.userData;
    ReplayFile::Write(locals.userData,
                      locals.userDataCursor - (char *)locals.userData);

    memset(locals.userData, 0, 0xffff);
    locals.userDataHeader = (ReplayUserDataHeader *)locals.userData;
    locals.userDataHeader->magic = 0x52455355;
    locals.userDataHeader->type = 1;
    locals.userDataCursor = (char *)(locals.userDataHeader + 1);
    locals.userDataCursor += sprintf(
        locals.userDataCursor,
        "\x83\x52\x83\x81\x83\x93\x83\x67\x82\xf0\x8f\x91\x82\xaf"
        "\x82\xdc\x82\xb7");
    locals.userDataCursor++;
    if ((locals.userDataCursor - (char *)locals.userData) % 4 != 0)
    {
        locals.userDataCursor +=
            4 - (locals.userDataCursor - (char *)locals.userData) % 4;
    }
    locals.userDataHeader->size =
        locals.userDataCursor - (char *)locals.userData;
    ReplayFile::Write(locals.userData,
                      locals.userDataCursor - (char *)locals.userData);
    free(locals.userData);
    ReplayFile::Close();
    return ZUN_SUCCESS;
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
