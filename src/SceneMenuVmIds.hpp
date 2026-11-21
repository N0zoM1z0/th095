#pragma once

#include "AnmVmId.hpp"

namespace th095
{

// The title allocation clears and fills both sets of handles explicitly.
// A stored handle has the same bits as the constructed ANM call argument.
struct SceneAnmVmId
{
    i32 value;

    operator AnmVmId() const
    {
        AnmVmId id;
        id.value = this->value;
        return id;
    }

    SceneAnmVmId &operator=(AnmVmId id)
    {
        this->value = id.value;
        return *this;
    }

    AnmVm *GetVm();
    void SetInterrupt(i32 interrupt);
};

struct SceneAnmVmIdArray
{
    enum { COUNT = 0x9a };

    SceneAnmVmId values[COUNT];

    SceneAnmVmId &operator[](i32 index)
    {
        return this->values[index];
    }

    inline void SetInterrupt(i32 index, i32 interrupt);
};

typedef char SceneAnmVmIdSizeIs4[(sizeof(SceneAnmVmId) == 4) ? 1 : -1];

} // namespace th095
