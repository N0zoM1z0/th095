#ifndef TH095_PHOTO_STAGE_HPP
#define TH095_PHOTO_STAGE_HPP

#include "PhotoCamera.hpp"

namespace th095
{

struct PhotoStageDisplayView
{
    AnmVm primaryVms[6];
    AnmVm overlayVms[6];
    i32 scoreData[8];
    i32 score;

    void Build(
        i32 score, Float3 *photoPosition, Float3 *entryPosition,
        const i32 *scoreData);
};

struct PhotoStageSlot
{
    PhotoAnmVmId entryVms[11];
    Float3 capturePosition;
    i32 captureWidth;
    i32 captureHeight;
    i32 captureSlot;
    PhotoStageDisplayView display;
    i32 timestamp;
    f32 slowRate;
    i32 width;
    i32 height;
    char comment[12];
};

// Canonical 0x25730 PhotoInf/stage owner. Exact-only source emission stays
// isolated in PhotoStageExact.inl and PhotoOverlayExact.inl.
struct PhotoStageStateView
{
    PhotoStageSlot slots[11];
    u8 unknown176dc[0x17720 - 0x176dc];
    PhotoAnmVmId capturedPhotoVms[11];
    AnmVm displayVms[80];
    f32 boundaryX;
    f32 boundaryY;
    i32 unknown25714;
    f32 scoreMultiplier;
    PhotoAnmLoadedView *anm;
    union
    {
        u32 flags;
        struct
        {
            u32 capturing : 1;
            u32 waitingForTexture : 1;
            u32 firstCaptureFrame : 1;
            u32 playerPassed : 1;
            u32 unknownFlags04 : 28;
        };
    };
    i32 captureFrame;
    ChainElem *calcChain;
    ChainElem *drawChain;

    PhotoStageStateView();
    ~PhotoStageStateView();

    static PhotoStageStateView *Create();
    void Destroy();
    i32 Initialize();
    i32 Draw();

    i32 Update();
    i32 SavePhoto(
        i32 slot, const Float3 *position, i32 width, i32 height,
        i32 score, const i32 *scoreData);
    i32 CapturePhotoPixels(i32 photoIndex);

    PhotoAnmVmId *GetEntryVms()
    {
        return reinterpret_cast<PhotoAnmVmId *>(this);
    }

    Float3 *GetCapturePosition()
    {
        return &this->slots[0].capturePosition;
    }

    i32 &GetCaptureWidth()
    {
        return this->slots[0].captureWidth;
    }

    i32 &GetCaptureHeight()
    {
        return this->slots[0].captureHeight;
    }

    i32 &GetCaptureSlot()
    {
        return this->slots[0].captureSlot;
    }
};

typedef char PhotoStageStateSizeIs25730[
    (sizeof(PhotoStageStateView) == 0x25730) ? 1 : -1];
typedef char PhotoStageScoreMultiplierAt25718[
    (offsetof(PhotoStageStateView, scoreMultiplier) == 0x25718) ? 1 : -1];
typedef char PhotoStageAnmAt2571C[
    (offsetof(PhotoStageStateView, anm) == 0x2571c) ? 1 : -1];
typedef char PhotoStageCalcChainAt25728[
    (offsetof(PhotoStageStateView, calcChain) == 0x25728) ? 1 : -1];
typedef char PhotoStageDrawChainAt2572C[
    (offsetof(PhotoStageStateView, drawChain) == 0x2572c) ? 1 : -1];

} // namespace th095

#endif
