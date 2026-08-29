#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "Global.hpp"
#include "SoundPlayer.hpp"
#include "Supervisor.hpp"
#include "dxutil.hpp"
#include "utils.hpp"

namespace th095
{

#define BGM_WAV_BITS_PER_SAMPLE 16
#define BGM_BUFFER_SIZE 0x8000
#define BGM_WAV_NUM_CHANNELS 2
#define BGM_WAV_BLOCK_ALIGN (BGM_WAV_BITS_PER_SAMPLE / 8 * BGM_WAV_NUM_CHANNELS)
#define BGM_WAV_SAMPLES_PER_SEC 44100

#define TH_ERR_SOUNDPLAYER_FAILED_TO_INITIALIZE_OBJECT                                                                  \
    "\x44\x69\x72\x65\x63\x74\x53\x6F\x75\x6E\x64\x20\x83\x49\x83\x75\x83\x57\x83\x46\x83\x4E\x83\x67" \
    "\x82\xCC\x8F\x89\x8A\xFA\x89\xBB\x82\xAA\x8E\xB8\x94\x73\x82\xB5\x82\xBD\x82\xE6\r\n"
#define TH_ERR_SOUNDPLAYER_FAILED_TO_LOAD_SOUND_FILE                                                                   \
    "\x65\x72\x72\x6F\x72\x20\x3A\x20\x53\x6F\x75\x6E\x64\x20\x83\x74\x83\x40\x83\x43\x83\x8B\x82\xAA" \
    "\x93\xC7\x82\xDD\x8D\x9E\x82\xDF\x82\xC8\x82\xA2\x20\x83\x66\x81\x5B\x83\x5E\x82\xF0\x8A\x6D\x94" \
    "\x46\x20\x25\x73\r\n"
#define TH_DBG_SOUNDPLAYER_INIT_SUCCESS                                                                                \
    "\x44\x69\x72\x65\x63\x74\x53\x6F\x75\x6E\x64\x20\x82\xCD\x90\xB3\x8F\xED\x82\xC9\x8F\x89\x8A\xFA" \
    "\x89\xBB\x82\xB3\x82\xEA\x82\xDC\x82\xB5\x82\xBD\r\n"
#define TH_ERR_NOT_A_WAV_FILE                                                                                          \
    "\x57\x61\x76\x20\x83\x74\x83\x40\x83\x43\x83\x8B\x82\xB6\x82\xE1\x82\xC8\x82\xA2\x3F\x20\x25\x73" \
    "\r\n"
#define TH_ERR_NOT_A_WAV_FILE2                                                                                         \
    "\x57\x61\x76\x20\x83\x74\x83\x40\x83\x43\x83\x8B\x82\xB6\x82\xE1\x82\xC8\x82\xA2\x20\x25\x73\r\n"

void __fastcall SoundPlayerWorkerThread(SoundPlayer *soundPlayer)
{
    g_SoundPlayer.InitializeDSound(g_SoundPlayer.workerWindow);
    while (g_SoundPlayer.workerStopRequest == 0)
    {
        Sleep(1);
    }
    utils::DebugPrint("SoundInit Thread Finish\n");
    g_SoundPlayer.unconsumedDword522c = 1;
}

#define bufDesc restartCommandProcessingLocal05
#define audioBuffer2Start averagedPanLocal12
#define audioBuffer2Len iLocal11
#define audioBuffer1Len commandCursorLocal02
#define audioBuffer1Start soundIndexLocal01
#define wavFormat jLocal00
#pragma var_order(bufDesc, audioBuffer2Start, audioBuffer2Len, audioBuffer1Len, audioBuffer1Start, wavFormat)
ZunResult SoundPlayer::InitializeDSound(HWND gameWindow)
{
    DSBUFFERDESC bufDesc;
    LPVOID audioBuffer2Start;
    DWORD audioBuffer2Len;
    DWORD audioBuffer1Len;
    LPVOID audioBuffer1Start;
    WAVEFORMATEX wavFormat;

    for (i32 i = 0; i < NUM_SOUND_BUFFERS; i++)
    {
        this->unconsumedMetadataBySound[i] = -1;
    }
    for (i32 i = 0; i < SFX_QUEUE_LENGTH; i++)
    {
        this->soundQueue[i] = -1;
    }

    this->manager = new CSoundManager();
    if (FAILED(this->manager->Initialize(gameWindow, 2, 2, BGM_WAV_SAMPLES_PER_SEC, BGM_WAV_BITS_PER_SAMPLE)))
    {
        g_GameErrorContext.Log(TH_ERR_SOUNDPLAYER_FAILED_TO_INITIALIZE_OBJECT);
        SAFE_DELETE(this->manager);
        return ZUN_ERROR;
    }

    this->dsoundHdl = this->manager->GetDirectSound();
    this->bgmThreadHandle = NULL;
    ZeroMemory(&bufDesc, sizeof(DSBUFFERDESC));
    bufDesc.dwSize = sizeof(DSBUFFERDESC);
    bufDesc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCSOFTWARE;
    bufDesc.dwBufferBytes = BGM_BUFFER_SIZE;
    ZeroMemory(&wavFormat, sizeof(WAVEFORMATEX));
    wavFormat.cbSize = 0;
    wavFormat.wFormatTag = WAVE_FORMAT_PCM;
    wavFormat.nChannels = BGM_WAV_NUM_CHANNELS;
    wavFormat.nSamplesPerSec = BGM_WAV_SAMPLES_PER_SEC;
    wavFormat.nAvgBytesPerSec = BGM_WAV_SAMPLES_PER_SEC * sizeof(INT16) * BGM_WAV_NUM_CHANNELS;
    wavFormat.nBlockAlign = BGM_WAV_BLOCK_ALIGN;
    wavFormat.wBitsPerSample = BGM_WAV_BITS_PER_SAMPLE;
    bufDesc.lpwfxFormat = &wavFormat;
    if (FAILED(this->dsoundHdl->CreateSoundBuffer(&bufDesc, &this->initSoundBuffer, NULL)))
    {
        return ZUN_ERROR;
    }
    if (FAILED(this->initSoundBuffer->Lock(0, BGM_BUFFER_SIZE, &audioBuffer1Start, &audioBuffer1Len, &audioBuffer2Start,
                                           &audioBuffer2Len, 0)))
    {
        return ZUN_ERROR;
    }

    ZeroMemory(audioBuffer1Start, BGM_BUFFER_SIZE);
    this->initSoundBuffer->Unlock(audioBuffer1Start, audioBuffer1Len, audioBuffer2Start, audioBuffer2Len);
    this->initSoundBuffer->Play(0, 0, 1);
    this->bgmVolume = 100;
    this->sfxVolume = 100;
    SetTimer(gameWindow, 0, 250, NULL);
    this->gameWindow = gameWindow;

    for (i32 i = 0; i < ARRAY_SIZE_SIGNED(g_SFXList); i++)
    {
        if (g_SoundPlayer.workerStopRequest == 2)
            return ZUN_ERROR;
        if (g_SoundPlayer.LoadSound(i, g_SFXList[i]) != ZUN_SUCCESS)
        {
            g_GameErrorContext.Log(TH_ERR_SOUNDPLAYER_FAILED_TO_LOAD_SOUND_FILE, g_SFXList[i]);
            return ZUN_ERROR;
        }
    }
    g_GameErrorContext.Log(TH_DBG_SOUNDPLAYER_INIT_SUCCESS);
    return ZUN_SUCCESS;
}
#undef bufDesc
#undef audioBuffer2Start
#undef audioBuffer2Len
#undef audioBuffer1Len
#undef audioBuffer1Start
#undef wavFormat

HANDLE StartSoundLoadThread()
{
    DWORD threadId;

    g_SoundPlayer.secondaryWorkerThreadHandle =
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SoundDataLoaderThread, &g_SoundPlayer, 0, &threadId);
    return g_SoundPlayer.secondaryWorkerThreadHandle;
}

