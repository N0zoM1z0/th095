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

    i32 operator==(i32 other) const
    {
        return this->value == other;
    }

    operator i32() const
    {
        return this->value;
    }

    AnmVmId &operator=(i32 value)
    {
        this->value = value;
        return *this;
    }

    AnmVm *GetVm();
    void SetInterrupt(i32 interrupt);
    void SetSprite(i32 spriteIndex);
};

// The manager and its VMs store handle values without running a constructor.
struct AnmVmIdStorage
{
    u32 value;

    i32 operator==(AnmVmId other) const
    {
        return this->value == static_cast<u32>(other.value);
    }

    u32 operator++(int)
    {
        return this->value++;
    }
};

typedef char AnmVmIdStorageSizeIs4[(sizeof(AnmVmIdStorage) == 4) ? 1 : -1];
typedef char AnmVmIdSizeIs4[(sizeof(AnmVmId) == 4) ? 1 : -1];

} // namespace th095

#endif
