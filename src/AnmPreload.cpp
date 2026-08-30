#include "AnmManager.hpp"

namespace th095
{

struct AnmPreloadSlotView
{
    AnmLoaded loaded;
    i32 releasePending;
    u8 path[0x100];
};

struct AnmManagerPreloadView
{
    u8 unknown000[0x2c];
    AnmPreloadSlotView slots[13];

    AnmLoaded *PostloadAnmEntry(AnmLoaded *anm);
    void ReleaseAnm(i32 anmIdx);
    ZunResult ServicePreloadedAnims();
};

typedef char AnmPreloadSlotViewSizeIs120[(sizeof(AnmPreloadSlotView) == 0x120) ? 1 : -1];

// FUNCTION: TH095 0x004438E0.
ZunResult AnmManagerPreloadView::ServicePreloadedAnims()
{
    u32 i;

    for (i = 0; i < 13; i++)
    {
        if (this->slots[i].releasePending != 0)
        {
            this->ReleaseAnm(i);
            this->slots[i].releasePending = 0;
        }
        else if (this->slots[i].loaded.numberEntriesToBeLoaded != 0 &&
                 this->PostloadAnmEntry(&this->slots[i].loaded) == NULL)
        {
            return ZUN_ERROR;
        }
    }

    return ZUN_SUCCESS;
}

} // namespace th095
