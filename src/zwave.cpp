#define STRICT

#include <stddef.h>

#include "SoundPlayer.hpp"
#include "dxutil.hpp"
#include "utils.hpp"
#include "zwave.hpp"

#include <dsound.h>
#include <windows.h>

namespace th095
{

// ZUN extensions to the Microsoft DSUtil sound wrapper. The TH08 source is
// the source-shape oracle; every TH095 address and layout remains target-local.

// FUNCTION: TH095 0x00452E70.
CSoundManager::~CSoundManager()
{
    SAFE_RELEASE(m_pDS);
}

// FUNCTION: TH095 0x00452EA0.
HRESULT CSoundManager::Initialize(
    HWND window,
    DWORD cooperativeLevel,
    DWORD primaryChannels,
    DWORD primaryFrequency,
    DWORD primaryBitRate)
{
    struct InitializeLocals
    {
        HRESULT hr;
        LPDIRECTSOUNDBUFFER primaryBuffer;
    } locals;
#define hr locals.hr
#define primaryBuffer locals.primaryBuffer

    primaryBuffer = NULL;

    SAFE_RELEASE(m_pDS);
    if (FAILED(hr = DirectSoundCreate8(NULL, &m_pDS, NULL)))
        return hr;
    if (FAILED(hr = m_pDS->SetCooperativeLevel(window, cooperativeLevel)))
        return hr;

    SetPrimaryBufferFormat(
        primaryChannels, primaryFrequency, primaryBitRate);
#undef primaryBuffer
#undef hr
    return S_OK;
}

// FUNCTION: TH095 0x00452F30.
HRESULT CSoundManager::SetPrimaryBufferFormat(
    DWORD primaryChannels,
    DWORD primaryFrequency,
    DWORD primaryBitRate)
{
    struct PrimaryFormatLocals
    {
        HRESULT hr;
        DSBUFFERDESC bufferDescription;
        WAVEFORMATEX format;
        LPDIRECTSOUNDBUFFER primaryBuffer;
    } locals;
#define hr locals.hr
#define bufferDescription locals.bufferDescription
#define format locals.format
#define primaryBuffer locals.primaryBuffer

    primaryBuffer = NULL;

    if (m_pDS == NULL)
        return CO_E_NOTINITIALIZED;

    ZeroMemory(&bufferDescription, sizeof(bufferDescription));
    bufferDescription.dwSize = sizeof(bufferDescription);
    bufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;
    bufferDescription.dwBufferBytes = 0;
    bufferDescription.lpwfxFormat = NULL;
    if (FAILED(hr = m_pDS->CreateSoundBuffer(
            &bufferDescription, &primaryBuffer, NULL)))
        return hr;

    ZeroMemory(&format, sizeof(format));
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = (WORD)primaryChannels;
    format.nSamplesPerSec = primaryFrequency;
    format.wBitsPerSample = (WORD)primaryBitRate;
    format.nBlockAlign = format.wBitsPerSample / 8 * format.nChannels;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
    if (FAILED(hr = primaryBuffer->SetFormat(&format)))
        return hr;

    SAFE_RELEASE(primaryBuffer);
#undef primaryBuffer
#undef format
#undef bufferDescription
#undef hr
    return S_OK;
}

// FUNCTION: TH095 0x00453050.
HRESULT CSoundManager::CreateStreaming(
    CStreamingSound **streamingSound,
    LPTSTR filename,
    DWORD creationFlags,
    GUID algorithm,
    DWORD notifyCount,
    DWORD notifySize,
    HANDLE notifyEvent,
    ThBgmFormat *format)
{
    struct StreamingLocals
    {
        LPDIRECTSOUNDNOTIFY notify;
        HRESULT hr;
        DSBUFFERDESC bufferDescription;
        LPDIRECTSOUNDBUFFER soundBuffer;
        CWaveFile *waveFile;
        DSBPOSITIONNOTIFY *notifications;
        DWORD bufferSize;
    } locals;
#define notify locals.notify
#define hr locals.hr
#define bufferDescription locals.bufferDescription
#define soundBuffer locals.soundBuffer
#define waveFile locals.waveFile
#define notifications locals.notifications
#define bufferSize locals.bufferSize

    if (m_pDS == NULL)
        return CO_E_NOTINITIALIZED;

    soundBuffer = NULL;
    waveFile = NULL;
    notifications = NULL;
    notify = NULL;

    waveFile = new CWaveFile();
    if (waveFile->Open(filename, format, WAVEFILE_READ) != S_OK)
    {
        delete waveFile;
        waveFile = NULL;
        return E_FAIL;
    }

    bufferSize = notifySize * notifyCount;
    ZeroMemory(&bufferDescription, sizeof(bufferDescription));
    bufferDescription.dwSize = sizeof(bufferDescription);
    bufferDescription.dwFlags =
        creationFlags | DSBCAPS_CTRLPOSITIONNOTIFY |
        DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2 |
        DSBCAPS_CTRLVOLUME | DSBCAPS_LOCSOFTWARE;
    bufferDescription.dwBufferBytes = bufferSize;
    bufferDescription.guid3DAlgorithm = algorithm;
    bufferDescription.lpwfxFormat = &waveFile->m_pzwf->format;

    if (FAILED(hr = m_pDS->CreateSoundBuffer(
            &bufferDescription, &soundBuffer, NULL)))
        return E_FAIL;
    if (FAILED(hr = soundBuffer->QueryInterface(
            IID_IDirectSoundNotify, (VOID **)&notify)))
        return E_FAIL;

    notifications = new DSBPOSITIONNOTIFY[notifyCount];
    if (notifications == NULL)
        return E_OUTOFMEMORY;
    for (DWORD i = 0; i < notifyCount; ++i)
    {
        notifications[i].dwOffset = notifySize * i + notifySize - 1;
        notifications[i].hEventNotify = notifyEvent;
    }

    if (FAILED(hr = notify->SetNotificationPositions(
            notifyCount, notifications)))
    {
        SAFE_RELEASE(notify);
        SAFE_DELETE(notifications);
        return E_FAIL;
    }
    SAFE_RELEASE(notify);
    SAFE_DELETE(notifications);

    *streamingSound =
        new CStreamingSound(soundBuffer, bufferSize, waveFile, notifySize);
    CopyMemory(
        &(*streamingSound)->m_dsbd,
        &bufferDescription,
        sizeof(DSBUFFERDESC));
    (*streamingSound)->m_pSoundManager = this;
    (*streamingSound)->m_hNotifyEvent = notifyEvent;
    (*streamingSound)->m_bIsLocked = FALSE;
#undef bufferSize
#undef notifications
#undef waveFile
#undef soundBuffer
#undef bufferDescription
#undef hr
#undef notify
    return S_OK;
}

// FUNCTION: TH095 0x004533B0.
HRESULT CSoundManager::CreateStreamingFromMemory(
    CStreamingSound **streamingSound,
    BYTE *data,
    ULONG dataSize,
    ThBgmFormat *format,
    DWORD creationFlags,
    GUID algorithm,
    DWORD notifyCount,
    DWORD notifySize,
    HANDLE notifyEvent)
{
    struct StreamingLocals
    {
        LPDIRECTSOUNDNOTIFY notify;
        HRESULT hr;
        DSBUFFERDESC bufferDescription;
        LPDIRECTSOUNDBUFFER soundBuffer;
        CWaveFile *waveFile;
        DSBPOSITIONNOTIFY *notifications;
        DWORD bufferSize;
    } locals;
#define notify locals.notify
#define hr locals.hr
#define bufferDescription locals.bufferDescription
#define soundBuffer locals.soundBuffer
#define waveFile locals.waveFile
#define notifications locals.notifications
#define bufferSize locals.bufferSize

    utils::DebugPrint("StreamingSound Create \r\n");
    if (m_pDS == NULL)
        return CO_E_NOTINITIALIZED;

    soundBuffer = NULL;
    waveFile = NULL;
    notifications = NULL;
    notify = NULL;

    waveFile = new CWaveFile();
    waveFile->OpenFromMemory(data, dataSize, format, 0);

    bufferSize = notifySize * notifyCount;
    ZeroMemory(&bufferDescription, sizeof(bufferDescription));
    bufferDescription.dwSize = sizeof(bufferDescription);
    bufferDescription.dwFlags =
        creationFlags | DSBCAPS_CTRLPOSITIONNOTIFY |
        DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2 |
        DSBCAPS_CTRLVOLUME | DSBCAPS_LOCSOFTWARE;
    bufferDescription.dwBufferBytes = bufferSize;
    bufferDescription.guid3DAlgorithm = algorithm;
    bufferDescription.lpwfxFormat = &waveFile->m_pzwf->format;

    if (FAILED(hr = m_pDS->CreateSoundBuffer(
            &bufferDescription, &soundBuffer, NULL)))
        return E_FAIL;
    if (FAILED(hr = soundBuffer->QueryInterface(
            IID_IDirectSoundNotify, (VOID **)&notify)))
        return E_FAIL;

    notifications = new DSBPOSITIONNOTIFY[notifyCount];
    if (notifications == NULL)
        return E_OUTOFMEMORY;
    for (DWORD i = 0; i < notifyCount; ++i)
    {
        notifications[i].dwOffset = notifySize * i + notifySize - 1;
        notifications[i].hEventNotify = notifyEvent;
    }

    if (FAILED(hr = notify->SetNotificationPositions(
            notifyCount, notifications)))
    {
        SAFE_RELEASE(notify);
        SAFE_DELETE(notifications);
        return E_FAIL;
    }
    SAFE_RELEASE(notify);
    SAFE_DELETE(notifications);

    *streamingSound =
        new CStreamingSound(soundBuffer, bufferSize, waveFile, notifySize);
    CopyMemory(
        &(*streamingSound)->m_dsbd,
        &bufferDescription,
        sizeof(DSBUFFERDESC));
    (*streamingSound)->m_pSoundManager = this;
    (*streamingSound)->m_hNotifyEvent = notifyEvent;
    (*streamingSound)->m_bIsLocked = FALSE;
    utils::DebugPrint("Success \r\n");
#undef bufferSize
#undef notifications
#undef waveFile
#undef soundBuffer
#undef bufferDescription
#undef hr
#undef notify
    return S_OK;
}

// FUNCTION: TH095 0x004536E0.
CSound::CSound(
    LPDIRECTSOUNDBUFFER *soundBuffers,
    DWORD bufferSize,
    DWORD bufferCount,
    CWaveFile *waveFile)
{
    DWORD i;

    m_apDSBuffer = new LPDIRECTSOUNDBUFFER[bufferCount];
    for (i = 0; i < bufferCount; ++i)
        m_apDSBuffer[i] = soundBuffers[i];

    m_dwDSBufferSize = bufferSize;
    m_dwNumBuffers = bufferCount;
    m_pWaveFile = waveFile;
    FillBufferWithSound(m_apDSBuffer[0], FALSE);
    for (i = 0; i < bufferCount; ++i)
        m_apDSBuffer[i]->SetCurrentPosition(0);
    m_bIsPlaying = FALSE;
}

// FUNCTION: TH095 0x004537F0.
HRESULT CStreamingSound::InitSoundBuffers()
{
    struct InitSoundBufferLocals
    {
        DWORD j;
        LPDIRECTSOUNDNOTIFY notify;
        DSBPOSITIONNOTIFY *notifications;
        DWORD i;
    } locals;
#define j locals.j
#define notify locals.notify
#define notifications locals.notifications
#define i locals.i

    m_bIsPlaying = FALSE;

    for (i = 0; i < m_dwNumBuffers; ++i)
        SAFE_RELEASE(m_apDSBuffer[i]);
    SAFE_DELETE(m_apDSBuffer);

    notifications = NULL;
    notify = NULL;
    m_apDSBuffer = new LPDIRECTSOUNDBUFFER[m_dwNumBuffers];

    for (i = 0; i < m_dwNumBuffers; ++i)
    {
        if (FAILED(m_pSoundManager->m_pDS->CreateSoundBuffer(
                &m_dsbd, &m_apDSBuffer[i], NULL)))
            return E_FAIL;
        if (FAILED(m_apDSBuffer[i]->QueryInterface(
                IID_IDirectSoundNotify, (VOID **)&notify)))
            return E_FAIL;

        notifications = new DSBPOSITIONNOTIFY[16];
        if (notifications == NULL)
            return E_OUTOFMEMORY;

        for (j = 0; j < 16; ++j)
        {
            notifications[j].dwOffset =
                m_dwNotifySize * j + m_dwNotifySize - 1;
            notifications[j].hEventNotify = m_hNotifyEvent;
        }

        if (FAILED(notify->SetNotificationPositions(16, notifications)))
        {
            SAFE_RELEASE(notify);
            SAFE_DELETE(notifications);
            return E_FAIL;
        }
        SAFE_RELEASE(notify);
        SAFE_DELETE(notifications);
    }
#undef i
#undef notifications
#undef notify
#undef j
    return S_OK;
}

// FUNCTION: TH095 0x00453A50.
CSound::~CSound()
{
    for (DWORD i = 0; i < m_dwNumBuffers; ++i)
        SAFE_RELEASE(m_apDSBuffer[i]);
    SAFE_DELETE_ARRAY(m_apDSBuffer);
    SAFE_DELETE(m_pWaveFile);
}

// FUNCTION: TH095 0x00453B40.
HRESULT CSound::FillBufferWithSound(
    LPDIRECTSOUNDBUFFER soundBuffer, BOOL repeatIfLarger)
{
    HRESULT hr;
    VOID *lockedBuffer = NULL;
    DWORD lockedSize = 0;
    DWORD waveBytesRead = 0;

    if (soundBuffer == NULL)
        return CO_E_NOTINITIALIZED;
    if (FAILED(hr = RestoreBuffer(soundBuffer, NULL)))
        return hr;
    if (FAILED(hr = soundBuffer->Lock(
            0,
            m_dwDSBufferSize,
            &lockedBuffer,
            &lockedSize,
            NULL,
            NULL,
            0)))
        return hr;

    m_pWaveFile->ResetFile(false);
    if (FAILED(hr = m_pWaveFile->Read(
            (BYTE *)lockedBuffer, lockedSize, &waveBytesRead)))
        return hr;

    if (waveBytesRead == 0)
    {
        FillMemory(
            (BYTE *)lockedBuffer,
            lockedSize,
            (BYTE)(m_pWaveFile->m_pzwf->format.wBitsPerSample == 8 ? 128 : 0));
    }
    else if (waveBytesRead < lockedSize)
    {
        if (repeatIfLarger)
        {
            DWORD readSoFar = waveBytesRead;
            while (readSoFar < lockedSize)
            {
                if (FAILED(hr = m_pWaveFile->ResetFile(false)))
                    return hr;
                hr = m_pWaveFile->Read(
                    (BYTE *)lockedBuffer + readSoFar,
                    lockedSize - readSoFar,
                    &waveBytesRead);
                if (FAILED(hr))
                    return hr;
                readSoFar += waveBytesRead;
            }
        }
        else
        {
            FillMemory(
                (BYTE *)lockedBuffer + waveBytesRead,
                lockedSize - waveBytesRead,
                (BYTE)(
                    m_pWaveFile->m_pzwf->format.wBitsPerSample == 8 ? 128 : 0));
        }
    }

    soundBuffer->Unlock(lockedBuffer, lockedSize, NULL, 0);
    return S_OK;
}

// FUNCTION: TH095 0x00453D30.
HRESULT CSound::RestoreBuffer(
    LPDIRECTSOUNDBUFFER soundBuffer, BOOL *wasRestored)
{
    HRESULT hr;

    if (soundBuffer == NULL)
        return CO_E_NOTINITIALIZED;
    if (wasRestored != NULL)
        *wasRestored = FALSE;

    DWORD status;
    if (FAILED(hr = soundBuffer->GetStatus(&status)))
        return hr;
    if (status & DSBSTATUS_BUFFERLOST)
    {
        do
        {
            hr = soundBuffer->Restore();
            if (hr == DSERR_BUFFERLOST)
                Sleep(10);
        } while (hr = soundBuffer->Restore());

        if (wasRestored != NULL)
            *wasRestored = TRUE;
        return S_OK;
    }
    return S_FALSE;
}

// FUNCTION: TH095 0x00453DE0.
LPDIRECTSOUNDBUFFER CSound::GetFreeBuffer()
{
    BOOL isPlaying = FALSE;

    if (m_apDSBuffer == NULL)
        return FALSE;
    for (DWORD i = 0; i < m_dwNumBuffers; ++i)
    {
        if (m_apDSBuffer[i] != NULL)
        {
            DWORD status = 0;
            m_apDSBuffer[i]->GetStatus(&status);
            if ((status & DSBSTATUS_PLAYING) == 0)
                break;
        }
    }

    if (i != m_dwNumBuffers)
        return m_apDSBuffer[i];
    return m_apDSBuffer[rand() % m_dwNumBuffers];
}

// FUNCTION: TH095 0x00453EA0.
LPDIRECTSOUNDBUFFER CSound::GetBuffer(DWORD index)
{
    if (m_apDSBuffer == NULL)
        return NULL;
    if (index >= m_dwNumBuffers)
        return NULL;
    return m_apDSBuffer[index];
}

// FUNCTION: TH095 0x00453EE0.
HRESULT CSound::Play(DWORD priority, DWORD flags)
{
    struct PlayLocals
    {
        HRESULT hr;
        BOOL restored;
        LPDIRECTSOUNDBUFFER soundBuffer;
    } locals;
#define hr locals.hr
#define restored locals.restored
#define soundBuffer locals.soundBuffer

    if (m_apDSBuffer == NULL)
        return CO_E_NOTINITIALIZED;
    soundBuffer = GetFreeBuffer();
    if (soundBuffer == NULL)
        return E_FAIL;
    if (FAILED(hr = RestoreBuffer(soundBuffer, &restored)))
        return hr;
    if (restored)
    {
        if (FAILED(hr = FillBufferWithSound(soundBuffer, FALSE)))
            return hr;
        Reset();
    }

    m_iFadeType = 0;
    m_iCurFadeProgress = 0;
    m_iTotalFade = 0;
    SetVolume(0);
    m_bIsPlaying = TRUE;
    m_dwPriority = priority;
    m_dwFlags = flags;
    unconsumedDword2C = 0;
    return soundBuffer->Play(0, priority, flags);
#undef soundBuffer
#undef restored
#undef hr
}

// FUNCTION: TH095 0x00453FD0.
HRESULT CSound::SetVolume(i32 volume)
{
    f32 volumeScale = g_SoundPlayer.bgmVolume / 100.0f;

    if (g_SoundPlayer.bgmVolume != 0)
    {
        volumeScale = 1.0f - volumeScale;
        volumeScale = volumeScale * volumeScale;
        volumeScale = 1.0f - volumeScale;
        return m_apDSBuffer[0]->SetVolume(
            (i32)((volume + 5000) * volumeScale) - 5000);
    }
    return m_apDSBuffer[0]->SetVolume(DSBVOLUME_MIN);
}

// FUNCTION: TH095 0x00454070.
HRESULT CSound::Stop()
{
    if (m_apDSBuffer == NULL)
        return CO_E_NOTINITIALIZED;

    HRESULT hr = 0;
    m_bIsPlaying = FALSE;
    for (DWORD i = 0; i < m_dwNumBuffers; ++i)
    {
        hr |= m_apDSBuffer[i]->Stop();
        hr |= m_apDSBuffer[i]->SetCurrentPosition(0);
    }
    m_iFadeType = 0;
    return hr;
}

// FUNCTION: TH095 0x00454120.
HRESULT CSound::Pause()
{
    if (m_apDSBuffer == NULL)
        return CO_E_NOTINITIALIZED;

    HRESULT hr = 0;
    m_bIsPlaying = FALSE;
    hr |= m_apDSBuffer[0]->Stop();
    return hr;
}

// FUNCTION: TH095 0x00454170.
HRESULT CSound::Unpause()
{
    if (m_apDSBuffer == NULL)
        return CO_E_NOTINITIALIZED;

    LPDIRECTSOUNDBUFFER buffer = m_apDSBuffer[0];
    m_bIsPlaying = TRUE;
    return buffer->Play(0, m_dwPriority, m_dwFlags);
}

// FUNCTION: TH095 0x004541C0.
HRESULT CSound::Reset()
{
    if (m_apDSBuffer == NULL)
        return CO_E_NOTINITIALIZED;

    HRESULT hr = 0;
    for (DWORD i = 0; i < m_dwNumBuffers; ++i)
        hr |= m_apDSBuffer[i]->SetCurrentPosition(0);
    return hr;
}

// FUNCTION: TH095 0x00454230.
CStreamingSound::CStreamingSound(
    LPDIRECTSOUNDBUFFER buffer,
    DWORD bufferSize,
    CWaveFile *waveFile,
    DWORD notifySize)
    : CSound(&buffer, bufferSize, 1, waveFile)
{
    m_dwLastPlayPos = 0;
    m_dwPlayProgress = 0;
    m_dwNotifySize = notifySize;
    m_dwNextWriteOffset = 0;
    m_bFillNextNotificationWithSilence = FALSE;
}

// FUNCTION: TH095 0x004542C0. The compiler emits the scalar deleting
// destructor at 0x00454290 from this virtual destructor definition.
CStreamingSound::~CStreamingSound()
{
}

// FUNCTION: TH095 0x004542E0.
HRESULT CStreamingSound::UpdateFadeOut()
{
    if (m_iFadeType == 1)
    {
        if (--m_iCurFadeProgress <= 0)
        {
            m_iFadeType = 0;
            m_apDSBuffer[0]->Stop();
            return S_FALSE;
        }
        i32 newVolume = m_iCurFadeProgress * 5000 / m_iTotalFade - 5000;
        HRESULT hr = SetVolume(newVolume);
    }
    return S_OK;
}

// FUNCTION: TH095 0x00454370.
HRESULT CStreamingSound::UpdateFadeIn()
{
    if (m_iFadeType == 2)
    {
        if (--m_iCurFadeProgress <= 0)
        {
            m_iFadeType = 0;
            return S_FALSE;
        }
        i32 newVolume = 0 - m_iCurFadeProgress * 5000 / m_iTotalFade;
        HRESULT hr = SetVolume(newVolume);
    }
    return S_OK;
}

// FUNCTION: TH095 0x00454450.
HRESULT CStreamingSound::UpdatePartialFadeOut()
{
    if (m_iFadeType == 4)
    {
        if (--m_iCurFadeProgress <= 0)
        {
            m_iFadeType = 0;
            return S_FALSE;
        }
        i32 newVolume = m_iCurFadeProgress * 1000 / m_iTotalFade - 1000;
        HRESULT hr = SetVolume(newVolume);
    }
    return S_OK;
}

// FUNCTION: TH095 0x004543E0.
HRESULT CStreamingSound::UpdatePartialFadeIn()
{
    if (m_iFadeType == 3)
    {
        if (--m_iCurFadeProgress <= 0)
        {
            m_iFadeType = 0;
            return S_FALSE;
        }
        i32 newVolume = 0 - m_iCurFadeProgress * 1000 / m_iTotalFade;
        HRESULT hr = SetVolume(newVolume);
    }
    return S_OK;
}

// FUNCTION: TH095 0x004544C0.
HRESULT CStreamingSound::HandleWaveStreamNotification(BOOL looped)
{
    struct StreamNotificationLocals
    {
        DWORD readSoFar;
        DWORD writeCursor;
        DWORD currentPlayPosition;
        DWORD lockedSize;
        HRESULT hr;
        DWORD playDelta;
        BOOL restored;
        VOID *lockedBuffer2;
        DWORD playCursor;
        DWORD bytesWritten;
        VOID *lockedBuffer;
        DWORD lockedSize2;
    } locals;
#define readSoFar locals.readSoFar
#define writeCursor locals.writeCursor
#define currentPlayPosition locals.currentPlayPosition
#define lockedSize locals.lockedSize
#define hr locals.hr
#define playDelta locals.playDelta
#define restored locals.restored
#define lockedBuffer2 locals.lockedBuffer2
#define playCursor locals.playCursor
#define bytesWritten locals.bytesWritten
#define lockedBuffer locals.lockedBuffer
#define lockedSize2 locals.lockedSize2

    if (m_apDSBuffer == NULL || m_pWaveFile == NULL)
        return CO_E_NOTINITIALIZED;

    m_apDSBuffer[0]->GetCurrentPosition(&playCursor, &writeCursor);
    if ((m_dwNextWriteOffset >= writeCursor - m_dwNotifySize &&
         m_dwNextWriteOffset < writeCursor) ||
        (writeCursor - m_dwNotifySize < 0 &&
         m_dwNextWriteOffset >= m_dwDSBufferSize - m_dwNotifySize))
    {
        utils::DebugPrint("Stream Skip\n");
        return CO_E_FIRST;
    }

    if (FAILED(hr = RestoreBuffer(m_apDSBuffer[0], &restored)))
    {
        utils::DebugPrint(
            "error : RestoreBuffer in HandleWaveStreamNotification\r\n");
        return hr;
    }
    if (restored)
    {
        if (FAILED(hr = FillBufferWithSound(m_apDSBuffer[0], FALSE)))
        {
            utils::DebugPrint(
                "error : FillBufferWithSound in HandleWaveStreamNotification\r\n");
            return hr;
        }
        return S_OK;
    }

    lockedBuffer = NULL;
    lockedBuffer2 = NULL;
    if (FAILED(hr = m_apDSBuffer[0]->Lock(
            m_dwNextWriteOffset,
            m_dwNotifySize,
            &lockedBuffer,
            &lockedSize,
            &lockedBuffer2,
            &lockedSize2,
            0)))
    {
        utils::DebugPrint(
            "error : Buffer->Lock in HandleWaveStreamNotification\r\n");
        return hr;
    }
    if (lockedBuffer2 != NULL)
        return E_UNEXPECTED;

    if (!m_bFillNextNotificationWithSilence)
    {
        if (FAILED(hr = m_pWaveFile->Read(
                (BYTE *)lockedBuffer, lockedSize, &bytesWritten)))
        {
            utils::DebugPrint(
                "error : m_pWaveFile->Read in HandleWaveStreamNotification\r\n");
            return hr;
        }
    }
    else
    {
        FillMemory(
            lockedBuffer,
            lockedSize,
            (BYTE)(m_pWaveFile->m_pzwf->format.wBitsPerSample == 8 ? 128 : 0));
        bytesWritten = lockedSize;
    }

    if (bytesWritten < lockedSize)
    {
        if (!looped)
        {
            FillMemory(
                (BYTE *)lockedBuffer + bytesWritten,
                lockedSize - bytesWritten,
                (BYTE)(m_pWaveFile->m_pzwf->format.wBitsPerSample == 8 ? 128 : 0));
            m_bFillNextNotificationWithSilence = TRUE;
        }
        else
        {
            readSoFar = bytesWritten;
            while (readSoFar < lockedSize)
            {
                if (FAILED(hr = m_pWaveFile->ResetFile(true)))
                {
                    utils::DebugPrint(
                        "error : m_pWaveFile->ResetFile in HandleWaveStreamNotification\r\n");
                    return hr;
                }
                if (FAILED(hr = m_pWaveFile->Read(
                        (BYTE *)lockedBuffer + readSoFar,
                        lockedSize - readSoFar,
                        &bytesWritten)))
                {
                    utils::DebugPrint(
                        "error : m_pWaveFile->Read(+) in HandleWaveStreamNotification\r\n");
                    return hr;
                }
                readSoFar += bytesWritten;
            }
        }
    }

    m_apDSBuffer[0]->Unlock(lockedBuffer, lockedSize, NULL, 0);
    if (FAILED(hr = m_apDSBuffer[0]->GetCurrentPosition(
            &currentPlayPosition, NULL)))
    {
        utils::DebugPrint(
            "error : m_apDSBuffer[0]->GetCurrentPosition in HandleWaveStreamNotification\r\n");
        return hr;
    }

    if (currentPlayPosition < m_dwLastPlayPos)
        playDelta =
            m_dwDSBufferSize - m_dwLastPlayPos + currentPlayPosition;
    else
        playDelta = currentPlayPosition - m_dwLastPlayPos;
    m_dwPlayProgress += playDelta;
    m_dwLastPlayPos = currentPlayPosition;

    if (m_bFillNextNotificationWithSilence &&
        m_dwPlayProgress >= m_pWaveFile->GetSize())
        m_apDSBuffer[0]->Stop();

    m_dwNextWriteOffset += lockedSize;
    m_dwNextWriteOffset %= m_dwDSBufferSize;
#undef lockedSize2
#undef lockedBuffer
#undef bytesWritten
#undef playCursor
#undef lockedBuffer2
#undef restored
#undef playDelta
#undef hr
#undef lockedSize
#undef currentPlayPosition
#undef writeCursor
#undef readSoFar
    return S_OK;
}

// FUNCTION: TH095 0x004548B0.
HRESULT CStreamingSound::Reset()
{
    struct ResetLocals
    {
        HRESULT hr;
        BOOL restored;
    } locals;

    if (m_apDSBuffer[0] == NULL || m_pWaveFile == NULL)
        return CO_E_NOTINITIALIZED;

    m_dwLastPlayPos = 0;
    m_dwPlayProgress = 0;
    m_dwNextWriteOffset = 0;
    m_bFillNextNotificationWithSilence = FALSE;

    if (FAILED(locals.hr = RestoreBuffer(m_apDSBuffer[0], &locals.restored)))
        return locals.hr;
    if (locals.restored)
    {
        if (FAILED(locals.hr = FillBufferWithSound(m_apDSBuffer[0], FALSE)))
            return locals.hr;
    }

    m_pWaveFile->ResetFile(false);
    return m_apDSBuffer[0]->SetCurrentPosition(0);
}

// FUNCTION: TH095 0x00454980.
CWaveFile::CWaveFile()
{
    m_pzwf = NULL;
    m_hmmio = NULL;
    m_dwSize = 0;
    m_bIsReadingFromMemory = FALSE;
}

// FUNCTION: TH095 0x004549C0.
CWaveFile::~CWaveFile()
{
    Close();
}

// FUNCTION: TH095 0x004549E0.
HRESULT CWaveFile::Open(
    LPTSTR filename, ThBgmFormat *format, DWORD flags)
{
    m_dwFlags = flags;
    m_bIsReadingFromMemory = FALSE;
    if (m_dwFlags == WAVEFILE_READ)
    {
        if (filename == NULL)
            return E_INVALIDARG;
        utils::DebugPrint("Streaming File Open %s\r\n", filename);
        m_hWaveFile = CreateFileA(
            filename,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
            NULL);
        if (m_hWaveFile == INVALID_HANDLE_VALUE)
            return E_FAIL;
        m_pzwf = format;
        ResetFile(false);
        m_dwSize = m_ck.cksize;
    }
    return S_OK;
}

// FUNCTION: TH095 0x00454A90.
HRESULT CWaveFile::Reopen(ThBgmFormat *format)
{
    if (m_bIsReadingFromMemory)
        return E_FAIL;
    if (m_hWaveFile == INVALID_HANDLE_VALUE)
        return E_FAIL;
    m_pzwf = format;
    ResetFile(false);
    m_dwSize = m_ck.cksize;
    return S_OK;
}

// FUNCTION: TH095 0x00454AF0.
HRESULT CWaveFile::OpenFromMemory(
    BYTE *data,
    ULONG dataSize,
    ThBgmFormat *format,
    DWORD flags)
{
    m_pzwf = format;
    m_ulDataSize = dataSize;
    m_pbData = data;
    m_pbDataCur = m_pbData;
    m_bIsReadingFromMemory = TRUE;
    if (flags != WAVEFILE_READ)
        return E_NOTIMPL;
    return S_OK;
}

// FUNCTION: TH095 0x00454B50.
DWORD CWaveFile::GetSize()
{
    return m_dwSize;
}

// FUNCTION: TH095 0x00454B70.
HRESULT CWaveFile::ResetFile(bool loop)
{
    DWORD seekResult;
    if (m_bIsReadingFromMemory)
    {
        m_pbDataCur = m_pbData;
        if (m_pzwf->totalLength > 0)
            m_ulDataSize = m_pzwf->totalLength;
        if (loop && m_pzwf->introLength > 0)
            m_pbDataCur += m_pzwf->introLength;
    }
    else
    {
        if (m_hWaveFile == NULL)
            return CO_E_NOTINITIALIZED;
        if (loop && m_pzwf->introLength > 0)
        {
            seekResult = SetFilePointer(
                m_hWaveFile,
                g_SoundPlayer.bgmFileBaseOffset +
                    m_pzwf->introLength + m_pzwf->startOffset,
                NULL,
                FILE_BEGIN);
            m_ck.cksize = m_pzwf->totalLength - m_pzwf->introLength;
        }
        else
        {
            seekResult = SetFilePointer(
                m_hWaveFile,
                g_SoundPlayer.bgmFileBaseOffset + m_pzwf->startOffset,
                NULL,
                FILE_BEGIN);
            m_ck.cksize = m_pzwf->totalLength;
        }
    }
    return S_OK;
}

// FUNCTION: TH095 0x00454CC0.
HRESULT CWaveFile::Read(
    BYTE *buffer,
    DWORD bytesToRead,
    DWORD *bytesRead)
{
    if (m_bIsReadingFromMemory)
    {
        if (m_pbDataCur == NULL)
            return CO_E_NOTINITIALIZED;
        if (bytesRead != NULL)
            *bytesRead = 0;
        if (m_pbDataCur + bytesToRead > m_pbData + m_ulDataSize)
            bytesToRead = m_ulDataSize - (DWORD)(m_pbDataCur - m_pbData);
        CopyMemory(buffer, m_pbDataCur, bytesToRead);
        m_pbDataCur += bytesToRead;
        if (bytesRead != NULL)
            *bytesRead = bytesToRead;
        return S_OK;
    }

    if (m_hWaveFile == NULL)
        return CO_E_NOTINITIALIZED;
    if (buffer == NULL || bytesRead == NULL)
        return E_INVALIDARG;

    UINT bytesIn = bytesToRead;
    if (bytesIn > m_ck.cksize)
        bytesIn = m_ck.cksize;
    m_ck.cksize -= bytesIn;

    DWORD size;
    ReadFile(m_hWaveFile, buffer, bytesIn, &size, NULL);
    if (bytesRead != NULL)
        *bytesRead = size;
    return S_OK;
}

// FUNCTION: TH095 0x00454E10.
HRESULT CWaveFile::Close()
{
    if (m_dwFlags == WAVEFILE_READ)
    {
        CloseHandle(m_hWaveFile);
        m_hWaveFile = INVALID_HANDLE_VALUE;
    }
    return S_OK;
}

} // namespace th095
