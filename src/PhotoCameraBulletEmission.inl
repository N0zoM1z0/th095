// PhotoCamera's target object decorates the Background owner at 0x004BDD90
// and the BulletInf owner at 0x004BDD98 with one historical manager spelling.
// Keep that exact-only receiver surface out of the normal ownership model.
struct PhotoAnmSpawnerView
{
    void SpawnInto(PhotoAnmVmId *output, i32 script, Float3 *position);
};

struct PhotoBulletManagerView
{
    u8 unknown0000[0x1760];
    ZunColor photoColor;
    u8 unknown1764[0x27c5b0 - 0x1764];
    PhotoAnmSpawnerView *anmSpawner;

    void BeginPhotoCapture(const Float3 *position, const Float3 *size);
    void DespawnAllBullets();
    i32 CountNearbyTargets(const Float3 *position, f32 radius);
    void *CapturePhotoTargets(const Float3 *position, const Float3 *size);
};

typedef char PhotoCameraEmissionAnmAt27C5B0[
    (offsetof(PhotoBulletManagerView, anmSpawner) == 0x27c5b0) ? 1 : -1];

extern PhotoBulletManagerView *g_PhotoBulletManager;
