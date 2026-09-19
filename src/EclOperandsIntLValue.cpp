#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoEnemyEclOperandAccess.hpp"
#include "ecl/EclOperands.hpp"
#if !defined(TH095_MATCH_EXACT) && !defined(DIFFBUILD)
#include "PhotoEnemyManager.hpp"
#endif

namespace th095
{

struct EclSharedIntLValueView
{
    u8 unknown000[0x168];
    i32 intVariables[4];
};

#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
struct EclIntLValueRuntimeView
{
    u8 unknown000000[0x4df4];
    EclSharedIntLValueView *sharedOperands;
};
#define TH095_ECL_INT_LVALUE_SHARED(runtime) ((runtime)->sharedOperands)
#else
typedef PhotoEnemyManagerView EclIntLValueRuntimeView;
#define TH095_ECL_INT_LVALUE_SHARED(runtime) \
    reinterpret_cast<EclSharedIntLValueView *>((runtime)->eclManager)
#endif

extern EclIntLValueRuntimeView *g_EclIntLValueRuntime;
#ifndef DIFFBUILD
#define g_EclIntLValueRuntime \
    TH095_RUNTIME_GLOBAL_PTR(EclIntLValueRuntimeView, g_RuntimeEnemyManagerOwner)
#endif

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

    case TH095_ECL_CALL_PARAMETER_INT0: return &enemy->activeEclContext->callParameterInts[0];
    case TH095_ECL_CALL_PARAMETER_INT1: return &enemy->activeEclContext->callParameterInts[1];
    case TH095_ECL_CALL_PARAMETER_INT2: return &enemy->activeEclContext->callParameterInts[2];
    case TH095_ECL_CALL_PARAMETER_INT3: return &enemy->activeEclContext->callParameterInts[3];
    case 0x2724: return &enemy->activeEclContext->extraIntVariables[0];
    case 0x2725: return &enemy->activeEclContext->extraIntVariables[1];
    case 0x2726: return &enemy->activeEclContext->extraIntVariables[2];
    case 0x2727: return &enemy->activeEclContext->extraIntVariables[3];

    case 0x2731: return &TH095_ECL_TIMER_CURRENT(enemy);
    case 0x2733: return &TH095_ECL_ENEMY_LIFE(enemy);
    case 0x275b: return &TH095_ECL_ITEM_DROP_TYPE(enemy);
    case 0x275c: return &TH095_ECL_ENEMY_SCORE(enemy);

    case 0x273c: return &TH095_ECL_INT_LVALUE_SHARED(g_EclIntLValueRuntime)->intVariables[0];
    case 0x273d: return &TH095_ECL_INT_LVALUE_SHARED(g_EclIntLValueRuntime)->intVariables[1];
    case 0x273e: return &TH095_ECL_INT_LVALUE_SHARED(g_EclIntLValueRuntime)->intVariables[2];
    case 0x273f: return &TH095_ECL_INT_LVALUE_SHARED(g_EclIntLValueRuntime)->intVariables[3];
    default: return operand;
    }
}

} // namespace EclOperands

#undef TH095_ECL_PHOTO_TARGET_SLOT
#undef TH095_ECL_SCHEDULED_FRAME3
#undef TH095_ECL_SCHEDULED_FRAME2
#undef TH095_ECL_SCHEDULED_FRAME1
#undef TH095_ECL_SCHEDULED_FRAME0
#undef TH095_ECL_ITEM_DROP_TYPE
#undef TH095_ECL_ENEMY_SCORE
#undef TH095_ECL_TIMER_CURRENT
#undef TH095_ECL_ENEMY_LIFE
#undef TH095_ECL_SCHEDULED_FRAME
#undef TH095_PHOTO_ENEMY_U8
#undef TH095_PHOTO_ENEMY_I32

} // namespace th095
