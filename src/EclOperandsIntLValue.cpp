#include "EnemyManager.hpp"
#include "ecl/EclOperands.hpp"

namespace th095
{

struct EclSharedIntLValueView
{
    u8 unknown000[0x168];
    i32 intVariables[4];
};

struct EclIntLValueRuntimeView
{
    u8 unknown000000[0x4df4];
    EclSharedIntLValueView *sharedOperands;
};

extern EclIntLValueRuntimeView *g_EclIntLValueRuntime;

#define ENEMY_I32(owner, offset) \
    (*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(owner) + (offset)))

namespace EclOperands
{

// FUNCTION: TH095 0x00410300.  The operand flag controls whether an ECL
// selector is writable; a disabled operand and every read-only selector keep
// pointing at the instruction's inline value.
i32 *__fastcall ResolveIntLValue(
    Enemy *enemy, i32 *operand, u16 flags, i32 flagIndex)
{
    if (flagIndex >= 0 && !(flags & (1 << flagIndex)))
        return operand;

    switch (*operand)
    {
    case 0x2710: return &enemy->activeEclContext->intVariables[0];
    case 0x2711: return &enemy->activeEclContext->intVariables[1];
    case 0x2712: return &enemy->activeEclContext->intVariables[2];
    case 0x2713: return &enemy->activeEclContext->intVariables[3];
    case 0x2714: return &enemy->activeEclContext->intVariables[4];
    case 0x2715: return &enemy->activeEclContext->intVariables[5];
    case 0x2716: return &enemy->activeEclContext->intVariables[6];
    case 0x2717: return &enemy->activeEclContext->intVariables[7];

    case 0x2734: return &enemy->activeEclContext->callParameterInts[0];
    case 0x2735: return &enemy->activeEclContext->callParameterInts[1];
    case 0x2736: return &enemy->activeEclContext->callParameterInts[2];
    case 0x2737: return &enemy->activeEclContext->callParameterInts[3];
    case 0x2724: return &enemy->activeEclContext->extraIntVariables[0];
    case 0x2725: return &enemy->activeEclContext->extraIntVariables[1];
    case 0x2726: return &enemy->activeEclContext->extraIntVariables[2];
    case 0x2727: return &enemy->activeEclContext->extraIntVariables[3];

    case 0x2731: return &ENEMY_I32(enemy, 0x2974);
    case 0x2733: return &ENEMY_I32(enemy, 0x2958);
    case 0x275b: return &ENEMY_I32(enemy, 0x2bd8);
    case 0x275c: return &ENEMY_I32(enemy, 0x2964);

    case 0x273c: return &g_EclIntLValueRuntime->sharedOperands->intVariables[0];
    case 0x273d: return &g_EclIntLValueRuntime->sharedOperands->intVariables[1];
    case 0x273e: return &g_EclIntLValueRuntime->sharedOperands->intVariables[2];
    case 0x273f: return &g_EclIntLValueRuntime->sharedOperands->intVariables[3];
    default: return operand;
    }
}

} // namespace EclOperands

#undef ENEMY_I32

} // namespace th095
