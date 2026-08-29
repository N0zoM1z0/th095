#include "AnmManager.hpp"

namespace th095
{

class ZunMemory
{
  public:
    // Volatile parameter reads preserve the addressable inline-argument homes
    // observed in the pinned VC7.1 target object.
    void *Alloc(size_t size, const char *debugText = NULL)
    {
        return malloc(*reinterpret_cast<volatile size_t *>(&size));
    }

    void Free(void *ptr)
    {
        free(*reinterpret_cast<void *volatile *>(&ptr));
    }
};

extern ZunMemory g_ZunMemory;

// These backing identifiers reproduce the target's VC7.1 local allocation
// order without relying on the patched TH08 var_order frontend.
#define data restartCommandProcessingLocal05
#define direction averagedPanLocal12
#define i iLocal11
#define vertex commandCursorLocal02
#define radialVelocity soundIndexLocal01
#define angle jLocal00
ZunResult AnmVm::InitializePulsingRadialTrail()
{
    PulsingRadialTrailData *data;
    AnmVertex *vertex;
    f32 angle;
    f32 radialVelocity;
    i32 i;
    Float3 direction;

    if (this->generatedVertices != NULL)
    {
        g_ZunMemory.Free(this->generatedVertices);
    }

    this->generatedVertices = g_ZunMemory.Alloc(sizeof(PulsingRadialTrailData));
    this->positionCallback = UpdatePulsingRadialTrail;
    this->drawCallback = DrawPulsingRadialTrail;

    data = (PulsingRadialTrailData *)this->generatedVertices;
    data->uvVelocity.x = g_Rng.GetRandomF32Signed() * (1.0f / 120.0f);
    data->uvVelocity.y = g_Rng.GetRandomF32Signed() * (1.0f / 120.0f);

    angle = -3.1415927f;
    vertex = data->vertices;
    vertex->position = this->position + this->positionOffset;
    vertex->rhw = 1.0f;
    vertex->uv.x = 0.5f;
    vertex->uv.y = 0.5f;
    vertex++;

    radialVelocity = g_Rng.GetRandomF32Signed() * (1.0f / 15.0f);
    for (i = 1; i < 32; i++)
    {
        if (angle >= 3.1415927f)
            angle -= 6.2831855f;

        vertex->rhw = 1.0f;
        direction.FromAngleMagnitude(angle, 0.5f);
        vertex->uv.x = direction.x + 0.5f;
        vertex->uv.y = direction.y + 0.5f;
        vertex->position.z = 0.0f;

        data->radii[i] = g_Rng.GetRandomF32Signed() * 8.0f + 80.0f;
        data->radialVelocities[i] = radialVelocity;
        radialVelocity += g_Rng.GetRandomF32Signed() * (1.0f / 30.0f);
        if (radialVelocity < -(1.0f / 15.0f))
            radialVelocity = -(1.0f / 15.0f);
        else if (radialVelocity > (1.0f / 15.0f))
            radialVelocity = (1.0f / 15.0f);

        vertex->position.FromAngleMagnitude(angle, data->radii[i]);
        vertex->position += this->position + this->positionOffset;
        vertex++;
        angle += 0.2026834041f;
    }

    return ZUN_SUCCESS;
}
#undef angle
#undef radialVelocity
#undef vertex
#undef i
#undef direction
#undef data

} // namespace th095
