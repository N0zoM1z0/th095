#pragma once

// Exact/DIFF access adapter for Enemy::UpdateShotAndAnm only.  The inherited
// ECL ABI includes AnmManagerEclView.hpp, which conflicts with the canonical
// AnmManager.hpp dependency of PhotoEnemy.hpp.  Normal source consumes the
// complete PhotoEnemyView; this adapter preserves only the fields used by the
// still-legacy Enemy method until that ANM declaration graph is unified.
namespace th095
{

struct PhotoEnemyView
{
    u8 unknown0000[0x2958];
    i32 life;                              // +0x2958
    u8 unknown295c[0x2b9c - 0x295c];
    u8 pendingShotInstruction[0x2c];       // +0x2b9c
    i32 shootIntervalFrames;               // +0x2bc8
    ZunTimer shootIntervalTimer;           // +0x2bcc
    u8 unknown2bd8[0x2bf4 - 0x2bd8];
    union
    {
        u32 flags1;                        // +0x2bf4
        struct
        {
            u32 unknownFlags000 : 16;
            u32 mirrorMovementX : 1;
            u32 unknownFlags017 : 14;
            u32 alternateAnmBank : 1;
        };
    };
    u8 unknown2bf8[0x2c0a - 0x2bf8];
    u8 anmDirection;                       // +0x2c0a
    u8 unknown2c0b[3];
    i16 idleAnmScript;                     // +0x2c0e
    i16 idleFromLeftAnmScript;             // +0x2c10
    i16 idleFromRightAnmScript;            // +0x2c12
    i16 moveLeftAnmScript;                 // +0x2c14
    i16 moveRightAnmScript;                // +0x2c16
};

typedef char EnemyShotEmissionLifeAt2958[
    (offsetof(PhotoEnemyView, life) == 0x2958) ? 1 : -1];
typedef char EnemyShotEmissionCadenceAt2B9C[
    (offsetof(PhotoEnemyView, pendingShotInstruction) == 0x2b9c &&
     offsetof(PhotoEnemyView, shootIntervalTimer) == 0x2bcc) ? 1 : -1];
typedef char EnemyShotEmissionFlagsAt2BF4[
    (offsetof(PhotoEnemyView, flags1) == 0x2bf4) ? 1 : -1];
typedef char EnemyShotEmissionAnmAt2C0A[
    (offsetof(PhotoEnemyView, anmDirection) == 0x2c0a &&
     offsetof(PhotoEnemyView, idleAnmScript) == 0x2c0e &&
     offsetof(PhotoEnemyView, moveRightAnmScript) == 0x2c16) ? 1 : -1];

} // namespace th095
