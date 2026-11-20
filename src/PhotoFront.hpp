#pragma once

#include <stddef.h>

#include "AnmManager.hpp"

namespace th095
{

class ChainElem;

struct PhotoFrontManagerView
{
    AnmVm vms[6];           // +0x0000
    AnmLoaded *frontAnm;    // +0x10c8
    ChainElem *calcChain;   // +0x10cc
    ChainElem *drawChain;   // +0x10d0

    PhotoFrontManagerView();
    ~PhotoFrontManagerView();

    static PhotoFrontManagerView *Create();
    void Destroy();
    i32 Initialize();
    i32 Update();
    i32 Draw();
    static i32 __fastcall OnUpdate(PhotoFrontManagerView *front);
    static i32 __fastcall OnDraw(PhotoFrontManagerView *front);
};

typedef char PhotoFrontManagerSizeIs10D4[
    (sizeof(PhotoFrontManagerView) == 0x10d4) ? 1 : -1];
typedef char PhotoFrontManagerAnmAt10C8[
    (offsetof(PhotoFrontManagerView, frontAnm) == 0x10c8) ? 1 : -1];

extern PhotoFrontManagerView *g_PhotoFrontManager;

} // namespace th095
