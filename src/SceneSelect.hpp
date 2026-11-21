#ifdef TH095_MATCH_EXACT
#include "SceneSelectExact.hpp"
#else
#ifndef TH095_SCENE_SELECT_HPP
#define TH095_SCENE_SELECT_HPP

#include "AnmManager.hpp"
#include "FrontEndGlobals.hpp"
#include "FrontEndPendingTextureData.hpp"
#include "SceneValueQueue.hpp"
#include "ScenePreviewDataQueue.hpp"
#include "Global.hpp"
#include "AnmVmId.hpp"
#include "PixelFormats.hpp"
#include "ResultScreen.hpp"
#include "ScoreData.hpp"
#include "SceneData.hpp"

namespace th095
{


struct SceneGroupCursorView
{
    i32 current;
    u8 unknown004[0xd4];
};

typedef AnmLoadedSprite SceneLoadedSpriteView;
typedef AnmVm SceneAnmVmView;
typedef AnmManager SceneAnmManagerView;

// Scene controller storage needs a POD four-byte handle because it is overlaid
// with the preview-text view.  Calls cross the boundary through the canonical
// AnmVmId, so the executable has only one manager/method ABI.
struct SceneAnmVmId
{
    i32 value;

    operator AnmVmId() const
    {
        AnmVmId id;
        id.value = this->value;
        return id;
    }

    SceneAnmVmId &operator=(AnmVmId id)
    {
        this->value = id.value;
        return *this;
    }

    SceneAnmVmView *GetVm()
    {
        AnmVmId id = *this;
        AnmVm *vm = id.GetVm();
        if (vm == NULL)
        {
            this->value = id.value;
        }
        return vm;
    }

    void SetInterrupt(i32 interrupt)
    {
        g_AnmManager->SetInterrupt(*this, interrupt);
    }
};

i32 __fastcall GetAnmFormat(i32 format);

typedef AnmLoaded SceneAnmLoadedView;

struct SceneAnmVmIdArray
{
    SceneAnmVmId values[165];

    SceneAnmVmId &operator[](i32 index)
    {
        return this->values[index];
    }

    void SetInterrupt(i32 index, i32 interrupt)
    {
        g_AnmManager->SetInterrupt(this->values[index], interrupt);
    }
};

struct SceneSelectControllerView
{
    SceneAnmLoadedView *sceneAnm;
    SceneAnmLoadedView *transitionAnm;
    ZunTimer stateTimer;
    ZunTimer animationTimer;
    i32 selectedGroup;
    u8 unknown0024[0xd4];
    // Replay Browser uses this second cursor for its per-page selection.
    ResultScreenReplayCursor secondaryCursor;
    SceneGroupCursorView groupCursors[12];
    i32 selectedScoreEntryIndex;
    SceneAnmVmIdArray vmIds;
    i8 specialDisplayStates[10];
    i8 currentDisplayState;
    u8 unknown0e93;
    AnmVmId previewTextVmIds[3];
    i32 previewTimer;
    u8 unknown0ea4[0x525c];
    AnmVmId transitionVm;
    u8 unknown6104[0x0c];
    FrontEndRequestedState requestedState;
    u8 unknown6114[0x0c];
    union
    {
        u32 flags;
        FrontEndControllerFlagBits flagBits;
    };
    u8 unknown6124[4];
    SceneValueQueue selectionQueue;
    SceneValueQueue loadedSceneQueue;
    ScenePreviewDataQueue groupPreviewDataQueue;
    SceneValueQueue groupPreviewSizeQueue;
    ScenePreviewDataQueue scenePreviewDataQueue;
    SceneValueQueue scenePreviewSizeQueue;
    SceneValueQueue groupPreviewQueue;
    SceneValueQueue scenePreviewQueue;
    SceneValueQueue loadedGroupQueue;
    SceneStateHistory stateHistory;
    u8 unknown63c0[0x0c];
    FrontEndPendingTextureData pendingTextures;

    i32 GetSelectedGroup()
    {
        return this->selectedGroup;
    }

    void UpdateSelectedSceneDetails();
    ChainCallbackResult UpdateSceneSelect();
    void CloseMainMenu();
    void UpdateMainMenuSelection();
    void SetDetailDigitSprite(i32 vmIndex, i32 spriteIndex);
    void ShowDetailDigit(i32 vmIndex);
    void HideDetailDigit(i32 vmIndex);

    void SetDetailDigitSpriteInline(i32 vmIndex, i32 spriteIndex)
    {
        this->sceneAnm->SetSprite(
            g_AnmManager->GetVm(this->vmIds.values[vmIndex]),
            spriteIndex);
    }

    void ShowDetailDigitInline(i32 vmIndex)
    {
        g_AnmManager->GetVm(this->vmIds.values[vmIndex])->drawEnabled = 1;
    }

