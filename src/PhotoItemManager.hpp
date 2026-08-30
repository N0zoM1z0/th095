#pragma once

#include "AnmManager.hpp"

namespace th095
{

// TH095's live 150-slot photography charge-item system ("ItemInf").

struct ChainElem;

struct PhotoItemView
{
    AnmVm vm;                         // +0x000
    Float3 position;                  // +0x2cc
    Float3 velocity;                  // +0x2d8
    f32 acceleration;                 // +0x2e4
    ZunTimer timer;                   // +0x2e8
    i32 active;                       // +0x2f4

    PhotoItemView();
    ~PhotoItemView();
};

typedef char PhotoItemSizeIs2F8[
    (sizeof(PhotoItemView) == 0x2f8) ? 1 : -1];
typedef char PhotoItemPositionAt2CC[
    (offsetof(PhotoItemView, position) == 0x2cc) ? 1 : -1];
typedef char PhotoItemTimerAt2E8[
    (offsetof(PhotoItemView, timer) == 0x2e8) ? 1 : -1];

struct PhotoItemManagerView
{
    u32 unknown000000;                // +0x00000
    PhotoItemView items[150];         // +0x00004
    ChainElem *calcChain;             // +0x1bd54
    ChainElem *drawChain;             // +0x1bd58

    PhotoItemManagerView();
    ~PhotoItemManagerView();

    static PhotoItemManagerView *Create();
    void Destroy();
    i32 Initialize();
    static i32 __fastcall OnUpdate(PhotoItemManagerView *manager);
    static i32 __fastcall OnDraw(PhotoItemManagerView *manager);
    i32 Update();
    i32 Draw();
    i32 Spawn(i32 type, Float3 *position, u32 color);
};

typedef char PhotoItemManagerSizeIs1BD5C[
    (sizeof(PhotoItemManagerView) == 0x1bd5c) ? 1 : -1];
typedef char PhotoItemManagerCalcChainAt1BD54[
    (offsetof(PhotoItemManagerView, calcChain) == 0x1bd54) ? 1 : -1];

extern PhotoItemManagerView *g_ItemManager;

} // namespace th095
