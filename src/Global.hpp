#pragma once

#include "Chain.hpp"
#include "GameErrorContext.hpp"
#include "Rng.hpp"
#include "ZunResult.hpp"
#include "diffbuild.hpp"
#include "inttypes.hpp"
#if !defined(TH095_MATCH_EXACT) && !defined(DIFFBUILD)
#include "InputRuntime.hpp"
#endif
#include "pbg/PbgArchive.hpp"
#include "utils.hpp"
#include <d3dx8.h>
#include <stddef.h>
#include <windows.h>

// Private TH095 layout probe: dependent TH08 assertions are intentionally
// disabled while individual target offsets are migrated.
#undef C_ASSERT
#define C_ASSERT(expression)

namespace th095
{

extern u32 g_PhotoScreenFadeColor;

#define IS_PRESSED(key) (g_CurFrameInput & (key))
#define WAS_PRESSED(key) (((g_CurFrameInput & (key)) != 0) && (g_CurFrameInput & (key)) != (g_LastFrameInput & (key)))
#define WAS_PRESSED_SCROLLING(key)                                                                                     \
    (WAS_PRESSED(key) || (((g_CurFrameInput & (key)) != 0) && (g_IsEighthFrameOfHeldInput != 0)))

/* zunName is ZUN's original name for this type */
#define ZUN_NEW(type, zunName) ((type *)g_ZunMemory.AddToRegistry(new type(), sizeof(type), zunName))
#define ZUN_NEW_ARRAY(type, number, zunName)                                                                           \
    ((type *)g_ZunMemory.AddToRegistry(new type[number], sizeof(type) * number, zunName))
#define ZUN_DELETE(p)                                                                                                  \
    g_ZunMemory.RemoveFromRegistry(p);                                                                                 \
    delete p;                                                                                                          \
    p = NULL;
#define ZUN_DELETE2(p)                                                                                                 \
    delete p;                                                                                                          \
    p = NULL;

#define ZUN_FREE(p)                                                                                                    \
    g_ZunMemory.Free(p);                                                                                               \
    p = NULL;

enum TouhouButton
{
    TH_BUTTON_SHOOT = 1 << 0,
    TH_BUTTON_BOMB = 1 << 1,
    TH_BUTTON_FOCUS = 1 << 2,
    TH_BUTTON_MENU = 1 << 3,
    TH_BUTTON_UP = 1 << 4,
    TH_BUTTON_DOWN = 1 << 5,
    TH_BUTTON_LEFT = 1 << 6,
    TH_BUTTON_RIGHT = 1 << 7,
    TH_BUTTON_SKIP = 1 << 8,
    TH_BUTTON_Q = 1 << 9,
    TH_BUTTON_S = 1 << 10,
    TH_BUTTON_HOME = 1 << 11,
    TH_BUTTON_ENTER = 1 << 12,
    TH_BUTTON_D = 1 << 13,
    TH_BUTTON_RESET = 1 << 14,

    TH_BUTTON_UP_LEFT = TH_BUTTON_UP | TH_BUTTON_LEFT,
    TH_BUTTON_UP_RIGHT = TH_BUTTON_UP | TH_BUTTON_RIGHT,
    TH_BUTTON_DOWN_LEFT = TH_BUTTON_DOWN | TH_BUTTON_LEFT,
    TH_BUTTON_DOWN_RIGHT = TH_BUTTON_DOWN | TH_BUTTON_RIGHT,
    TH_BUTTON_DIRECTION = TH_BUTTON_DOWN | TH_BUTTON_RIGHT | TH_BUTTON_UP | TH_BUTTON_LEFT,

    TH_BUTTON_SELECTMENU = TH_BUTTON_ENTER | TH_BUTTON_SHOOT,
    TH_BUTTON_RETURNMENU = TH_BUTTON_MENU | TH_BUTTON_BOMB,
    TH_BUTTON_WRONG_CHEATCODE =
        TH_BUTTON_SHOOT | TH_BUTTON_BOMB | TH_BUTTON_MENU | TH_BUTTON_Q | TH_BUTTON_S | TH_BUTTON_ENTER,
    TH_BUTTON_ANY = 0xFFFF,
};

namespace Controller
{
u16 GetJoystickCaps();
u32 SetButtonFromControllerInputs(u16 *outButtons, i16 controllerButtonToTest, u16 touhouButton, u32 inputButtons);

u32 SetButtonFromDirectInputJoystate(u16 *outButtons, i16 controllerButtonToTest, u16 touhouButton, u8 *inputButtons);

u16 GetControllerInput(u16 buttons);
u8 *GetControllerState();
u16 GetInput();
void ResetKeyboard();
}; // namespace Controller

namespace FileSystem
{
LPBYTE Decrypt(LPBYTE inData, i32 size, u8 xorValue, u8 xorValueInc, i32 chunkSize, i32 maxBytes);
LPBYTE TryDecryptFromTable(LPBYTE inData, LPINT unused, i32 size);
LPBYTE Encrypt(LPBYTE inData, i32 size, u8 xorValue, u8 xorValueInc, i32 chunkSize, i32 maxBytes);
LPBYTE OpenFile(LPCSTR path, i32 *fileSize, BOOL isExternalResource);
BOOL CheckIfFileAlreadyExists(LPCSTR path);
int WriteDataToFile(LPCSTR path, LPVOID data, size_t size);
}; // namespace FileSystem

class ZunMemory
{
  public:
    ZunMemory();
    ~ZunMemory();

    // NOTE: the default parameter for debugText is probably just __FILE__
    void *Alloc(size_t size, const char *debugText = "d:\\cygwin\\home\\zun\\prog\\th08\\global.h")
    {
        return malloc(size);
    }

    void Free(void *ptr)
    {
        free(ptr);
    }

