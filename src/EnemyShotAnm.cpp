#include "EnemyManager.hpp"
#include "ecl/EclManager.hpp"

namespace th095
{
namespace EclRunHigh
{
extern u8 *g_Th095Runtime;
void __fastcall DispatchShotInstruction(Enemy *enemy, EclRawInstruction *instruction);
}

struct TargetEnemyAnmScripts
{
    i16 idleInitial;
    i16 idleFromLeft;
    i16 idleFromRight;
    i16 moveLeft;
    i16 moveRight;
    i16 special;
};
C_ASSERT(sizeof(TargetEnemyAnmScripts) == 0x0c);

static __forceinline i32 &TargetEnemyLife(Enemy *enemy)
{
    return *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2958);
}
static __forceinline EclRawInstruction *TargetEnemyPendingShot(Enemy *enemy)
{
    return reinterpret_cast<EclRawInstruction *>(reinterpret_cast<u8 *>(enemy) + 0x2b9c);
}
static __forceinline i32 &TargetEnemyShootInterval(Enemy *enemy)
{
    return *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bc8);
}
static __forceinline ZunTimer &TargetEnemyShootTimer(Enemy *enemy)
{
    return *reinterpret_cast<ZunTimer *>(reinterpret_cast<u8 *>(enemy) + 0x2bcc);
}
static __forceinline u32 &TargetEnemyFlags1(Enemy *enemy)
{
    return *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bf4);
}
static __forceinline u8 &TargetEnemyAnmDirection(Enemy *enemy)
{
    return *reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(enemy) + 0x2c0a);
}
static __forceinline TargetEnemyAnmScripts &TargetEnemyAnmScriptsView(Enemy *enemy)
{
    return *reinterpret_cast<TargetEnemyAnmScripts *>(reinterpret_cast<u8 *>(enemy) + 0x2c0e);
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
                anm = ((TargetEnemyFlags1(this) >> 31) & 1)
                    ? *reinterpret_cast<AnmLoaded **>(EclRunHigh::g_Th095Runtime + 0x4dfc)
                    : *reinterpret_cast<AnmLoaded **>(EclRunHigh::g_Th095Runtime + 0x4df8);

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
