#include "AnmManager.hpp"

#include <stdio.h>

namespace th095
{

struct PhotoBackgroundManagerView
{
    static PhotoBackgroundManagerView *Create();
    void Destroy();
};

struct PhotoFrontManagerView
{
    static PhotoFrontManagerView *Create();
    void Destroy();
};

struct PhotoBulletManagerTaskView
{
    static PhotoBulletManagerTaskView *Create();
    void Destroy();
};

struct PhotoOverlayManagerView
{
    static PhotoOverlayManagerView *Create();
    void Destroy();
};

struct PhotoPlayerManagerView
{
    static PhotoPlayerManagerView *Create();
    void Destroy();
};

struct PhotoEnemyManagerTaskView
{
    static PhotoEnemyManagerTaskView *Create();
    void Destroy();
};

struct PhotoItemManagerTaskView
{
    static PhotoItemManagerTaskView *Create();
    void Destroy();
};

struct PhotoPauseManagerView
{
    static PhotoPauseManagerView *Create();
    void Destroy();
};

struct PhotoLaserManagerTaskView
{
    static PhotoLaserManagerTaskView *Create();
    void Destroy();
};

struct ReplayManagerTaskView
{
    static ReplayManagerTaskView *__fastcall Create(i32 mode, char *path);
    void Destroy();
};

struct PhotoEclFileManagerView
{
    void Destroy();
};

struct PhotoGameFileSystemView
{
    static BOOL CheckIfFileAlreadyExists(LPCSTR path);
};

struct PhotoGameSupervisorView
{
    i32 LoadMusic(i32 slot, char *path);
    i32 StopAudio();
};

struct PhotoRuntimeConfigView
{
    u32 values[50];
};

typedef char PhotoRuntimeConfigSizeIsC8[
    (sizeof(PhotoRuntimeConfigView) == 0xc8) ? 1 : -1];

struct PhotoSceneDefinitionTaskView
{
    i32 bestShotIndex;                  // +0x00
    i32 level;                          // +0x04
    i32 scene;                          // +0x08
    u8 unknown0c[0x0c];
    char *musicPath;                    // +0x18
};

struct PhotoSceneSaveDataTaskView
{
    u8 bytes[1];
};

struct PhotoGameTaskView
{
    PhotoBackgroundManagerView *background; // +0x000
    PhotoFrontManagerView *front;            // +0x004
    PhotoBulletManagerTaskView *bullets;     // +0x008
    PhotoPlayerManagerView *player;          // +0x00c
    ReplayManagerTaskView *replay;           // +0x010
    PhotoEnemyManagerTaskView *enemies;      // +0x014
    PhotoOverlayManagerView *photoOverlay;   // +0x018
    PhotoItemManagerTaskView *items;         // +0x01c
    PhotoPauseManagerView *pause;            // +0x020
    PhotoLaserManagerTaskView *lasers;       // +0x024
    u8 unknown028[0x0c];
    PhotoRuntimeConfigView runtimeConfig;    // +0x034
    i32 unknown0fc;
    i32 bestShotIndex;                       // +0x100
    u8 unknown104[0x14];
    ChainElem *calcChain;                    // +0x118
    ChainElem *drawChain;                    // +0x11c
    i32 replayMode;                          // +0x120

