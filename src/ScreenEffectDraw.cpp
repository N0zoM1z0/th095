#define WIN32_LEAN_AND_MEAN
#include "ScreenEffect.hpp"

#include <windows.h>
#include <d3d8.h>
#include <stddef.h>

namespace th095
{

struct ScreenEffectFloat3
{
    ScreenEffectFloat3()
    {
    }

    ScreenEffectFloat3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float x;
    float y;
    float z;
};

struct ScreenEffectVertexDiffuseXyzrhw
{
    ScreenEffectFloat3 position;
    float w;
    unsigned int diffuse;
};
typedef char ScreenEffectVertexDiffuseXyzrhwSizeIs14[
    (sizeof(ScreenEffectVertexDiffuseXyzrhw) == 0x14) ? 1 : -1];

struct ScreenEffectAnmManagerView
{
    unsigned char unknown000[0x1760];
    void *currentTexture;
    unsigned char currentBlendMode;
    unsigned char currentColorOp;
    unsigned char currentVertexShader;
    unsigned char disableZWrite;
    unsigned int currentTextureFactor;
    void *currentSprite;

    void FlushVertexBuffer();

    __forceinline void ClearBlendMode()
    {
        currentBlendMode = 3;
    }

    __forceinline void ClearColorOp()
    {
        currentColorOp = 0xff;
    }

    __forceinline void ClearVertexShader()
    {
        currentVertexShader = 0xff;
    }

    __forceinline void ClearTexture()
    {
        currentTexture = NULL;
    }

    __forceinline void ClearSprite()
    {
        currentSprite = NULL;
    }

    __forceinline void ClearZWrite()
    {
        disableZWrite = 0xff;
    }
};
typedef char ScreenEffectAnmTextureAt1760[
    (offsetof(ScreenEffectAnmManagerView, currentTexture) == 0x1760) ? 1 : -1];
typedef char ScreenEffectAnmSpriteAt176C[
    (offsetof(ScreenEffectAnmManagerView, currentSprite) == 0x176c) ? 1 : -1];

struct ScreenEffectSupervisorView
{
    unsigned char unknown000[8];
    IDirect3DDevice8 *d3dDevice;
};

extern ScreenEffectAnmManagerView *g_AnmManager;
extern ScreenEffectSupervisorView g_Supervisor;

// FUNCTION: TH095 0x00436920; TH08 0x0045B1E0 is the source-shape oracle.
// TH095 omits TH08's conditional ZWRITE-disable device call. The six inline
// cache-clear calls are codegen-significant: the isolated TU preserves one real
// VC7.1 `this` home per call, accounting for the target's extra 0x18 live frame.
void ScreenEffect::DrawSquare(ScreenEffectRect *rect, unsigned int color)
{
    g_AnmManager->FlushVertexBuffer();

    ScreenEffectVertexDiffuseXyzrhw vertices[4];

    vertices[0].position =
        ScreenEffectFloat3(rect->left, rect->top, 0.0f);
    vertices[1].position =
        ScreenEffectFloat3(rect->right, rect->top, 0.0f);
    vertices[2].position =
        ScreenEffectFloat3(rect->left, rect->bottom, 0.0f);
    vertices[3].position =
        ScreenEffectFloat3(rect->right, rect->bottom, 0.0f);

    vertices[3].w = 1.0f;
    vertices[2].w = vertices[3].w;
    vertices[1].w = vertices[2].w;
    vertices[0].w = vertices[1].w;

    vertices[3].diffuse = color;
    vertices[2].diffuse = vertices[3].diffuse;
    vertices[1].diffuse = vertices[2].diffuse;
    vertices[0].diffuse = vertices[1].diffuse;

    g_Supervisor.d3dDevice->SetTextureStageState(
        0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_Supervisor.d3dDevice->SetTextureStageState(
        0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_Supervisor.d3dDevice->SetTextureStageState(
        0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_Supervisor.d3dDevice->SetTextureStageState(
        0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

    g_Supervisor.d3dDevice->SetRenderState(
        D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_Supervisor.d3dDevice->SetVertexShader(
        D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    g_Supervisor.d3dDevice->DrawPrimitiveUP(
        D3DPT_TRIANGLESTRIP, 2, vertices,
        sizeof(ScreenEffectVertexDiffuseXyzrhw));

    g_AnmManager->ClearVertexShader();
    g_AnmManager->ClearSprite();
    g_AnmManager->ClearTexture();
    g_AnmManager->ClearColorOp();
    g_AnmManager->ClearBlendMode();
    g_AnmManager->ClearZWrite();

    g_Supervisor.d3dDevice->SetTextureStageState(
        0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    g_Supervisor.d3dDevice->SetTextureStageState(
        0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_Supervisor.d3dDevice->SetTextureStageState(
        0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_Supervisor.d3dDevice->SetTextureStageState(
        0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
}

} // namespace th095
