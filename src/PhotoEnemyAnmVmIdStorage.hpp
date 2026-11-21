#pragma once

#include "inttypes.hpp"

namespace th095
{

// The target constructor treats these embedded VM identifiers as POD storage.
// ECL copies the ANM spawn result into the embedded slot as a whole.
struct PhotoEnemyAnmVmIdStorage
{
    i32 value;
};

} // namespace th095
