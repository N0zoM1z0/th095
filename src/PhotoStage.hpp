#ifndef TH095_PHOTO_STAGE_HPP
#define TH095_PHOTO_STAGE_HPP

#include "PhotoCamera.hpp"

#include <stddef.h>

namespace th095
{

struct PhotoStageCapture
{
    PhotoAnmVmId entryVms[11];
    Float3 position;
    i32 width;
    i32 height;
    i32 slot;
};

struct PhotoStageSlot
{
    AnmVm primaryVms[6];
    AnmVm overlayVms[6];
    i32 scoreData[8];
    i32 score;

    i32 timestamp;
    f32 slowRate;
    i32 width;
    i32 height;
    char comment[0x50];

    PhotoStageSlot();
    ~PhotoStageSlot();

    void Build(
        i32 score, Float3 *photoPosition, Float3 *entryPosition,
        const i32 *scoreData);
};

typedef char PhotoStageCaptureSizeIs44[
    (sizeof(PhotoStageCapture) == 0x44) ? 1 : -1];
typedef char PhotoStageCapturePositionAt2C[
    (offsetof(PhotoStageCapture, position) == 0x2c) ? 1 : -1];
typedef char PhotoStageCaptureWidthAt38[
    (offsetof(PhotoStageCapture, width) == 0x38) ? 1 : -1];
typedef char PhotoStageCaptureHeightAt3C[
    (offsetof(PhotoStageCapture, height) == 0x3c) ? 1 : -1];
typedef char PhotoStageCaptureSlotAt40[
    (offsetof(PhotoStageCapture, slot) == 0x40) ? 1 : -1];
typedef char PhotoStageSlotSizeIs2214[
    (sizeof(PhotoStageSlot) == 0x2214) ? 1 : -1];
typedef char PhotoStageSlotPrimaryVmsAt0[
    (offsetof(PhotoStageSlot, primaryVms) == 0) ? 1 : -1];
typedef char PhotoStageSlotOverlayVmsAt10C8[
    (offsetof(PhotoStageSlot, overlayVms) == 0x10c8) ? 1 : -1];
typedef char PhotoStageSlotScoreDataAt2190[
    (offsetof(PhotoStageSlot, scoreData) == 0x2190) ? 1 : -1];
typedef char PhotoStageSlotScoreAt21B0[
    (offsetof(PhotoStageSlot, score) == 0x21b0) ? 1 : -1];
typedef char PhotoStageSlotTimestampAt21B4[
    (offsetof(PhotoStageSlot, timestamp) == 0x21b4) ? 1 : -1];
typedef char PhotoStageSlotSlowRateAt21B8[
    (offsetof(PhotoStageSlot, slowRate) == 0x21b8) ? 1 : -1];
typedef char PhotoStageSlotWidthAt21BC[
    (offsetof(PhotoStageSlot, width) == 0x21bc) ? 1 : -1];
typedef char PhotoStageSlotHeightAt21C0[
    (offsetof(PhotoStageSlot, height) == 0x21c0) ? 1 : -1];
typedef char PhotoStageSlotCommentAt21C4[
    (offsetof(PhotoStageSlot, comment) == 0x21c4) ? 1 : -1];

// Canonical 0x25730 PhotoInf/stage owner.
struct PhotoStageStateView
{
    PhotoStageCapture capture;
    PhotoStageSlot slots[11];
    PhotoAnmVmId capturedPhotoVms[11];
    AnmVm displayVms[80];
    Float3 displayBoundary;
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

    Float3 *GetCapturePosition()
    {
        return &this->capture.position;
    }

    i32 &GetCaptureWidth()
    {
        return this->capture.width;
    }

    i32 &GetCaptureHeight()
    {
        return this->capture.height;
    }

    i32 &GetCaptureSlot()
    {
        return this->capture.slot;
    }
};

typedef char PhotoStageStateSizeIs25730[
    (sizeof(PhotoStageStateView) == 0x25730) ? 1 : -1];
typedef char PhotoStageSlotsAt44[
    (offsetof(PhotoStageStateView, slots) == 0x44) ? 1 : -1];
typedef char PhotoStageCapturedVmsAt17720[
    (offsetof(PhotoStageStateView, capturedPhotoVms) == 0x17720) ? 1 : -1];
typedef char PhotoStageDisplayVmsAt1774C[
    (offsetof(PhotoStageStateView, displayVms) == 0x1774c) ? 1 : -1];
typedef char PhotoStageDisplayBoundaryAt2570C[
    (offsetof(PhotoStageStateView, displayBoundary) == 0x2570c) ? 1 : -1];
typedef char PhotoStageFlagsAt25720[
    (offsetof(PhotoStageStateView, flags) == 0x25720) ? 1 : -1];
typedef char PhotoStageCaptureFrameAt25724[
    (offsetof(PhotoStageStateView, captureFrame) == 0x25724) ? 1 : -1];
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
