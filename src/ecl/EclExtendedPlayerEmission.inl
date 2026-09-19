// Exact-emission adapter for the historical EclExtended receiver and global
// spellings.  It deliberately owns no Player or camera storage layout.
struct ExtendedPhotoCameraView
{
    i32 CountPhotoTargets(f32 *closestDistance, f32 *bossRate);
};

struct ExtendedPlayerView;
extern ExtendedPlayerView *g_Player;

#define TH095_EXT_PLAYER_TYPE ExtendedPlayerView
#define TH095_EXT_PLAYER_STORAGE(player) \
    reinterpret_cast<::th095::PhotoPlayerRuntimeView *>(player)
#define TH095_EXT_CAMERA_METHOD(camera) \
    reinterpret_cast<ExtendedPhotoCameraView *>(&(camera))
