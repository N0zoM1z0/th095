// ANM call declarations used by the pinned front-end comparisons.
#ifndef TH095_SCENE_SELECT_HPP
#define TH095_SCENE_SELECT_HPP

#include "Global.hpp"
#include "FrontEndGlobals.hpp"
#include "FrontEndPendingTextureData.hpp"
#include "SceneValueQueue.hpp"
#include "ScenePreviewDataQueue.hpp"
#include "SceneMenuVmIds.hpp"
#include "PixelFormats.hpp"
#include "ReplayScanWorker.hpp"
#include "SupervisorFlags.hpp"
#include "ResultScreen.hpp"
#include "ScoreData.hpp"
#include "SceneData.hpp"
#include "ZunTimer.hpp"
#include <time.h>

namespace th095
{

struct Float3;

struct SceneLoadedSpriteView
{
    u8 unknown000[0x28];
    f32 uvEndX;
    f32 uvEndY;
};

struct SceneAnmVmView
{
    u8 unknown000[0x40];
    f32 spriteWidth;
    f32 spriteHeight;
    u8 unknown048[0x1d8];
    u32 color1;
    u32 color2;
    u32 flagsWord;
    u8 unknown22c[2];
    i16 pendingInterrupt;
    u8 unknown230[0x14];
    SceneLoadedSpriteView *loadedSprite;
    u8 unknown248[0x78];
    u8 glyphWidth;
    u8 glyphHeight;
};

struct SceneAnmManagerView
{
    void SetInterrupt(AnmVmId id, i32 interrupt);
    void SetInterrupt(SceneAnmVmId id, i32 interrupt);
    SceneAnmVmView *GetVm(SceneAnmVmId id);
    void MarkVmForDeletion(SceneAnmVmId id);
};

extern SceneAnmManagerView *g_SceneAnmManager;

struct AnmTextureEntryView;

i32 __fastcall GetAnmFormat(i32 format);

struct SceneAnmLoadedView
{
    u8 unknown000[0x14];
    AnmTextureEntryView *textures;

    void SetSprite(SceneAnmVmView *vm, i32 spriteIndex);
    void SetSprite(AnmVm *vm, i32 spriteIndex);
    SceneAnmVmId CreateVm(i32 scriptIndex, i32 renderMode);
    SceneAnmVmId CreateVm(i32 scriptIndex, Float3 *position);
};

inline void SceneAnmVmIdArray::SetInterrupt(i32 index, i32 interrupt)
{
    g_SceneAnmManager->SetInterrupt(this->values[index], interrupt);
}

// The controller constructor clears raw handle storage with the rest of the
// allocation. Constructed AnmVmId fields add four initializations. Preserve
// the original by-value argument lowering when passing the stored bits from
// the preview slots to the ANM manager in the pinned comparison build.
#define SCENE_MENU_AUX_HANDLE(stored) (reinterpret_cast<AnmVmId &>(stored))

#define SCENE_DETAIL_SHOW(vm) ((vm)->flagsWord |= 2)
#define SCENE_DETAIL_HIDE(vm) ((vm)->flagsWord &= ~2)
#define SCENE_DETAIL_MANAGER g_SceneAnmManager

typedef char SceneAnmVmGlyphSizeAt2C0[
    (offsetof(SceneAnmVmView, glyphWidth) == 0x2c0 &&
     offsetof(SceneAnmVmView, glyphHeight) == 0x2c1) ? 1 : -1];
typedef char SceneAnmVmFlagsAt228[
    (offsetof(SceneAnmVmView, flagsWord) == 0x228) ? 1 : -1];
typedef char SceneAnmVmSpriteSizeAt40[
    (offsetof(SceneAnmVmView, spriteWidth) == 0x40 &&
     offsetof(SceneAnmVmView, spriteHeight) == 0x44) ? 1 : -1];
typedef char SceneAnmVmColor1At220[
    (offsetof(SceneAnmVmView, color1) == 0x220) ? 1 : -1];
typedef char SceneAnmVmInterruptAt22E[
    (offsetof(SceneAnmVmView, pendingInterrupt) == 0x22e) ? 1 : -1];
typedef char SceneAnmVmLoadedSpriteAt244[
    (offsetof(SceneAnmVmView, loadedSprite) == 0x244) ? 1 : -1];

extern u8 g_SceneTextBuffer[0x40];
extern u32 g_SceneGroupColors[11];
extern u32 g_SceneLockedTransitionColor;
extern u32 g_SceneLockedInitialColor;

void __fastcall LoadSceneSelectionAssets(void *unused);

struct AnmManager;

void __cdecl SceneWriteText(AnmManager *manager,
                            AnmVm *vm, u32 color, u32 unknown,
                            const char *text);

} // namespace th095

#endif
