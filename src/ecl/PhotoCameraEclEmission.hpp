#ifndef TH095_PHOTO_CAMERA_ECL_EMISSION_HPP
#define TH095_PHOTO_CAMERA_ECL_EMISSION_HPP

#include "../ZunMath.hpp"

namespace th095
{
namespace EclRunHigh
{

// Compiler-emission adapter only. All six target calls at 0x0040ABB7,
// 0x0040AC92, 0x0040D58A, 0x0040DB18, 0x0040E1FA, and 0x0040ED4D pass the
// PlayerInf root at 0x004C4E70 to AngleFromPoint @ 0x004303E0. Historical
// EclRun COFF decorates the last four through the PhotoCamera/GetAngle names
// below; the first two retain their separate Player/AngleToPoint decoration.
// Normal source uses PhotoPlayerRuntimeView and never treats this as an owner.
struct PhotoCamera
{
    f32 GetAngle(Float3 *position);
};

extern PhotoCamera *g_Th095PhotoCamera;

} // namespace EclRunHigh
} // namespace th095

#endif
