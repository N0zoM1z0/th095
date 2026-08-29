#ifndef TH095_ANM_VM_ID_HPP
#define TH095_ANM_VM_ID_HPP

#include "inttypes.hpp"

namespace th095
{

struct AnmVm;

struct AnmVmId
{
    i32 value;

    AnmVmId()
    {
        this->value = 0;
    }

    i32 operator==(AnmVmId other)
    {
        return this->value == other.value;
    }

    AnmVm *GetVm();
    void SetInterrupt(i32 interrupt);
    void SetSprite(i32 spriteIndex);
};

typedef char AnmVmIdSizeIs4[(sizeof(AnmVmId) == 4) ? 1 : -1];

} // namespace th095

#endif