void __fastcall SoundDataLoaderThread(SoundPlayer *soundPlayer)
{
    i32 i;

    for (i = 0; i < ARRAY_SIZE_SIGNED(g_SFXList); i++)
    {
        if (g_SoundPlayer.workerStopRequest == 2)
            return;
        if (g_SoundPlayer.LoadSoundData(i, g_SFXList[i]) != ZUN_SUCCESS)
        {
            g_GameErrorContext.Log(TH_ERR_SOUNDPLAYER_FAILED_TO_LOAD_SOUND_FILE, g_SFXList[i]);
            return;
        }
    }
    while (g_SoundPlayer.workerStopRequest == 0)
    {
        Sleep(1);
    }
}

ZunResult SoundPlayer::Initialize(HWND window)
{
    memset(this, 0, sizeof(SoundPlayer));
    this->workerWindow = window;
    this->workerThreadHandle =
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SoundPlayerWorkerThread, this, 0, &this->workerThreadId);
    return ZUN_SUCCESS;
}

ZunResult SoundPlayer::RequestThreadStop()
{
    this->workerStopRequest = 2;
    return ZUN_SUCCESS;
}

ZunResult SoundPlayer::JoinThread()
{
    if (this->workerThreadHandle != NULL)
    {
        if (this->workerStopRequest == 0)
        {
            this->workerStopRequest = 1;
        }
        while (WaitForSingleObject(this->workerThreadHandle, 100) == WAIT_TIMEOUT)
        {
            Sleep(1);
        }
        while (WaitForSingleObject(this->secondaryWorkerThreadHandle, 100) == WAIT_TIMEOUT)
        {
            Sleep(1);
        }
        CloseHandle(this->workerThreadHandle);
        CloseHandle(this->secondaryWorkerThreadHandle);
        this->workerThreadHandle = NULL;
        this->secondaryWorkerThreadHandle = NULL;
    }
    return ZUN_SUCCESS;
}

#define sFDCursor restartCommandProcessingLocal05
#define dsBuffer averagedPanLocal12
#define wavDataPtr iLocal11
#define formatSize commandCursorLocal02
#define audioPtr2 soundIndexLocal01
#define audioSize2 jLocal00
#define audioSize1 preloadBufferLocal03
#define audioPtr1 bgmPathLocal18
#define soundFileData bgmFormatIndexLocal05
#define wavData reopenedBufferLocal01
#define fileSize volumeScaleLocal00
#pragma var_order(sFDCursor, dsBuffer, wavDataPtr, formatSize, audioPtr2, audioSize2, audioSize1, audioPtr1,           \
                  soundFileData, wavData, fileSize)
