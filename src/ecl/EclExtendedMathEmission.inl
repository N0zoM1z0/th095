#ifndef TH095_ECL_EXTENDED_MATH_EMISSION_INL
#define TH095_ECL_EXTENDED_MATH_EMISSION_INL

// Exact/DIFF receiver spelling for Float3::FromAngleMagnitude @ 0x00441DA0.
// This file is included inside th095::EclExtended.  Normal production aliases
// the storage directly to Float3 and must not treat this as another vector ABI.
struct ExtendedVector
{
    f32 x;
    f32 y;
    f32 z;
    void FromAngleMagnitude(f32 angle, f32 magnitude);
};

typedef char ExtendedVectorSizeC[(sizeof(ExtendedVector) == 0x0c) ? 1 : -1];

#endif
