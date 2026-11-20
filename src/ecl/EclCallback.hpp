#pragma once

namespace th095
{

struct Enemy;
struct EclRawInstruction;

typedef void (__fastcall *EclExtendedCallback)(
    Enemy *enemy, EclRawInstruction *instruction);

} // namespace th095
