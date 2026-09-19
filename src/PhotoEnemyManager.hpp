#pragma once

#include "AnmManager.hpp"
#include "ZunTimer.hpp"
#include "inttypes.hpp"

#include <stddef.h>

namespace th095
{

class ChainElem;
struct PhotoCardInfoView;
struct PhotoEnemyEclManagerView;
struct PhotoEnemyView;

// The compact TH095 enemy element is still being recovered in
// EnemyManagerUpdate.cpp.  This wrapper gives the canonical manager one
// honest inline owner for each target-proven 0x4CC0 slot without publishing a
// second partial element layout.  Its out-of-line normal-build constructor
// and destructor preserve the embedded PhotoEnemyView lifetime.
struct PhotoEnemySlotStorage
{
    u32 words[0x4cc0 / sizeof(u32)];

    PhotoEnemySlotStorage();
    ~PhotoEnemySlotStorage();

    PhotoEnemyView *Get()
    {
        return reinterpret_cast<PhotoEnemyView *>(this);
    }

    const PhotoEnemyView *Get() const
    {
        return reinterpret_cast<const PhotoEnemyView *>(this);
    }
};

struct PhotoEnemyTimelineView
{
    ZunTimer timer;
    void *instruction;

    PhotoEnemyTimelineView();
    void Run();
};

// Canonical TH095 EnemyInf allocation.  The factory at 0x004149F0 allocates
// exactly 0x26AE30 bytes and publishes this receiver through target global
// 0x004BDDC0.  The neighboring pointer-sized storage at +0x4DFC has consumers
// but no independently proved producer or lifetime, so it deliberately stays
// opaque rather than being named as a second ANM owner.
struct PhotoEnemyManagerView
{
    PhotoEnemySlotStorage spawnTemplate;       // +0x000000
    PhotoEnemyTimelineView timelines[16];      // +0x004cc0
    PhotoEnemyView *drawGroupHeads[4];         // +0x004dc0
    u8 unknown4dd0[4];
    i32 timelineEventSlots[4];                 // +0x004dd4
    PhotoEnemyView *timelineEnemySlots[4];     // +0x004de4
    PhotoEnemyEclManagerView *eclManager;      // +0x004df4
    AnmLoaded *enemyAnm;                       // +0x004df8
    u8 unknown4dfc[4];                         // +0x004dfc
    PhotoEnemySlotStorage enemyPool[128];      // +0x004e00
    PhotoEnemyView *photoTargets[8];           // +0x26ae00
    ChainElem *calcChain;                      // +0x26ae20
    ChainElem *drawChain;                      // +0x26ae24
    PhotoCardInfoView *eclPhotoCardSession;    // +0x26ae28
    i32 activeEnemyCount;                      // +0x26ae2c

    PhotoEnemyManagerView();
    ~PhotoEnemyManagerView();
    void Destroy();
    i32 LoadResources();
    static i32 __fastcall OnUpdate(PhotoEnemyManagerView *enemyManager);
    PhotoEnemyView *Spawn(
        i32 subroutineId,
        const Float3 *position,
        i32 life,
        i32 itemDrop,
        i32 score,
        u32 mirrorMovementX);
    PhotoEnemyView *SpawnWithContext(
        i32 subroutineId,
        const Float3 *position,
        i32 life,
        i32 itemDrop,
        i32 score,
        const i32 *contextValues);
    i32 CountPhotoTargets(const Float3 *position, const Float3 *size);
    static void __fastcall ResetNonPhotoTargets(
        PhotoEnemyManagerView *enemyManager);
    static void __fastcall RestartPhotoTargetEcls(
        PhotoEnemyManagerView *enemyManager);
    static void __fastcall ResetNonPhotoTargetsAndPhotoTargetEcls(
        PhotoEnemyManagerView *enemyManager);

    PhotoEnemyView *SpawnTemplate()
    {
        return this->spawnTemplate.Get();
    }

    PhotoEnemyView *EnemyAt(i32 index)
    {
        return this->enemyPool[index].Get();
    }
};

typedef char PhotoEnemySlotStorageSizeIs4CC0[
    (sizeof(PhotoEnemySlotStorage) == 0x4cc0) ? 1 : -1];
typedef char PhotoEnemyTimelineSizeIs10[
    (sizeof(PhotoEnemyTimelineView) == 0x10) ? 1 : -1];
typedef char PhotoEnemyManagerTimelinesAt4CC0[
    (offsetof(PhotoEnemyManagerView, timelines) == 0x4cc0) ? 1 : -1];
typedef char PhotoEnemyManagerDrawGroupsAt4DC0[
    (offsetof(PhotoEnemyManagerView, drawGroupHeads) == 0x4dc0) ? 1 : -1];
typedef char PhotoEnemyManagerTimelineEventsAt4DD4[
    (offsetof(PhotoEnemyManagerView, timelineEventSlots) == 0x4dd4) ? 1 : -1];
typedef char PhotoEnemyManagerEclManagerAt4DF4[
    (offsetof(PhotoEnemyManagerView, eclManager) == 0x4df4) ? 1 : -1];
typedef char PhotoEnemyManagerEnemyAnmAt4DF8[
    (offsetof(PhotoEnemyManagerView, enemyAnm) == 0x4df8) ? 1 : -1];
typedef char PhotoEnemyManagerUnknown4DFCAt4DFC[
    (offsetof(PhotoEnemyManagerView, unknown4dfc) == 0x4dfc) ? 1 : -1];
typedef char PhotoEnemyManagerEnemiesAt4E00[
    (offsetof(PhotoEnemyManagerView, enemyPool) == 0x4e00) ? 1 : -1];
typedef char PhotoEnemyManagerPhotoTargetsAt26AE00[
    (offsetof(PhotoEnemyManagerView, photoTargets) == 0x26ae00) ? 1 : -1];
typedef char PhotoEnemyManagerChainsAt26AE20[
    (offsetof(PhotoEnemyManagerView, calcChain) == 0x26ae20 &&
     offsetof(PhotoEnemyManagerView, drawChain) == 0x26ae24) ? 1 : -1];
typedef char PhotoEnemyManagerPhotoCardSessionAt26AE28[
    (offsetof(PhotoEnemyManagerView, eclPhotoCardSession) == 0x26ae28) ? 1 : -1];
typedef char PhotoEnemyManagerCountAt26AE2C[
    (offsetof(PhotoEnemyManagerView, activeEnemyCount) == 0x26ae2c) ? 1 : -1];
typedef char PhotoEnemyManagerSizeIs26AE30[
    (sizeof(PhotoEnemyManagerView) == 0x26ae30) ? 1 : -1];

} // namespace th095
