#include "SceneSelect.hpp"

#include <string.h>

namespace th095
{

struct SceneTextureClearLocals
{
    D3DSURFACE_DESC description;
    D3DLOCKED_RECT lockedRect;
    IDirect3DSurface8 *surface;
};

typedef char SceneTextureClearLocalsSizeIs2C[
    (sizeof(SceneTextureClearLocals) == 0x2c) ? 1 : -1];

void SceneTextureEntryView::Clear()
{
    SceneTextureClearLocals locals;

    this->texture->GetSurfaceLevel(0, &locals.surface);
    locals.surface->GetDesc(&locals.description);
    locals.surface->LockRect(&locals.lockedRect, NULL, 0);
    memset(locals.lockedRect.pBits, 0,
           locals.lockedRect.Pitch * locals.description.Height);
    locals.surface->UnlockRect();
    if (locals.surface != NULL)
    {
        locals.surface->Release();
        locals.surface = NULL;
    }
}

} // namespace th095
