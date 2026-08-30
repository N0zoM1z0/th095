#define TH095_DECLARE_ANM_LOADED_INITIALIZE_VM
#include "AnmManager.hpp"

namespace th095
{

// FUNCTION: TH095 0x00404B80.
void AnmLoaded::InitializeVm(AnmVm *vm, i32 scriptIndex)
{
    vm->Initialize();
    vm->scriptIndex = (i16)scriptIndex;
    vm->positionOffset = Float3(0.0f, 0.0f, 0.0f);
    vm->position = Float3(0.0f, 0.0f, 0.0f);
    vm->alternatePosition = Float3(0.0f, 0.0f, 0.0f);
    vm->unknown2c0[1] = 0x0f;
    vm->unknown2c0[0] = 0x0f;
    this->SetAndExecuteScript(vm, this->scripts[scriptIndex]);
}

} // namespace th095
