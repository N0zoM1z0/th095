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

// FUNCTION: TH095 0x00413030; TH08 UpdateShotAndAnm is the source-shape oracle.
void PhotoEnemyView::UpdateShotAndAnm()
{
    i32 direction;
    AnmLoaded *anm;

    if (this->life > 0)
    {
        if (this->shootIntervalFrames > 0)
        {
            this->shootIntervalTimer++;
            if (this->shootIntervalTimer >= this->shootIntervalFrames)
            {
                EclRunHigh::DispatchShotInstruction(
                    reinterpret_cast<Enemy *>(this), &this->pendingShotInstruction);
                this->shootIntervalTimer = 0;
            }
        }

        if (this->anmScripts.moveLeft >= 0)
        {
            direction = 0;
            if (this->control.mirrorMovementX == 0)
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

            if (this->anmDirection != direction)
            {
                anm = this->control.alternateAnmBank
                    ? g_PhotoEnemyManager->unknown4dfc
                    : g_PhotoEnemyManager->enemyAnm;

                switch (direction)
                {
                case 0:
                    if (this->anmDirection == 0xff)
                        anm->SetAndExecuteScriptIdx(&this->vm, this->anmScripts.idleInitial);
                    else if (this->anmDirection == 1)
                        anm->SetAndExecuteScriptIdx(&this->vm, this->anmScripts.idleFromLeft);
                    else
                        anm->SetAndExecuteScriptIdx(&this->vm, this->anmScripts.idleFromRight);
                    break;
                case 1:
                    anm->SetAndExecuteScriptIdx(&this->vm, this->anmScripts.moveLeft);
                    break;
                case 2:
                    anm->SetAndExecuteScriptIdx(&this->vm, this->anmScripts.moveRight);
                    break;
                }
                this->anmDirection = static_cast<u8>(direction);
            }
        }
    }
}
}
