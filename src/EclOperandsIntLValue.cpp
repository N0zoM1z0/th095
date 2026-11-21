#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoEnemyEclManager.hpp"
#include "PhotoEnemyManagerLayout.hpp"
#include "ecl/EclOperands.hpp"

namespace th095
{


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
    case 0x2710: return &enemy->activeEclContext->scriptState.intVariables[0];
    case 0x2711: return &enemy->activeEclContext->scriptState.intVariables[1];
    case 0x2712: return &enemy->activeEclContext->scriptState.intVariables[2];
    case 0x2713: return &enemy->activeEclContext->scriptState.intVariables[3];
    case 0x2714: return &enemy->activeEclContext->scriptState.intVariables[4];
    case 0x2715: return &enemy->activeEclContext->scriptState.intVariables[5];
    case 0x2716: return &enemy->activeEclContext->scriptState.intVariables[6];
    case 0x2717: return &enemy->activeEclContext->scriptState.intVariables[7];

    case ECL_CALL_PARAMETER_INT0: return &enemy->activeEclContext->scriptState.callParameters.ints[0];
    case ECL_CALL_PARAMETER_INT1: return &enemy->activeEclContext->scriptState.callParameters.ints[1];
    case ECL_CALL_PARAMETER_INT2: return &enemy->activeEclContext->scriptState.callParameters.ints[2];
    case ECL_CALL_PARAMETER_INT3: return &enemy->activeEclContext->scriptState.callParameters.ints[3];
    case 0x2724: return &enemy->activeEclContext->scriptState.extraIntVariables[0];
    case 0x2725: return &enemy->activeEclContext->scriptState.extraIntVariables[1];
    case 0x2726: return &enemy->activeEclContext->scriptState.extraIntVariables[2];
    case 0x2727: return &enemy->activeEclContext->scriptState.extraIntVariables[3];

    case ECL_ENEMY_TIMER: return &reinterpret_cast<PhotoEnemyView *>(enemy)->eclTimer.current;
    case ECL_ENEMY_LIFE: return &reinterpret_cast<PhotoEnemyView *>(enemy)->life;
    case ECL_ENEMY_ITEM_DROP_TYPE: return &reinterpret_cast<PhotoEnemyView *>(enemy)->itemDropType;
    case ECL_ENEMY_SCORE: return &reinterpret_cast<PhotoEnemyView *>(enemy)->score;

    case ECL_MANAGER_PARAMETER_INT0: return &g_PhotoEnemyManager->eclManager->callParameters.ints[0];
    case ECL_MANAGER_PARAMETER_INT1: return &g_PhotoEnemyManager->eclManager->callParameters.ints[1];
    case ECL_MANAGER_PARAMETER_INT2: return &g_PhotoEnemyManager->eclManager->callParameters.ints[2];
    case ECL_MANAGER_PARAMETER_INT3: return &g_PhotoEnemyManager->eclManager->callParameters.ints[3];
    default: return operand;
    }
}

} // namespace EclOperands


} // namespace th095
