#ifndef TH095_OPTIONS_MENU_HPP
#define TH095_OPTIONS_MENU_HPP

#include "ReplayBrowser.hpp"

namespace th095
{

struct OptionsControllerBinding
{
    i16 button00;
    i16 button02;
    i16 unknown04;
    i16 button06;
    u8 unknown08[0x0a];
};

struct OptionsGameConfigView
{
    OptionsControllerBinding controllerBinding;
    u8 unknown012[0x9d];
    u8 windowed;
    u8 unknown0b0[5];
    i8 bgmVolume;
    i8 sfxVolume;
};

struct OptionsControllerMappingView
{
    OptionsControllerBinding primaryBinding;
};

extern OptionsGameConfigView g_OptionsGameConfig;
extern OptionsControllerMappingView g_OptionsControllerMapping;
extern i16 g_OptionsLastJoystickButton;

struct OptionsMenuView
{
    SceneAnmLoadedView *sceneAnm;
    SceneAnmLoadedView *transitionAnm;
    ResultScreenTimer stateTimer;
    i32 unknown0014;
    i32 unknown0018;
    i32 frameCounter;
    ResultScreenReplayCursor cursor;
    u8 unknown00f8[0xafc];
    SceneAnmVmIdArray vmIds;
    u8 unknown0e88[0x1c];
    i8 savedWindowed;
    u8 unknown0ea5[0x143];
    OptionsControllerBinding controllerBinding;
    u8 unknown0ffa[0x5106];
    AnmVmId transitionVm;
    u8 unknown6104[8];
    i32 state;
    i32 requestedState;
    u8 unknown6114[0x0c];
    u32 outerFlags;

    ChainCallbackResult Update();

    __forceinline void SetDigitSprite(i32 vmIndex, i32 digit)
    {
        this->sceneAnm->SetSprite(
            g_SceneAnmManager->GetVm(this->vmIds[vmIndex]),
            digit + 0x66);
    }

    __forceinline void UpdateButtonSprites(i32 firstVm, i32 value)
    {
        i32 tensSprite = value / 10 + 0x66;
        i32 onesSprite = value % 10 + 0x66;

        this->sceneAnm->SetSprite(
            g_SceneAnmManager->GetVm(this->vmIds[firstVm]),
            tensSprite);
        this->sceneAnm->SetSprite(
            g_SceneAnmManager->GetVm(this->vmIds[firstVm + 1]),
            onesSprite);
    }

    __forceinline void UpdateVolumeSprites(i32 firstVm, i32 value)
    {
        if (value < 100)
        {
            g_SceneAnmManager->GetVm(
                this->vmIds[firstVm])->flagsWord &= ~2;
        }
        else
        {
            g_SceneAnmManager->GetVm(
                this->vmIds[firstVm])->flagsWord |= 2;
            this->SetDigitSprite(firstVm, (value / 100) % 10);
        }

        if (value < 10)
        {
            g_SceneAnmManager->GetVm(
                this->vmIds[firstVm + 1])->flagsWord &= ~2;
        }
        else
        {
            g_SceneAnmManager->GetVm(
                this->vmIds[firstVm + 1])->flagsWord |= 2;
            this->SetDigitSprite(firstVm + 1, (value / 10) % 10);
        }
        this->SetDigitSprite(firstVm + 2, value % 10);
    }

    __forceinline void UpdateWindowModeSprites()
    {
        if (g_OptionsGameConfig.windowed == 0)
        {
            this->vmIds.SetInterrupt(0x78, 4);
            this->vmIds.SetInterrupt(0x79, 5);
        }
        else
        {
            this->vmIds.SetInterrupt(0x78, 5);
            this->vmIds.SetInterrupt(0x79, 4);
        }
    }

