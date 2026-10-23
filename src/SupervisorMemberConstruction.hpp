#pragma once

#include "GameConfiguration.hpp"

namespace th095
{

// The target constructs Supervisor's configuration before the parent body's
// 0x7BC reset. Other GameConfiguration objects include persistent images and
// task snapshots, so their default construction must not acquire this policy.
// This existing fieldless construction adapter is shared by all Supervisor
// clients; GameConfiguration remains the one owner of the 0xC8 data layout.
struct GameConfigurationConstructionAdapter : GameConfiguration
{
    // Preserve assignment from a loaded base configuration image without a
    // second copy helper or a conversion constructor that initializes defaults.
    using GameConfiguration::operator=;

    GameConfigurationConstructionAdapter()
    {
        Initialize();
    }
};

typedef char GameConfigurationConstructionAdapterSizeIsC8[
    (sizeof(GameConfigurationConstructionAdapter) == 0xc8) ? 1 : -1];

} // namespace th095
