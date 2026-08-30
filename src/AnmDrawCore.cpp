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

static __forceinline u8 MixAnmColor(u8 first, u8 second)
{
    u32 value = (u32)first * (u32)second >> 7;
    if (value >= 256)
        value = 255;
    return (u8)value;
}

static f32 RoundAnmCoordinateToNearestEven(f32 value)
{
    f32 rounded = (f32)floor(value);
    f32 fraction = value - rounded;
    if (fraction > 0.5f ||
        (fraction == 0.5f && (((i32)rounded & 1) != 0)))
        rounded += 1.0f;
    return rounded;
}

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

// FUNCTION: TH095 0x0043ECD0.
ZunResult AnmManager::DrawInner(AnmVm *vm, i32 flags)
{
    ZunColor color;
    f32 triangleX1;
    f32 triangleX2;
    f32 triangleY1;
    f32 triangleY2;
    D3DVIEWPORT8 &viewport =
        reinterpret_cast<AnmSupervisorDrawView *>(&g_Supervisor)
            ->currentViewport->viewport;

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
        // The target emits x87 round-to-nearest-even followed by the D3D8
        // half-pixel correction. VC7.1 has no natural C++ intrinsic for
        // FRNDINT, so express the same finite screen-coordinate result here.
        triangleX1 = RoundAnmCoordinateToNearestEven(
                         g_AnmTexturedVertices[0].x) - 0.5f;
        triangleX2 = RoundAnmCoordinateToNearestEven(
                         g_AnmTexturedVertices[1].x) - 0.5f;
        triangleY1 = RoundAnmCoordinateToNearestEven(
                         g_AnmTexturedVertices[0].y) - 0.5f;
        triangleY2 = RoundAnmCoordinateToNearestEven(
                         g_AnmTexturedVertices[2].y) - 0.5f;
        g_AnmTexturedVertices[2].y =
            g_AnmTexturedVertices[3].y = triangleY2;
        g_AnmTexturedVertices[0].y =
            g_AnmTexturedVertices[1].y = triangleY1;
        g_AnmTexturedVertices[1].x =
            g_AnmTexturedVertices[3].x = triangleX2;
        g_AnmTexturedVertices[0].x =
            g_AnmTexturedVertices[2].x = triangleX1;
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

    if (triangleX1 < viewport.X || triangleY1 < viewport.Y ||
        triangleX2 > viewport.X + viewport.Width ||
        triangleY2 > viewport.Y + viewport.Height)
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
        color.color = vm->flag15 ? vm->color2.color : vm->color1.color;
        if (this->useMixColor)
        {
            color.r = MixAnmColor(color.r, this->color.r);
            color.g = MixAnmColor(color.g, this->color.g);
            color.b = MixAnmColor(color.b, this->color.b);
            color.a = MixAnmColor(color.a, this->color.a);
        }

        g_AnmTexturedVertices[0].diffuse = color.color;
        g_AnmTexturedVertices[1].diffuse = color.color;
        g_AnmTexturedVertices[2].diffuse = color.color;
        g_AnmTexturedVertices[3].diffuse = color.color;
    }

    this->SetRenderStateForVm(vm);
    this->AddSpriteToDrawBuffer(g_AnmTexturedVertices);
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
