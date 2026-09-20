// Compiler-emission adapter for PhotoCamera's script-0x124 spawn only.  The
// canonical storage and semantic receiver is AnmLoaded; spelling the call as
// CreateVmAtWorld makes VC7 construct a non-target AnmVmId return temporary.
// Both decorations resolve to the canonical target at 0x00445060.
namespace th095
{

struct PhotoAnmSpawnerView
{
    void SpawnInto(PhotoAnmVmId *output, i32 script, Float3 *position);
};

} // namespace th095

#pragma comment(linker, "/alternatename:?SpawnInto@PhotoAnmSpawnerView@th095@@QAEXPAUPhotoAnmVmId@2@HPAUFloat3@2@@Z=?CreateVmAtWorld@AnmLoaded@th095@@QAE?AUAnmVmId@2@HPAUFloat3@2@@Z")

#define TH095_PHOTO_BULLET_SPAWN_WORLD(anm, output, script, position) \
    reinterpret_cast<::th095::PhotoAnmSpawnerView *>(anm)->SpawnInto( \
        output, script, position)