    i32 InitializeSubsystems();
    void ShutdownSubsystems();
};

typedef char PhotoGameTaskSizeIs124[
    (sizeof(PhotoGameTaskView) == 0x124) ? 1 : -1];
typedef char PhotoGameTaskConfigAt34[
    (offsetof(PhotoGameTaskView, runtimeConfig) == 0x34) ? 1 : -1];
typedef char PhotoGameTaskChainsAt118[
    (offsetof(PhotoGameTaskView, calcChain) == 0x118) ? 1 : -1];

extern PhotoRuntimeConfigView g_PhotoRuntimeConfig;
extern char g_ReplayPath[];
extern PhotoSceneDefinitionTaskView *g_PhotoSceneDefinition;
extern PhotoSceneSaveDataTaskView *g_PhotoSceneSaveData;
extern PhotoEclFileManagerView *g_PhotoEclFileManager;
extern PhotoGameTaskView *g_PhotoGameTask;
extern PhotoGameSupervisorView g_PhotoGameSupervisor;
extern u32 g_ControllerRuntimeFlags;
extern i32 g_ReplayUsesArchive;
extern double g_PhotoGameClock;
extern double g_PhotoGameClock2;
extern u32 g_PhotoScreenFadeColor;

i32 PhotoGameTaskView::InitializeSubsystems()
{
    char bestShotPath[0x108];

    this->runtimeConfig = g_PhotoRuntimeConfig;
    this->replay = ReplayManagerTaskView::Create(
        this->replayMode, g_ReplayPath);
    if (this->replay == NULL)
    {
        return ZUN_ERROR;
    }

    this->bestShotIndex = g_PhotoSceneDefinition->bestShotIndex;
    utils::DebugPrint(
        "Start %d-%d\n",
        g_PhotoSceneDefinition->level + 1,
        g_PhotoSceneDefinition->scene + 1);

    if (g_PhotoSceneDefinition->level != 10)
    {
        sprintf(
            bestShotPath,
            "bestshot/bs_%.2d_%d.dat",
            g_PhotoSceneDefinition->level + 1,
            g_PhotoSceneDefinition->scene + 1);
    }
    else
    {
        sprintf(
            bestShotPath,
            "bestshot/bs_ex_%d.dat",
            g_PhotoSceneDefinition->scene + 1);
    }

    if (!PhotoGameFileSystemView::CheckIfFileAlreadyExists(bestShotPath))
    {
        *reinterpret_cast<i32 *>(
            &g_PhotoSceneSaveData->bytes[
                0x49c + this->bestShotIndex * 0x60]) = 0;
        *reinterpret_cast<i32 *>(
            &g_PhotoSceneSaveData->bytes[
                0x478 + this->bestShotIndex * 0x60]) = 0;
    }

    this->background = PhotoBackgroundManagerView::Create();
    if (this->background == NULL)
    {
        return ZUN_ERROR;
    }
    this->front = PhotoFrontManagerView::Create();
    if (this->front == NULL)
    {
        return ZUN_ERROR;
    }
    this->bullets = PhotoBulletManagerTaskView::Create();
    if (this->bullets == NULL)
    {
        return ZUN_ERROR;
    }
    this->photoOverlay = PhotoOverlayManagerView::Create();
    if (this->photoOverlay == NULL)
    {
        return ZUN_ERROR;
    }
    this->player = PhotoPlayerManagerView::Create();
    if (this->player == NULL)
    {
        return ZUN_ERROR;
    }
    this->enemies = PhotoEnemyManagerTaskView::Create();
    if (this->enemies == NULL)
    {
        return ZUN_ERROR;
    }
    this->items = PhotoItemManagerTaskView::Create();
    if (this->items == NULL)
    {
        return ZUN_ERROR;
    }
    this->pause = PhotoPauseManagerView::Create();
    if (this->pause == NULL)
    {
        return ZUN_ERROR;
    }
    this->lasers = PhotoLaserManagerTaskView::Create();
    if (this->lasers == NULL)
    {
        return ZUN_ERROR;
    }

    if (((g_ControllerRuntimeFlags >> 9) & 1) == 0 &&
        g_ReplayUsesArchive == 0)
    {
        g_PhotoGameSupervisor.LoadMusic(
            0, g_PhotoSceneDefinition->musicPath);
    }
    g_PhotoGameClock2 = g_PhotoGameClock = 0.0;
    return ZUN_SUCCESS;
}

void PhotoGameTaskView::ShutdownSubsystems()
{
    utils::DebugPrint("shitdown GameTaskInf\n");
    this->background->Destroy();
    this->front->Destroy();
    this->bullets->Destroy();
    this->player->Destroy();
    this->replay->Destroy();
    this->enemies->Destroy();
    this->photoOverlay->Destroy();
    this->items->Destroy();
    this->pause->Destroy();
    this->lasers->Destroy();
    if (g_PhotoEclFileManager != NULL)
    {
        g_PhotoEclFileManager->Destroy();
    }
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);
    g_PhotoGameTask = NULL;

    if (((g_ControllerRuntimeFlags >> 9) & 1) == 0 &&
        g_ReplayUsesArchive == 0)
    {
        g_PhotoGameSupervisor.StopAudio();
    }
    if (((g_ControllerRuntimeFlags >> 9) & 1) != 0)
    {
        g_PhotoScreenFadeColor = 0;
    }
    else
    {
        g_PhotoScreenFadeColor = 0xff000000;
    }
}

} // namespace th095
