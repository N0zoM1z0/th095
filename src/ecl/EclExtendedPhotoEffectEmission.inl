#ifndef TH095_ECL_EXTENDED_PHOTO_EFFECT_EMISSION_INL
#define TH095_ECL_EXTENDED_PHOTO_EFFECT_EMISSION_INL

// Exact/DIFF receiver spelling for the canonical PhotoEffectManagerView owner.
// The prefix exposes listRoot.next at +0x08 and nextId at +0x58; normal
// production uses PhotoEffectRuntime.hpp directly.
struct ExtendedPhotoEffectManager
{
    u8 unknown000[8];
    ExtendedPhotoEffectNode *first;
    u8 unknown00c[0x4c];
    i32 nextId;
    i32 Spawn(i32 type, void *args);
};

typedef char ExtendedPhotoEffectManagerNextIdAt58[
    (offsetof(ExtendedPhotoEffectManager, nextId) == 0x58) ? 1 : -1];

#endif
