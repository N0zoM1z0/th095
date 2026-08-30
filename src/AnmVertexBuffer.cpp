#include "AnmManager.hpp"

namespace th095
{

struct AnmVertexBufferSourceView
{
    Float3 position;
    f32 w;
    u32 diffuse;
};

struct AnmBackgroundQuadVertexView
{
    Float3 position;
    f32 w;
    Float2 textureUV;
};

// Target-global software fallback copy of the manager's four 3D quad vertices.
extern VertexTex1Xyzrhw g_BackgroundQuadVertices[4];
extern AnmManager *g_AnmManager;

// FUNCTION: TH095 0x00442260; TH08 0x00465250 is the source-shape oracle.
void AnmManager::SetupVertexBuffer()
{
    void *lockedVertexBuffer;

    this->untexturedVertices[2].x = -128.0f;
    this->untexturedVertices[0].x = -128.0f;
    this->untexturedVertices[3].x = 128.0f;
    this->untexturedVertices[1].x = 128.0f;
    this->untexturedVertices[1].y = -128.0f;
    this->untexturedVertices[0].y = -128.0f;
    this->untexturedVertices[3].y = 128.0f;
    this->untexturedVertices[2].y = 128.0f;
    this->untexturedVertices[3].z = 0.0f;
    this->untexturedVertices[2].z = 0.0f;
    this->untexturedVertices[1].z = 0.0f;
    this->untexturedVertices[0].z = 0.0f;
    this->untexturedVertices[2].w = 0.0f;
    this->untexturedVertices[0].w = 0.0f;
    this->untexturedVertices[3].w = 1.0f;
    this->untexturedVertices[1].w = 1.0f;
    this->untexturedVertices[1].diffuse = 0;
    this->untexturedVertices[0].diffuse = 0;
    this->untexturedVertices[3].diffuse = 0x3f800000;
    this->untexturedVertices[2].diffuse = 0x3f800000;

    reinterpret_cast<AnmBackgroundQuadVertexView *>(g_BackgroundQuadVertices)[0].position = reinterpret_cast<AnmVertexBufferSourceView *>(this->untexturedVertices)[0].position;
    reinterpret_cast<AnmBackgroundQuadVertexView *>(g_BackgroundQuadVertices)[1].position = reinterpret_cast<AnmVertexBufferSourceView *>(this->untexturedVertices)[1].position;
    reinterpret_cast<AnmBackgroundQuadVertexView *>(g_BackgroundQuadVertices)[2].position = reinterpret_cast<AnmVertexBufferSourceView *>(this->untexturedVertices)[2].position;
    reinterpret_cast<AnmBackgroundQuadVertexView *>(g_BackgroundQuadVertices)[3].position = reinterpret_cast<AnmVertexBufferSourceView *>(this->untexturedVertices)[3].position;
    *reinterpret_cast<u32 *>(&reinterpret_cast<AnmBackgroundQuadVertexView *>(g_BackgroundQuadVertices)[0].textureUV.x) =
        *reinterpret_cast<u32 *>(&reinterpret_cast<AnmVertexBufferSourceView *>(this->untexturedVertices)[0].w);
    *reinterpret_cast<u32 *>(&reinterpret_cast<AnmBackgroundQuadVertexView *>(g_BackgroundQuadVertices)[0].textureUV.y) =
        reinterpret_cast<AnmVertexBufferSourceView *>(this->untexturedVertices)[0].diffuse;
    *reinterpret_cast<u32 *>(&reinterpret_cast<AnmBackgroundQuadVertexView *>(g_BackgroundQuadVertices)[1].textureUV.x) =
        *reinterpret_cast<u32 *>(&reinterpret_cast<AnmVertexBufferSourceView *>(this->untexturedVertices)[1].w);
    *reinterpret_cast<u32 *>(&reinterpret_cast<AnmBackgroundQuadVertexView *>(g_BackgroundQuadVertices)[1].textureUV.y) =
        reinterpret_cast<AnmVertexBufferSourceView *>(this->untexturedVertices)[1].diffuse;
    *reinterpret_cast<u32 *>(&reinterpret_cast<AnmBackgroundQuadVertexView *>(g_BackgroundQuadVertices)[2].textureUV.x) =
        *reinterpret_cast<u32 *>(&reinterpret_cast<AnmVertexBufferSourceView *>(this->untexturedVertices)[2].w);
    *reinterpret_cast<u32 *>(&reinterpret_cast<AnmBackgroundQuadVertexView *>(g_BackgroundQuadVertices)[2].textureUV.y) =
        reinterpret_cast<AnmVertexBufferSourceView *>(this->untexturedVertices)[2].diffuse;
    *reinterpret_cast<u32 *>(&reinterpret_cast<AnmBackgroundQuadVertexView *>(g_BackgroundQuadVertices)[3].textureUV.x) =
        *reinterpret_cast<u32 *>(&reinterpret_cast<AnmVertexBufferSourceView *>(this->untexturedVertices)[3].w);
    *reinterpret_cast<u32 *>(&reinterpret_cast<AnmBackgroundQuadVertexView *>(g_BackgroundQuadVertices)[3].textureUV.y) =
        reinterpret_cast<AnmVertexBufferSourceView *>(this->untexturedVertices)[3].diffuse;

    g_Supervisor.d3dDevice->CreateVertexBuffer(
        sizeof(this->untexturedVertices), 0, D3DFVF_XYZ | D3DFVF_TEX1,
        D3DPOOL_MANAGED, &this->quadVertexBuffer);
    this->quadVertexBuffer->Lock(0, 0,
        reinterpret_cast<BYTE **>(&lockedVertexBuffer), 0);
    memcpy(lockedVertexBuffer, this->untexturedVertices,
           sizeof(this->untexturedVertices));
    this->quadVertexBuffer->Unlock();
    g_Supervisor.d3dDevice->SetStreamSource(
        0, g_AnmManager->quadVertexBuffer, sizeof(VertexDiffuseXyzrhw));
}

} // namespace th095
