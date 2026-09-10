#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
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
#ifndef DIFFBUILD
#define g_EclIntLValueRuntime \
    TH095_RUNTIME_GLOBAL_PTR(EclIntLValueRuntimeView, g_RuntimeEnemyManagerOwner)
#endif

#define ENEMY_I32(owner, offset) \
    (*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(owner) + (offset)))
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_ENEMY_LIFE(owner) ENEMY_I32((owner), 0x2958)
#else
struct EclIntLValueOperandEnemyLifeView
{
    u8 unknown0000[0x2958];
    i32 life;
};
typedef char EclIntLValueOperandEnemyLifeAt2958[
    (offsetof(EclIntLValueOperandEnemyLifeView, life) == 0x2958) ? 1 : -1];
#define TH095_ECL_ENEMY_LIFE(owner) \
    (reinterpret_cast<EclIntLValueOperandEnemyLifeView *>(owner)->life)
#endif
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_TIMER_CURRENT(owner) ENEMY_I32((owner), 0x2974)
#else
struct EclIntLValueOperandTimerView
{
    i32 previous;
    f32 subFrame;
    i32 current;
};
typedef char EclIntLValueOperandTimerViewSizeC[
    (sizeof(EclIntLValueOperandTimerView) == 0x0c) ? 1 : -1];
struct EclIntLValueOperandEnemyTimerView
{
    u8 unknown0000[0x296c];
    EclIntLValueOperandTimerView eclTimer;
};
typedef char EclIntLValueOperandEnemyTimerAt296C[
    (offsetof(EclIntLValueOperandEnemyTimerView, eclTimer) == 0x296c) ? 1 : -1];
#define TH095_ECL_TIMER_CURRENT(owner) \
    (reinterpret_cast<EclIntLValueOperandEnemyTimerView *>(owner)->eclTimer.current)
#endif
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_ENEMY_SCORE(owner) ENEMY_I32((owner), 0x2964)
#else
struct EclIntLValueOperandEnemyScoreView
{
    u8 unknown0000[0x2964];
    i32 score;
};
typedef char EclIntLValueOperandEnemyScoreAt2964[
    (offsetof(EclIntLValueOperandEnemyScoreView, score) == 0x2964) ? 1 : -1];
#define TH095_ECL_ENEMY_SCORE(owner) \
    (reinterpret_cast<EclIntLValueOperandEnemyScoreView *>(owner)->score)
#endif
#if defined(TH095_MATCH_EXACT)
#define TH095_ECL_ITEM_DROP_TYPE(owner) ENEMY_I32((owner), 0x2bd8)
#else
struct EclIntLValueOperandItemDropTypeView
{
    u8 unknown0000[0x2bd8];
    i32 itemDropType;
};
typedef char EclIntLValueOperandItemDropTypeAt2BD8[
    (offsetof(EclIntLValueOperandItemDropTypeView, itemDropType) == 0x2bd8) ? 1 : -1];
#define TH095_ECL_ITEM_DROP_TYPE(owner) \
    (reinterpret_cast<EclIntLValueOperandItemDropTypeView *>(owner)->itemDropType)
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

    case 0x2734: return &enemy->activeEclContext->callParameterInts[0];
    case 0x2735: return &enemy->activeEclContext->callParameterInts[1];
    case 0x2736: return &enemy->activeEclContext->callParameterInts[2];
    case 0x2737: return &enemy->activeEclContext->callParameterInts[3];
    case 0x2724: return &enemy->activeEclContext->extraIntVariables[0];
    case 0x2725: return &enemy->activeEclContext->extraIntVariables[1];
    case 0x2726: return &enemy->activeEclContext->extraIntVariables[2];
    case 0x2727: return &enemy->activeEclContext->extraIntVariables[3];

    case 0x2731: return &TH095_ECL_TIMER_CURRENT(enemy);
    case 0x2733: return &TH095_ECL_ENEMY_LIFE(enemy);
    case 0x275b: return &TH095_ECL_ITEM_DROP_TYPE(enemy);
    case 0x275c: return &TH095_ECL_ENEMY_SCORE(enemy);

    case 0x273c: return &g_EclIntLValueRuntime->sharedOperands->intVariables[0];
    case 0x273d: return &g_EclIntLValueRuntime->sharedOperands->intVariables[1];
    case 0x273e: return &g_EclIntLValueRuntime->sharedOperands->intVariables[2];
    case 0x273f: return &g_EclIntLValueRuntime->sharedOperands->intVariables[3];
    default: return operand;
    }
}

} // namespace EclOperands

#undef TH095_ECL_ITEM_DROP_TYPE
#undef TH095_ECL_ENEMY_SCORE
#undef TH095_ECL_TIMER_CURRENT
#undef TH095_ECL_ENEMY_LIFE
#undef ENEMY_I32

} // namespace th095
