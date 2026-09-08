#pragma once

#include "inttypes.hpp"

namespace th095
{

struct ZunTimer
{
    i32 previous;
    f32 subFrame;
    i32 current;

    ZunTimer()
    {
        Initialize();
    }

    void Initialize()
    {
        this->current = 0;
        this->previous = -999999;
        this->subFrame = 0.0f;
    }

    void SetCurrent(i32 value)
    {
        this->current = value;
        this->subFrame = (f32)value;
        this->previous = -999999;
    }

    i32 GetCurrent()
    {
        return this->current;
    }

    void Reset()
    {
        this->Initialize();
    }

    void Set(i32 value)
    {
        this->SetCurrent(value);
    }

    void operator=(i32 value)
    {
        this->SetCurrent(value);
    }

    operator i32()
    {
        return this->current;
    }

    operator f32()
    {
        return this->subFrame;
    }

    i32 Tick();
    void Add(f32 value);

    void Increment(i32 value)
    {
        this->Add((f32)value);
    }

    void Decrement(i32 value)
    {
        this->Add((f32)-value);
    }

    void operator++(int)
    {
        this->Tick();
    }

    void operator--(int)
    {
        this->Decrement(1);
    }

    void operator+=(f32 value)
    {
        this->Add(value);
    }

    void operator+=(i32 value)
    {
        this->Add((f32)value);
    }

    u32 operator==(i32 value) { return this->current == value; }
    u32 operator!=(i32 value) { return this->current != value; }
    u32 operator<(i32 value) { return this->current < value; }
    u32 operator<=(i32 value) { return this->current <= value; }
    u32 operator>(i32 value) { return this->current > value; }
    u32 operator>=(i32 value) { return this->current >= value; }
};

typedef char ZunTimerSizeIsC[(sizeof(ZunTimer) == 0xc) ? 1 : -1];

} // namespace th095
