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

extern AnmBackgroundViewportView *g_CurrentBackgroundViewport;
extern AnmBackgroundStateDrawView *g_Background;
extern Float3 g_BackgroundCameraPosition;

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
    sprite.cosine = (f32)cos(sprite.rotation);
    sprite.sine = (f32)sin(sprite.rotation);
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

// FUNCTION: TH095 0x0043FC60.
ZunResult AnmManager::ProjectCameraFacingQuad(AnmVm *vm)
{
    f32 rotation = vm->rotation.z;
    f32 cosine = (f32)cos(rotation);
    f32 sine = (f32)sin(rotation);
    Float3 origin(0.0f, 0.0f, 0.0f);
    D3DXMATRIX worldMatrix;
    Float3 projectedPosition;
    Float3 projectedReference;
    Float3 delta;
    f32 x[4];
    f32 y[4];

    D3DXMatrixIdentity(&worldMatrix);
    worldMatrix._41 = vm->position.x + vm->positionOffset.x;
    worldMatrix._42 = vm->position.y + vm->positionOffset.y;
    worldMatrix._43 = vm->position.z + vm->positionOffset.z;

    D3DXVec3Project(reinterpret_cast<D3DXVECTOR3 *>(&projectedPosition),
                    reinterpret_cast<D3DXVECTOR3 *>(&origin),
                    &g_CurrentBackgroundViewport->viewport,
                    &g_CurrentBackgroundViewport->projectionMatrix,
                    &g_CurrentBackgroundViewport->viewMatrix, &worldMatrix);
    if (projectedPosition.z < 0.0f || projectedPosition.z > 1.0f)
        return ZUN_ERROR;

    D3DXVec3Project(reinterpret_cast<D3DXVECTOR3 *>(&projectedReference),
                    reinterpret_cast<D3DXVECTOR3 *>(
                        &g_CurrentBackgroundViewport->cameraRight),
                    &g_CurrentBackgroundViewport->viewport,
                    &g_CurrentBackgroundViewport->projectionMatrix,
                    &g_CurrentBackgroundViewport->viewMatrix, &worldMatrix);

    delta = projectedReference - projectedPosition;
    f32 projectedScale =
        D3DXVec3Length(reinterpret_cast<D3DXVECTOR3 *>(&delta)) * 0.5f;
    f32 spriteHalfWidth =
        projectedScale * vm->spriteSize.x * vm->scale.x;
    f32 spriteHalfHeight =
        projectedScale * vm->spriteSize.y * vm->scale.y;
    f32 xOffset = projectedPosition.x;
    f32 yOffset = projectedPosition.y;

    cosine = (f32)cos(rotation);
    sine = (f32)sin(rotation);

    switch (vm->renderStateA)
    {
    case 1:
        x[0] = x[2] = 0.0f;
        x[1] = x[3] = spriteHalfWidth;
        break;
    case 0:
        x[0] = x[2] = -spriteHalfWidth * 0.5f;
        x[1] = x[3] = spriteHalfWidth * 0.5f;
        break;
    case 2:
        x[0] = x[2] = -spriteHalfWidth;
        x[1] = x[3] = 0.0f;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        y[0] = y[1] = 0.0f;
        y[2] = y[3] = spriteHalfHeight;
        break;
    case 0:
        y[0] = y[1] = -spriteHalfHeight * 0.5f;
        y[2] = y[3] = spriteHalfHeight * 0.5f;
        break;
    case 2:
        y[0] = y[1] = -spriteHalfHeight;
        y[2] = y[3] = 0.0f;
        break;
    }

    for (i32 i = 0; i < 4; i++)
    {
        this->TranslateRotation(&g_AnmTexturedVertices[i], x[i], y[i], sine,
                                cosine, xOffset, yOffset);
    }

    g_AnmTexturedVertices[0].z = g_AnmTexturedVertices[1].z =
        g_AnmTexturedVertices[2].z = g_AnmTexturedVertices[3].z =
            vm->position.z;
    return ZUN_SUCCESS;
}

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