    __forceinline void UpdateSelectionSprites()
    {
        this->vmIds.SetInterrupt(0x6b, (this->cursor.GetCurrent() != 0) + 2);
        this->vmIds.SetInterrupt(0x6c, (this->cursor.GetCurrent() != 1) + 2);
        this->vmIds.SetInterrupt(0x6d, (this->cursor.GetCurrent() != 2) + 2);
        this->vmIds.SetInterrupt(0x6e, (this->cursor.GetCurrent() != 3) + 2);
        this->vmIds.SetInterrupt(0x6f, (this->cursor.GetCurrent() != 4) + 2);
        this->vmIds.SetInterrupt(0x70, (this->cursor.GetCurrent() != 5) + 2);
        this->vmIds.SetInterrupt(0x71, (this->cursor.GetCurrent() != 6) + 2);
        this->vmIds.SetInterrupt(0x72, (this->cursor.GetCurrent() != 0) + 2);
        this->vmIds.SetInterrupt(0x73, (this->cursor.GetCurrent() != 0) + 2);
        this->vmIds.SetInterrupt(0x74, (this->cursor.GetCurrent() != 1) + 2);
        this->vmIds.SetInterrupt(0x75, (this->cursor.GetCurrent() != 1) + 2);
        this->vmIds.SetInterrupt(0x76, (this->cursor.GetCurrent() != 2) + 2);
        this->vmIds.SetInterrupt(0x77, (this->cursor.GetCurrent() != 2) + 2);
        this->vmIds.SetInterrupt(0x78, (this->cursor.GetCurrent() != 3) + 2);
        this->vmIds.SetInterrupt(0x79, (this->cursor.GetCurrent() != 3) + 2);
        this->vmIds.SetInterrupt(0x7a, (this->cursor.GetCurrent() != 4) + 2);
        this->vmIds.SetInterrupt(0x7b, (this->cursor.GetCurrent() != 4) + 2);
        this->vmIds.SetInterrupt(0x7c, (this->cursor.GetCurrent() != 4) + 2);
        this->vmIds.SetInterrupt(0x7d, (this->cursor.GetCurrent() != 4) + 2);
        this->vmIds.SetInterrupt(0x7e, (this->cursor.GetCurrent() != 5) + 2);
        this->vmIds.SetInterrupt(0x7f, (this->cursor.GetCurrent() != 5) + 2);
        this->vmIds.SetInterrupt(0x80, (this->cursor.GetCurrent() != 5) + 2);
        this->vmIds.SetInterrupt(0x81, (this->cursor.GetCurrent() != 5) + 2);
    }
};

typedef char OptionsControllerBindingSizeIs12[
    (sizeof(OptionsControllerBinding) == 0x12) ? 1 : -1];
typedef char OptionsGameConfigWindowedAtAF[
    (offsetof(OptionsGameConfigView, windowed) == 0xaf) ? 1 : -1];
typedef char OptionsGameConfigVolumesAtB5[
    (offsetof(OptionsGameConfigView, bgmVolume) == 0xb5 &&
     offsetof(OptionsGameConfigView, sfxVolume) == 0xb6) ? 1 : -1];
typedef char OptionsMenuCursorAt20[
    (offsetof(OptionsMenuView, cursor) == 0x20) ? 1 : -1];
typedef char OptionsMenuVmIdsAtBF4[
    (offsetof(OptionsMenuView, vmIds) == 0xbf4) ? 1 : -1];
typedef char OptionsMenuSavedWindowedAtEA4[
    (offsetof(OptionsMenuView, savedWindowed) == 0xea4) ? 1 : -1];
typedef char OptionsMenuControllerBindingAtFE8[
    (offsetof(OptionsMenuView, controllerBinding) == 0xfe8) ? 1 : -1];
typedef char OptionsMenuTransitionVmAt6100[
    (offsetof(OptionsMenuView, transitionVm) == 0x6100) ? 1 : -1];
typedef char OptionsMenuStateAt610C[
    (offsetof(OptionsMenuView, state) == 0x610c) ? 1 : -1];
typedef char OptionsMenuRequestedStateAt6110[
    (offsetof(OptionsMenuView, requestedState) == 0x6110) ? 1 : -1];
typedef char OptionsMenuOuterFlagsAt6120[
    (offsetof(OptionsMenuView, outerFlags) == 0x6120) ? 1 : -1];

} // namespace th095

#endif
