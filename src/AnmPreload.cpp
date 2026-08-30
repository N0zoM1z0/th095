#include "AnmManager.hpp"

#include <stdlib.h>

namespace th095
{

struct AnmRawEntryView
{
    i32 numSprites;
    i32 numScripts;
    u32 textureIdx;
    i32 width;
    i32 height;
    u32 format;
    u32 colorKey;
    u32 nameOffset;
    u32 spriteIdxOffset;
    u32 mipmapNameOffset;
    u32 version;
    u32 priority;
    u32 textureOffset;
    u8 hasData;
    u8 padding035[3];
    u32 nextOffset;
    u32 serializedReserved03c;
};

struct AnmRawSpriteView
{
    u32 id;
    f32 x;
    f32 y;
    f32 width;
    f32 height;
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
    i32 CreateTextureFromFile(AnmTextureEntryView *entry, i32 format,
                              i32 colorKey);
    i32 CreateTextureFromAnm(IDirect3DTexture8 **outTexture,
                             void *textureData, i32 format);
    i32 CreateEmptyTexture(IDirect3DTexture8 **outTexture, i32 width,
                           i32 height, i32 format);
    void ReleaseAnm(i32 anmIdx);
    void ReleaseAnmEntry(AnmTextureEntryView *entry);
    void MarkVmsForDeletion(AnmLoaded *anm);
    ZunResult ServicePreloadedAnims();
};

typedef char AnmPreloadSlotViewSizeIs120[(sizeof(AnmPreloadSlotView) == 0x120) ? 1 : -1];
typedef char AnmRawEntryViewNextAt38[(offsetof(AnmRawEntryView, nextOffset) == 0x38) ? 1 : -1];
typedef char AnmRawEntryViewSizeIs40[(sizeof(AnmRawEntryView) == 0x40) ? 1 : -1];
typedef char AnmTextureEntryViewSizeIs10[(sizeof(AnmTextureEntryView) == 0x10) ? 1 : -1];

// FUNCTION: TH095 0x004435A0.
i32 AnmManagerPreloadView::LoadTextureData(
    AnmLoaded *anm, i32 entryNumber, i32 currentSpriteNumber,
    i32 currentScriptNumber, AnmRawEntryView *rawEntry)
{
    struct LoadTextureState
    {
        AnmLoadedSprite loadedSprite;
        u32 loadedSpriteAlignment;
        u32 *currentOffset;
        i32 i;
        AnmRawSpriteView *rawSprite;
        const char *path;
        D3DSURFACE_DESC surfaceDesc;
        AnmRawEntryView *startOfEntry;
        i32 result;
    } state;

    state.result = 0;

    if (rawEntry == NULL)
    {
        g_GameErrorContext.Fatal(
            "\x83\x41\x83\x6a\x83\x81\x82\xaa\x93\xc7\x82\xdd\x8d\x9e"
            "\x82\xdf\x82\xdc\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x8e\xb8\x82\xed\x82\xea\x82\xc4\x82\xe9"
            "\x82\xa9\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7"
            "\r\n");
        return ZUN_ERROR;
    }

    state.startOfEntry = rawEntry;
    if (state.startOfEntry->version != 4)
    {
        g_GameErrorContext.Fatal(
            "\x83\x41\x83\x6a\x83\x81\x82\xcc\x83\x6f\x81\x5b\x83\x57"
            "\x83\x87\x83\x93\x82\xaa\x88\xe1\x82\xa2\x82\xdc\x82\xb7"
            "\r\n");
        return ZUN_ERROR;
    }

    if (!state.startOfEntry->hasData)
    {
        state.path = reinterpret_cast<const char *>(
            reinterpret_cast<u8 *>(state.startOfEntry) + state.startOfEntry->nameOffset);

        if (state.path[0] == '@')
        {
            this->CreateEmptyTexture(
                &reinterpret_cast<AnmTextureEntryView *>(anm->textures)[entryNumber].texture,
                state.startOfEntry->width, state.startOfEntry->height,
                state.startOfEntry->format);
        }
        else if (this->CreateTextureFromFile(
                     &reinterpret_cast<AnmTextureEntryView *>(anm->textures)[entryNumber],
                     state.startOfEntry->format,
                     state.startOfEntry->colorKey) != ZUN_SUCCESS)
        {
            g_GameErrorContext.Fatal(
                "\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83 %s \x82\xaa"
                "\x8d\xec\x90\xac\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1"
                "\x81\x42\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8e\xb8\x82\xed"
                "\x82\xea\x82\xc4\x82\xe9\x82\xa9\x89\xf3\x82\xea\x82\xc4"
                "\x82\xa2\x82\xdc\x82\xb7\r\n",
                state.path);
            return ZUN_ERROR;
        }
    }
    else if (this->CreateTextureFromAnm(
                 &reinterpret_cast<AnmTextureEntryView *>(anm->textures)[entryNumber].texture,
                 reinterpret_cast<u8 *>(state.startOfEntry) +
                     state.startOfEntry->textureOffset,
                 state.startOfEntry->format) != ZUN_SUCCESS)
    {
        g_GameErrorContext.Fatal(
            "\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x82\xaa\x93\xc7"
            "\x82\xdd\x8d\x9e\x82\xdf\x82\xdc\x82\xb9\x82\xf1\x81\x42"
            "\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8e\xb8\x82\xed\x82\xea"
            "\x82\xc4\x82\xe9\x82\xa9\x89\xf3\x82\xea\x82\xc4\x82\xa2"
            "\x82\xdc\x82\xb7\r\n");
        return ZUN_ERROR;
    }

    reinterpret_cast<AnmTextureEntryView *>(anm->textures)[entryNumber].texture->SetPriority(
        state.startOfEntry->priority);
    reinterpret_cast<AnmTextureEntryView *>(anm->textures)[entryNumber].texture->PreLoad();
    reinterpret_cast<AnmTextureEntryView *>(anm->textures)[entryNumber].texture->GetLevelDesc(
        0, &state.surfaceDesc);

    state.currentOffset = reinterpret_cast<u32 *>(
        reinterpret_cast<u8 *>(state.startOfEntry) + sizeof(AnmRawEntryView));
    for (state.i = 0; state.i < state.startOfEntry->numSprites;
         state.i++, state.currentOffset++)
    {
        state.rawSprite = reinterpret_cast<AnmRawSpriteView *>(
            reinterpret_cast<u8 *>(state.startOfEntry) + *state.currentOffset);

        state.loadedSprite.anmIdx = anm->anmIdx;
        state.loadedSprite.texture =
            reinterpret_cast<AnmTextureEntryView *>(anm->textures)[entryNumber].texture;
        state.loadedSprite.scaleFactor.x =
            state.surfaceDesc.Width / (f32)state.startOfEntry->width;
        state.loadedSprite.scaleFactor.y =
            state.surfaceDesc.Height / (f32)state.startOfEntry->height;
        state.loadedSprite.startPixelInclusive.x =
            state.rawSprite->x * state.loadedSprite.scaleFactor.x;
        state.loadedSprite.startPixelInclusive.y =
            state.rawSprite->y * state.loadedSprite.scaleFactor.y;
        state.loadedSprite.endPixelInclusive.x =
            (state.rawSprite->x + state.rawSprite->width) *
            state.loadedSprite.scaleFactor.x;
        state.loadedSprite.endPixelInclusive.y =
            (state.rawSprite->y + state.rawSprite->height) *
            state.loadedSprite.scaleFactor.y;
        state.loadedSprite.width = state.surfaceDesc.Width;
        state.loadedSprite.height = state.surfaceDesc.Height;

        anm->LoadSprite(currentSpriteNumber, &state.loadedSprite);
        currentSpriteNumber++;
    }

    for (state.i = 0; state.i < state.startOfEntry->numScripts;
         state.i++, state.currentOffset += 2)
    {
        anm->scripts[currentScriptNumber] = reinterpret_cast<AnmRawInstr *>(
            reinterpret_cast<u8 *>(state.startOfEntry) + state.currentOffset[1]);
        currentScriptNumber++;
    }

    return state.result + 1;
}

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

// FUNCTION: TH095 0x00443AC0.
void AnmManagerPreloadView::ReleaseAnmEntry(AnmTextureEntryView *entry)
{
    if (entry->texture != NULL)
    {
        entry->texture->Release();
        entry->texture = NULL;
    }
    if (entry->rawData != NULL)
    {
        g_AnmPreloadMemory.Free(entry->rawData);
    }
}

} // namespace th095
