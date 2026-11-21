#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "ecl/EclManager.hpp"
#include "PhotoEnemyManagerLayout.hpp"

namespace th095
{
namespace EclRunHigh
{
void __fastcall DispatchShotInstruction(Enemy *enemy, EclShotInstruction *instruction);
}

#define TH095_ENEMY_PRIMARY_ANM (g_PhotoEnemyManager->enemyAnm)
#define TH095_ENEMY_UNKNOWN_4DFC_ANM \
    (*reinterpret_cast<AnmLoaded **>(&g_PhotoEnemyManager->unknown4dfc[0]))

static __forceinline i32 &TargetEnemyLife(Enemy *enemy)
{
    return reinterpret_cast<PhotoEnemyView *>(enemy)->life;
}
#define TargetEnemyMirrorMovementX(enemy) \
    (reinterpret_cast<PhotoEnemyView *>(enemy)->mirrorMovementX)
#define TargetEnemyAlternateAnmBank(enemy) \
    (reinterpret_cast<PhotoEnemyView *>(enemy)->alternateAnmBank)
static __forceinline u8 &TargetEnemyAnmDirection(Enemy *enemy)
{
    return reinterpret_cast<PhotoEnemyView *>(enemy)->anmDirection;
}
#define TargetEnemyIdleAnmScript(enemy) \
    (reinterpret_cast<PhotoEnemyView *>(enemy)->idleAnmScript)
#define TargetEnemyIdleFromLeftAnmScript(enemy) \
    (reinterpret_cast<PhotoEnemyView *>(enemy)->idleFromLeftAnmScript)
#define TargetEnemyIdleFromRightAnmScript(enemy) \
    (reinterpret_cast<PhotoEnemyView *>(enemy)->idleFromRightAnmScript)
#define TargetEnemyMoveLeftAnmScript(enemy) \
    (reinterpret_cast<PhotoEnemyView *>(enemy)->moveLeftAnmScript)
#define TargetEnemyMoveRightAnmScript(enemy) \
    (reinterpret_cast<PhotoEnemyView *>(enemy)->moveRightAnmScript)

// FUNCTION: TH095 0x00413030; TH08 UpdateShotAndAnm is the source-shape oracle.
void Enemy::UpdateShotAndAnm()
{
    i32 direction;
    AnmLoaded *anm;

    if (TargetEnemyLife(this) > 0)
    {
        if (reinterpret_cast<PhotoEnemyView *>(this)->shootIntervalFrames > 0)
        {
            reinterpret_cast<PhotoEnemyView *>(this)->shootIntervalTimer++;
            if (reinterpret_cast<PhotoEnemyView *>(this)->shootIntervalTimer >= reinterpret_cast<PhotoEnemyView *>(this)->shootIntervalFrames)
            {
                EclRunHigh::DispatchShotInstruction(
                    this, &reinterpret_cast<PhotoEnemyView *>(this)->pendingShotInstruction);
                reinterpret_cast<PhotoEnemyView *>(this)->shootIntervalTimer = 0;
            }
        }

        if (TargetEnemyMoveLeftAnmScript(this) >= 0)
        {
            direction = 0;
            if (TargetEnemyMirrorMovementX(this) == 0)
            {
                if (this->velocity.x < -0.01f)
                    direction = 1;
                else if (this->velocity.x > 0.01f)
                    direction = 2;
            }
            else
            {
                if (this->velocity.x < -0.01f)
                    direction = 2;
                else if (this->velocity.x > 0.01f)
                    direction = 1;
            }

            if (TargetEnemyAnmDirection(this) != direction)
            {
                anm = TargetEnemyAlternateAnmBank(this)
                    ? TH095_ENEMY_UNKNOWN_4DFC_ANM
                    : TH095_ENEMY_PRIMARY_ANM;

                switch (direction)
                {
                case 0:
                    if (TargetEnemyAnmDirection(this) == 0xff)
                        anm->SetAndExecuteScriptIdx(&this->vm, TargetEnemyIdleAnmScript(this));
                    else if (TargetEnemyAnmDirection(this) == 1)
                        anm->SetAndExecuteScriptIdx(&this->vm, TargetEnemyIdleFromLeftAnmScript(this));
                    else
                        anm->SetAndExecuteScriptIdx(&this->vm, TargetEnemyIdleFromRightAnmScript(this));
                    break;
                case 1:
                    anm->SetAndExecuteScriptIdx(&this->vm, TargetEnemyMoveLeftAnmScript(this));
                    break;
                case 2:
                    anm->SetAndExecuteScriptIdx(&this->vm, TargetEnemyMoveRightAnmScript(this));
                    break;
                }
                TargetEnemyAnmDirection(this) = static_cast<u8>(direction);
            }
        }
    }
}
}
