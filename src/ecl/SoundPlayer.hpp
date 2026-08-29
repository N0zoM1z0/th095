#pragma once

#include <dsound.h>
#include <windows.h>

#include "ZunResult.hpp"
#include "diffbuild.hpp"
#include "inttypes.hpp"
#include "zwave.hpp"

#define SOUNDPLAYER_SILENT_VOLUME (DSBVOLUME_MIN)
#define SOUNDPLAYER_MAX_VOLUME (DSBVOLUME_MIN / 2)
#define SOUNDPLAYER_VOLUME_RANGE (SOUNDPLAYER_MAX_VOLUME - SOUNDPLAYER_SILENT_VOLUME)

namespace th095
{
enum SoundIdx
{
    NO_SOUND = -1,
    SOUND_SHOOT = 0,
    SOUND_1,
    SOUND_2,
    SOUND_3,
    SOUND_PICHUN,
    SOUND_5,
    SOUND_6,
    SOUND_7,
    SOUND_8,
    SOUND_9,
    SOUND_SELECT,
    SOUND_BACK,
    SOUND_MOVE_MENU,
    SOUND_D,
    SOUND_E,
    SOUND_F,
    SOUND_10,
    SOUND_11,
    SOUND_TOTAL_BOSS_DEATH,
    SOUND_13,
    SOUND_DAMAGE,
    SOUND_ITEM,
    SOUND_16,
    SOUND_17,
    SOUND_18,
    SOUND_19,
    SOUND_1A,
    SOUND_1B,
    SOUND_1UP,
    SOUND_TIMEOUT,
    SOUND_GRAZE,
    SOUND_POWERUP,
    SOUND_20,
    SOUND_21,
    SOUND_PAUSE,
    SOUND_SPELL_CAPTURE,
    SOUND_FAMILIAR_SPAWN,
    SOUND_DAMAGE_LOW_HEALTH,
    SOUND_TIMEOUT_2,
    SOUND_FAMILIAR_UNHIDE,
    SOUND_FAMILIAR_HIDE,
    SOUND_INVALID_ACTION,
    SOUND_2A,
    SOUND_2B,
    SOUND_2C,
    SOUND_2D,
    SOUND_2E,
};

struct SoundBufferIdxVolume
{
    i32 bufferIdx;
    i16 volume;
    i16 unconsumedMetadata;
};
C_ASSERT(sizeof(SoundBufferIdxVolume) == 0x8);
DIFFABLE_EXTERN_ARRAY(SoundBufferIdxVolume, 47, g_SoundBufferIdxVol)
DIFFABLE_EXTERN_ARRAY(char *, 37, g_SFXList)

struct SoundPlayerCommand
{
    i32 opcode;
    i32 argument;
    i32 step;
    char path[256];
};
C_ASSERT(sizeof(SoundPlayerCommand) == 0x10c);
C_ASSERT(offsetof(SoundPlayerCommand, step) == 0x8);
C_ASSERT(offsetof(SoundPlayerCommand, path) == 0xc);

enum SoundPlayerCommandOpcode
{
    SOUNDPLAYER_COMMAND_NONE = 0,
    SOUNDPLAYER_COMMAND_PRELOAD_BGM = 1,
    SOUNDPLAYER_COMMAND_LOAD_BGM = 2,
    SOUNDPLAYER_COMMAND_STOP_BGM = 3,
    SOUNDPLAYER_COMMAND_RELEASE_BGM = 4,
    SOUNDPLAYER_COMMAND_FADE_OUT = 5,
    SOUNDPLAYER_COMMAND_PAUSE = 6,
    SOUNDPLAYER_COMMAND_UNPAUSE = 7,
    SOUNDPLAYER_COMMAND_SET_VOLUME = 8,
};

#define NUM_SOUND_BUFFERS 128
#define NUM_BGM_SLOTS 16
#define SFX_QUEUE_LENGTH 12
#define BGM_QUEUE_LENGTH 31

class SoundPlayer
{
  public:
    SoundPlayer();

    ZunResult Initialize(HWND window);
    ZunResult RequestThreadStop();
    ZunResult JoinThread();
    ZunResult InitializeDSound(HWND window);
    ZunResult InitSoundBuffers();
    ZunResult Release();

    ZunResult LoadSound(i32 idx, char *path);
    ZunResult LoadSoundData(i32 idx, char *path);
    static WAVEFORMATEX *GetWavFormatData(u8 *soundData, char *formatString, i32 *formatSize,
                                          u32 fileSizeExcludingFormat);

    void QueueCommand(i32 opcode, i32 argument, char *path);
    i32 ProcessQueues();
    void PlaySoundByIdx(SoundIdx idx, i32 pan);
    void PlaySoundPositionedByIdx(SoundIdx idx, f32 pan);
    void StopSoundByIdx(SoundIdx idx);
    ZunResult StartBGM(char *path);
    ZunResult ReopenBGM(char *path);
    ZunResult PreloadBGM(i32 idx, char *path);
    ZunResult LoadBGM(i32 idx);
    void FreePreloadedBGM(i32 idx);
    void StopBGM();
    void FadeOut(f32 seconds)
    {
        if (this->bgm != NULL)
        {
            this->bgm->FadeOut(seconds);
        }
    }
    void FadeIn(f32 seconds)
    {
        if (this->bgm != NULL)
        {
            this->bgm->FadeIn(seconds);
        }
    }
    void PartialFadeOut(f32 seconds)
    {
        if (this->bgm != NULL)
        {
            this->bgm->PartialFadeOut(seconds);
        }
    }
    void PartialFadeIn(f32 seconds)
    {
        if (this->bgm != NULL)
        {
            this->bgm->PartialFadeIn(seconds);
        }
    }
    void Pause()
    {
        this->QueueCommand(SOUNDPLAYER_COMMAND_PAUSE, 0, "Pause");
    }
    void UnPause()
    {
        this->QueueCommand(SOUNDPLAYER_COMMAND_UNPAUSE, 0, "UnPause");
    }

