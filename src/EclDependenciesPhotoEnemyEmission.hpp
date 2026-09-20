#pragma once

// Exact/DIFF access adapter for EclDependencies only.  The inherited ECL ABI
// cannot include PhotoEnemy.hpp until its private ANM declaration graph is
// unified.  Normal source consumes the complete canonical PhotoEnemyView.
namespace th095
{

struct PhotoEnemyView
{
    u8 unknown0000[0x2bf4];
    union
    {
        u32 flags1;                        // +0x2bf4
        struct
        {
            u32 unknownFlags000 : 10;
            u32 movementMode : 2;
            u32 movementEasing : 3;
            u32 unknownFlags015 : 9;
            u32 suppressEclCallStack : 1;
            u32 unknownFlags025 : 7;
        };
    };
    u32 flags2;                            // +0x2bf8
    u8 unknown2bfc[0x2c0a - 0x2bfc];
    u8 anmDirection;                       // +0x2c0a
    u8 unknown2c0b[3];
    i16 idleAnmScript;                     // +0x2c0e
    i16 idleFromLeftAnmScript;             // +0x2c10
    i16 idleFromRightAnmScript;            // +0x2c12
    i16 moveLeftAnmScript;                 // +0x2c14
    i16 moveRightAnmScript;                // +0x2c16
    i16 specialAnmScript;                  // +0x2c18
    u8 unknown2c1a[0x2c3c - 0x2c1a];
    Float2 movementBoundsMin;              // +0x2c3c
    Float2 movementBoundsMax;              // +0x2c44
    u8 unknown2c4c[0x2cac - 0x2c4c];
    void *childEclBlocks[16];              // +0x2cac
};

typedef char EclDependencyEmissionFlagsAt2BF4[
    (offsetof(PhotoEnemyView, flags1) == 0x2bf4) ? 1 : -1];
typedef char EclDependencyEmissionAnmAt2C0A[
    (offsetof(PhotoEnemyView, anmDirection) == 0x2c0a &&
     offsetof(PhotoEnemyView, specialAnmScript) == 0x2c18) ? 1 : -1];
typedef char EclDependencyEmissionBoundsAt2C3C[
    (offsetof(PhotoEnemyView, movementBoundsMin) == 0x2c3c &&
     offsetof(PhotoEnemyView, movementBoundsMax) == 0x2c44) ? 1 : -1];
typedef char EclDependencyEmissionChildrenAt2CAC[
    (offsetof(PhotoEnemyView, childEclBlocks) == 0x2cac) ? 1 : -1];

} // namespace th095
