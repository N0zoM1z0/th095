#ifdef TH095_MATCH_EXACT
#include "SceneSelectEmission.hpp"
#else
#ifndef TH095_SCENE_SELECT_HPP
#define TH095_SCENE_SELECT_HPP

#include "AnmManager.hpp"
#include "FrontEndGlobals.hpp"
#include "FrontEndPendingTextureData.hpp"
#include "SceneValueQueue.hpp"
#include "ScenePreviewDataQueue.hpp"
#include "Global.hpp"
#include "SceneMenuVmIds.hpp"
#include "PixelFormats.hpp"
#include "ResultScreen.hpp"
#include "ScoreData.hpp"
#include "SceneData.hpp"

namespace th095
{


typedef AnmLoadedSprite SceneLoadedSpriteView;
typedef AnmVm SceneAnmVmView;
typedef AnmManager SceneAnmManagerView;

inline AnmVm *SceneAnmVmId::GetVm()
{
    AnmVmId id = *this;
    AnmVm *vm = id.GetVm();
    if (vm == NULL)
    {
        this->value = id.value;
    }
    return vm;
}

inline void SceneAnmVmId::SetInterrupt(i32 interrupt)
{
    g_AnmManager->SetInterrupt(*this, interrupt);
}

i32 __fastcall GetAnmFormat(i32 format);

typedef AnmLoaded SceneAnmLoadedView;

inline void SceneAnmVmIdArray::SetInterrupt(i32 index, i32 interrupt)
{
    g_AnmManager->SetInterrupt(this->values[index], interrupt);
}

#define SCENE_MENU_AUX_HANDLE(stored) (stored)
#define SCENE_DETAIL_SHOW(vm) ((vm)->drawEnabled = 1)
#define SCENE_DETAIL_HIDE(vm) ((vm)->drawEnabled = 0)
#define SCENE_DETAIL_MANAGER g_AnmManager

extern u8 g_SceneTextBuffer[0x40];
extern u32 g_SceneGroupColors[11];
extern u32 g_SceneLockedTransitionColor;
extern u32 g_SceneLockedInitialColor;

void __fastcall LoadSceneSelectionAssets(void *unused);

void __cdecl SceneWriteText(SceneAnmManagerView *manager,
                            SceneAnmVmView *vm, u32 color, u32 shadowColor,
                            const char *text);

} // namespace th095

#endif

#endif // TH095_MATCH_EXACT