    void UpdateFades();

    void QueueSetVolumeCommand()
    {
        this->QueueCommand(SOUNDPLAYER_COMMAND_SET_VOLUME, 0, "SetVol");
    }

    static DWORD WINAPI BGMPlayerThread(LPVOID lpThreadParameter);

    i32 GetFmtIndexByName(char *name);
    ZunResult LoadFmt(char *path);

    LPDIRECTSOUND dsoundHdl;
    i32 unconsumedDword04;
    LPDIRECTSOUNDBUFFER soundBuffers[NUM_SOUND_BUFFERS];
    LPDIRECTSOUNDBUFFER duplicateSoundBuffers[NUM_SOUND_BUFFERS];
    i32 unconsumedMetadataBySound[NUM_SOUND_BUFFERS];
    LPDIRECTSOUNDBUFFER initSoundBuffer;
    HWND gameWindow;
    CSoundManager *manager;
    DWORD bgmThreadId;
    HANDLE bgmThreadHandle;
    i32 unconsumedDword61C;
    i32 soundQueue[SFX_QUEUE_LENGTH];
    i32 soundQueueRequestCounts[SFX_QUEUE_LENGTH];
    u32 soundQueuePanData[SFX_QUEUE_LENGTH][128];
    ThBgmFormat *bgmPreloadFmtData[NUM_BGM_SLOTS];
    LPBYTE bgmPreloadAllocations[NUM_BGM_SLOTS];
    LPBYTE bgmPreloadData[NUM_BGM_SLOTS];
    DWORD bgmPreloadAllocSizes[NUM_BGM_SLOTS];
    u32 loadedBgmSlot;
    ThBgmFormat *bgmFmtData;
    SoundPlayerCommand commandQueue[BGM_QUEUE_LENGTH + 1];
    char bgmFileNames[NUM_BGM_SLOTS][256];
    char currentBgmFileName[256];
    CStreamingSound *bgm;
    HANDLE bgmUpdateEvent;
    i32 unconsumedDword5210;
    u32 bgmFileBaseOffset;
    HANDLE workerThreadHandle;
    HANDLE secondaryWorkerThreadHandle;
    DWORD workerThreadId;
    i32 workerStopRequest;
    HWND workerWindow;
    i32 unconsumedDword522c;
    void *ownedMusicMetadata[37];
    i32 bgmVolume;
    i32 sfxVolume;
    i32 unconsumedBgmAttenuation;
};
C_ASSERT(sizeof(SoundPlayer) == 0x52d0);
C_ASSERT(offsetof(SoundPlayer, unconsumedDword04) == 0x4);
C_ASSERT(offsetof(SoundPlayer, unconsumedMetadataBySound) == 0x408);
C_ASSERT(offsetof(SoundPlayer, unconsumedDword61C) == 0x61c);
C_ASSERT(offsetof(SoundPlayer, soundQueue) == 0x620);
C_ASSERT(offsetof(SoundPlayer, soundQueueRequestCounts) == 0x650);
C_ASSERT(offsetof(SoundPlayer, soundQueuePanData) == 0x680);
C_ASSERT(offsetof(SoundPlayer, bgmPreloadFmtData) == 0x1e80);
C_ASSERT(offsetof(SoundPlayer, bgmPreloadAllocations) == 0x1ec0);
C_ASSERT(offsetof(SoundPlayer, bgmPreloadData) == 0x1f00);
C_ASSERT(offsetof(SoundPlayer, bgmPreloadAllocSizes) == 0x1f40);
C_ASSERT(offsetof(SoundPlayer, loadedBgmSlot) == 0x1f80);
C_ASSERT(offsetof(SoundPlayer, unconsumedDword5210) == 0x5210);
C_ASSERT(offsetof(SoundPlayer, bgmFileBaseOffset) == 0x5214);
C_ASSERT(offsetof(SoundPlayer, workerThreadHandle) == 0x5218);
C_ASSERT(offsetof(SoundPlayer, secondaryWorkerThreadHandle) == 0x521c);
C_ASSERT(offsetof(SoundPlayer, workerThreadId) == 0x5220);
C_ASSERT(offsetof(SoundPlayer, workerStopRequest) == 0x5224);
C_ASSERT(offsetof(SoundPlayer, workerWindow) == 0x5228);
C_ASSERT(offsetof(SoundPlayer, ownedMusicMetadata) == 0x5230);
C_ASSERT(offsetof(SoundPlayer, bgmVolume) == 0x52c4);
C_ASSERT(offsetof(SoundPlayer, unconsumedBgmAttenuation) == 0x52cc);

DIFFABLE_EXTERN(SoundPlayer, g_SoundPlayer)

void __fastcall SoundPlayerWorkerThread(SoundPlayer *soundPlayer);
HANDLE StartSoundLoadThread();
void __fastcall SoundDataLoaderThread(SoundPlayer *soundPlayer);
}; // namespace th095
