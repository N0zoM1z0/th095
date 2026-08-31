#include "inttypes.hpp"
#include <windows.h>
#include <mmreg.h>
#include <mmsystem.h>

#include "Global.hpp"
#include "Midi.hpp"
#include "Supervisor.hpp"
#include "utils.hpp"

namespace th095
{

// TH095 inlines the 32-bit endian swap inside ParseFile; there is no standalone
// Ntohl target body between ParseFile and LoadTracks.
static __forceinline u32 ParseMidiNtohl(u32 val)
{
    u8 tmp[4];
    tmp[0] = ((u8 *)&val)[3];
    tmp[1] = ((u8 *)&val)[2];
    tmp[2] = ((u8 *)&val)[1];
    tmp[3] = ((u8 *)&val)[0];
    return *(const u32 *)tmp;
}

DIFFABLE_STATIC(LARGE_INTEGER, g_DummyMidiTimerPerformanceCounter);

void MidiTimer::OnTimerElapsed()
{
}

MidiDevice::MidiDevice()
{
    handle = NULL;
    this->deviceId = 0;
}

// FUNCTION: TH095 0x00421E00; TH08 Midi.cpp is the source-shape oracle.
MidiDevice::~MidiDevice()
{
    Close();
}

// FUNCTION: TH095 0x00421E20; TH08 Midi.cpp is the source-shape oracle.
BOOL MidiDevice::OpenDevice(UINT uDeviceId)
{
    if (handle != NULL)
    {
        if (this->deviceId != uDeviceId)
        {
            Close();
        }
        else
        {
            return FALSE;
        }
    }

    this->deviceId = uDeviceId;

    return midiOutOpen(&handle, uDeviceId, (DWORD_PTR)g_Supervisor.hwndGameWindow, NULL, CALLBACK_WINDOW) !=
           MMSYSERR_NOERROR;
}

// FUNCTION: TH095 0x00421E80; TH08 Midi.cpp is the source-shape oracle.
ZunResult MidiDevice::Close()
{
    if (handle == NULL)
    {
        return ZUN_ERROR;
    }

    midiOutReset(handle);
    midiOutClose(handle);
    handle = NULL;

    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00421EC0; TH08 Midi.cpp is the source-shape oracle.
BOOL MidiDevice::SendLongMsg(LPMIDIHDR pmh)
{
    if (handle == NULL)
    {
        return FALSE;
    }
    else
    {
        if (midiOutPrepareHeader(handle, pmh, sizeof(*pmh)) != MMSYSERR_NOERROR)
        {
            return TRUE;
        }

        return midiOutLongMsg(handle, pmh, sizeof(*pmh)) != MMSYSERR_NOERROR;
    }
}

union MidiShortMsg {
    struct
    {
        u8 midiStatus;
        i8 firstByte;
        i8 secondByte;
        i8 unused;
    } msg;
    DWORD dwMsg;
};

// FUNCTION: TH095 0x00421F10; TH08 Midi.cpp is the source-shape oracle.
BOOL MidiDevice::SendShortMsg(u8 midiStatus, u8 firstByte, u8 secondByte)
{
    MidiShortMsg pkt;

    if (handle == NULL)
    {
        return FALSE;
    }
    else
    {
        pkt.msg.midiStatus = midiStatus;
        pkt.msg.firstByte = firstByte;
        pkt.msg.secondByte = secondByte;
        return midiOutShortMsg(handle, pkt.dwMsg) != MMSYSERR_NOERROR;
    }
}

// FUNCTION: TH095 0x00421F60; TH08 Midi.cpp is the source-shape oracle.
MidiTimer::MidiTimer()
{
    timeGetDevCaps(&this->timeCaps, sizeof(TIMECAPS));
    this->timerId = 0;
}

// FUNCTION: TH095 0x00421F90; TH08 Midi.cpp is the source-shape oracle.
MidiTimer::~MidiTimer()
{
    StopTimerImpl();
    timeEndPeriod(this->timeCaps.wPeriodMin);
}

// FUNCTION: TH095 0x00421FC0; TH08 Midi.cpp is the source-shape oracle.
UINT MidiTimer::StartTimerImpl(u32 delay, LPTIMECALLBACK cb, DWORD_PTR data)
{
    StopTimerImpl();
    timeBeginPeriod(this->timeCaps.wPeriodMin);

    if (cb != NULL)
    {
        this->timerId = timeSetEvent(delay, this->timeCaps.wPeriodMin, cb, data, TIME_PERIODIC);
    }
    else
    {
        this->timerId = timeSetEvent(delay, this->timeCaps.wPeriodMin, (LPTIMECALLBACK)MidiTimer::DefaultTimerCallback,
                                     (DWORD_PTR)this, TIME_PERIODIC);
    }

    return this->timerId;
}

// FUNCTION: TH095 0x00422040; TH08 Midi.cpp is the source-shape oracle.
BOOL MidiTimer::StopTimerImpl()
{
    if (this->timerId != 0)
    {
        timeKillEvent(this->timerId);
    }
    timeEndPeriod(this->timeCaps.wPeriodMin);
    this->timerId = 0;
    return TRUE;
}

// FUNCTION: TH095 0x00422080; TH08 Midi.cpp is the source-shape oracle.
void CALLBACK MidiTimer::DefaultTimerCallback(u32 uTimerID, u32 uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    MidiTimer *timer = (MidiTimer *)dwUser;
    timer->OnTimerElapsed();
}

// FUNCTION: TH095 0x004220A0; TH08 Midi.cpp is the source-shape oracle.
u16 MidiOutput::Ntohs(u16 val)
{
    u8 tmp[2];

    tmp[0] = ((u8 *)&val)[1];
    tmp[1] = ((u8 *)&val)[0];

    return *(const u16 *)(&tmp);
}

// FUNCTION: TH095 0x004220C0; TH08 Midi.cpp is the source-shape oracle.
u32 MidiOutput::SkipVariableLength(LPBYTE *curTrackDataCursor)
{
    u32 length;
    u8 tmp;

    length = 0;
    do
    {
        tmp = **curTrackDataCursor;
        *curTrackDataCursor = *curTrackDataCursor + 1;
        length = length * 0x80 + (tmp & 0x7f);
    } while ((tmp & 0x80) != 0);

    return length;
}

// FUNCTION: TH095 0x00422110; TH08 Midi.cpp is the source-shape oracle.
MidiOutput::~MidiOutput()
{
    StopPlayback();
    ClearTracks();
    for (i32 i = 0; i < 32; i++)
    {
        ReleaseFileData(i);
    }
}

// FUNCTION: TH095 0x004221B0; TH08 Midi.cpp is the source-shape oracle.
ZunResult MidiOutput::ReadFileData(int idx, LPCSTR path)
{
    if (this->activeFileIndex == idx)
    {
        StopPlayback();
    }

    ReleaseFileData(idx);

    this->fileData[idx] = FileSystem::OpenFile((LPSTR)path, NULL, false);
    if (this->fileData[idx] == NULL)
    {
        g_GameErrorContext.Log("error : MIDI File " "\x82\xaa\x93\xc7\x82\xdd\x8d\x9e\x82\xdf\x82\xc8\x82\xa2" " %s \r" "v\r\n", path);
        return ZUN_ERROR;
    }

    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00422230; TH08 Midi.cpp is the source-shape oracle.
void MidiOutput::ReleaseFileData(int idx)
{
    g_ZunMemory.Free(this->fileData[idx]);
    this->fileData[idx] = NULL;
}

// FUNCTION: TH095 0x00422270; TH08 Midi.cpp is the source-shape oracle.
void MidiOutput::ClearTracks()
{
    for (i32 trackIndex = 0; trackIndex < this->numTracks; trackIndex++)
    {
        g_ZunMemory.Free(this->tracks[trackIndex].data);
    }

    g_ZunMemory.Free(this->tracks);
    this->tracks = NULL;
    this->numTracks = 0;
}

// FUNCTION: TH095 0x00422300; TH08 Midi.cpp provides the source/var-order ancestry.
ZunResult MidiOutput::ParseFile(int fileIdx)
{
    // The complete 0x24 source-order record reproduces TH08's documented
    // var_order under stock VC7.1. parseFileData is intentionally store-only:
    // TH08 source contains the snapshot and the TH095 target independently
    // preserves its EBP-0x10 store. It is provenance evidence, not padding.
    struct MidiParseLocals
    {
        LPBYTE parseEndOfHeader;
        u32 parseTrackLength;
        u8 parseHdrRaw[8];
        u32 parseHdrLength;
        LPBYTE parseFileData;
        LPBYTE parseCurrentTrack;
        LPBYTE parseCursor;
        i32 parseTrackIndex;
    } locals;

#define endOfHeaderPointer locals.parseEndOfHeader
#define trackLength locals.parseTrackLength
#define hdrRaw locals.parseHdrRaw
#define hdrLength locals.parseHdrLength
#define currentCursorTrack locals.parseCurrentTrack
#define currentCursor locals.parseCursor
#define trackIdx locals.parseTrackIndex

    ClearTracks();
    currentCursor = this->fileData[fileIdx];
    locals.parseFileData = currentCursor;
    if (currentCursor == NULL)
    {
        utils::DebugPrint("Midi File not loaded\n");
        return ZUN_ERROR;
    }

    memcpy(&hdrRaw, currentCursor, 8);
    currentCursor += sizeof(hdrRaw);
    hdrLength = ParseMidiNtohl(*(u32 *)(&hdrRaw[4]));

    endOfHeaderPointer = currentCursor;
    currentCursor += hdrLength;

    this->fileFormat = MidiOutput::Ntohs(*(u16 *)endOfHeaderPointer);
    this->ticksPerQuarterNote =
        MidiOutput::Ntohs(*(u16 *)(endOfHeaderPointer + 4));
    this->numTracks =
        MidiOutput::Ntohs(*(u16 *)(endOfHeaderPointer + 2));

    this->tracks = (MidiTrack *)g_ZunMemory.Alloc(
        sizeof(MidiTrack) * this->numTracks, "midi");
    memset(this->tracks, 0, sizeof(MidiTrack) * this->numTracks);
    for (trackIdx = 0; trackIdx < this->numTracks; trackIdx += 1)
    {
        currentCursorTrack = currentCursor;
        currentCursor += 8;

        trackLength = ParseMidiNtohl(*(u32 *)&currentCursorTrack[4]);
        this->tracks[trackIdx].dataSize = trackLength;
        this->tracks[trackIdx].data =
            (LPBYTE)g_ZunMemory.Alloc(trackLength, "midi");
        this->tracks[trackIdx].trackPlaying = TRUE;
        memcpy(this->tracks[trackIdx].data, currentCursor, trackLength);
        currentCursor += trackLength;
    }
    this->microsecondsPerQuarterNote = 1000000;
    this->activeFileIndex = fileIdx;
    utils::DebugPrint(" midi open %d\n", fileIdx);
#undef trackIdx
#undef currentCursor
#undef currentCursorTrack
#undef hdrLength
#undef hdrRaw
#undef trackLength
#undef endOfHeaderPointer
    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00422530; TH08 Midi.cpp is the source-shape oracle.
void MidiOutput::LoadTracks()
{
    i32 trackIndex;
    MidiTrack *track = this->tracks;

    this->fadeOutVolumeMultiplier = 1.0;
    this->resetOnlyFadeState2DC = 0;
    this->fadeOutActive = FALSE;
    this->elapsedMillisecondsAtCurrentTempo = 0;
    this->elapsedTicksBeforeTempoChange = 0;

    for (trackIndex = 0; trackIndex < this->numTracks; trackIndex++, track++)
    {
        track->cursor = track->data;
        track->loopCursor = track->cursor;
        track->trackPlaying = TRUE;
        track->nextEventTick = MidiOutput::SkipVariableLength(&track->cursor);
    }
}

// FUNCTION: TH095 0x00422600; TH08 Midi.cpp is the source-shape oracle.
ZunResult MidiOutput::Play()
{
    if (this->tracks == NULL)
    {
        return ZUN_ERROR;
    }

    LoadTracks();
    this->outputDevice.OpenDevice(MIDI_MAPPER);
    StartTimerImpl(1, NULL, NULL);
    utils::DebugPrint(" midi play\n");

    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00422660; TH08 Midi.cpp is the source-shape oracle.
ZunResult MidiOutput::StopPlayback()
{
    if (this->tracks == NULL)
    {
        return ZUN_ERROR;
    }

    for (i32 i = 0; i < ARRAY_SIZE_SIGNED(this->pendingLongMessageHeaders); i++)
    {
        if (this->pendingLongMessageHeaders[i] != NULL)
        {
            UnprepareHeader(this->pendingLongMessageHeaders[i]);
        }
    }

    StopTimerImpl();
    this->outputDevice.Close();
    this->activeFileIndex = -1;

    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x004226E0; TH08 Midi.cpp is the source-shape oracle.
ZunResult MidiOutput::UnprepareHeader(LPMIDIHDR pmh)
{
    if (pmh == NULL)
    {
        utils::DebugPrint("error :\r\n");
    }

    if (this->outputDevice.handle == NULL)
    {
        utils::DebugPrint("error :\r\n");
    }

    for (i32 i = 0; i < ARRAY_SIZE_SIGNED(this->pendingLongMessageHeaders); i++)
    {
        if (this->pendingLongMessageHeaders[i] == pmh)
        {
            this->pendingLongMessageHeaders[i] = NULL;
            goto success;
        }
    }

    return ZUN_ERROR;

success:
    MMRESULT res = midiOutUnprepareHeader(this->outputDevice.handle, pmh, sizeof(*pmh));
    if (res != MMSYSERR_NOERROR)
    {
        utils::DebugPrint("error :\r\n");
    }

    g_ZunMemory.Free(pmh->lpData);
    g_ZunMemory.Free(pmh);
    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x004227B0; TH08 Midi.cpp is the source-shape oracle.
ZunResult MidiOutput::SetFadeOut(u32 ms)
{
    this->fadeOutVolumeMultiplier = 0.0;
    this->fadeOutDurationMs = ms;
    this->fadeOutElapsedMs = 0;
    this->resetOnlyFadeState2DC = 0;
    this->fadeOutActive = TRUE;

    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00422800; TH08 Midi.cpp provides timer-loop ancestry.
void MidiOutput::OnTimerElapsed()
{
    // Target-proven stock-VC7 identifier bucket for the real trackLoaded flag.
    // It moves only this live BOOL to the original allocation phase; the 64-bit
    // next-event comparison temporaries then land naturally at EBP-0x20/-0x1C.
    BOOL bgmFormatIndexLocal05 = FALSE;
    ULONGLONG currentPlaybackTick =
        this->elapsedTicksBeforeTempoChange +
        (this->elapsedMillisecondsAtCurrentTempo *
         this->ticksPerQuarterNote * 1000) /
            this->microsecondsPerQuarterNote;

    if (this->fadeOutActive != FALSE)
    {
        if (this->fadeOutElapsedMs < this->fadeOutDurationMs)
        {
            this->fadeOutVolumeMultiplier =
                1.0f - (f32)this->fadeOutElapsedMs /
                           (f32)this->fadeOutDurationMs;
            if ((u32)(this->fadeOutVolumeMultiplier * 128.0f) !=
                this->fadeOutLastSetVolume)
            {
                FadeOutSetVolume(0);
            }
            this->fadeOutLastSetVolume =
                this->fadeOutVolumeMultiplier * 128.0f;
            this->fadeOutElapsedMs++;
        }
        else
        {
            this->fadeOutVolumeMultiplier = 0.0;
            return;
        }
    }

    i32 trackIndex;
    for (trackIndex = 0; trackIndex < this->numTracks; trackIndex++)
    {
        if (this->tracks[trackIndex].trackPlaying)
        {
            bgmFormatIndexLocal05 = TRUE;
            while (this->tracks[trackIndex].trackPlaying)
            {
                if (this->tracks[trackIndex].nextEventTick <=
                    currentPlaybackTick)
                {
                    ProcessMsg(&this->tracks[trackIndex]);
                    currentPlaybackTick =
                        this->elapsedTicksBeforeTempoChange +
                        (this->elapsedMillisecondsAtCurrentTempo *
                         this->ticksPerQuarterNote * 1000 /
                         this->microsecondsPerQuarterNote);
                    continue;
                }
                break;
            }
        }
    }
    this->elapsedMillisecondsAtCurrentTempo++;
    if (!bgmFormatIndexLocal05)
    {
        LoadTracks();
    }
}

// FUNCTION: TH095 0x00423310; TH08 Midi.cpp provides fade-loop ancestry.
void MidiOutput::FadeOutSetVolume(i32 volumeOffset)
{
    struct MidiFadeLocals
    {
        i32 fadeVolumeClamped;
        u32 fadeStatusByte;
        u32 fadeVolumeByte;
        i32 fadeChannelIndex;
        i32 fadeControllerNumber;
    } locals;

#define volumeClamped locals.fadeVolumeClamped
#define statusByte locals.fadeStatusByte
#define volumeByte locals.fadeVolumeByte
#define channelIndex locals.fadeChannelIndex
#define controllerNumber locals.fadeControllerNumber

    if (this->volumeUpdatesSuppressed != 0)
    {
        return;
    }
    controllerNumber = MIDI_CONTROLLER_CHANNEL_VOLUME;
    for (channelIndex = 0;
         channelIndex < ARRAY_SIZE_SIGNED(this->channels);
         channelIndex += 1)
    {
        statusByte = (u8)(channelIndex + MIDI_OPCODE_CONTROL_CHANGE);
        volumeClamped =
            (i32)(this->channels[channelIndex].channelVolume *
                  this->fadeOutVolumeMultiplier) +
            volumeOffset;
        if (volumeClamped < 0)
        {
            volumeClamped = 0;
        }
        else if (volumeClamped > 127)
        {
            volumeClamped = 127;
        }
        volumeByte = (u8)volumeClamped;
        this->outputDevice.SendShortMsg(
            statusByte, controllerNumber, volumeByte);
    }
#undef controllerNumber
#undef channelIndex
#undef volumeByte
#undef statusByte
#undef volumeClamped
    return;
}

// FUNCTION: TH095 0x004233D0; TH08 Midi.cpp is the source-shape oracle.
void DummyMidiTimer::OnTimerElapsed()
{
    QueryPerformanceCounter(&g_DummyMidiTimerPerformanceCounter);
}

// FUNCTION: TH095 0x004233F0; TH08 Midi.cpp is the source-shape oracle.
void MidiTimer::StartTimer()
{
    StartTimerImpl(16, NULL, NULL);
}

// FUNCTION: TH095 0x00423410; TH08 Midi.cpp is the source-shape oracle.
void MidiTimer::StopTimer()
{
    StopTimerImpl();
}

}; // namespace th095
