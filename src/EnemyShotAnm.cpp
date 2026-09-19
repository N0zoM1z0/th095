#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "ecl/EclManager.hpp"
#if !defined(DIFFBUILD) && !defined(TH095_MATCH_EXACT)
#include "PhotoEnemyManager.hpp"
#else
#include "EnemyShotAnmEmission.hpp"
#endif

namespace th095
{
namespace EclRunHigh
{
extern u8 *g_Th095Runtime;
void __fastcall DispatchShotInstruction(Enemy *enemy, EclRawInstruction *instruction);
}

#ifdef DIFFBUILD
#define TH095_ENEMY_SHOT_RUNTIME EclRunHigh::g_Th095Runtime
#else
#define TH095_ENEMY_SHOT_RUNTIME \
    TH095_RUNTIME_GLOBAL_PTR(u8, g_RuntimeEnemyManagerOwner)
#endif

#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
#define TH095_ENEMY_PRIMARY_ANM \
    (*reinterpret_cast<AnmLoaded **>(TH095_ENEMY_SHOT_RUNTIME + 0x4df8))
#define TH095_ENEMY_UNKNOWN_4DFC_ANM \
    (*reinterpret_cast<AnmLoaded **>(TH095_ENEMY_SHOT_RUNTIME + 0x4dfc))
#else
#define TH095_ENEMY_MANAGER_RUNTIME \
    TH095_RUNTIME_GLOBAL_PTR(PhotoEnemyManagerView, g_RuntimeEnemyManagerOwner)
#define TH095_ENEMY_PRIMARY_ANM (TH095_ENEMY_MANAGER_RUNTIME->enemyAnm)
#define TH095_ENEMY_UNKNOWN_4DFC_ANM \
    (*reinterpret_cast<AnmLoaded **>( \
        &TH095_ENEMY_MANAGER_RUNTIME->unknown4dfc[0]))
#endif

static __forceinline i32 &TargetEnemyLife(Enemy *enemy)
{
    return reinterpret_cast<PhotoEnemyView *>(enemy)->life;
}
static __forceinline EclRawInstruction *TargetEnemyPendingShot(Enemy *enemy)
{
    return reinterpret_cast<EclRawInstruction *>(
        reinterpret_cast<PhotoEnemyView *>(enemy)->pendingShotInstruction);
}
static __forceinline i32 &TargetEnemyShootInterval(Enemy *enemy)
{
    return reinterpret_cast<PhotoEnemyView *>(enemy)->shootIntervalFrames;
}
static __forceinline ZunTimer &TargetEnemyShootTimer(Enemy *enemy)
{
    return reinterpret_cast<PhotoEnemyView *>(enemy)->shootIntervalTimer;
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
        if (TargetEnemyShootInterval(this) > 0)
        {
            TargetEnemyShootTimer(this)++;
            if (TargetEnemyShootTimer(this) >= TargetEnemyShootInterval(this))
            {
                EclRunHigh::DispatchShotInstruction(this, TargetEnemyPendingShot(this));
                TargetEnemyShootTimer(this) = 0;
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