ZunResult SoundPlayer::LoadSound(i32 idx, char *path)
{
    u8 *soundFileData;
    u8 *sFDCursor;
    i32 fileSize;
    WAVEFORMATEX *wavDataPtr;
    WAVEFORMATEX *audioPtr1;
    WAVEFORMATEX *audioPtr2;
    DWORD audioSize1;
    DWORD audioSize2;
    WAVEFORMATEX wavData;
    i32 formatSize;
    DSBUFFERDESC dsBuffer;

    if (this->manager == NULL)
        return ZUN_SUCCESS;

    SAFE_RELEASE(this->soundBuffers[idx]);
    while (this->ownedMusicMetadata[idx] == NULL)
    {
        Sleep(10);
        if (g_SoundPlayer.workerStopRequest == 2)
            return ZUN_SUCCESS;
    }
    soundFileData = (u8 *)this->ownedMusicMetadata[idx];
    sFDCursor = soundFileData;
    this->ownedMusicMetadata[idx] = NULL;
    if (sFDCursor == NULL)
        return ZUN_ERROR;

    if (strncmp((char *)sFDCursor, "RIFF", 4))
    {
        g_GameErrorContext.Log(TH_ERR_NOT_A_WAV_FILE2, path);
        g_ZunMemory.Free(soundFileData);
        return ZUN_ERROR;
    }
    sFDCursor += 4;
    fileSize = *(i32 *)sFDCursor;
    sFDCursor += 4;
    if (strncmp((char *)sFDCursor, "WAVE", 4))
    {
        g_GameErrorContext.Log(TH_ERR_NOT_A_WAV_FILE, path);
        g_ZunMemory.Free(soundFileData);
        return ZUN_ERROR;
    }
    sFDCursor += 4;
    wavDataPtr = GetWavFormatData(sFDCursor, "fmt ", &formatSize, fileSize - 12);
    if (wavDataPtr == NULL)
    {
        g_GameErrorContext.Log(TH_ERR_NOT_A_WAV_FILE, path);
        g_ZunMemory.Free(soundFileData);
        return ZUN_ERROR;
    }
    wavData = *wavDataPtr;

    wavDataPtr = GetWavFormatData(sFDCursor, "data", &formatSize, fileSize - 12);
    if (wavDataPtr == NULL)
    {
        g_GameErrorContext.Log(TH_ERR_NOT_A_WAV_FILE, path);
        g_ZunMemory.Free(soundFileData);
        return ZUN_ERROR;
    }
    ZeroMemory(&dsBuffer, sizeof(dsBuffer));
    dsBuffer.dwSize = sizeof(dsBuffer);
    dsBuffer.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_LOCSOFTWARE;
    dsBuffer.dwBufferBytes = formatSize;
    dsBuffer.lpwfxFormat = &wavData;
    if (FAILED(this->dsoundHdl->CreateSoundBuffer(&dsBuffer, &this->soundBuffers[idx], NULL)))
    {
        g_ZunMemory.Free(soundFileData);
        return ZUN_ERROR;
    }
    if (FAILED(soundBuffers[idx]->Lock(0, formatSize, (LPVOID *)&audioPtr1, (LPDWORD)&audioSize1, (LPVOID *)&audioPtr2,
                                       (LPDWORD)&audioSize2, NULL)))
    {
        g_ZunMemory.Free(soundFileData);
        return ZUN_ERROR;
    }
    CopyMemory(audioPtr1, wavDataPtr, audioSize1);
    if (audioSize2 != 0)
    {
        CopyMemory(audioPtr2, (i8 *)wavDataPtr + audioSize1, audioSize2);
    }
    soundBuffers[idx]->Unlock((LPVOID *)audioPtr1, audioSize1, (LPVOID *)audioPtr2, audioSize2);
    g_ZunMemory.Free(soundFileData);
    utils::DebugPrint("Create Sound Buffer %s\n", path);
    return ZUN_SUCCESS;
}
#undef sFDCursor
#undef dsBuffer
#undef wavDataPtr
#undef formatSize
#undef audioPtr2
#undef audioSize2
#undef audioSize1
#undef audioPtr1
#undef soundFileData
#undef wavData
#undef fileSize

#define pos iLocal06
#define i posLocal03
#define buffer bufferLocal04
#pragma var_order(pos, i, buffer)
i32 SoundPlayer::GetFmtIndexByName(char *name)
{
    i32 i = 0;
    char *pos;
    char buffer[128];

    pos = strrchr(name, '/');
    if (pos == NULL)
    {
        pos = strrchr(name, '\\');
    }
    if (pos == NULL)
    {
        strcpy(buffer, name);
    }
    else
    {
        strcpy(buffer, pos + 1);
    }
    while (this->bgmFmtData[i].name[0] != '\0')
    {
        if (strcmp(this->bgmFmtData[i].name, buffer) == 0)
        {
            break;
        }
        i++;
    }
    if (this->bgmFmtData[i].name[0] == '\0')
    {
        i = 0;
    }
    return i;
}
#undef pos
#undef i
#undef buffer

void SoundPlayer::PlaySoundByIdx(SoundIdx idx, i32 pan)
{
    i32 unconsumedMetadata;
    i32 i;

    unconsumedMetadata = g_SoundBufferIdxVol[idx].unconsumedMetadata;
    for (i = 0; i < SFX_QUEUE_LENGTH; i++)
    {
        if (this->soundQueue[i] < 0)
            break;

        if (this->soundQueue[i] == idx)
        {
            if (this->soundQueueRequestCounts[i] < 0x80)
                this->soundQueuePanData[i][this->soundQueueRequestCounts[i]++] = pan;

            return;
        }
    }

    if (i >= SFX_QUEUE_LENGTH)
        return;

    this->soundQueue[i] = idx;
    this->unconsumedMetadataBySound[idx] = unconsumedMetadata;
    this->soundQueuePanData[i][0] = pan;
    this->soundQueueRequestCounts[i]++;
}

