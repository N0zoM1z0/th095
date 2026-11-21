#pragma once

#include "inttypes.hpp"

#include <stddef.h>

namespace th095
{

struct EnemyAnmScripts
{
    i16 idleInitial;
    i16 idleFromLeft;
    i16 idleFromRight;
    i16 moveLeft;
    i16 moveRight;
    i16 special;
};
typedef char EnemyAnmScriptsSizeIsC[
    (sizeof(EnemyAnmScripts) == 0xc) ? 1 : -1];
typedef char EnemyAnmScriptsSpecialAtA[
    (offsetof(EnemyAnmScripts, special) == 0xa) ? 1 : -1];

} // namespace th095
