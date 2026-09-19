#ifndef TH095_PHOTO_CAMERA_MODE_HPP
#define TH095_PHOTO_CAMERA_MODE_HPP

namespace th095
{

enum PhotoCameraMode
{
    PHOTO_CAMERA_TRACKING = 0,
    PHOTO_CAMERA_CHARGING = 1,
    PHOTO_CAMERA_CAPTURED = 2,
    PHOTO_CAMERA_RECOVERING = 3,
    PHOTO_CAMERA_DISABLED = 4,
};

typedef char PhotoCameraModeSizeIs4[
    (sizeof(PhotoCameraMode) == 4) ? 1 : -1];

} // namespace th095

#endif
