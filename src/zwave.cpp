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
