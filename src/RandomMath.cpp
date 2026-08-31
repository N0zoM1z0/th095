#include "Global.hpp"
#include "ZunMath.hpp"

#include <limits.h>

namespace th095
{
// FUNCTION: TH095 0x0041B410; TH08 0x0043ECC0 is the source-shape oracle.
u16 Rng::GetRandomU16(void)
{
    u16 temp = (this->seed ^ 0x9630) - 0x6553;
    this->seed = (((temp & 0xc000) >> 14) + temp * 4) & 0xffff;
    this->generationCount++;
    return this->seed;
}

// FUNCTION: TH095 0x0041B470; TH08 0x0043ED20 is the source-shape oracle.
u32 Rng::GetRandomU32(void)
{
    return GetRandomU16() << 16 | GetRandomU16();
}

// FUNCTION: TH095 0x0041B4A0; TH08 0x0043ED50 is the source-shape oracle.
f32 Rng::GetRandomF32(void)
{
    return (f32)GetRandomU32() / (f32)UINT_MAX;
}

// FUNCTION: TH095 0x0041B4D0; TH08 0x0043ED80 is the source-shape oracle.
f32 Rng::GetRandomF32Signed(void)
{
    return (f32)GetRandomU32() / (f32)INT_MAX - 1.0f;
}

// FUNCTION: TH095 0x0041B500; TH08 0x0043EDB0 is the source-shape oracle.
f32 AddNormalizeAngle(f32 a, f32 b)
{
    i32 i;

    i = 0;
    a += b;
    while (a > ZUN_PI)
    {
        a -= ZUN_2PI;
        if (i++ > 32)
            break;
    }
    while (a < -ZUN_PI)
    {
        a += ZUN_2PI;
        if (i++ > 32)
            break;
    }
    return a;
}
} // namespace th095
