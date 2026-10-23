#include "AnmManager.hpp"

namespace th095
{

// Target 0x004CA290: a distinct 24-byte-stride copy with four untouched
// +0x0C words. SetupVertexBuffer is the only established data writer. A
// compiler-generated constructor walk also references this array, but no
// rendering reader is established. See docs/ANM_QUAD_VERTICES.md.
DIFFABLE_STATIC_ARRAY(AnmQuadVertexCopy, 4, g_BackgroundQuadVertices);
extern AnmManager *g_AnmManager;

// FUNCTION: TH095 0x00442260; TH08 0x00465250 is the source-shape oracle.
void AnmManager::SetupVertexBuffer()
{
    void *lockedVertexBuffer;

    this->quadVertices[2].position.x = -128.0f;
    this->quadVertices[0].position.x = -128.0f;
    this->quadVertices[3].position.x = 128.0f;
    this->quadVertices[1].position.x = 128.0f;
    this->quadVertices[1].position.y = -128.0f;
    this->quadVertices[0].position.y = -128.0f;
    this->quadVertices[3].position.y = 128.0f;
    this->quadVertices[2].position.y = 128.0f;
    this->quadVertices[3].position.z = 0.0f;
    this->quadVertices[2].position.z = 0.0f;
    this->quadVertices[1].position.z = 0.0f;
    this->quadVertices[0].position.z = 0.0f;
    this->quadVertices[2].textureUV.x = 0.0f;
    this->quadVertices[0].textureUV.x = 0.0f;
    this->quadVertices[3].textureUV.x = 1.0f;
    this->quadVertices[1].textureUV.x = 1.0f;
    this->quadVertices[1].textureUV.y = 0.0f;
    this->quadVertices[0].textureUV.y = 0.0f;
    this->quadVertices[3].textureUV.y = 1.0f;
    this->quadVertices[2].textureUV.y = 1.0f;

    g_BackgroundQuadVertices[0].position = this->quadVertices[0].position;
    g_BackgroundQuadVertices[1].position = this->quadVertices[1].position;
    g_BackgroundQuadVertices[2].position = this->quadVertices[2].position;
    g_BackgroundQuadVertices[3].position = this->quadVertices[3].position;
    g_BackgroundQuadVertices[0].textureUV.x = this->quadVertices[0].textureUV.x;
    g_BackgroundQuadVertices[0].textureUV.y = this->quadVertices[0].textureUV.y;
    g_BackgroundQuadVertices[1].textureUV.x = this->quadVertices[1].textureUV.x;
    g_BackgroundQuadVertices[1].textureUV.y = this->quadVertices[1].textureUV.y;
    g_BackgroundQuadVertices[2].textureUV.x = this->quadVertices[2].textureUV.x;
    g_BackgroundQuadVertices[2].textureUV.y = this->quadVertices[2].textureUV.y;
    g_BackgroundQuadVertices[3].textureUV.x = this->quadVertices[3].textureUV.x;
    g_BackgroundQuadVertices[3].textureUV.y = this->quadVertices[3].textureUV.y;

    g_Supervisor.d3dDevice->CreateVertexBuffer(
        sizeof(this->quadVertices), 0, D3DFVF_XYZ | D3DFVF_TEX1,
        D3DPOOL_MANAGED, &this->quadVertexBuffer);
    this->quadVertexBuffer->Lock(0, 0,
        reinterpret_cast<BYTE **>(&lockedVertexBuffer), 0);
    memcpy(lockedVertexBuffer, this->quadVertices,
           sizeof(this->quadVertices));
    this->quadVertexBuffer->Unlock();
    // Target 0x004424DF..0x004424FE binds the GLOBAL manager's buffer,
    // after creating and uploading this receiver's buffer above.
    g_Supervisor.d3dDevice->SetStreamSource(
        0, g_AnmManager->quadVertexBuffer, sizeof(AnmQuadVertex));
}

} // namespace th095
