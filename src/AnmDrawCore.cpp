#include "AnmManager.hpp"

namespace th095
{

struct AnmViewportConfigurationView
{
    u8 unknown000[0xcc];
    D3DVIEWPORT8 viewport;
};

struct AnmSupervisorDrawView
{
    u8 unknown000[0x3c4];
    AnmViewportConfigurationView *currentViewport;
};

struct AnmSpriteDimensions
{
    f32 width;
    f32 halfHeight;
    f32 height;
    f32 halfWidth;
};

struct AnmRotatedSpriteLayout
{
    f32 xOffset;
    f32 yOffset;
    f32 x[4];
    f32 y[4];
    f32 spriteHeight;
    f32 spriteWidth;
    f32 cosine;
    f32 rotation;
    f32 sine;
};

// Fully live camera-facing locals in target memory order. Grouping the values
// that remain adjacent in the recovered source lifetime keeps stock VC7.1 from
// hashing each scalar/array into an unrelated home.
struct AnmCameraFacingDeepLocals
{
    f32 vertexX[4];
    f32 vertexY[4];
    Float3 origin;
    f32 cosine;
    Float3 delta;
    Float3 projectedPosition;
    Float3 projectedReference;
    f32 rotation;
};

struct AnmCameraFacingShallowLocals
{
    f32 sine;
    f32 xOffset;
    f32 yOffset;
    f32 spriteHalfHeight;
    f32 spriteHalfWidth;
};

typedef char AnmCameraFacingDeepLocalsSizeIs58[
    (sizeof(AnmCameraFacingDeepLocals) == 0x58) ? 1 : -1];
typedef char AnmCameraFacingShallowLocalsSizeIs14[
    (sizeof(AnmCameraFacingShallowLocals) == 0x14) ? 1 : -1];

struct AnmBackgroundViewportView
{
    Float3 cameraPosition;
    Float3 cameraLookAtOffset;
    Float3 cameraUp;
    Float3 cameraForward;
    Float3 cameraRight;
    Float3 cameraPositionOffset;
    f32 fieldOfView;
    D3DXMATRIX viewMatrix;
    D3DXMATRIX projectionMatrix;
    D3DVIEWPORT8 viewport;
};

struct AnmBackgroundPhotoBlendView
{
    f32 nearDistance;
    f32 farDistance;
    ZunColor color;
};

struct AnmBackgroundStateDrawView
{
    u8 unknown000[0x1fec];
    AnmBackgroundPhotoBlendView photoBlendCurrent;
};

struct AnmPhotoBlendDrawLocals
{
    Float3 cameraDelta;
    ZunColor color;
    f32 distanceRange;
    f32 distance;
};

struct AnmProjectedPhotoBlendDrawLocals
{
    f32 distanceRange;
    Float3 cameraDelta;
    ZunResult result;
    ZunColor color;
    i32 i;
    f32 distance;
};

#ifdef TH095_MATCH_EXACT
extern AnmBackgroundViewportView *g_CurrentBackgroundViewport;
#else
// The target address 0x004C4A34 is g_Supervisor + 0x3c4.  ANM needs only the
// common leading camera/matrix/viewport layout, so production reads the
// supervisor-owned active pointer through this narrow draw view.
static __forceinline AnmBackgroundViewportView *AnmCurrentBackgroundViewport()
{
    return reinterpret_cast<AnmBackgroundViewportView *>(
        g_Supervisor.currentBackgroundViewport);
}
#define g_CurrentBackgroundViewport AnmCurrentBackgroundViewport()
#endif
#ifdef TH095_MATCH_EXACT
extern AnmBackgroundStateDrawView *g_Background;
#else
struct Background;
extern Background *g_Background;
static __forceinline AnmBackgroundStateDrawView *AnmBackgroundStateOwner()
{
    return reinterpret_cast<AnmBackgroundStateDrawView *>(g_Background);
}
#define g_Background AnmBackgroundStateOwner()
#endif
extern Float3 g_BackgroundCameraPosition;

static __forceinline u8 MixAnmColor(u8 first, u8 second)
{
    u32 value = (u32)first * (u32)second >> 7;
    if (value >= 256)
        value = 255;
    return (u8)value;
}

static const f32 g_AnmHalfPixel = 0.5f;

#if !defined(_MSC_VER) || !defined(_M_IX86)
static f32 RoundAnmCoordinateToNearestEven(f32 value)
{
    f32 rounded = (f32)floor(value);
    f32 fraction = value - rounded;
    if (fraction > 0.5f ||
        (fraction == 0.5f && (((i32)rounded & 1) != 0)))
        rounded += 1.0f;
    return rounded;
}
#endif

// FUNCTION: TH095 0x0043EA20.
void AnmManager::SetRenderStateForVm3D(AnmVm *vm)
{
    ZunColor color;

    if (this->currentBlendMode != vm->blendMode)
    {
        this->FlushVertexBuffer();
        this->currentBlendMode = vm->blendMode;

        switch (this->currentBlendMode)
        {
        case 0:
            g_Supervisor.d3dDevice->SetRenderState(
                D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            break;
        case 1:
            g_Supervisor.d3dDevice->SetRenderState(
                D3DRS_DESTBLEND, D3DBLEND_ONE);
            break;
        case 2:
            g_Supervisor.d3dDevice->SetRenderState(
                D3DRS_DESTBLEND, D3DBLEND_ONE);
            break;
        }
    }

    color.color = vm->flag15 ? vm->color2.color : vm->color1.color;
    if (this->useMixColor)
    {
        color.r = MixAnmColor(color.r, this->color.r);
        color.g = MixAnmColor(color.g, this->color.g);
        color.b = MixAnmColor(color.b, this->color.b);
        color.a = MixAnmColor(color.a, this->color.a);
    }

    if (this->currentTextureFactor != color.color)
    {
        this->FlushVertexBuffer();
        this->currentTextureFactor = color.color;
        g_Supervisor.d3dDevice->SetRenderState(
            D3DRS_TEXTUREFACTOR, this->currentTextureFactor);
    }

    this->renderStateChangesThisFrame++;
}

// FUNCTION: TH095 0x0043EC20.
void AnmManager::SetRenderStateForVm(AnmVm *vm)
{
    if (this->currentBlendMode != vm->blendMode)
    {
        this->FlushVertexBuffer();
        this->currentBlendMode = vm->blendMode;

        switch (this->currentBlendMode)
        {
        case 0:
            g_Supervisor.d3dDevice->SetRenderState(
                D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            break;
        case 1:
            g_Supervisor.d3dDevice->SetRenderState(
                D3DRS_DESTBLEND, D3DBLEND_ONE);
            break;
        }
    }

    this->renderStateChangesThisFrame++;
}

// TH08 records the original shallow-to-deep order as triangleY1, triangleY2,
// triangleX2, triangleX1, color. Stock VC7.1 ignores patched var_order, so map
// those five genuine locals to already calibrated identifier buckets. This
// changes only their compiler-selected homes; it adds no padding or dead state.
#define triangleY1 restartCommandProcessingLocal05
#define triangleY2 averagedPanLocal12
#define triangleX2 iLocal11
#define triangleX1 commandCursorLocal02
#pragma var_order(triangleY1, triangleY2, triangleX2, triangleX1, color)
// FUNCTION: TH095 0x0043ECD0.
ZunResult AnmManager::DrawInner(AnmVm *vm, i32 flags)
{
    ZunColor soundIndexLocal01;
    f32 triangleX1;
    f32 triangleX2;
    f32 triangleY1;
    f32 triangleY2;

    g_AnmTexturedVertices[0].x += this->screenShakeOffset.x;
    g_AnmTexturedVertices[0].y += this->screenShakeOffset.y;
    g_AnmTexturedVertices[1].x += this->screenShakeOffset.x;
    g_AnmTexturedVertices[1].y += this->screenShakeOffset.y;
    g_AnmTexturedVertices[2].x += this->screenShakeOffset.x;
    g_AnmTexturedVertices[2].y += this->screenShakeOffset.y;
    g_AnmTexturedVertices[3].x += this->screenShakeOffset.x;
    g_AnmTexturedVertices[3].y += this->screenShakeOffset.y;

    if ((flags & 1) != 0)
    {
#if defined(_MSC_VER) && defined(_M_IX86)
        // Reconstruction decision: this is a deliberately narrow inline-x87
        // exception for DrawInner, approved for the final TH095 ANM residual.
        // The verified target executes FRNDINT at 0043ED77/85/93/A1 and then
        // subtracts the shared 0.5f constant while all four rounded values are
        // still on the x87 stack. Stock VC7.1 build 3077 exposes no C/C++
        // intrinsic that emits FRNDINT; the previously tested floor/nearbyint
        // forms instead add calls and control flow. The adjacent exact TH08
        // reconstruction uses this same four-value inline-asm stack sequence,
        // so this preserves source-family provenance rather than copying target
        // bytes. Do not generalize this exception beyond this function.
        // Non-MSVC/non-x86 builds use the equivalent C++ path below.
        __asm
        {
            fld g_AnmTexturedVertices[0 * TYPE g_AnmTexturedVertices].x
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmTexturedVertices[1 * TYPE g_AnmTexturedVertices].x
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmTexturedVertices[0 * TYPE g_AnmTexturedVertices].y
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmTexturedVertices[2 * TYPE g_AnmTexturedVertices].y
            frndint
            fsub g_AnmHalfPixel
            fst g_AnmTexturedVertices[2 * TYPE g_AnmTexturedVertices].y
            fstp g_AnmTexturedVertices[3 * TYPE g_AnmTexturedVertices].y
            fst g_AnmTexturedVertices[0 * TYPE g_AnmTexturedVertices].y
            fstp g_AnmTexturedVertices[1 * TYPE g_AnmTexturedVertices].y
            fst g_AnmTexturedVertices[1 * TYPE g_AnmTexturedVertices].x
            fstp g_AnmTexturedVertices[3 * TYPE g_AnmTexturedVertices].x
            fst g_AnmTexturedVertices[0 * TYPE g_AnmTexturedVertices].x
            fstp g_AnmTexturedVertices[2 * TYPE g_AnmTexturedVertices].x
        }
#else
        triangleX1 = RoundAnmCoordinateToNearestEven(
                         g_AnmTexturedVertices[0].x) - g_AnmHalfPixel;
        triangleX2 = RoundAnmCoordinateToNearestEven(
                         g_AnmTexturedVertices[1].x) - g_AnmHalfPixel;
        triangleY1 = RoundAnmCoordinateToNearestEven(
                         g_AnmTexturedVertices[0].y) - g_AnmHalfPixel;
        triangleY2 = RoundAnmCoordinateToNearestEven(
                         g_AnmTexturedVertices[2].y) - g_AnmHalfPixel;
        g_AnmTexturedVertices[2].y =
            g_AnmTexturedVertices[3].y = triangleY2;
        g_AnmTexturedVertices[0].y =
            g_AnmTexturedVertices[1].y = triangleY1;
        g_AnmTexturedVertices[1].x =
            g_AnmTexturedVertices[3].x = triangleX2;
        g_AnmTexturedVertices[0].x =
            g_AnmTexturedVertices[2].x = triangleX1;
#endif
    }

    g_AnmTexturedVertices[0].u = g_AnmTexturedVertices[2].u =
        vm->loadedSprite->uvStart.x + vm->uvScrollPos.x;
    g_AnmTexturedVertices[1].u = g_AnmTexturedVertices[3].u =
        vm->loadedSprite->uvEnd.x + vm->uvScrollPos.x;
    g_AnmTexturedVertices[0].v = g_AnmTexturedVertices[1].v =
        vm->loadedSprite->uvStart.y + vm->uvScrollPos.y;
    g_AnmTexturedVertices[2].v = g_AnmTexturedVertices[3].v =
        vm->loadedSprite->uvEnd.y + vm->uvScrollPos.y;

    triangleX1 = g_AnmTexturedVertices[0].x > g_AnmTexturedVertices[1].x
                     ? g_AnmTexturedVertices[0].x
                     : g_AnmTexturedVertices[1].x;
    triangleX1 = g_AnmTexturedVertices[2].x > triangleX1
                     ? g_AnmTexturedVertices[2].x
                     : triangleX1;
    triangleX1 = g_AnmTexturedVertices[3].x > triangleX1
                     ? g_AnmTexturedVertices[3].x
                     : triangleX1;

    triangleY1 = g_AnmTexturedVertices[0].y > g_AnmTexturedVertices[1].y
                     ? g_AnmTexturedVertices[0].y
                     : g_AnmTexturedVertices[1].y;
    triangleY1 = g_AnmTexturedVertices[2].y > triangleY1
                     ? g_AnmTexturedVertices[2].y
                     : triangleY1;
    triangleY1 = g_AnmTexturedVertices[3].y > triangleY1
                     ? g_AnmTexturedVertices[3].y
                     : triangleY1;

    triangleX2 = g_AnmTexturedVertices[0].x < g_AnmTexturedVertices[1].x
                     ? g_AnmTexturedVertices[0].x
                     : g_AnmTexturedVertices[1].x;
    triangleX2 = g_AnmTexturedVertices[2].x < triangleX2
                     ? g_AnmTexturedVertices[2].x
                     : triangleX2;
    triangleX2 = g_AnmTexturedVertices[3].x < triangleX2
                     ? g_AnmTexturedVertices[3].x
                     : triangleX2;

    triangleY2 = g_AnmTexturedVertices[0].y < g_AnmTexturedVertices[1].y
                     ? g_AnmTexturedVertices[0].y
                     : g_AnmTexturedVertices[1].y;
    triangleY2 = g_AnmTexturedVertices[2].y < triangleY2
                     ? g_AnmTexturedVertices[2].y
                     : triangleY2;
    triangleY2 = g_AnmTexturedVertices[3].y < triangleY2
                     ? g_AnmTexturedVertices[3].y
                     : triangleY2;

    if (triangleX1 < reinterpret_cast<AnmSupervisorDrawView *>(&g_Supervisor)
                           ->currentViewport->viewport.X ||
        triangleY1 < reinterpret_cast<AnmSupervisorDrawView *>(&g_Supervisor)
                           ->currentViewport->viewport.Y ||
        triangleX2 > reinterpret_cast<AnmSupervisorDrawView *>(&g_Supervisor)
                             ->currentViewport->viewport.X +
                         reinterpret_cast<AnmSupervisorDrawView *>(&g_Supervisor)
                             ->currentViewport->viewport.Width ||
        triangleY2 > reinterpret_cast<AnmSupervisorDrawView *>(&g_Supervisor)
                             ->currentViewport->viewport.Y +
                         reinterpret_cast<AnmSupervisorDrawView *>(&g_Supervisor)
                             ->currentViewport->viewport.Height)
        return ZUN_SUCCESS;

    if (this->currentTexture != vm->loadedSprite->texture)
    {
        this->currentTexture = vm->loadedSprite->texture;
        this->FlushVertexBuffer();
        g_Supervisor.d3dDevice->SetTexture(0, this->currentTexture);
    }

    if (this->currentVertexShader != 1)
    {
        this->FlushVertexBuffer();
        this->currentVertexShader = 1;
    }

    if ((flags & 2) == 0)
    {
        soundIndexLocal01.color =
            vm->flag15 ? vm->color2.color : vm->color1.color;
        if (this->useMixColor)
        {
            soundIndexLocal01.r =
                MixAnmColor(soundIndexLocal01.r, this->color.r);
            soundIndexLocal01.g =
                MixAnmColor(soundIndexLocal01.g, this->color.g);
            soundIndexLocal01.b =
                MixAnmColor(soundIndexLocal01.b, this->color.b);
            soundIndexLocal01.a =
                MixAnmColor(soundIndexLocal01.a, this->color.a);
        }

        g_AnmTexturedVertices[0].diffuse = soundIndexLocal01.color;
        g_AnmTexturedVertices[1].diffuse = soundIndexLocal01.color;
        g_AnmTexturedVertices[2].diffuse = soundIndexLocal01.color;
        g_AnmTexturedVertices[3].diffuse = soundIndexLocal01.color;
    }

    this->SetRenderStateForVm(vm);
    this->AddSpriteToDrawBuffer(g_AnmTexturedVertices);
    return ZUN_SUCCESS;
}
#undef triangleY1
#undef triangleY2
#undef triangleX2
#undef triangleX1

// FUNCTION: TH095 0x0043F4A0.
ZunResult AnmManager::DrawNoRotation(AnmVm *vm)
{
    AnmSpriteDimensions sprite;
    sprite.width = vm->spriteSize.x * vm->scale.x;
    sprite.height = vm->spriteSize.y * vm->scale.y;
    sprite.halfWidth = sprite.width / 2.0f;
    sprite.halfHeight = sprite.height / 2.0f;

    switch (vm->renderStateA)
    {
    case 1:
        g_AnmTexturedVertices[0].x = g_AnmTexturedVertices[2].x =
            vm->position.x + vm->positionOffset.x;
        g_AnmTexturedVertices[1].x = g_AnmTexturedVertices[3].x =
            vm->position.x + vm->positionOffset.x + sprite.width;
        break;
    case 0:
        g_AnmTexturedVertices[0].x = g_AnmTexturedVertices[2].x =
            (f32)floor(vm->position.x + vm->positionOffset.x -
                       sprite.halfWidth);
        g_AnmTexturedVertices[1].x = g_AnmTexturedVertices[3].x =
            g_AnmTexturedVertices[0].x + sprite.width;
        break;
    case 2:
        g_AnmTexturedVertices[0].x = g_AnmTexturedVertices[2].x =
            vm->position.x + vm->positionOffset.x - sprite.width;
        g_AnmTexturedVertices[1].x = g_AnmTexturedVertices[3].x =
            vm->position.x + vm->positionOffset.x;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        g_AnmTexturedVertices[0].y = g_AnmTexturedVertices[1].y =
            vm->position.y + vm->positionOffset.y;
        g_AnmTexturedVertices[2].y = g_AnmTexturedVertices[3].y =
            vm->position.y + vm->positionOffset.y + sprite.height;
        break;
    case 0:
        g_AnmTexturedVertices[0].y = g_AnmTexturedVertices[1].y =
            (f32)floor(vm->position.y + vm->positionOffset.y -
                       sprite.halfHeight);
        g_AnmTexturedVertices[2].y = g_AnmTexturedVertices[3].y =
            g_AnmTexturedVertices[0].y + sprite.height;
        break;
    case 2:
        g_AnmTexturedVertices[0].y = g_AnmTexturedVertices[1].y =
            vm->position.y + vm->positionOffset.y - sprite.height;
        g_AnmTexturedVertices[2].y = g_AnmTexturedVertices[3].y =
            vm->position.y + vm->positionOffset.y;
        break;
    }

    g_AnmTexturedVertices[0].z = g_AnmTexturedVertices[1].z =
        g_AnmTexturedVertices[2].z = g_AnmTexturedVertices[3].z =
            vm->position.z + vm->positionOffset.z;
    return this->DrawInner(vm, 1);
}

// FUNCTION: TH095 0x0043F760.
ZunResult AnmManager::DrawNoRotationNoRound(AnmVm *vm)
{
    AnmSpriteDimensions sprite;
    sprite.width = vm->spriteSize.x * vm->scale.x;
    sprite.height = vm->spriteSize.y * vm->scale.y;
    sprite.halfWidth = sprite.width / 2.0f;
    sprite.halfHeight = sprite.height / 2.0f;

    switch (vm->renderStateA)
    {
    case 1:
        g_AnmTexturedVertices[0].x = g_AnmTexturedVertices[2].x =
            vm->position.x + vm->positionOffset.x;
        g_AnmTexturedVertices[1].x = g_AnmTexturedVertices[3].x =
            vm->position.x + vm->positionOffset.x + sprite.width;
        break;
    case 0:
        g_AnmTexturedVertices[0].x = g_AnmTexturedVertices[2].x =
            vm->position.x + vm->positionOffset.x - sprite.halfWidth;
        g_AnmTexturedVertices[1].x = g_AnmTexturedVertices[3].x =
            g_AnmTexturedVertices[0].x + sprite.width;
        break;
    case 2:
        g_AnmTexturedVertices[0].x = g_AnmTexturedVertices[2].x =
            vm->position.x + vm->positionOffset.x - sprite.width;
        g_AnmTexturedVertices[1].x = g_AnmTexturedVertices[3].x =
            vm->position.x + vm->positionOffset.x;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        g_AnmTexturedVertices[0].y = g_AnmTexturedVertices[1].y =
            vm->position.y + vm->positionOffset.y;
        g_AnmTexturedVertices[2].y = g_AnmTexturedVertices[3].y =
            vm->position.y + vm->positionOffset.y + sprite.height;
        break;
    case 0:
        g_AnmTexturedVertices[0].y = g_AnmTexturedVertices[1].y =
            vm->position.y + vm->positionOffset.y - sprite.halfHeight;
        g_AnmTexturedVertices[2].y = g_AnmTexturedVertices[3].y =
            g_AnmTexturedVertices[0].y + sprite.height;
        break;
    case 2:
        g_AnmTexturedVertices[0].y = g_AnmTexturedVertices[1].y =
            vm->position.y + vm->positionOffset.y - sprite.height;
        g_AnmTexturedVertices[2].y = g_AnmTexturedVertices[3].y =
            vm->position.y + vm->positionOffset.y;
        break;
    }

    g_AnmTexturedVertices[0].z = g_AnmTexturedVertices[1].z =
        g_AnmTexturedVertices[2].z = g_AnmTexturedVertices[3].z =
            vm->position.z + vm->positionOffset.z;
    return this->DrawInner(vm, 0);
}

// FUNCTION: TH095 0x0043FA00.
void AnmManager::TranslateRotation(VertexTex1DiffuseXyzrhw *vertex, f32 x,
                                   f32 y, f32 sine, f32 cosine, f32 xOffset,
                                   f32 yOffset)
{
    vertex->x = x * cosine - y * sine + xOffset;
    vertex->y = x * sine + y * cosine + yOffset;
}

// FUNCTION: TH095 0x0043FA40.
ZunResult AnmManager::Draw2D(AnmVm *vm)
{
    AnmRotatedSpriteLayout sprite;

    if (vm->rotation.z == 0.0f)
        return this->DrawNoRotationNoRound(vm);

    sprite.rotation = vm->rotation.z;
#if defined(_MSC_VER) && defined(_M_IX86)
    // Reconstruction decision: use inline x87 only at this target-proven
    // Draw2D site. TH095 has one FSINCOS at 0043FA7B, with cosine popped before
    // sine; stock VC7.1 build 3077 compiles every tested paired sin/cos C++ form
    // as separate FSIN and FCOS and rejects sincos/fsincos intrinsics. The
    // adjacent exact TH08 reconstruction's sincos macro expands to this
    // FLD/FSINCOS/FSTP source family. This user-approved exception is required
    // for instruction identity and is not permission to use assembly
    // elsewhere. The fallback remains semantic.
    __asm
    {
        fld sprite.rotation
        fsincos
        fstp sprite.cosine
        fstp sprite.sine
    }
#else
    sprite.cosine = (f32)cos(sprite.rotation);
    sprite.sine = (f32)sin(sprite.rotation);
#endif
    sprite.xOffset = vm->position.x + vm->positionOffset.x;
    sprite.yOffset = vm->position.y + vm->positionOffset.y;
    sprite.spriteWidth = vm->spriteSize.x * vm->scale.x;
    sprite.spriteHeight = vm->spriteSize.y * vm->scale.y;

    switch (vm->renderStateA)
    {
    case 1:
        sprite.x[0] = sprite.x[2] = 0.0f;
        sprite.x[1] = sprite.x[3] = sprite.spriteWidth;
        break;
    case 0:
        sprite.x[0] = sprite.x[2] = -sprite.spriteWidth * 0.5f;
        sprite.x[1] = sprite.x[3] = sprite.spriteWidth * 0.5f;
        break;
    case 2:
        sprite.x[0] = sprite.x[2] = -sprite.spriteWidth;
        sprite.x[1] = sprite.x[3] = 0.0f;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        sprite.y[0] = sprite.y[1] = 0.0f;
        sprite.y[2] = sprite.y[3] = sprite.spriteHeight;
        break;
    case 0:
        sprite.y[0] = sprite.y[1] = -sprite.spriteHeight * 0.5f;
        sprite.y[2] = sprite.y[3] = sprite.spriteHeight * 0.5f;
        break;
    case 2:
        sprite.y[0] = sprite.y[1] = -sprite.spriteHeight;
        sprite.y[2] = sprite.y[3] = 0.0f;
        break;
    }

    for (i32 i = 0; i < 4; i++)
    {
        this->TranslateRotation(&g_AnmTexturedVertices[i], sprite.x[i],
                                sprite.y[i], sprite.sine, sprite.cosine,
                                sprite.xOffset, sprite.yOffset);
    }

    g_AnmTexturedVertices[0].z = g_AnmTexturedVertices[1].z =
        g_AnmTexturedVertices[2].z = g_AnmTexturedVertices[3].z =
            vm->position.z;
    return this->DrawInner(vm, 0);
}

// TH08's source orders the camera-facing locals shallow-to-deep as half width,
// half height, Y/X offsets, sine, matrix, rotation, projected reference,
// projected position, delta, cosine, and origin. TH095 then owns two real
// four-float alignment arrays and the loop index. The target also reuses the
// X-offset home for the now-dead projected scale. Stock VC7.1 ignores TH08's
// patched var_order pragma, so keep the two naturally contiguous lifetime
// groups intact and rank only those real groups, the matrix, and the loop index
// through established identifier buckets. No padding, inert field, or fake
// lifetime is introduced; every aggregate field is used by the algorithm.
#define cameraShallow restartCommandProcessingLocal05
#define worldMatrix averagedPanLocal12
#define cameraDeep iLocal11
#define i commandCursorLocal02
#pragma var_order(cameraShallow, worldMatrix, cameraDeep, i, this)
// FUNCTION: TH095 0x0043FC60.
ZunResult AnmManager::ProjectCameraFacingQuad(AnmVm *vm)
{
    AnmCameraFacingShallowLocals cameraShallow;
    D3DXMATRIX worldMatrix;
    AnmCameraFacingDeepLocals cameraDeep;
    i32 i;

    cameraDeep.rotation = vm->rotation.z;

#if defined(_MSC_VER) && defined(_M_IX86)
    // Reconstruction decision: this first ProjectCameraFacingQuad sin/cos pair
    // must be inline x87. The verified target has FLD/FSINCOS/two FSTP at
    // 0043FC78..0043FC85; the stock-3077 frontend has no intrinsic capable of
    // producing it, while the adjacent exact TH08 reconstruction supplies the
    // same ZUN sincos macro shape. The exception is limited to this function
    // and this target-observed site.
    __asm
    {
        fld cameraDeep.rotation
        fsincos
        fstp cameraDeep.cosine
        fstp cameraShallow.sine
    }
#else
    cameraDeep.cosine = (f32)cos(cameraDeep.rotation);
    cameraShallow.sine = (f32)sin(cameraDeep.rotation);
#endif

    cameraDeep.origin.x = 0.0f;
    cameraDeep.origin.y = 0.0f;
    cameraDeep.origin.z = 0.0f;

    D3DXMatrixIdentity(&worldMatrix);
    worldMatrix._41 = vm->position.x + vm->positionOffset.x;
    worldMatrix._42 = vm->position.y + vm->positionOffset.y;
    worldMatrix._43 = vm->position.z + vm->positionOffset.z;

    D3DXVec3Project(
        reinterpret_cast<D3DXVECTOR3 *>(&cameraDeep.projectedPosition),
        reinterpret_cast<D3DXVECTOR3 *>(&cameraDeep.origin),
        &g_CurrentBackgroundViewport->viewport,
        &g_CurrentBackgroundViewport->projectionMatrix,
        &g_CurrentBackgroundViewport->viewMatrix, &worldMatrix);
    if (cameraDeep.projectedPosition.z < 0.0f ||
        cameraDeep.projectedPosition.z > 1.0f)
        return ZUN_ERROR;

    D3DXVec3Project(
        reinterpret_cast<D3DXVECTOR3 *>(&cameraDeep.projectedReference),
        reinterpret_cast<D3DXVECTOR3 *>(
            &g_CurrentBackgroundViewport->cameraRight),
        &g_CurrentBackgroundViewport->viewport,
        &g_CurrentBackgroundViewport->projectionMatrix,
        &g_CurrentBackgroundViewport->viewMatrix, &worldMatrix);

    cameraDeep.delta =
        cameraDeep.projectedReference - cameraDeep.projectedPosition;
    cameraShallow.xOffset =
        D3DXVec3Length(
            reinterpret_cast<D3DXVECTOR3 *>(&cameraDeep.delta)) * 0.5f;
    cameraShallow.spriteHalfWidth =
        cameraShallow.xOffset * vm->spriteSize.x * vm->scale.x;
    cameraShallow.spriteHalfHeight =
        cameraShallow.xOffset * vm->spriteSize.y * vm->scale.y;
    cameraShallow.xOffset = cameraDeep.projectedPosition.x;
    cameraShallow.yOffset = cameraDeep.projectedPosition.y;

#if defined(_MSC_VER) && defined(_M_IX86)
    // Reconstruction decision: retain the target's second, intentionally
    // repeated x87 evaluation at 0043FEB0..0043FEBD. Reusing the earlier values
    // would remove target instructions; ordinary C++ again becomes separate
    // FSIN/FCOS. The exact TH08 reconstruction proves the
    // FLD/FSINCOS/FSTP source idiom, and the user approved it only for the
    // three final ANM functions. The portable branch below keeps the same
    // observable recomputation without inline assembly.
    __asm
    {
        fld cameraDeep.rotation
        fsincos
        fstp cameraDeep.cosine
        fstp cameraShallow.sine
    }
#else
    cameraDeep.cosine = (f32)cos(cameraDeep.rotation);
    cameraShallow.sine = (f32)sin(cameraDeep.rotation);
#endif

    switch (vm->renderStateA)
    {
    case 1:
        cameraDeep.vertexX[0] = cameraDeep.vertexX[2] = 0.0f;
        cameraDeep.vertexX[1] =
            cameraDeep.vertexX[3] = cameraShallow.spriteHalfWidth;
        break;
    case 0:
        cameraDeep.vertexX[0] = cameraDeep.vertexX[2] =
            -cameraShallow.spriteHalfWidth * 0.5f;
        cameraDeep.vertexX[1] = cameraDeep.vertexX[3] =
            cameraShallow.spriteHalfWidth * 0.5f;
        break;
    case 2:
        cameraDeep.vertexX[0] = cameraDeep.vertexX[2] =
            -cameraShallow.spriteHalfWidth;
        cameraDeep.vertexX[1] = cameraDeep.vertexX[3] = 0.0f;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        cameraDeep.vertexY[0] = cameraDeep.vertexY[1] = 0.0f;
        cameraDeep.vertexY[2] =
            cameraDeep.vertexY[3] = cameraShallow.spriteHalfHeight;
        break;
    case 0:
        cameraDeep.vertexY[0] = cameraDeep.vertexY[1] =
            -cameraShallow.spriteHalfHeight * 0.5f;
        cameraDeep.vertexY[2] = cameraDeep.vertexY[3] =
            cameraShallow.spriteHalfHeight * 0.5f;
        break;
    case 2:
        cameraDeep.vertexY[0] = cameraDeep.vertexY[1] =
            -cameraShallow.spriteHalfHeight;
        cameraDeep.vertexY[2] = cameraDeep.vertexY[3] = 0.0f;
        break;
    }

    for (i = 0; i < 4; i++)
    {
        this->TranslateRotation(
            &g_AnmTexturedVertices[i], cameraDeep.vertexX[i],
            cameraDeep.vertexY[i], cameraShallow.sine, cameraDeep.cosine,
            cameraShallow.xOffset, cameraShallow.yOffset);
    }

    g_AnmTexturedVertices[0].z = g_AnmTexturedVertices[1].z =
        g_AnmTexturedVertices[2].z = g_AnmTexturedVertices[3].z =
            vm->position.z;
    return ZUN_SUCCESS;
}
#undef cameraShallow
#undef worldMatrix
#undef cameraDeep
#undef i

// FUNCTION: TH095 0x004400F0.
ZunResult AnmManager::DrawCameraFacingQuad(AnmVm *vm)
{
    if (this->ProjectCameraFacingQuad(vm) != ZUN_SUCCESS)
        return ZUN_ERROR;
    return this->DrawInner(vm, 0);
}

// FUNCTION: TH095 0x00440120.
ZunResult AnmManager::DrawMode6(AnmVm *vm)
{
    AnmPhotoBlendDrawLocals draw;

    if (this->ProjectCameraFacingQuad(vm) != ZUN_SUCCESS)
        return ZUN_ERROR;

    draw.distanceRange =
        g_Background->photoBlendCurrent.nearDistance -
        g_Background->photoBlendCurrent.farDistance;
    draw.color.color = vm->flag15 ? vm->color2.color : vm->color1.color;
    draw.cameraDelta =
        vm->position + vm->positionOffset - g_BackgroundCameraPosition;
    draw.distance = D3DXVec3Length(
        reinterpret_cast<D3DXVECTOR3 *>(&draw.cameraDelta));

    if (this->useMixColor)
    {
        draw.color.r = MixAnmColor(draw.color.r, this->color.r);
        draw.color.g = MixAnmColor(draw.color.g, this->color.g);
        draw.color.b = MixAnmColor(draw.color.b, this->color.b);
        draw.color.a = MixAnmColor(draw.color.a, this->color.a);
    }

    if (g_Background->photoBlendCurrent.nearDistance < draw.distance)
    {
        draw.distance =
            (g_Background->photoBlendCurrent.nearDistance - draw.distance) /
            draw.distanceRange;
        if (draw.distance >= 1.0f)
            return ZUN_ERROR;

        reinterpret_cast<ZunColor *>(
            &g_AnmTexturedVertices[0].diffuse)->b =
            draw.color.b -
            (u8)((draw.color.b -
                  g_Background->photoBlendCurrent.color.b) * draw.distance);
        reinterpret_cast<ZunColor *>(
            &g_AnmTexturedVertices[0].diffuse)->g =
            draw.color.g -
            (u8)((draw.color.g -
                  g_Background->photoBlendCurrent.color.g) * draw.distance);
        reinterpret_cast<ZunColor *>(
            &g_AnmTexturedVertices[0].diffuse)->r =
            draw.color.r -
            (u8)((draw.color.r -
                  g_Background->photoBlendCurrent.color.r) * draw.distance);
        reinterpret_cast<ZunColor *>(
            &g_AnmTexturedVertices[0].diffuse)->a =
            (u8)(draw.color.a * (1.0f - draw.distance));
    }
    else
    {
        g_AnmTexturedVertices[0].diffuse = draw.color.color;
    }

    g_AnmTexturedVertices[1].diffuse =
        g_AnmTexturedVertices[0].diffuse;
    g_AnmTexturedVertices[2].diffuse =
        g_AnmTexturedVertices[0].diffuse;
    g_AnmTexturedVertices[3].diffuse =
        g_AnmTexturedVertices[0].diffuse;
    return this->DrawInner(vm, 2);
}

// FUNCTION: TH095 0x00440440.
ZunResult AnmManager::Project3DQuad(AnmVm *vm)
{
    // Stock VC7.1 allocates these compiler-facing identifiers in the inverse
    // order used by the original patched var_order frontend. Both matrices
    // remain live; the aliases only restore the target's stack-slot order.
    D3DXMATRIX rotationMatrix;
    D3DXMATRIX worldTransformMatrix;
#define projectedRotationMatrix worldTransformMatrix
#define projectedWorldTransformMatrix rotationMatrix

    if (!vm->unknownFlag14 && (vm->updateScale || vm->updateRotation))
    {
        vm->matrix2 = vm->matrix1;
        vm->matrix2._11 *= vm->scale.x;
        vm->matrix2._22 *= vm->scale.y;
        vm->updateScale = false;

        if (vm->rotation.x != 0.0)
        {
            D3DXMatrixRotationX(&projectedRotationMatrix, vm->rotation.x);
            D3DXMatrixMultiply(
                &vm->matrix2, &vm->matrix2, &projectedRotationMatrix);
        }
        if (vm->rotation.y != 0.0)
        {
            D3DXMatrixRotationY(&projectedRotationMatrix, vm->rotation.y);
            D3DXMatrixMultiply(
                &vm->matrix2, &vm->matrix2, &projectedRotationMatrix);
        }
        if (vm->rotation.z != 0.0)
        {
            D3DXMatrixRotationZ(&projectedRotationMatrix, vm->rotation.z);
            D3DXMatrixMultiply(
                &vm->matrix2, &vm->matrix2, &projectedRotationMatrix);
        }
        vm->updateRotation = false;
    }

    projectedWorldTransformMatrix = vm->matrix2;
    projectedWorldTransformMatrix._41 +=
        vm->positionOffset.x + vm->position.x;
    projectedWorldTransformMatrix._42 +=
        vm->positionOffset.y + vm->position.y;
    projectedWorldTransformMatrix._43 = vm->position.z;

    Float3 vertices[4];

    switch (vm->renderStateA)
    {
    case 1:
        vertices[0].x = vertices[2].x = 0.0f;
        vertices[1].x = vertices[3].x = 256.0f;
        break;
    case 0:
        vertices[0].x = vertices[2].x = -128.0f;
        vertices[1].x = vertices[3].x = 128.0f;
        break;
    case 2:
        vertices[0].x = vertices[2].x = -256.0f;
        vertices[1].x = vertices[3].x = 0.0f;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        vertices[0].y = vertices[1].y = 0.0f;
        vertices[2].y = vertices[3].y = 256.0f;
        break;
    case 0:
        vertices[0].y = vertices[1].y = -128.0f;
        vertices[2].y = vertices[3].y = 128.0f;
        break;
    case 2:
        vertices[0].y = vertices[1].y = -256.0f;
        vertices[2].y = vertices[3].y = 0.0f;
        break;
    }

    vertices[0].z = vertices[1].z =
        vertices[2].z = vertices[3].z = 0.0f;

    D3DXVec3Project(
        reinterpret_cast<D3DXVECTOR3 *>(&g_AnmTexturedVertices[0]),
        reinterpret_cast<D3DXVECTOR3 *>(&vertices[0]),
        &g_CurrentBackgroundViewport->viewport,
        &g_CurrentBackgroundViewport->projectionMatrix,
        &g_CurrentBackgroundViewport->viewMatrix,
        &projectedWorldTransformMatrix);
    D3DXVec3Project(
        reinterpret_cast<D3DXVECTOR3 *>(&g_AnmTexturedVertices[1]),
        reinterpret_cast<D3DXVECTOR3 *>(&vertices[1]),
        &g_CurrentBackgroundViewport->viewport,
        &g_CurrentBackgroundViewport->projectionMatrix,
        &g_CurrentBackgroundViewport->viewMatrix,
        &projectedWorldTransformMatrix);
    D3DXVec3Project(
        reinterpret_cast<D3DXVECTOR3 *>(&g_AnmTexturedVertices[2]),
        reinterpret_cast<D3DXVECTOR3 *>(&vertices[2]),
        &g_CurrentBackgroundViewport->viewport,
        &g_CurrentBackgroundViewport->projectionMatrix,
        &g_CurrentBackgroundViewport->viewMatrix,
        &projectedWorldTransformMatrix);
    D3DXVec3Project(
        reinterpret_cast<D3DXVECTOR3 *>(&g_AnmTexturedVertices[3]),
        reinterpret_cast<D3DXVECTOR3 *>(&vertices[3]),
        &g_CurrentBackgroundViewport->viewport,
        &g_CurrentBackgroundViewport->projectionMatrix,
        &g_CurrentBackgroundViewport->viewMatrix,
        &projectedWorldTransformMatrix);

    this->cachedWorldMatrix = projectedWorldTransformMatrix;
#undef projectedWorldTransformMatrix
#undef projectedRotationMatrix
    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x004408F0.
ZunResult AnmManager::DrawProjected3DQuad(AnmVm *vm)
{
    ZunResult result;

    this->Project3DQuad(vm);
    result = this->DrawInner(vm, 0);
    g_AnmTexturedVertices[0].w = g_AnmTexturedVertices[1].w =
        g_AnmTexturedVertices[2].w = g_AnmTexturedVertices[3].w = 1.0f;
    return result;
}

// FUNCTION: TH095 0x00440950.
ZunResult AnmManager::DrawMode7(AnmVm *vm)
{
    this->Project3DQuad(vm);

    D3DXVECTOR4 transformedVertices[4];
    AnmProjectedPhotoBlendDrawLocals draw;

    draw.distanceRange =
        g_Background->photoBlendCurrent.nearDistance -
        g_Background->photoBlendCurrent.farDistance;
    draw.color.color = vm->flag15 ? vm->color2.color : vm->color1.color;

    for (draw.i = 0; draw.i < 4; draw.i++)
    {
        D3DXVec4Transform(
            &transformedVertices[draw.i],
            reinterpret_cast<D3DXVECTOR4 *>(
                &this->untexturedVertices[draw.i]),
            &this->cachedWorldMatrix);
        draw.cameraDelta.x =
            transformedVertices[draw.i].x - g_BackgroundCameraPosition.x;
        draw.cameraDelta.y =
            transformedVertices[draw.i].y - g_BackgroundCameraPosition.y;
        draw.cameraDelta.z =
            transformedVertices[draw.i].z - g_BackgroundCameraPosition.z;
        draw.distance = D3DXVec3Length(
            reinterpret_cast<D3DXVECTOR3 *>(&draw.cameraDelta));

        if (g_Background->photoBlendCurrent.nearDistance < draw.distance)
        {
            draw.distance =
                (g_Background->photoBlendCurrent.nearDistance -
                 draw.distance) / draw.distanceRange;
            if (draw.distance >= 1.0f)
            {
                g_AnmTexturedVertices[draw.i].diffuse =
                    g_Background->photoBlendCurrent.color.color;
                reinterpret_cast<ZunColor *>(
                    &g_AnmTexturedVertices[draw.i].diffuse)->a = draw.color.a;
            }
            else
            {
                reinterpret_cast<ZunColor *>(
                    &g_AnmTexturedVertices[draw.i].diffuse)->b =
                    draw.color.b -
                    (u8)((draw.color.b -
                          g_Background->photoBlendCurrent.color.b) *
                         draw.distance);
                reinterpret_cast<ZunColor *>(
                    &g_AnmTexturedVertices[draw.i].diffuse)->g =
                    draw.color.g -
                    (u8)((draw.color.g -
                          g_Background->photoBlendCurrent.color.g) *
                         draw.distance);
                reinterpret_cast<ZunColor *>(
                    &g_AnmTexturedVertices[draw.i].diffuse)->r =
                    draw.color.r -
                    (u8)((draw.color.r -
                          g_Background->photoBlendCurrent.color.r) *
                         draw.distance);
                reinterpret_cast<ZunColor *>(
                    &g_AnmTexturedVertices[draw.i].diffuse)->a = draw.color.a;
            }
        }
        else
        {
            g_AnmTexturedVertices[draw.i].diffuse = draw.color.color;
        }
    }

    draw.result = this->DrawInner(vm, 2);
    g_AnmTexturedVertices[0].w = g_AnmTexturedVertices[1].w =
        g_AnmTexturedVertices[2].w = g_AnmTexturedVertices[3].w = 1.0f;
    return draw.result;
}

// FUNCTION: TH095 0x00440C10.
// Stock VC7.1 allocates locals through identifier hash chains. Reuse the
// target-proven backing buckets that reproduce TH08's documented patched
// var_order without depending on that non-stock frontend.
#define textureMatrix restartCommandProcessingLocal05
#define rotationMatrix averagedPanLocal12
#define worldTransformMatrix iLocal11
#pragma var_order(textureMatrix, rotationMatrix, worldTransformMatrix, this)
ZunResult AnmManager::Draw3D(AnmVm *vm)
{
    D3DMATRIX textureMatrix;
    D3DXMATRIX rotationMatrix;
    D3DXMATRIX worldTransformMatrix;

    if (!vm->visible)
        return ZUN_ERROR;
    if (!vm->unknownFlag1)
        return ZUN_ERROR;
    if (vm->color1.a == 0)
        return ZUN_ERROR;

    if (this->spritesToDraw != 0)
        this->FlushVertexBuffer();

    if (!vm->unknownFlag14 && (vm->updateScale || vm->updateRotation))
    {
        vm->matrix2 = vm->matrix1;
        vm->matrix2._11 *= vm->scale.x;
        vm->matrix2._22 *= vm->scale.y;
        vm->updateScale = false;

        if (vm->rotation.x != 0.0)
        {
            D3DXMatrixRotationX(&rotationMatrix, vm->rotation.x);
            D3DXMatrixMultiply(&vm->matrix2, &vm->matrix2, &rotationMatrix);
        }
        if (vm->rotation.y != 0.0)
        {
            D3DXMatrixRotationY(&rotationMatrix, vm->rotation.y);
            D3DXMatrixMultiply(&vm->matrix2, &vm->matrix2, &rotationMatrix);
        }
        if (vm->rotation.z != 0.0)
        {
            D3DXMatrixRotationZ(&rotationMatrix, vm->rotation.z);
            D3DXMatrixMultiply(&vm->matrix2, &vm->matrix2, &rotationMatrix);
        }
        vm->updateRotation = false;
    }

    worldTransformMatrix = vm->matrix2;
    switch (vm->renderStateA)
    {
    case 1:
        worldTransformMatrix._41 =
            vm->position.x + vm->positionOffset.x -
            (f32)fabs(vm->spriteSize.x * vm->scale.x / 2.0f);
        break;
    case 0:
        worldTransformMatrix._41 = vm->position.x + vm->positionOffset.x;
        break;
    case 2:
        worldTransformMatrix._41 =
            vm->position.x + vm->positionOffset.x +
            (f32)fabs(vm->spriteSize.x * vm->scale.x / 2.0f);
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        worldTransformMatrix._42 =
            vm->position.y + vm->positionOffset.y -
            (f32)fabs(vm->spriteSize.y * vm->scale.y / 2.0f);
        break;
    case 0:
        worldTransformMatrix._42 = vm->position.y + vm->positionOffset.y;
        break;
    case 2:
        worldTransformMatrix._42 =
            vm->position.y + vm->positionOffset.y +
            (f32)fabs(vm->spriteSize.y * vm->scale.y / 2.0f);
        break;
    }

    worldTransformMatrix._43 = vm->position.z + vm->positionOffset.z;
    this->SetRenderStateForVm3D(vm);
    worldTransformMatrix._43 = vm->position.z + vm->positionOffset.z;
    g_Supervisor.d3dDevice->SetTransform(D3DTS_WORLD, &worldTransformMatrix);

    if (this->currentSprite != vm->loadedSprite ||
        vm->uvScrollPos.x != 0.0f || vm->uvScrollPos.x != 0.0f)
    {
        this->currentSprite = vm->loadedSprite;
        textureMatrix = vm->matrix3;
        textureMatrix._31 = vm->loadedSprite->uvStart.x + vm->uvScrollPos.x;
        textureMatrix._32 = vm->loadedSprite->uvStart.y + vm->uvScrollPos.y;
        g_Supervisor.d3dDevice->SetTransform(D3DTS_TEXTURE0, &textureMatrix);
    }

    if (this->currentTexture != vm->loadedSprite->texture)
    {
        this->currentTexture = vm->loadedSprite->texture;
        g_Supervisor.d3dDevice->SetTexture(0, this->currentTexture);
    }

    if (this->currentVertexShader != 2)
    {
        g_Supervisor.d3dDevice->SetVertexShader(D3DFVF_XYZ | D3DFVF_TEX1);
        g_Supervisor.d3dDevice->SetStreamSource(
            0, this->quadVertexBuffer, sizeof(VertexDiffuseXyzrhw));
        g_Supervisor.d3dDevice->SetTextureStageState(
            0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
        g_Supervisor.d3dDevice->SetTextureStageState(
            0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
        this->currentVertexShader = 2;
    }

    g_Supervisor.d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    return ZUN_SUCCESS;
}
#undef textureMatrix
#undef rotationMatrix
#undef worldTransformMatrix

// FUNCTION: TH095 0x004411D0.
#define stripY restartCommandProcessingLocal05
#define stripI averagedPanLocal12
#define stripVertex iLocal11
#define stripX commandCursorLocal02
#define stripCurrentX soundIndexLocal01
#define stripStep jLocal00
#define stripXSpan preloadBufferLocal03
#pragma var_order(stripY, stripI, stripVertex, stripX, stripCurrentX, stripStep, stripXSpan)
ZunResult AnmManager::InitializeHorizontalTextureStrip(
    AnmVm *vm, AnmVertex *vertices, i32 vertexCount)
{
    f32 stripY;
    i32 stripI;
    AnmVertex *stripVertex;
    f32 stripX;
    f32 stripCurrentX;
    f32 stripStep;
    f32 stripXSpan;

    if (vertexCount < 3)
        return ZUN_ERROR;

    stripX = vm->loadedSprite->uvEnd.x + vm->uvScrollPos.x;
    stripXSpan = vm->loadedSprite->uvEnd.x - vm->loadedSprite->uvStart.x;
    stripY = vm->loadedSprite->uvStart.y + vm->uvScrollPos.y;
    stripVertex = vertices;
    stripStep = stripXSpan / ((vertexCount + 1) / 2 - 1);
    stripI = 0;
    stripCurrentX = stripX;
    for (; stripI < vertexCount; stripI += 2, stripVertex += 2,
           stripCurrentX -= stripStep)
    {
        stripVertex->uv.x = stripCurrentX;
        stripVertex->uv.y = stripY;
        stripVertex->diffuse.color = vm->color1.color;
        stripVertex->rhw = 1.0f;
    }

    stripY = vm->loadedSprite->uvEnd.y + vm->uvScrollPos.y;
    stripVertex = vertices + 1;
    stripI = 1;
    stripCurrentX = stripX;
    for (; stripI < vertexCount; stripI += 2, stripVertex += 2,
           stripCurrentX -= stripStep)
    {
        stripVertex->uv.x = stripCurrentX;
        stripVertex->uv.y = stripY;
        stripVertex->diffuse.color = vm->color1.color;
        stripVertex->rhw = 1.0f;
    }
    return ZUN_SUCCESS;
}
#undef stripY
#undef stripI
#undef stripVertex
#undef stripX
#undef stripCurrentX
#undef stripStep
#undef stripXSpan

// FUNCTION: TH095 0x00441330.
ZunResult AnmManager::DrawVertices(
    AnmVm *vm, AnmVertex *vertices, i32 vertexCount)
{
    if (!vm->visible)
        return ZUN_ERROR;
    if (!vm->unknownFlag1)
        return ZUN_ERROR;
    if (vm->color1.a == 0)
        return ZUN_ERROR;

    if (this->spritesToDraw != 0)
        this->FlushVertexBuffer();

    if (this->currentTexture != vm->loadedSprite->texture)
    {
        this->currentTexture = vm->loadedSprite->texture;
        g_Supervisor.d3dDevice->SetTexture(0, this->currentTexture);
    }

    if (this->currentVertexShader != 3)
    {
        g_Supervisor.d3dDevice->SetVertexShader(
            D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
        this->currentVertexShader = 3;
    }

    this->SetRenderStateForVm(vm);
    g_Supervisor.d3dDevice->SetTextureStageState(
        0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    g_Supervisor.d3dDevice->SetTextureStageState(
        0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_Supervisor.d3dDevice->DrawPrimitiveUP(
        D3DPT_TRIANGLESTRIP, vertexCount - 2, vertices, sizeof(AnmVertex));
    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00441480.
ZunResult AnmManager::DrawTriangleFan(
    AnmVm *vm, AnmVertex *vertices, i32 vertexCount)
{
    if (this->spritesToDraw != 0)
        this->FlushVertexBuffer();

    if (this->currentVertexShader != 3)
    {
        g_Supervisor.d3dDevice->SetVertexShader(
            D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
        this->currentVertexShader = 3;
    }

    this->SetRenderStateForVm(vm);

    if (this->currentTexture != vm->loadedSprite->texture)
    {
        this->currentTexture = vm->loadedSprite->texture;
        g_Supervisor.d3dDevice->SetTexture(0, this->currentTexture);
    }

    g_Supervisor.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    g_Supervisor.d3dDevice->SetTextureStageState(
        0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    g_Supervisor.d3dDevice->SetTextureStageState(
        0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_Supervisor.d3dDevice->DrawPrimitiveUP(
        D3DPT_TRIANGLEFAN, vertexCount - 2, vertices, sizeof(AnmVertex));
    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x0043F3C0.
ZunResult AnmManager::AddSpriteToDrawBuffer(
    VertexTex1DiffuseXyzrhw *vertices)
{
    this->vertexBufferEndPtr[0] = vertices[0];
    this->vertexBufferEndPtr[1] = vertices[1];
    this->vertexBufferEndPtr[2] = vertices[2];
    this->vertexBufferEndPtr[3] = vertices[1];
    this->vertexBufferEndPtr[4] = vertices[2];
    this->vertexBufferEndPtr[5] = vertices[3];

    this->vertexBufferEndPtr += 6;
    this->spritesToDraw++;
    return ZUN_SUCCESS;
}

}
