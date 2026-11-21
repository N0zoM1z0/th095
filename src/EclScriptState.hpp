#pragma once

#include "EclCallParameters.hpp"

namespace th095
{

struct EclScriptState
{
    i32 intVariables[8];
    f32 floatVariables[8];
    i32 extraIntVariables[4];
    f32 extraFloatVariables[4];
    EclCallParameters callParameters;
};

typedef char EclScriptStateSizeIs80[
    (sizeof(EclScriptState) == 0x80) ? 1 : -1];
typedef char EclScriptStateFloatVariablesAt20[
    (offsetof(EclScriptState, floatVariables) == 0x20) ? 1 : -1];
typedef char EclScriptStateExtraIntVariablesAt40[
    (offsetof(EclScriptState, extraIntVariables) == 0x40) ? 1 : -1];
typedef char EclScriptStateExtraFloatVariablesAt50[
    (offsetof(EclScriptState, extraFloatVariables) == 0x50) ? 1 : -1];
typedef char EclScriptStateCallParametersAt60[
    (offsetof(EclScriptState, callParameters) == 0x60) ? 1 : -1];

} // namespace th095
