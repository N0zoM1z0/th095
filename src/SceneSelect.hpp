#ifdef TH095_MATCH_EXACT
#include "SceneSelectExact.hpp"
#else
#ifndef TH095_SCENE_SELECT_HPP
#define TH095_SCENE_SELECT_HPP

#include "AnmManager.hpp"
#include "Global.hpp"
#include "AnmVmId.hpp"
#include "PixelFormats.hpp"
#include "ScoreData.hpp"
#include "SceneData.hpp"

namespace th095
{

typedef ResultScoreEntryView SceneScoreEntryView;

struct SceneValueQueue
{
    i32 values[16];
    i32 count;
    i32 capacity;

    i32 Push(i32 value);
    i32 Pop();
    i32 Size()
    {
        return this->count;
    }
};

struct SceneGroupCursorView
{
    i32 current;
    u8 unknown004[0xd4];
};

struct SceneStateHistoryView
{
    i32 values[3];
    i32 count;
};

typedef AnmLoadedSprite SceneLoadedSpriteView;
typedef AnmVm SceneAnmVmView;
typedef AnmManager SceneAnmManagerView;
typedef AnmTextureEntryView SceneTextureEntryView;

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
        return g_AnmManager->GetVm(*this);
    }

    void SetInterrupt(i32 interrupt)
    {
        g_AnmManager->SetInterrupt(*this, interrupt);
    }
};

typedef char SceneTextureEntrySizeIs10[
    (sizeof(SceneTextureEntryView) == 0x10) ? 1 : -1];

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

struct ScenePreviewTextSourcesView
{
    u8 unknown0bf4[0x26c];
    i32 lockedTextId;
    i32 unattemptedTextId;
    i32 belowRequirementTextId;
    i32 attemptedTextId;
    u8 unknown0e70[0x18];
};

struct SceneSelectControllerView
{
    SceneAnmLoadedView *sceneAnm;
    u8 unknown0004[0x1c];
    i32 selectedGroup;
    u8 unknown0024[0x1ac];
    SceneGroupCursorView groupCursors[12];
    i32 selectedScoreEntryIndex;
    union
    {
        SceneAnmVmIdArray vmIds;
        ScenePreviewTextSourcesView previewTextSources;
    };
    i8 lockedDisplayState;
    i8 unattemptedDisplayState;
    i8 belowRequirementDisplayState;
    i8 attemptedDisplayState;
    u8 unknown0e8c[6];
    i8 currentDisplayState;
    u8 unknown0e93;
    SceneAnmVmId previewTextVmIds[3];
    i32 previewTimer;
    u8 unknown0ea4[0x5284];
    SceneValueQueue selectionQueue;
    u8 unknown6170[0x168];
    SceneValueQueue groupPreviewQueue;
    SceneValueQueue scenePreviewQueue;
    u8 unknown6368[0x48];
    SceneStateHistoryView stateHistory;

    i32 GetSelectedGroup()
    {
        return this->selectedGroup;
    }

    void RefreshSceneSelection(i32 unused);
    void BuildScenePreviewText();
    void UpdateSelectedSceneDetails();
    ChainCallbackResult Update();
    ChainCallbackResult UpdateMainMenu();
    ChainCallbackResult UpdateSceneSelect();
    void CloseMainMenu();
    void UpdateMainMenuSelection();
    ChainCallbackResult Draw();
    static void __fastcall OnUpdate(SceneSelectControllerView *controller);
    static void __fastcall OnDraw(SceneSelectControllerView *controller);
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
        g_AnmManager->GetVm(this->vmIds.values[vmIndex])->flagsWord |= 2;
    }

    void HideDetailDigitInline(i32 vmIndex)
    {
        g_AnmManager->GetVm(this->vmIds.values[vmIndex])->flagsWord &= ~2;
    }

    char *ResolveSceneText(i32 textId, i32 column, i32 argument1,
                           i32 argument2);
};

typedef ResultSaveDataView SceneSaveDataView;

typedef char SceneScoreEntrySizeIs60[
    (sizeof(SceneScoreEntryView) == 0x60) ? 1 : -1];
typedef char SceneScoreEntryScoreAt10[
    (offsetof(SceneScoreEntryView, score) == 0x10) ? 1 : -1];
