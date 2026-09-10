#include "EnemyManager.hpp"
#include "GameplayGlobals.hpp"
#include "ecl/EclManager.hpp"

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

struct EnemyShotCadenceView
{
    u8 unknown0000[0x2b9c];
    u8 pendingShotInstruction[0x2c];
    i32 shootIntervalFrames;
    ZunTimer shootIntervalTimer;
};
typedef char EnemyShotCadencePendingInstructionAt2B9C[
    (offsetof(EnemyShotCadenceView, pendingShotInstruction) == 0x2b9c) ? 1 : -1];
typedef char EnemyShotCadenceIntervalAt2BC8[
    (offsetof(EnemyShotCadenceView, shootIntervalFrames) == 0x2bc8) ? 1 : -1];
typedef char EnemyShotCadenceTimerAt2BCC[
    (offsetof(EnemyShotCadenceView, shootIntervalTimer) == 0x2bcc) ? 1 : -1];

static __forceinline i32 &TargetEnemyLife(Enemy *enemy)
{
    return *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2958);
}
static __forceinline EclRawInstruction *TargetEnemyPendingShot(Enemy *enemy)
{
    return reinterpret_cast<EclRawInstruction *>(
        reinterpret_cast<EnemyShotCadenceView *>(enemy)->pendingShotInstruction);
}
static __forceinline i32 &TargetEnemyShootInterval(Enemy *enemy)
{
    return reinterpret_cast<EnemyShotCadenceView *>(enemy)->shootIntervalFrames;
}
static __forceinline ZunTimer &TargetEnemyShootTimer(Enemy *enemy)
{
    return reinterpret_cast<EnemyShotCadenceView *>(enemy)->shootIntervalTimer;
}
static __forceinline u32 &TargetEnemyFlags1(Enemy *enemy)
{
    return *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4);
}
static __forceinline u8 &TargetEnemyAnmDirection(Enemy *enemy)
{
    return *reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(enemy) + 0x2c0a);
}
static __forceinline EnemyAnmScripts &TargetEnemyAnmScriptsView(Enemy *enemy)
{
    return *reinterpret_cast<EnemyAnmScripts *>(reinterpret_cast<u8 *>(enemy) + 0x2c0e);
}

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

        if (TargetEnemyAnmScriptsView(this).moveLeft >= 0)
        {
            direction = 0;
            if (((TargetEnemyFlags1(this) >> 16) & 1) == 0)
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
                anm = ((TargetEnemyFlags1(this) >> TH095_PHOTO_ENEMY_FLAG_ALTERNATE_ANM_BANK_SHIFT) & 1)
                    ? *reinterpret_cast<AnmLoaded **>(TH095_ENEMY_SHOT_RUNTIME + 0x4dfc)
                    : *reinterpret_cast<AnmLoaded **>(TH095_ENEMY_SHOT_RUNTIME + 0x4df8);

                switch (direction)
                {
                case 0:
                    if (TargetEnemyAnmDirection(this) == 0xff)
                        anm->SetAndExecuteScriptIdx(&this->vm, TargetEnemyAnmScriptsView(this).idleInitial);
                    else if (TargetEnemyAnmDirection(this) == 1)
                        anm->SetAndExecuteScriptIdx(&this->vm, TargetEnemyAnmScriptsView(this).idleFromLeft);
                    else
                        anm->SetAndExecuteScriptIdx(&this->vm, TargetEnemyAnmScriptsView(this).idleFromRight);
                    break;
                case 1:
                    anm->SetAndExecuteScriptIdx(&this->vm, TargetEnemyAnmScriptsView(this).moveLeft);
                    break;
                case 2:
                    anm->SetAndExecuteScriptIdx(&this->vm, TargetEnemyAnmScriptsView(this).moveRight);
                    break;
                }
                TargetEnemyAnmDirection(this) = static_cast<u8>(direction);
            }
        }
    }
}
}
