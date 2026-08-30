#include "AnmText.hpp"
#include "AnmVmId.hpp"
#include "utils.hpp"

#include <string.h>

namespace th095
{

struct PhotoCardAnmLoadedView
{
    AnmVmId CreateVm(i32 scriptIndex, Float3 *position);
};

struct PhotoCardStageStateView
{
    u8 unknown000[0x25720];
    u32 flags;
};

struct PhotoCardGameRuntimeView
{
    u8 unknown000[0x1e34];
    f32 hudFade;
};

struct PhotoCardGameTaskView
{
    u8 unknown000[0xfc];
    u32 flags;
};

struct PhotoCardInfoView
{
    i32 unknown000;                 // +0x00
    i32 backgroundVmId;             // +0x04
    i32 textVmId;                   // +0x08
    i32 state;                      // +0x0c
    ZunTimer timer;                 // +0x10
    u32 savedScreenFadeColor;       // +0x1c
    char text[0x30];                // +0x20
    u8 unknown050[0x10];            // +0x50
    ChainElem *calcChain;           // +0x60
    ChainElem *drawChain;           // +0x64

    PhotoCardInfoView();
    ~PhotoCardInfoView();

    i32 Initialize(char *encodedText);
    i32 Show();
    static PhotoCardInfoView *__fastcall Create(char *encodedText);
    void Destroy();
    i32 Update();
    i32 Draw();
    static i32 __fastcall OnUpdate(PhotoCardInfoView *cardInfo);
    static i32 __fastcall OnDraw(PhotoCardInfoView *cardInfo);
};

typedef char PhotoCardInfoSizeIs68[
    (sizeof(PhotoCardInfoView) == 0x68) ? 1 : -1];
typedef char PhotoCardInfoTimerAt10[
    (offsetof(PhotoCardInfoView, timer) == 0x10) ? 1 : -1];
typedef char PhotoCardInfoTextAt20[
    (offsetof(PhotoCardInfoView, text) == 0x20) ? 1 : -1];
typedef char PhotoCardInfoChainsAt60[
    (offsetof(PhotoCardInfoView, calcChain) == 0x60 &&
     offsetof(PhotoCardInfoView, drawChain) == 0x64) ? 1 : -1];

extern PhotoCardInfoView *g_PhotoCardInfo;
extern PhotoCardAnmLoadedView *g_PhotoCardBackgroundAnm;
extern PhotoCardAnmLoadedView *g_PhotoCardUiAnm;
extern PhotoCardStageStateView *g_PhotoCardStageState;
extern PhotoCardGameRuntimeView *g_PhotoCardGameRuntime;
extern PhotoCardGameTaskView *g_PhotoCardGameTask;
extern u32 g_PhotoScreenFadeColor;

// FUNCTION: TH095 0x00408610.
PhotoCardInfoView::PhotoCardInfoView()
{
    utils::DebugPrint("Ainitialize CardInf\n");
    memset(this, 0, sizeof(PhotoCardInfoView));
    g_PhotoCardInfo = this;
}

// FUNCTION: TH095 0x00408670.
i32 PhotoCardInfoView::Initialize(char *encodedText)
{
    Float3 textPosition;
    Float3 backgroundPosition;
    for (i32 index = 0; index < 0x30; ++index)
    {
        this->text[index] = encodedText[index] ^ 0xaa;
    }

    backgroundPosition.x = 0.0f;
    backgroundPosition.y = 0.0f;
    backgroundPosition.z = 0.0f;
    this->backgroundVmId =
        g_PhotoCardBackgroundAnm->CreateVm(1, &backgroundPosition).value;

    textPosition.x = 0.0f;
    textPosition.y = 0.0f;
    textPosition.z = 0.0f;
    this->textVmId =
        g_PhotoCardUiAnm->CreateVm(0x1e, &textPosition).value;

    reinterpret_cast<AnmTextManagerView *>(g_AnmManager)
        ->DrawTextRight(
            reinterpret_cast<AnmTextVmView *>(
                g_AnmManager->GetVm(
                    *reinterpret_cast<AnmVmId *>(&this->textVmId))),
            0xffffff, 0, this->text);
    this->savedScreenFadeColor = g_PhotoScreenFadeColor;
    return 0;
}

// FUNCTION: TH095 0x00408760.
PhotoCardInfoView::~PhotoCardInfoView()
{
    utils::DebugPrint("shutdown CardInf\n");
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);
    g_AnmManager->MarkVmForDeletion(
        *reinterpret_cast<AnmVmId *>(&this->backgroundVmId));
    g_AnmManager->MarkVmForDeletion(
        *reinterpret_cast<AnmVmId *>(&this->textVmId));
    g_PhotoCardInfo = NULL;
}

// FUNCTION: TH095 0x004087D0.
i32 PhotoCardInfoView::Show()
{
    g_AnmManager->SetInterrupt(
        *reinterpret_cast<AnmVmId *>(&this->backgroundVmId), 1);
    g_AnmManager->SetInterrupt(
        *reinterpret_cast<AnmVmId *>(&this->textVmId), 1);
    this->state = 1;
    this->timer = 0;
    g_PhotoScreenFadeColor = this->savedScreenFadeColor;
    return 0;
}