void SoundPlayer::PlaySoundPositionedByIdx(SoundIdx idx, f32 pan)
{
    i32 unconsumedMetadata;
    i32 panAsInt;
    i32 i;

    unconsumedMetadata = g_SoundBufferIdxVol[idx].unconsumedMetadata;
    panAsInt = (pan * 1000) / 192;

    for (i = 0; i < SFX_QUEUE_LENGTH; i++)
    {
        if (this->soundQueue[i] < 0)
            break;

        if (this->soundQueue[i] == idx)
        {
            if (this->soundQueueRequestCounts[i] < 0x80)
                this->soundQueuePanData[i][this->soundQueueRequestCounts[i]++] = panAsInt;

            return;
        }
    }

    if (i >= SFX_QUEUE_LENGTH)
        return;

    this->soundQueue[i] = idx;
    this->unconsumedMetadataBySound[idx] = unconsumedMetadata;
    this->soundQueuePanData[i][0] = panAsInt;
    this->soundQueueRequestCounts[i]++;
}

void SoundPlayer::StopSoundByIdx(SoundIdx idx)
{
    i32 unconsumedMetadata;
    i32 i;

    unconsumedMetadata = g_SoundBufferIdxVol[idx].unconsumedMetadata;
    for (i = 0; i < SFX_QUEUE_LENGTH; i++)
    {
        if (this->soundQueue[i] < 0)
            break;

        if (this->soundQueue[i] == idx)
        {
            this->soundQueueRequestCounts[i] = -1;
            return;
        }
    }

    if (i >= SFX_QUEUE_LENGTH)
        return;

    this->soundQueue[i] = idx;
    this->soundQueueRequestCounts[i] = -1;
}

WAVEFORMATEX *SoundPlayer::GetWavFormatData(u8 *soundData, char *formatString, i32 *formatSize,
                                            u32 fileSizeExcludingFormat)
{
    while (fileSizeExcludingFormat > 0)
    {
        *formatSize = *(i32 *)(soundData + 4);
        if (strncmp((char *)soundData, formatString, 4) == 0)
        {
            return (WAVEFORMATEX *)(soundData + 8);
        }
        fileSizeExcludingFormat -= *formatSize + 8;
        soundData += *formatSize + 8;
    }
    return NULL;
}

ZunResult SoundPlayer::LoadSoundData(i32 idx, char *path)
{
    this->ownedMusicMetadata[idx] = FileSystem::OpenFile(path, NULL, FALSE);
    if (this->ownedMusicMetadata[idx] == NULL)
        return ZUN_ERROR;
    return ZUN_SUCCESS;
}

ZunResult SoundPlayer::LoadFmt(char *path)
{
    this->bgmFmtData = (ThBgmFormat *)FileSystem::OpenFile(path, NULL, FALSE);
    return this->bgmFmtData != NULL ? ZUN_SUCCESS : ZUN_ERROR;
}

#define notifySize restartCommandProcessingLocal30
#define fmtData restartCommandProcessingLocal18
#define res restartCommandProcessingLocal05
#define numSamplesPerSec restartCommandProcessingLocal09
#define blockAlign restartCommandProcessingLocal26
#pragma var_order(notifySize, fmtData, res, numSamplesPerSec, blockAlign)
ZunResult SoundPlayer::StartBGM(char *path)
{
    DWORD numSamplesPerSec;
    ThBgmFormat *fmtData;
    DWORD blockAlign;
    HRESULT res;
    DWORD notifySize;

    strcpy(this->currentBgmFileName, path);

    if (this->manager == NULL)
        return ZUN_ERROR;

    if (this->dsoundHdl == NULL)
        return ZUN_ERROR;

    utils::DebugPrint("Streming BGM Start\r\n");
    this->StopBGM();

    fmtData = this->bgmFmtData;
    blockAlign = fmtData->format.nBlockAlign;
    numSamplesPerSec = fmtData->format.nSamplesPerSec;
    notifySize = numSamplesPerSec * 4 * blockAlign / BGM_WAV_BITS_PER_SAMPLE;
    notifySize -= notifySize % blockAlign;
    this->bgmUpdateEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
    this->bgmThreadHandle =
        CreateThread(NULL, 0, SoundPlayer::BGMPlayerThread, g_Supervisor.hwndGameWindow, 0, &this->bgmThreadId);
    res = this->manager->CreateStreaming(&this->bgm, path,
                                         DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY, GUID_NULL, 16,
                                         notifySize, this->bgmUpdateEvent, fmtData);
    if (FAILED(res))
    {
        utils::DebugPrint("error : create streaming sound buffer\r\n");
        return ZUN_ERROR;
    }
    return ZUN_SUCCESS;
}
#undef notifySize
#undef fmtData
#undef res
#undef numSamplesPerSec
#undef blockAlign

ZunResult SoundPlayer::ReopenBGM(char *path)
{
    if (this->bgm == NULL)
        return ZUN_ERROR;

    i32 idx = GetFmtIndexByName(path);

    this->bgm->GetWaveFile()->Reopen(&this->bgmFmtData[idx]);
    utils::DebugPrint("Streming BGM Reopen %d\r\n", idx);
    return ZUN_SUCCESS;
}

