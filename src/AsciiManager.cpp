#include "AsciiManager.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

namespace th095
{

struct AsciiBackgroundSupervisorView
{
    void ConfigureBackgroundViewport(i32 index);
};

struct AsciiAnmLoadedView : AnmLoaded
{
    void InitializeAndSetSprite(AnmVm *vm, i32 sprite)
    {
        vm->Initialize();
        vm->anmFile = this;
        this->SetSprite(vm, sprite);
    }
};

struct AsciiGlobalStateView
{
    u8 unknown000[0xfc];
    u32 active : 1;
    u32 unknownFlag1 : 1;
    u32 suppressStringReset : 1;
    u32 unknownFlags3 : 29;
};

struct AsciiStageStateView
{
    u8 unknown000[0x25720];
    u32 active : 1;
    u32 unknownFlag1 : 1;
    u32 suppressGuiStrings : 1;
    u32 unknownFlags3 : 29;
};

extern AsciiGlobalStateView *g_AsciiGlobalState;
extern AsciiStageStateView *g_AsciiStageState;

DIFFABLE_STATIC(ChainElem, g_AsciiManagerCalcChain);
DIFFABLE_STATIC(ChainElem, g_AsciiManagerDrawChainLowPrio);
DIFFABLE_STATIC(ChainElem, g_AsciiManagerDrawChainHighPrio);

static __forceinline i32 AsciiEitherFlag(i32 first, i32 second)
{
    return first | second;
}

// FUNCTION: TH095 0x00401000.
i32 AsciiManager::OnUpdate(AsciiManager *ascii)
{
    if (g_AsciiGlobalState != NULL &&
        AsciiEitherFlag(g_AsciiGlobalState->active,
                        g_AsciiGlobalState->suppressStringReset) != 0 &&
        g_AsciiGlobalState->suppressStringReset == 0)
    {
        return 1;
    }

    ascii->numStrings = 0;
    ascii->numGuiStrings = 0;
    ascii->frameCounter++;
    return 1;
}

// FUNCTION: TH095 0x00401090.
i32 AsciiManager::OnDrawLowPrio(AsciiManager *ascii)
{
    ascii->DrawStrings();
    return 1;
}

// FUNCTION: TH095 0x004010B0.
i32 AsciiManager::OnDrawHighPrio(AsciiManager *ascii)
{
    if (g_AsciiStageState != NULL &&
        g_AsciiStageState->suppressGuiStrings != 0)
    {
        return 1;
    }

    ascii->DrawGuiStrings();
    return 1;
}

// FUNCTION: TH095 0x00401D30.
AsciiManager::AsciiManager()
{
}

// FUNCTION: TH095 0x00402010.
AsciiManager::~AsciiManager()
{
}

// FUNCTION: TH095 0x004010F0.
void AsciiManager::Reset()
{
    memset(&this->smallText, 0, sizeof(AnmVm));
    memset(&this->popupText, 0, sizeof(AnmVm));
    memset(&this->largeText, 0, sizeof(AnmVm));
    memset(&this->strings, 0, sizeof(this->strings));
    memset(&this->guiStrings, 0, sizeof(this->guiStrings));
    memset(&this->scorePopups, 0, sizeof(this->scorePopups));
    memset(&this->timePopups, 0, sizeof(this->timePopups));

    this->numStrings = 0;
    this->isGui = 0;
    this->isSelected = 0;
    this->nextScorePopupIndex = 0;
    this->nextTimePopupIndex = 0;
    this->resetOnlyState80b4 = 0;
    this->color.color = 0xffffffff;
    this->scaleX = 1.0f;
    this->scaleY = 1.0f;

    reinterpret_cast<AsciiAnmLoadedView *>(this->asciiAnm)
        ->InitializeAndSetSprite(&this->smallText, 0);
    reinterpret_cast<AsciiAnmLoadedView *>(this->asciiAnm)
        ->InitializeAndSetSprite(&this->largeText, 32);
    this->smallText.position.z = 0.1f;
    this->isSelected = 0;
    this->spaceWidth = 9;
}

// FUNCTION: TH095 0x00401280.
ZunResult AsciiManager::RegisterChain()
{
    AsciiManager *ascii = &g_AsciiManager;

    g_AsciiManagerCalcChain.callback =
        reinterpret_cast<ChainCallback>(AsciiManager::OnUpdate);
    g_AsciiManagerCalcChain.addedCallback = NULL;
    g_AsciiManagerCalcChain.deletedCallback = NULL;
    g_AsciiManagerCalcChain.addedCallback =
        reinterpret_cast<ChainLifetimeCallback>(AsciiManager::AddedCallback);
    g_AsciiManagerCalcChain.deletedCallback =
        reinterpret_cast<ChainLifetimeCallback>(AsciiManager::DeletedCallback);
    g_AsciiManagerCalcChain.arg = ascii;
    if (g_Chain.AddToCalcChain(&g_AsciiManagerCalcChain, 1) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }

    g_AsciiManagerDrawChainLowPrio.callback =
        reinterpret_cast<ChainCallback>(AsciiManager::OnDrawLowPrio);
    g_AsciiManagerDrawChainLowPrio.addedCallback = NULL;
    g_AsciiManagerDrawChainLowPrio.deletedCallback = NULL;
    g_AsciiManagerDrawChainLowPrio.arg = ascii;
    g_Chain.AddToDrawChain(&g_AsciiManagerDrawChainLowPrio, 0x1c);

    g_AsciiManagerDrawChainHighPrio.callback =
        reinterpret_cast<ChainCallback>(AsciiManager::OnDrawHighPrio);
    g_AsciiManagerDrawChainHighPrio.addedCallback = NULL;
    g_AsciiManagerDrawChainHighPrio.deletedCallback = NULL;
    g_AsciiManagerDrawChainHighPrio.arg = ascii;
    g_Chain.AddToDrawChain(&g_AsciiManagerDrawChainHighPrio, 0x15);
    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00401360.
ZunResult AsciiManager::AddedCallback(AsciiManager *ascii)
{
    memset(ascii, 0, sizeof(AsciiManager));
    ascii->asciiAnm = g_AnmManager->LoadAnm(1, "ascii.anm");
    if (ascii->asciiAnm == NULL)
    {
        return ZUN_ERROR;
    }

    ascii->captureAnm = g_AnmManager->LoadAnm(3, "capture.anm");
    if (ascii->captureAnm == NULL)
    {
        return ZUN_ERROR;
    }

    ascii->Reset();
    ascii->InitializeVms();
    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x004013F0.
ZunResult AsciiManager::DeletedCallback(AsciiManager *)
{
    g_AnmManager->ReleaseAnm(1);
    g_AnmManager->ReleaseAnm(3);
    return ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00401420.
void AsciiManager::CutChain()
{
    g_Chain.Cut(&g_AsciiManagerCalcChain);
    g_Chain.Cut(&g_AsciiManagerDrawChainLowPrio);
}

// FUNCTION: TH095 0x00423430.
void AsciiManager::InitializeVms()
{
}

// FUNCTION: TH095 0x00401450.
void AsciiManager::AddString(Float3 *position, const char *string)
{
    AsciiManagerString *nextString;

    if (this->numStrings >= ASCII_MAX_STRINGS)
    {
        return;
    }

    nextString = &this->strings[this->numStrings];
    this->numStrings++;
    strcpy(nextString->text, string);
    nextString->position = *position;
    nextString->color = this->color.color;
    nextString->scaleX = this->scaleX;
    nextString->scaleY = this->scaleY;
    nextString->isGui = this->isGui;
}

// FUNCTION: TH095 0x00401530.
void AsciiManager::AddGuiString(Float3 *position, const char *string)
{
    AsciiManagerString *nextString;

    if (this->numGuiStrings >= ASCII_MAX_GUI_STRINGS)
    {
        return;
    }

    nextString = &this->guiStrings[this->numGuiStrings];
    this->numGuiStrings++;
    strcpy(nextString->text, string);
    nextString->position = *position;
    nextString->color = this->color.color;
    nextString->scaleX = this->scaleX;
    nextString->scaleY = this->scaleY;
    nextString->isGui = this->isGui;
}

// FUNCTION: TH095 0x00401610.
void AsciiManager::AddFormatText(Float3 *position, const char *fmt, ...)
{
    char buffer[512];
    va_list args;

    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    this->AddString(position, buffer);
    va_end(args);
}

// FUNCTION: TH095 0x00401660.
int AsciiManager::AddGuiFormatText(Float3 *position, const char *fmt, ...)
{
    char buffer[512];
    va_list args;

    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    this->AddGuiString(position, buffer);
    va_end(args);
    return strlen(buffer);
}

// FUNCTION: TH095 0x00401700.
#pragma var_order(spaceWidth, i, currentString, text, isGui)
void AsciiManager::DrawStrings()
{
    f32 spaceWidth;
    i32 i;
    AsciiManagerString *currentString;
    u8 *text;
    i32 isGui = 1;

    currentString = &this->strings[0];
    this->largeText.visible = true;
    this->largeText.renderStateA = 1;
    this->largeText.renderStateB = 1;

    for (i = 0; i < this->numStrings; i++, currentString++)
    {
        this->largeText.position = currentString->position;
        text = reinterpret_cast<u8 *>(currentString->text);
        this->largeText.scale.x = currentString->scaleX;
        this->largeText.scale.y = currentString->scaleY;
        spaceWidth = this->spaceWidth * currentString->scaleX;

        if (isGui != currentString->isGui)
        {
            isGui = currentString->isGui;
            g_AnmManager->FlushVertexBuffer();
            if (isGui != 0)
            {
                reinterpret_cast<AsciiBackgroundSupervisorView *>(&g_Supervisor)
                    ->ConfigureBackgroundViewport(0);
            }
            else
            {
                reinterpret_cast<AsciiBackgroundSupervisorView *>(&g_Supervisor)
                    ->ConfigureBackgroundViewport(1);
            }
        }

        while (*text != '\0')
        {
            if (*text == '\n')
            {
                this->largeText.position.y += 16.0f * currentString->scaleY;
                this->largeText.position.x = currentString->position.x;
            }
            else if (*text == ' ')
            {
                this->largeText.position.x += spaceWidth;
            }
            else
            {
                this->largeText.loadedSprite =
                    &this->asciiAnm->sprites[*text - ' '];
                this->largeText.color1.color = currentString->color;
                g_AnmManager->DrawNoRotation(&this->largeText);
                this->largeText.position.x += spaceWidth;
            }
            text++;
        }
    }

    if (isGui != 0)
    {
        g_AnmManager->FlushVertexBuffer();
        reinterpret_cast<AsciiBackgroundSupervisorView *>(&g_Supervisor)
            ->ConfigureBackgroundViewport(1);
    }
}

// FUNCTION: TH095 0x00401920.
#pragma var_order(spaceWidth, i, currentString, text, isGui)
void AsciiManager::DrawGuiStrings()
{
    f32 spaceWidth;
    i32 i;
    AsciiManagerString *currentString;
    u8 *text;
    i32 isGui = 1;

    currentString = &this->guiStrings[0];
    this->largeText.visible = true;
    this->largeText.renderStateA = 1;
    this->largeText.renderStateB = 1;

    for (i = 0; i < this->numGuiStrings; i++, currentString++)
    {
        this->largeText.position = currentString->position;
        text = reinterpret_cast<u8 *>(currentString->text);
        this->largeText.scale.x = currentString->scaleX;
        this->largeText.scale.y = currentString->scaleY;
        spaceWidth = this->spaceWidth * currentString->scaleX;

        if (isGui != currentString->isGui)
        {
            isGui = currentString->isGui;
            g_AnmManager->FlushVertexBuffer();
            if (isGui != 0)
            {
                reinterpret_cast<AsciiBackgroundSupervisorView *>(&g_Supervisor)
                    ->ConfigureBackgroundViewport(0);
            }
            else
            {
                reinterpret_cast<AsciiBackgroundSupervisorView *>(&g_Supervisor)
                    ->ConfigureBackgroundViewport(1);
            }
        }

        while (*text != '\0')
        {
            if (*text == '\n')
            {
                this->largeText.position.y += 16.0f * currentString->scaleY;
                this->largeText.position.x = currentString->position.x;
            }
            else if (*text == ' ')
            {
                this->largeText.position.x += spaceWidth;
            }
            else
            {
                this->largeText.loadedSprite =
                    &this->asciiAnm->sprites[*text - ' '];
                this->largeText.color1.color = currentString->color;
                if (this->largeText.scale.x == 1.0f)
                {
                    g_AnmManager->DrawNoRotation(&this->largeText);
                }
                else
                {
                    g_AnmManager->DrawNoRotationNoRound(&this->largeText);
                }
                this->largeText.position.x += spaceWidth;
            }
            text++;
        }
    }

    if (isGui != 0)
    {
        g_AnmManager->FlushVertexBuffer();
        reinterpret_cast<AsciiBackgroundSupervisorView *>(&g_Supervisor)
            ->ConfigureBackgroundViewport(1);
    }
}

} // namespace th095
