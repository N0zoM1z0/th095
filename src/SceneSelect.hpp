#ifndef TH095_SCENE_SELECT_HPP
#define TH095_SCENE_SELECT_HPP

#include "Global.hpp"
#include "AnmVmId.hpp"
#include <time.h>

namespace th095
{

struct SceneScoreEntryView
{
    u8 unknown000[0x10];
    i32 score;
    u8 unknown014[4];
    i32 detailScore;
    u8 unknown01c[0x20];
    union
    {
        i32 attemptCount;
        time_t captureTime;
    };
    i32 bestShotChecksum;
    i32 unlockScore;
    f32 slowRate;
    f32 successRate;
    union
    {
        u32 flags;
        struct
        {
            u32 captured : 1;
            u32 showSuccessRateMarker : 1;
            u32 unknownFlags : 30;
        };
    };
    u8 unknown054[0x0c];
};

struct SceneDefinitionView
{
    i32 scoreEntryIndex;
    i32 titleArgument1;
    i32 titleArgument2;
    u8 unknown00c[0x14];
    i8 groupDisplayValue;
    i8 sceneDisplayValue;
    u8 unknown022[2];
    i32 scoreRequirement;
    i32 titleTextId;
    i8 displayState;
    u8 unknown02d[3];
};

struct SceneValueQueue
{
    i32 values[16];
    i32 count;
    i32 capacity;

    i32 Push(i32 value);
    i32 Pop();
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

struct ReplayScanWorker
{
    uintptr_t handle;
    unsigned int threadId;
    i32 stopRequested;
    i32 active;
    u8 unknown0010[4];
    void (__fastcall *threadProc)(void *);

    ~ReplayScanWorker();
    void Stop();
    void Start(void (__fastcall *callback)(void *), void *argument);
};

struct SceneSupervisorView
{
    u8 unknown000[0x648];
    ReplayScanWorker replayScanWorker;
    u8 unknown660[4];
    CRITICAL_SECTION criticalSections[7];
    u8 lockCounts[7];

    void EnterCriticalSectionWrapper(i32 id)
    {
        EnterCriticalSection(&this->criticalSections[id]);
    }

    void LeaveCriticalSectionWrapper(i32 id)
    {
        LeaveCriticalSection(&this->criticalSections[id]);
    }

    ZunResult StartReplayScan(void (__fastcall *callback)(void *),
                              void *argument);
    void StopReplayScan();
};

struct SceneAnmVmView;
struct Float3;

struct SceneAnmVmId
{
    i32 value;

    SceneAnmVmView *GetVm();
    void SetInterrupt(i32 interrupt);
};

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

struct SceneTextureEntryView
{
    IDirect3DTexture8 *texture;
    u8 unknown004[0x0c];

    i32 Load(u8 *data, i32 *size, i32 format, i32 unknown, i32 hasAlpha);
    i32 LoadRegion(u8 *data, i32 *size, i32 format, i32 unknown,
                   i32 hasAlpha, i32 top);
    void Clear();
};

struct SceneAnmLoadedView
{
    u8 unknown000[0x14];
    SceneTextureEntryView *textures;

    void SetSprite(SceneAnmVmView *vm, i32 spriteIndex);
    SceneAnmVmId CreateVm(i32 scriptIndex, i32 renderMode);
    SceneAnmVmId CreateVm(i32 scriptIndex, Float3 *position);
};

struct SceneAnmVmIdArray
{
    SceneAnmVmId values[165];

    SceneAnmVmId &operator[](i32 index)
    {
        return this->values[index];
    }

    void SetInterrupt(i32 index, i32 interrupt)
    {
        g_SceneAnmManager->SetInterrupt(this->values[index], interrupt);
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
            g_SceneAnmManager->GetVm(this->vmIds.values[vmIndex]),
            spriteIndex);
    }

    void ShowDetailDigitInline(i32 vmIndex)
    {
        g_SceneAnmManager->GetVm(this->vmIds.values[vmIndex])->flagsWord |= 2;
    }