#define fmtIdx restartCommandProcessingLocal18
#define numBytesRead restartCommandProcessingLocal30
#define handle restartCommandProcessingLocal09
#define bufferPtr restartCommandProcessingLocal05
#pragma var_order(fmtIdx, numBytesRead, handle, bufferPtr)
ZunResult SoundPlayer::PreloadBGM(i32 idx, char *path)
{
    LPBYTE bufferPtr;
    DWORD numBytesRead;
    HANDLE handle;
    i32 fmtIdx;

    if (this->bgmPreloadAllocations[idx] != NULL)
    {
        if (strcmp(path, this->bgmFileNames[idx]) == 0)
            return ZUN_SUCCESS;
    }
    strcpy(g_SoundPlayer.bgmFileNames[idx], path);

    if (!g_Supervisor.IsMusicPreloadEnabled())
        return ZUN_SUCCESS;

    if (this->manager == NULL)
        return ZUN_SUCCESS;

    if (this->bgmPreloadAllocations[idx] != NULL)
    {
        g_ZunMemory.Free(this->bgmPreloadAllocations[idx]);
        this->bgmPreloadAllocations[idx] = NULL;
    }
    utils::DebugPrint("Streming BGM PreLoad %d\r\n", idx);

    handle = CreateFileA(this->currentBgmFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        utils::DebugPrint("error : bgmfile is not find %s\r\n", this->currentBgmFileName);
        return ZUN_ERROR;
    }

    fmtIdx = this->GetFmtIndexByName(path);
    SetFilePointer(handle, this->bgmFmtData[fmtIdx].startOffset, 0, FILE_BEGIN);

    bufferPtr = (u8 *)g_ZunMemory.Alloc(this->bgmFmtData[fmtIdx].preloadAllocSize);
    if (bufferPtr == NULL)
    {
        CloseHandle(handle);
        utils::DebugPrint("error : bgmfile is not find %s\r\n", this->currentBgmFileName);
        return ZUN_ERROR;
    }

    ReadFile(handle, bufferPtr, this->bgmFmtData[fmtIdx].preloadAllocSize, &numBytesRead, NULL);
    CloseHandle(handle);
    this->bgmPreloadFmtData[idx] = &this->bgmFmtData[fmtIdx];
    this->bgmPreloadAllocations[idx] = bufferPtr;
    this->bgmPreloadData[idx] = bufferPtr;
    this->bgmPreloadAllocSizes[idx] = this->bgmPreloadFmtData[idx]->preloadAllocSize;

    return ZUN_SUCCESS;
}
#undef fmtIdx
#undef numBytesRead
#undef handle
#undef bufferPtr

#define notifySize restartCommandProcessingLocal30
#define hr restartCommandProcessingLocal18
#define numSamplesPerSec restartCommandProcessingLocal05
#define blockAlign restartCommandProcessingLocal09
#pragma var_order(notifySize, hr, numSamplesPerSec, blockAlign)
ZunResult SoundPlayer::LoadBGM(i32 idx)
{
    HRESULT hr;
    DWORD blockAlign;
    DWORD numSamplesPerSec;
    DWORD notifySize;

    if (this->manager == NULL)
        return ZUN_ERROR;

    if (g_Supervisor.cfg.musicMode == OFF)
        return ZUN_ERROR;

    if (this->dsoundHdl == NULL)
        return ZUN_ERROR;

    if (!g_Supervisor.IsMusicPreloadEnabled())
        return this->ReopenBGM(this->bgmFileNames[idx]);

    if (this->bgmPreloadAllocations[idx] == NULL)
        return ZUN_ERROR;

    utils::DebugPrint("Streming BGM Load no %d\r\n", idx);

    blockAlign = this->bgmPreloadFmtData[idx]->format.nBlockAlign;
    numSamplesPerSec = this->bgmPreloadFmtData[idx]->format.nSamplesPerSec;
    notifySize = numSamplesPerSec * 4 * blockAlign / BGM_WAV_BITS_PER_SAMPLE;
    notifySize -= notifySize % blockAlign;
    this->bgmUpdateEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
    this->bgmThreadHandle =
        CreateThread(NULL, 0, SoundPlayer::BGMPlayerThread, g_Supervisor.hwndGameWindow, 0, &this->bgmThreadId);
    hr = this->manager->CreateStreamingFromMemory(
        &this->bgm, this->bgmPreloadData[idx], this->bgmPreloadAllocSizes[idx], this->bgmPreloadFmtData[idx],
        DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY, GUID_NULL, 16, notifySize, this->bgmUpdateEvent);
    if (FAILED(hr))
    {
        utils::DebugPrint("error : create streaming sound buffer\r\n");
        return ZUN_ERROR;
    }

    utils::DebugPrint("load comp\r\n");
    this->loadedBgmSlot = idx;
    return ZUN_SUCCESS;
}
#undef notifySize
#undef hr
#undef numSamplesPerSec
#undef blockAlign

void SoundPlayer::StopBGM()
{
    if (this->bgm != NULL)
    {
        utils::DebugPrint("Streming BGM stop\r\n");
        this->bgm->Stop();
        if (this->bgmThreadHandle != NULL)
        {
            PostThreadMessageA(this->bgmThreadId, WM_QUIT, 0, 0);
            utils::DebugPrint("stop m_dwNotifyThreadID\r\n");

            while (WaitForSingleObject(this->bgmThreadHandle, 256))
                PostThreadMessageA(this->bgmThreadId, WM_QUIT, 0, 0);

            utils::DebugPrint("stop comp\r\n");
            CloseHandle(this->bgmThreadHandle);
            CloseHandle(this->bgmUpdateEvent);
            this->bgmThreadHandle = NULL;
        }
        SAFE_DELETE(this->bgm);
    }
}