    void *AddToRegistry(void *ptr, size_t size, char *name)
    {
#ifdef DEBUG
        this->bRegistryInUse = TRUE;
        for (i32 i = 0; i < ARRAY_SIZE_SIGNED(this->registry); i++)
        {
            if (this->registry[i] == NULL)
            {
                RegistryInfo *info = (RegistryInfo *)malloc(sizeof(*info));
                if (info != NULL)
                {
                    info->data = ptr;
                    info->size = size;
                    info->name = name;
                    this->registry[i] = info;
                }
                break;
            }
        }
#endif
        return ptr;
    }

    void RemoveFromRegistry(VOID *ptr)
    {
#ifdef DEBUG
        for (i32 i = 0; i < ARRAY_SIZE_SIGNED(this->registry); i++)
        {
            if (this->registry[i] == ptr)
            {
                free(this->registry[i]);
                this->registry[i] = NULL;
                break;
            }
        }
#endif
    }

  private:
    struct RegistryInfo
    {
        void *data;
        size_t size;
        char *name;
    };

    RegistryInfo *registry[0x1000];
    BOOL bRegistryInUse;
};

struct ControllerButtonMapping
{
    i16 shotButton;
    i16 bombButton;
    i16 focusButton;
    i16 menuButton;
    i16 upButton;
    i16 downButton;
    i16 leftButton;
    i16 rightButton;
    i16 skipButton;
};

struct ZunGlobals
{
    u32 displayScore;
    i32 grazeInStage;
    u32 score;
    i32 graze;
    i32 scoreDisplayStep;
    u32 displayedHighScore;
    u8 continuesUsedInHighScore;
    /* 3 bytes pad */
    i32 spellcardsCaptured;
    i16 youkaiGaugeCopy;
    i16 youkaiGauge;
    i32 pointItemValue;
    i8 clockTime;
    u8 numRetries;
    /* 2 bytes pad */
    i32 pointItemsCollectedInStage;
    i32 pointItemsCollected;
    u32 pointItemExtendsSoFar;
    i32 nextPointItemExtendThreshold;
    i32 currentTimeOrbs;
    i32 lastSpellTimeOrbThreshold;
    i32 totalTimeOrbs;
    i32 rng1[7];
    f32 deaths;
    f32 deathInStage;
    f32 rng2[2];
    f32 livesRemaining;
    f32 rng3[2];
    f32 bombsRemaining;
    f32 bombsUsed;
    f32 bombsUsedInStage;
    f32 rng4[3];
    f32 playerPower;
    f32 rng5[2];
    i32 rng6;
    i32 rng7[8];
    u32 antiTamperValue;
    i32 antiTamperChecksum;
    i32 rng8[5];
};

C_ASSERT(sizeof(ZunGlobals) == 0xe4);
C_ASSERT(offsetof(ZunGlobals, displayScore) == 0x0);
C_ASSERT(offsetof(ZunGlobals, grazeInStage) == 0x4);
C_ASSERT(offsetof(ZunGlobals, score) == 0x8);
C_ASSERT(offsetof(ZunGlobals, graze) == 0xC);
C_ASSERT(offsetof(ZunGlobals, scoreDisplayStep) == 0x10);
C_ASSERT(offsetof(ZunGlobals, displayedHighScore) == 0x14);
C_ASSERT(offsetof(ZunGlobals, spellcardsCaptured) == 0x1C);
C_ASSERT(offsetof(ZunGlobals, youkaiGaugeCopy) == 0x20);
C_ASSERT(offsetof(ZunGlobals, youkaiGauge) == 0x22);
C_ASSERT(offsetof(ZunGlobals, pointItemValue) == 0x24);
C_ASSERT(offsetof(ZunGlobals, clockTime) == 0x28);
C_ASSERT(offsetof(ZunGlobals, numRetries) == 0x29);
C_ASSERT(offsetof(ZunGlobals, pointItemsCollectedInStage) == 0x2C);
C_ASSERT(offsetof(ZunGlobals, pointItemsCollected) == 0x30);
C_ASSERT(offsetof(ZunGlobals, pointItemExtendsSoFar) == 0x34);
C_ASSERT(offsetof(ZunGlobals, currentTimeOrbs) == 0x3C);
C_ASSERT(offsetof(ZunGlobals, lastSpellTimeOrbThreshold) == 0x40);
C_ASSERT(offsetof(ZunGlobals, totalTimeOrbs) == 0x44);
C_ASSERT(offsetof(ZunGlobals, deaths) == 0x64);
C_ASSERT(offsetof(ZunGlobals, deathInStage) == 0x68);
C_ASSERT(offsetof(ZunGlobals, livesRemaining) == 0x74);
C_ASSERT(offsetof(ZunGlobals, bombsRemaining) == 0x80);
C_ASSERT(offsetof(ZunGlobals, bombsUsed) == 0x84);
C_ASSERT(offsetof(ZunGlobals, bombsUsedInStage) == 0x88);
C_ASSERT(offsetof(ZunGlobals, playerPower) == 0x98);

DIFFABLE_EXTERN(u16, g_CurFrameInput);
DIFFABLE_EXTERN(u16, g_LastFrameInput);
DIFFABLE_EXTERN(u16, g_NumOfFramesInputsWereHeld);
DIFFABLE_EXTERN(u16, g_IsEighthFrameOfHeldInput);
DIFFABLE_EXTERN(u16, g_ResultMenuInput);
DIFFABLE_EXTERN(u16, g_PressedButtons);
DIFFABLE_EXTERN(PbgArchive, g_PbgArchive);
DIFFABLE_EXTERN(ZunMemory, g_ZunMemory);

i32 IsResourceReloadEnabled();
}; // namespace th095
