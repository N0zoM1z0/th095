#ifndef TH095_ANM_VM_LIFECYCLE_HPP
#define TH095_ANM_VM_LIFECYCLE_HPP

#include "AnmManager.hpp"
#include "AnmVmId.hpp"

#include <stdlib.h>

namespace th095
{

struct AnmVmLifecycleView
{
    AnmVmLifecycleView *next;
    u8 unknown004[4];
    AnmVmLifecycleView *previous;
    u32 renderMode;
    struct Id
    {
        i32 value;

        Id()
        {
            this->value = 0;
        }

        i32 operator==(Id other)
        {
            return this->value == other.value;
        }

        i32 operator++(int)
        {
            return this->value++;
        }
    } id;
};

struct AnmVmDeleteView;

// Canonical bounded receiver used by the exact AddVm/RemoveVm units.  Its list
// pointers coincide with AnmManager::vmListHead/vmListTail at +0x381814.
struct AnmManagerVmLifecycleView
{
    u8 unknown000[0x381814];
    AnmVmLifecycleView *vmListHead;
    AnmVmLifecycleView *vmListTail;
    u8 unknown38181c[0x192c];
    AnmVmLifecycleView::Id nextVmId;

    AnmVmId AddVm(AnmVmLifecycleView *vm);
    i32 RemoveVm(AnmVmDeleteView *vm);
};

struct AnmVmDeleteView
{
    AnmVmDeleteView *next;
    u8 unknown004[4];
    AnmVmDeleteView *previous;
    u8 unknown00c[8];
    void *generatedVertices;

    ~AnmVmDeleteView()
    {
        if (this->generatedVertices != NULL)
        {
            void *generatedVertices = this->generatedVertices;
            free(generatedVertices);
        }
    }
};

} // namespace th095

#endif
