#pragma once

#include "../PhotoEnemyAnmVmIdStorage.hpp"

namespace th095
{
namespace EclRunHigh
{

// The three stored results in RunEcl copy a POD handle through the pointer
// returned in EAX. CreateVmAtWorld receives the hidden result pointer before
// the script and position, and returns that pointer. A canonical AnmVmId
// result at these sites changes the caller's copy order under VC7.1.
struct WorldVmSpawnEmission
{
    PhotoEnemyAnmVmIdStorage CreateVmAtWorld(i32 script, Float3 *position);
};

} // namespace EclRunHigh
} // namespace th095

#pragma comment(linker, "/alternatename:?CreateVmAtWorld@WorldVmSpawnEmission@EclRunHigh@th095@@QAE?AUPhotoEnemyAnmVmIdStorage@3@HPAUFloat3@3@@Z=?CreateVmAtWorld@AnmLoaded@th095@@QAE?AUAnmVmId@2@HPAUFloat3@2@@Z")

#define TH095_ECL_ANM_SPAWN_WORLD(anm, script, position) \
    reinterpret_cast<::th095::EclRunHigh::WorldVmSpawnEmission *>(anm) \
        ->CreateVmAtWorld((script), (position))
