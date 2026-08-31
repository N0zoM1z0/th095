#include "ZunMath.hpp"
#include <math.h>

namespace th095
{

// FUNCTION: TH095 0x0041B580; TH08 AddNormalizeAngle is the bounded source oracle.
f32 NormalizeAngle(f32 a)
{
    i32 i;
    i = 0;
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

// FUNCTION: TH095 0x0041B600; TH08 Rotate establishes arithmetic ancestry.
// TH095 computes sine/cosine once each; declaration and assignment order are codegen-visible.
void Rotate(Float3 *outVector, Float3 *point, f32 angle)
{
    f32 cosine;
    f32 sine;
    sine = sin(angle);
    cosine = cos(angle);
    outVector->x = point->x * cosine - point->y * sine;
    outVector->y = point->y * cosine + point->x * sine;
}

} // namespace th095
