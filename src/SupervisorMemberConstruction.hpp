#pragma once

#include "GameConfiguration.hpp"

namespace th095
{

// Supervisor and PhotoGameTask initialize their configuration members before
// clearing the parent allocation. Persistent images use GameConfiguration's
// plain 0xC8 layout directly.
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
