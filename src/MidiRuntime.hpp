#ifndef TH095_MIDI_RUNTIME_HPP
#define TH095_MIDI_RUNTIME_HPP

#include "ZunResult.hpp"
#include "inttypes.hpp"
#include <mmsystem.h>

namespace th095
{

// Production-only canonical method view for the exact MidiOutput implementation
// in Midi.cpp. Main stores only a MidiOutput pointer, so no object layout is
// duplicated here; the signatures are target/exact-proven at 0x004221B0,
// 0x00422300, and 0x00422600..0x004227B0.
struct MidiOutput
{
    ::ZunResult ReadFileData(i32 slot, const char *path);
    ::ZunResult StopPlayback();
    ::ZunResult ParseFile(i32 index);
    ::ZunResult Play();
    ::ZunResult SetFadeOut(u32 milliseconds);
    ::ZunResult UnprepareHeader(LPMIDIHDR header);
    ~MidiOutput();
};

} // namespace th095

#endif