ZunResult SoundPlayer::InitSoundBuffers()
{
    i32 i;

    for (i = 0; i < SFX_QUEUE_LENGTH; i++)
    {
        this->soundQueue[i] = -1;
    }
    g_SoundPlayer.JoinThread();
    if (this->manager == NULL)
        return ZUN_ERROR;

    if (this->dsoundHdl == NULL)
        return ZUN_SUCCESS;

    for (i = 0; i < ARRAY_SIZE(g_SoundBufferIdxVol); i++)
    {
        this->dsoundHdl->DuplicateSoundBuffer(this->soundBuffers[g_SoundBufferIdxVol[i].bufferIdx],
                                              &this->duplicateSoundBuffers[i]);
        this->duplicateSoundBuffers[i]->SetCurrentPosition(0);
        this->duplicateSoundBuffers[i]->SetVolume(g_SoundBufferIdxVol[i].volume);
    }

    return ZUN_SUCCESS;
}

// Stock VC7.1 allocates locals through identifier hash chains. These scoped
// backing names reproduce the target order documented by TH08's var_order
// annotation without replacing the pinned compiler frontend.
#define restartCommandProcessing restartCommandProcessingLocal05
#define averagedPan averagedPanLocal12
#define i iLocal11
#define commandCursor commandCursorLocal02
#define soundIndex soundIndexLocal01
#define j jLocal00
#define preloadBuffer preloadBufferLocal03
#define bgmPath bgmPathLocal18
#define bgmFormatIndex bgmFormatIndexLocal05
#define reopenedBuffer reopenedBufferLocal01
#define volumeScale volumeScaleLocal00
#pragma var_order(restartCommandProcessing, averagedPan, i, commandCursor, soundIndex, j, preloadBuffer, bgmPath,     \
                  bgmFormatIndex, reopenedBuffer, volumeScale)
