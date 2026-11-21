#pragma once

namespace th095
{

struct PhotoEnemyView;
struct EclRawInstruction;

typedef void (__fastcall *EclExtendedCallback)(
    PhotoEnemyView *enemy, EclRawInstruction *instruction);

extern EclExtendedCallback g_EclExtendedCallbacks[22];

} // namespace th095
