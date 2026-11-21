#pragma once

#include "../PhotoEnemyAnmVmIdStorage.hpp"
#include "../PhotoStraightLaserArgs.hpp"
#include "../PhotoRotatingLaserArgs.hpp"

// Helpers and lexical operand access for the TH095 high handlers.
namespace th095
{
namespace EclRunHigh
{

void __fastcall DispatchShotInstruction(
    PhotoEnemyView *enemy, EclShotInstruction *instruction);

struct SpawnPacketSmall
{
    i32 eclSubroutineId;
    Float3 position;
};
C_ASSERT(sizeof(SpawnPacketSmall) == 0x10);

// The two effect packets have distinct profile-independent owners.  Do not
// merge their overlapping prefixes: PhotoStraightLaserSpawnArgs is 0x28 and
// PhotoRotatingLaserSpawnArgs is 0x48.

template <typename Camera>
inline void AssignPhotoCameraLimit(Camera *camera, i32 value)
{
    camera->photoLimit = value;
}

struct PhotoAnmSpawner
{
    PhotoEnemyAnmVmIdStorage Spawn(i32 script, Float3 *position);
};

struct AnmManagerLookup
{
    AnmVm *FindVm(i32 handle);
    void RemoveVm(i32 handle);
    void ConfigureEnemyPhotoAnm(AnmVm *vm, void *work, i32 value);
};

struct PhotoSession
{
    void ReplaceActive();
    void Finish();
};

struct PhotoSessionDescriptor
{
    PhotoSession *Create();
};

struct PhotoModeController
{
    void Begin();
    void End();
};


// The target's inlined allocation wrapper keeps its size parameter addressable,
// so VC7.1 materializes it in RunEcl's shared temporary region before malloc.
inline void *Th095Alloc(size_t size)
{
    return malloc(*reinterpret_cast<volatile size_t *>(&size));
}

inline i32 Th095PreserveI32(i32 value)
{
    return *reinterpret_cast<volatile i32 *>(&value);
}

extern AnmManagerLookup *g_Th095AnmManager;
extern PhotoModeController *g_Th095PhotoMode;

inline void AddTimerValue(ZunTimer *timer, i32 value)
{
    timer->Add(static_cast<f32>(value));
}

inline void SetTimerValue(ZunTimer *timer, i32 value)
{
    *timer = value;
}

inline void DecrementTimer(ZunTimer *timer, i32 value)
{
    timer->Add(static_cast<f32>(-value));
}

// These expressions expand inside RunEcl to preserve operand order and
// the compiler's temporary allocation for indirect reads and writes.
#define ECL_RAW_INT(index) (instruction->operands[(index)].asInt)

#define ECL_READ_INT(index) \
    ((instruction->operandFlags & (1U << (index))) \
         ? EclOperands::ResolveInt(enemy, ECL_RAW_INT(index)) \
         : ECL_RAW_INT(index))

#define ECL_READ_FLOAT_BITS(index) \
    ((instruction->operandFlags & (1U << (index))) \
         ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[(index)]) \
         : *reinterpret_cast<f32 *>(&instruction->operands[(index)].asInt))

#define ECL_WRITE_FLOAT(index) \
    EclOperands::ResolveFloatLValue( \
        enemy, &instruction->operands[(index)].asFloat, instruction->operandFlags, (index))

#define TH095_ECL_ASSIGN_FLOAT(field, index) ((field) = ECL_READ_FLOAT_BITS(index))

} // namespace EclRunHigh
} // namespace th095