i32 SoundPlayer::ProcessQueues()
{
    BOOL restartCommandProcessing;
    i32 averagedPan;
    i32 i;
    SoundPlayerCommand *commandCursor;
    i32 soundIndex;
    i32 j;
    LPDIRECTSOUNDBUFFER preloadBuffer;
    char *bgmPath;
    i32 bgmFormatIndex;
    LPDIRECTSOUNDBUFFER reopenedBuffer;
    f32 volumeScale;

    if (this->manager == NULL)
        return 0;

    commandCursor = this->commandQueue;

loop:
    restartCommandProcessing = FALSE;

    switch (commandCursor->opcode)
    {
    case SOUNDPLAYER_COMMAND_SET_VOLUME:
        if (this->bgm != NULL)
            this->bgm->SetVolume(this->bgmVolume);
        goto next_command;
    case SOUNDPLAYER_COMMAND_PRELOAD_BGM:
        if (g_Supervisor.IsMusicPreloadEnabled())
        {
            utils::DebugPrint("Sound : PreLoad Stage\r\n");
            if (commandCursor->step == 0)
            {
                this->StopBGM();
                this->PreloadBGM(commandCursor->argument, commandCursor->path);
                restartCommandProcessing = TRUE;
                goto next_command;
            }
        }
        else
        {
            utils::DebugPrint("Sound : PreLoad Stage\r\n");
            this->PreloadBGM(commandCursor->argument, commandCursor->path);
            restartCommandProcessing = TRUE;
            goto next_command;
        }
        commandCursor->step++;
        break;
    case SOUNDPLAYER_COMMAND_LOAD_BGM:
        if (g_Supervisor.IsMusicPreloadEnabled() && commandCursor->argument >= 0)
        {
            if (commandCursor->step == 0)
            {
                utils::DebugPrint("Sound : Load Stage\r\n");
                if (this->LoadBGM(commandCursor->argument) != ZUN_SUCCESS)
                    goto next_command;
            }
            else if (commandCursor->step == 2)
            {
                utils::DebugPrint("Sound : Reset Stage\r\n");
                if (this->bgm != NULL)
                {
                    if (FAILED(this->bgm->Reset()))
                        goto next_command;
                }
            }
            else if (commandCursor->step == 5)
            {
                utils::DebugPrint("Sound : Fill Buffer Stage\r\n");
                preloadBuffer = this->bgm->GetBuffer(0);
                commandCursor->argument = this->bgm->GetWaveFile()->GetFormat()->totalLength != 0;
                if (FAILED(this->bgm->FillBufferWithSound(preloadBuffer, commandCursor->argument)))
                    goto next_command;
            }
            else if (commandCursor->step == 7)
            {
                utils::DebugPrint("Sound : Play Stage\r\n");
                this->bgm->Play(0, DSBPLAY_LOOPING);
            }
            else if (commandCursor->step >= 20)
            {
                goto next_command;
            }
        }
        else if (this->bgm == NULL)
        {
            goto next_command;
        }
        else if (commandCursor->step == 0)
        {
            utils::DebugPrint("Sound : Stop Stage\r\n");
            this->bgm->Stop();
        }
        else if (commandCursor->step == 1)
        {
            if (this->bgm->m_bIsLocked)
                break;
            utils::DebugPrint("Sound : Recreate Stage\r\n");
            this->bgm->InitSoundBuffers();
        }
        else if (commandCursor->step == 2)
        {
            utils::DebugPrint("Sound : ReOpen Stage\r\n");
            bgmPath = commandCursor->argument >= 0 ? this->bgmFileNames[commandCursor->argument] : commandCursor->path;
            bgmFormatIndex = this->GetFmtIndexByName(bgmPath);
            this->bgm->GetWaveFile()->Reopen(&this->bgmFmtData[bgmFormatIndex]);
        }
        else if (commandCursor->step == 3)
        {
            utils::DebugPrint("Sound : Fill Buffer Stage\r\n");
            reopenedBuffer = this->bgm->GetBuffer(0);
            this->bgm->Reset();
            commandCursor->argument = this->bgm->GetWaveFile()->GetFormat()->totalLength != 0;
            if (FAILED(this->bgm->FillBufferWithSound(reopenedBuffer, commandCursor->argument)))
                goto next_command;
        }
        else if (commandCursor->step == 4)
        {
            utils::DebugPrint("Sound : Play Stage\r\n");
            this->bgm->Play(0, DSBPLAY_LOOPING);
        }
        else if (commandCursor->step >= 7)
        {
            goto next_command;
        }
        commandCursor->step++;
        break;
    case SOUNDPLAYER_COMMAND_RELEASE_BGM:
        if (this->bgm == NULL)
            goto next_command;
        if (commandCursor->step == 0)
        {
            utils::DebugPrint("Sound : Stop Stage\r\n");
            this->bgm->Stop();
        }
        else if (commandCursor->step == 1)
        {
            utils::DebugPrint("Sound : Thread Stop Stage\r\n");
            if (this->bgmThreadHandle == NULL)
                goto next_command;
            PostThreadMessageA(this->bgmThreadId, WM_QUIT, 0, 0);
        }
        else if (commandCursor->step == 2)
        {
            if (WaitForSingleObject(this->bgmThreadHandle, 256))
            {
                utils::DebugPrint("Sound : Thread Stop Wait Stage\r\n");
                PostThreadMessageA(this->bgmThreadId, WM_QUIT, 0, 0);
                commandCursor->step--;
            }
            else
            {
                this->bgmThreadHandle = NULL;
            }
        }
        else if (commandCursor->step == 3)
        {
            utils::DebugPrint("Sound : Handle Close Stage\r\n");
            CloseHandle(this->bgmThreadHandle);
            CloseHandle(this->bgmUpdateEvent);
            this->bgmThreadHandle = NULL;
            if (this->bgm != NULL)
            {
                delete this->bgm;
                this->bgm = NULL;
            }
            this->bgm = NULL;
        }
        else if (commandCursor->step == 10)
        {
            goto next_command;
        }
        commandCursor->step++;
        break;
    case SOUNDPLAYER_COMMAND_STOP_BGM:
        if (this->bgm == NULL)
            goto next_command;
        if (commandCursor->step == 0)
        {
            utils::DebugPrint("Sound : Stop Stage\r\n");
            this->bgm->Stop();
        }
        else if (commandCursor->step == 1)
        {
            goto next_command;
        }
        commandCursor->step++;
        break;
    case SOUNDPLAYER_COMMAND_FADE_OUT:
        utils::DebugPrint("Sound : Fade Out Stage %d\r\n", commandCursor->argument);
        g_SoundPlayer.FadeOut((f32)commandCursor->argument);
        goto next_command;
    case SOUNDPLAYER_COMMAND_PAUSE:
        if (g_Supervisor.cfg.musicMode == WAV)
        {
            if (this->bgm->m_bIsLocked)
            {
                utils::DebugPrint("locked\n");
                break;
            }
            if (this->bgm != NULL)
                this->bgm->Pause();
        }
        goto next_command;
    case SOUNDPLAYER_COMMAND_UNPAUSE:
        if (g_Supervisor.cfg.musicMode == WAV)
        {
            if (this->bgm->m_bIsLocked)
                break;
            if (this->bgm != NULL)
                this->bgm->Unpause();
        }
        goto next_command;

    default:
        break;

    next_command:
        for (i = 0; i < BGM_QUEUE_LENGTH; i++, commandCursor++)
        {
            if (commandCursor->opcode == SOUNDPLAYER_COMMAND_NONE)
                break;
            memcpy(commandCursor, commandCursor + 1, sizeof(*commandCursor));
        }

        if (restartCommandProcessing)
            goto loop;
    }

    if (!g_Supervisor.cfg.playSounds)
        return this->commandQueue[0].opcode;
    for (i = 0; i < SFX_QUEUE_LENGTH; i++)
    {
        if (this->soundQueue[i] < 0)
            break;
        soundIndex = this->soundQueue[i];
        this->soundQueue[i] = -1;
        if (this->soundQueueRequestCounts[i] < 0)
        {
            if (this->duplicateSoundBuffers[soundIndex] != NULL)
                this->duplicateSoundBuffers[soundIndex]->Stop();
            this->soundQueueRequestCounts[i] = 0;
            continue;
        }

        averagedPan = 0;
        for (j = 0; j < this->soundQueueRequestCounts[i]; j++)
            averagedPan += this->soundQueuePanData[i][j];
        averagedPan /= this->soundQueueRequestCounts[i];
        this->soundQueueRequestCounts[i] = 0;
        if (this->duplicateSoundBuffers[soundIndex] == NULL)
        {
            continue;
        }
        this->duplicateSoundBuffers[soundIndex]->Stop();
        this->duplicateSoundBuffers[soundIndex]->SetCurrentPosition(0);
        this->duplicateSoundBuffers[soundIndex]->SetPan(averagedPan);

        volumeScale = (f32)g_SoundPlayer.sfxVolume / 100.0f;

        if (g_SoundPlayer.sfxVolume != 0)
        {
            volumeScale = 1.0f - volumeScale;
            volumeScale = volumeScale * volumeScale * volumeScale;
            volumeScale = 1.0f - volumeScale;
            this->duplicateSoundBuffers[soundIndex]->SetVolume(
                (i32)((g_SoundBufferIdxVol[soundIndex].volume + 5000) * volumeScale) - 5000);
        }
        else
        {
            this->duplicateSoundBuffers[soundIndex]->SetVolume(-10000);
        }
        this->duplicateSoundBuffers[soundIndex]->Play(0, 0, 0);
    }

    return this->commandQueue[0].opcode;
}

