#include "AnmManager.hpp"
#include "AnmVmId.hpp"

namespace th095
{

AnmVm *AnmManager::GetVm(AnmVmId id)
{
    AnmVmId nullId;

    if (id == nullId)
    {
        return NULL;
    }

    AnmVm *vm;
    vm = this->vmListHead;
    while (vm != NULL)
    {
        if (vm->id == id)
        {
            return vm;
        }
        vm = vm->next;
    }
    return NULL;
}

void AnmManager::SetInterrupt(AnmVmId id, i32 interrupt)
{
    AnmVm *vm = this->GetVm(id);
    if (vm != NULL)
    {
        vm->pendingInterrupt = (i16)interrupt;
    }
}

void AnmManager::MarkVmForDeletion(AnmVmId id)
{
    AnmVm *vm = this->GetVm(id);
    if (vm != NULL)
    {
        vm->pendingDeletion = true;
    }
}

void AnmManager::SetPosition(AnmVmId id, Float3 *position)
{
    AnmVm *vm = this->GetVm(id);
    if (vm != NULL)
    {
        vm->positionOffset = *position;
    }
}

Float3 *AnmManager::GetPosition(AnmVmId id)
{
    AnmVm *vm = this->GetVm(id);
    return vm != NULL ? &vm->positionOffset : NULL;
}

void AnmManager::MarkVmsForDeletion(AnmLoaded *anmFile)
{
    AnmVm *next;
    AnmVm *vm;

    vm = this->vmListHead;
    while (vm != NULL)
    {
        next = vm->next;
        if (vm->anmFile == anmFile)
        {
            vm->pendingDeletion = true;
        }
        vm = next;
    }
}

AnmVm *AnmVmId::GetVm()
{
    AnmVm *vm = g_AnmManager->GetVm(*this);
    if (vm == NULL)
    {
        this->value = 0;
    }
    return vm;
}

void AnmVmId::SetInterrupt(i32 interrupt)
{
    g_AnmManager->SetInterrupt(*this, interrupt);
}

void AnmVmId::SetSprite(i32 spriteIndex)
{
    AnmVm *vm = g_AnmManager->GetVm(*this);
    if (vm != NULL)
    {
        AnmLoaded *anmFile = vm->anmFile;
        anmFile->SetSprite(vm, spriteIndex);
    }
}

} // namespace th095
