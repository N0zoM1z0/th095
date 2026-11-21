#pragma once

#include "AnmVmId.hpp"

#include <stddef.h>

namespace th095
{

// Photo camera/stage storage is a trivial four-byte handle: the target
// PhotoCameraState constructor initializes only its AnmVm array and does not
// run AnmVmId default constructors for these slots.  Operations cross this
// storage boundary through the canonical AnmVmId ABI.
struct PhotoAnmVmId
{
    i32 value;

    operator i32() const
    {
        return this->value;
    }

    operator AnmVmId() const
    {
        AnmVmId id;
        id.value = this->value;
        return id;
    }

    void operator=(i32 value)
    {
        this->value = value;
    }

    PhotoAnmVmId &operator=(AnmVmId id)
    {
        this->value = id.value;
        return *this;
    }

    AnmVm *GetVm()
    {
        return reinterpret_cast<AnmVmId *>(this)->GetVm();
    }

    void SetInterrupt(i32 interrupt)
    {
        reinterpret_cast<AnmVmId *>(this)->SetInterrupt(interrupt);
    }
};

typedef char PhotoAnmVmIdSizeIs4[(sizeof(PhotoAnmVmId) == 4) ? 1 : -1];
typedef char PhotoAnmVmIdValueAt0[
    (offsetof(PhotoAnmVmId, value) == 0x0) ? 1 : -1];

} // namespace th095
