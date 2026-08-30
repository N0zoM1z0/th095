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

// Keep descriptive names in the recovered source while feeding VC7.1 the
// identifier/declaration order that reproduces the target's nine local slots.
#define trailI vertex
#define trailData i
#define trailVertex firstVWrapIndex
#define trailAngleStep firstUWrapIndex
#define trailAngle uWrapIndex
#define trailFirstUWrapIndex vWrapIndex
#define trailFirstVWrapIndex angle
#define trailUWrapIndex data
#define trailVWrapIndex angleStep
ZunResult __fastcall UpdatePulsingRadialTrail(AnmVm *vm)
{
    i32 trailI;
    AnmVertex *trailVertex;
    PulsingRadialTrailData *trailData;
    f32 trailAngleStep;
    f32 trailAngle;
    i32 trailFirstUWrapIndex;
    i32 trailFirstVWrapIndex;
    i32 trailUWrapIndex;
    i32 trailVWrapIndex;

    trailData = (PulsingRadialTrailData *)vm->generatedVertices;
    trailAngleStep = 0.2026834041f;
    trailAngle = -3.1415927f;
    trailVertex = trailData->vertices;

    trailVertex->position = vm->position + vm->positionOffset;
    trailVertex->uv.x += trailData->uvVelocity.x;
    if (trailVertex->uv.x < 0.0f)
    {
        for (trailFirstUWrapIndex = 0; trailFirstUWrapIndex < 33;
             trailFirstUWrapIndex++)
            trailData->vertices[trailFirstUWrapIndex].uv.x += 1.0f;
    }
    trailVertex->uv.y += trailData->uvVelocity.x;
    if (trailVertex->uv.y < 0.0f)
    {
        for (trailFirstVWrapIndex = 0; trailFirstVWrapIndex < 33;
             trailFirstVWrapIndex++)
            trailData->vertices[trailFirstVWrapIndex].uv.y += 1.0f;
    }
    trailVertex->diffuse.color = vm->color1.color;
    trailVertex++;

    for (trailI = 1; trailI < 32; trailI++)
    {
        trailVertex->uv.x += trailData->uvVelocity.x;
        if (trailVertex->uv.x < 0.0f)
        {
            for (trailUWrapIndex = 0; trailUWrapIndex < 33;
                 trailUWrapIndex++)
                trailData->vertices[trailUWrapIndex].uv.x += 1.0f;
        }
        trailVertex->uv.y += trailData->uvVelocity.x;
        if (trailVertex->uv.y < 0.0f)
        {
            for (trailVWrapIndex = 0; trailVWrapIndex < 33;
                 trailVWrapIndex++)
                trailData->vertices[trailVWrapIndex].uv.y += 1.0f;
        }

        trailVertex->diffuse.color = vm->color1.color;
        trailVertex->diffuse.a = 0;
        trailData->radii[trailI] += trailData->radialVelocities[trailI];
        trailVertex->position.FromAngleMagnitude(
            trailAngle, trailData->radii[trailI]);
        trailVertex->position += vm->position + vm->positionOffset;
        trailVertex++;
        trailAngle += trailAngleStep;
    }

    *trailVertex = trailData->vertices[1];
    return ZUN_SUCCESS;
}
#undef trailVWrapIndex
#undef trailUWrapIndex
#undef trailFirstVWrapIndex
#undef trailFirstUWrapIndex
#undef trailAngle
#undef trailAngleStep
#undef trailVertex
#undef trailData
#undef trailI

ZunResult __fastcall DrawPulsingRadialTrail(AnmVm *vm)
{
    PulsingRadialTrailData *data;

    data = (PulsingRadialTrailData *)vm->generatedVertices;
    g_AnmManager->DrawTriangleFan(vm, data->vertices, 33);
    return ZUN_SUCCESS;
}

} // namespace th095