typedef char SceneScoreEntryDetailScoreAt18[
    (offsetof(SceneScoreEntryView, detailScore) == 0x18) ? 1 : -1];
typedef char SceneScoreEntryAttemptCountAt44[
    (offsetof(SceneScoreEntryView, attemptCount) == 0x44) ? 1 : -1];
typedef char SceneScoreEntryFlagsAt50[
    (offsetof(SceneScoreEntryView, flags) == 0x50) ? 1 : -1];
typedef char SceneScoreEntrySlowRatesAt48[
    (offsetof(SceneScoreEntryView, highScoreSlowRate) == 0x48 &&
     offsetof(SceneScoreEntryView, bestShotSlowRate) == 0x4c) ? 1 : -1];
typedef char SceneDefinitionTitleArgumentsAt04[
    (offsetof(SceneDefinitionView, titleArgument1) == 0x04 &&
     offsetof(SceneDefinitionView, titleArgument2) == 0x08) ? 1 : -1];
typedef char SceneDefinitionTitleTextIdAt28[
    (offsetof(SceneDefinitionView, titleTextId) == 0x28) ? 1 : -1];
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
typedef char SceneSaveDataScoresAt460[
    (offsetof(SceneSaveDataView, sceneScores) == 0x460) ? 1 : -1];
typedef char SceneSaveDataSelectionAt1E[
    (offsetof(SceneSaveDataView, lastSelectedGroup) == 0x1e &&
     offsetof(SceneSaveDataView, lastSelectedScene) == 0x20) ? 1 : -1];
typedef char SceneSaveBestShotRecordsAt3160[
    (offsetof(SceneSaveDataView, bestShotRecords) == 0x3160) ? 1 : -1];
typedef char SceneScoreEntryCaptureTimeAt3C[
    (offsetof(SceneScoreEntryView, captureTime) == 0x3c) ? 1 : -1];
typedef char SceneSelectGroupCursorsAt1D0[
    (offsetof(SceneSelectControllerView, groupCursors) == 0x1d0) ? 1 : -1];
typedef char SceneSelectVmIdsAtBF4[
    (offsetof(SceneSelectControllerView, vmIds) == 0xbf4) ? 1 : -1];
typedef char SceneSelectScoreEntryAtBF0[
    (offsetof(SceneSelectControllerView, selectedScoreEntryIndex) == 0xbf0)
        ? 1 : -1];
typedef char SceneSelectDisplayStatesAtE88[
    (offsetof(SceneSelectControllerView, lockedDisplayState) == 0xe88) ? 1 : -1];
typedef char SceneSelectPreviewTextSourcesAtE60[
    (offsetof(SceneSelectControllerView, previewTextSources) +
         offsetof(ScenePreviewTextSourcesView, lockedTextId) == 0xe60) ? 1 : -1];
typedef char SceneSelectPreviewTextVmIdsAtE94[
    (offsetof(SceneSelectControllerView, previewTextVmIds) == 0xe94) ? 1 : -1];
typedef char SceneSelectPreviewTimerAtEA0[
    (offsetof(SceneSelectControllerView, previewTimer) == 0xea0) ? 1 : -1];
typedef char SceneSelectSelectionQueueAt6128[
    (offsetof(SceneSelectControllerView, selectionQueue) == 0x6128) ? 1 : -1];
typedef char SceneSelectGroupPreviewQueueAt62D8[
    (offsetof(SceneSelectControllerView, groupPreviewQueue) == 0x62d8) ? 1 : -1];
typedef char SceneSelectScenePreviewQueueAt6320[
    (offsetof(SceneSelectControllerView, scenePreviewQueue) == 0x6320) ? 1 : -1];
typedef char SceneSelectStateHistoryAt63B0[
    (offsetof(SceneSelectControllerView, stateHistory) == 0x63b0) ? 1 : -1];

extern u8 g_SceneTextBuffer[0x40];
extern u32 g_SceneGroupColors[11];
extern u32 g_SceneLockedTransitionColor;
extern u32 g_SceneLockedInitialColor;

void __fastcall LoadSceneSelectionAssets(void *unused);

void __cdecl SceneWriteText(SceneAnmManagerView *manager,
                            SceneAnmVmView *vm, u32 color, u32 unknown,
                            const char *text);

} // namespace th095

#endif

#endif // TH095_MATCH_EXACT
