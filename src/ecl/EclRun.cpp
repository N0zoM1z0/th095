#include "../PhotoGameTask.hpp"
#include "../ZunBool.hpp"
#include "../Background.hpp"
#include "../Global.hpp"
#include "../PhotoBulletManagerLayout.hpp"
#include "EclOperands.hpp"

#include <string.h>
#include <stdlib.h>
#include <math.h>

#define TH095_ECL_RUN_DECLARATIONS_ONLY
#include "EclRunLow.inl"
#include "EclRunHigh.inl"
#undef TH095_ECL_RUN_DECLARATIONS_ONLY

#include "../GameplayGlobals.hpp"
#include "../PhotoCardInfo.hpp"
#include "../PhotoEffectRuntime.hpp"
#include "../PhotoEnemyEclManager.hpp"
#include "../PhotoEnemyManagerLayout.hpp"
#include "../PhotoStageLayout.hpp"
#include "../PhotoGamePlayer.hpp"
#include "EclAnmHandleEmission.inl"
#include "EclWorldVmSpawnEmission.inl"

#define TH095_ECL_RESOLVE_FLOAT(enemy, operand) \
    (enemy)->ResolveFloat((operand).asFloat)

// RunEcl and CallEclSub return the target's global ::ZunResult enum in both
// production and exact builds. The numeric values match th095::ZunResult, but
// the enum identity is part of the VC7.1 decorated symbol.
#define ZUN_SUCCESS TH095_LEGACY_ZUN_SUCCESS
#define ZUN_ERROR TH095_LEGACY_ZUN_ERROR

namespace th095
{

// The low/high opcode bodies are included lexically below so VC7 can reproduce
// RunEcl's target handler order, shared labels, locals, and stack frame.

static __forceinline void AssignEclShotPosition(
    PhotoBulletVector &destination, const Float3 &position)
{
    memcpy(&destination, &position, sizeof(destination));
}

static __forceinline void InitializeEclTargetTimer(ZunTimer *timer)
{
    timer->current = 0;
    timer->subFrame = 0.0f;
    timer->previous = -999999;
}

// FUNCTION: TH095 0x00408E70; TH08 0x004184B0 is the adjacent source oracle.
::ZunResult PhotoEnemyEclManagerView::RunEcl(PhotoEnemyView *enemy)
{
    using namespace EclRunLow;
    using namespace EclRunHigh;

    EclRawInstruction *instruction;
    i32 activeChildContext = -1;
    i32 lhsInt;

    enemy->activeEclCallStack = enemy->eclCallStack;
    enemy->activeEclContext = &enemy->mainEclContext;
    enemy->activeEclCallStackDepth = enemy->mainEclCallStackDepth;

restart_context:
    instruction = enemy->activeEclContext->currentInstr;

    for (;;)
    {
        if (enemy->pendingEclSubroutineIndex >= 0)
            goto enter_subroutine;

low_redispatch_instruction:
        enemy->worldPosition = enemy->position + enemy->positionOffset;

        if ((int)enemy->activeEclContext->secondaryTime > 0)
        {
            DecrementTimer(&enemy->activeEclContext->secondaryTime, 1);
            DecrementTimer(&enemy->activeEclContext->time, 1);
            break;
        }

        if (enemy->activeEclContext->time == instruction->time)
        {
            {
#define context (enemy->activeEclContext)
#define TH095_ECL_RUN_LOW_BODY
#define TH095_ECL_RUN_SHARED_SWITCH
            switch (instruction->opcode)
            {
                {
#include "EclRunLow.inl"
                }
                {
#include "EclRunTargetHigh.inl"
                }
            default:
                break;
            }
#undef TH095_ECL_RUN_SHARED_SWITCH
#undef TH095_ECL_RUN_LOW_BODY
#undef context
            }

low_advance_instruction:
            instruction = reinterpret_cast<EclRawInstruction *>(
                reinterpret_cast<u8 *>(instruction) + instruction->nextOffset);
            goto low_redispatch_instruction;

            break;
        }

        break;
    }

    // Keep the complete frame tail lexical so VC7 can share RunEcl's locals and emit
    // the observed in-function easing switch and child-context back edge.
    if (enemy->life > 0)
    {
        i32 i;
        f32 progress;
        {
        i32 restorePosition = 0;
        EnemyEclInterpolationSlot *entry =
            enemy->activeEclContext->interpolationSlots;
        {
        Float3 savedPosition = enemy->position;

        if (enemy->activeEclContext->perFrameCallback)
            enemy->activeEclContext->perFrameCallback(
                enemy, enemy->activeEclContext->perFrameInstruction);

        for (i = 0; i < 8; ++i, ++entry)
        {
            if (entry->callback)
            {
                entry->timer++;
                if (entry->timer >= entry->duration)
                    SetTimerValue(&entry->timer, entry->duration);

                progress = static_cast<f32>(entry->timer) / entry->duration;
                switch (entry->easing)
                {
                case 1: progress = progress * progress; break;
                case 2: progress = progress * progress * progress; break;
                case 3: progress = progress * progress * progress * progress; break;
                case 4:
                    progress = 1.0f - progress;
                    progress = progress * progress;
                    progress = 1.0f - progress;
                    break;
                case 5:
                    progress = 1.0f - progress;
                    progress = progress * progress * progress;
                    progress = 1.0f - progress;
                    break;
                case 6:
                    progress = 1.0f - progress;
                    progress = progress * progress * progress * progress;
                    progress = 1.0f - progress;
                    break;
                }

                entry->callback(enemy, entry, progress);
                if (entry->timer >= entry->duration)
                    entry->callback = 0;

                if (entry->affectedVariable == 10026.0f ||
                    entry->affectedVariable == 10027.0f ||
                    entry->affectedVariable == 10028.0f)
                    restorePosition = 1;
            }
        }

        if (restorePosition)
        {
            enemy->velocity.x = enemy->position.x - savedPosition.x;
            enemy->velocity.y = enemy->position.y - savedPosition.y;
            enemy->movementAngle = atan2f(enemy->velocity.y, enemy->velocity.x);
            enemy->position = savedPosition;
        }
        }
        }
    }

    if (activeChildContext == -1)
        enemy->mainEclCallStackDepth = enemy->activeEclCallStackDepth;
    else
        enemy->childEclBlocks[activeChildContext]->callStackDepth =
            enemy->activeEclCallStackDepth;

    enemy->activeEclContext->currentInstr = instruction;
    enemy->activeEclContext->time.operator++(0);

low_select_next_context:
    for (i32 next = activeChildContext + 1; next < 16; ++next)
    {
        if (enemy->childEclBlocks[next])
        {
            EnemyChildEclBlock *childContext =
                enemy->childEclBlocks[next];
            enemy->activeEclCallStack = childContext->callStack;
            enemy->activeEclContext = &childContext->eclContext;
            instruction = enemy->activeEclContext->currentInstr;
            enemy->activeEclContext->childContextSlot = next + 1;
            enemy->activeEclCallStackDepth = childContext->callStackDepth;
            activeChildContext = next;
            goto low_redispatch_instruction;
        }
    }

    enemy->activeEclCallStack = enemy->eclCallStack;
    enemy->activeEclContext = &enemy->mainEclContext;
    enemy->UpdateMovement();
    enemy->UpdateShotAndAnm();

    return ZUN_SUCCESS;
}

} // namespace th095