#undef restartCommandProcessing
#undef averagedPan
#undef i
#undef commandCursor
#undef soundIndex
#undef j
#undef preloadBuffer
#undef bgmPath
#undef bgmFormatIndex
#undef reopenedBuffer
#undef volumeScale

#define msg restartCommandProcessingLocal05
#define looped averagedPanLocal12
#define lpThreadParameterCopy iLocal11
#define waitObj commandCursorLocal02
#define res soundIndexLocal01
#define stopped jLocal00
#pragma var_order(msg, looped, lpThreadParameterCopy, waitObj, res, stopped)
DWORD WINAPI SoundPlayer::BGMPlayerThread(LPVOID lpThreadParameter)
{
    HRESULT res;
    DWORD waitObj;
    u32 looped;
    LPVOID lpThreadParameterCopy;
    u32 stopped;
    MSG msg;

    lpThreadParameterCopy = lpThreadParameter;
    stopped = false;
    looped = true;
    while (!stopped)
    {
        waitObj = MsgWaitForMultipleObjects(1, &g_SoundPlayer.bgmUpdateEvent, FALSE, INFINITE, QS_ALLEVENTS);
        if (g_SoundPlayer.bgm == NULL)
        {
            stopped = true;
        }
        switch (waitObj)
        {
        case 0:
            if (g_SoundPlayer.bgm != NULL && g_SoundPlayer.bgm->m_bIsPlaying)
            {
                g_SoundPlayer.bgm->m_bIsLocked = TRUE;
                res = g_SoundPlayer.bgm->HandleWaveStreamNotification(looped);
                g_SoundPlayer.bgm->m_bIsLocked = FALSE;
            }
            break;
        case 1:
            while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) != 0)
            {
                if (msg.message == WM_QUIT)
                {
                    stopped = true;
                }
            }
            break;
        }
    }
    utils::DebugPrint("\x61\x74\x65\x6E\x74\x69\x6F\x6E\x20\x3A\x20\x83\x58\x83\x67\x83\x8A\x81\x5B\x83\x7E\x83\x93\x83\x4F\x97\x70\x83\x58\x83\x8C\x83\x62\x83\x68\x82\xCD\x8F\x49\x97\xB9\x82\xB5\x82\xDC\x82\xB5\x82\xBD\x81\x42\r\n");
    return 0;
}
#undef msg
#undef looped
#undef lpThreadParameterCopy
#undef waitObj
#undef res
#undef stopped

void SoundPlayer::QueueCommand(i32 opcode, i32 argument, char *path)
{
    i32 i;

    for (i = 0; i < ARRAY_SIZE_SIGNED(this->commandQueue) - 1; i++)
    {
        if (this->commandQueue[i].opcode != SOUNDPLAYER_COMMAND_NONE)
            continue;

        this->commandQueue[i].opcode = opcode;
        this->commandQueue[i].argument = argument;
        strcpy(this->commandQueue[i].path, path);
        this->commandQueue[i].step = 0;

        break;
    }

    utils::DebugPrint("Sound Que Add %d\r\n", opcode);
}

ZunResult SoundPlayer::Release()
{
    i32 i;

    if (this->bgmFmtData != NULL)
    {
        g_ZunMemory.Free(this->bgmFmtData);
    }
    for (i = 0; i < NUM_SOUND_BUFFERS; i++)
    {
        if (this->duplicateSoundBuffers[i] != NULL)
        {
            this->duplicateSoundBuffers[i]->Release();
            this->duplicateSoundBuffers[i] = NULL;
        }
        if (this->soundBuffers[i] != NULL)
        {
            this->soundBuffers[i]->Release();
            this->soundBuffers[i] = NULL;
        }
    }
    for (i = 0; i < 37; i++)
    {
        if (this->ownedMusicMetadata[i] != NULL)
        {
            g_ZunMemory.Free(this->ownedMusicMetadata[i]);
            this->ownedMusicMetadata[i] = NULL;
        }
    }
    if (this->manager == NULL)
    {
        return ZUN_SUCCESS;
    }
    KillTimer(this->gameWindow, 1);
    this->StopBGM();
    this->dsoundHdl = NULL;
    this->initSoundBuffer->Stop();
    if (this->initSoundBuffer != NULL)
    {
        this->initSoundBuffer->Release();
        this->initSoundBuffer = NULL;
    }
    if (this->bgm != NULL)
    {
        delete this->bgm;
        this->bgm = NULL;
    }
    if (this->manager != NULL)
    {
        delete this->manager;
        this->manager = NULL;
    }
    for (i = 0; i < NUM_BGM_SLOTS; i++)
    {
        if (this->bgmPreloadAllocations[i] != NULL)
        {
            g_ZunMemory.Free(this->bgmPreloadAllocations[i]);
            this->bgmPreloadAllocations[i] = NULL;
        }
    }
    return ZUN_SUCCESS;
}

SoundPlayer::SoundPlayer()
{
    ZeroMemory(this, sizeof(SoundPlayer));
    for (i32 i = 0; i < NUM_SOUND_BUFFERS; i++)
    {
        this->unconsumedMetadataBySound[i] = -1;
    }
}

} // namespace th095
