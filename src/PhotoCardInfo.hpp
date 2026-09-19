#ifndef TH095_PHOTO_CARD_INFO_HPP
#define TH095_PHOTO_CARD_INFO_HPP

#include "AnmVmId.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

namespace th095
{

class ChainElem;

enum PhotoCardInfoState
{
    PHOTO_CARD_INFO_STATE_ACTIVE = 0,
    PHOTO_CARD_INFO_STATE_FINISHING = 1,
};

// Canonical 0x68 CardInf allocation published through target 0x004BDD9C.
// EnemyInf +0x26AE28 is only an ECL-held session handle to this object; it is
// not CardInf storage and does not own the object's post-Show lifetime.
struct PhotoCardInfoView
{
    i32 unknown000;                 // +0x00
    AnmVmId backgroundVmId;         // +0x04
    AnmVmId textVmId;               // +0x08
    PhotoCardInfoState state;       // +0x0c
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

typedef char PhotoCardInfoStateSizeIs4[
    (sizeof(PhotoCardInfoState) == sizeof(i32)) ? 1 : -1];
typedef char PhotoCardInfoSizeIs68[
    (sizeof(PhotoCardInfoView) == 0x68) ? 1 : -1];
typedef char PhotoCardInfoStateAt0C[
    (offsetof(PhotoCardInfoView, state) == 0x0c) ? 1 : -1];
typedef char PhotoCardInfoTimerAt10[
    (offsetof(PhotoCardInfoView, timer) == 0x10) ? 1 : -1];
typedef char PhotoCardInfoTextAt20[
    (offsetof(PhotoCardInfoView, text) == 0x20) ? 1 : -1];
typedef char PhotoCardInfoChainsAt60[
    (offsetof(PhotoCardInfoView, calcChain) == 0x60 &&
     offsetof(PhotoCardInfoView, drawChain) == 0x64) ? 1 : -1];

extern PhotoCardInfoView *g_PhotoCardInfo;

} // namespace th095

#endif
