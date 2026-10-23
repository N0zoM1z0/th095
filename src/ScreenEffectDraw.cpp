#define WIN32_LEAN_AND_MEAN
#include "AnmManager.hpp"
#include "ScreenEffect.hpp"

#include <windows.h>
#include <d3d8.h>
#include <stddef.h>

namespace th095
{

struct ScreenEffectQuadVertex
{
    Float3 position;
    float rhw;
    unsigned int diffuse;
};
typedef char ScreenEffectQuadVertexSizeIs14[
    (sizeof(ScreenEffectQuadVertex) == 0x14 &&
     offsetof(ScreenEffectQuadVertex, rhw) == 0x0c &&
     offsetof(ScreenEffectQuadVertex, diffuse) == 0x10) ? 1 : -1];

// FUNCTION: TH095 0x00436920; TH08 0x0045B1E0 is the source-shape oracle.
// TH095 omits TH08's conditional ZWRITE-disable device call. The six inline
// cache-clear calls are codegen-significant: the isolated TU preserves one real
// VC7.1 `this` home per call, accounting for the target's extra 0x18 live frame.
void ScreenEffect::DrawSquare(ScreenEffectRect *rect, unsigned int color)
{
    g_AnmManager->FlushVertexBuffer();

    ScreenEffectQuadVertex vertices[4];

    vertices[0].position =
        Float3(rect->left, rect->top, 0.0f);
    vertices[1].position =
        Float3(rect->right, rect->top, 0.0f);
    vertices[2].position =
        Float3(rect->left, rect->bottom, 0.0f);
    vertices[3].position =
        Float3(rect->right, rect->bottom, 0.0f);

    vertices[3].rhw = 1.0f;
    vertices[2].rhw = vertices[3].rhw;
    vertices[1].rhw = vertices[2].rhw;
    vertices[0].rhw = vertices[1].rhw;

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
        sizeof(ScreenEffectQuadVertex));

    g_AnmManager->InvalidateDrawPath();
    g_AnmManager->ClearSprite();
    g_AnmManager->ClearTexture();
    g_AnmManager->ClearColorOp();
    g_AnmManager->InvalidateBlendMode();
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
