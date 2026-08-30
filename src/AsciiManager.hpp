#pragma once
#include "AnmManager.hpp"
#include "diffbuild.hpp"

#define ASCII_MAX_STRINGS 256
#define ASCII_MAX_GUI_STRINGS 64
#define ASCII_MAX_SCORE_POPUPS 723
#define ASCII_MAX_TIME_POPUPS 128

namespace th095
{

struct AsciiManagerString
{
    AsciiManagerString()
    {
    }

    char text[64];
    Float3 position;
    D3DCOLOR color;
    f32 scaleX;
    f32 scaleY;
    i32 isSelected;
    i32 isGui;
};

C_ASSERT(sizeof(AsciiManagerString) == 0x60);

struct AsciiManagerPopup
{
    AsciiManagerPopup()
    {
    }

    char text[12];
    Float3 position;
    D3DCOLOR color;
    ZunTimer timer;
    Float2 scale;
    bool inUse;
    BYTE characterCount;
    u32 unconsumedDword34;
};

C_ASSERT(sizeof(AsciiManagerPopup) == 0x38);
C_ASSERT(offsetof(AsciiManagerPopup, characterCount) == 0x31);
C_ASSERT(offsetof(AsciiManagerPopup, unconsumedDword34) == 0x34);

struct AsciiManager
{
    AsciiManager();
    ~AsciiManager();
    static i32 OnUpdate(AsciiManager *mgr);
    static i32 OnDrawLowPrio(AsciiManager *mgr);
    static i32 OnDrawHighPrio(AsciiManager *mgr);
    static ZunResult RegisterChain();
    static ZunResult AddedCallback(AsciiManager *mgr);
    static ZunResult DeletedCallback(AsciiManager *mgr);
    static void CutChain();
    void AddString(Float3 *position, const char *string);
    void AddFormatText(Float3 *position, const char *fmt, ...);
    int AddFormatText2(Float3 *position, const char *fmt, ...);
    void UpdateVms();
    void OnDrawLowPrioImpl();
    void CreateScorePopup(Float3 *position, i32 number, D3DCOLOR color);
    void CreatePlayerPointPopup(Float3 *position, i32 number, D3DCOLOR color);
    void CreateTimePopup(Float3 *position, i32 primaryNumber, i32 secondaryNumber, D3DCOLOR color);
    void CreateFamiliarPopup(Float3 *position, i32 primaryNumber, i32 secondaryNumber, D3DCOLOR color);
    void OnDrawHighPrioImpl();
    void DrawPercentage(Float3 *position, i32 percentage, D3DCOLOR color);
    void SetBossMarkerInterrupt(i32 slot, i16 state);
    void SetBossMarkerState(i32 index, u32 value);
    void SetBossMarkerPosition(i32 slot, D3DXVECTOR3 *position);

    void Reset();
    void InitializeVms();

    void SetColor(D3DCOLOR color)
    {
        this->color.color = color;
    }

    void SetIsSelected(i32 selected)
    {
        this->isSelected = selected;
    }

    void SetScale(float scaleX, float scaleY);

    void SetGaugeInterrupt(i32 interrupt);

    i32 GetGaugeInterrupt();
    void ResetStrings();
    void SetSpaceWidth(i32 spaceWidth);

    void SetIsGuiMode(u32 value);

    AnmVm largeText;                                      // +0x0000
    AnmVm smallText;                                      // +0x02cc
    AnmVm popupText;                                      // +0x0598
    AsciiManagerString strings[ASCII_MAX_STRINGS];        // +0x0864
    AsciiManagerString guiStrings[ASCII_MAX_GUI_STRINGS]; // +0x6864
    i32 numStrings;                                       // +0x8064
    i32 numGuiStrings;                                    // +0x8068
    ZunColor color;                                       // +0x806c
    f32 scaleX;                                           // +0x8070
    f32 scaleY;                                           // +0x8074
    i32 isGui;                                            // +0x8078
    i32 isSelected;                                       // +0x807c
    u8 unknown8080[0x8098 - 0x8080];
    i32 spaceWidth;                                       // +0x8098
    u8 unknown809c[4];
    AnmLoaded *asciiAnm;                                  // +0x80a0
    AnmLoaded *captureAnm;                                // +0x80a4
    u8 unknown80a8[0x10];
    AnmVm auxiliaryVm;                                    // +0x80b8
    AsciiManagerPopup scorePopups[ASCII_MAX_SCORE_POPUPS];// +0x8384
    AsciiManagerPopup timePopups[ASCII_MAX_TIME_POPUPS];  // +0x121ac
};

C_ASSERT(sizeof(AsciiManager) == 0x13dac);
C_ASSERT(offsetof(AsciiManager, strings) == 0x0864);
C_ASSERT(offsetof(AsciiManager, guiStrings) == 0x6864);
C_ASSERT(offsetof(AsciiManager, color) == 0x806c);
C_ASSERT(offsetof(AsciiManager, asciiAnm) == 0x80a0);
C_ASSERT(offsetof(AsciiManager, auxiliaryVm) == 0x80b8);
C_ASSERT(offsetof(AsciiManager, scorePopups) == 0x8384);
C_ASSERT(offsetof(AsciiManager, timePopups) == 0x121ac);
DIFFABLE_EXTERN(AsciiManager, g_AsciiManager);

} // namespace th095