// FUNCTION: TH095 0x00408850.
PhotoCardInfoView *__fastcall PhotoCardInfoView::Create(char *encodedText)
{
    ChainElem *elem;
    PhotoCardInfoView *cardInfo;

    cardInfo = new PhotoCardInfoView();
    if (cardInfo->Initialize(encodedText) != 0)
    {
        goto failure;
    }

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoCardInfoView::OnUpdate));
    elem->arg = cardInfo;
    g_Chain.AddToCalcChain(elem, 0x10);
    cardInfo->calcChain = elem;

    elem = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(PhotoCardInfoView::OnDraw));
    elem->arg = cardInfo;
    g_Chain.AddToDrawChain(elem, 0x13);
    cardInfo->drawChain = elem;
    return cardInfo;

failure:
    if (cardInfo != NULL)
    {
        delete cardInfo;
        cardInfo = NULL;
    }
    return NULL;
}

// FUNCTION: TH095 0x00408990.
void PhotoCardInfoView::Destroy()
{
    PhotoCardInfoView *cardInfo = this;
    if (cardInfo != NULL)
    {
        delete cardInfo;
        cardInfo = NULL;
    }
}

// FUNCTION: TH095 0x004089F0.
i32 PhotoCardInfoView::Update()
{
    switch (this->state)
    {
    case 1:
        if (this->timer > 0x28)
        {
            return 0;
        }
        break;
    }

    if (this->state != 1 && this->timer >= 0x3c)
    {
        if (g_PhotoScreenFadeColor != 0)
        {
            this->savedScreenFadeColor = g_PhotoScreenFadeColor;
        }
        g_PhotoScreenFadeColor = 0;
    }

    i32 alpha = 0xff;
    if (((g_PhotoCardStageState->flags >> 2) & 1) != 0)
    {
        if (g_AnmManager->GetVm(
                *reinterpret_cast<AnmVmId *>(&this->backgroundVmId)) != NULL)
        {
            g_AnmManager->GetVm(
                *reinterpret_cast<AnmVmId *>(&this->backgroundVmId))
                ->flagsWord &= ~2u;
        }
        if (g_AnmManager->GetVm(
                *reinterpret_cast<AnmVmId *>(&this->textVmId)) != NULL)
        {
            g_AnmManager->GetVm(
                *reinterpret_cast<AnmVmId *>(&this->textVmId))
                ->flagsWord &= ~2u;
        }
    }
    else
    {
        if (g_AnmManager->GetVm(
                *reinterpret_cast<AnmVmId *>(&this->backgroundVmId)) != NULL)
        {
            g_AnmManager->GetVm(
                *reinterpret_cast<AnmVmId *>(&this->backgroundVmId))
                ->flagsWord |= 2;
        }
        if (g_AnmManager->GetVm(
                *reinterpret_cast<AnmVmId *>(&this->textVmId)) != NULL)
        {
            g_AnmManager->GetVm(
                *reinterpret_cast<AnmVmId *>(&this->textVmId))
                ->flagsWord |= 2;
        }

        if (g_PhotoCardGameRuntime->hudFade < 64.0f)
        {
            alpha = 0x40;
        }
        else if (g_PhotoCardGameRuntime->hudFade < 128.0f)
        {
            alpha =
                ((u32)(g_PhotoCardGameRuntime->hudFade - 64.0f) * 0xbf >> 6) +
                0x40;
        }
    }

    if (g_AnmManager->GetVm(
            *reinterpret_cast<AnmVmId *>(&this->backgroundVmId)) != NULL)
    {
        g_AnmManager->GetVm(
            *reinterpret_cast<AnmVmId *>(&this->backgroundVmId))
            ->color1.a = alpha;
    }
    if (g_AnmManager->GetVm(
            *reinterpret_cast<AnmVmId *>(&this->textVmId)) != NULL)
    {
        g_AnmManager->GetVm(
            *reinterpret_cast<AnmVmId *>(&this->textVmId))
            ->color1.a = alpha;
    }
    this->timer.Tick();
    return 1;
}

// FUNCTION: TH095 0x00408C50.
i32 PhotoCardInfoView::Draw()
{
    return 1;
}

// FUNCTION: TH095 0x00408C60.
i32 __fastcall PhotoCardInfoView::OnUpdate(PhotoCardInfoView *cardInfo)
{
    if (((g_PhotoCardGameTask->flags >> 2) & 1) != 0)
    {
        return 1;
    }
    if (((g_PhotoCardGameTask->flags >> 10) & 1) != 0)
    {
        return 1;
    }
    return cardInfo->Update();
}

// FUNCTION: TH095 0x00408CB0.
i32 __fastcall PhotoCardInfoView::OnDraw(PhotoCardInfoView *cardInfo)
{
    if (((g_PhotoCardGameTask->flags >> 2) & 1) != 0)
    {
        return 1;
    }
    return cardInfo->Draw();
}

} // namespace th095
