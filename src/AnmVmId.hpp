#ifndef TH095_ANM_VM_ID_HPP
#define TH095_ANM_VM_ID_HPP

#include "inttypes.hpp"

namespace th095
{

struct AnmVm;

struct AnmVmId
{
    i32 value;

    AnmVm *GetVm();
    void SetInterrupt(i32 interrupt);
};

typedef char AnmVmIdSizeIs4[(sizeof(AnmVmId) == 4) ? 1 : -1];

} // namespace th095

#endif