    void HideDetailDigitInline(i32 vmIndex)
    {
        g_AnmManager->GetVm(this->vmIds.values[vmIndex])->drawEnabled = 0;
    }
};


typedef char SceneSelectPendingTextureDataLayout[
    (sizeof(FrontEndPendingTextureData) == 0x34 &&
     offsetof(SceneSelectControllerView, pendingTextures) == 0x63cc &&
     offsetof(FrontEndPendingTextureData, primaryData) == 0x04 &&
     offsetof(FrontEndPendingTextureData, primarySize) == 0x10 &&
     offsetof(FrontEndPendingTextureData, secondaryData) == 0x1c &&
     offsetof(FrontEndPendingTextureData, secondarySize) == 0x28) ? 1 : -1];
typedef char SceneSelectAnimationTimerAt14[
    (offsetof(SceneSelectControllerView, animationTimer) == 0x14) ? 1 : -1];

typedef char SceneAnmVmIdSizeIs4[
    (sizeof(SceneAnmVmId) == 4) ? 1 : -1];
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
typedef char SceneValueQueueSizeIs48[
    (sizeof(SceneValueQueue) == 0x48) ? 1 : -1];
typedef char SceneGroupCursorSizeIsD8[
    (sizeof(SceneGroupCursorView) == 0xd8) ? 1 : -1];
typedef char SceneSelectSecondaryCursorAtF8[
    (offsetof(SceneSelectControllerView, secondaryCursor) == 0xf8) ? 1 : -1];
typedef char SceneSelectGroupCursorsAt1D0[
    (offsetof(SceneSelectControllerView, groupCursors) == 0x1d0) ? 1 : -1];
typedef char SceneSelectVmIdsAtBF4[
    (offsetof(SceneSelectControllerView, vmIds) == 0xbf4) ? 1 : -1];
typedef char SceneSelectScoreEntryAtBF0[
    (offsetof(SceneSelectControllerView, selectedScoreEntryIndex) == 0xbf0)
        ? 1 : -1];
typedef char SceneSelectDisplayStatesAtE88[
    (offsetof(SceneSelectControllerView, specialDisplayStates) == 0xe88) ? 1 : -1];
typedef char SceneSelectPreviewTextVmIdsAtE94[
    (offsetof(SceneSelectControllerView, previewTextVmIds) == 0xe94) ? 1 : -1];
typedef char SceneSelectPreviewTimerAtEA0[
    (offsetof(SceneSelectControllerView, previewTimer) == 0xea0) ? 1 : -1];
typedef char SceneSelectTransitionVmAt6100[
    (offsetof(SceneSelectControllerView, transitionVm) == 0x6100) ? 1 : -1];
typedef char SceneSelectRequestedStateAt6110[
    (offsetof(SceneSelectControllerView, requestedState) == 0x6110) ? 1 : -1];
typedef char SceneSelectFlagsAt6120[
    (offsetof(SceneSelectControllerView, flags) == 0x6120) ? 1 : -1];
typedef char SceneSelectSelectionQueueAt6128[
    (offsetof(SceneSelectControllerView, selectionQueue) == 0x6128) ? 1 : -1];
typedef char SceneSelectLoadedSceneQueueAt6170[
    (offsetof(SceneSelectControllerView, loadedSceneQueue) == 0x6170) ? 1 : -1];
typedef char SceneSelectGroupPreviewDataQueueAt61B8[
    (offsetof(SceneSelectControllerView, groupPreviewDataQueue) == 0x61b8) ? 1 : -1];
typedef char SceneSelectGroupPreviewSizeQueueAt6200[
    (offsetof(SceneSelectControllerView, groupPreviewSizeQueue) == 0x6200) ? 1 : -1];
typedef char SceneSelectScenePreviewDataQueueAt6248[
    (offsetof(SceneSelectControllerView, scenePreviewDataQueue) == 0x6248) ? 1 : -1];
typedef char SceneSelectScenePreviewSizeQueueAt6290[
    (offsetof(SceneSelectControllerView, scenePreviewSizeQueue) == 0x6290) ? 1 : -1];
typedef char SceneSelectGroupPreviewQueueAt62D8[
    (offsetof(SceneSelectControllerView, groupPreviewQueue) == 0x62d8) ? 1 : -1];
typedef char SceneSelectScenePreviewQueueAt6320[
    (offsetof(SceneSelectControllerView, scenePreviewQueue) == 0x6320) ? 1 : -1];
typedef char SceneSelectLoadedGroupQueueAt6368[
    (offsetof(SceneSelectControllerView, loadedGroupQueue) == 0x6368) ? 1 : -1];
typedef char SceneSelectStateHistoryAt63B0[
    (offsetof(SceneSelectControllerView, stateHistory) == 0x63b0) ? 1 : -1];
typedef char SceneSelectPendingTextureCountAt63CC[
    (offsetof(SceneSelectControllerView, pendingTextures) == 0x63cc) ? 1 : -1];

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
