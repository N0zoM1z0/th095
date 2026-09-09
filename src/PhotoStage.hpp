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

// Production-visible stage owner. Exact builds keep their frozen source view
// in PhotoStageExact.inl; this definition lets other production TUs call the
// real stage methods without inventing alternate decorated names.
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
        return reinterpret_cast<Float3 *>(reinterpret_cast<u8 *>(this) + 0x2c);
    }

    i32 &GetCaptureWidth()
    {
        return *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x38);
    }

    i32 &GetCaptureHeight()
    {
        return *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x3c);
    }

    i32 &GetCaptureSlot()
    {
        return *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x40);
    }
};

} // namespace th095

#endif
