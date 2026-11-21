#pragma once

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

// Opcode 152 queues a signed 16-bit interrupt. VC7 allocates the operand
// temporary differently when the inline receiver takes an i32 parameter.
inline void SetVmInterrupt(AnmVm *vm, i16 interrupt)
{
    vm->pendingInterrupt = interrupt;
}

struct AnmManagerLookup
{
    AnmVm *FindVm(i32 handle);
    void RemoveVm(i32 handle);
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
         : instruction->operands[(index)].asFloat)

#define ECL_WRITE_FLOAT(index) \
    EclOperands::ResolveFloatLValue( \
        enemy, &instruction->operands[(index)].asFloat, instruction->operandFlags, (index))

#define TH095_ECL_ASSIGN_FLOAT(field, index) ((field) = ECL_READ_FLOAT_BITS(index))

} // namespace EclRunHigh
} // namespace th095
