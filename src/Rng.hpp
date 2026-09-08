#pragma once

#include "diffbuild.hpp"
#include "inttypes.hpp"

namespace th095
{

// Both gameplay RNG instances use the same eight-byte state.  The fields are
// public because target-authored initialization writes the seeds directly;
// keeping one shared declaration prevents class/struct name-decoration splits
// from creating multiple link-time identities for the same target storage.
#if defined(TH095_MATCH_EXACT) && defined(TH095_MATCH_RNG_AS_STRUCT)
struct Rng
#else
class Rng
#endif
{
  public:
    u16 GetRandomU16();
    u32 GetRandomU32();
    f32 GetRandomF32();
    f32 GetRandomF32Signed();

    void ResetGenerationCount();
    void SetSeed(u16 newSeed);
    u16 GetSeed();

    void SaveSeed()
    {
        this->seedBackup = this->seed;
    }

    void RestoreSavedSeed()
    {
        this->seed = this->seedBackup;
    }

    u16 GetRandomU16InRange(u16 range)
    {
        return range != 0 ? GetRandomU16() % range : 0;
    }

    u32 GetRandomU32InRange(u32 range)
    {
        return range != 0 ? GetRandomU32() % range : 0;
    }

    f32 GetRandomF32InRange(f32 range)
    {
        return GetRandomF32() * range;
    }

    f32 GetRandomF32SignedInRange(f32 range)
    {
        return GetRandomF32Signed() * range;
    }

    u16 seed;
    u16 seedBackup;
    u32 generationCount;
};

typedef char RngSizeIs8[(sizeof(Rng) == 8) ? 1 : -1];

DIFFABLE_EXTERN(Rng, g_Rng);
DIFFABLE_EXTERN(Rng, g_Rng2);

} // namespace th095
