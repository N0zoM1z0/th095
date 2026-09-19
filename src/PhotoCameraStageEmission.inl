// VC7 emission adapter for PhotoCamera only. The normal product consumes the
// complete PhotoStageStateView owner from PhotoStage.hpp.
struct PhotoStageStateView
{
    u8 unknown00000[0x25718];
    f32 scoreMultiplier;
    PhotoAnmLoadedView *anm;
    u32 flags;

    i32 SavePhoto(i32 slot, const Float3 *position, i32 width, i32 height,
                  i32 score, const i32 *scoreData);
};

typedef char PhotoCameraStageScoreMultiplierAt25718[
    (offsetof(PhotoStageStateView, scoreMultiplier) == 0x25718) ? 1 : -1];
