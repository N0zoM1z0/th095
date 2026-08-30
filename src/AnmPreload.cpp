#include "AnmManager.hpp"

#include <stdlib.h>

namespace th095
{

struct AnmRawEntryView
{
    i32 numSprites;
    i32 numScripts;
    u8 unknown008[0x30];
    u32 nextOffset;
};

struct AnmTextureEntryView
{
    IDirect3DTexture8 *texture;
    u8 *rawData;
    i32 size;
    i32 unknown00c;
};

class AnmPreloadMemoryView
{
  public:
    void Free(void *ptr)
    {
        free(ptr);
    }
};

extern AnmPreloadMemoryView g_AnmPreloadMemory;

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
    i32 LoadTextureData(AnmLoaded *anm, i32 entryNumber, i32 spriteCount,
                        i32 scriptCount, AnmRawEntryView *rawEntry);
    void ReleaseAnm(i32 anmIdx);
    void ReleaseAnmEntry(AnmTextureEntryView *entry);
    void MarkVmsForDeletion(AnmLoaded *anm);
    ZunResult ServicePreloadedAnims();
};

typedef char AnmPreloadSlotViewSizeIs120[(sizeof(AnmPreloadSlotView) == 0x120) ? 1 : -1];
typedef char AnmRawEntryViewNextAt38[(offsetof(AnmRawEntryView, nextOffset) == 0x38) ? 1 : -1];
typedef char AnmTextureEntryViewSizeIs10[(sizeof(AnmTextureEntryView) == 0x10) ? 1 : -1];

// FUNCTION: TH095 0x00443480.
AnmLoaded *AnmManagerPreloadView::PostloadAnmEntry(AnmLoaded *anm)
{
    struct PostloadState
    {
        AnmRawEntryView *rawEntry;
        i32 currentNumScripts;
        i32 result;
        AnmRawEntryView *rawData;
        i32 entryLoadNumber;
        i32 currentNumSprites;
        i32 currentEntryNumber;
    } state;

    utils::DebugPrint("::postloadAnim : %d, %d\n", anm->anmIdx,
                      anm->numberEntriesToBeLoaded);

    state.rawData = reinterpret_cast<AnmRawEntryView *>(anm->rawData);
    state.entryLoadNumber = 0;
    state.currentNumScripts = 0;
    state.currentNumSprites = 0;
    state.currentEntryNumber = 0;
    anm->rawData = state.rawData;
    state.rawEntry = state.rawData;

    while (true)
    {
        if (state.entryLoadNumber == anm->numberEntriesToBeLoaded - 1 &&
            (state.result = this->LoadTextureData(
                 anm, state.currentEntryNumber, state.currentNumSprites,
                 state.currentNumScripts, state.rawEntry)) < 0)
        {
            anm->numberEntriesToBeLoaded = 0;
            return NULL;
        }

        state.currentNumSprites += state.rawEntry->numSprites;
        state.currentNumScripts += state.rawEntry->numScripts;
        state.currentEntryNumber++;
        if (state.rawEntry->nextOffset == 0)
            break;

        state.rawEntry = reinterpret_cast<AnmRawEntryView *>(
            reinterpret_cast<u8 *>(state.rawEntry) + state.rawEntry->nextOffset);
        state.entryLoadNumber++;
        if (state.entryLoadNumber == anm->numberEntriesToBeLoaded)
        {
            anm->numberEntriesToBeLoaded++;
            return anm;
        }
    }

    anm->numberEntriesToBeLoaded = 0;
    return anm;
}

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

// FUNCTION: TH095 0x00443980.
void AnmManagerPreloadView::ReleaseAnm(i32 anmIdx)
{
    i32 i;

    if (anmIdx < 0 || anmIdx >= sizeof(this->slots) / sizeof(this->slots[0]))
        return;

    if (this->slots[anmIdx].loaded.rawData != NULL)
    {
        this->MarkVmsForDeletion(&this->slots[anmIdx].loaded);
        for (i = 0; i < this->slots[anmIdx].loaded.totalEntries; i++)
        {
            this->ReleaseAnmEntry(
                &reinterpret_cast<AnmTextureEntryView *>(
                    this->slots[anmIdx].loaded.textures)[i]);
        }

        g_AnmPreloadMemory.Free(this->slots[anmIdx].loaded.textures);
        g_AnmPreloadMemory.Free(this->slots[anmIdx].loaded.sprites);
        g_AnmPreloadMemory.Free(this->slots[anmIdx].loaded.scripts);
        g_AnmPreloadMemory.Free(this->slots[anmIdx].loaded.rawData);
        memset(&this->slots[anmIdx], 0, sizeof(AnmPreloadSlotView));
    }
}

} // namespace th095