    void HideDetailDigitInline(i32 vmIndex)
    {
        g_SceneAnmManager->GetVm(this->vmIds.values[vmIndex])->flagsWord &= ~2;
    }

    char *ResolveSceneText(i32 textId, i32 column, i32 argument1,
                           i32 argument2);
};

struct SceneSaveDataView
{
    u8 unknown0000[0x1e];
    i16 lastSelectedGroup;
    i16 lastSelectedScene;
    u8 unknown0022[0x43e];
    SceneScoreEntryView sceneScores[120];

    ZunResult LoadScenePreviewTexture(SceneAnmLoadedView *anm,
                                      i32 textureIndex, i32 sceneIndex);
    i32 LoadBestShotForScene(i32 group, i32 scene);
    i32 IsSceneGroupUnlocked(i32 group);
    i32 FindHighestUnlockedSceneGroup();
    i32 CountCapturedScenes();
    i32 CountCapturedScenesInGroup(i32 group);
    i32 GetSceneGroupUnlockScore(i32 group);
};

typedef char SceneScoreEntrySizeIs60[
    (sizeof(SceneScoreEntryView) == 0x60) ? 1 : -1];
typedef char SceneScoreEntryScoreAt10[
    (offsetof(SceneScoreEntryView, score) == 0x10) ? 1 : -1];
typedef char SceneScoreEntryDetailScoreAt18[
    (offsetof(SceneScoreEntryView, detailScore) == 0x18) ? 1 : -1];
typedef char SceneScoreEntryUnlockScoreAt44[
    (offsetof(SceneScoreEntryView, unlockScore) == 0x44) ? 1 : -1];
typedef char SceneScoreEntryFlagsAt50[
    (offsetof(SceneScoreEntryView, flags) == 0x50) ? 1 : -1];
typedef char SceneScoreEntryRatesAt48[
    (offsetof(SceneScoreEntryView, slowRate) == 0x48 &&
     offsetof(SceneScoreEntryView, successRate) == 0x4c) ? 1 : -1];
typedef char SceneDefinitionSizeIs30[
    (sizeof(SceneDefinitionView) == 0x30) ? 1 : -1];
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
typedef char ReplayScanWorkerSizeIs18[
    (sizeof(ReplayScanWorker) == 0x18) ? 1 : -1];
typedef char SceneSupervisorReplayScanWorkerAt648[
    (offsetof(SceneSupervisorView, replayScanWorker) == 0x648) ? 1 : -1];
typedef char SceneSupervisorCriticalSectionsAt664[
    (offsetof(SceneSupervisorView, criticalSections) == 0x664) ? 1 : -1];
typedef char SceneSupervisorLockCountsAt70C[
    (offsetof(SceneSupervisorView, lockCounts) == 0x70c) ? 1 : -1];
typedef char SceneSaveDataScoresAt460[
    (offsetof(SceneSaveDataView, sceneScores) == 0x460) ? 1 : -1];
typedef char SceneSaveDataSelectionAt1E[
    (offsetof(SceneSaveDataView, lastSelectedGroup) == 0x1e &&
     offsetof(SceneSaveDataView, lastSelectedScene) == 0x20) ? 1 : -1];
typedef char SceneScoreEntryAttemptCountAt3C[
    (offsetof(SceneScoreEntryView, attemptCount) == 0x3c) ? 1 : -1];
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

extern SceneDefinitionView *g_SceneGroups[12];
extern i32 g_SceneGroupCounts[12];
extern i32 g_SceneUnlockScoreRequirements[12];
extern i32 g_SceneUnlockCaptureRequirements[12];
extern i32 g_SceneUnlockGroupCaptureRequirements[12];
extern SceneSaveDataView *g_SceneSaveData;
extern SceneDefinitionView *g_SelectedScene;
extern SceneSupervisorView g_SceneSupervisor;
extern SceneAnmLoadedView *g_SceneUiAnm;
extern SceneSelectControllerView *g_SceneSelectController;
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
