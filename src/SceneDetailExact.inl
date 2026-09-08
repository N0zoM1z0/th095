#include "SceneSelect.hpp"

namespace th095
{

#define SET_DETAIL_SPRITE(vmIndex, spriteIndex)                               \
    this->SetDetailDigitSpriteInline(vmIndex, spriteIndex)

void SceneSelectControllerView::UpdateSelectedSceneDetails()
{
    i32 value;
    tm *captureTime;

    if (g_SceneSaveData->sceneScores[this->selectedScoreEntryIndex]
            .captureTime == 0)
    {
        SET_DETAIL_SPRITE(0x4c, 0x34);
        SET_DETAIL_SPRITE(0x4d, 0x34);
        SET_DETAIL_SPRITE(0x4f, 0x34);
        SET_DETAIL_SPRITE(0x50, 0x34);
        SET_DETAIL_SPRITE(0x51, 0x34);
        SET_DETAIL_SPRITE(0x52, 0x34);
        SET_DETAIL_SPRITE(0x54, 0x34);
        SET_DETAIL_SPRITE(0x55, 0x34);
        SET_DETAIL_SPRITE(0x56, 0x34);
        SET_DETAIL_SPRITE(0x57, 0x34);
        SET_DETAIL_SPRITE(0x58, 0x34);
        SET_DETAIL_SPRITE(0x59, 0x34);
        SET_DETAIL_SPRITE(0x5a, 0x34);
        SET_DETAIL_SPRITE(0x5b, 0x34);
    }
    else
    {
        captureTime = localtime(
            &g_SceneSaveData->sceneScores[this->selectedScoreEntryIndex]
                 .captureTime);

        SET_DETAIL_SPRITE(0x4c, (captureTime->tm_mon + 1) / 10 + 0x27);
        SET_DETAIL_SPRITE(0x4d, (captureTime->tm_mon + 1) % 10 + 0x27);
        SET_DETAIL_SPRITE(0x4f, captureTime->tm_mday / 10 + 0x27);
        SET_DETAIL_SPRITE(0x50, captureTime->tm_mday % 10 + 0x27);
        SET_DETAIL_SPRITE(0x51, captureTime->tm_hour / 10 + 0x27);
        SET_DETAIL_SPRITE(0x52, captureTime->tm_hour % 10 + 0x27);
        SET_DETAIL_SPRITE(0x54, captureTime->tm_min / 10 + 0x27);
        SET_DETAIL_SPRITE(0x55, captureTime->tm_min % 10 + 0x27);

        value = g_SceneSaveData
                    ->sceneScores[this->selectedScoreEntryIndex]
                    .detailScore;
        SET_DETAIL_SPRITE(0x56, value / 100000 % 10 + 0x27);
        SET_DETAIL_SPRITE(0x57, value / 10000 % 10 + 0x27);
        SET_DETAIL_SPRITE(0x58, value / 1000 % 10 + 0x27);
        SET_DETAIL_SPRITE(0x59, value / 100 % 10 + 0x27);
        SET_DETAIL_SPRITE(0x5a, value / 10 % 10 + 0x27);
        SET_DETAIL_SPRITE(0x5b, value % 10 + 0x27);
    }

    value = g_SceneSaveData->sceneScores[this->selectedScoreEntryIndex]
                .unlockScore;
    if (value / 100000 != 0)
    {
        SET_DETAIL_SPRITE(0x5e, value / 100000 % 10 + 0x4b);
        this->ShowDetailDigitInline(0x5e);
    }
    else
    {
        this->HideDetailDigitInline(0x5e);
    }

    if (value / 10000 != 0)
    {
        SET_DETAIL_SPRITE(0x5f, value / 10000 % 10 + 0x4b);
        this->ShowDetailDigitInline(0x5f);
    }
    else
    {
        this->HideDetailDigitInline(0x5f);
    }

    if (value / 1000 != 0)
    {
        SET_DETAIL_SPRITE(0x60, value / 1000 % 10 + 0x4b);
        this->ShowDetailDigit(0x60);
    }
    else
    {
        this->HideDetailDigit(0x60);
    }

    if (value / 100 != 0)
    {
        this->SetDetailDigitSprite(0x61, value / 100 % 10 + 0x4b);
        this->ShowDetailDigit(0x61);
    }
    else
    {
        this->HideDetailDigit(0x61);
    }

    if (value / 10 != 0)
    {
        this->SetDetailDigitSprite(0x62, value / 10 % 10 + 0x4b);
        this->ShowDetailDigit(0x62);
    }
    else
    {
        this->HideDetailDigit(0x62);
    }

    this->SetDetailDigitSprite(0x63, value % 10 + 0x4b);
}

void SceneSelectControllerView::SetDetailDigitSprite(i32 vmIndex,
                                                      i32 spriteIndex)
{
    this->sceneAnm->SetSprite(
        g_SceneAnmManager->GetVm(this->vmIds.values[vmIndex]), spriteIndex);
}

void SceneSelectControllerView::ShowDetailDigit(i32 vmIndex)
{
    g_SceneAnmManager->GetVm(this->vmIds.values[vmIndex])->flagsWord |= 2;
}

void SceneSelectControllerView::HideDetailDigit(i32 vmIndex)
{
    g_SceneAnmManager->GetVm(this->vmIds.values[vmIndex])->flagsWord &= ~2;
}

#undef SET_DETAIL_SPRITE

} // namespace th095
