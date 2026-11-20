#pragma once

#include "ZunResult.hpp"
#include "inttypes.hpp"
#include <mmsystem.h>

namespace th095
{

// Fieldless ABI adapter for Main and MainExact. Midi.hpp is
// the sole layout/behavior owner, but MainExact.inl defines target-emission
// MidiTimer types that cannot coexist with that complete graph. Keep this
// shared declaration canonical in signatures and deliberately empty in state.
class MidiOutput
{
  public:
    ::ZunResult ReadFileData(i32 slot, const char *path);
    ::ZunResult StopPlayback();
    ::ZunResult ParseFile(i32 index);
    ::ZunResult Play();
    ::ZunResult SetFadeOut(u32 milliseconds);
    ::ZunResult UnprepareHeader(LPMIDIHDR header);
    ~MidiOutput();
};

} // namespace th095
