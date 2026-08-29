#include "AnmManager.hpp"
#include "AnmVmId.hpp"

namespace th095
{

struct AnmManagerVmIdView
{
    AnmVm *GetVm(i32 id);
    void SetInterrupt(i32 id, i32 interrupt);
};

extern AnmManagerVmIdView *g_AnmVmIdManager;

AnmVm *AnmVmId::GetVm()
{
    AnmVm *vm = g_AnmVmIdManager->GetVm(this->value);
    if (vm == NULL)
    {
        this->value = 0;
    }
    return vm;
}

void AnmVmId::SetInterrupt(i32 interrupt)
{
    g_AnmVmIdManager->SetInterrupt(this->value, interrupt);
}

} // namespace th095
